# TAR-SF-003: Terrain Engine Core Verification & DAL A Coverage Enhancement

| Field          | Value                                              |
|----------------|----------------------------------------------------|
| TAR Number     | TAR-SF-003                                         |
| Title          | Terrain Engine Core Verification & DAL A Coverage Enhancement |
| Date Issued    | 2026-03-14                                         |
| Status         | Implementation Complete (PR #9 merged)              |
| Priority       | Critical (terrain engine feeds CFIT alerting and mission planning) |
| Assigned To    | Development Team (LLM-Assisted)                    |
| DAL            | A (Catastrophic) — DO-178C                         |
| WBS            | WBS 9.x — Terrain Engine (Increment 3)             |
| Product        | SynthFlight — UAV Mission Simulation and Verification Platform |
| Coding Standard| C++17 (2023), GCC (SCS-SF-2026-001)               |
| Author         | LLM-Assisted (Claude Opus 4.6)                    |

---

## 1. Objective

Achieve DAL A structural coverage (Statement, Decision, MC/DC) for the two terrain engine modules — **TerrainGrid** (regular elevation grid with bilinear interpolation) and **TerrainCollisionDetector** (CFIT look-ahead prediction and alert generation) — and generate requirements-based tests for all public API functions. These modules provide terrain awareness and collision prevention services used by mission planning, flight dynamics, and alerting subsystems; all terrain-related computations depend on the correctness verified here.

Specific objectives:

- Achieve 100% Statement Coverage (SC), 100% Decision Coverage (DC), and 100% Modified Condition/Decision Coverage (MC/DC) for both modules.
- Generate requirements-based test cases for every public API function, including grid construction, elevation queries, bilinear interpolation, coordinate conversion, CFIT trajectory projection, and alert level determination.
- Establish verification evidence per SVP-SF-2026-001.
- Produce traceability from SRS requirements through test cases to coverage results.

---

## 2. Scope

### 2.1 Source Files Under Test

| # | File                                        | Role                         |
|---|---------------------------------------------|------------------------------|
| 1 | `src/terrain/TerrainGrid.h`                 | Terrain grid header          |
| 2 | `src/terrain/TerrainGrid.cpp`               | Terrain grid implementation  |
| 3 | `src/terrain/TerrainCollisionDetector.h`    | Collision detector header    |
| 4 | `src/terrain/TerrainCollisionDetector.cpp`  | Collision detector implementation |

### 2.2 Modules Under Test

#### TerrainGrid — Regular elevation grid with bilinear interpolation (namespace SynthFlight)

Public API functions:

| #  | Function / Method                                              | Category              |
|----|----------------------------------------------------------------|-----------------------|
| 1  | Constructor `TerrainGrid(rows, cols, postingMeters, originLat, originLon)` | Construction |
| 2  | `SetElevation(row, col, elevation)`                            | Data Entry            |
| 3  | `GetElevationAtCell(row, col)`                                 | Cell Access           |
| 4  | `GetRows()`                                                    | Query                 |
| 5  | `GetCols()`                                                    | Query                 |
| 6  | `GetPostingMeters()`                                           | Query                 |
| 7  | `GetOriginLat()`                                               | Query                 |
| 8  | `GetOriginLon()`                                               | Query                 |
| 9  | `IsInsideGrid(lat, lon)`                                       | Bounds Check          |
| 10 | `GetElevation(lat, lon)`                                       | Interpolation         |
| 11 | `GetElevationBatch(lats, lons, elevations, count)`             | Batch Query           |
| 12 | `FillFlat(elevation)`                                          | Grid Fill             |
| 13 | `FillSlope(baseElevation, slopeNS_degrees, slopeEW_degrees)`  | Grid Fill             |
| 14 | `LatLonToGrid(lat, lon, row, col)`                             | Coordinate Conversion |
| 15 | `GridToLatLon(row, col, lat, lon)`                             | Coordinate Conversion |
| 16 | `GetMinElevation()`                                            | Statistics            |
| 17 | `GetMaxElevation()`                                            | Statistics            |
| 18 | `GetMeanElevation()`                                           | Statistics            |

#### TerrainCollisionDetector — CFIT look-ahead prediction (namespace SynthFlight)

Public API functions:

| #  | Function / Method                                                                  | Category              |
|----|------------------------------------------------------------------------------------|-----------------------|
| 1  | Constructor `TerrainCollisionDetector(terrain)`                                    | Construction          |
| 2  | `SetConfig(config)`                                                                | Configuration         |
| 3  | `GetConfig()`                                                                      | Query                 |
| 4  | `SetClearanceThreshold(thresholdFt)`                                               | Configuration         |
| 5  | `GetClearanceThreshold()`                                                          | Query                 |
| 6  | `SetLookaheadTime(timeSec)`                                                        | Configuration         |
| 7  | `GetLookaheadTime()`                                                               | Query                 |
| 8  | `Evaluate(state)` — returns `CFITResult` (includes alertLevel, timeToImpactSec, minimumClearanceFt) | CFIT Evaluation |
| 9  | `PredictPosition(state, timeSec, lat, lon, altMSL)` — static                      | Trajectory Prediction |
| 10 | `FeetToMeters(ft)` — static constexpr                                             | Unit Conversion       |
| 11 | `MetersToFeet(m)` — static constexpr                                              | Unit Conversion       |
| 12 | `KnotsToFPS(kts)` — static constexpr                                              | Unit Conversion       |

### 2.3 Test Framework

- **Primary:** Google Test (`tests/googletest/test_terrain_gtest.cpp`)
- **Existing tests:** None (new module — Increment 3)

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
| REQ-SF-0400    | Multi-Format Terrain Data Ingestion (partial — grid data model only) | Constructor, SetElevation, FillFlat, FillSlope, GetElevationAtCell, GetRows, GetCols, GetPostingMeters, GetOriginLat, GetOriginLon, GetMinElevation, GetMaxElevation, GetMeanElevation |
| REQ-SF-0402    | Terrain Collision Detection (CFIT Prevention)    | TerrainCollisionDetector::Evaluate, SetConfig, GetConfig, SetClearanceThreshold, SetLookaheadTime, GetClearanceThreshold, GetLookaheadTime, PredictPosition, FeetToMeters, MetersToFeet, KnotsToFPS |
| REQ-SF-0404    | Parameterized Infrastructure Obstacle Library (future increment) | Placeholder — not covered in this TAR; reserved for Increment 4 |
| REQ-SF-0406    | Elevation Query API                              | GetElevation, GetElevationBatch, IsInsideGrid, LatLonToGrid, GridToLatLon |
| REQ-SF-1400    | DO-178C DAL A structural coverage evidence       | All functions — coverage artifacts (SC, DC, MC/DC) produced for each              |

---

## 4. Existing Test Baseline

- **File:** `tests/googletest/test_terrain_gtest.cpp` (new — 0 existing test cases)
- **Approximate existing coverage:** None (Increment 3 new development)
- **NOT at DAL A level.** Known gaps:
  - No tests exist; all test categories must be created from scratch
  - MC/DC for multi-condition decisions (IsInsideGrid 4-condition AND, Evaluate loop compound condition) not exercised
  - Bilinear interpolation boundary clamping paths not tested
  - Out-of-bounds grid access error handling not tested
  - CFIT alert level transitions (NONE -> CAUTION -> WARNING) not verified
  - Collision time computation accuracy not validated against analytical reference
  - Coordinate conversion roundtrip accuracy not tested
  - Batch query edge cases (empty list, large batch) not tested
  - FillSlope loop decisions not exercised
  - SetClearanceThreshold and SetLookaheadTime clamping behavior not tested
  - Zero-speed and zero-altitude CFIT edge cases not tested
  - Grid with minimum dimensions (2x2) not tested

---

## 5. Test Plan Summary

### 5.1 Test Case Estimates by Category

| Category            | Estimated Cases | Existing | New Required | Description                                                              |
|---------------------|-----------------|----------|--------------|--------------------------------------------------------------------------|
| Normal range        | ~15             | 0        | ~15          | Standard elevation queries, CFIT evaluations on flat/sloped terrain      |
| Boundary value      | ~25             | 0        | ~25          | Grid edges, min/max posting, altitude extremes, 2x2 grid, exact grid points |
| Robustness          | ~20             | 0        | ~20          | Out-of-bounds queries, zero speed, extreme angles, near-zero clearance   |
| MC/DC               | ~35             | 0        | ~35          | Bilinear interpolation decisions, CFIT alert level decisions, clamping   |
| Error handling      | ~15             | 0        | ~15          | Invalid grid dimensions, out-of-range indices, degenerate inputs         |
| State/sequence      | ~10             | 0        | ~10          | Config changes mid-evaluation, grid modifications, threshold changes     |
| Integration         | ~10             | 0        | ~10          | TerrainGrid + CollisionDetector together, end-to-end CFIT scenarios      |
| **Total**           | **~130**        | **0**    | **~130**     |                                                                          |

### 5.2 Test Organization

Tests will be organized into Google Test fixtures and test suites:

- `TerrainGridConstructionTest` — constructor validation, invalid dimensions, posting checks
- `TerrainGridDataEntryTest` — SetElevation, FillFlat, FillSlope, boundary access
- `TerrainGridInterpolationTest` — bilinear interpolation, boundary clamping, known-good reference values
- `TerrainGridCoordinateTest` — LatLonToGrid, GridToLatLon, roundtrip accuracy
- `TerrainGridBatchTest` — GetElevationBatch edge cases
- `TerrainCollisionDetectorConfigTest` — SetConfig/GetConfig, threshold/lookahead configuration and clamping
- `TerrainCollisionDetectorEvaluateTest` — CFIT evaluation on synthetic terrains
- `TerrainMCDCTest` — dedicated MC/DC condition-isolation tests
- `TerrainRobustnessTest` — extreme inputs, degenerate grids, edge stress tests
- `TerrainIntegrationTest` — end-to-end TerrainGrid + CollisionDetector scenarios

### 5.3 Test Approach

- **Synthetic terrain grids:** Create deterministic grids (flat, sloped, step, valley, peak) with analytically known elevation values for verification.
- **Known-good bilinear interpolation:** Compute reference interpolation values by hand for grid midpoints, quarter-points, and edge positions. Verify against module output within floating-point tolerance.
- **CFIT alert verification:** Construct terrain and trajectory combinations where collision time and clearance are analytically computable. Verify alert levels match expected transitions.
- **Coordinate conversion roundtrip:** Verify `LatLonToGrid(GridToLatLon(r, c)) == (r, c)` within tolerance for all grid positions.

---

## 6. Decision Points Requiring MC/DC Analysis

### 6.1 TerrainGrid

| # | Decision ID | Decision                                  | Location                     | Conditions | MC/DC Test Cases |
|---|-------------|-------------------------------------------|------------------------------|------------|------------------|
| 1 | TG-D1       | `if (rows < 2 \|\| cols < 2)`            | Constructor                  | **2**      | **3 (MC/DC)**    |
| 2 | TG-D2       | `if (postingMeters <= 0.0)`               | Constructor                  | 1          | 2 (DC)           |
| 3 | TG-D3       | `if (row >= rows_ \|\| col >= cols_)`     | `SetElevation()`             | **2**      | **3 (MC/DC)**    |
| 4 | TG-D4       | `if (row >= rows_ \|\| col >= cols_)`     | `GetElevationAtCell()`       | **2**      | **3 (MC/DC)**    |
| 5 | TG-D5       | `if (gridRow >= 0.0 && gridRow <= maxRow && gridCol >= 0.0 && gridCol <= maxCol)` | `IsInsideGrid()` | **4** | **5 (MC/DC)** |
| 6 | TG-D6a      | `if (r0 < 0)`                             | `GetElevation()` clamping    | 1          | 2 (DC)           |
| 7 | TG-D6b      | `if (r0 >= maxRow)`                        | `GetElevation()` clamping    | 1          | 2 (DC)           |
| 8 | TG-D6c      | `if (c0 < 0)`                             | `GetElevation()` clamping    | 1          | 2 (DC)           |
| 9 | TG-D6d      | `if (c0 >= maxCol)`                        | `GetElevation()` clamping    | 1          | 2 (DC)           |
| 10| TG-D7a      | `for (row < rows_)`                        | `FillSlope()` outer loop     | 1          | 2 (DC)           |
| 11| TG-D7b      | `for (col < cols_)`                        | `FillSlope()` inner loop     | 1          | 2 (DC)           |

### 6.2 TerrainCollisionDetector

| # | Decision ID | Decision                                           | Location                     | Conditions | MC/DC Test Cases |
|---|-------------|------------------------------------------------------|------------------------------|------------|------------------|
| 12| CD-D8       | `if (threshold < 25.0)`                              | `SetClearanceThreshold()`    | 1          | 2 (DC)           |
| 13| CD-D9       | `if (threshold > 500.0)`                             | `SetClearanceThreshold()`    | 1          | 2 (DC)           |
| 14| CD-D10      | `if (time < 60.0)`                                  | `SetLookaheadTime()`         | 1          | 2 (DC)           |
| 15| CD-D11      | `for (step <= maxSteps && t <= lookaheadTime)`       | `Evaluate()` loop            | **2**      | **3 (MC/DC)**    |
| 16| CD-D12      | `if (clearance <= 0.0)`                              | `Evaluate()` WARNING         | 1          | 2 (DC)           |
| 17| CD-D13      | `if (clearance < threshold * cautionMultiplier)`     | `Evaluate()` CAUTION         | 1          | 2 (DC)           |
| 18| CD-D14      | `if (clearance < minClearance)`                      | `Evaluate()` tracking minimum| 1          | 2 (DC)           |

### 6.3 Decision Summary

| Module                       | Decisions | Multi-Condition | Total Conditions | MC/DC Test Points |
|------------------------------|-----------|-----------------|------------------|-------------------|
| TerrainGrid                  | 11        | 3               | 15               | 28                |
| TerrainCollisionDetector     | 7         | 1               | 9                | 16                |
| **Total**                    | **18**    | **4**           | **24**           | **44**            |

**TG-D1 — Constructor dimension validation (2 conditions, 3 MC/DC test cases):**

| Test Case | rows | cols | A (rows < 2) | B (cols < 2) | Decision (A \|\| B) | Independence Shows |
|-----------|------|------|---------------|--------------|----------------------|--------------------|
| MC-TG1-1  | 1    | 10   | T             | F            | T                    | A independently affects outcome |
| MC-TG1-2  | 10   | 1    | F             | T            | T                    | B independently affects outcome |
| MC-TG1-3  | 10   | 10   | F             | F            | F                    | Both false -> decision false    |

**TG-D3 — SetElevation bounds check (2 conditions, 3 MC/DC test cases):**

| Test Case | row     | col     | A (row >= rows_) | B (col >= cols_) | Decision (A \|\| B) | Independence Shows |
|-----------|---------|---------|------------------|------------------|----------------------|--------------------|
| MC-TG3-1  | rows_   | 0       | T                | F                | T                    | A independently affects outcome |
| MC-TG3-2  | 0       | cols_   | F                | T                | T                    | B independently affects outcome |
| MC-TG3-3  | 0       | 0       | F                | F                | F                    | Both false -> decision false    |

**TG-D5 — IsInsideGrid (4 conditions, 5 MC/DC test cases):**

| Test Case | A (gridRow >= 0) | B (gridRow <= maxRow) | C (gridCol >= 0) | D (gridCol <= maxCol) | Decision (A && B && C && D) | Independence Shows |
|-----------|------------------|-----------------------|------------------|-----------------------|-----------------------------|--------------------|
| MC-TG5-1  | T                | T                     | T                | T                     | T                           | Baseline — all true |
| MC-TG5-2  | F                | T                     | T                | T                     | F                           | A independently affects outcome |
| MC-TG5-3  | T                | F                     | T                | T                     | F                           | B independently affects outcome |
| MC-TG5-4  | T                | T                     | F                | T                     | F                           | C independently affects outcome |
| MC-TG5-5  | T                | T                     | T                | F                     | F                           | D independently affects outcome |

**CD-D11 — Evaluate loop condition (2 conditions, 3 MC/DC test cases):**

| Test Case  | A (step <= maxSteps) | B (t <= lookaheadTime) | Decision (A && B) | Independence Shows |
|------------|----------------------|------------------------|--------------------|--------------------|
| MC-CD11-1  | T                    | T                      | T                  | Baseline — loop continues |
| MC-CD11-2  | F                    | T                      | F                  | A independently affects outcome |
| MC-CD11-3  | T                    | F                      | F                  | B independently affects outcome |

---

## 7. Deliverables

| # | Deliverable                          | Artifact                                                             | Format         |
|---|--------------------------------------|----------------------------------------------------------------------|----------------|
| 1 | Unit test suite                      | `tests/googletest/test_terrain_gtest.cpp`                            | C++ / GTest    |
| 2 | TAR document                         | `docs/tars/TAR-SF-003.md`                                           | Markdown       |
| 3 | MC/DC analysis document              | `docs/tars/TAR-SF-003-MCDC-Analysis.md`                             | Markdown       |
| 4 | SVR entry                            | Test results and coverage numbers in SVR-SF-2026-001                 | Markdown       |
| 5 | Requirements traceability matrix     | Requirement -> test case -> result mapping                           | Markdown/CSV   |
| 6 | Coverage report                      | Statement, Decision, MC/DC coverage for both modules                 | lcov / HTML    |

---

## 8. Acceptance Criteria

All of the following must be satisfied before TAR-SF-003 can be closed:

1. **All tests pass** -- zero failures, zero errors in Google Test suite.
2. **Statement coverage >= 100%** for `TerrainGrid.h`, `TerrainGrid.cpp`, `TerrainCollisionDetector.h`, `TerrainCollisionDetector.cpp`.
3. **Decision coverage >= 100%** for all four source files.
4. **MC/DC documented** for every multi-condition decision identified in Section 6, with truth tables and independence pairs.
5. **Clamping coverage complete** -- every clamping branch in GetElevation (TG-D6a through TG-D6d) exercised in both TRUE and FALSE paths.
6. **No dead code** -- every reachable statement is exercised; any unreachable code is documented and justified.
7. **All source code traces to requirements** -- no orphan code (code with no requirement justification).
8. **Human review completed** -- independent review required per DAL A (DO-178C Section 6.3.4, Table A-7).
9. **Traceability matrix complete** -- every requirement maps to at least one test case; every test case maps to at least one requirement.
10. **CFIT alert accuracy verified** -- collision times match analytical reference values within 1 time-step tolerance.

---

## 9. Estimated Effort

| Phase                  | Description                                           | Effort Estimate      | Status       |
|------------------------|-------------------------------------------------------|----------------------|--------------|
| Phase 1 -- Intake      | TAR creation, scope definition, MC/DC analysis        | 1 session            | **Complete** |
| Phase 2 -- Plan        | Detailed test plan with MC/DC pairs, test case IDs    | 1 session            | **Complete** |
| Phase 3 -- Generate    | Test code generation, compilation, initial debug      | 2-3 sessions         | **Complete** |
| Phase 4 -- Validate    | Run tests, coverage analysis, gap identification      | 1 session            | **Complete** |
| Phase 5 -- Document    | SVR entry, traceability matrix, MC/DC analysis report | 1 session            | **Complete** |

**Total estimated:** 6-7 sessions

---

## 10. Dependencies

| Dependency                              | Status       | Notes                                    |
|-----------------------------------------|--------------|------------------------------------------|
| TAR-SF-001 complete (Math Library)      | Complete     | Math modules verified at DAL A           |
| TAR-SF-002 complete (Atmosphere Model)  | Complete     | Atmosphere modules verified at DAL A     |
| PSAC baselined (PSAC-SF-2026-001)       | Complete     |                                          |
| SDP baselined (SDP-SF-2026-001)         | Complete     |                                          |
| SVP baselined (SVP-SF-2026-001)         | Complete     |                                          |
| SCMP baselined (SCMP-SF-2026-001)       | Complete     |                                          |
| SQAP baselined (SQAP-SF-2026-001)       | Complete     |                                          |
| SRS-Std baselined                       | Complete     |                                          |
| SDS baselined                           | Complete     |                                          |
| SCS baselined (SCS-SF-2026-001)         | Complete     |                                          |
| SRS requirements available              | Complete     | REQ-SF-0400, 0402, 0404, 0406, 1400     |
| Build system functional                 | Complete     | CMake + GCC toolchain                    |
| CI/CD operational                       | Complete     |                                          |

---

## 11. Risks

| # | Risk                                                                                         | Severity | Mitigation                                                                                          |
|---|----------------------------------------------------------------------------------------------|----------|------------------------------------------------------------------------------------------------------|
| 1 | Bilinear interpolation numerical accuracy at grid boundaries may produce off-by-one clamping errors when floating-point rounding pushes coordinates outside [0, maxRow] or [0, maxCol] | High     | Test at exact grid boundaries and boundary +/- epsilon; verify clamping produces correct values      |
| 2 | CFIT trajectory projection accumulates position error over many time steps; long lookahead times may produce false alerts or missed alerts | High     | Use analytically computable trajectories (straight line over flat/sloped terrain); compare collision time within 1 time-step tolerance |
| 3 | IsInsideGrid 4-condition AND decision: floating-point equality at grid edges (`gridRow == maxRow`) may produce inconsistent results depending on coordinate conversion path | Medium   | Test exact boundary values and boundary +/- epsilon; document whether boundaries are inclusive or exclusive |
| 4 | SetClearanceThreshold and SetLookaheadTime clamping boundaries (25.0, 500.0, 60.0) may not align with operational requirements; need to verify against SRS | Medium   | Cross-reference clamping values with REQ-SF-0402; add boundary tests at exact clamp values           |
| 5 | Grid with minimum dimensions (2x2) produces a single interpolation cell; interpolation and collision detection must still work correctly in this degenerate case | Medium   | Create dedicated 2x2 grid tests for all interpolation and CFIT scenarios                             |
| 6 | Coordinate conversion roundtrip (world -> grid -> world) accumulates floating-point error; tolerance selection for EXPECT_NEAR must account for grid spacing | Low      | Use `EXPECT_NEAR` with tolerance proportional to posting resolution; document tolerance rationale     |
| 7 | FillSlope with extreme slope values (near-infinite north/east gradients) may produce elevation values exceeding float range | Low      | Test with large but finite slopes; verify no overflow/NaN production                                 |

---

## 12. Revision History

| Rev | Date       | Author                       | Description              |
|-----|------------|------------------------------|--------------------------|
| 1.0 | 2026-03-14 | LLM-Assisted (Claude Opus 4.6) | Initial TAR creation  |
| 1.1 | 2026-03-15 | LLM-Assisted (Claude Opus 4.6) | Updated Section 2.2 API to match as-built source; added 11 undocumented public functions; aligned per traceability audit findings |
