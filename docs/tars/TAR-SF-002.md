# TAR-SF-002: Atmosphere Model Verification & DAL A Coverage Enhancement

| Field          | Value                                              |
|----------------|----------------------------------------------------|
| TAR Number     | TAR-SF-002                                         |
| Title          | Atmosphere Model Verification & DAL A Coverage Enhancement |
| Date Issued    | 2026-03-14                                         |
| Status         | Open                                               |
| Priority       | Critical (atmosphere model feeds all aerodynamic computations) |
| Assigned To    | Development Team (LLM-Assisted)                    |
| DAL            | A (Catastrophic) — DO-178C                         |
| WBS            | 2.0 — Atmosphere Model                             |
| Product        | SynthFlight — UAV Mission Simulation and Verification Platform |
| Coding Standard| C++17 (2023), GCC (SCS-SF-2026-001)               |

---

## 1. Objective

Achieve DAL A structural coverage (Statement, Decision, MC/DC) for the three atmosphere modules — **FGAtmosphereBase** (abstract base class), **FGStandardAtmosphere1976** (ISA 1976 derived class), and **SimpleTable** (1-D interpolation utility) — and generate requirements-based tests for all ~40+ public API functions. These modules sit directly above the math library in the integration stack (math -> atmosphere -> aerodynamics -> flight dynamics); all environmental computations depend on the correctness verified here.

Specific objectives:

- Achieve 100% Statement Coverage (SC), 100% Decision Coverage (DC), and 100% Modified Condition/Decision Coverage (MC/DC) for all three modules.
- Generate requirements-based test cases for every public API function, including unit conversion utilities, ISA temperature/pressure/density calculations, and table interpolation.
- Establish verification evidence per SVP-SF-2026-001.
- Produce traceability from SRS requirements through test cases to coverage results.

---

## 2. Scope

### 2.1 Source Files Under Test

| # | File                                        | Role                    |
|---|---------------------------------------------|-------------------------|
| 1 | `src/models/FGAtmosphereBase.h`             | Base class (header-only)|
| 2 | `src/models/FGStandardAtmosphere1976.h`     | ISA 1976 header         |
| 3 | `src/models/FGStandardAtmosphere1976.cpp`   | ISA 1976 implementation |

### 2.2 Modules Under Test

#### SimpleTable — 1-D lookup table with linear interpolation (namespace JSBSimExtract)

6 public API functions:

| #  | Function / Operator                       | Category       |
|----|-------------------------------------------|----------------|
| 1  | Constructor (reserve rows)                | Construction   |
| 2  | `operator<<` (stream insertion of data)   | Data Entry     |
| 3  | `GetValue()` (1-D interpolation)          | Interpolation  |
| 4  | `operator()` (row/col access)             | Access         |
| 5  | `GetNumRows()`                            | Query          |
| 6  | `GetMinValue()`                           | Query          |

#### FGAtmosphereBase — abstract base class for atmosphere models (namespace JSBSimExtract)

22 public API functions:

| #  | Function / Operator                                | Category         |
|----|----------------------------------------------------|------------------|
| 1  | Default constructor                                | Construction     |
| 2  | `GetTemperature()` (no args)                       | Temperature      |
| 3  | `GetTemperature(double)` (pure virtual)            | Temperature      |
| 4  | `GetTemperatureSL()`                               | Temperature      |
| 5  | `GetTemperatureRatio()` (no args)                  | Temperature      |
| 6  | `GetTemperatureRatio(double)`                      | Temperature      |
| 7  | `GetPressure()` (no args)                          | Pressure         |
| 8  | `GetPressure(double)` (pure virtual)               | Pressure         |
| 9  | `GetPressureSL(ePressure)`                         | Pressure         |
| 10 | `GetPressureRatio()`                               | Pressure         |
| 11 | `GetDensity()` (no args)                           | Density          |
| 12 | `GetDensity(double)`                               | Density          |
| 13 | `GetDensitySL()`                                   | Density          |
| 14 | `GetDensityRatio()`                                | Density          |
| 15 | `GetSoundSpeed()` (no args)                        | Sound Speed      |
| 16 | `GetSoundSpeed(double)`                            | Sound Speed      |
| 17 | `GetSoundSpeedSL()`                                | Sound Speed      |
| 18 | `GetSoundSpeedRatio()`                             | Sound Speed      |
| 19 | `GetAbsoluteViscosity()`                           | Viscosity        |
| 20 | `GetKinematicViscosity()`                          | Viscosity        |
| 21 | `GetDensityAltitude()`                             | Altitude         |
| 22 | `GetPressureAltitude()`                            | Altitude         |
| 23 | `Calculate(double)`                                | Computation      |
| 24 | `ConvertToRankine(double, eTemperature)`           | Unit Conversion  |
| 25 | `ConvertFromRankine(double, eTemperature)`         | Unit Conversion  |
| 26 | `ConvertToPSF(double, ePressure)`                  | Unit Conversion  |
| 27 | `ConvertFromPSF(double, ePressure)`                | Unit Conversion  |
| 28 | `ValidatePressure(double, string, bool)` (protected)| Validation      |
| 29 | `ValidateTemperature(double, string, bool)` (protected)| Validation  |

#### FGStandardAtmosphere1976 — ISA 1976 standard atmosphere (namespace JSBSimExtract)

21 public API functions (beyond inherited):

| #  | Function / Operator                                | Category         |
|----|----------------------------------------------------|------------------|
| 1  | Constructor                                        | Construction     |
| 2  | `InitModel()`                                      | Initialization   |
| 3  | `GetTemperature(double)` override                  | Temperature      |
| 4  | `GetStdTemperature(double)`                        | Temperature      |
| 5  | `GetStdTemperatureSL()`                            | Temperature      |
| 6  | `GetStdTemperatureRatio(double)`                   | Temperature      |
| 7  | `GetTemperatureBias()`                             | Temperature      |
| 8  | `GetTemperatureDeltaGradient()`                    | Temperature      |
| 9  | `SetTemperatureSL(double, eTemperature)`           | Temperature      |
| 10 | `SetTemperature(double, double, eTemperature)`     | Temperature      |
| 11 | `SetTemperatureBias(eTemperature, double)`         | Temperature      |
| 12 | `SetSLTemperatureGradedDelta(eTemperature, double)`| Temperature      |
| 13 | `SetTemperatureGradedDelta(double, double, eTemperature)` | Temperature |
| 14 | `ResetSLTemperature()`                             | Temperature      |
| 15 | `GetPressure(double)` override                     | Pressure         |
| 16 | `GetStdPressure(double)`                           | Pressure         |
| 17 | `SetPressureSL(ePressure, double)`                 | Pressure         |
| 18 | `ResetSLPressure()`                                | Pressure         |
| 19 | `GetStdDensity(double)`                            | Density          |
| 20 | `Calculate(double)` override                       | Computation      |
| 21 | `GeopotentialAltitude(double)`                     | Conversion       |
| 22 | `GeometricAltitude(double)`                        | Conversion       |
| 23 | `GetLapseRates()`                                  | Test Support     |
| 24 | `GetPressureBreakpoints()`                         | Test Support     |
| 25 | `CalculateLapseRates()` (protected)                | Internal         |
| 26 | `CalculatePressureBreakpoints(double)` (protected) | Internal         |
| 27 | `CalculateStdDensityBreakpoints()` (protected)     | Internal         |
| 28 | `CalculateDensityAltitude(double, double)` override| Internal         |
| 29 | `CalculatePressureAltitude(double, double)` override| Internal        |
| 30 | `CalculateSLDensity()` (protected)                 | Internal         |
| 31 | `CalculateSLSoundSpeedAndDensity()` (protected)    | Internal         |

### 2.3 Test Framework

- **Primary:** Google Test (`tests/googletest/test_atmosphere_gtest.cpp`)
- **Existing tests:** 46 test cases (pre-DAL A baseline)

### 2.4 Coverage Targets

| Metric                                | Target |
|---------------------------------------|--------|
| Statement Coverage (SC)               | 100%   |
| Decision Coverage (DC)                | 100%   |
| Modified Condition/Decision Coverage  | 100%   |

---

## 3. Requirements Traceability

| Requirement ID | Description                                      | Functions Covered                                                                 |
|----------------|--------------------------------------------------|-----------------------------------------------------------------------------------|
| REQ-SF-0300    | ISA 1976 Standard Atmosphere Model               | GetTemperature, GetStdTemperature, GetPressure, GetStdPressure, GetStdDensity, GetDensity, GetSoundSpeed, GeopotentialAltitude, GeometricAltitude, Calculate, all lapse/breakpoint internals |
| REQ-SF-0305    | Density Altitude Computation                     | CalculateDensityAltitude, CalculatePressureAltitude, GetDensityAltitude, GetPressureAltitude |
| REQ-SF-1400    | DO-178C Verification Evidence Generation         | All functions — coverage artifacts (SC, DC, MC/DC) produced for each              |
| REQ-SF-1401    | Requirements Traceability Matrix Generation      | All functions — each test case maps to requirement and function under test         |

---

## 4. Existing Test Baseline

- **File:** `tests/googletest/test_atmosphere_gtest.cpp` (~46 test cases)
- **Approximate existing coverage:** Basic ISA model validation at standard altitudes
- **NOT at DAL A level.** Known gaps:
  - MC/DC for multi-condition decisions (`SetTemperatureBias`, `SetTemperatureGradedDelta` unit checks) not exercised
  - Switch statement default cases (invalid unit enums) not tested for unit conversion functions
  - SimpleTable edge cases (empty table, single-entry table, below/above clamp) not fully covered
  - Below-sea-level extrapolation path not tested
  - Temperature bias clamping near absolute zero not tested
  - Graded delta clamping at minimum delta not tested
  - Isothermal vs. non-isothermal pressure layer paths not independently verified for MC/DC
  - Density altitude and pressure altitude inverse computations not verified across all layers
  - ValidatePressure and ValidateTemperature boundary cases not tested
  - GeometricAltitude singularity at H = EarthRadius not tested
  - Viscosity (Sutherland's law) output not verified

---

## 5. Test Plan Summary

### 5.1 Test Case Estimates by Category

| Category            | Estimated Cases | Existing | New Required | Description                                                              |
|---------------------|-----------------|----------|--------------|--------------------------------------------------------------------------|
| Normal range        | ~30             | ~25      | ~5           | Standard ISA altitudes (0, 11km, 20km, 32km, 47km, 51km, 71km, 84.8km) |
| Boundary value      | ~20             | ~5       | ~15          | Layer boundaries, sea level, table endpoints, minimum temperature        |
| Robustness          | ~10             | 0        | ~10          | Extreme altitudes, near-zero pressure, extreme bias, empty table         |
| Error handling      | ~10             | ~5       | ~5           | Invalid unit enums, division by zero edge cases, singularity guard       |
| MC/DC               | ~25             | 0        | ~25          | Condition isolation for all 25+ decisions across 3 modules               |
| Unit conversion     | ~15             | ~5       | ~10          | All 4 temperature units, all 4 pressure units, round-trip conversions    |
| State/sequence      | ~10             | ~6       | ~4           | InitModel reset, SetTemperature then Calculate, bias+gradient interaction|
| **Total**           | **~120**        | **~46**  | **~74**      |                                                                          |

### 5.2 Test Organization

Tests will be organized into Google Test fixtures and test suites:

- `SimpleTableTest` — table construction, data entry, interpolation, edge cases
- `FGAtmosphereBaseTest` — unit conversions, validation, base class interface
- `FGStandardAtmosphere1976Test` — ISA model temperature, pressure, density
- `AtmosphereMCDCTest` — dedicated MC/DC condition-isolation tests
- `AtmosphereRobustnessTest` — extreme inputs, boundary stress tests
- `AtmosphereIntegrationTest` — end-to-end Calculate() with cross-property verification

---

## 6. Decision Points Requiring MC/DC Analysis

### 6.1 FGAtmosphereBase

| # | Decision ID | Decision                                  | Location                     | Conditions | MC/DC Test Cases |
|---|-------------|-------------------------------------------|------------------------------|------------|------------------|
| 1 | AB-D1       | `if (p < MinPressure)`                    | `ValidatePressure()`         | 1          | 2 (DC)           |
| 2 | AB-D2       | `if (t < minT)`                           | `ValidateTemperature()`      | 1          | 2 (DC)           |
| 3 | AB-D3       | `switch (unit)` — 4 cases + default       | `ConvertToRankine()`         | 4+1 paths  | 5                |
| 4 | AB-D4       | `switch (unit)` — 4 cases + default       | `ConvertFromRankine()`       | 4+1 paths  | 5                |
| 5 | AB-D5       | `switch (unit)` — 4 cases + default       | `ConvertToPSF()`             | 4+1 paths  | 5                |
| 6 | AB-D6       | `switch (unit)` — 4 cases + default       | `ConvertFromPSF()`           | 4+1 paths  | 5                |

### 6.2 FGStandardAtmosphere1976

| # | Decision ID | Decision                                           | Location                     | Conditions | MC/DC Test Cases |
|---|-------------|------------------------------------------------------|------------------------------|------------|------------------|
| 7 | SA-D1       | `if (GeoPotAlt >= 0.0)`                             | `GetTemperature()`           | 1          | 2 (DC)           |
| 8 | SA-D2       | `if (GeoPotAlt <= GradientFadeoutAltitude)`          | `GetTemperature()` (D1-TRUE) | 1          | 2 (DC)           |
| 9 | SA-D3       | `if (GeoPotAlt <= GradientFadeoutAltitude)`          | `GetTemperature()` (after bias)| 1        | 2 (DC)           |
| 10| SA-D4       | `if (GeoPotAlt >= 0.0)`                             | `GetStdTemperature()`        | 1          | 2 (DC)           |
| 11| SA-D5       | `if (GeoPotAlt < testAlt)` (loop break)             | `GetPressure()` layer search | 1 per iter | 2 (DC)           |
| 12| SA-D6       | `if (Lmb != 0.0)`                                   | `GetPressure()` isothermal   | 1          | 2 (DC)           |
| 13| SA-D7       | `if (GeoPotAlt < testAlt)` (loop break)             | `GetStdPressure()` layer search| 1 per iter| 2 (DC)           |
| 14| SA-D8       | `if (Lmb != 0.0)`                                   | `GetStdPressure()` isothermal| 1          | 2 (DC)           |
| 15| SA-D9       | `if (unit == eCelsius \|\| unit == eKelvin)`         | `SetTemperatureBias()`       | **2**      | **3 (MC/DC)**    |
| 16| SA-D10      | `if (minStdTemp + TemperatureBias < minUniverseTemp)`| `SetTemperatureBias()` clamp | 1          | 2 (DC)           |
| 17| SA-D11      | `if (unit == eCelsius \|\| unit == eKelvin)`         | `SetTemperatureGradedDelta()`| **2**      | **3 (MC/DC)**    |
| 18| SA-D12      | `if (deltemp <= minDelta)`                           | `SetTemperatureGradedDelta()`| 1          | 2 (DC)           |
| 19| SA-D13      | `if (LapseRates[b] != 0.0)` (loop body)             | `CalculatePressureBreakpoints()`| 1        | 2 (DC)           |
| 20| SA-D14      | `if (density >= StdDensityBreakpoints[b+1])` (loop)  | `CalculateDensityAltitude()` | 1 per iter | 2 (DC)           |
| 21| SA-D15      | `if (Lmb != 0.0)`                                   | `CalculateDensityAltitude()` | 1          | 2 (DC)           |
| 22| SA-D16      | `if (pressure >= StdPressureBreakpoints[b+1])` (loop)| `CalculatePressureAltitude()`| 1 per iter | 2 (DC)           |
| 23| SA-D17      | `if (Lmb != 0.0)`                                   | `CalculatePressureAltitude()`| 1          | 2 (DC)           |

### 6.3 SimpleTable

| # | Decision ID | Decision                                  | Location            | Conditions | MC/DC Test Cases |
|---|-------------|-------------------------------------------|----------------------|------------|------------------|
| 24| ST-D1       | `if (pendingKey_)`                        | `operator<<`         | 1          | 2 (DC)           |
| 25| ST-D2       | `if (keys_.empty())`                      | `GetValue()`         | 1          | 2 (DC)           |
| 26| ST-D3       | `if (key <= keys_.front())`               | `GetValue()`         | 1          | 2 (DC)           |
| 27| ST-D4       | `if (key >= keys_.back())`                | `GetValue()`         | 1          | 2 (DC)           |
| 28| ST-D5       | `if (i == 0) i = 1`                       | `GetValue()`         | 1          | 2 (DC)           |

### 6.4 Decision Summary

| Module                       | Decisions | Multi-Condition | Total Conditions | MC/DC Test Points |
|------------------------------|-----------|-----------------|------------------|-------------------|
| FGAtmosphereBase             | 6         | 0               | 6 + 4 switches   | 24                |
| FGStandardAtmosphere1976     | 17        | 2               | 19               | 26                |
| SimpleTable                  | 5         | 0               | 5                | 10                |
| **Total**                    | **28**    | **2**           | **30 + switches**| **60**            |

**SA-D9 — SetTemperatureBias unit check (2 conditions, 3 MC/DC test cases):**

| Test Case | unit     | A (== eCelsius) | B (== eKelvin) | Decision (A \|\| B) | Independence Shows |
|-----------|----------|-----------------|----------------|----------------------|--------------------|
| MC-SA9-1  | eCelsius | T               | F              | T                    | A independently affects outcome |
| MC-SA9-2  | eKelvin  | F               | T              | T                    | B independently affects outcome |
| MC-SA9-3  | eRankine | F               | F              | F                    | Both false -> decision false    |

**SA-D11 — SetTemperatureGradedDelta unit check (2 conditions, 3 MC/DC test cases):**

| Test Case | unit     | A (== eCelsius) | B (== eKelvin) | Decision (A \|\| B) | Independence Shows |
|-----------|----------|-----------------|----------------|----------------------|--------------------|
| MC-SA11-1 | eCelsius | T               | F              | T                    | A independently affects outcome |
| MC-SA11-2 | eKelvin  | F               | T              | T                    | B independently affects outcome |
| MC-SA11-3 | eRankine | F               | F              | F                    | Both false -> decision false    |

---

## 7. Deliverables

| # | Deliverable                          | Artifact                                                             | Format         |
|---|--------------------------------------|----------------------------------------------------------------------|----------------|
| 1 | Enhanced unit test suite             | `tests/googletest/test_atmosphere_gtest.cpp`                         | C++ / GTest    |
| 2 | TAR document                         | `docs/tars/TAR-SF-002.md`                                           | Markdown       |
| 3 | MC/DC analysis document              | `docs/tars/TAR-SF-002-MCDC-Analysis.md`                             | Markdown       |
| 4 | SVR entry                            | Test results and coverage numbers in SVR-SF-2026-001                 | Markdown       |
| 5 | Requirements traceability matrix     | Requirement -> test case -> result mapping                           | Markdown/CSV   |
| 6 | Coverage report                      | Statement, Decision, MC/DC coverage for all 3 modules                | lcov / HTML    |

---

## 8. Acceptance Criteria

All of the following must be satisfied before TAR-SF-002 can be closed:

1. **All tests pass** -- zero failures, zero errors in Google Test suite.
2. **Statement coverage >= 100%** for `FGAtmosphereBase.h`, `FGStandardAtmosphere1976.h`, `FGStandardAtmosphere1976.cpp`.
3. **Decision coverage >= 100%** for all three source files.
4. **MC/DC documented** for every multi-condition decision identified in Section 6, with truth tables and independence pairs.
5. **Switch coverage complete** -- every case label (including default/throw paths) exercised for all 4 unit-conversion switch statements.
6. **No dead code** -- every reachable statement is exercised; any unreachable code is documented and justified.
7. **All source code traces to requirements** -- no orphan code (code with no requirement justification).
8. **Human review completed** -- independent review required per DAL A (DO-178C Section 6.3.4, Table A-7).
9. **Traceability matrix complete** -- every requirement maps to at least one test case; every test case maps to at least one requirement.

---

## 9. Estimated Effort

| Phase                  | Description                                           | Effort Estimate      | Status       |
|------------------------|-------------------------------------------------------|----------------------|--------------|
| Phase 1 -- Intake      | TAR creation, scope definition, MC/DC analysis        | 1 session            | **Complete** |
| Phase 2 -- Plan        | Detailed test plan with MC/DC pairs, test case IDs    | 1 session            | Not Started  |
| Phase 3 -- Generate    | Test code generation, compilation, initial debug      | 2-3 sessions         | Not Started  |
| Phase 4 -- Validate    | Run tests, coverage analysis, gap identification      | 1 session            | Not Started  |
| Phase 5 -- Document    | SVR entry, traceability matrix, MC/DC analysis report | 1 session            | Not Started  |

**Total estimated:** 6-7 sessions

---

## 10. Dependencies

| Dependency                              | Status       | Notes                                    |
|-----------------------------------------|--------------|------------------------------------------|
| TAR-SF-001 complete (Math Library)      | Complete     | Math modules verified at DAL A           |
| PSAC baselined (PSAC-SF-2026-001)       | Complete     |                                          |
| SDP baselined (SDP-SF-2026-001)         | Complete     |                                          |
| SVP baselined (SVP-SF-2026-001)         | Complete     |                                          |
| SCMP baselined (SCMP-SF-2026-001)       | Complete     |                                          |
| SQAP baselined (SQAP-SF-2026-001)       | Complete     |                                          |
| SRS-Std baselined                       | Complete     |                                          |
| SDS baselined                           | Complete     |                                          |
| SCS baselined (SCS-SF-2026-001)         | Complete     |                                          |
| SRS requirements available              | Complete     | REQ-SF-0300, 0305, 1400, 1401           |
| Build system functional                 | Complete     | CMake + GCC toolchain                    |
| CI/CD operational                       | Complete     |                                          |

---

## 11. Risks

| # | Risk                                                                                         | Severity | Mitigation                                                                                          |
|---|----------------------------------------------------------------------------------------------|----------|------------------------------------------------------------------------------------------------------|
| 1 | Floating-point comparison in pressure layer search (`GeoPotAlt < testAlt`) may produce incorrect layer selection at exact layer boundaries | High     | Test at exact boundary altitudes (e.g., 36089.2388 ft) with boundary +/- epsilon; verify correct layer is selected in each case |
| 2 | `GeometricAltitude()` singularity at `H = EarthRadius` (division by zero when `EarthRadius - geopotalt == 0`) | High     | Document singularity; test near-singularity values; consider if guard clause is needed               |
| 3 | SimpleTable empty-table edge cases: `GetValue()` returns 0.0 on empty table, `GetMinValue()` returns 0.0 on empty values | Medium   | Test empty table, single-entry table, and verify boundary behavior is intentional                    |
| 4 | Temperature bias clamping near absolute zero depends on `GetMinValue()` returning correct min of ISA profile | Medium   | Verify `GetMinValue()` returns 336.5028 R (minimum ISA temperature); test bias that would drive below 1.8 R |
| 5 | `SetTemperatureGradedDelta` division by `(GradientFadeoutAltitude - GeopotentialAltitude(h))` can produce division by zero if `h` converts to fadeout altitude | Medium   | Test with `h = 0.0` (safe) and document the constraint on `h` parameter range                       |
| 6 | Cross-platform floating-point behavior may vary for `std::pow` and `std::exp` in pressure calculations | Low      | Use `EXPECT_NEAR` with ISA-appropriate tolerances; document platform assumptions                     |
| 7 | MC/DC test count may increase if compiler generates additional implicit decisions for switch statements | Low      | Review generated assembly or compiler output if source-level analysis is insufficient                |

---

## 12. Revision History

| Rev | Date       | Author                       | Description              |
|-----|------------|------------------------------|--------------------------|
| 1.0 | 2026-03-14 | Development Team (LLM-Assisted) | Initial TAR creation  |
