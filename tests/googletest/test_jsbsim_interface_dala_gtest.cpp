// test_jsbsim_interface_dala_gtest.cpp — DAL A verification tests for JSBSimInterface
// TAR-SF-005: JSBSim 6-DOF API Wrapper
// REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration
// DAL A (Catastrophic) — DO-178C
// Test Framework: Google Test v1.15.2

#include <gtest/gtest.h>
#include "JSBSimInterface.h"
#include <cmath>
#include <string>

// ─── Test Constants ─────────────────────────────────────────────────────────
// Named constants per SCS-SF-2026-001 (no magic numbers)

namespace {

// JSBSim root directory (relative to build directory)
const std::string kJSBSimRootDir = JSBSIM_ROOT_DIR;

// Reference aircraft model
const std::string kReferenceAircraft = "c172x";

// Integration rate limits per REQ-SF-0200
constexpr double kMinRateHz     = 100.0;
constexpr double kMaxRateHz     = 400.0;
constexpr double kDefaultRateHz = 200.0;

// Boundary values just outside valid range
constexpr double kBelowMinRateHz = 99.0;
constexpr double kAboveMaxRateHz = 401.0;

// Default initial conditions for level flight
constexpr double kDefaultLatDeg   = 47.0;
constexpr double kDefaultLonDeg   = -122.0;
constexpr double kDefaultAltFt    = 5000.0;
constexpr double kDefaultVcasKts  = 100.0;
constexpr double kDefaultHdgDeg   = 0.0;
constexpr double kDefaultPitchDeg = 0.0;
constexpr double kDefaultRollDeg  = 0.0;

// Tolerances for floating-point comparison
constexpr double kPositionTolRad  = 1.0e-3;   // ~60m at equator
constexpr double kAltitudeTolFt   = 100.0;     // Altitude tolerance
constexpr double kVelocityTolFps  = 50.0;      // Velocity tolerance
constexpr double kAngleTolRad     = 0.5;       // Attitude tolerance
constexpr double kTimeTolSec      = 1.0e-9;    // Time comparison tolerance
constexpr double kRateTolHz       = 1.0e-9;    // Rate comparison tolerance

// Conversion factors
constexpr double kDegToRad = M_PI / 180.0;

// Number of steps for multi-step tests
constexpr int kTenSteps     = 10;
constexpr int kHundredSteps = 100;

} // anonymous namespace

// ─── Test Fixture ───────────────────────────────────────────────────────────

class JSBSimInterfaceDalA : public ::testing::Test {
protected:
    SynthFlight::JSBSimInterface iface;

    /// Helper: fully initialize the interface with default settings
    void SetUpReady() {
        ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
        ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));
        SynthFlight::InitialConditions ic;
        ic.latitude_deg    = kDefaultLatDeg;
        ic.longitude_deg   = kDefaultLonDeg;
        ic.altitude_asl_ft = kDefaultAltFt;
        ic.vcalibrated_kts = kDefaultVcasKts;
        ic.heading_deg     = kDefaultHdgDeg;
        ic.pitch_deg       = kDefaultPitchDeg;
        ic.roll_deg        = kDefaultRollDeg;
        ASSERT_TRUE(iface.SetInitialConditions(ic));
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// NORMAL RANGE TESTS
// ═══════════════════════════════════════════════════════════════════════════

// Verifies REQ-SF-0200 — Initialize succeeds with valid root directory
TEST_F(JSBSimInterfaceDalA, Initialize_ValidRootDir_Succeeds) {
    EXPECT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_TRUE(iface.IsInitialized());
}

// Verifies REQ-SF-0200 — Load C172X model from XML configuration
TEST_F(JSBSimInterfaceDalA, LoadAircraftModel_C172X_Succeeds) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));
    EXPECT_TRUE(iface.IsModelLoaded());
}

// Verifies REQ-SF-0200 — Default integration rate is 200 Hz
TEST_F(JSBSimInterfaceDalA, DefaultRate_Is200Hz) {
    EXPECT_NEAR(iface.GetIntegrationRate(), kDefaultRateHz, kRateTolHz);
}

// Verifies REQ-SF-0200 — Set and apply initial conditions
TEST_F(JSBSimInterfaceDalA, SetInitialConditions_ValidIC_Succeeds) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));

    SynthFlight::InitialConditions ic;
    ic.latitude_deg    = kDefaultLatDeg;
    ic.longitude_deg   = kDefaultLonDeg;
    ic.altitude_asl_ft = kDefaultAltFt;
    ic.vcalibrated_kts = kDefaultVcasKts;
    EXPECT_TRUE(iface.SetInitialConditions(ic));
    EXPECT_TRUE(iface.IsICSet());
}

// Verifies REQ-SF-0200 — Single step advances simulation time
TEST_F(JSBSimInterfaceDalA, Step_SingleFrame_AdvancesSimTime) {
    SetUpReady();
    const double time_before = iface.GetSimTime();
    ASSERT_TRUE(iface.Step());
    const double time_after = iface.GetSimTime();
    const double expected_dt = 1.0 / kDefaultRateHz;
    EXPECT_NEAR(time_after - time_before, expected_dt, kTimeTolSec);
}

// Verifies REQ-SF-0200 — State vector populated after stepping
TEST_F(JSBSimInterfaceDalA, GetState_AfterStep_HasNonZeroValues) {
    SetUpReady();
    ASSERT_TRUE(iface.Step());

    const auto state = iface.GetState();
    // Latitude should be near initial value
    EXPECT_NEAR(state.latitude_rad, kDefaultLatDeg * kDegToRad, kPositionTolRad);
    // Longitude should be near initial value
    EXPECT_NEAR(state.longitude_rad, kDefaultLonDeg * kDegToRad, kPositionTolRad);
    // Altitude should be near initial value
    EXPECT_NEAR(state.altitude_ft, kDefaultAltFt, kAltitudeTolFt);
    // Sim time should be one dt
    EXPECT_NEAR(state.sim_time_s, 1.0 / kDefaultRateHz, kTimeTolSec);
}

// Verifies REQ-SF-0200 — RunSteps executes multiple frames
TEST_F(JSBSimInterfaceDalA, RunSteps_TenFrames_CorrectSimTime) {
    SetUpReady();
    ASSERT_TRUE(iface.RunSteps(kTenSteps));
    const double expected_time = kTenSteps * (1.0 / kDefaultRateHz);
    EXPECT_NEAR(iface.GetSimTime(), expected_time, kTimeTolSec);
}

// Verifies REQ-SF-0200 — IsReady returns true when fully configured
TEST_F(JSBSimInterfaceDalA, IsReady_FullyConfigured_ReturnsTrue) {
    SetUpReady();
    EXPECT_TRUE(iface.IsReady());
}

// ═══════════════════════════════════════════════════════════════════════════
// BOUNDARY VALUE TESTS
// ═══════════════════════════════════════════════════════════════════════════

// Verifies REQ-SF-0200 — Rate at minimum (100 Hz) accepted
TEST_F(JSBSimInterfaceDalA, SetRate_AtMinimum100Hz_Succeeds) {
    EXPECT_TRUE(iface.SetIntegrationRate(kMinRateHz));
    EXPECT_NEAR(iface.GetIntegrationRate(), kMinRateHz, kRateTolHz);
}

// Verifies REQ-SF-0200 — Rate at maximum (400 Hz) accepted
TEST_F(JSBSimInterfaceDalA, SetRate_AtMaximum400Hz_Succeeds) {
    EXPECT_TRUE(iface.SetIntegrationRate(kMaxRateHz));
    EXPECT_NEAR(iface.GetIntegrationRate(), kMaxRateHz, kRateTolHz);
}

// Verifies REQ-SF-0200 — Rate just below minimum (99 Hz) rejected
TEST_F(JSBSimInterfaceDalA, SetRate_BelowMinimum99Hz_Fails) {
    EXPECT_FALSE(iface.SetIntegrationRate(kBelowMinRateHz));
    // Rate should remain at default since set failed
    EXPECT_NEAR(iface.GetIntegrationRate(), kDefaultRateHz, kRateTolHz);
}

// Verifies REQ-SF-0200 — Rate just above maximum (401 Hz) rejected
TEST_F(JSBSimInterfaceDalA, SetRate_AboveMaximum401Hz_Fails) {
    EXPECT_FALSE(iface.SetIntegrationRate(kAboveMaxRateHz));
    EXPECT_NEAR(iface.GetIntegrationRate(), kDefaultRateHz, kRateTolHz);
}

// Verifies REQ-SF-0200 — Step at 100 Hz produces correct dt
TEST_F(JSBSimInterfaceDalA, Step_At100Hz_CorrectDeltaT) {
    SetUpReady();
    // Set rate AFTER initialization (Initialize resets rate to default)
    ASSERT_TRUE(iface.SetIntegrationRate(kMinRateHz));
    const double time_before = iface.GetSimTime();
    ASSERT_TRUE(iface.Step());
    const double expected_dt = 1.0 / kMinRateHz;  // 0.01s
    EXPECT_NEAR(iface.GetSimTime() - time_before, expected_dt, kTimeTolSec);
}

// Verifies REQ-SF-0200 — Step at 400 Hz produces correct dt
TEST_F(JSBSimInterfaceDalA, Step_At400Hz_CorrectDeltaT) {
    SetUpReady();
    // Set rate AFTER initialization (Initialize resets rate to default)
    ASSERT_TRUE(iface.SetIntegrationRate(kMaxRateHz));
    const double time_before = iface.GetSimTime();
    ASSERT_TRUE(iface.Step());
    const double expected_dt = 1.0 / kMaxRateHz;  // 0.0025s
    EXPECT_NEAR(iface.GetSimTime() - time_before, expected_dt, kTimeTolSec);
}

// Verifies REQ-SF-0200 — Zero altitude IC
TEST_F(JSBSimInterfaceDalA, SetIC_ZeroAltitude_Succeeds) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));

    SynthFlight::InitialConditions ic;
    ic.altitude_asl_ft = 0.0;
    ic.vcalibrated_kts = 0.0;  // On ground
    EXPECT_TRUE(iface.SetInitialConditions(ic));
}

// Verifies REQ-SF-0200 — High altitude IC (10000 ft)
TEST_F(JSBSimInterfaceDalA, SetIC_HighAltitude10000ft_Succeeds) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));

    SynthFlight::InitialConditions ic;
    ic.altitude_asl_ft = 10000.0;
    ic.vcalibrated_kts = kDefaultVcasKts;
    EXPECT_TRUE(iface.SetInitialConditions(ic));

    const auto state = iface.GetState();
    EXPECT_NEAR(state.altitude_ft, 10000.0, kAltitudeTolFt);
}

// ═══════════════════════════════════════════════════════════════════════════
// ROBUSTNESS TESTS
// ═══════════════════════════════════════════════════════════════════════════

// Verifies REQ-SF-0200 — Initialize with empty path fails
TEST_F(JSBSimInterfaceDalA, Initialize_EmptyPath_Fails) {
    EXPECT_FALSE(iface.Initialize(""));
    EXPECT_FALSE(iface.IsInitialized());
}

// Verifies REQ-SF-0200 — Load nonexistent aircraft model fails
TEST_F(JSBSimInterfaceDalA, LoadModel_NonexistentAircraft_Fails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_FALSE(iface.LoadAircraftModel("nonexistent_aircraft_xyz"));
    EXPECT_FALSE(iface.IsModelLoaded());
}

// Verifies REQ-SF-0200 — Load empty model name fails
TEST_F(JSBSimInterfaceDalA, LoadModel_EmptyName_Fails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_FALSE(iface.LoadAircraftModel(""));
    EXPECT_FALSE(iface.IsModelLoaded());
}

// Verifies REQ-SF-0200 — GetState before initialization returns zeroed state
TEST_F(JSBSimInterfaceDalA, GetState_BeforeInit_ReturnsZeroState) {
    const auto state = iface.GetState();
    EXPECT_NEAR(state.latitude_rad, 0.0, kTimeTolSec);
    EXPECT_NEAR(state.longitude_rad, 0.0, kTimeTolSec);
    EXPECT_NEAR(state.altitude_ft, 0.0, kTimeTolSec);
    EXPECT_NEAR(state.sim_time_s, 0.0, kTimeTolSec);
}

// Verifies REQ-SF-0200 — Double initialization succeeds (resets state)
TEST_F(JSBSimInterfaceDalA, Initialize_CalledTwice_ResetsState) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));
    EXPECT_TRUE(iface.IsModelLoaded());

    // Re-initialize should reset model_loaded
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_FALSE(iface.IsModelLoaded());
    EXPECT_FALSE(iface.IsICSet());
}

// Verifies REQ-SF-0200 — RunSteps with zero steps fails
TEST_F(JSBSimInterfaceDalA, RunSteps_ZeroSteps_Fails) {
    SetUpReady();
    EXPECT_FALSE(iface.RunSteps(0));
}

// Verifies REQ-SF-0200 — RunSteps with negative steps fails
TEST_F(JSBSimInterfaceDalA, RunSteps_NegativeSteps_Fails) {
    SetUpReady();
    EXPECT_FALSE(iface.RunSteps(-1));
}

// ═══════════════════════════════════════════════════════════════════════════
// ERROR HANDLING TESTS
// ═══════════════════════════════════════════════════════════════════════════

// Verifies REQ-SF-0200 — Step before LoadModel fails
TEST_F(JSBSimInterfaceDalA, Step_BeforeLoadModel_Fails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    EXPECT_FALSE(iface.Step());
}

// Verifies REQ-SF-0200 — Step before SetIC fails
TEST_F(JSBSimInterfaceDalA, Step_BeforeSetIC_Fails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));
    EXPECT_FALSE(iface.Step());
}

// Verifies REQ-SF-0200 — LoadModel before Initialize fails
TEST_F(JSBSimInterfaceDalA, LoadModel_BeforeInit_Fails) {
    EXPECT_FALSE(iface.LoadAircraftModel(kReferenceAircraft));
    EXPECT_FALSE(iface.IsModelLoaded());
}

// Verifies REQ-SF-0200 — SetIC before LoadModel fails
TEST_F(JSBSimInterfaceDalA, SetIC_BeforeLoadModel_Fails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    SynthFlight::InitialConditions ic;
    EXPECT_FALSE(iface.SetInitialConditions(ic));
    EXPECT_FALSE(iface.IsICSet());
}

// Verifies REQ-SF-0200 — SetRate with zero Hz fails
TEST_F(JSBSimInterfaceDalA, SetRate_ZeroHz_Fails) {
    EXPECT_FALSE(iface.SetIntegrationRate(0.0));
}

// Verifies REQ-SF-0200 — SetRate with negative Hz fails
TEST_F(JSBSimInterfaceDalA, SetRate_NegativeHz_Fails) {
    EXPECT_FALSE(iface.SetIntegrationRate(-100.0));
}

// ═══════════════════════════════════════════════════════════════════════════
// MC/DC TESTS — DAL A required
// ═══════════════════════════════════════════════════════════════════════════

// Decision D1: rate_hz >= kMinRateHz && rate_hz <= kMaxRateHz (SetIntegrationRate)
// Conditions: A = (rate_hz >= 100), B = (rate_hz <= 400)
// MC/DC requires 3 test cases showing each condition independently affects outcome:
//   (A=T, B=T) → true   : rate=200  (both true)
//   (A=F, B=T) → false  : rate=50   (A alone flips outcome)
//   (A=T, B=F) → false  : rate=500  (B alone flips outcome)

// Verifies REQ-SF-0200 — MC/DC D1: A=T, B=T → accepted (rate=200)
TEST_F(JSBSimInterfaceDalA, MCDC_D1_RateValid_BothTrue_Accepted) {
    // A: 200 >= 100 = TRUE, B: 200 <= 400 = TRUE → outcome: TRUE
    EXPECT_TRUE(iface.SetIntegrationRate(200.0));
}

// Verifies REQ-SF-0200 — MC/DC D1: A=F, B=T → rejected (rate=50)
TEST_F(JSBSimInterfaceDalA, MCDC_D1_RateTooLow_AFlips_Rejected) {
    // A: 50 >= 100 = FALSE, B: 50 <= 400 = TRUE → outcome: FALSE
    // A independently caused the change (compare with A=T,B=T case above)
    EXPECT_FALSE(iface.SetIntegrationRate(50.0));
}

// Verifies REQ-SF-0200 — MC/DC D1: A=T, B=F → rejected (rate=500)
TEST_F(JSBSimInterfaceDalA, MCDC_D1_RateTooHigh_BFlips_Rejected) {
    // A: 500 >= 100 = TRUE, B: 500 <= 400 = FALSE → outcome: FALSE
    // B independently caused the change (compare with A=T,B=T case above)
    EXPECT_FALSE(iface.SetIntegrationRate(500.0));
}

// Decision D2: initialized_ && model_loaded_ && ic_set_ (IsReady / Step gate)
// 3-condition AND: A = initialized_, B = model_loaded_, C = ic_set_
// MC/DC requires 4 test cases:
//   (T,T,T) → true
//   (F,T,T) → false  (A independently flips)
//   (T,F,T) → false  (B independently flips)
//   (T,T,F) → false  (C independently flips)

// Verifies REQ-SF-0200 — MC/DC D2: A=T,B=T,C=T → Step succeeds
TEST_F(JSBSimInterfaceDalA, MCDC_D2_AllSet_StepSucceeds) {
    SetUpReady();
    // initialized_=T, model_loaded_=T, ic_set_=T → Step returns TRUE
    EXPECT_TRUE(iface.Step());
}

// Verifies REQ-SF-0200 — MC/DC D2: A=F,B=T,C=T → Step fails (not initialized)
TEST_F(JSBSimInterfaceDalA, MCDC_D2_NotInitialized_StepFails) {
    // initialized_=F (never called Initialize) → Step returns FALSE
    // Cannot have model_loaded=T or ic_set=T without init, so this tests A=F
    EXPECT_FALSE(iface.Step());
    EXPECT_FALSE(iface.IsReady());
}

// Verifies REQ-SF-0200 — MC/DC D2: A=T,B=F,C=T → Step fails (no model)
// Note: Can't have ic_set=T without model loaded (SetIC checks model_loaded_)
// So this tests A=T, B=F which implies C=F → Step fails
TEST_F(JSBSimInterfaceDalA, MCDC_D2_NoModel_StepFails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    // initialized_=T, model_loaded_=F → Step returns FALSE
    EXPECT_FALSE(iface.Step());
    EXPECT_FALSE(iface.IsReady());
}

// Verifies REQ-SF-0200 — MC/DC D2: A=T,B=T,C=F → Step fails (no IC)
TEST_F(JSBSimInterfaceDalA, MCDC_D2_NoIC_StepFails) {
    ASSERT_TRUE(iface.Initialize(kJSBSimRootDir));
    ASSERT_TRUE(iface.LoadAircraftModel(kReferenceAircraft));
    // initialized_=T, model_loaded_=T, ic_set_=F → Step returns FALSE
    EXPECT_FALSE(iface.Step());
    EXPECT_FALSE(iface.IsReady());
}

// ═══════════════════════════════════════════════════════════════════════════
// STATE VECTOR VERIFICATION TESTS
// ═══════════════════════════════════════════════════════════════════════════

// Verifies REQ-SF-0200 — Body-frame velocities present after stepping
TEST_F(JSBSimInterfaceDalA, GetState_VelocityUVW_ReasonableValues) {
    SetUpReady();
    ASSERT_TRUE(iface.RunSteps(kTenSteps));

    const auto state = iface.GetState();
    // With 100 kts VCAS, u_fps should be in a reasonable range
    // 100 kts ≈ 168.8 fps
    constexpr double kExpectedUFps = 168.8;
    EXPECT_NEAR(state.u_fps, kExpectedUFps, kVelocityTolFps);
}

// Verifies REQ-SF-0200 — Euler angles present after stepping
TEST_F(JSBSimInterfaceDalA, GetState_EulerAngles_NearLevelFlight) {
    SetUpReady();
    ASSERT_TRUE(iface.RunSteps(kTenSteps));

    const auto state = iface.GetState();
    // For level flight initialization, phi and theta should be small
    EXPECT_NEAR(state.phi_rad, 0.0, kAngleTolRad);
    EXPECT_NEAR(state.theta_rad, 0.0, kAngleTolRad);
}

// Verifies REQ-SF-0200 — Simulation time accumulates correctly over 100 steps
TEST_F(JSBSimInterfaceDalA, RunSteps_100Frames_TimeAccumulates) {
    SetUpReady();
    ASSERT_TRUE(iface.RunSteps(kHundredSteps));
    const double expected_time = kHundredSteps * (1.0 / kDefaultRateHz);
    EXPECT_NEAR(iface.GetSimTime(), expected_time, kTimeTolSec);
}
