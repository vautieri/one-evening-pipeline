/*==============================================================================
 * TAR-SF-002 | DAL A Test Suite for Atmosphere Model
 *
 * Requirements Verified:
 *   REQ-SF-0300  ISA 1976 standard atmosphere model accuracy
 *   REQ-SF-0305  Density altitude and pressure altitude computation
 *   REQ-SF-1400  DO-178C DAL A structural coverage evidence
 *
 * Coverage Targets: 100% Statement, Decision, and MC/DC (DAL A)
 * Standard: SCS-SF-2026-001
 * Date: 2026-03-14
 *
 * Reference: "U.S. Standard Atmosphere, 1976", NASA TM-X-74335
 *
 * Modules Under Test:
 *   JSBSimExtract::FGAtmosphereBase
 *   JSBSimExtract::FGStandardAtmosphere1976
 *   JSBSimExtract::SimpleTable
 *============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <stdexcept>
#include "FGStandardAtmosphere1976.h"

using namespace JSBSimExtract;

// =============================================================================
//  Named constants (SCS-SF-2026-001: no magic numbers)
// =============================================================================

static constexpr double TEMP_TOLERANCE    = 0.05;       // 0.05 R
static constexpr double REL_TOLERANCE     = 1.0e-3;     // 0.1%
static constexpr double LOOSE_TOLERANCE   = 5.0e-3;     // 0.5%
static constexpr double TIGHT_TOLERANCE   = 1.0e-10;    // roundtrip / identity
static constexpr double CONV_TOLERANCE    = 1.0e-6;     // unit conversion arithmetic

// ISA geopotential layer boundaries [ft]
static constexpr double GEOPOT_0KM   =      0.0;
static constexpr double GEOPOT_11KM  =  36089.2388;
static constexpr double GEOPOT_20KM  =  65616.7979;
static constexpr double GEOPOT_32KM  = 104986.8766;
static constexpr double GEOPOT_47KM  = 154199.4751;
static constexpr double GEOPOT_51KM  = 167322.8346;
static constexpr double GEOPOT_71KM  = 232939.6325;
static constexpr double GEOPOT_84KM  = 278385.8268;

// ISA reference temperatures at layer boundaries [Rankine]
static constexpr double T_0KM   = 518.67;
static constexpr double T_11KM  = 389.97;
static constexpr double T_20KM  = 389.97;
static constexpr double T_32KM  = 411.57;
static constexpr double T_47KM  = 487.17;
static constexpr double T_51KM  = 487.17;
static constexpr double T_71KM  = 386.37;
static constexpr double T_84KM  = 336.5028;

// ISA standard sea-level values
static constexpr double P_SL    = 2116.228;      // psf

// ISA tropospheric lapse rate [R/ft geopotential]
static constexpr double TROPO_LAPSE_RATE = -3.5662e-03;

// Altitude constants [ft geometric]
static constexpr double ALT_5000FT     =  5000.0;
static constexpr double ALT_10000FT    = 10000.0;
static constexpr double ALT_18000FT    = 18000.0;
static constexpr double ALT_20000FT    = 20000.0;
static constexpr double ALT_25000FT    = 25000.0;
static constexpr double ALT_30000FT    = 30000.0;
static constexpr double ALT_35000FT    = 35000.0;
static constexpr double ALT_50000FT    = 50000.0;
static constexpr double ALT_NEG_1000   = -1000.0;
static constexpr double ALT_NEG_10000  = -10000.0;
static constexpr double ALT_EXTREME    = 500000.0;

// Physical / bias constants
static constexpr double MIN_TEMP_RANKINE  = 1.8;        // 1 K in Rankine
static constexpr double BIAS_SMALL        = 10.0;       // 10 R
static constexpr double BIAS_LARGE        = 25.0;       // 25 R
static constexpr double BIAS_EXTREME_NEG  = -1000.0;    // R, triggers clamp

/// Relative-error helper: |actual - expected| / |expected|
static double RelErr(double actual, double expected) {
    if (expected == 0.0) return std::fabs(actual);
    return std::fabs((actual - expected) / expected);
}

// =============================================================================
//  Test fixtures (one base, aliased per category)
// =============================================================================

class AtmosphereDalA : public ::testing::Test {
protected:
    FGStandardAtmosphere1976 atm;

    // Geometric altitudes corresponding to geopotential breakpoints
    double geom_0km,  geom_11km, geom_20km, geom_32km;
    double geom_47km, geom_51km, geom_71km, geom_84km;

    void SetUp() override {
        geom_0km  = atm.GeometricAltitude(GEOPOT_0KM);
        geom_11km = atm.GeometricAltitude(GEOPOT_11KM);
        geom_20km = atm.GeometricAltitude(GEOPOT_20KM);
        geom_32km = atm.GeometricAltitude(GEOPOT_32KM);
        geom_47km = atm.GeometricAltitude(GEOPOT_47KM);
        geom_51km = atm.GeometricAltitude(GEOPOT_51KM);
        geom_71km = atm.GeometricAltitude(GEOPOT_71KM);
        geom_84km = atm.GeometricAltitude(GEOPOT_84KM);
    }
};

// Category aliases reusing the same fixture setup
using AtmosphereDalA_MCDC        = AtmosphereDalA;
using AtmosphereDalA_Boundary    = AtmosphereDalA;
using AtmosphereDalA_Robustness  = AtmosphereDalA;
using AtmosphereDalA_ISA         = AtmosphereDalA;
using AtmosphereDalA_State       = AtmosphereDalA;
using AtmosphereDalA_SimpleTable = AtmosphereDalA;
using AtmosphereDalA_Error       = AtmosphereDalA;

// #############################################################################
//  MC/DC TESTS (25 tests)
// #############################################################################

// -----------------------------------------------------------------------------
//  D1: GetTemperature -- if (GeoPotAlt >= 0.0)
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, GetTemperature_AltAboveSL_D1True)
{
    // Verifies REQ-SF-0300 | MC/DC: D1 TRUE (GeoPotAlt >= 0)
    double T = atm.GetTemperature(ALT_10000FT);
    double T_std = atm.GetStdTemperature(ALT_10000FT);
    EXPECT_NEAR(T, T_std, TEMP_TOLERANCE);
    EXPECT_GT(T, 0.0);
}

TEST_F(AtmosphereDalA_MCDC, GetTemperature_AltBelowSL_D1False)
{
    // Verifies REQ-SF-0300 | MC/DC: D1 FALSE (GeoPotAlt < 0)
    // Below sea level: extrapolates using tropospheric lapse rate
    double T = atm.GetTemperature(ALT_NEG_1000);
    EXPECT_GT(T, T_0KM);
}

// -----------------------------------------------------------------------------
//  D2/D3: GetTemperature -- gradient fadeout decisions
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, GetTemperature_WithGradient_BelowFadeout_D2True)
{
    // Verifies REQ-SF-0300 | MC/DC: D2 TRUE -- alt within gradient region
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);
    double T = atm.GetTemperature(ALT_10000FT);
    double T_std = atm.GetStdTemperature(ALT_10000FT);
    // Temperature should differ from standard due to gradient
    EXPECT_NE(T, T_std);
}

TEST_F(AtmosphereDalA_MCDC, GetTemperature_WithGradient_AboveFadeout_D2False)
{
    // Verifies REQ-SF-0300 | MC/DC: D2 FALSE -- alt above gradient fadeout
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);
    double T = atm.GetTemperature(ALT_EXTREME);
    EXPECT_GT(T, 0.0);
}

TEST_F(AtmosphereDalA_MCDC, GetTemperature_SecondGradientCheck_D3True)
{
    // Verifies REQ-SF-0300 | MC/DC: D3 TRUE -- gradient correction applied
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);
    double T_with = atm.GetTemperature(ALT_5000FT);
    atm.ResetSLTemperature();
    double T_without = atm.GetTemperature(ALT_5000FT);
    EXPECT_NE(T_with, T_without);
}

TEST_F(AtmosphereDalA_MCDC, GetTemperature_SecondGradientCheck_D3False)
{
    // Verifies REQ-SF-0300 | MC/DC: D3 FALSE -- above fadeout, no correction
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);
    double T = atm.GetTemperature(ALT_EXTREME);
    EXPECT_GT(T, 0.0);
}

// -----------------------------------------------------------------------------
//  D4: GetStdTemperature -- if (GeoPotAlt >= 0.0)
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, GetStdTemperature_AboveSL_D4True)
{
    // Verifies REQ-SF-0300 | MC/DC: D4 TRUE
    double T = atm.GetStdTemperature(ALT_10000FT);
    EXPECT_GT(T, 0.0);
    EXPECT_LT(T, T_0KM);
}

TEST_F(AtmosphereDalA_MCDC, GetStdTemperature_BelowSL_D4False)
{
    // Verifies REQ-SF-0300 | MC/DC: D4 FALSE
    double T = atm.GetStdTemperature(ALT_NEG_1000);
    EXPECT_GT(T, T_0KM);
}

// -----------------------------------------------------------------------------
//  D5/D6: GetPressure -- loop and isothermal check
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, GetPressure_Troposphere_NonZeroLapse_D6True)
{
    // Verifies REQ-SF-0300 | MC/DC: D6 TRUE (Lmb != 0 in troposphere)
    double P = atm.GetPressure(ALT_5000FT);
    EXPECT_GT(P, 0.0);
    EXPECT_LT(P, P_SL);
}

TEST_F(AtmosphereDalA_MCDC, GetPressure_Tropopause_ZeroLapse_D6False)
{
    // Verifies REQ-SF-0300 | MC/DC: D6 FALSE (Lmb == 0, isothermal tropopause)
    double geom_mid = atm.GeometricAltitude(40000.0);
    double P = atm.GetPressure(geom_mid);
    EXPECT_GT(P, 0.0);
}

TEST_F(AtmosphereDalA_MCDC, GetPressure_Stratopause_ZeroLapse_D6FalseAlt)
{
    // Verifies REQ-SF-0300 | MC/DC: D6 FALSE (Lmb == 0, stratopause 47-51 km)
    double geom_mid = atm.GeometricAltitude(160000.0);
    double P = atm.GetPressure(geom_mid);
    EXPECT_GT(P, 0.0);
}

TEST_F(AtmosphereDalA_MCDC, GetPressure_LayerSearch_MultipleIterations_D5)
{
    // Verifies REQ-SF-0300 | MC/DC: D5 loop iterates past multiple layers
    double P = atm.GetPressure(atm.GeometricAltitude(70000.0));
    EXPECT_GT(P, 0.0);
}

// -----------------------------------------------------------------------------
//  D7: ValidatePressure -- if (p < MinPressure)
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, ValidatePressure_NormalPressure_D7False)
{
    // Verifies REQ-SF-1400 | MC/DC: D7 FALSE -- no clamp
    atm.SetPressureSL(FGAtmosphereBase::ePSF, P_SL);
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressure(), P_SL, P_SL * REL_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, ValidatePressure_ZeroPressure_D7True)
{
    // Verifies REQ-SF-1400 | MC/DC: D7 TRUE -- pressure clamped to minimum
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 0.0);
    atm.Calculate(0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
}

// -----------------------------------------------------------------------------
//  D8: ValidateTemperature -- if (t < minT)
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, ValidateTemperature_NormalTemp_D8False)
{
    // Verifies REQ-SF-1400 | MC/DC: D8 FALSE -- no clamp
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), T_0KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, ValidateTemperature_ExtremeNegBias_D8True)
{
    // Verifies REQ-SF-1400 | MC/DC: D8 TRUE -- clamped to 1.8 R
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, BIAS_EXTREME_NEG);
    atm.Calculate(geom_84km);
    EXPECT_GE(atm.GetTemperature(), MIN_TEMP_RANKINE);
}

// -----------------------------------------------------------------------------
//  D13: SetTemperatureBias -- if (unit == eCelsius || unit == eKelvin)
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_Celsius_D13TrueFirst)
{
    // Verifies REQ-SF-0300 | MC/DC: D13 TRUE via eCelsius (first condition)
    atm.SetTemperatureBias(FGAtmosphereBase::eCelsius, 5.0);
    EXPECT_NEAR(atm.GetTemperatureBias(), 9.0, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_Kelvin_D13TrueSecond)
{
    // Verifies REQ-SF-0300 | MC/DC: D13 TRUE via eKelvin (second condition)
    atm.SetTemperatureBias(FGAtmosphereBase::eKelvin, 5.0);
    EXPECT_NEAR(atm.GetTemperatureBias(), 9.0, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_Rankine_D13False)
{
    // Verifies REQ-SF-0300 | MC/DC: D13 FALSE (eRankine, no scaling)
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, 9.0);
    EXPECT_NEAR(atm.GetTemperatureBias(), 9.0, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_Fahrenheit_D13False_Alt)
{
    // Verifies REQ-SF-0300 | MC/DC: D13 FALSE (eFahrenheit, no scaling)
    atm.SetTemperatureBias(FGAtmosphereBase::eFahrenheit, 9.0);
    EXPECT_NEAR(atm.GetTemperatureBias(), 9.0, TEMP_TOLERANCE);
}

// -----------------------------------------------------------------------------
//  D14: SetTemperatureBias -- clamp to prevent sub-absolute-zero
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_ExtremeNeg_Clamped_D14True)
{
    // Verifies REQ-SF-1400 | MC/DC: D14 TRUE -- bias clamped
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, BIAS_EXTREME_NEG);
    double expected = MIN_TEMP_RANKINE - T_84KM;
    EXPECT_NEAR(atm.GetTemperatureBias(), expected, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureBias_ModerateNeg_NotClamped_D14False)
{
    // Verifies REQ-SF-1400 | MC/DC: D14 FALSE -- moderate bias, no clamp
    double moderate = -5.0;
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, moderate);
    EXPECT_NEAR(atm.GetTemperatureBias(), moderate, TEMP_TOLERANCE);
}

// -----------------------------------------------------------------------------
//  D19/D21: Density/Pressure altitude isothermal check
// -----------------------------------------------------------------------------

TEST_F(AtmosphereDalA_MCDC, DensityAltitude_NonIsothermal_D19True)
{
    // Verifies REQ-SF-0305 | MC/DC: D19 TRUE (Lmb != 0, troposphere)
    atm.Calculate(ALT_5000FT);
    EXPECT_NEAR(atm.GetDensityAltitude(), ALT_5000FT,
                ALT_5000FT * LOOSE_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, DensityAltitude_Isothermal_D19False)
{
    // Verifies REQ-SF-0305 | MC/DC: D19 FALSE (Lmb == 0, tropopause)
    double geom_40k = atm.GeometricAltitude(40000.0);
    atm.Calculate(geom_40k);
    EXPECT_NEAR(atm.GetDensityAltitude(), geom_40k,
                geom_40k * LOOSE_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, PressureAltitude_NonIsothermal_D21True)
{
    // Verifies REQ-SF-0305 | MC/DC: D21 TRUE (Lmb != 0, troposphere)
    atm.Calculate(ALT_5000FT);
    EXPECT_NEAR(atm.GetPressureAltitude(), ALT_5000FT,
                ALT_5000FT * LOOSE_TOLERANCE);
}

TEST_F(AtmosphereDalA_MCDC, PressureAltitude_Isothermal_D21False)
{
    // Verifies REQ-SF-0305 | MC/DC: D21 FALSE (Lmb == 0, tropopause)
    double geom_40k = atm.GeometricAltitude(40000.0);
    atm.Calculate(geom_40k);
    EXPECT_NEAR(atm.GetPressureAltitude(), geom_40k,
                geom_40k * LOOSE_TOLERANCE);
}

// #############################################################################
//  MC/DC: Temperature gradient delta decisions (D15, D16)
// #############################################################################

TEST_F(AtmosphereDalA_MCDC, SetTemperatureGradedDelta_Celsius_D15True)
{
    // Verifies REQ-SF-0300 | MC/DC: D15 TRUE -- eCelsius triggers *1.8 scaling
    atm.SetTemperatureGradedDelta(5.0, 0.0, FGAtmosphereBase::eCelsius);
    EXPECT_NE(atm.GetTemperatureDeltaGradient(), 0.0);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureGradedDelta_Rankine_D15False)
{
    // Verifies REQ-SF-0300 | MC/DC: D15 FALSE -- eRankine, no scaling
    atm.SetTemperatureGradedDelta(5.0, 0.0, FGAtmosphereBase::eRankine);
    EXPECT_NE(atm.GetTemperatureDeltaGradient(), 0.0);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureGradedDelta_ExtremeNeg_D16True)
{
    // Verifies REQ-SF-1400 | MC/DC: D16 TRUE -- delta clamped to minDelta
    atm.SetTemperatureGradedDelta(BIAS_EXTREME_NEG, 0.0,
                                  FGAtmosphereBase::eRankine);
    double T = atm.GetTemperature(ALT_30000FT);
    EXPECT_GT(T, 0.0);
}

TEST_F(AtmosphereDalA_MCDC, SetTemperatureGradedDelta_ModeratePos_D16False)
{
    // Verifies REQ-SF-0300 | MC/DC: D16 FALSE -- positive delta, no clamp
    atm.SetTemperatureGradedDelta(BIAS_SMALL, 0.0,
                                  FGAtmosphereBase::eRankine);
    EXPECT_NE(atm.GetTemperatureDeltaGradient(), 0.0);
}

// #############################################################################
//  BOUNDARY VALUE TESTS (18 tests)
// #############################################################################

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_0km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 0 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_0km), T_0KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_11km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 11 km (tropopause)
    EXPECT_NEAR(atm.GetStdTemperature(geom_11km), T_11KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_20km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 20 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_20km), T_20KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_32km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 32 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_32km), T_32KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_47km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 47 km (stratopause)
    EXPECT_NEAR(atm.GetStdTemperature(geom_47km), T_47KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_51km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 51 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_51km), T_51KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_71km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 71 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_71km), T_71KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, TemperatureContinuity_84km)
{
    // Verifies REQ-SF-0300 | Boundary: temperature at 84.852 km
    EXPECT_NEAR(atm.GetStdTemperature(geom_84km), T_84KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, PressureContinuity_AcrossLayerBoundaries)
{
    // Verifies REQ-SF-0300 | Boundary: pressure continuous across all layers
    double boundaries[] = { geom_11km, geom_20km, geom_32km, geom_47km,
                            geom_51km, geom_71km };
    constexpr double DELTA_FT = 1.0;
    for (double h : boundaries) {
        double P_below = atm.GetStdPressure(h - DELTA_FT);
        double P_at    = atm.GetStdPressure(h);
        double P_above = atm.GetStdPressure(h + DELTA_FT);
        EXPECT_LT(RelErr(P_below, P_at), REL_TOLERANCE)
            << "Pressure discontinuity below boundary at h=" << h;
        EXPECT_LT(RelErr(P_above, P_at), REL_TOLERANCE)
            << "Pressure discontinuity above boundary at h=" << h;
    }
}

TEST_F(AtmosphereDalA_Boundary, AltitudeExactlyZero_SeaLevel)
{
    // Verifies REQ-SF-0300 | Boundary: altitude = 0.0
    EXPECT_NEAR(atm.GetStdTemperature(0.0), T_0KM, TEMP_TOLERANCE);
    EXPECT_NEAR(atm.GetStdPressure(0.0), P_SL, P_SL * REL_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, AltitudeNegative1000ft_ExceedsSL)
{
    // Verifies REQ-SF-0300 | Boundary: below sea level
    EXPECT_GT(atm.GetStdTemperature(ALT_NEG_1000), T_0KM);
    EXPECT_GT(atm.GetStdPressure(ALT_NEG_1000), P_SL);
}

TEST_F(AtmosphereDalA_Boundary, JustBelowTropopause_StillInGradient)
{
    // Verifies REQ-SF-0300 | Boundary: 10 ft below tropopause
    double h_below = geom_11km - 10.0;
    double T = atm.GetStdTemperature(h_below);
    EXPECT_GT(T, T_11KM);
    EXPECT_LT(T, T_0KM);
}

TEST_F(AtmosphereDalA_Boundary, JustAboveTropopause_InIsothermal)
{
    // Verifies REQ-SF-0300 | Boundary: 10 ft above tropopause
    double h_above = geom_11km + 10.0;
    double T = atm.GetStdTemperature(h_above);
    EXPECT_NEAR(T, T_11KM, 0.1);
}

TEST_F(AtmosphereDalA_Boundary, GeopotentialAltitude_AtZero_ReturnsZero)
{
    // Verifies REQ-SF-0300 | Boundary: H(h=0) = 0
    EXPECT_DOUBLE_EQ(atm.GeopotentialAltitude(0.0), 0.0);
}

TEST_F(AtmosphereDalA_Boundary, GeometricAltitude_AtZero_ReturnsZero)
{
    // Verifies REQ-SF-0300 | Boundary: h(H=0) = 0
    EXPECT_DOUBLE_EQ(atm.GeometricAltitude(0.0), 0.0);
}

TEST_F(AtmosphereDalA_Boundary, GeopotentialAltitude_VeryHigh_BoundedByEarthRadius)
{
    // Verifies REQ-SF-0300 | Boundary: H < EarthRadius for any finite h
    double R_E_ft = 6356766.0 / 0.3048;
    double h = 10.0 * R_E_ft;
    double H = atm.GeopotentialAltitude(h);
    double expected = R_E_ft * 10.0 / 11.0;
    EXPECT_NEAR(H, expected, expected * TIGHT_TOLERANCE);
    EXPECT_LT(H, R_E_ft);
}

TEST_F(AtmosphereDalA_Boundary, GeometricAltitude_HalfEarthRadius_Diverges)
{
    // Verifies REQ-SF-0300 | Boundary: h(H=0.5*R_E) = R_E
    double R_E_ft = 6356766.0 / 0.3048;
    double H = 0.5 * R_E_ft;
    double h = atm.GeometricAltitude(H);
    EXPECT_NEAR(h, R_E_ft, R_E_ft * TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_Boundary, GeopotentialRoundtrip_MultipleAltitudes)
{
    // Verifies REQ-SF-0300 | Boundary: roundtrip fidelity
    double altitudes[] = { 0.0, ALT_10000FT, ALT_35000FT, 250000.0 };
    for (double h : altitudes) {
        double H = atm.GeopotentialAltitude(h);
        double h_back = atm.GeometricAltitude(H);
        EXPECT_NEAR(h_back, h, 1e-4) << "Roundtrip failed at h=" << h;
    }
}

// #############################################################################
//  ROBUSTNESS TESTS (14 tests)
// #############################################################################

TEST_F(AtmosphereDalA_Robustness, VeryNegativeAltitude_NoNaN)
{
    // Verifies REQ-SF-1400 | Robustness: -10000 ft produces valid output
    atm.Calculate(ALT_NEG_10000);
    EXPECT_FALSE(std::isnan(atm.GetTemperature()));
    EXPECT_FALSE(std::isnan(atm.GetPressure()));
    EXPECT_FALSE(std::isnan(atm.GetDensity()));
    EXPECT_GT(atm.GetTemperature(), 0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
    EXPECT_GT(atm.GetDensity(), 0.0);
}

TEST_F(AtmosphereDalA_Robustness, ExtremeAltitude500k_FinitePositive)
{
    // Verifies REQ-SF-1400 | Robustness: 500000 ft above all table entries
    atm.Calculate(ALT_EXTREME);
    EXPECT_FALSE(std::isnan(atm.GetTemperature()));
    EXPECT_FALSE(std::isnan(atm.GetPressure()));
    EXPECT_FALSE(std::isnan(atm.GetDensity()));
    EXPECT_FALSE(std::isnan(atm.GetSoundSpeed()));
    EXPECT_GT(atm.GetTemperature(), 0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
    EXPECT_GT(atm.GetDensity(), 0.0);
    EXPECT_GT(atm.GetSoundSpeed(), 0.0);
}

TEST_F(AtmosphereDalA_Robustness, PressureAtExtremeAlt_VerySmallButPositive)
{
    // Verifies REQ-SF-0300 | Robustness: pressure at 500k ft
    double P = atm.GetPressure(ALT_EXTREME);
    EXPECT_GT(P, 0.0);
    EXPECT_LT(P, P_SL * 1e-3);
}

TEST_F(AtmosphereDalA_Robustness, TemperatureAtExtremeAlt_BoundedPositive)
{
    // Verifies REQ-SF-1400 | Robustness: temperature bounded at extreme altitude
    double T = atm.GetTemperature(ALT_EXTREME);
    EXPECT_GT(T, 0.0);
    EXPECT_LT(T, 1000.0);
}

TEST_F(AtmosphereDalA_Robustness, SetTemperatureBias_EnormousNeg_Clamped)
{
    // Verifies REQ-SF-1400 | Robustness: extreme negative bias triggers clamp
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, -5000.0);
    double bias = atm.GetTemperatureBias();
    EXPECT_GE(T_84KM + bias, MIN_TEMP_RANKINE - TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, SetPressureSL_Zero_Clamped)
{
    // Verifies REQ-SF-1400 | Robustness: zero pressure clamped
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 0.0);
    atm.Calculate(0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
}

TEST_F(AtmosphereDalA_Robustness, SetPressureSL_Negative_Clamped)
{
    // Verifies REQ-SF-1400 | Robustness: negative pressure clamped
    atm.SetPressureSL(FGAtmosphereBase::ePSF, -100.0);
    atm.Calculate(0.0);
    EXPECT_GT(atm.GetPressure(), 0.0);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_CelsiusRankineCelsius)
{
    // Verifies REQ-SF-1400 | Robustness: temperature conversion roundtrip
    double T_c = 15.0;
    double T_r = atm.ConvertToRankine(T_c, FGAtmosphereBase::eCelsius);
    double T_back = atm.ConvertFromRankine(T_r, FGAtmosphereBase::eCelsius);
    EXPECT_NEAR(T_back, T_c, CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_KelvinRankineKelvin)
{
    // Verifies REQ-SF-1400 | Robustness: Kelvin roundtrip
    double T_k = 288.15;
    double T_r = atm.ConvertToRankine(T_k, FGAtmosphereBase::eKelvin);
    double T_back = atm.ConvertFromRankine(T_r, FGAtmosphereBase::eKelvin);
    EXPECT_NEAR(T_back, T_k, CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_FahrenheitRankineFahrenheit)
{
    // Verifies REQ-SF-1400 | Robustness: Fahrenheit roundtrip
    double T_f = 59.0;
    double T_r = atm.ConvertToRankine(T_f, FGAtmosphereBase::eFahrenheit);
    double T_back = atm.ConvertFromRankine(T_r, FGAtmosphereBase::eFahrenheit);
    EXPECT_NEAR(T_back, T_f, CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_PSF_Millibars_PSF)
{
    // Verifies REQ-SF-1400 | Robustness: pressure conversion roundtrip
    double P_mbar = atm.ConvertFromPSF(P_SL, FGAtmosphereBase::eMillibars);
    double P_back = atm.ConvertToPSF(P_mbar, FGAtmosphereBase::eMillibars);
    EXPECT_NEAR(P_back, P_SL, P_SL * CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_PSF_Pascals_PSF)
{
    // Verifies REQ-SF-1400 | Robustness: Pascals roundtrip
    double P_pa = atm.ConvertFromPSF(P_SL, FGAtmosphereBase::ePascals);
    double P_back = atm.ConvertToPSF(P_pa, FGAtmosphereBase::ePascals);
    EXPECT_NEAR(P_back, P_SL, P_SL * CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, ConversionRoundtrip_PSF_InchesHg_PSF)
{
    // Verifies REQ-SF-1400 | Robustness: InchesHg roundtrip
    double P_inhg = atm.ConvertFromPSF(P_SL, FGAtmosphereBase::eInchesHg);
    double P_back = atm.ConvertToPSF(P_inhg, FGAtmosphereBase::eInchesHg);
    EXPECT_NEAR(P_back, P_SL, P_SL * CONV_TOLERANCE);
}

TEST_F(AtmosphereDalA_Robustness, DensityMonotonicallyDecreases_WithAltitude)
{
    // Verifies REQ-SF-0300 | Robustness: density strictly decreases
    double prev_rho = 1e30;
    double altitudes[] = { 0.0, ALT_10000FT, ALT_20000FT, ALT_35000FT,
                           ALT_50000FT, 100000.0, 200000.0 };
    for (double h : altitudes) {
        atm.Calculate(h);
        double rho = atm.GetDensity();
        EXPECT_LT(rho, prev_rho) << "Density not decreasing at h=" << h;
        prev_rho = rho;
    }
}

// #############################################################################
//  ISA VERIFICATION TESTS (15 tests)
// #############################################################################

TEST_F(AtmosphereDalA_ISA, TroposphereLapseRate_MatchesISA)
{
    // Verifies REQ-SF-0300 | ISA: first lapse rate = -3.5662e-03 R/ft
    const auto& lr = atm.GetLapseRates();
    EXPECT_NEAR(lr[0], TROPO_LAPSE_RATE, 1.0e-5);
}

TEST_F(AtmosphereDalA_ISA, TropopauseIsothermal_ZeroLapseRate)
{
    // Verifies REQ-SF-0300 | ISA: tropopause (11-20 km) lapse = 0
    const auto& lr = atm.GetLapseRates();
    EXPECT_NEAR(lr[1], 0.0, 1.0e-8);
}

TEST_F(AtmosphereDalA_ISA, StratospherePositiveLapseRate)
{
    // Verifies REQ-SF-0300 | ISA: stratosphere (20-32 km) positive lapse
    const auto& lr = atm.GetLapseRates();
    EXPECT_GT(lr[2], 0.0);
}

TEST_F(AtmosphereDalA_ISA, StratopauseIsothermal_ZeroLapseRate)
{
    // Verifies REQ-SF-0300 | ISA: stratopause (47-51 km) lapse = 0
    const auto& lr = atm.GetLapseRates();
    EXPECT_NEAR(lr[4], 0.0, 1.0e-8);
}

TEST_F(AtmosphereDalA_ISA, TemperatureAt5000ft_MatchesLapseRate)
{
    // Verifies REQ-SF-0300 | ISA: T(5000 ft geopot) per lapse rate
    double H = 5000.0;
    double h = atm.GeometricAltitude(H);
    double T_expected = T_0KM + TROPO_LAPSE_RATE * H;
    EXPECT_NEAR(atm.GetStdTemperature(h), T_expected, 0.5);
}

TEST_F(AtmosphereDalA_ISA, TemperatureAt25000ft_MatchesLapseRate)
{
    // Verifies REQ-SF-0300 | ISA: T(25000 ft geopot) per lapse rate
    double H = 25000.0;
    double h = atm.GeometricAltitude(H);
    double T_expected = T_0KM + TROPO_LAPSE_RATE * H;
    EXPECT_NEAR(atm.GetStdTemperature(h), T_expected, 0.5);
}

TEST_F(AtmosphereDalA_ISA, TemperatureAt50000ft_InIsothermalTropopause)
{
    // Verifies REQ-SF-0300 | ISA: T ~ 389.97 R in tropopause isothermal layer
    double H = 50000.0;
    double h = atm.GeometricAltitude(H);
    EXPECT_NEAR(atm.GetStdTemperature(h), T_11KM, 1.0);
}

TEST_F(AtmosphereDalA_ISA, PressureAt18000ft_ApproxHalfSL)
{
    // Verifies REQ-SF-0300 | ISA: P(18000 ft) ~ 0.5 * P_SL
    double h = atm.GeometricAltitude(18000.0);
    double ratio = atm.GetStdPressure(h) / P_SL;
    EXPECT_NEAR(ratio, 0.5, 0.02);
}

TEST_F(AtmosphereDalA_ISA, DensityRatioAt10000ft_MatchesISA)
{
    // Verifies REQ-SF-0300 | ISA: sigma(10000 ft) ~ 0.7385
    double h = atm.GeometricAltitude(10000.0);
    double sigma = atm.GetStdDensity(h) / FGAtmosphereBase::StdDaySLdensity;
    EXPECT_NEAR(sigma, 0.7385, 0.01);
}

TEST_F(AtmosphereDalA_ISA, SoundSpeedDecreases_InTroposphere)
{
    // Verifies REQ-SF-0300 | ISA: sound speed decreases with altitude
    double a_sl = atm.GetSoundSpeed(0.0);
    double a_10k = atm.GetSoundSpeed(ALT_10000FT);
    double a_30k = atm.GetSoundSpeed(ALT_30000FT);
    EXPECT_GT(a_sl, a_10k);
    EXPECT_GT(a_10k, a_30k);
}

TEST_F(AtmosphereDalA_ISA, SoundSpeedConstant_InTropopause)
{
    // Verifies REQ-SF-0300 | ISA: constant speed of sound in isothermal layer
    double h1 = atm.GeometricAltitude(37000.0);
    double h2 = atm.GeometricAltitude(50000.0);
    double a1 = atm.GetSoundSpeed(h1);
    double a2 = atm.GetSoundSpeed(h2);
    EXPECT_NEAR(a1, a2, a1 * REL_TOLERANCE);
}

TEST_F(AtmosphereDalA_ISA, DynamicViscosityDecreases_WithAltitude)
{
    // Verifies REQ-SF-0300 | ISA: mu decreases with temperature (Sutherland)
    atm.Calculate(0.0);
    double mu_sl = atm.GetAbsoluteViscosity();
    atm.Calculate(ALT_35000FT);
    double mu_alt = atm.GetAbsoluteViscosity();
    EXPECT_GT(mu_sl, mu_alt);
}

TEST_F(AtmosphereDalA_ISA, KinematicViscosityIncreases_WithAltitude)
{
    // Verifies REQ-SF-0300 | ISA: nu increases (density drops faster than mu)
    atm.Calculate(0.0);
    double nu_sl = atm.GetKinematicViscosity();
    atm.Calculate(ALT_35000FT);
    double nu_alt = atm.GetKinematicViscosity();
    EXPECT_LT(nu_sl, nu_alt);
}

TEST_F(AtmosphereDalA_ISA, StdDensityConsistency_EquationOfState)
{
    // Verifies REQ-SF-0300 | ISA: rho = P / (R * T) at multiple altitudes
    double altitudes[] = { 0.0, ALT_10000FT, ALT_25000FT, ALT_50000FT };
    for (double h : altitudes) {
        double P = atm.GetStdPressure(h);
        double T = atm.GetStdTemperature(h);
        double rho_eq = P / (FGAtmosphereBase::Reng0 * T);
        double rho_fn = atm.GetStdDensity(h);
        EXPECT_NEAR(rho_fn, rho_eq, rho_eq * REL_TOLERANCE)
            << "P/(R*T) != GetStdDensity at h=" << h;
    }
}

TEST_F(AtmosphereDalA_ISA, PressureBreakpoints_MonotonicallyDecrease)
{
    // Verifies REQ-SF-0300 | ISA: pressure breakpoints decrease with altitude
    const auto& pb = atm.GetPressureBreakpoints();
    for (size_t i = 1; i < pb.size(); ++i) {
        EXPECT_LT(pb[i], pb[i - 1])
            << "Pressure breakpoint not decreasing at index " << i;
    }
}

// #############################################################################
//  STATE / SEQUENCE TESTS (12 tests)
// #############################################################################

TEST_F(AtmosphereDalA_State, CalculateAtSL_AllMembersConsistent)
{
    // Verifies REQ-SF-0300 | State: Calculate(0) produces consistent SL values
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), T_0KM, TEMP_TOLERANCE);
    EXPECT_NEAR(atm.GetPressure(), P_SL, P_SL * REL_TOLERANCE);
    double rho_expected = atm.GetPressure()
                          / (FGAtmosphereBase::Reng0 * atm.GetTemperature());
    EXPECT_NEAR(atm.GetDensity(), rho_expected,
                FGAtmosphereBase::StdDaySLdensity * REL_TOLERANCE);
    double a_expected = std::sqrt(FGAtmosphereBase::SHRatio
                                  * FGAtmosphereBase::Reng0
                                  * atm.GetTemperature());
    EXPECT_NEAR(atm.GetSoundSpeed(), a_expected, 1.0);
}

TEST_F(AtmosphereDalA_State, CalculateAtAlt_ThenSL_RestoredToSL)
{
    // Verifies REQ-SF-0300 | State: sequential Calculate restores cached values
    atm.Calculate(ALT_30000FT);
    EXPECT_LT(atm.GetTemperature(), T_0KM);
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetTemperature(), T_0KM, TEMP_TOLERANCE);
    EXPECT_NEAR(atm.GetPressure(), P_SL, P_SL * REL_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, SetBias_ThenReset_BiasCleared)
{
    // Verifies REQ-SF-0300 | State: ResetSLTemperature clears bias
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, BIAS_LARGE);
    EXPECT_NEAR(atm.GetTemperatureBias(), BIAS_LARGE, TEMP_TOLERANCE);
    atm.ResetSLTemperature();
    EXPECT_NEAR(atm.GetTemperatureBias(), 0.0, TIGHT_TOLERANCE);
    EXPECT_NEAR(atm.GetTemperatureDeltaGradient(), 0.0, TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, SetPressure_ThenReset_PressureRestored)
{
    // Verifies REQ-SF-0300 | State: ResetSLPressure restores standard
    double P_custom = 2050.0;
    atm.SetPressureSL(FGAtmosphereBase::ePSF, P_custom);
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressure(), P_custom, P_custom * REL_TOLERANCE);
    atm.ResetSLPressure();
    atm.Calculate(0.0);
    EXPECT_NEAR(atm.GetPressure(), P_SL, P_SL * REL_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, InitModel_RestoresAllStandard)
{
    // Verifies REQ-SF-0300 | State: InitModel restores everything
    atm.SetTemperatureBias(FGAtmosphereBase::eRankine, BIAS_LARGE);
    atm.SetPressureSL(FGAtmosphereBase::ePSF, 2050.0);
    atm.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);

    atm.InitModel();
    atm.Calculate(0.0);

    EXPECT_NEAR(atm.GetTemperature(), T_0KM, TEMP_TOLERANCE);
    EXPECT_NEAR(atm.GetPressure(), P_SL, P_SL * REL_TOLERANCE);
    EXPECT_NEAR(atm.GetTemperatureBias(), 0.0, TIGHT_TOLERANCE);
    EXPECT_NEAR(atm.GetTemperatureDeltaGradient(), 0.0, TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, SetTemperatureAt10000ft_BiasCorrect)
{
    // Verifies REQ-SF-0300 | State: SetTemperature computes correct bias
    double T_target = 480.0;
    atm.SetTemperature(T_target, ALT_10000FT, FGAtmosphereBase::eRankine);
    EXPECT_NEAR(atm.GetTemperature(ALT_10000FT), T_target, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, SetTemperatureSL_EquivalentToSetTemp0)
{
    // Verifies REQ-SF-0300 | State: SetTemperatureSL(t) == SetTemperature(t,0)
    double T_target = 530.0;
    FGStandardAtmosphere1976 a1;
    a1.SetTemperatureSL(T_target, FGAtmosphereBase::eRankine);
    a1.Calculate(0.0);

    FGStandardAtmosphere1976 a2;
    a2.SetTemperature(T_target, 0.0, FGAtmosphereBase::eRankine);
    a2.Calculate(0.0);

    EXPECT_NEAR(a1.GetTemperature(), a2.GetTemperature(), TIGHT_TOLERANCE);
    EXPECT_NEAR(a1.GetPressure(), a2.GetPressure(),
                a2.GetPressure() * TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, SetSLGradedDelta_EquivalentToGradedDelta0)
{
    // Verifies REQ-SF-0300 | State: SetSLTemperatureGradedDelta == SetTemperatureGradedDelta(t,0)
    FGStandardAtmosphere1976 a1;
    a1.SetSLTemperatureGradedDelta(FGAtmosphereBase::eRankine, BIAS_SMALL);

    FGStandardAtmosphere1976 a2;
    a2.SetTemperatureGradedDelta(BIAS_SMALL, 0.0, FGAtmosphereBase::eRankine);

    EXPECT_NEAR(a1.GetTemperatureDeltaGradient(),
                a2.GetTemperatureDeltaGradient(), TIGHT_TOLERANCE);
    EXPECT_NEAR(a1.GetTemperature(ALT_10000FT),
                a2.GetTemperature(ALT_10000FT), TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, DensityAltitude_AtISA_MatchesGeometric)
{
    // Verifies REQ-SF-0305 | State: density altitude ~ geometric under ISA
    double altitudes[] = { 0.0, ALT_10000FT, ALT_20000FT, ALT_30000FT };
    for (double h : altitudes) {
        atm.Calculate(h);
        EXPECT_NEAR(atm.GetDensityAltitude(), h,
                    std::max(h * LOOSE_TOLERANCE, 10.0))
            << "Density altitude differs at h=" << h;
    }
}

TEST_F(AtmosphereDalA_State, PressureAltitude_AtISA_MatchesGeometric)
{
    // Verifies REQ-SF-0305 | State: pressure altitude ~ geometric under ISA
    double altitudes[] = { 0.0, ALT_10000FT, ALT_20000FT, ALT_30000FT };
    for (double h : altitudes) {
        atm.Calculate(h);
        EXPECT_NEAR(atm.GetPressureAltitude(), h,
                    std::max(h * LOOSE_TOLERANCE, 10.0))
            << "Pressure altitude differs at h=" << h;
    }
}

TEST_F(AtmosphereDalA_State, GetTemperatureSL_AfterCalculateAtAltitude)
{
    // Verifies REQ-SF-0300 | State: SL temperature unchanged after Calculate at alt
    atm.Calculate(ALT_30000FT);
    EXPECT_NEAR(atm.GetTemperatureSL(), T_0KM, TEMP_TOLERANCE);
}

TEST_F(AtmosphereDalA_State, GetTemperatureRatio_AtAltitude_Consistent)
{
    // Verifies REQ-SF-0300 | State: theta = T(h) / T_SL
    atm.Calculate(ALT_20000FT);
    double theta = atm.GetTemperatureRatio();
    double expected = atm.GetTemperature() / atm.GetTemperatureSL();
    EXPECT_NEAR(theta, expected, TIGHT_TOLERANCE);
}

// #############################################################################
//  SIMPLE TABLE TESTS (9 tests)
// #############################################################################

TEST_F(AtmosphereDalA_SimpleTable, GetValue_LinearInterpolation)
{
    // Verifies REQ-SF-0300 | SimpleTable: linear interpolation
    SimpleTable t(2);
    t << 0.0 << 100.0
      << 10.0 << 200.0;
    EXPECT_NEAR(t.GetValue(5.0), 150.0, TIGHT_TOLERANCE);
}

TEST_F(AtmosphereDalA_SimpleTable, GetValue_ClampBelowTable_D23True)
{
    // Verifies REQ-SF-0300 | MC/DC: D23 TRUE (key <= keys_.front())
    SimpleTable t(2);
    t << 0.0 << 100.0
      << 10.0 << 200.0;
    EXPECT_DOUBLE_EQ(t.GetValue(-5.0), 100.0);
}

TEST_F(AtmosphereDalA_SimpleTable, GetValue_ClampAboveTable_D24True)
{
    // Verifies REQ-SF-0300 | MC/DC: D24 TRUE (key >= keys_.back())
    SimpleTable t(2);
    t << 0.0 << 100.0
      << 10.0 << 200.0;
    EXPECT_DOUBLE_EQ(t.GetValue(15.0), 200.0);
}

TEST_F(AtmosphereDalA_SimpleTable, GetValue_ExactEntry)
{
    // Verifies REQ-SF-0300 | SimpleTable: exact key match
    SimpleTable t(3);
    t << 0.0 << 100.0
      << 5.0 << 150.0
      << 10.0 << 200.0;
    EXPECT_DOUBLE_EQ(t.GetValue(5.0), 150.0);
}

TEST_F(AtmosphereDalA_SimpleTable, GetNumRows_MatchesInsertedCount)
{
    // Verifies REQ-SF-0300 | SimpleTable: row count
    SimpleTable t(3);
    t << 0.0 << 100.0
      << 5.0 << 150.0
      << 10.0 << 200.0;
    EXPECT_EQ(t.GetNumRows(), 3u);
}

TEST_F(AtmosphereDalA_SimpleTable, OperatorAccess_1Based_KeyAndValue)
{
    // Verifies REQ-SF-0300 | SimpleTable: operator()(row,col) 1-based
    SimpleTable t(2);
    t << 0.0 << 100.0
      << 10.0 << 200.0;
    EXPECT_DOUBLE_EQ(t(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(t(1, 1), 100.0);
    EXPECT_DOUBLE_EQ(t(2, 0), 10.0);
    EXPECT_DOUBLE_EQ(t(2, 1), 200.0);
}

TEST_F(AtmosphereDalA_SimpleTable, EmptyTable_GetValueReturnsZero_D22True)
{
    // Verifies REQ-SF-0300 | MC/DC: D22 TRUE (keys_.empty())
    SimpleTable t;
    EXPECT_DOUBLE_EQ(t.GetValue(42.0), 0.0);
}

TEST_F(AtmosphereDalA_SimpleTable, GetMinValue_ReturnsMinimumInTable)
{
    // Verifies REQ-SF-0300 | SimpleTable: GetMinValue
    SimpleTable t(4);
    t << 0.0 << 300.0
      << 1.0 << 100.0
      << 2.0 << 200.0
      << 3.0 << 150.0;
    EXPECT_DOUBLE_EQ(t.GetMinValue(), 100.0);
}

TEST_F(AtmosphereDalA_SimpleTable, OperatorInsert_AlternatesKeyValue_D25)
{
    // Verifies REQ-SF-0300 | MC/DC: D25 -- operator<< alternates key/value
    SimpleTable t;
    t << 1.0;   // D25 FALSE: pendingKey_ was false, push key
    t << 10.0;  // D25 TRUE:  pendingKey_ was true, push value
    EXPECT_EQ(t.GetNumRows(), 1u);
    EXPECT_DOUBLE_EQ(t(1, 0), 1.0);
    EXPECT_DOUBLE_EQ(t(1, 1), 10.0);
}

// #############################################################################
//  ERROR HANDLING TESTS (8 tests)
// #############################################################################

TEST_F(AtmosphereDalA_Error, ConvertToRankine_NoTempUnit_Throws)
{
    // Verifies REQ-SF-1400 | MC/DC: D9 default case throws
    EXPECT_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eNoTempUnit),
                 std::invalid_argument);
}

TEST_F(AtmosphereDalA_Error, ConvertFromRankine_NoTempUnit_Throws)
{
    // Verifies REQ-SF-1400 | MC/DC: D10 default case throws
    EXPECT_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eNoTempUnit),
                 std::invalid_argument);
}

TEST_F(AtmosphereDalA_Error, ConvertToPSF_NoPressUnit_Throws)
{
    // Verifies REQ-SF-1400 | MC/DC: D11 default case throws
    EXPECT_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                 std::invalid_argument);
}

TEST_F(AtmosphereDalA_Error, ConvertFromPSF_NoPressUnit_Throws)
{
    // Verifies REQ-SF-1400 | MC/DC: D12 default case throws
    EXPECT_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eNoPressUnit),
                 std::invalid_argument);
}

TEST_F(AtmosphereDalA_Error, ConvertToRankine_AllValidUnits_NoThrow)
{
    // Verifies REQ-SF-1400 | MC/DC: D9 all valid cases
    EXPECT_NO_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eFahrenheit));
    EXPECT_NO_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eCelsius));
    EXPECT_NO_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eRankine));
    EXPECT_NO_THROW(atm.ConvertToRankine(100.0, FGAtmosphereBase::eKelvin));
}

TEST_F(AtmosphereDalA_Error, ConvertFromRankine_AllValidUnits_NoThrow)
{
    // Verifies REQ-SF-1400 | MC/DC: D10 all valid cases
    EXPECT_NO_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eFahrenheit));
    EXPECT_NO_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eCelsius));
    EXPECT_NO_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eRankine));
    EXPECT_NO_THROW(atm.ConvertFromRankine(518.67, FGAtmosphereBase::eKelvin));
}

TEST_F(AtmosphereDalA_Error, ConvertToPSF_AllValidUnits_NoThrow)
{
    // Verifies REQ-SF-1400 | MC/DC: D11 all valid cases
    EXPECT_NO_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::ePSF));
    EXPECT_NO_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::eMillibars));
    EXPECT_NO_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::ePascals));
    EXPECT_NO_THROW(atm.ConvertToPSF(100.0, FGAtmosphereBase::eInchesHg));
}

TEST_F(AtmosphereDalA_Error, ConvertFromPSF_AllValidUnits_NoThrow)
{
    // Verifies REQ-SF-1400 | MC/DC: D12 all valid cases
    EXPECT_NO_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::ePSF));
    EXPECT_NO_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eMillibars));
    EXPECT_NO_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::ePascals));
    EXPECT_NO_THROW(atm.ConvertFromPSF(100.0, FGAtmosphereBase::eInchesHg));
}
