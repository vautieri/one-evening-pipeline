/*
 * fuzz_atmosphere.cpp — libFuzzer target for FGStandardAtmosphere1976
 *
 * Exercises the ISA atmosphere model with fuzzer-generated altitudes and
 * temperature/pressure perturbations. Catches numerical instabilities,
 * division-by-zero in density calculations, and edge cases at layer
 * boundaries.
 *
 * Minimum input: 24 bytes (3 doubles: altitude, temp bias, pressure)
 */

#include "FGStandardAtmosphere1976.h"
#include <cstdint>
#include <cstring>
#include <cmath>

using namespace JSBSimExtract;

static double sanitize(double v) {
    if (std::isnan(v) || std::isinf(v)) return 0.0;
    if (v > 1e8) return 1e8;
    if (v < -1e4) return -1e4;
    return v;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;  // Need at least 3 doubles

    double vals[3];
    std::memcpy(vals, Data, sizeof(vals));

    double altitude  = sanitize(vals[0]);
    double tempBias  = sanitize(vals[1]);
    double pressure  = sanitize(vals[2]);

    FGStandardAtmosphere1976 atm;

    // Full atmosphere calculation at the fuzzed altitude
    atm.Calculate(altitude);

    // Read all properties
    volatile double temp = atm.GetTemperature();
    volatile double pres = atm.GetPressure();
    volatile double dens = atm.GetDensity();
    volatile double ss   = atm.GetSoundSpeed();
    volatile double visc = atm.GetAbsoluteViscosity();
    volatile double kvisc = atm.GetKinematicViscosity();

    // Temperature at specific altitude
    volatile double tempAlt = atm.GetTemperature(altitude);
    volatile double stdTemp = atm.GetStdTemperature(altitude);

    // Pressure at specific altitude
    volatile double presAlt = atm.GetPressure(altitude);
    volatile double stdPres = atm.GetStdPressure(altitude);

    // Density at specific altitude
    volatile double densAlt = atm.GetDensity(altitude);
    volatile double stdDens = atm.GetStdDensity(altitude);

    // Sound speed at altitude
    volatile double ssAlt = atm.GetSoundSpeed(altitude);

    // Ratios
    volatile double tempRatio = atm.GetTemperatureRatio();
    volatile double presRatio = atm.GetPressureRatio();
    volatile double densRatio = atm.GetDensityRatio();
    volatile double ssRatio   = atm.GetSoundSpeedRatio();

    // Sea-level getters
    volatile double tempSL = atm.GetTemperatureSL();
    volatile double presSL = atm.GetPressureSL();
    volatile double densSL = atm.GetDensitySL();
    volatile double ssSL   = atm.GetSoundSpeedSL();

    // Altitude conversions
    volatile double geopot = atm.GeopotentialAltitude(altitude);
    volatile double geomet = atm.GeometricAltitude(altitude);

    // Temperature bias manipulation
    if (std::abs(tempBias) < 200.0) {
        atm.SetTemperatureBias(FGAtmosphereBase::eRankine, tempBias);
        atm.Calculate(altitude);
        volatile double biasedTemp = atm.GetTemperature();
        (void)biasedTemp;
    }

    // Graded delta
    if (std::abs(tempBias) < 100.0) {
        atm.ResetSLTemperature();
        atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, tempBias);
        atm.Calculate(altitude);
        volatile double gradedTemp = atm.GetTemperature();
        (void)gradedTemp;
    }

    // Pressure SL manipulation
    if (pressure > 100.0 && pressure < 1e6) {
        atm.SetPressureSL(FGAtmosphereBase::ePSF, pressure);
        atm.Calculate(altitude);
        volatile double newPres = atm.GetPressure();
        (void)newPres;
    }

    // Unit conversions with various inputs
    volatile double toR1 = atm.ConvertToRankine(tempBias, FGAtmosphereBase::eFahrenheit);
    volatile double toR2 = atm.ConvertToRankine(tempBias, FGAtmosphereBase::eCelsius);
    volatile double toR3 = atm.ConvertToRankine(tempBias, FGAtmosphereBase::eKelvin);
    volatile double fromR1 = atm.ConvertFromRankine(500.0, FGAtmosphereBase::eFahrenheit);
    volatile double fromR2 = atm.ConvertFromRankine(500.0, FGAtmosphereBase::eCelsius);

    volatile double toP1 = atm.ConvertToPSF(pressure, FGAtmosphereBase::eMillibars);
    volatile double toP2 = atm.ConvertToPSF(pressure, FGAtmosphereBase::ePascals);
    volatile double toP3 = atm.ConvertToPSF(pressure, FGAtmosphereBase::eInchesHg);
    volatile double fromP1 = atm.ConvertFromPSF(2116.0, FGAtmosphereBase::eMillibars);
    volatile double fromP2 = atm.ConvertFromPSF(2116.0, FGAtmosphereBase::eInchesHg);

    // Reset and reinitialize
    atm.ResetSLTemperature();
    atm.ResetSLPressure();
    atm.InitModel();
    atm.Calculate(0.0);

    (void)temp; (void)pres; (void)dens; (void)ss; (void)visc; (void)kvisc;
    (void)tempAlt; (void)stdTemp; (void)presAlt; (void)stdPres;
    (void)densAlt; (void)stdDens; (void)ssAlt;
    (void)tempRatio; (void)presRatio; (void)densRatio; (void)ssRatio;
    (void)tempSL; (void)presSL; (void)densSL; (void)ssSL;
    (void)geopot; (void)geomet;
    (void)toR1; (void)toR2; (void)toR3; (void)fromR1; (void)fromR2;
    (void)toP1; (void)toP2; (void)toP3; (void)fromP1; (void)fromP2;

    return 0;
}
