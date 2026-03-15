// JSBSimInterface.cpp — SynthFlight wrapper around JSBSim FGFDMExec
// Implements REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration
// DAL A (Catastrophic) — DO-178C
// SCS-SF-2026-001 compliant

#include "JSBSimInterface.h"

#include "FGFDMExec.h"
#include "initialization/FGInitialCondition.h"
#include "models/FGPropagate.h"
#include "models/FGAuxiliary.h"

namespace SynthFlight {

JSBSimInterface::JSBSimInterface() = default;

JSBSimInterface::~JSBSimInterface() = default;

bool JSBSimInterface::Initialize(const std::string& root_dir) {
    if (root_dir.empty()) {
        return false;
    }

    fdm_exec_ = std::make_unique<JSBSim::FGFDMExec>();
    fdm_exec_->SetRootDir(SGPath(root_dir));
    fdm_exec_->SetAircraftPath(SGPath("aircraft"));
    fdm_exec_->SetEnginePath(SGPath("engine"));
    fdm_exec_->SetSystemsPath(SGPath("systems"));

    // Set default integration rate: 200 Hz = 0.005s per REQ-SF-0200
    fdm_exec_->Setdt(1.0 / kDefaultRateHz);

    // Suppress JSBSim console output during normal operation
    fdm_exec_->SetDebugLevel(0);

    initialized_ = true;
    model_loaded_ = false;
    ic_set_ = false;
    rate_hz_ = kDefaultRateHz;

    return true;
}

bool JSBSimInterface::LoadAircraftModel(const std::string& model_name) {
    // Gate: must be initialized and model_name must not be empty
    if (!initialized_ || model_name.empty()) {
        return false;
    }

    const bool result = fdm_exec_->LoadModel(model_name);
    if (result) {
        model_loaded_ = true;
        ic_set_ = false;  // New model invalidates previous ICs
    }
    return result;
}

bool JSBSimInterface::SetIntegrationRate(double rate_hz) {
    // REQ-SF-0200: "configurable integration rate of 100 Hz to 400 Hz"
    if (rate_hz < kMinRateHz || rate_hz > kMaxRateHz) {
        return false;
    }

    rate_hz_ = rate_hz;

    if (initialized_) {
        fdm_exec_->Setdt(1.0 / rate_hz_);
    }

    return true;
}

double JSBSimInterface::GetIntegrationRate() const {
    return rate_hz_;
}

bool JSBSimInterface::SetInitialConditions(const InitialConditions& ic) {
    // Gate: must be initialized AND model loaded
    if (!initialized_ || !model_loaded_) {
        return false;
    }

    auto fgic = fdm_exec_->GetIC();
    if (!fgic) {
        return false;
    }

    fgic->SetLatitudeDegIC(ic.latitude_deg);
    fgic->SetLongitudeDegIC(ic.longitude_deg);
    fgic->SetAltitudeASLFtIC(ic.altitude_asl_ft);
    fgic->SetVcalibratedKtsIC(ic.vcalibrated_kts);
    fgic->SetPsiDegIC(ic.heading_deg);
    fgic->SetThetaDegIC(ic.pitch_deg);
    fgic->SetPhiDegIC(ic.roll_deg);

    const bool result = fdm_exec_->RunIC();
    if (result) {
        ic_set_ = true;
    }
    return result;
}

bool JSBSimInterface::Step() {
    // Gate: must be initialized AND model loaded AND IC set
    if (!initialized_ || !model_loaded_ || !ic_set_) {
        return false;
    }

    return fdm_exec_->Run();
}

bool JSBSimInterface::RunSteps(int num_steps) {
    if (num_steps <= 0) {
        return false;
    }

    for (int i = 0; i < num_steps; ++i) {
        if (!Step()) {
            return false;
        }
    }
    return true;
}

VehicleState JSBSimInterface::GetState() const {
    VehicleState state;

    if (!initialized_ || !model_loaded_) {
        return state;  // Return zeroed state
    }

    auto propagate = fdm_exec_->GetPropagate();
    if (!propagate) {
        return state;
    }

    // Position
    state.latitude_rad  = propagate->GetLatitude();
    state.longitude_rad = propagate->GetLongitude();
    state.altitude_ft   = propagate->GetAltitudeASL();

    // Body-frame velocity (UVW) — 1-based indexing per JSBSim convention
    state.u_fps = propagate->GetUVW(1);
    state.v_fps = propagate->GetUVW(2);
    state.w_fps = propagate->GetUVW(3);

    // Euler attitude angles — 1-based: phi=1, theta=2, psi=3
    state.phi_rad   = propagate->GetEuler(1);
    state.theta_rad = propagate->GetEuler(2);
    state.psi_rad   = propagate->GetEuler(3);

    // Angular rates — 1-based: p=1, q=2, r=3
    state.p_radps = propagate->GetPQR(1);
    state.q_radps = propagate->GetPQR(2);
    state.r_radps = propagate->GetPQR(3);

    // Simulation time
    state.sim_time_s = fdm_exec_->GetSimTime();

    return state;
}

double JSBSimInterface::GetSimTime() const {
    if (!initialized_) {
        return 0.0;
    }
    return fdm_exec_->GetSimTime();
}

bool JSBSimInterface::IsInitialized() const {
    return initialized_;
}

bool JSBSimInterface::IsModelLoaded() const {
    return model_loaded_;
}

bool JSBSimInterface::IsICSet() const {
    return ic_set_;
}

bool JSBSimInterface::IsReady() const {
    return initialized_ && model_loaded_ && ic_set_;
}

} // namespace SynthFlight
