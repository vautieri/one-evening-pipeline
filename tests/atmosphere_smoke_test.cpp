/*
 * atmosphere_smoke_test.cpp
 *
 * Quick standalone verification that the ISA 1976 extract produces correct
 * standard-day values. Compile and run without any test framework.
 *
 *   g++ -std=c++17 -I../src/models atmosphere_smoke_test.cpp
 *       ../src/models/FGStandardAtmosphere1976.cpp -o atmo_test && ./atmo_test
 */

#include "FGStandardAtmosphere1976.h"
#include <cstdio>
#include <cmath>

using namespace JSBSimExtract;

static int failures = 0;

static void check(const char* label, double got, double expected, double tol_pct)
{
    double err = std::abs(got - expected) / std::abs(expected) * 100.0;
    bool ok = err < tol_pct;
    if (!ok) ++failures;
    std::printf("  %-45s  got=%.6f  exp=%.6f  err=%.4f%%  %s\n",
                label, got, expected, err, ok ? "OK" : "FAIL");
}

int main()
{
    FGStandardAtmosphere1976 atmo;

    std::printf("=== ISA 1976 Standard Atmosphere - Smoke Test ===\n\n");

    // ── Sea level (h = 0 ft) ─────────────────────────────────────────────────
    std::printf("Sea Level (h = 0 ft):\n");
    check("Temperature [R]",     atmo.GetStdTemperature(0.0),   518.67,    0.01);
    check("Pressure [psf]",      atmo.GetStdPressure(0.0),      2116.228,  0.01);
    check("Density [slug/ft3]",  atmo.GetStdDensity(0.0),       0.002377,  0.1);

    // Speed of sound at SL: sqrt(1.4 * 1716.49 * 518.67) = ~1116.45 ft/s
    double a_sl = std::sqrt(FGAtmosphereBase::SHRatio *
                            (FGAtmosphereBase::Rstar / FGAtmosphereBase::Mair) *
                            518.67);
    check("Sound speed [ft/s]",  atmo.GetSoundSpeedSL(),        a_sl,      0.01);

    // ── 36,089 ft (tropopause, ~11 km geopotential) ─────────────────────────
    std::printf("\nTropopause (h ~ 36,089 ft geopotential = ~36,151 ft geometric):\n");
    double h_tropo_geom = atmo.GeometricAltitude(36089.2388);
    check("Temperature [R]",    atmo.GetStdTemperature(h_tropo_geom), 389.97,  0.02);

    // ISA tropopause pressure: 22632.1 Pa = 472.68 psf
    double p_tropo = atmo.GetStdPressure(h_tropo_geom);
    check("Pressure [psf]",     p_tropo, 472.68, 0.15);

    // ── 65,617 ft (~20 km geopotential, top of isothermal layer) ────────────
    std::printf("\n20 km geopotential (~65,617 ft):\n");
    double h_20km_geom = atmo.GeometricAltitude(65616.7979);
    check("Temperature [R]",    atmo.GetStdTemperature(h_20km_geom), 389.97, 0.02);

    // ISA 20km pressure: 5474.89 Pa = 114.35 psf
    double p_20km = atmo.GetStdPressure(h_20km_geom);
    check("Pressure [psf]",     p_20km, 114.35, 0.3);

    // ── 10,000 ft ────────────────────────────────────────────────────────────
    std::printf("\n10,000 ft geometric:\n");
    // ISA @ 10,000 ft: T ~ 483.04 R, P ~ 1455.6 psf
    check("Temperature [R]",    atmo.GetStdTemperature(10000.0), 483.04, 0.1);
    check("Pressure [psf]",     atmo.GetStdPressure(10000.0),    1455.6, 0.3);

    // ── Verify Calculate() populates member variables ────────────────────────
    std::printf("\nCalculate(35000) and read back:\n");
    atmo.Calculate(35000.0);
    check("Temperature [R]",    atmo.GetTemperature(), atmo.GetStdTemperature(35000.0), 0.01);
    check("Pressure [psf]",     atmo.GetPressure(),    atmo.GetStdPressure(35000.0),    0.01);
    check("Density [slug/ft3]", atmo.GetDensity(),     atmo.GetStdDensity(35000.0),     0.01);

    // ── Temperature bias ─────────────────────────────────────────────────────
    std::printf("\nTemperature bias (+10 R):\n");
    atmo.SetTemperatureBias(FGAtmosphereBase::eRankine, 10.0);
    atmo.Calculate(0.0);
    check("SL temp with +10R bias", atmo.GetTemperature(), 518.67 + 10.0, 0.01);
    atmo.ResetSLTemperature();

    // ── Geopotential conversion round-trip ───────────────────────────────────
    std::printf("\nGeopotential/geometric round-trip:\n");
    double h_test = 50000.0;
    double gp = atmo.GeopotentialAltitude(h_test);
    double gm = atmo.GeometricAltitude(gp);
    check("Round-trip 50000 ft", gm, h_test, 0.0001);

    // ── Summary ──────────────────────────────────────────────────────────────
    std::printf("\n=== Result: %d failure(s) ===\n", failures);
    return failures;
}
