/*
 * TerrainCollisionDetector.h
 *
 * Terrain collision detection and CFIT (Controlled Flight Into Terrain)
 * prevention for the SynthFlight terrain engine.
 *
 * Implements REQ-SF-0402: Terrain Collision Detection / CFIT Prevention.
 *
 * The detector samples a predicted flight trajectory over a configurable
 * look-ahead window and compares predicted altitude against terrain
 * elevation data provided by a TerrainGrid instance.
 */

#ifndef SYNTHFLIGHT_TERRAIN_COLLISION_DETECTOR_H
#define SYNTHFLIGHT_TERRAIN_COLLISION_DETECTOR_H

#include <cstddef>

namespace SynthFlight {

// ═══════════════════════════════════════════════════════════════════════════════
//  Forward declarations
// ═══════════════════════════════════════════════════════════════════════════════

class TerrainGrid;

// ═══════════════════════════════════════════════════════════════════════════════
//  Aircraft state input
// ═══════════════════════════════════════════════════════════════════════════════

struct AircraftState {
    double latitude;           // degrees WGS-84
    double longitude;          // degrees WGS-84
    double altitudeAGL;        // feet above ground level
    double altitudeMSL;        // feet mean sea level
    double groundSpeedKts;     // knots
    double trackAngleDeg;      // degrees true (0=N, 90=E)
    double flightPathAngleDeg; // degrees (positive = climbing)
    double verticalSpeedFPM;   // feet per minute
};

// ═══════════════════════════════════════════════════════════════════════════════
//  CFIT alert levels
// ═══════════════════════════════════════════════════════════════════════════════

enum class CFITAlertLevel {
    eNone,      // no terrain conflict
    eCaution,   // predicted clearance < 1.5x threshold
    eWarning    // predicted flight path intersects terrain within look-ahead
};

// ═══════════════════════════════════════════════════════════════════════════════
//  CFIT evaluation result
// ═══════════════════════════════════════════════════════════════════════════════

struct CFITResult {
    CFITAlertLevel alertLevel;
    double timeToImpactSec;    // seconds until terrain intersection (-1 if none)
    double minimumClearanceFt; // minimum terrain clearance in look-ahead window
    double terrainElevationFt; // terrain elevation at closest point (MSL)
    double distanceToClosestFt;// slant distance to closest terrain point
    double closestLat;         // latitude of closest terrain point
    double closestLon;         // longitude of closest terrain point
};

// ═══════════════════════════════════════════════════════════════════════════════
//  Detection configuration
// ═══════════════════════════════════════════════════════════════════════════════

struct CFITConfig {
    double lookaheadTimeSec    = 60.0;  // seconds (REQ-SF-0402: >= 60 sec)
    double clearanceThresholdFt = 100.0; // feet AGL (configurable 25-500, default 100)
    double cautionMultiplier   = 1.5;   // caution at 1.5x threshold
    double stepTimeSec         = 1.0;   // time step for trajectory sampling
    size_t maxSteps            = 120;   // max prediction steps
};

// ═══════════════════════════════════════════════════════════════════════════════
//  TerrainCollisionDetector
// ═══════════════════════════════════════════════════════════════════════════════

class TerrainCollisionDetector {
public:

    explicit TerrainCollisionDetector(const TerrainGrid& terrain);

    // ── Configuration ─────────────────────────────────────────────────────────

    /// Replace the full detection configuration.
    void SetConfig(const CFITConfig& config);

    /// Retrieve the current detection configuration.
    CFITConfig GetConfig() const;

    /// Set clearance threshold (REQ-SF-0402: clamped to [25, 500] ft).
    void SetClearanceThreshold(double thresholdFt);

    /// Retrieve the current clearance threshold [ft].
    double GetClearanceThreshold() const;

    /// Set look-ahead time (REQ-SF-0402: minimum 60 sec).
    void SetLookaheadTime(double timeSec);

    /// Retrieve the current look-ahead time [sec].
    double GetLookaheadTime() const;

    // ── Primary evaluation ────────────────────────────────────────────────────

    /// Evaluate CFIT risk for the given aircraft state.
    CFITResult Evaluate(const AircraftState& state) const;

    // ── Trajectory prediction ─────────────────────────────────────────────────

    /// Predict position at time t given current state (simple linear
    /// dead-reckoning extrapolation).
    static void PredictPosition(const AircraftState& state, double timeSec,
                                double& lat, double& lon, double& altMSL);

    // ── Unit-conversion constants and helpers ─────────────────────────────────

    /// Convert feet to metres.
    static constexpr double FeetToMeters(double ft) { return ft * kFeetToMetersRatio; }

    /// Convert metres to feet.
    static constexpr double MetersToFeet(double m) { return m / kFeetToMetersRatio; }

    /// Convert knots to feet per second.
    static constexpr double KnotsToFPS(double kts) { return kts * kKnotsToFPSRatio; }

private:

    // ── Named constants ───────────────────────────────────────────────────────

    /// Feet-to-metres conversion factor.
    static constexpr double kFeetToMetersRatio = 0.3048;

    /// Knots-to-feet-per-second conversion factor.
    static constexpr double kKnotsToFPSRatio = 1.6878098571;

    /// Metres per degree of latitude (WGS-84 mean).
    static constexpr double kMetersPerDegLat = 111320.0;

    /// Feet per metre (inverse of kFeetToMetersRatio).
    static constexpr double kFeetPerMeter = 1.0 / kFeetToMetersRatio;

    /// Degrees to radians conversion factor.
    static constexpr double kDegToRad = 3.14159265358979323846 / 180.0;

    /// REQ-SF-0402: minimum clearance threshold [ft].
    static constexpr double kMinClearanceThresholdFt = 25.0;

    /// REQ-SF-0402: maximum clearance threshold [ft].
    static constexpr double kMaxClearanceThresholdFt = 500.0;

    /// REQ-SF-0402: minimum look-ahead time [sec].
    static constexpr double kMinLookaheadTimeSec = 60.0;

    /// Sentinel value indicating no time-to-impact.
    static constexpr double kNoTimeToImpact = -1.0;

    // ── Data members ──────────────────────────────────────────────────────────

    const TerrainGrid& terrain_;
    CFITConfig config_;
};

} // namespace SynthFlight

#endif // SYNTHFLIGHT_TERRAIN_COLLISION_DETECTOR_H
