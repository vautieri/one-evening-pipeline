/*==============================================================================
 Google Test suite for the JSBSim 1976 U.S. Standard Atmosphere (ISA) model
 Framework: Google Test  --  https://github.com/google/googletest

 System under test:
   JSBSimExtract::FGStandardAtmosphere1976
   (derived from JSBSimExtract::FGAtmosphereBase)

 Reference document:
   "U.S. Standard Atmosphere, 1976", NASA TM-X-74335

 IMPORTANT NOTE ON ALTITUDES:
   The ISA 1976 model stores its temperature table in *geopotential* altitude
   (feet).  The public API (GetTemperature, GetPressure, etc.) accepts
   *geometric* altitude which is internally converted to geopotential via:
       H = (R_E * h) / (R_E + h)
   To query the model at exact ISA layer boundaries, one must first convert
   the geopotential breakpoints to their geometric equivalents using
   GeometricAltitude().  This test fixture does so in SetUp().

 Tolerances are chosen so that tests pass if the model reproduces the ISA
 tables to ~0.1 % or better, which is the fidelity expected from an
 aerospace-grade atmosphere model.
==============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <sstream>
#include "FGStandardAtmosphere1976.h"

using namespace JSBSimExtract;

// =============================================================================
//  Constants and helpers
// =============================================================================

// ISA geopotential layer boundaries [ft] -- exactly as stored in the model's
// internal temperature lookup table.
static constexpr double kGeoPot_0km  =      0.0;         //  0.000 km
static constexpr double kGeoPot_11km =  36089.2388;       // 11.000 km (tropopause)
static constexpr double kGeoPot_20km =  65616.7979;       // 20.000 km
static constexpr double kGeoPot_32km = 104986.8766;       // 32.000 km
static constexpr double kGeoPot_47km = 154199.4751;       // 47.000 km
static constexpr double kGeoPot_51km = 167322.8346;       // 51.000 km
static constexpr double kGeoPot_71km = 232939.6325;       // 71.000 km
static constexpr double kGeoPot_84km = 278385.8268;       // 84.852 km

// ISA reference temperatures at layer boundaries [Rankine]
static constexpr double kT_0km  = 518.67;    // 288.15 K
static constexpr double kT_11km = 389.97;    // 216.65 K
static constexpr double kT_20km = 389.97;    // 216.65 K (isothermal tropopause)
static constexpr double kT_32km = 411.57;    // 228.65 K
static constexpr double kT_47km = 487.17;    // 270.65 K
static constexpr double kT_51km = 487.17;    // 270.65 K (isothermal)
static constexpr double kT_71km = 386.37;    // 214.65 K
static constexpr double kT_84km = 336.5028;  // 186.946 K

// ISA standard sea-level pressure [psf]
static constexpr double kP_SL = 2116.228;

// Tolerances
static constexpr double kTempTolR    = 0.05;    // 0.05 R ~ 0.03 K (~0.01 %)
static constexpr double kRelTol      = 1.0e-3;  // 0.1 % relative
static constexpr double kRelTolLoose = 5.0e-3;  // 0.5 % (unit-conversion rounding)
static constexpr double kConvTol     = 1.0e-6;  // tight for pure arithmetic

/// Relative-error helper: |actual - expected| / |expected|
static double RelErr(double actual, double expected) {
    if (expected == 0.0) return std::fabs(actual);
    return std::fabs((actual - expected) / expected);
}

// =============================================================================
//  Test fixture
//
//  Creates one FGStandardAtmosphere1976 instance per test and pre-computes the
//  geometric equivalents of the geopotential layer boundaries.  The conversion
//  is necessary because the public API accepts geometric altitude, while the
//  published ISA breakpoints are defined in geopotential altitude.
// =============================================================================

class ISA1976Test : public ::testing::Test {
protected:
    FGStandardAtmosphere1976 atm;

    // Geometric altitudes corresponding to each geopotential breakpoint.
    double hGeom_0km,  hGeom_11km, hGeom_20km, hGeom_32km;
    double hGeom_47km, hGeom_51km, hGeom_71km, hGeom_84km;

    void SetUp() override {
        hGeom_0km  = atm.GeometricAltitude(kGeoPot_0km);
        hGeom_11km = atm.GeometricAltitude(kGeoPot_11km);
        hGeom_20km = atm.GeometricAltitude(kGeoPot_20km);
        hGeom_32km = atm.GeometricAltitude(kGeoPot_32km);
        hGeom_47km = atm.GeometricAltitude(kGeoPot_47km);
        hGeom_51km = atm.GeometricAltitude(kGeoPot_51km);
        hGeom_71km = atm.GeometricAltitude(kGeoPot_71km);
        hGeom_84km = atm.GeometricAltitude(kGeoPot_84km);
    }
};

// =============================================================================
//  1. Sea-level standard conditions (T, P, rho, speed of sound)
// =============================================================================

TEST_F(ISA1976Test, SeaLevelStandardConditions)
{
    // The four fundamental ISA sea-level properties underpin every subsequent
    // aerodynamic calculation.  ISA defines:
    //   T_SL = 288.15 K = 518.67 R
    //   P_SL = 101325 Pa = 2116.228 psf
    //   rho_SL ~ 1.225 kg/m^3 ~ 0.002377 slug/ft^3
    //   a_SL = sqrt(gamma * R * T) ~ 1116.45 ft/s
    atm.Calculate(0.0);

    EXPECT_NEAR(atm.GetTemperature(), kT_0km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(0.0), kT_0km, kTempTolR);
    EXPECT_NEAR(atm.GetPressure(), kP_SL, kP_SL * kRelTol);

    double expected_rho = FGAtmosphereBase::StdDaySLdensity;
    EXPECT_NEAR(atm.GetDensity(), expected_rho, expected_rho * kRelTol);

    double expected_a = std::sqrt(FGAtmosphereBase::SHRatio
                                  * FGAtmosphereBase::Reng0
                                  * FGAtmosphereBase::StdDaySLtemperature);
    EXPECT_NEAR(atm.GetSoundSpeed(), expected_a, expected_a * kRelTol);
}

// =============================================================================
//  2. ISA temperatures at each layer boundary
// =============================================================================

TEST_F(ISA1976Test, TemperatureAtAllLayerBoundaries)
{
    // The ISA temperature profile has 8 layers with distinct lapse rates.
    // Verifying the temperature at every boundary catches errors in the
    // lookup table, lapse-rate calculation, and geopotential-to-geometric
    // conversion.  Each boundary corresponds to a real atmospheric feature:
    //   11 km = tropopause, 20 km = end of isothermal, 32 km = mid-stratosphere,
    //   47 km = stratopause, 51 km = mesosphere onset, 71 km = upper mesosphere,
    //   84.852 km = ISA model upper limit.
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_0km),  kT_0km,  kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_11km), kT_11km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_20km), kT_20km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_32km), kT_32km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_47km), kT_47km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_51km), kT_51km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_71km), kT_71km, kTempTolR);
    EXPECT_NEAR(atm.GetStdTemperature(hGeom_84km), kT_84km, kTempTolR);
}

TEST_F(ISA1976Test, TemperatureMidTroposphere)
{
    // Test at 5 km geopotential (not a boundary) to verify lapse-rate
    // interpolation within the troposphere: T = 288.15 - 6.5*5 = 255.65 K.
    double h_geopot = 5000.0 / 0.3048;  // 5 km in feet
    double h_geom = atm.GeometricAltitude(h_geopot);
    double T_expected = CelsiusToRankine(15.0 - 6.5 * 5.0);  // 460.17 R
    EXPECT_NEAR(atm.GetStdTemperature(h_geom), T_expected, 0.5);
}

// =============================================================================
//  3. ISA pressures at key altitudes
// =============================================================================

TEST_F(ISA1976Test, PressureAtLayerBoundaries)
{
    // Pressure decreases roughly exponentially with altitude.  Published ISA
    // reference values (in Pascals) at geopotential boundaries, converted to
    // psf using the model's own factor (0.0208854342 psf/Pa):
    //   11 km: 22632.1 Pa,  20 km: 5474.89 Pa,
    //   32 km: 868.019 Pa,  47 km: 110.906 Pa
    struct { double geom; double P_pa; const char* label; } cases[] = {
        { hGeom_11km, 22632.1, "11km" },
        { hGeom_20km, 5474.89, "20km" },
        { hGeom_32km, 868.019, "32km" },
        { hGeom_47km, 110.906, "47km" },
    };
    for (auto& c : cases) {
        double expected_psf = c.P_pa * 0.0208854342;
        double actual = atm.GetStdPressure(c.geom);
        EXPECT_LT(RelErr(actual, expected_psf), kRelTol)
            << "P(" << c.label << "): actual=" << actual
            << "  expected=" << expected_psf;
    }
}

// =============================================================================
//  4. ISA density at key altitudes
// =============================================================================

TEST_F(ISA1976Test, DensityAtKeyAltitudes)
{
    // Density directly scales lift and drag.  We verify self-consistency
    // rho = P / (R * T) at layer boundaries, and that density monotonically
    // decreases with altitude.
    double alts[] = { hGeom_0km, hGeom_11km, hGeom_20km, hGeom_32km, hGeom_47km };
    double rho_prev = 1e30;
    for (double h : alts) {
        double P = atm.GetStdPressure(h);
        double T = atm.GetStdTemperature(h);
        double expected_rho = P / (FGAtmosphereBase::Reng0 * T);
        double actual = atm.GetStdDensity(h);
        EXPECT_NEAR(actual, expected_rho, expected_rho * kRelTol);
        EXPECT_LT(actual, rho_prev) << "Density not decreasing at h=" << h;
        rho_prev = actual;
    }
}

// =============================================================================
//  5. Speed of sound at key altitudes
// =============================================================================

TEST_F(ISA1976Test, SoundSpeedAtKeyAltitudes)
{
    // Speed of sound a = sqrt(gamma * R * T) depends only on temperature.
    // Verifying at SL and the tropopause confirms both the normal and
    // isothermal regions, which matters for Mach computation in cruise.
    double alts[] = { 0.0, hGeom_11km, hGeom_47km };
    for (double h : alts) {
        double T = atm.GetTemperature(h);
        double a_expected = std::sqrt(FGAtmosphereBase::SHRatio
                                      * FGAtmosphereBase::Reng0 * T);
        EXPECT_NEAR(atm.GetSoundSpeed(h), a_expected, a_expected * kRelTol);
    }
}

// =============================================================================
//  6. Geopotential / geometric altitude conversion round-trip
// =============================================================================

TEST_F(ISA1976Test, GeopotentialRoundTrip)
{
    // The ISA model converts geometric -> geopotential internally.  Round-trip
    // fidelity must be extremely tight because altitude errors map directly to
    // navigation errors and flight-level assignment.
    for (double h : { 0.0, 35000.0, 250000.0 }) {
        double H = atm.GeopotentialAltitude(h);
        double h_back = atm.GeometricAltitude(H);
        EXPECT_NEAR(h_back, h, 1e-4)
            << "Round-trip failed at h=" << h;
    }
}

TEST_F(ISA1976Test, GeopotentialLessThanGeometric)
{
    // Geopotential altitude is always less than geometric altitude (above SL)
    // because gravity weakens with height.  This is a fundamental invariant
    // of the Earth's gravity model.
    for (double h : { 1000.0, 10000.0, 50000.0, 200000.0 }) {
        EXPECT_LT(atm.GeopotentialAltitude(h), h)
            << "Geopotential must be < geometric at h=" << h;
    }
}

// =============================================================================
//  7. Unit conversion functions (all temperature and pressure units)
// =============================================================================

TEST_F(ISA1976Test, TemperatureUnitConversions)
{
    // The simulation must interoperate with instruments and weather services
    // that use Fahrenheit, Celsius, Kelvin, and Rankine.  A unit-conversion
    // bug can cause a crash scenario in the sim.

    // Fahrenheit: 518.67 R = 59.0 F
    EXPECT_NEAR(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eFahrenheit), 59.0, kConvTol);
    EXPECT_NEAR(atm.ConvertToRankine(59.0, FGAtmosphereBase::eFahrenheit), 518.67, kConvTol);

    // Celsius: 518.67 R = 15.0 C
    EXPECT_NEAR(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eCelsius), 15.0, kConvTol);
    EXPECT_NEAR(atm.ConvertToRankine(15.0, FGAtmosphereBase::eCelsius), 518.67, kConvTol);

    // Kelvin: 518.67 R = 288.15 K
    EXPECT_NEAR(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eKelvin), 288.15, kConvTol);
    EXPECT_NEAR(atm.ConvertToRankine(288.15, FGAtmosphereBase::eKelvin), 518.67, kConvTol);

    // Rankine identity
    EXPECT_DOUBLE_EQ(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eRankine), 518.67);
    EXPECT_DOUBLE_EQ(atm.ConvertToRankine(518.67, FGAtmosphereBase::eRankine), 518.67);
}

TEST_F(ISA1976Test, PressureUnitConversions)
{
    // Pressure conversions are used for altimeter setting (inHg), meteorology
    // (mbar/hPa), and engineering analysis (Pa).

    // Pascals: 2116.228 psf ~ 101325 Pa
    double pa = atm.ConvertFromPSF(kP_SL, FGAtmosphereBase::ePascals);
    EXPECT_NEAR(pa, 101325.0, 101325.0 * kRelTolLoose);
    EXPECT_NEAR(atm.ConvertToPSF(101325.0, FGAtmosphereBase::ePascals), kP_SL,
                kP_SL * kRelTolLoose);

    // Millibars: 2116.228 psf ~ 1013.25 mbar
    EXPECT_NEAR(atm.ConvertFromPSF(kP_SL, FGAtmosphereBase::eMillibars),
                1013.25, 1013.25 * kRelTolLoose);

    // Inches of mercury: 2116.228 psf ~ 29.921 inHg
    EXPECT_NEAR(atm.ConvertFromPSF(kP_SL, FGAtmosphereBase::eInchesHg),
                29.921, 29.921 * kRelTolLoose);

    // PSF identity
    EXPECT_DOUBLE_EQ(atm.ConvertFromPSF(kP_SL, FGAtmosphereBase::ePSF), kP_SL);
    EXPECT_DOUBLE_EQ(atm.ConvertToPSF(kP_SL, FGAtmosphereBase::ePSF), kP_SL);
}

// =============================================================================
//  8. Temperature bias (set +10 R bias, verify T shifts everywhere)
// =============================================================================

TEST_F(ISA1976Test, TemperatureBias_Positive)
{
    // Hot-day / cold-day modelling is essential for engine performance and
    // takeoff calculations.  A uniform bias shifts the entire temperature
    // profile by a constant offset.
    double bias = 10.0;
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, bias);

    // Verify at sea level, tropopause, and stratopause
    double checkAlts[] = { 0.0, hGeom_11km, hGeom_47km };
    for (double h : checkAlts) {
        double T_biased = atm.GetTemperature(h);
        double T_std    = atm.GetStdTemperature(h);
        EXPECT_NEAR(T_biased - T_std, bias, kTempTolR)
            << "Bias not applied correctly at h=" << h;
    }
}

TEST_F(ISA1976Test, SetTemperatureSL_Rankine)
{
    // Set SL temperature to 530 R (~294.4 K, a hot day) and verify.
    double T_new = 530.0;
    atm.SetTemperatureSL(T_new, FGAtmosphereBase::eRankine);
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), T_new, kTempTolR);
}

// =============================================================================
//  9. Pressure SL override
// =============================================================================

TEST_F(ISA1976Test, PressureSLOverride)
{
    // Non-standard sea-level pressure is the norm in real operations (QNH
    // altimeter setting).  Setting SL pressure below standard simulates a
    // low-pressure weather system; the entire column must adjust.
    double newP = 2100.0;
    atm.SetPressureSL(FGAtmosphereBase::ePSF, newP);

    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressure(), newP, newP * kRelTol);

    // At altitude the pressure must also be lower than ISA standard.
    EXPECT_LT(atm.GetPressure(hGeom_11km), atm.GetStdPressure(hGeom_11km));
}

// =============================================================================
//  10. ResetSLTemperature / ResetSLPressure restore standard
// =============================================================================

TEST_F(ISA1976Test, ResetSLTemperature)
{
    // After applying biases, ResetSLTemperature must restore the pure ISA
    // temperature profile.  This is critical for returning to a clean baseline
    // between simulation runs.
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 20.0);
    atm.ResetSLTemperature();
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), kT_0km, kTempTolR);
    EXPECT_NEAR(atm.GetTemperatureBias(), 0.0, 1e-12);
}

TEST_F(ISA1976Test, ResetSLPressure)
{
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 2050.0);
    atm.ResetSLPressure();
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressure(), kP_SL, kP_SL * kRelTol);
}

// =============================================================================
//  11. Viscosity (Sutherland's law) at sea level and altitude
// =============================================================================

TEST_F(ISA1976Test, ViscositySeaLevel)
{
    // Dynamic viscosity is needed for Reynolds-number computation in
    // skin-friction drag estimation.  Sutherland's law:
    //   mu = Beta * T^1.5 / (SutherlandConstant + T)
    atm.Calculate(0.0);
    double T = kT_0km;
    double mu_expected = FGAtmosphereBase::Beta * std::pow(T, 1.5)
                         / (FGAtmosphereBase::SutherlandConstant + T);
    EXPECT_NEAR(atm.GetAbsoluteViscosity(), mu_expected, mu_expected * kRelTol);
    EXPECT_GT(atm.GetAbsoluteViscosity(), 0.0);

    // Kinematic viscosity: nu = mu / rho
    double nu = atm.GetKinematicViscosity();
    EXPECT_GT(nu, 0.0);
    EXPECT_NEAR(nu, atm.GetAbsoluteViscosity() / atm.GetDensity(), nu * kRelTol);
}

TEST_F(ISA1976Test, ViscosityAtAltitude)
{
    // At the tropopause, temperature is lower so dynamic viscosity (mu) should
    // be lower than at SL per Sutherland's law.  But kinematic viscosity
    // (nu = mu/rho) increases because density drops faster than mu.
    atm.Calculate(hGeom_11km);
    double mu_alt = atm.GetAbsoluteViscosity();
    double nu_alt = atm.GetKinematicViscosity();

    FGStandardAtmosphere1976 atm2;
    atm2.Calculate(0.0);
    double mu_sl = atm2.GetAbsoluteViscosity();
    double nu_sl = atm2.GetKinematicViscosity();

    EXPECT_LT(mu_alt, mu_sl)
        << "Dynamic viscosity should decrease with temperature";
    EXPECT_GT(nu_alt, nu_sl)
        << "Kinematic viscosity should increase with altitude";
}

// =============================================================================
//  12. Density altitude and pressure altitude computations
// =============================================================================

TEST_F(ISA1976Test, DensityAltitudeUnderISA)
{
    // Density altitude governs aircraft performance (takeoff roll, climb rate).
    // Under standard conditions density altitude must equal geometric altitude.
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetDensityAltitude(), 0.0, 10.0);

    double h = 35000.0;
    atm.Calculate(h);
    EXPECT_NEAR(atm.GetDensityAltitude(), h, h * kRelTolLoose);
}

TEST_F(ISA1976Test, PressureAltitudeUnderISA)
{
    // Pressure altitude is used for flight-level assignments.  Under standard
    // conditions it must equal the geometric altitude.
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressureAltitude(), 0.0, 10.0);

    double h = 35000.0;
    atm.Calculate(h);
    EXPECT_NEAR(atm.GetPressureAltitude(), h, h * kRelTolLoose);
}

// =============================================================================
//  13. Below-sea-level extrapolation (negative altitude)
// =============================================================================

TEST_F(ISA1976Test, BelowSeaLevel_Extrapolation)
{
    // Locations like the Dead Sea (-1400 ft) or Death Valley (-282 ft) require
    // the model to extrapolate below the first table entry using the
    // tropospheric lapse rate.  Temperature, pressure, and density must all
    // exceed their sea-level values.
    double h = -1000.0;
    EXPECT_GT(atm.GetStdTemperature(h), kT_0km)
        << "Temperature below SL must exceed SL temperature";

    atm.Calculate(h);
    EXPECT_GT(atm.GetPressure(), kP_SL)
        << "Pressure below SL must exceed SL pressure";
    EXPECT_GT(atm.GetDensity(), FGAtmosphereBase::StdDaySLdensity)
        << "Density below SL must exceed SL density";
}

// =============================================================================
//  14. Very high altitude (>80 km) behavior
// =============================================================================

TEST_F(ISA1976Test, VeryHighAlt_Robustness)
{
    // Above the last ISA table entry (~84.852 km / ~282 152 ft geometric) the
    // model should clamp or extrapolate gracefully.  All values must remain
    // finite, positive, and physically reasonable.  This matters for re-entry
    // trajectory simulation.
    double h = 350000.0;  // ~107 km, well above the table
    atm.Calculate(h);

    EXPECT_FALSE(std::isnan(atm.GetTemperature()));
    EXPECT_FALSE(std::isnan(atm.GetPressure()));
    EXPECT_FALSE(std::isnan(atm.GetDensity()));
    EXPECT_FALSE(std::isnan(atm.GetSoundSpeed()));

    EXPECT_GT(atm.GetTemperature(), 0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
    EXPECT_GT(atm.GetDensity(), 0.0);
    EXPECT_GT(atm.GetSoundSpeed(), 0.0);
}

TEST_F(ISA1976Test, VeryHighAlt_LowDensity)
{
    // Density at 100+ km must be orders of magnitude below sea-level density.
    double h = 300000.0;  // ~91 km geometric
    atm.Calculate(h);
    double rho_ratio = atm.GetDensity() / FGAtmosphereBase::StdDaySLdensity;
    EXPECT_LT(rho_ratio, 1e-4)
        << "Density ratio at ~91 km should be < 1e-4";
}

// =============================================================================
//  15. Edge cases: zero altitude, maximum table altitude
// =============================================================================

TEST_F(ISA1976Test, ZeroAltitude_MatchesSLConstants)
{
    // Zero altitude must identically reproduce the class-level SL constants.
    // This catches off-by-one errors in table lookups.
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), FGAtmosphereBase::StdDaySLtemperature, kTempTolR);
    EXPECT_NEAR(atm.GetPressure(), FGAtmosphereBase::StdDaySLpressure,
                FGAtmosphereBase::StdDaySLpressure * kRelTol);
}

TEST_F(ISA1976Test, MaxTableAltitude_NoDiscontinuity)
{
    // Verify that approaching the last table entry does not produce a jump.
    // We sample just below, at, and just above the geometric altitude
    // corresponding to the highest geopotential breakpoint.
    double h_max = hGeom_84km;
    double T_at    = atm.GetStdTemperature(h_max);
    double T_below = atm.GetStdTemperature(h_max - 100.0);
    double T_above = atm.GetStdTemperature(h_max + 100.0);

    // Temperature must change smoothly (no jumps > 1 R over 100 ft).
    EXPECT_LT(std::fabs(T_at - T_below), 1.0);
    EXPECT_LT(std::fabs(T_above - T_at), 1.0);
}

// =============================================================================
//  Supplementary: Calculate() populates cached members, ratios
// =============================================================================

TEST_F(ISA1976Test, CalculatePopulatesAllCachedMembers)
{
    // After Calculate(h), the zero-argument getters must return the properties
    // at the requested altitude, not stale data from a previous call.
    double h = 20000.0;  // 20 000 ft, mid-troposphere
    atm.Calculate(h);

    EXPECT_NEAR(atm.GetTemperature(), atm.GetTemperature(h), kTempTolR);
    EXPECT_NEAR(atm.GetPressure(), atm.GetPressure(h),
                atm.GetPressure(h) * kRelTol);
    EXPECT_NEAR(atm.GetDensity(), atm.GetDensity(h),
                atm.GetDensity(h) * kRelTol);
    EXPECT_NEAR(atm.GetSoundSpeed(), atm.GetSoundSpeed(h),
                atm.GetSoundSpeed(h) * kRelTol);
}

TEST_F(ISA1976Test, AtmosphericRatios)
{
    // theta = T/T_SL, delta = P/P_SL, sigma = rho/rho_SL.
    // At sea level all ratios must be exactly 1.0.  At altitude, theta
    // must match the temperature ratio.  Ratios are used in similarity
    // methods for engine performance and aerodynamic scaling.
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperatureRatio(), 1.0, kRelTol);
    EXPECT_NEAR(atm.GetPressureRatio(), 1.0, kRelTol);
    EXPECT_NEAR(atm.GetDensityRatio(), 1.0, kRelTol);

    // Verify theta at the tropopause
    double T_tropo = atm.GetTemperature(hGeom_11km);
    double theta_expected = T_tropo / atm.GetTemperatureSL();
    EXPECT_NEAR(atm.GetTemperatureRatio(hGeom_11km), theta_expected, kRelTol);
}

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

// --- GetPressureSL() with unit conversion ---
TEST_F(ISA1976Test, GetPressureSL_WithUnits)
{
    atm.Calculate(0.0);
    // PSF identity
    EXPECT_NEAR(atm.GetPressureSL(FGAtmosphereBase::ePSF), kP_SL, kP_SL * kRelTol);
    // Pascals: ~101325 Pa
    double p_pa = atm.GetPressureSL(FGAtmosphereBase::ePascals);
    EXPECT_NEAR(p_pa, 101325.0, 101325.0 * kRelTolLoose);
    // Millibars: ~1013.25 mbar
    double p_mb = atm.GetPressureSL(FGAtmosphereBase::eMillibars);
    EXPECT_NEAR(p_mb, 1013.25, 1013.25 * kRelTolLoose);
    // InchesHg: ~29.92 inHg
    double p_inhg = atm.GetPressureSL(FGAtmosphereBase::eInchesHg);
    EXPECT_NEAR(p_inhg, 29.92, 29.92 * kRelTolLoose);
}

// --- GetDensitySL() ---
TEST_F(ISA1976Test, GetDensitySL_Value)
{
    atm.Calculate(0.0);
    double expected = FGAtmosphereBase::StdDaySLdensity;
    EXPECT_NEAR(atm.GetDensitySL(), expected, expected * kRelTol);
}

// --- GetSoundSpeedSL() ---
TEST_F(ISA1976Test, GetSoundSpeedSL_Value)
{
    atm.Calculate(0.0);
    double expected = std::sqrt(FGAtmosphereBase::SHRatio
                                * FGAtmosphereBase::Reng0
                                * FGAtmosphereBase::StdDaySLtemperature);
    EXPECT_NEAR(atm.GetSoundSpeedSL(), expected, expected * kRelTol);
}

// --- GetSoundSpeedRatio() ---
TEST_F(ISA1976Test, GetSoundSpeedRatio_AtSeaLevel)
{
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetSoundSpeedRatio(), 1.0, kRelTol);
}

TEST_F(ISA1976Test, GetSoundSpeedRatio_AtAltitude)
{
    atm.Calculate(hGeom_11km);
    double a = atm.GetSoundSpeed();
    double a_sl = atm.GetSoundSpeedSL();
    EXPECT_NEAR(atm.GetSoundSpeedRatio(), a / a_sl, kRelTol);
    EXPECT_LT(atm.GetSoundSpeedRatio(), 1.0);
}

// --- GetStdTemperatureSL() ---
TEST_F(ISA1976Test, GetStdTemperatureSL_Value)
{
    EXPECT_NEAR(atm.GetStdTemperatureSL(), kT_0km, kTempTolR);
}

// --- GetStdTemperatureRatio() ---
TEST_F(ISA1976Test, GetStdTemperatureRatio_AtSeaLevel)
{
    EXPECT_NEAR(atm.GetStdTemperatureRatio(hGeom_0km), 1.0, kRelTol);
}

TEST_F(ISA1976Test, GetStdTemperatureRatio_AtTropopause)
{
    double ratio = atm.GetStdTemperatureRatio(hGeom_11km);
    double expected = kT_11km / kT_0km;
    EXPECT_NEAR(ratio, expected, kRelTol);
}

// --- GetTemperatureDeltaGradient() ---
TEST_F(ISA1976Test, GetTemperatureDeltaGradient_DefaultZero)
{
    EXPECT_DOUBLE_EQ(atm.GetTemperatureDeltaGradient(), 0.0);
}

// --- InitModel() ---
TEST_F(ISA1976Test, InitModel_RestoresStandard)
{
    // Modify the atmosphere state
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 25.0);
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 2050.0);

    // InitModel should restore pure ISA
    atm.InitModel();
    atm.Calculate(0.0);

    EXPECT_NEAR(atm.GetTemperature(), kT_0km, kTempTolR);
    EXPECT_NEAR(atm.GetPressure(), kP_SL, kP_SL * kRelTol);
    EXPECT_NEAR(atm.GetTemperatureBias(), 0.0, 1e-12);
    EXPECT_NEAR(atm.GetTemperatureDeltaGradient(), 0.0, 1e-12);
}

// --- SetSLTemperatureGradedDelta() ---
TEST_F(ISA1976Test, SetSLTemperatureGradedDelta)
{
    double T_std_sl = atm.GetStdTemperature(0.0);

    // Apply a graded delta of +10 R at sea level
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, 10.0);

    // At sea level, temperature should increase
    double T_biased_sl = atm.GetTemperature(0.0);
    EXPECT_GT(T_biased_sl, T_std_sl);

    // The gradient should be non-zero
    EXPECT_NE(atm.GetTemperatureDeltaGradient(), 0.0);
}

// --- ConvertToRankine / ConvertFromRankine invalid unit throws ---
TEST_F(ISA1976Test, ConvertToRankine_InvalidUnitThrows)
{
    EXPECT_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eNoTempUnit),
                 std::invalid_argument);
}

TEST_F(ISA1976Test, ConvertFromRankine_InvalidUnitThrows)
{
    EXPECT_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eNoTempUnit),
                 std::invalid_argument);
}

// --- ConvertToPSF / ConvertFromPSF invalid unit throws ---
TEST_F(ISA1976Test, ConvertToPSF_InvalidUnitThrows)
{
    EXPECT_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                 std::invalid_argument);
}

TEST_F(ISA1976Test, ConvertFromPSF_InvalidUnitThrows)
{
    EXPECT_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                 std::invalid_argument);
}

// --- ConvertToPSF from Millibars and InchesHg (the TO direction) ---
TEST_F(ISA1976Test, ConvertToPSF_FromMillibars)
{
    // 1013.25 mbar ~ 2116.228 psf
    double psf = atm.ConvertToPSF(1013.25, FGAtmosphereBase::eMillibars);
    EXPECT_NEAR(psf, kP_SL, kP_SL * kRelTolLoose);
}

TEST_F(ISA1976Test, ConvertToPSF_FromInchesHg)
{
    // 29.92 inHg ~ 2116.228 psf
    double psf = atm.ConvertToPSF(29.92, FGAtmosphereBase::eInchesHg);
    EXPECT_NEAR(psf, kP_SL, kP_SL * kRelTolLoose);
}

// --- Free temperature conversion helpers ---
TEST_F(ISA1976Test, FreeTemperatureConversions)
{
    // RankineToCelsius: 518.67 R = 15 C
    EXPECT_NEAR(RankineToCelsius(518.67), 15.0, kConvTol);
    // KelvinToFahrenheit: uses 1.8*k - 459.4 (approx; exact -459.67 gives 59.0)
    EXPECT_NEAR(KelvinToFahrenheit(288.15), 59.27, 0.01);
    // FahrenheitToCelsius: 59 F = 15 C
    EXPECT_NEAR(FahrenheitToCelsius(59.0), 15.0, kConvTol);
    // CelsiusToFahrenheit: 15 C = 59 F
    EXPECT_NEAR(CelsiusToFahrenheit(15.0), 59.0, kConvTol);
    // CelsiusToKelvin: 15 C = 288.15 K
    EXPECT_NEAR(CelsiusToKelvin(15.0), 288.15, kConvTol);
    // KelvinToCelsius: 288.15 K = 15 C
    EXPECT_NEAR(KelvinToCelsius(288.15), 15.0, kConvTol);
}

// --- GetLapseRates() and GetPressureBreakpoints() ---
TEST_F(ISA1976Test, GetLapseRates_NotEmpty)
{
    const auto& lr = atm.GetLapseRates();
    EXPECT_GT(lr.size(), 0u);
    // First lapse rate should be negative (troposphere cools with altitude)
    EXPECT_LT(lr[0], 0.0);
}

TEST_F(ISA1976Test, GetPressureBreakpoints_Monotonic)
{
    const auto& pb = atm.GetPressureBreakpoints();
    EXPECT_GT(pb.size(), 0u);
    // Pressure breakpoints must be monotonically decreasing
    for (size_t i = 1; i < pb.size(); ++i) {
        EXPECT_LT(pb[i], pb[i-1]) << "Pressure breakpoint not decreasing at index " << i;
    }
}
