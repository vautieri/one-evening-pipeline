/*
 * FGAtmosphereBase.h
 *
 * Self-contained extract of the JSBSim FGAtmosphere base class interface.
 * Derived from JSBSim source (LGPL-2.1+), stripped of all JSBSim framework
 * dependencies (FGModel, FGFDMExec, SGPropertyNode, FGLogging, etc.)
 * so this file compiles stand-alone for unit-testing purposes.
 *
 * Original authors: Jon Berndt (jon@jsbsim.org), Tony Peden
 * Extract by: automated tool
 *
 * Reference: U.S. Standard Atmosphere, 1976 (NASA TM-X-74335)
 */

#ifndef FGATMOSPHEREBASE_H
#define FGATMOSPHEREBASE_H

#include <cmath>
#include <string>
#include <stdexcept>

namespace JSBSimExtract {

// ═══════════════════════════════════════════════════════════════════════════════
//  Unit-conversion constants (from FGJSBBase.h)
// ═══════════════════════════════════════════════════════════════════════════════

static constexpr double fttom       = 0.3048;
static constexpr double kgtoslug    = 0.06852168;
static constexpr double psftopa     = 47.88;
static constexpr double slugtolb    = 32.174049;
static constexpr double inchtoft    = 1.0 / 12.0;

// Temperature helpers (constexpr-safe)
static constexpr double KelvinToRankine(double k)      { return k * 1.8; }
static constexpr double CelsiusToRankine(double c)     { return c * 1.8 + 491.67; }
static constexpr double RankineToCelsius(double r)      { return (r - 491.67) / 1.8; }
static constexpr double KelvinToFahrenheit(double k)   { return 1.8 * k - 459.4; }
static constexpr double FahrenheitToCelsius(double f)  { return (f - 32.0) / 1.8; }
static constexpr double CelsiusToFahrenheit(double c)  { return c * 1.8 + 32.0; }
static constexpr double CelsiusToKelvin(double c)      { return c + 273.15; }
static constexpr double KelvinToCelsius(double k)      { return k - 273.15; }

// ═══════════════════════════════════════════════════════════════════════════════
//  FGAtmosphereBase -- abstract base  (maps to JSBSim's FGAtmosphere)
// ═══════════════════════════════════════════════════════════════════════════════

class FGAtmosphereBase {
public:

    /// Temperature unit enum
    enum eTemperature { eNoTempUnit = 0, eFahrenheit, eCelsius, eRankine, eKelvin };

    /// Pressure unit enum
    enum ePressure    { eNoPressUnit = 0, ePSF, eMillibars, ePascals, eInchesHg };

    // ── ISA physical constants ───────────────────────────────────────────────

    /// Universal gas constant  [ft*lbf / (R * mol)]
    ///   = 8.31432 J/(K mol) converted to Imperial
    static constexpr double Rstar = 8.31432 * kgtoslug
                                    / (KelvinToRankine(1.0) * (fttom * fttom));
    //  Note: KelvinToRankine(fttom*fttom) in JSBSim source is equivalent;
    //  the expression is evaluated at compile-time.

    /// Mean molecular weight of dry air  [slug / mol]
    static constexpr double Mair = 28.9645 * kgtoslug / 1000.0;

    /// Sea-level gravitational acceleration for ISA [ft/s^2]
    ///   (at latitude 45 deg)
    static constexpr double g0 = 9.80665 / fttom;

    /// Specific gas constant for dry air  [ft*lbf / (slug * R)]
    static constexpr double Reng0 = Rstar / Mair;

    /// Ratio of specific heats for air (gamma)
    static constexpr double SHRatio = 1.4;

    /// ISA standard-day sea-level temperature [Rankine]
    static constexpr double StdDaySLtemperature = 518.67;

    /// ISA standard-day sea-level pressure [psf]
    static constexpr double StdDaySLpressure = 2116.228;

    /// ISA standard-day sea-level density [slug/ft^3]
    ///   rho_SL = P_SL / (Reng0 * T_SL)
    static constexpr double StdDaySLdensity =
        StdDaySLpressure / (Reng0 * StdDaySLtemperature);

    /// Sutherland constant for viscosity [Rankine]
    static constexpr double SutherlandConstant = 198.72;

    /// Beta constant for Sutherland's law  [slug / (s * ft * R^0.5)]
    static constexpr double Beta = 2.269690E-08;

    // ── Constructor / destructor ─────────────────────────────────────────────

    FGAtmosphereBase()
        : StdDaySLsoundspeed(std::sqrt(SHRatio * Reng0 * StdDaySLtemperature)),
          SLtemperature(StdDaySLtemperature),
          SLpressure(StdDaySLpressure),
          SLdensity(StdDaySLdensity),
          SLsoundspeed(std::sqrt(SHRatio * Reng0 * StdDaySLtemperature)),
          Temperature(StdDaySLtemperature),
          Pressure(StdDaySLpressure),
          Density(StdDaySLdensity),
          Soundspeed(std::sqrt(SHRatio * Reng0 * StdDaySLtemperature)),
          Reng(Reng0)
    {}

    virtual ~FGAtmosphereBase() = default;

    // ── Temperature access ───────────────────────────────────────────────────

    /// Current modeled temperature at last-calculated altitude [R]
    virtual double GetTemperature() const { return Temperature; }

    /// Modeled temperature at a given geometric altitude [R]
    virtual double GetTemperature(double altitude) const = 0;

    /// Sea-level temperature [R]
    virtual double GetTemperatureSL() const { return SLtemperature; }

    /// theta = T / T_SL
    virtual double GetTemperatureRatio() const { return GetTemperature() / SLtemperature; }
    virtual double GetTemperatureRatio(double h) const { return GetTemperature(h) / SLtemperature; }

    // ── Pressure access ──────────────────────────────────────────────────────

    /// Current modeled pressure [psf]
    virtual double GetPressure() const { return Pressure; }

    /// Pressure at a given geometric altitude [psf]
    virtual double GetPressure(double altitude) const = 0;

    /// Sea-level pressure [psf] (with optional unit conversion)
    virtual double GetPressureSL(ePressure to = ePSF) const { return ConvertFromPSF(SLpressure, to); }

    /// delta = P / P_SL
    virtual double GetPressureRatio() const { return Pressure / SLpressure; }

    // ── Density access ───────────────────────────────────────────────────────

    /// Current modeled density [slug/ft^3]
    virtual double GetDensity() const { return Density; }

    /// Density at a given geometric altitude [slug/ft^3]
    virtual double GetDensity(double altitude) const;

    /// Sea-level density [slug/ft^3]
    virtual double GetDensitySL() const { return SLdensity; }

    /// sigma = rho / rho_SL
    virtual double GetDensityRatio() const { return Density / SLdensity; }

    // ── Speed of sound access ────────────────────────────────────────────────

    /// Current speed of sound [ft/s]
    virtual double GetSoundSpeed() const { return Soundspeed; }

    /// Speed of sound at a given geometric altitude [ft/s]
    virtual double GetSoundSpeed(double altitude) const;

    /// Sea-level speed of sound [ft/s]
    virtual double GetSoundSpeedSL() const { return SLsoundspeed; }

    /// a-ratio = a / a_SL
    virtual double GetSoundSpeedRatio() const { return Soundspeed / SLsoundspeed; }

    // ── Viscosity access ─────────────────────────────────────────────────────

    virtual double GetAbsoluteViscosity() const { return Viscosity; }
    virtual double GetKinematicViscosity() const { return KinematicViscosity; }

    // ── Altitude access ──────────────────────────────────────────────────────

    virtual double GetDensityAltitude() const { return DensityAltitude; }
    virtual double GetPressureAltitude() const { return PressureAltitude; }

    // ── Calculate for a given geometric altitude ─────────────────────────────

    /// Main entry: recompute all atmosphere properties at the given altitude.
    virtual void Calculate(double altitude);

    // ── Unit conversion utilities ────────────────────────────────────────────

    double ConvertToRankine(double t, eTemperature unit) const;
    double ConvertFromRankine(double t, eTemperature unit) const;
    double ConvertToPSF(double p, ePressure unit = ePSF) const;
    double ConvertFromPSF(double p, ePressure unit = ePSF) const;

protected:

    const double StdDaySLsoundspeed;

    // Sea-level conditions (modeled, may differ from standard)
    double SLtemperature;
    double SLpressure;
    double SLdensity;
    double SLsoundspeed;

    // Current conditions at last-computed altitude
    double Temperature;
    double Pressure;
    double Density;
    double Soundspeed;
    double PressureAltitude = 0.0;
    double DensityAltitude  = 0.0;
    double Viscosity        = 0.0;
    double KinematicViscosity = 0.0;

    double Reng;   // Effective gas constant (may include humidity effects)

    // Overridable hooks for density/pressure altitude
    virtual double CalculateDensityAltitude(double density, double geometricAlt)
    { return geometricAlt; }
    virtual double CalculatePressureAltitude(double pressure, double geometricAlt)
    { return geometricAlt; }

    /// Validate pressure (minimum: ~1E-15 Pa, outer-space inter-stellar)
    double ValidatePressure(double p, const std::string& msg, bool quiet = false) const;

    /// Validate temperature (minimum: 1 K = 1.8 R, Boomerang Nebula)
    double ValidateTemperature(double t, const std::string& msg, bool quiet = false) const;
};

// ═══════════════════════════════════════════════════════════════════════════════
//  Inline / out-of-class definitions for FGAtmosphereBase
// ═══════════════════════════════════════════════════════════════════════════════

inline double FGAtmosphereBase::GetDensity(double altitude) const
{
    return GetPressure(altitude) / (Reng * GetTemperature(altitude));
}

inline double FGAtmosphereBase::GetSoundSpeed(double altitude) const
{
    return std::sqrt(SHRatio * Reng * GetTemperature(altitude));
}

inline void FGAtmosphereBase::Calculate(double altitude)
{
    double t = GetTemperature(altitude);
    double p = GetPressure(altitude);

    Temperature = ValidateTemperature(t, "", true);
    Pressure    = ValidatePressure(p, "", true);
    Density     = Pressure / (Reng * Temperature);
    Soundspeed  = std::sqrt(SHRatio * Reng * Temperature);

    PressureAltitude = CalculatePressureAltitude(Pressure, altitude);
    DensityAltitude  = CalculateDensityAltitude(Density, altitude);

    // Sutherland's law for viscosity
    Viscosity          = Beta * std::pow(Temperature, 1.5) / (SutherlandConstant + Temperature);
    KinematicViscosity = Viscosity / Density;
}

inline double FGAtmosphereBase::ValidatePressure(double p, const std::string& /*msg*/, bool /*quiet*/) const
{
    const double MinPressure = ConvertToPSF(1E-15, ePascals);
    return (p < MinPressure) ? MinPressure : p;
}

inline double FGAtmosphereBase::ValidateTemperature(double t, const std::string& /*msg*/, bool /*quiet*/) const
{
    constexpr double minT = KelvinToRankine(1.0);  // 1.8 R
    return (t < minT) ? minT : t;
}

// ── Unit conversion implementations ─────────────────────────────────────────

inline double FGAtmosphereBase::ConvertToRankine(double t, eTemperature unit) const
{
    switch (unit) {
        case eFahrenheit: return t + 459.67;
        case eCelsius:    return (t + 273.15) * 1.8;
        case eRankine:    return t;
        case eKelvin:     return t * 1.8;
        default: throw std::invalid_argument("Undefined temperature unit");
    }
}

inline double FGAtmosphereBase::ConvertFromRankine(double t, eTemperature unit) const
{
    switch (unit) {
        case eFahrenheit: return t - 459.67;
        case eCelsius:    return t / 1.8 - 273.15;
        case eRankine:    return t;
        case eKelvin:     return t / 1.8;
        default: throw std::invalid_argument("Undefined temperature unit");
    }
}

inline double FGAtmosphereBase::ConvertToPSF(double p, ePressure unit) const
{
    switch (unit) {
        case ePSF:       return p;
        case eMillibars: return p * 2.08854342;
        case ePascals:   return p * 0.0208854342;
        case eInchesHg:  return p * 70.7180803;
        default: throw std::invalid_argument("Undefined pressure unit");
    }
}

inline double FGAtmosphereBase::ConvertFromPSF(double p, ePressure unit) const
{
    switch (unit) {
        case ePSF:       return p;
        case eMillibars: return p / 2.08854342;
        case ePascals:   return p / 0.0208854342;
        case eInchesHg:  return p / 70.7180803;
        default: throw std::invalid_argument("Undefined pressure unit");
    }
}

} // namespace JSBSimExtract

#endif // FGATMOSPHEREBASE_H
