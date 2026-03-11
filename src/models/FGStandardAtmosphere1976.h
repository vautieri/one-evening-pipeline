/*
 * FGStandardAtmosphere1976.h
 *
 * Self-contained extract of the JSBSim FGStandardAtmosphere class.
 * Implements the 1976 U.S. Standard Atmosphere (ISA) model with full
 * temperature/pressure/density/sound-speed computation.
 *
 * Derived from JSBSim source (LGPL-2.1+), stripped of all framework
 * dependencies so this file compiles stand-alone for unit testing.
 *
 * Original authors: Jon Berndt (jon@jsbsim.org), Tony Peden
 * Reference: "U.S. Standard Atmosphere, 1976", NASA TM-X-74335
 */

#ifndef FGSTANDARDATMOSPHERE1976_H
#define FGSTANDARDATMOSPHERE1976_H

#include "FGAtmosphereBase.h"
#include <vector>
#include <cmath>
#include <algorithm>

namespace JSBSimExtract {

// ═══════════════════════════════════════════════════════════════════════════════
//  Simple 1-D lookup table with linear interpolation
//  (replaces JSBSim's FGTable for this extract)
// ═══════════════════════════════════════════════════════════════════════════════

class SimpleTable {
public:
    explicit SimpleTable(unsigned int reserveRows = 0)
    { keys_.reserve(reserveRows); values_.reserve(reserveRows); }

    /// Append a (key, value) pair; keys MUST be in ascending order.
    SimpleTable& operator<<(double v) {
        if (pendingKey_) {
            values_.push_back(v);
            pendingKey_ = false;
        } else {
            keys_.push_back(v);
            pendingKey_ = true;
        }
        return *this;
    }

    /// 1-D linear interpolation (clamps at table boundaries).
    double GetValue(double key) const {
        if (keys_.empty()) return 0.0;
        if (key <= keys_.front()) return values_.front();
        if (key >= keys_.back())  return values_.back();

        // Binary search for the interval
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key);
        unsigned int i = static_cast<unsigned int>(it - keys_.begin());
        if (i == 0) i = 1;
        double frac = (key - keys_[i-1]) / (keys_[i] - keys_[i-1]);
        return values_[i-1] + frac * (values_[i] - values_[i-1]);
    }

    /// Access by 1-based row index: (row, 0) = key, (row, 1) = value
    double operator()(unsigned int row, unsigned int col) const {
        if (col == 0) return keys_.at(row - 1);
        else          return values_.at(row - 1);
    }

    unsigned int GetNumRows() const { return static_cast<unsigned int>(keys_.size()); }

    double GetMinValue() const {
        if (values_.empty()) return 0.0;
        return *std::min_element(values_.begin(), values_.end());
    }

private:
    std::vector<double> keys_;
    std::vector<double> values_;
    bool pendingKey_ = false;
};

// ═══════════════════════════════════════════════════════════════════════════════
//  FGStandardAtmosphere1976  (maps to JSBSim's FGStandardAtmosphere)
//
//  ISA 1976 model equations:
//
//    Geopotential altitude:  H = (r_E * h) / (r_E + h)
//        where h = geometric alt, r_E = 6356766 m = 20855531.5 ft
//
//    Temperature profile:    T(H) = T_b + L_b * (H - H_b)
//        (linear within each layer; L_b = lapse rate for layer b)
//
//    Pressure (non-zero lapse):
//        P = P_b * [T_b / (T_b + L_b*(H - H_b))]^(g0 / (R*L_b))
//
//    Pressure (zero lapse / isothermal):
//        P = P_b * exp[-g0*(H - H_b) / (R*T_b)]
//
//    Density:  rho = P / (R * T)
//
//    Speed of sound:  a = sqrt(gamma * R * T)
//
//    Viscosity (Sutherland's law):
//        mu = Beta * T^1.5 / (SutherlandConst + T)
//
// ═══════════════════════════════════════════════════════════════════════════════

class FGStandardAtmosphere1976 : public FGAtmosphereBase {
public:

    FGStandardAtmosphere1976();
    ~FGStandardAtmosphere1976() override = default;

    /// Reinitialize to default ISA conditions
    void InitModel();

    // ── Temperature ──────────────────────────────────────────────────────────

    using FGAtmosphereBase::GetTemperature;   // unhide GetTemperature(void)

    /// Modeled temperature at geometric altitude [R] (includes bias + gradient)
    double GetTemperature(double altitude) const override;

    /// Standard (unbiased) temperature at geometric altitude [R]
    double GetStdTemperature(double altitude) const;

    /// Standard sea-level temperature [R]
    double GetStdTemperatureSL() const { return StdSLtemperature; }

    /// Ratio of std temperature at altitude over std SL temperature
    double GetStdTemperatureRatio(double h) const { return GetStdTemperature(h) / StdSLtemperature; }

    /// Current temperature bias [R]
    double GetTemperatureBias() const { return TemperatureBias; }

    /// Current temperature delta gradient [R/ft geopotential]
    double GetTemperatureDeltaGradient() const { return TemperatureDeltaGradient; }

    /// Set SL temperature (computes bias from standard)
    void SetTemperatureSL(double t, eTemperature unit = eFahrenheit);

    /// Set temperature at a given altitude (computes bias from standard)
    void SetTemperature(double t, double h, eTemperature unit = eFahrenheit);

    /// Set a uniform temperature bias [R] over entire profile
    void SetTemperatureBias(eTemperature unit, double t);

    /// Set a graded temperature delta that linearly fades to zero at ~86 km
    void SetSLTemperatureGradedDelta(eTemperature unit, double deltemp);

    /// Set a graded temperature delta at a specified altitude
    void SetTemperatureGradedDelta(double deltemp, double h, eTemperature unit = eFahrenheit);

    /// Reset temperature to pure ISA standard
    void ResetSLTemperature();

    // ── Pressure ─────────────────────────────────────────────────────────────

    using FGAtmosphereBase::GetPressure;      // unhide GetPressure(void)

    /// Modeled pressure at geometric altitude [psf]
    double GetPressure(double altitude) const override;

    /// Standard (ISA) pressure at geometric altitude [psf]
    double GetStdPressure(double altitude) const;

    /// Set sea-level pressure
    void SetPressureSL(ePressure unit, double pressure);

    /// Reset SL pressure to ISA standard
    void ResetSLPressure();

    // ── Density ──────────────────────────────────────────────────────────────

    using FGAtmosphereBase::GetDensity;       // unhide GetDensity(void)
    using FGAtmosphereBase::GetSoundSpeed;    // unhide GetSoundSpeed(void)

    /// Standard density at geometric altitude [slug/ft^3]
    double GetStdDensity(double altitude) const;

    // ── Calculate ────────────────────────────────────────────────────────────

    /// Compute all atmosphere properties at the given geometric altitude
    void Calculate(double altitude) override;

    // ── Geopotential / geometric conversions ─────────────────────────────────

    /// Convert geometric altitude (ft) to geopotential altitude (ft)
    double GeopotentialAltitude(double geometalt) const
    { return (geometalt * EarthRadius) / (EarthRadius + geometalt); }

    /// Convert geopotential altitude (ft) to geometric altitude (ft)
    double GeometricAltitude(double geopotalt) const
    { return (geopotalt * EarthRadius) / (EarthRadius - geopotalt); }

    // ── Expose internals for testing ─────────────────────────────────────────

    const std::vector<double>& GetLapseRates() const { return LapseRates; }
    const std::vector<double>& GetPressureBreakpoints() const { return PressureBreakpoints; }

protected:

    /// Earth radius for ISA [ft]  (6356766 m)
    static constexpr double EarthRadius = 6356766.0 / fttom;

    /// Specific gas constant for dry air (same as Reng0 but named for clarity
    /// in the ISA formulae; see JSBSim source where Rdry = Rstar/Mair)
    static constexpr double Rdry = Rstar / Mair;

    // Standard sea-level values (ISA, never modified)
    double StdSLtemperature;
    double StdSLpressure;
    double StdSLdensity;
    double StdSLsoundspeed;

    // Bias / gradient
    double TemperatureBias            = 0.0;
    double TemperatureDeltaGradient   = 0.0;
    double GradientFadeoutAltitude    = 0.0;

    // Tables and layer data
    SimpleTable StdAtmosTemperatureTable;

    std::vector<double> LapseRates;
    std::vector<double> PressureBreakpoints;
    std::vector<double> StdPressureBreakpoints;
    std::vector<double> StdDensityBreakpoints;
    std::vector<double> StdLapseRates;

    // ── Internal methods ─────────────────────────────────────────────────────

    void CalculateLapseRates();
    void CalculatePressureBreakpoints(double SLpress);
    void CalculateStdDensityBreakpoints();

    double CalculateDensityAltitude(double density, double geometricAlt) override;
    double CalculatePressureAltitude(double pressure, double geometricAlt) override;

    void CalculateSLDensity() { SLdensity = SLpressure / (Reng * SLtemperature); }

    void CalculateSLSoundSpeedAndDensity() {
        SLsoundspeed = std::sqrt(SHRatio * Reng * SLtemperature);
        CalculateSLDensity();
    }
};

} // namespace JSBSimExtract

#endif // FGSTANDARDATMOSPHERE1976_H
