# TAR-SF-004: JSBSim Submodule Integration

| Field          | Value                                              |
|----------------|----------------------------------------------------|
| TAR Number     | TAR-SF-004                                         |
| Title          | JSBSim Submodule Integration                       |
| Date Issued    | 2026-03-15                                         |
| Status         | Implementation Complete                            |
| Priority       | Critical (JSBSim is the primary 6-DOF engine)      |
| Assigned To    | Development Team (LLM-Assisted)                    |
| DAL            | A (Catastrophic) — DO-178C                         |
| WBS            | WBS 8.x — Simulation Engine                        |
| Product        | SynthFlight — UAV Mission Simulation and Verification Platform |
| Coding Standard| C++17 (2023), GCC (SCS-SF-2026-001)               |
| Author         | LLM-Assisted (Claude Opus 4.6)                    |

---

## 1. Objective

Integrate JSBSim as a git submodule into the SynthFlight repository, establishing the controlled baseline required for the Previously Developed Software (PDS) assessment per DO-178C Section 12.3 and SVP-SF-2026-001 Section 10.1. This TAR establishes the JSBSim source under configuration management; subsequent TARs will implement the 6-DOF API wrapper, XML model loading, and DAL A verification.

---

## 2. Scope

### 2.1 In Scope

| # | Task | Deliverable |
|---|------|-------------|
| 1 | Add JSBSim as git submodule at `extern/jsbsim` | `.gitmodules`, `extern/jsbsim/` |
| 2 | Pin to release tag v1.2.4 (commit `e07a7d81`) | Submodule checkout |
| 3 | Integrate into CMake build system | `CMakeLists.txt` update |
| 4 | Update SCMP Section 8.3 dependency table | `docs/plans/SCMP-SynthFlight.md` |
| 5 | Verify build succeeds with JSBSim included | Build output |
| 6 | Verify all existing tests pass (no regression) | Test output |

### 2.2 Out of Scope (Future TARs)

- REQ-SF-0201 through REQ-SF-0208 (Simulation Engine features)
- JSBSim PDS assessment (service history, gap analysis)
- JSBSim DAL A verification (MC/DC, structural coverage)
- SynthFlight-to-JSBSim API wrapper code
- JSBSim XML aircraft model loading

---

## 3. Requirements Traceability

| Requirement ID | Description | Coverage in This TAR |
|----------------|-------------|---------------------|
| REQ-SF-0200 | JSBSim 6-DOF Flight Dynamics Integration | Partial — establishes JSBSim baseline under CM; full verification deferred to subsequent TARs |

---

## 4. JSBSim Baseline

| Field | Value |
|-------|-------|
| Source | https://github.com/JSBSim-Team/jsbsim.git |
| Version | v1.2.4 |
| Commit | e07a7d810d4f1048039d42545d008b8a998636fa |
| Integration | Git submodule at `extern/jsbsim` |
| CMake Target | `libJSBSim` (static library) |
| License | LGPL-2.1 |

---

## 5. Changes Made

### 5.1 `.gitmodules` (new file)
Defines the JSBSim submodule at `extern/jsbsim` pointing to the upstream repository.

### 5.2 `CMakeLists.txt` (modified)
Added JSBSim via `add_subdirectory(extern/jsbsim)` with `BUILD_DOCS=OFF` and `BUILD_SHARED_LIBS=OFF`.

### 5.3 `docs/plans/SCMP-SynthFlight.md` (modified)
Updated Section 8.3 dependency table: JSBSim changed from "FetchContent with pinned Git tag or commit hash" to "Git submodule pinned to tag v1.2.4 (commit e07a7d81) at extern/jsbsim".

---

## 6. Verification Results

| Check | Result |
|-------|--------|
| CMake configure | Success |
| Build (all targets) | Success — `libJSBSim` and `JSBSim` targets built |
| Existing tests | 648/648 passed (557 SynthFlight + 91 JSBSim) |
| Regression | None — zero failures |

---

## 7. Acceptance Criteria

| # | Criterion | Status |
|---|-----------|--------|
| 1 | JSBSim submodule added at `extern/jsbsim` | Complete |
| 2 | Pinned to specific tag (v1.2.4) | Complete |
| 3 | Build succeeds with JSBSim included | Complete |
| 4 | All existing tests pass | Complete (648/648) |
| 5 | SCMP updated to reflect submodule approach | Complete |
| 6 | Human review completed | Pending |

---

## 8. Revision History

| Rev | Date       | Author                       | Description              |
|-----|------------|------------------------------|--------------------------|
| 1.0 | 2026-03-15 | LLM-Assisted (Claude Opus 4.6) | Initial TAR creation  |
