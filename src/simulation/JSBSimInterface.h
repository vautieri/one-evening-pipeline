// JSBSimInterface.h — SynthFlight wrapper around JSBSim FGFDMExec
// Implements REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration
// DAL A (Catastrophic) — DO-178C
// SCS-SF-2026-001 compliant

#ifndef SYNTHFLIGHT_JSBSIM_INTERFACE_H
#define SYNTHFLIGHT_JSBSIM_INTERFACE_H

#include <memory>
#include <string>

// Forward-declare JSBSim types to avoid exposing JSBSim headers to consumers
namespace JSBSim {
class FGFDMExec;
}

namespace SynthFlight {

/// Vehicle state vector output from JSBSim propagation.
/// All angles in radians, distances in feet, rates in rad/s.
/// REQ-SF-0200: "computing vehicle state (position, velocity, attitude, angular rates)"
struct VehicleState {
    // Position
    double latitude_rad  = 0.0;  ///< Geodetic latitude [rad]
    double longitude_rad = 0.0;  ///< Longitude [rad]
    double altitude_ft   = 0.0;  ///< Altitude ASL [ft]

    // Body-frame velocity (UVW)
    double u_fps = 0.0;  ///< Forward velocity [ft/s]
    double v_fps = 0.0;  ///< Lateral velocity [ft/s]
    double w_fps = 0.0;  ///< Vertical velocity [ft/s]

    // Euler attitude angles
    double phi_rad   = 0.0;  ///< Roll [rad]
    double theta_rad = 0.0;  ///< Pitch [rad]
    double psi_rad   = 0.0;  ///< Yaw/heading [rad]

    // Angular rates (body-frame)
    double p_radps = 0.0;  ///< Roll rate [rad/s]
    double q_radps = 0.0;  ///< Pitch rate [rad/s]
    double r_radps = 0.0;  ///< Yaw rate [rad/s]

    // Simulation time
    double sim_time_s = 0.0;  ///< Current simulation time [s]
};

/// Initial condition parameters for JSBSim.
/// Uses degree/knot units for user-facing convenience; converted internally.
struct InitialConditions {
    double latitude_deg  = 47.0;   ///< Geodetic latitude [deg]
    double longitude_deg = -122.0; ///< Longitude [deg]
    double altitude_asl_ft = 5000.0; ///< Altitude ASL [ft]
    double vcalibrated_kts = 100.0;  ///< Calibrated airspeed [kts]
    double heading_deg   = 0.0;    ///< True heading (psi) [deg]
    double pitch_deg     = 0.0;    ///< Pitch angle (theta) [deg]
    double roll_deg      = 0.0;    ///< Roll angle (phi) [deg]
};

/// SynthFlight wrapper around JSBSim's FGFDMExec.
/// Provides a simplified interface for 6-DOF flight dynamics.
/// REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration
class JSBSimInterface {
public:
    /// Integration rate limits per REQ-SF-0200: 100 Hz to 400 Hz
    static constexpr double kMinRateHz = 100.0;
    static constexpr double kMaxRateHz = 400.0;
    static constexpr double kDefaultRateHz = 200.0;

    /// Construct the interface. Does NOT initialize JSBSim yet.
    JSBSimInterface();

    /// Destructor.
    ~JSBSimInterface();

    // Non-copyable, non-movable (owns FGFDMExec)
    JSBSimInterface(const JSBSimInterface&) = delete;
    JSBSimInterface& operator=(const JSBSimInterface&) = delete;
    JSBSimInterface(JSBSimInterface&&) = delete;
    JSBSimInterface& operator=(JSBSimInterface&&) = delete;

    /// Initialize JSBSim with paths to aircraft, engine, and systems directories.
    /// @param root_dir  Path to JSBSim root (contains aircraft/, engine/, systems/)
    /// @return true if FGFDMExec initialized successfully
    bool Initialize(const std::string& root_dir);

    /// Load an aircraft model from its XML configuration.
    /// REQ-SF-0200: "The JSBSim aircraft model shall be loadable from standard
    ///              JSBSim XML configuration files."
    /// @param model_name  Aircraft directory name (e.g., "c172x")
    /// @return true if model loaded successfully
    bool LoadAircraftModel(const std::string& model_name);

    /// Set the integration rate.
    /// REQ-SF-0200: "configurable integration rate of 100 Hz to 400 Hz (default 200 Hz)"
    /// @param rate_hz  Integration rate in Hz. Must be >= 100 and <= 400.
    /// @return true if rate is within valid range and was set
    bool SetIntegrationRate(double rate_hz);

    /// Get the current integration rate in Hz.
    double GetIntegrationRate() const;

    /// Apply initial conditions and initialize the simulation state.
    /// @param ic  Initial conditions struct
    /// @return true if ICs were applied and RunIC succeeded
    bool SetInitialConditions(const InitialConditions& ic);

    /// Advance the simulation by one integration step (dt).
    /// REQ-SF-0200: "computing vehicle state ... at a configurable integration rate"
    /// Requires: Initialize() + LoadAircraftModel() + SetInitialConditions() called.
    /// @return true if the step completed successfully
    bool Step();

    /// Run the simulation for a specified number of steps.
    /// @param num_steps  Number of integration steps to execute
    /// @return true if all steps completed successfully
    bool RunSteps(int num_steps);

    /// Get the current vehicle state vector.
    /// @return VehicleState populated from JSBSim's propagation model
    VehicleState GetState() const;

    /// Get the current simulation time in seconds.
    double GetSimTime() const;

    /// Check if the interface has been initialized.
    bool IsInitialized() const;

    /// Check if an aircraft model has been loaded.
    bool IsModelLoaded() const;

    /// Check if initial conditions have been applied.
    bool IsICSet() const;

    /// Check if the simulation is ready to run (initialized, model loaded, IC set).
    bool IsReady() const;

private:
    std::unique_ptr<JSBSim::FGFDMExec> fdm_exec_;
    bool initialized_ = false;
    bool model_loaded_ = false;
    bool ic_set_ = false;
    double rate_hz_ = kDefaultRateHz;
};

} // namespace SynthFlight

#endif // SYNTHFLIGHT_JSBSIM_INTERFACE_H
