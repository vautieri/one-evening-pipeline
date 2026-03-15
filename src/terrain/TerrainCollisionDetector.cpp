/*
 * TerrainCollisionDetector.cpp
 *
 * Implementation of CFIT detection logic for the SynthFlight terrain engine.
 * Implements REQ-SF-0402: Terrain Collision Detection / CFIT Prevention.
 */

#include "TerrainCollisionDetector.h"
#include "TerrainGrid.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace SynthFlight {

// ═══════════════════════════════════════════════════════════════════════════════
//  Constructor
// ═══════════════════════════════════════════════════════════════════════════════

TerrainCollisionDetector::TerrainCollisionDetector(const TerrainGrid& terrain)
    : terrain_(terrain),
      config_()
{}

// ═══════════════════════════════════════════════════════════════════════════════
//  Configuration
// ═══════════════════════════════════════════════════════════════════════════════

void TerrainCollisionDetector::SetConfig(const CFITConfig& config)
{
    config_ = config;

    // Enforce REQ-SF-0402 constraints on the incoming configuration.
    config_.clearanceThresholdFt = std::clamp(config_.clearanceThresholdFt,
                                              kMinClearanceThresholdFt,
                                              kMaxClearanceThresholdFt);
    config_.lookaheadTimeSec     = std::max(config_.lookaheadTimeSec,
                                            kMinLookaheadTimeSec);
}

CFITConfig TerrainCollisionDetector::GetConfig() const
{
    return config_;
}

void TerrainCollisionDetector::SetClearanceThreshold(double thresholdFt)
{
    config_.clearanceThresholdFt = std::clamp(thresholdFt,
                                              kMinClearanceThresholdFt,
                                              kMaxClearanceThresholdFt);
}

double TerrainCollisionDetector::GetClearanceThreshold() const
{
    return config_.clearanceThresholdFt;
}

void TerrainCollisionDetector::SetLookaheadTime(double timeSec)
{
    config_.lookaheadTimeSec = std::max(timeSec, kMinLookaheadTimeSec);
}

double TerrainCollisionDetector::GetLookaheadTime() const
{
    return config_.lookaheadTimeSec;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Trajectory prediction (static)
// ═══════════════════════════════════════════════════════════════════════════════

void TerrainCollisionDetector::PredictPosition(const AircraftState& state,
                                               double timeSec,
                                               double& lat,
                                               double& lon,
                                               double& altMSL)
{
    // Convert angles to radians.
    const double trackRad   = state.trackAngleDeg * kDegToRad;
    const double fpaRad     = state.flightPathAngleDeg * kDegToRad;
    const double latRad     = state.latitude * kDegToRad;

    // Ground speed in feet per second.
    const double gsFPS      = KnotsToFPS(state.groundSpeedKts);

    // Horizontal distance travelled [ft].
    const double horizDistFt = gsFPS * timeSec * std::cos(fpaRad);

    // Altitude change [ft].
    const double altChangeFt = gsFPS * timeSec * std::sin(fpaRad);

    // Metres per degree of longitude at the current latitude.
    const double metersPerDegLon = kMetersPerDegLat * std::cos(latRad);

    // Convert horizontal distance from feet to metres, then to degrees.
    const double horizDistM = horizDistFt / kFeetPerMeter;  // same as * kFeetToMetersRatio

    // Latitude change:  northward component in degrees.
    const double deltaLat = (horizDistM * std::cos(trackRad)) / kMetersPerDegLat;

    // Longitude change: eastward component in degrees.
    const double deltaLon = (metersPerDegLon > 0.0)
                            ? (horizDistM * std::sin(trackRad)) / metersPerDegLon
                            : 0.0;

    lat    = state.latitude  + deltaLat;
    lon    = state.longitude + deltaLon;
    altMSL = state.altitudeMSL + altChangeFt;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  CFIT evaluation
// ═══════════════════════════════════════════════════════════════════════════════

CFITResult TerrainCollisionDetector::Evaluate(const AircraftState& state) const
{
    CFITResult result{};
    result.alertLevel        = CFITAlertLevel::eNone;
    result.timeToImpactSec   = kNoTimeToImpact;
    result.minimumClearanceFt = std::numeric_limits<double>::max();
    result.terrainElevationFt = 0.0;
    result.distanceToClosestFt = std::numeric_limits<double>::max();
    result.closestLat        = state.latitude;
    result.closestLon        = state.longitude;

    // Determine the number of trajectory sample steps.
    const double totalTime = config_.lookaheadTimeSec;
    const double dt        = config_.stepTimeSec;

    const size_t numSteps  = std::min(
        static_cast<size_t>(std::ceil(totalTime / dt)),
        config_.maxSteps);

    // Caution clearance = clearance threshold * caution multiplier.
    const double cautionClearanceFt = config_.clearanceThresholdFt
                                      * config_.cautionMultiplier;

    bool impactDetected = false;

    for (size_t i = 0; i <= numSteps; ++i) {
        const double t = static_cast<double>(i) * dt;

        // Predict aircraft position at time t.
        double predLat   = 0.0;
        double predLon   = 0.0;
        double predAltMSL = 0.0;
        PredictPosition(state, t, predLat, predLon, predAltMSL);

        // Query terrain elevation (metres) and convert to feet.
        const double terrainElevM  = terrain_.GetElevation(predLat, predLon);
        const double terrainElevFt = MetersToFeet(terrainElevM);

        // Compute clearance.
        const double clearanceFt = predAltMSL - terrainElevFt;

        // Slant distance approximation: horizontal distance + vertical clearance.
        // Use simple hypotenuse of horizontal ground distance and altitude diff.
        const double horizDistFt = KnotsToFPS(state.groundSpeedKts) * t
                                   * std::cos(state.flightPathAngleDeg * kDegToRad);
        const double slantDistFt = std::sqrt(horizDistFt * horizDistFt
                                             + clearanceFt * clearanceFt);

        // Track the point with the minimum clearance.
        if (clearanceFt < result.minimumClearanceFt) {
            result.minimumClearanceFt  = clearanceFt;
            result.terrainElevationFt  = terrainElevFt;
            result.distanceToClosestFt = slantDistFt;
            result.closestLat          = predLat;
            result.closestLon          = predLon;
        }

        // Check for terrain intersection (WARNING level).
        if (clearanceFt <= 0.0 && !impactDetected) {
            result.alertLevel      = CFITAlertLevel::eWarning;
            result.timeToImpactSec = t;
            impactDetected         = true;
            // Continue scanning to find the absolute minimum clearance.
        }
    }

    // If no impact was detected, check for caution-level alert.
    if (!impactDetected) {
        if (result.minimumClearanceFt < cautionClearanceFt) {
            result.alertLevel = CFITAlertLevel::eCaution;
        }
    }

    return result;
}

} // namespace SynthFlight
