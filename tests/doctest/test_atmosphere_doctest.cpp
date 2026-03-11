/*==============================================================================
 doctest suite for JSBSim ISA 1976 Standard Atmosphere model
 Framework #3 of 5 — https://github.com/doctest/doctest
==============================================================================*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cmath>
#include "FGStandardAtmosphere1976.h"

using namespace JSBSimExtract;

// Relative tolerance for atmosphere comparisons (~0.1%)
static constexpr double REL_TOL = 1e-3;

// Tighter tolerance for unit conversions and exact identities
static constexpr double TIGHT_TOL = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// Sea-level standard conditions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("SeaLevelConditions") {

TEST_CASE("sea-level standard temperature")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    CHECK(atm.GetTemperature()   == doctest::Approx(518.67).epsilon(REL_TOL));
    CHECK(atm.GetTemperatureSL() == doctest::Approx(518.67).epsilon(REL_TOL));
    CHECK(atm.GetStdTemperature(0.0) == doctest::Approx(518.67).epsilon(REL_TOL));
}

TEST_CASE("sea-level standard pressure")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    CHECK(atm.GetPressure()   == doctest::Approx(2116.228).epsilon(REL_TOL));
    CHECK(atm.GetPressureSL() == doctest::Approx(2116.228).epsilon(REL_TOL));
    CHECK(atm.GetStdPressure(0.0) == doctest::Approx(2116.228).epsilon(REL_TOL));
}

TEST_CASE("sea-level standard density")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    // rho_SL = P_SL / (R * T_SL)
    double expected_density = FGAtmosphereBase::StdDaySLpressure /
        (FGAtmosphereBase::Reng0 * FGAtmosphereBase::StdDaySLtemperature);

    CHECK(atm.GetDensity()   == doctest::Approx(expected_density).epsilon(REL_TOL));
    CHECK(atm.GetDensitySL() == doctest::Approx(expected_density).epsilon(REL_TOL));
}

} // TEST_SUITE SeaLevelConditions

// ═══════════════════════════════════════════════════════════════════════════════
// Temperature at ISA layer boundaries
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("TemperatureProfile") {

TEST_CASE("tropopause temperature at 36089 ft")
{
    FGStandardAtmosphere1976 atm;

    // At the tropopause (geopotential 36089.24 ft), temperature should be 389.97 R
    double T = atm.GetStdTemperature(36089.0);
    CHECK(T == doctest::Approx(389.97).epsilon(REL_TOL));
}

TEST_CASE("isothermal layer between tropopause and 65617 ft")
{
    FGStandardAtmosphere1976 atm;

    // In the isothermal layer, temperature remains constant at 389.97 R
    double T_tropo = atm.GetStdTemperature(36089.0);
    double T_mid   = atm.GetStdTemperature(50000.0);
    double T_top   = atm.GetStdTemperature(65617.0);

    CHECK(T_mid == doctest::Approx(T_tropo).epsilon(REL_TOL));
    CHECK(T_top == doctest::Approx(389.97).epsilon(REL_TOL));
}

TEST_CASE("temperature at upper stratosphere layers")
{
    FGStandardAtmosphere1976 atm;

    // API takes geometric altitude; convert geopotential breakpoints to geometric
    double geom_32km = atm.GeometricAltitude(104986.8766);
    double geom_47km = atm.GeometricAltitude(154199.4751);

    double T_32km = atm.GetStdTemperature(geom_32km);
    CHECK(T_32km == doctest::Approx(411.57).epsilon(REL_TOL));

    double T_47km = atm.GetStdTemperature(geom_47km);
    CHECK(T_47km == doctest::Approx(487.17).epsilon(REL_TOL));
}

} // TEST_SUITE TemperatureProfile

// ═══════════════════════════════════════════════════════════════════════════════
// Pressure and density at key altitudes
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("PressureAndDensity") {

TEST_CASE("pressure decreases monotonically with altitude")
{
    FGStandardAtmosphere1976 atm;

    double P_sl  = atm.GetStdPressure(0.0);
    double P_10k = atm.GetStdPressure(10000.0);
    double P_20k = atm.GetStdPressure(20000.0);
    double P_36k = atm.GetStdPressure(36000.0);
    double P_50k = atm.GetStdPressure(50000.0);

    CHECK(P_sl  > P_10k);
    CHECK(P_10k > P_20k);
    CHECK(P_20k > P_36k);
    CHECK(P_36k > P_50k);

    // At 36000 ft geometric, pressure is in the ~470-480 psf range
    CHECK(P_36k == doctest::Approx(476.1).epsilon(1e-2));
}

TEST_CASE("density decreases with altitude and obeys ideal gas law")
{
    FGStandardAtmosphere1976 atm;

    double rho_sl  = atm.GetStdDensity(0.0);
    double rho_10k = atm.GetStdDensity(10000.0);
    double rho_20k = atm.GetStdDensity(20000.0);
    double rho_40k = atm.GetStdDensity(40000.0);

    CHECK(rho_sl  > rho_10k);
    CHECK(rho_10k > rho_20k);
    CHECK(rho_20k > rho_40k);

    // Verify ideal gas law: rho = P / (R * T)
    double P = atm.GetStdPressure(20000.0);
    double T = atm.GetStdTemperature(20000.0);
    double rho_calc = P / (FGAtmosphereBase::Reng0 * T);
    CHECK(rho_20k == doctest::Approx(rho_calc).epsilon(REL_TOL));
}

} // TEST_SUITE PressureAndDensity

// ═══════════════════════════════════════════════════════════════════════════════
// Unit conversions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("UnitConversions") {

TEST_CASE("temperature unit conversions round-trip")
{
    FGStandardAtmosphere1976 atm;
    double T_R = 518.67;

    SUBCASE("Rankine to Fahrenheit and back") {
        double T_F = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eFahrenheit);
        double T_back = atm.ConvertToRankine(T_F, FGAtmosphereBase::eFahrenheit);
        CHECK(T_back == doctest::Approx(T_R).epsilon(TIGHT_TOL));
        CHECK(T_F == doctest::Approx(59.0).epsilon(1e-6));
    }

    SUBCASE("Rankine to Celsius and back") {
        double T_C = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eCelsius);
        double T_back = atm.ConvertToRankine(T_C, FGAtmosphereBase::eCelsius);
        CHECK(T_back == doctest::Approx(T_R).epsilon(TIGHT_TOL));
        CHECK(T_C == doctest::Approx(15.0).epsilon(1e-6));
    }

    SUBCASE("Rankine to Kelvin and back") {
        double T_K = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eKelvin);
        double T_back = atm.ConvertToRankine(T_K, FGAtmosphereBase::eKelvin);
        CHECK(T_back == doctest::Approx(T_R).epsilon(TIGHT_TOL));
        CHECK(T_K == doctest::Approx(288.15).epsilon(1e-6));
    }

    SUBCASE("Rankine identity") {
        double T_back = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eRankine);
        CHECK(T_back == doctest::Approx(T_R).epsilon(TIGHT_TOL));
    }
}

TEST_CASE("pressure unit conversions round-trip")
{
    FGStandardAtmosphere1976 atm;
    double P_psf = 2116.228;

    SUBCASE("PSF to Millibars and back") {
        double P_mb = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::eMillibars);
        double P_back = atm.ConvertToPSF(P_mb, FGAtmosphereBase::eMillibars);
        CHECK(P_back == doctest::Approx(P_psf).epsilon(TIGHT_TOL));
        CHECK(P_mb == doctest::Approx(1013.25).epsilon(2e-3));
    }

    SUBCASE("PSF to Pascals and back") {
        double P_pa = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::ePascals);
        double P_back = atm.ConvertToPSF(P_pa, FGAtmosphereBase::ePascals);
        CHECK(P_back == doctest::Approx(P_psf).epsilon(TIGHT_TOL));
        CHECK(P_pa == doctest::Approx(101325.0).epsilon(2e-3));
    }

    SUBCASE("PSF to InchesHg and back") {
        double P_inhg = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::eInchesHg);
        double P_back = atm.ConvertToPSF(P_inhg, FGAtmosphereBase::eInchesHg);
        CHECK(P_back == doctest::Approx(P_psf).epsilon(TIGHT_TOL));
        CHECK(P_inhg == doctest::Approx(29.92).epsilon(2e-3));
    }
}

} // TEST_SUITE UnitConversions

// ═══════════════════════════════════════════════════════════════════════════════
// Geopotential / geometric altitude conversions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("AltitudeConversions") {

TEST_CASE("geopotential-geometric altitude round-trip")
{
    FGStandardAtmosphere1976 atm;

    double test_alts[] = {0.0, 10000.0, 36089.0, 65617.0, 100000.0, 200000.0};

    for (double h : test_alts) {
        double geopot = atm.GeopotentialAltitude(h);
        double h_back = atm.GeometricAltitude(geopot);
        CHECK(h_back == doctest::Approx(h).epsilon(TIGHT_TOL));
    }
}

TEST_CASE("geopotential is zero at sea level")
{
    FGStandardAtmosphere1976 atm;
    CHECK(atm.GeopotentialAltitude(0.0) == 0.0);
    CHECK(atm.GeometricAltitude(0.0) == 0.0);
}

TEST_CASE("geopotential less than geometric at positive altitude")
{
    FGStandardAtmosphere1976 atm;

    // H = r*h / (r+h) < h for h > 0
    double h = 100000.0;
    double H = atm.GeopotentialAltitude(h);
    CHECK(H < h);
    CHECK(H > 0.0);
}

} // TEST_SUITE AltitudeConversions

// ═══════════════════════════════════════════════════════════════════════════════
// Temperature bias and pressure override
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("TemperatureAndPressureOverrides") {

TEST_CASE("temperature bias shifts profile uniformly")
{
    FGStandardAtmosphere1976 atm;

    double T_std_sl  = atm.GetStdTemperature(0.0);
    double T_std_20k = atm.GetStdTemperature(20000.0);

    // Apply a +10 Rankine bias
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 10.0);

    double T_biased_sl  = atm.GetTemperature(0.0);
    double T_biased_20k = atm.GetTemperature(20000.0);

    CHECK(T_biased_sl  == doctest::Approx(T_std_sl + 10.0).epsilon(REL_TOL));
    CHECK(T_biased_20k == doctest::Approx(T_std_20k + 10.0).epsilon(REL_TOL));

    // Standard temperature must remain unaffected
    CHECK(atm.GetStdTemperature(0.0) == doctest::Approx(T_std_sl).epsilon(TIGHT_TOL));
}

TEST_CASE("reset temperature restores standard profile")
{
    FGStandardAtmosphere1976 atm;

    double T_std = atm.GetTemperature(0.0);

    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 20.0);
    CHECK(atm.GetTemperature(0.0) != doctest::Approx(T_std).epsilon(1e-6));

    atm.ResetSLTemperature();
    CHECK(atm.GetTemperature(0.0) == doctest::Approx(T_std).epsilon(REL_TOL));
}

TEST_CASE("pressure override changes SL pressure and resets")
{
    FGStandardAtmosphere1976 atm;

    double new_p_psf = 2200.0;
    atm.SetPressureSL(FGAtmosphereBase::ePSF, new_p_psf);

    CHECK(atm.GetPressureSL() == doctest::Approx(new_p_psf).epsilon(REL_TOL));

    atm.Calculate(0.0);
    CHECK(atm.GetPressure() == doctest::Approx(new_p_psf).epsilon(REL_TOL));

    // Reset restores standard
    atm.ResetSLPressure();
    CHECK(atm.GetPressureSL() == doctest::Approx(2116.228).epsilon(REL_TOL));
}

} // TEST_SUITE TemperatureAndPressureOverrides

// ═══════════════════════════════════════════════════════════════════════════════
// Speed of sound
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("SpeedOfSound") {

TEST_CASE("speed of sound at sea level")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    double a_expected = std::sqrt(FGAtmosphereBase::SHRatio *
                                  FGAtmosphereBase::Reng0 *
                                  FGAtmosphereBase::StdDaySLtemperature);

    CHECK(atm.GetSoundSpeed()   == doctest::Approx(a_expected).epsilon(REL_TOL));
    CHECK(atm.GetSoundSpeedSL() == doctest::Approx(a_expected).epsilon(REL_TOL));

    // Known value: ~1116.45 ft/s
    CHECK(atm.GetSoundSpeed() == doctest::Approx(1116.45).epsilon(2e-3));
}

TEST_CASE("speed of sound decreases in troposphere")
{
    FGStandardAtmosphere1976 atm;

    double a_sl  = atm.GetSoundSpeed(0.0);
    double a_10k = atm.GetSoundSpeed(10000.0);
    double a_30k = atm.GetSoundSpeed(30000.0);

    CHECK(a_sl  > a_10k);
    CHECK(a_10k > a_30k);

    // Verify from temperature: a = sqrt(gamma * R * T)
    double T_10k = atm.GetTemperature(10000.0);
    double a_calc = std::sqrt(FGAtmosphereBase::SHRatio * FGAtmosphereBase::Reng0 * T_10k);
    CHECK(a_10k == doctest::Approx(a_calc).epsilon(REL_TOL));
}

} // TEST_SUITE SpeedOfSound

// ═══════════════════════════════════════════════════════════════════════════════
// Below-sea-level behavior
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("BelowSeaLevel") {

TEST_CASE("below sea level extrapolation increases T and P")
{
    FGStandardAtmosphere1976 atm;

    // Temperature should increase below sea level (tropospheric lapse extrapolated)
    double T_sl = atm.GetStdTemperature(0.0);
    double T_neg1000 = atm.GetStdTemperature(-1000.0);
    CHECK(T_neg1000 > T_sl);

    // Pressure should also increase below sea level
    double P_sl = atm.GetPressure(0.0);
    double P_neg1000 = atm.GetPressure(-1000.0);
    CHECK(P_neg1000 > P_sl);
}

} // TEST_SUITE BelowSeaLevel

// ═══════════════════════════════════════════════════════════════════════════════
// Viscosity
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("Viscosity") {

TEST_CASE("viscosity is positive and self-consistent")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    CHECK(atm.GetAbsoluteViscosity()  > 0.0);
    CHECK(atm.GetKinematicViscosity() > 0.0);

    // Kinematic viscosity = absolute viscosity / density
    double mu  = atm.GetAbsoluteViscosity();
    double rho = atm.GetDensity();
    double nu  = atm.GetKinematicViscosity();
    CHECK(nu == doctest::Approx(mu / rho).epsilon(REL_TOL));
}

} // TEST_SUITE Viscosity

// ═══════════════════════════════════════════════════════════════════════════════
// Calculate and derived quantities
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("CalculateConsistency") {

TEST_CASE("Calculate populates all fields consistently at altitude")
{
    FGStandardAtmosphere1976 atm;
    double alt = 25000.0;
    atm.Calculate(alt);

    double T   = atm.GetTemperature();
    double P   = atm.GetPressure();
    double rho = atm.GetDensity();
    double a   = atm.GetSoundSpeed();

    // Cached values should match altitude-query functions
    CHECK(T   == doctest::Approx(atm.GetTemperature(alt)).epsilon(REL_TOL));
    CHECK(P   == doctest::Approx(atm.GetPressure(alt)).epsilon(REL_TOL));
    CHECK(rho == doctest::Approx(atm.GetDensity(alt)).epsilon(REL_TOL));
    CHECK(a   == doctest::Approx(atm.GetSoundSpeed(alt)).epsilon(REL_TOL));

    // Ideal gas law: rho = P / (R * T)
    double rho_calc = P / (FGAtmosphereBase::Reng0 * T);
    CHECK(rho == doctest::Approx(rho_calc).epsilon(REL_TOL));

    // Sound speed: a = sqrt(gamma * R * T)
    double a_calc = std::sqrt(FGAtmosphereBase::SHRatio * FGAtmosphereBase::Reng0 * T);
    CHECK(a == doctest::Approx(a_calc).epsilon(REL_TOL));
}

TEST_CASE("SetTemperatureSL changes SL temperature")
{
    FGStandardAtmosphere1976 atm;

    // Set SL temperature to 70 F (standard is 59 F)
    atm.SetTemperatureSL(70.0, FGAtmosphereBase::eFahrenheit);

    double T_sl = atm.GetTemperatureSL();
    double T_expected = atm.ConvertToRankine(70.0, FGAtmosphereBase::eFahrenheit);

    CHECK(T_sl == doctest::Approx(T_expected).epsilon(REL_TOL));
}

TEST_CASE("pressure and density altitude near zero at standard SL")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    // At standard conditions and sea level, both should be approximately zero
    CHECK(atm.GetPressureAltitude() == doctest::Approx(0.0).epsilon(1e-3));
    CHECK(atm.GetDensityAltitude()  == doctest::Approx(0.0).epsilon(1e-3));
}

} // TEST_SUITE CalculateConsistency

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("CoverageGaps") {

TEST_CASE("GetPressureSL with unit conversions")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    CHECK(atm.GetPressureSL(FGAtmosphereBase::ePSF) == doctest::Approx(2116.228).epsilon(REL_TOL));
    CHECK(atm.GetPressureSL(FGAtmosphereBase::ePascals) == doctest::Approx(101325.0).epsilon(5e-3));
    CHECK(atm.GetPressureSL(FGAtmosphereBase::eMillibars) == doctest::Approx(1013.25).epsilon(5e-3));
    CHECK(atm.GetPressureSL(FGAtmosphereBase::eInchesHg) == doctest::Approx(29.92).epsilon(5e-3));
}

TEST_CASE("GetDensitySL value")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    double expected = FGAtmosphereBase::StdDaySLdensity;
    CHECK(atm.GetDensitySL() == doctest::Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("GetSoundSpeedSL value")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    double expected = std::sqrt(FGAtmosphereBase::SHRatio *
                                FGAtmosphereBase::Reng0 *
                                FGAtmosphereBase::StdDaySLtemperature);
    CHECK(atm.GetSoundSpeedSL() == doctest::Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("GetSoundSpeedRatio")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    CHECK(atm.GetSoundSpeedRatio() == doctest::Approx(1.0).epsilon(REL_TOL));

    atm.Calculate(35000.0);
    double ratio = atm.GetSoundSpeed() / atm.GetSoundSpeedSL();
    CHECK(atm.GetSoundSpeedRatio() == doctest::Approx(ratio).epsilon(REL_TOL));
    CHECK(atm.GetSoundSpeedRatio() < 1.0);
}

TEST_CASE("GetStdTemperatureSL")
{
    FGStandardAtmosphere1976 atm;
    CHECK(atm.GetStdTemperatureSL() == doctest::Approx(518.67).epsilon(REL_TOL));
}

TEST_CASE("GetStdTemperatureRatio")
{
    FGStandardAtmosphere1976 atm;
    CHECK(atm.GetStdTemperatureRatio(0.0) == doctest::Approx(1.0).epsilon(REL_TOL));

    double ratio_11km = atm.GetStdTemperatureRatio(36089.0);
    double expected = 389.97 / 518.67;
    CHECK(ratio_11km == doctest::Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("GetTemperatureDeltaGradient default zero")
{
    FGStandardAtmosphere1976 atm;
    CHECK(atm.GetTemperatureDeltaGradient() == 0.0);
}

TEST_CASE("InitModel restores standard")
{
    FGStandardAtmosphere1976 atm;
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 25.0);
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 2050.0);

    atm.InitModel();
    atm.Calculate(0.0);

    CHECK(atm.GetTemperature() == doctest::Approx(518.67).epsilon(REL_TOL));
    CHECK(atm.GetPressure() == doctest::Approx(2116.228).epsilon(REL_TOL));
    CHECK(atm.GetTemperatureBias() == doctest::Approx(0.0).epsilon(1e-12));
}

TEST_CASE("SetSLTemperatureGradedDelta")
{
    FGStandardAtmosphere1976 atm;
    double T_std_sl = atm.GetStdTemperature(0.0);

    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, 10.0);

    CHECK(atm.GetTemperature(0.0) > T_std_sl);
    CHECK(atm.GetTemperatureDeltaGradient() != 0.0);
}

TEST_CASE("ConvertToRankine invalid unit throws")
{
    FGStandardAtmosphere1976 atm;
    CHECK_THROWS_AS(atm.ConvertToRankine(100.0, FGAtmosphereBase::eNoTempUnit),
                    std::invalid_argument);
}

TEST_CASE("ConvertFromRankine invalid unit throws")
{
    FGStandardAtmosphere1976 atm;
    CHECK_THROWS_AS(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eNoTempUnit),
                    std::invalid_argument);
}

TEST_CASE("ConvertToPSF invalid unit throws")
{
    FGStandardAtmosphere1976 atm;
    CHECK_THROWS_AS(atm.ConvertToPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                    std::invalid_argument);
}

TEST_CASE("ConvertFromPSF invalid unit throws")
{
    FGStandardAtmosphere1976 atm;
    CHECK_THROWS_AS(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                    std::invalid_argument);
}

TEST_CASE("ConvertToPSF from Millibars and InchesHg")
{
    FGStandardAtmosphere1976 atm;
    double psf_from_mb = atm.ConvertToPSF(1013.25, FGAtmosphereBase::eMillibars);
    CHECK(psf_from_mb == doctest::Approx(2116.228).epsilon(5e-3));

    double psf_from_inhg = atm.ConvertToPSF(29.92, FGAtmosphereBase::eInchesHg);
    CHECK(psf_from_inhg == doctest::Approx(2116.228).epsilon(5e-3));
}

TEST_CASE("Free temperature conversion helpers")
{
    CHECK(RankineToCelsius(518.67) == doctest::Approx(15.0).epsilon(1e-6));
    // KelvinToFahrenheit uses 1.8*k - 459.4 (approx; exact -459.67 gives 59.0)
    CHECK(KelvinToFahrenheit(288.15) == doctest::Approx(59.27).epsilon(1e-3));
    CHECK(FahrenheitToCelsius(59.0) == doctest::Approx(15.0).epsilon(1e-6));
    CHECK(CelsiusToFahrenheit(15.0) == doctest::Approx(59.0).epsilon(1e-6));
    CHECK(CelsiusToKelvin(15.0) == doctest::Approx(288.15).epsilon(1e-6));
    CHECK(KelvinToCelsius(288.15) == doctest::Approx(15.0).epsilon(1e-6));
}

TEST_CASE("GetLapseRates and GetPressureBreakpoints")
{
    FGStandardAtmosphere1976 atm;

    const auto& lr = atm.GetLapseRates();
    CHECK(lr.size() > 0);
    CHECK(lr[0] < 0.0);

    const auto& pb = atm.GetPressureBreakpoints();
    CHECK(pb.size() > 0);
    for (size_t i = 1; i < pb.size(); ++i)
        CHECK(pb[i] < pb[i-1]);
}

} // TEST_SUITE CoverageGaps
