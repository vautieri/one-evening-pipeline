/*
 * FGStandardAtmosphere1976.cpp
 *
 * Self-contained implementation of the 1976 U.S. Standard Atmosphere model.
 * Extracted from JSBSim (LGPL-2.1+), stripped of framework dependencies.
 *
 * Original authors: Jon Berndt (jon@jsbsim.org), Tony Peden
 * Reference: "U.S. Standard Atmosphere, 1976", NASA TM-X-74335
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 *  ISA 1976 Standard Temperature Profile (geopotential altitude)
 *
 *   Layer   GeoPot Alt (km)   Temp (K)    Lapse (K/km)
 *   ─────   ───────────────   ────────    ────────────
 *     0       0.000           288.15       -6.5
 *     1      11.000           216.65        0.0
 *     2      20.000           216.65       +1.0
 *     3      32.000           228.65       +2.8
 *     4      47.000           270.65        0.0
 *     5      51.000           270.65       -2.8
 *     6      71.000           214.65       -2.0
 *     7      84.852           186.946       --
 *
 *  Stored internally in Rankine / feet (geopotential).
 *
 *  Pressure equations (ISA document eqs. 33a, 33b):
 *
 *    Non-zero lapse rate:
 *      P = P_b * [T_b / (T_b + L_b * dH)]^(g0 / (R * L_b))
 *
 *    Isothermal layer:
 *      P = P_b * exp(-g0 * dH / (R * T_b))
 *
 *    where dH = H - H_b (geopotential altitude difference)
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#include "FGStandardAtmosphere1976.h"
#include <cmath>
#include <algorithm>

namespace JSBSimExtract {

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor: build the ISA temperature table and derived vectors
// ─────────────────────────────────────────────────────────────────────────────

FGStandardAtmosphere1976::FGStandardAtmosphere1976()
    : FGAtmosphereBase(),
      StdSLpressure(StdDaySLpressure),
      StdAtmosTemperatureTable(9)
{
    // ISA 1976 temperature vs. geopotential altitude
    //                        GeoPot Alt (ft)    Temp (R)
    //                        ===============    ========
    StdAtmosTemperatureTable <<      0.0000 << 518.67    //  0.000 km
                             <<  36089.2388 << 389.97    // 11.000 km
                             <<  65616.7979 << 389.97    // 20.000 km
                             << 104986.8766 << 411.57    // 32.000 km
                             << 154199.4751 << 487.17    // 47.000 km
                             << 167322.8346 << 487.17    // 51.000 km
                             << 232939.6325 << 386.37    // 71.000 km
                             << 278385.8268 << 336.5028  // 84.852 km
                             << 298556.4304 << 336.5028; // 91.000 km (high-alt regime start)

    unsigned int numRows = StdAtmosTemperatureTable.GetNumRows();

    // Compute standard lapse rates from the table
    CalculateLapseRates();
    StdLapseRates = LapseRates;

    // The gradient fadeout altitude is the highest entry in the table
    GradientFadeoutAltitude = StdAtmosTemperatureTable(numRows, 0);

    // Compute standard pressure breakpoints
    PressureBreakpoints.resize(numRows);
    CalculatePressureBreakpoints(StdSLpressure);
    StdPressureBreakpoints = PressureBreakpoints;

    // Standard sea-level values
    StdSLtemperature = StdAtmosTemperatureTable(1, 1);   // 518.67 R
    StdSLdensity     = StdSLpressure / (Rdry * StdSLtemperature);
    StdSLsoundspeed  = std::sqrt(SHRatio * Rdry * StdSLtemperature);

    CalculateStdDensityBreakpoints();

    // Set the base-class sea-level fields
    SLtemperature = StdSLtemperature;
    SLpressure    = StdSLpressure;
    SLdensity     = StdSLdensity;
    SLsoundspeed  = StdSLsoundspeed;
    Reng          = Rdry;

    // Initialize current-conditions to sea level
    Calculate(0.0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  InitModel: reset to pure ISA
// ─────────────────────────────────────────────────────────────────────────────

void FGStandardAtmosphere1976::InitModel()
{
    GradientFadeoutAltitude =
        StdAtmosTemperatureTable(StdAtmosTemperatureTable.GetNumRows(), 0);

    TemperatureBias           = 0.0;
    TemperatureDeltaGradient  = 0.0;
    LapseRates                = StdLapseRates;
    PressureBreakpoints       = StdPressureBreakpoints;

    SLpressure    = StdSLpressure;
    SLtemperature = StdSLtemperature;
    SLdensity     = StdSLdensity;
    SLsoundspeed  = StdSLsoundspeed;
    Reng          = Rdry;

    Calculate(0.0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Calculate: master computation at a given geometric altitude
// ─────────────────────────────────────────────────────────────────────────────

void FGStandardAtmosphere1976::Calculate(double altitude)
{
    // Delegate to base which calls GetTemperature / GetPressure and derives
    // density, sound speed, viscosity, etc.
    FGAtmosphereBase::Calculate(altitude);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Temperature
// ═══════════════════════════════════════════════════════════════════════════════

double FGStandardAtmosphere1976::GetTemperature(double altitude) const
{
    double GeoPotAlt = GeopotentialAltitude(altitude);
    double T;

    if (GeoPotAlt >= 0.0) {
        T = StdAtmosTemperatureTable.GetValue(GeoPotAlt);

        if (GeoPotAlt <= GradientFadeoutAltitude)
            T -= TemperatureDeltaGradient * GeoPotAlt;
    } else {
        // Below sea level: extrapolate using first layer's lapse rate
        T = StdAtmosTemperatureTable.GetValue(0.0) + GeoPotAlt * LapseRates[0];
    }

    T += TemperatureBias;

    if (GeoPotAlt <= GradientFadeoutAltitude)
        T += TemperatureDeltaGradient * GradientFadeoutAltitude;

    return T;
}

double FGStandardAtmosphere1976::GetStdTemperature(double altitude) const
{
    double GeoPotAlt = GeopotentialAltitude(altitude);

    if (GeoPotAlt >= 0.0)
        return StdAtmosTemperatureTable.GetValue(GeoPotAlt);
    else
        return StdAtmosTemperatureTable.GetValue(0.0) + GeoPotAlt * StdLapseRates[0];
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Pressure  (ISA equations 33a and 33b)
// ═══════════════════════════════════════════════════════════════════════════════

double FGStandardAtmosphere1976::GetPressure(double altitude) const
{
    double GeoPotAlt = GeopotentialAltitude(altitude);

    // Find the base layer
    double BaseAlt = StdAtmosTemperatureTable(1, 0);
    unsigned int numRows = StdAtmosTemperatureTable.GetNumRows();
    unsigned int b;

    for (b = 0; b < numRows - 2; ++b) {
        double testAlt = StdAtmosTemperatureTable(b + 2, 0);
        if (GeoPotAlt < testAlt) break;
        BaseAlt = testAlt;
    }

    double Tmb    = GetTemperature(GeometricAltitude(BaseAlt));
    double deltaH = GeoPotAlt - BaseAlt;
    double Lmb    = LapseRates[b];

    if (Lmb != 0.0) {
        // Equation 33a: non-isothermal layer
        double Exp    = g0 / (Rdry * Lmb);
        double factor = Tmb / (Tmb + Lmb * deltaH);
        return PressureBreakpoints[b] * std::pow(factor, Exp);
    } else {
        // Equation 33b: isothermal layer
        return PressureBreakpoints[b] * std::exp(-g0 * deltaH / (Rdry * Tmb));
    }
}

double FGStandardAtmosphere1976::GetStdPressure(double altitude) const
{
    double GeoPotAlt = GeopotentialAltitude(altitude);

    double BaseAlt = StdAtmosTemperatureTable(1, 0);
    unsigned int numRows = StdAtmosTemperatureTable.GetNumRows();
    unsigned int b;

    for (b = 0; b < numRows - 2; ++b) {
        double testAlt = StdAtmosTemperatureTable(b + 2, 0);
        if (GeoPotAlt < testAlt) break;
        BaseAlt = testAlt;
    }

    double Tmb    = GetStdTemperature(GeometricAltitude(BaseAlt));
    double deltaH = GeoPotAlt - BaseAlt;
    double Lmb    = StdLapseRates[b];

    if (Lmb != 0.0) {
        double Exp    = g0 / (Rdry * Lmb);
        double factor = Tmb / (Tmb + Lmb * deltaH);
        return StdPressureBreakpoints[b] * std::pow(factor, Exp);
    } else {
        return StdPressureBreakpoints[b] * std::exp(-g0 * deltaH / (Rdry * Tmb));
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Density
// ═══════════════════════════════════════════════════════════════════════════════

double FGStandardAtmosphere1976::GetStdDensity(double altitude) const
{
    return GetStdPressure(altitude) / (Rdry * GetStdTemperature(altitude));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Temperature setters (bias, gradient, explicit)
// ═══════════════════════════════════════════════════════════════════════════════

void FGStandardAtmosphere1976::SetTemperatureSL(double t, eTemperature unit)
{
    SetTemperature(t, 0.0, unit);
}

void FGStandardAtmosphere1976::SetTemperature(double t, double h, eTemperature unit)
{
    double targetTemp = ConvertToRankine(t, unit);
    double GeoPotAlt  = GeopotentialAltitude(h);
    double bias       = targetTemp - GetStdTemperature(h);

    if (GeoPotAlt <= GradientFadeoutAltitude)
        bias -= TemperatureDeltaGradient * (GradientFadeoutAltitude - GeoPotAlt);

    SetTemperatureBias(eRankine, bias);
    CalculatePressureBreakpoints(SLpressure);

    SLtemperature = GetTemperature(0.0);
    CalculateSLSoundSpeedAndDensity();
}

void FGStandardAtmosphere1976::SetTemperatureBias(eTemperature unit, double t)
{
    if (unit == eCelsius || unit == eKelvin)
        t *= 1.80;

    TemperatureBias = t;

    // Clamp so that minimum atmosphere temp stays above absolute zero
    constexpr double minUniverseTemperature = KelvinToRankine(1.0);
    double minStdTemp = StdAtmosTemperatureTable.GetMinValue();
    if (minStdTemp + TemperatureBias < minUniverseTemperature)
        TemperatureBias = minUniverseTemperature - minStdTemp;

    CalculatePressureBreakpoints(SLpressure);

    SLtemperature = GetTemperature(0.0);
    CalculateSLSoundSpeedAndDensity();
}

void FGStandardAtmosphere1976::SetSLTemperatureGradedDelta(eTemperature unit, double deltemp)
{
    SetTemperatureGradedDelta(deltemp, 0.0, unit);
}

void FGStandardAtmosphere1976::SetTemperatureGradedDelta(double deltemp, double h, eTemperature unit)
{
    if (unit == eCelsius || unit == eKelvin)
        deltemp *= 1.80;

    // Clamp the delta
    double minStdTemp  = StdAtmosTemperatureTable.GetMinValue();
    double minDelta    = minStdTemp - StdSLtemperature;
    if (deltemp <= minDelta)
        deltemp = minDelta;

    TemperatureDeltaGradient = deltemp / (GradientFadeoutAltitude - GeopotentialAltitude(h));

    CalculateLapseRates();
    CalculatePressureBreakpoints(SLpressure);

    SLtemperature = GetTemperature(0.0);
    CalculateSLSoundSpeedAndDensity();
}

void FGStandardAtmosphere1976::ResetSLTemperature()
{
    TemperatureBias = TemperatureDeltaGradient = 0.0;
    CalculateLapseRates();
    CalculatePressureBreakpoints(SLpressure);

    SLtemperature = StdSLtemperature;
    CalculateSLSoundSpeedAndDensity();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Pressure setters
// ═══════════════════════════════════════════════════════════════════════════════

void FGStandardAtmosphere1976::SetPressureSL(ePressure unit, double pressure)
{
    double p = ConvertToPSF(pressure, unit);
    SLpressure = ValidatePressure(p, "Sea Level pressure");
    CalculateSLDensity();
    CalculatePressureBreakpoints(SLpressure);
}

void FGStandardAtmosphere1976::ResetSLPressure()
{
    SLpressure = StdSLpressure;
    CalculateSLDensity();
    CalculatePressureBreakpoints(StdSLpressure);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Internal: lapse rates from temperature table
// ═══════════════════════════════════════════════════════════════════════════════

void FGStandardAtmosphere1976::CalculateLapseRates()
{
    unsigned int numRows = StdAtmosTemperatureTable.GetNumRows();
    LapseRates.clear();

    for (unsigned int bh = 0; bh < numRows - 1; ++bh) {
        double t0 = StdAtmosTemperatureTable(bh + 1, 1);
        double t1 = StdAtmosTemperatureTable(bh + 2, 1);
        double h0 = StdAtmosTemperatureTable(bh + 1, 0);
        double h1 = StdAtmosTemperatureTable(bh + 2, 0);
        LapseRates.push_back((t1 - t0) / (h1 - h0) - TemperatureDeltaGradient);
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Internal: pressure breakpoints (ISA eqs 33a/33b applied at layer boundaries)
// ═══════════════════════════════════════════════════════════════════════════════

void FGStandardAtmosphere1976::CalculatePressureBreakpoints(double SLpress)
{
    PressureBreakpoints[0] = SLpress;

    for (unsigned int b = 0; b < PressureBreakpoints.size() - 1; ++b) {
        double BaseTemp = StdAtmosTemperatureTable(b + 1, 1);
        double BaseAlt  = StdAtmosTemperatureTable(b + 1, 0);
        double UpperAlt = StdAtmosTemperatureTable(b + 2, 0);
        double deltaH   = UpperAlt - BaseAlt;
        double Tmb      = BaseTemp
                          + TemperatureBias
                          + (GradientFadeoutAltitude - BaseAlt) * TemperatureDeltaGradient;

        if (LapseRates[b] != 0.0) {
            double Lmb    = LapseRates[b];
            double Exp    = g0 / (Rdry * Lmb);
            double factor = Tmb / (Tmb + Lmb * deltaH);
            PressureBreakpoints[b + 1] = PressureBreakpoints[b] * std::pow(factor, Exp);
        } else {
            PressureBreakpoints[b + 1] = PressureBreakpoints[b]
                                         * std::exp(-g0 * deltaH / (Rdry * Tmb));
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Internal: standard density breakpoints
// ═══════════════════════════════════════════════════════════════════════════════

void FGStandardAtmosphere1976::CalculateStdDensityBreakpoints()
{
    StdDensityBreakpoints.clear();
    for (unsigned int i = 0; i < StdPressureBreakpoints.size(); ++i) {
        StdDensityBreakpoints.push_back(
            StdPressureBreakpoints[i] / (Rdry * StdAtmosTemperatureTable(i + 1, 1))
        );
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Density altitude (inverse of density-altitude relationship)
// ═══════════════════════════════════════════════════════════════════════════════

double FGStandardAtmosphere1976::CalculateDensityAltitude(double density, double geometricAlt)
{
    // Find the layer in the standard density profile
    unsigned int b = 0;
    for (; b < StdDensityBreakpoints.size() - 2; ++b) {
        if (density >= StdDensityBreakpoints[b + 1])
            break;
    }

    double Tmb = StdAtmosTemperatureTable(b + 1, 1);
    double Hb  = StdAtmosTemperatureTable(b + 1, 0);
    double Lmb = StdLapseRates[b];
    double pb  = StdDensityBreakpoints[b];

    double density_altitude;

    if (Lmb != 0.0) {
        double Exp = -1.0 / (1.0 + g0 / (Rdry * Lmb));
        density_altitude = Hb + (Tmb / Lmb) * (std::pow(density / pb, Exp) - 1.0);
    } else {
        double Factor = -Rdry * Tmb / g0;
        density_altitude = Hb + Factor * std::log(density / pb);
    }

    return GeometricAltitude(density_altitude);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Pressure altitude (inverse of pressure-altitude relationship)
// ═══════════════════════════════════════════════════════════════════════════════

double FGStandardAtmosphere1976::CalculatePressureAltitude(double pressure, double geometricAlt)
{
    // Find the layer in the standard pressure profile
    unsigned int b = 0;
    for (; b < StdPressureBreakpoints.size() - 2; ++b) {
        if (pressure >= StdPressureBreakpoints[b + 1])
            break;
    }

    double Tmb = StdAtmosTemperatureTable(b + 1, 1);
    double Hb  = StdAtmosTemperatureTable(b + 1, 0);
    double Lmb = StdLapseRates[b];
    double Pb  = StdPressureBreakpoints[b];

    double pressure_altitude;

    if (Lmb != 0.0) {
        // Equation 33a solved for H
        double Exp = -Rdry * Lmb / g0;
        pressure_altitude = Hb + (Tmb / Lmb) * (std::pow(pressure / Pb, Exp) - 1.0);
    } else {
        // Equation 33b solved for H
        double Factor = -Rdry * Tmb / g0;
        pressure_altitude = Hb + Factor * std::log(pressure / Pb);
    }

    return GeometricAltitude(pressure_altitude);
}

} // namespace JSBSimExtract
