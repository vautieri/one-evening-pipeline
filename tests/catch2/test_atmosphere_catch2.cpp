/*==============================================================================
 Catch2 suite for JSBSim ISA 1976 Standard Atmosphere model
 Framework #2 of 5 — https://github.com/catchorg/Catch2
==============================================================================*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include "FGStandardAtmosphere1976.h"

using namespace JSBSimExtract;
using Catch::Approx;

// Relative tolerance for atmosphere comparisons (~0.1%)
static constexpr double REL_TOL = 1e-3;

// Tighter tolerance for unit conversions and exact identities
static constexpr double TIGHT_TOL = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// Sea-level standard conditions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: sea-level standard temperature", "[atmosphere][sea-level]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    REQUIRE(atm.GetTemperature()  == Approx(518.67).epsilon(REL_TOL));
    REQUIRE(atm.GetTemperatureSL() == Approx(518.67).epsilon(REL_TOL));
    REQUIRE(atm.GetStdTemperature(0.0) == Approx(518.67).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: sea-level standard pressure", "[atmosphere][sea-level]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    REQUIRE(atm.GetPressure()  == Approx(2116.228).epsilon(REL_TOL));
    REQUIRE(atm.GetPressureSL() == Approx(2116.228).epsilon(REL_TOL));
    REQUIRE(atm.GetStdPressure(0.0) == Approx(2116.228).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: sea-level standard density", "[atmosphere][sea-level]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    // rho_SL = P_SL / (R * T_SL) = 2116.228 / (Reng0 * 518.67)
    double expected_density = FGAtmosphereBase::StdDaySLpressure /
        (FGAtmosphereBase::Reng0 * FGAtmosphereBase::StdDaySLtemperature);

    REQUIRE(atm.GetDensity()   == Approx(expected_density).epsilon(REL_TOL));
    REQUIRE(atm.GetDensitySL() == Approx(expected_density).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Temperature at ISA layer boundaries
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: tropopause temperature at 36089 ft", "[atmosphere][temperature]")
{
    FGStandardAtmosphere1976 atm;

    // At the tropopause (geopotential 36089.24 ft), temperature should be 389.97 R
    double T = atm.GetStdTemperature(36089.0);
    REQUIRE(T == Approx(389.97).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: isothermal layer at 65617 ft", "[atmosphere][temperature]")
{
    FGStandardAtmosphere1976 atm;

    // Still in the isothermal layer (tropopause to 65617 ft), T stays at 389.97 R
    double T_tropo = atm.GetStdTemperature(36089.0);
    double T_upper = atm.GetStdTemperature(50000.0);
    REQUIRE(T_upper == Approx(T_tropo).epsilon(REL_TOL));

    // At the top of the isothermal layer
    double T_top = atm.GetStdTemperature(65617.0);
    REQUIRE(T_top == Approx(389.97).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: temperature at stratosphere layers", "[atmosphere][temperature]")
{
    FGStandardAtmosphere1976 atm;

    // API takes geometric altitude; convert geopotential breakpoints to geometric
    double geom_32km = atm.GeometricAltitude(104986.8766);
    double geom_47km = atm.GeometricAltitude(154199.4751);

    double T_32km = atm.GetStdTemperature(geom_32km);
    REQUIRE(T_32km == Approx(411.57).epsilon(REL_TOL));

    double T_47km = atm.GetStdTemperature(geom_47km);
    REQUIRE(T_47km == Approx(487.17).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Pressure and density at key altitudes
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: pressure decreases with altitude", "[atmosphere][pressure]")
{
    FGStandardAtmosphere1976 atm;

    double P_sl   = atm.GetStdPressure(0.0);
    double P_10k  = atm.GetStdPressure(10000.0);
    double P_20k  = atm.GetStdPressure(20000.0);
    double P_36k  = atm.GetStdPressure(36000.0);
    double P_50k  = atm.GetStdPressure(50000.0);

    REQUIRE(P_sl  > P_10k);
    REQUIRE(P_10k > P_20k);
    REQUIRE(P_20k > P_36k);
    REQUIRE(P_36k > P_50k);

    // At 36000 ft geometric, pressure is in the ~470-480 psf range
    REQUIRE(P_36k == Approx(476.1).epsilon(1e-2));
}

TEST_CASE("Atmosphere: density decreases with altitude", "[atmosphere][density]")
{
    FGStandardAtmosphere1976 atm;

    double rho_sl  = atm.GetStdDensity(0.0);
    double rho_10k = atm.GetStdDensity(10000.0);
    double rho_20k = atm.GetStdDensity(20000.0);
    double rho_40k = atm.GetStdDensity(40000.0);

    REQUIRE(rho_sl  > rho_10k);
    REQUIRE(rho_10k > rho_20k);
    REQUIRE(rho_20k > rho_40k);

    // Standard density consistent with ideal gas law
    double P = atm.GetStdPressure(20000.0);
    double T = atm.GetStdTemperature(20000.0);
    double rho_calc = P / (FGAtmosphereBase::Reng0 * T);
    REQUIRE(rho_20k == Approx(rho_calc).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Unit conversions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: temperature unit conversions round-trip", "[atmosphere][conversion]")
{
    FGStandardAtmosphere1976 atm;

    // Rankine -> Fahrenheit -> Rankine
    double T_R = 518.67;
    double T_F = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eFahrenheit);
    double T_back = atm.ConvertToRankine(T_F, FGAtmosphereBase::eFahrenheit);
    REQUIRE(T_back == Approx(T_R).margin(TIGHT_TOL));

    // Rankine -> Celsius -> Rankine
    double T_C = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eCelsius);
    T_back = atm.ConvertToRankine(T_C, FGAtmosphereBase::eCelsius);
    REQUIRE(T_back == Approx(T_R).margin(TIGHT_TOL));

    // Rankine -> Kelvin -> Rankine
    double T_K = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eKelvin);
    T_back = atm.ConvertToRankine(T_K, FGAtmosphereBase::eKelvin);
    REQUIRE(T_back == Approx(T_R).margin(TIGHT_TOL));

    // Rankine -> Rankine (identity)
    T_back = atm.ConvertFromRankine(T_R, FGAtmosphereBase::eRankine);
    REQUIRE(T_back == Approx(T_R).margin(TIGHT_TOL));

    // Check known values: 518.67 R = 59 F = 15 C = 288.15 K
    REQUIRE(T_F == Approx(59.0).margin(1e-6));
    REQUIRE(T_C == Approx(15.0).margin(1e-6));
    REQUIRE(T_K == Approx(288.15).margin(1e-6));
}

TEST_CASE("Atmosphere: pressure unit conversions round-trip", "[atmosphere][conversion]")
{
    FGStandardAtmosphere1976 atm;

    double P_psf = 2116.228;

    // PSF -> Millibars -> PSF
    double P_mb = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::eMillibars);
    double P_back = atm.ConvertToPSF(P_mb, FGAtmosphereBase::eMillibars);
    REQUIRE(P_back == Approx(P_psf).epsilon(TIGHT_TOL));

    // PSF -> Pascals -> PSF
    double P_pa = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::ePascals);
    P_back = atm.ConvertToPSF(P_pa, FGAtmosphereBase::ePascals);
    REQUIRE(P_back == Approx(P_psf).epsilon(TIGHT_TOL));

    // PSF -> InchesHg -> PSF
    double P_inhg = atm.ConvertFromPSF(P_psf, FGAtmosphereBase::eInchesHg);
    P_back = atm.ConvertToPSF(P_inhg, FGAtmosphereBase::eInchesHg);
    REQUIRE(P_back == Approx(P_psf).epsilon(TIGHT_TOL));

    // Standard atmosphere is ~1013.25 mbar = 101325 Pa = 29.92 inHg
    REQUIRE(P_mb   == Approx(1013.25).epsilon(2e-3));
    REQUIRE(P_pa   == Approx(101325.0).epsilon(2e-3));
    REQUIRE(P_inhg == Approx(29.92).epsilon(2e-3));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Geopotential / geometric altitude conversions
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: geopotential-geometric altitude round-trip", "[atmosphere][altitude]")
{
    FGStandardAtmosphere1976 atm;

    // Round-trip at several altitudes
    double test_alts[] = {0.0, 10000.0, 36089.0, 65617.0, 100000.0, 200000.0};

    for (double h : test_alts) {
        double geopot = atm.GeopotentialAltitude(h);
        double h_back = atm.GeometricAltitude(geopot);
        REQUIRE(h_back == Approx(h).epsilon(TIGHT_TOL));
    }
}

TEST_CASE("Atmosphere: geopotential is zero at sea level", "[atmosphere][altitude]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE(atm.GeopotentialAltitude(0.0) == 0.0);
    REQUIRE(atm.GeometricAltitude(0.0) == 0.0);
}

TEST_CASE("Atmosphere: geopotential less than geometric at altitude", "[atmosphere][altitude]")
{
    FGStandardAtmosphere1976 atm;

    // Geopotential altitude is always less than geometric at positive altitudes
    // because gravity weakens with height: H = r*h / (r+h) < h for h > 0
    double h = 100000.0;
    double H = atm.GeopotentialAltitude(h);
    REQUIRE(H < h);
    REQUIRE(H > 0.0);
}

// ═══════════════════════════════════════════════════════════════════════════════
// Temperature bias and pressure override
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: temperature bias shifts profile uniformly", "[atmosphere][bias]")
{
    FGStandardAtmosphere1976 atm;

    double T_std_sl = atm.GetStdTemperature(0.0);
    double T_std_20k = atm.GetStdTemperature(20000.0);

    // Apply a +10 Rankine bias
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 10.0);

    double T_biased_sl = atm.GetTemperature(0.0);
    double T_biased_20k = atm.GetTemperature(20000.0);

    REQUIRE(T_biased_sl  == Approx(T_std_sl + 10.0).epsilon(REL_TOL));
    REQUIRE(T_biased_20k == Approx(T_std_20k + 10.0).epsilon(REL_TOL));

    // Standard temperature should not be affected by bias
    REQUIRE(atm.GetStdTemperature(0.0) == Approx(T_std_sl).epsilon(TIGHT_TOL));
}

TEST_CASE("Atmosphere: reset temperature restores standard", "[atmosphere][bias]")
{
    FGStandardAtmosphere1976 atm;

    double T_std = atm.GetTemperature(0.0);

    // Apply bias, then reset
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 20.0);
    REQUIRE(atm.GetTemperature(0.0) != Approx(T_std).epsilon(1e-6));

    atm.ResetSLTemperature();
    REQUIRE(atm.GetTemperature(0.0) == Approx(T_std).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: pressure override changes SL pressure", "[atmosphere][pressure]")
{
    FGStandardAtmosphere1976 atm;

    // Set a non-standard sea-level pressure (e.g., high-pressure day)
    double new_p_psf = 2200.0;
    atm.SetPressureSL(FGAtmosphereBase::ePSF, new_p_psf);

    REQUIRE(atm.GetPressureSL() == Approx(new_p_psf).epsilon(REL_TOL));

    // Pressure at altitude should also shift
    atm.Calculate(0.0);
    REQUIRE(atm.GetPressure() == Approx(new_p_psf).epsilon(REL_TOL));

    // Reset restores standard
    atm.ResetSLPressure();
    REQUIRE(atm.GetPressureSL() == Approx(2116.228).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Speed of sound
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: speed of sound at sea level", "[atmosphere][sound]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    // a = sqrt(gamma * R * T) = sqrt(1.4 * Reng0 * 518.67)
    double a_expected = std::sqrt(FGAtmosphereBase::SHRatio *
                                  FGAtmosphereBase::Reng0 *
                                  FGAtmosphereBase::StdDaySLtemperature);

    REQUIRE(atm.GetSoundSpeed()   == Approx(a_expected).epsilon(REL_TOL));
    REQUIRE(atm.GetSoundSpeedSL() == Approx(a_expected).epsilon(REL_TOL));

    // Known value: ~1116.45 ft/s
    REQUIRE(atm.GetSoundSpeed() == Approx(1116.45).epsilon(2e-3));
}

TEST_CASE("Atmosphere: speed of sound decreases in troposphere", "[atmosphere][sound]")
{
    FGStandardAtmosphere1976 atm;

    double a_sl  = atm.GetSoundSpeed(0.0);
    double a_10k = atm.GetSoundSpeed(10000.0);
    double a_30k = atm.GetSoundSpeed(30000.0);

    // Sound speed decreases as temperature drops in troposphere
    REQUIRE(a_sl  > a_10k);
    REQUIRE(a_10k > a_30k);

    // Sound speed depends only on temperature: a = sqrt(gamma * R * T)
    double T_10k = atm.GetTemperature(10000.0);
    double a_calc = std::sqrt(FGAtmosphereBase::SHRatio * FGAtmosphereBase::Reng0 * T_10k);
    REQUIRE(a_10k == Approx(a_calc).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Below-sea-level behavior
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: below sea level extrapolation", "[atmosphere][below-sl]")
{
    FGStandardAtmosphere1976 atm;

    // Below sea level, temperature should increase (negative lapse rate in troposphere
    // means dT/dH = -3.566 R/1000ft, so going negative in H increases T)
    double T_sl = atm.GetStdTemperature(0.0);
    double T_neg1000 = atm.GetStdTemperature(-1000.0);

    REQUIRE(T_neg1000 > T_sl);

    // Pressure should also increase below sea level
    double P_sl = atm.GetPressure(0.0);
    double P_neg1000 = atm.GetPressure(-1000.0);

    REQUIRE(P_neg1000 > P_sl);
}

// ═══════════════════════════════════════════════════════════════════════════════
// Viscosity
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: viscosity is positive at sea level", "[atmosphere][viscosity]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    REQUIRE(atm.GetAbsoluteViscosity()  > 0.0);
    REQUIRE(atm.GetKinematicViscosity() > 0.0);

    // Kinematic viscosity = absolute viscosity / density
    double mu = atm.GetAbsoluteViscosity();
    double rho = atm.GetDensity();
    double nu = atm.GetKinematicViscosity();
    REQUIRE(nu == Approx(mu / rho).epsilon(REL_TOL));
}

// ═══════════════════════════════════════════════════════════════════════════════
// Calculate and derived quantities
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: Calculate populates all fields consistently", "[atmosphere][calculate]")
{
    FGStandardAtmosphere1976 atm;
    double alt = 25000.0;
    atm.Calculate(alt);

    // After Calculate, cached values should match altitude-query functions
    double T = atm.GetTemperature();
    double P = atm.GetPressure();
    double rho = atm.GetDensity();
    double a = atm.GetSoundSpeed();

    REQUIRE(T   == Approx(atm.GetTemperature(alt)).epsilon(REL_TOL));
    REQUIRE(P   == Approx(atm.GetPressure(alt)).epsilon(REL_TOL));
    REQUIRE(rho == Approx(atm.GetDensity(alt)).epsilon(REL_TOL));
    REQUIRE(a   == Approx(atm.GetSoundSpeed(alt)).epsilon(REL_TOL));

    // Ideal gas law consistency: rho = P / (R * T)
    double rho_calc = P / (FGAtmosphereBase::Reng0 * T);
    REQUIRE(rho == Approx(rho_calc).epsilon(REL_TOL));

    // Sound speed consistency: a = sqrt(gamma * R * T)
    double a_calc = std::sqrt(FGAtmosphereBase::SHRatio * FGAtmosphereBase::Reng0 * T);
    REQUIRE(a == Approx(a_calc).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: SetTemperatureSL changes SL temperature", "[atmosphere][setter]")
{
    FGStandardAtmosphere1976 atm;

    // Set SL temperature to 70 F (standard is 59 F)
    atm.SetTemperatureSL(70.0, FGAtmosphereBase::eFahrenheit);

    double T_sl = atm.GetTemperatureSL();
    double T_expected = atm.ConvertToRankine(70.0, FGAtmosphereBase::eFahrenheit); // 529.67 R

    REQUIRE(T_sl == Approx(T_expected).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: pressure and density altitude at sea level", "[atmosphere][altitude]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    // At standard conditions and sea level, pressure altitude and density altitude
    // should be approximately zero
    REQUIRE(atm.GetPressureAltitude() == Approx(0.0).margin(1.0));
    REQUIRE(atm.GetDensityAltitude()  == Approx(0.0).margin(1.0));
}

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Atmosphere: GetPressureSL with unit conversions", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);

    REQUIRE(atm.GetPressureSL(FGAtmosphereBase::ePSF) == Approx(2116.228).epsilon(REL_TOL));
    REQUIRE(atm.GetPressureSL(FGAtmosphereBase::ePascals) == Approx(101325.0).epsilon(5e-3));
    REQUIRE(atm.GetPressureSL(FGAtmosphereBase::eMillibars) == Approx(1013.25).epsilon(5e-3));
    REQUIRE(atm.GetPressureSL(FGAtmosphereBase::eInchesHg) == Approx(29.92).epsilon(5e-3));
}

TEST_CASE("Atmosphere: GetDensitySL value", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    double expected = FGAtmosphereBase::StdDaySLdensity;
    REQUIRE(atm.GetDensitySL() == Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: GetSoundSpeedSL value", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    double expected = std::sqrt(FGAtmosphereBase::SHRatio *
                                FGAtmosphereBase::Reng0 *
                                FGAtmosphereBase::StdDaySLtemperature);
    REQUIRE(atm.GetSoundSpeedSL() == Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: GetSoundSpeedRatio", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    atm.Calculate(0.0);
    REQUIRE(atm.GetSoundSpeedRatio() == Approx(1.0).epsilon(REL_TOL));

    atm.Calculate(35000.0);
    double ratio = atm.GetSoundSpeed() / atm.GetSoundSpeedSL();
    REQUIRE(atm.GetSoundSpeedRatio() == Approx(ratio).epsilon(REL_TOL));
    REQUIRE(atm.GetSoundSpeedRatio() < 1.0);
}

TEST_CASE("Atmosphere: GetStdTemperatureSL", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE(atm.GetStdTemperatureSL() == Approx(518.67).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: GetStdTemperatureRatio", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE(atm.GetStdTemperatureRatio(0.0) == Approx(1.0).epsilon(REL_TOL));

    double ratio_11km = atm.GetStdTemperatureRatio(36089.0);
    double expected = 389.97 / 518.67;
    REQUIRE(ratio_11km == Approx(expected).epsilon(REL_TOL));
}

TEST_CASE("Atmosphere: GetTemperatureDeltaGradient default zero", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE(atm.GetTemperatureDeltaGradient() == 0.0);
}

TEST_CASE("Atmosphere: InitModel restores standard", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 25.0);
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 2050.0);

    atm.InitModel();
    atm.Calculate(0.0);

    REQUIRE(atm.GetTemperature() == Approx(518.67).epsilon(REL_TOL));
    REQUIRE(atm.GetPressure() == Approx(2116.228).epsilon(REL_TOL));
    REQUIRE(atm.GetTemperatureBias() == Approx(0.0).margin(1e-12));
}

TEST_CASE("Atmosphere: SetSLTemperatureGradedDelta", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    double T_std_sl = atm.GetStdTemperature(0.0);

    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, 10.0);

    REQUIRE(atm.GetTemperature(0.0) > T_std_sl);
    REQUIRE(atm.GetTemperatureDeltaGradient() != 0.0);
}

TEST_CASE("Atmosphere: ConvertToRankine invalid unit throws", "[atmosphere][coverage][error]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE_THROWS_AS(atm.ConvertToRankine(100.0, FGAtmosphereBase::eNoTempUnit),
                      std::invalid_argument);
}

TEST_CASE("Atmosphere: ConvertFromRankine invalid unit throws", "[atmosphere][coverage][error]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE_THROWS_AS(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eNoTempUnit),
                      std::invalid_argument);
}

TEST_CASE("Atmosphere: ConvertToPSF invalid unit throws", "[atmosphere][coverage][error]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE_THROWS_AS(atm.ConvertToPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                      std::invalid_argument);
}

TEST_CASE("Atmosphere: ConvertFromPSF invalid unit throws", "[atmosphere][coverage][error]")
{
    FGStandardAtmosphere1976 atm;
    REQUIRE_THROWS_AS(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                      std::invalid_argument);
}

TEST_CASE("Atmosphere: ConvertToPSF from Millibars and InchesHg", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;
    double psf_from_mb = atm.ConvertToPSF(1013.25, FGAtmosphereBase::eMillibars);
    REQUIRE(psf_from_mb == Approx(2116.228).epsilon(5e-3));

    double psf_from_inhg = atm.ConvertToPSF(29.92, FGAtmosphereBase::eInchesHg);
    REQUIRE(psf_from_inhg == Approx(2116.228).epsilon(5e-3));
}

TEST_CASE("Atmosphere: free temperature conversion helpers", "[atmosphere][coverage]")
{
    REQUIRE(RankineToCelsius(518.67) == Approx(15.0).margin(1e-6));
    // KelvinToFahrenheit uses 1.8*k - 459.4 (approx; exact -459.67 gives 59.0)
    REQUIRE(KelvinToFahrenheit(288.15) == Approx(59.27).epsilon(1e-3));
    REQUIRE(FahrenheitToCelsius(59.0) == Approx(15.0).margin(1e-6));
    REQUIRE(CelsiusToFahrenheit(15.0) == Approx(59.0).margin(1e-6));
    REQUIRE(CelsiusToKelvin(15.0) == Approx(288.15).margin(1e-6));
    REQUIRE(KelvinToCelsius(288.15) == Approx(15.0).margin(1e-6));
}

TEST_CASE("Atmosphere: GetLapseRates and GetPressureBreakpoints", "[atmosphere][coverage]")
{
    FGStandardAtmosphere1976 atm;

    const auto& lr = atm.GetLapseRates();
    REQUIRE(lr.size() > 0);
    REQUIRE(lr[0] < 0.0);  // Troposphere lapse rate is negative

    const auto& pb = atm.GetPressureBreakpoints();
    REQUIRE(pb.size() > 0);
    for (size_t i = 1; i < pb.size(); ++i)
        REQUIRE(pb[i] < pb[i-1]);  // Pressure decreases
}
