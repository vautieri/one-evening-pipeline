# TAR-SF-001: Math Library Verification & DAL A Coverage Enhancement

| Field          | Value                                              |
|----------------|----------------------------------------------------|
| TAR Number     | TAR-SF-001                                         |
| Title          | Math Library Verification & DAL A Coverage Enhancement |
| Date Issued    | 2026-03-14                                         |
| Status         | Open                                               |
| Priority       | Critical (foundation for all subsequent TARs)      |
| Assigned To    | Development Team (LLM-Assisted)                    |
| DAL            | A (Catastrophic) — DO-178C                         |
| WBS            | 1.0 — JSBSim Foundation / Math Library             |
| Product        | SynthFlight — UAV Mission Simulation and Verification Platform |
| Coding Standard| C++17 (2023), GCC (SCS-SF-2026-001)               |

---

## 1. Objective

Achieve DAL A structural coverage (Statement, Decision, MC/DC) for the two foundational math modules — **FGColumnVector3** and **FGMatrix33** — and generate requirements-based tests for all 58 public API functions. This TAR sits at the bottom of the integration stack (math → models → systems → application); all subsequent TARs depend on the correctness verified here.

Specific objectives:

- Achieve 100% Statement Coverage (SC), 100% Decision Coverage (DC), and 100% Modified Condition/Decision Coverage (MC/DC) for both modules.
- Generate requirements-based test cases for every public API function and operator.
- Establish verification evidence per SVP-SF-2026-001.
- Produce traceability from SRS requirements through test cases to coverage results.

---

## 2. Scope

### 2.1 Source Files Under Test

| # | File                              | Role           |
|---|-----------------------------------|----------------|
| 1 | `src/math/FGColumnVector3.h`      | Vector header  |
| 2 | `src/math/FGColumnVector3.cpp`    | Vector impl    |
| 3 | `src/math/FGMatrix33.h`           | Matrix header  |
| 4 | `src/math/FGMatrix33.cpp`         | Matrix impl    |

### 2.2 Modules Under Test

#### FGColumnVector3 — 3-element column vector (1-based indexing, namespace JSBSim)

28 public API functions/operators:

| #  | Function / Operator                     | Category       |
|----|-----------------------------------------|----------------|
| 1  | Default constructor                     | Construction   |
| 2  | Parameterized constructor (3 doubles)   | Construction   |
| 3  | Copy constructor                        | Construction   |
| 4  | `operator()` const                      | Access         |
| 5  | `operator()` non-const                  | Access         |
| 6  | `Entry()` const                         | Access         |
| 7  | `Entry()` non-const                     | Access         |
| 8  | `operator=` (copy)                      | Assignment     |
| 9  | `operator=` (initializer_list)          | Assignment     |
| 10 | `operator==`                            | Comparison     |
| 11 | `operator!=`                            | Comparison     |
| 12 | `operator+` (vector)                    | Arithmetic     |
| 13 | `operator-` (vector)                    | Arithmetic     |
| 14 | `operator+=` (vector)                   | Arithmetic     |
| 15 | `operator-=` (vector)                   | Arithmetic     |
| 16 | `operator*` (scalar)                    | Arithmetic     |
| 17 | `operator/` (scalar)                    | Arithmetic     |
| 18 | `operator*=` (scalar)                   | Arithmetic     |
| 19 | `operator/=` (scalar)                   | Arithmetic     |
| 20 | `operator*` (cross product)             | Arithmetic     |
| 21 | `operator-` (unary negation)            | Arithmetic     |
| 22 | `InitMatrix()` (no args)               | State          |
| 23 | `InitMatrix()` (1 arg)                 | State          |
| 24 | `InitMatrix()` (3 args)                | State          |
| 25 | `Magnitude()` (no args)                | Query          |
| 26 | `Magnitude()` (2 args)                 | Query          |
| 27 | `Normalize()`                           | Mutation       |
| 28 | `Dump()`                                | Diagnostic     |
| F1 | `DotProduct()` (free function)          | Arithmetic     |
| F2 | `operator*` (free, scalar * vec)        | Arithmetic     |
| F3 | `operator<<` (free, stream insertion)   | Diagnostic     |

#### FGMatrix33 — 3x3 matrix (1-based indexing, flat storage `data[9]`, namespace JSBSim)

30 public API functions/operators:

| #  | Function / Operator                     | Category       |
|----|-----------------------------------------|----------------|
| 1  | Default constructor                     | Construction   |
| 2  | Copy constructor                        | Construction   |
| 3  | 9-parameter constructor                 | Construction   |
| 4  | `operator()` const                      | Access         |
| 5  | `operator()` non-const                  | Access         |
| 6  | `Entry()` const                         | Access         |
| 7  | `Entry()` non-const                     | Access         |
| 8  | `Rows()`                                | Query          |
| 9  | `Cols()`                                | Query          |
| 10 | `Determinant()`                         | Query          |
| 11 | `Invertible()`                          | Query          |
| 12 | `Transposed()`                          | Arithmetic     |
| 13 | `T()` (alias for Transposed)            | Arithmetic     |
| 14 | `Inverse()`                             | Arithmetic     |
| 15 | `InitMatrix()` (no args)               | State          |
| 16 | `InitMatrix()` (1 arg)                 | State          |
| 17 | `GetEuler()`                            | Extraction     |
| 18 | `operator+` (matrix)                    | Arithmetic     |
| 19 | `operator-` (matrix)                    | Arithmetic     |
| 20 | `operator+=` (matrix)                   | Arithmetic     |
| 21 | `operator-=` (matrix)                   | Arithmetic     |
| 22 | `operator*` (scalar)                    | Arithmetic     |
| 23 | `operator/` (scalar)                    | Arithmetic     |
| 24 | `operator*=` (scalar)                   | Arithmetic     |
| 25 | `operator/=` (scalar)                   | Arithmetic     |
| 26 | `operator*` (matrix)                    | Arithmetic     |
| 27 | `operator*=` (matrix)                   | Arithmetic     |
| 28 | `operator*` (vector)                    | Arithmetic     |
| 29 | `operator=` (copy)                      | Assignment     |
| 30 | `operator==`                            | Comparison     |
| 31 | `operator!=`                            | Comparison     |
| 32 | `Dump()`                                | Diagnostic     |
| F1 | `operator*` (free, scalar * matrix)     | Arithmetic     |
| F2 | `operator<<` (free, stream insertion)   | Diagnostic     |

### 2.3 Test Framework

- **Primary:** Google Test (`tests/googletest/test_math_gtest.cpp`)
- **Parallel suites:** Catch2 and Doctest (for REQ-SF-0103 multi-framework compliance)

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
| REQ-SF-0200    | JSBSim 6-DOF Flight Dynamics Integration         | All 58 functions — math library supports state vector computation                 |
| REQ-SF-0103    | Unit Test Framework Support (5 frameworks)        | All test cases (Google Test primary, Catch2/Doctest parallel)                     |
| REQ-SF-1300    | Batch Simulation Execution                        | Arithmetic operators, Magnitude, Normalize, Determinant, Inverse, GetEuler        |
| REQ-SF-1400    | DO-178C Verification Evidence Generation          | All functions — coverage artifacts (SC, DC, MC/DC) produced for each              |
| REQ-SF-1401    | Requirements Traceability Matrix Generation       | All functions — each test case maps to requirement and function under test         |

---

## 4. Existing Test Baseline

- **File:** `tests/googletest/test_math_gtest.cpp` (~500+ lines)
- **Approximate existing coverage:** ~50 test cases covering basic operations
- **NOT at DAL A level.** Known gaps:
  - MC/DC for bounds-checking decisions not exercised
  - Robustness cases (NaN, Inf, overflow) absent
  - `initializer_list` edge cases (fewer than 3 elements, more than 3 elements) not tested
  - Gimbal lock edge cases in `GetEuler` not tested at threshold boundary
  - 2-argument `Magnitude()` overload not covered
  - `Dump()` output formatting not verified
  - Free function coverage incomplete (`DotProduct`, `operator*` scalar*vec, `operator<<`)

---

## 5. Test Plan Summary

### 5.1 Test Case Estimates by Category

| Category            | Estimated Cases | Existing | New Required | Description                                                              |
|---------------------|-----------------|----------|--------------|--------------------------------------------------------------------------|
| Normal range        | ~40             | ~25      | ~15          | Standard inputs, expected outputs                                        |
| Boundary value      | ~30             | ~5       | ~25          | 1-based index boundaries (1,3), zero vectors, identity matrix            |
| Robustness          | ~25             | 0        | ~25          | NaN, Inf, -Inf, overflow, denormal, very large/small values              |
| Error handling      | ~15             | ~5       | ~10          | Out-of-range index, division by zero, singular matrix inverse            |
| MC/DC               | ~20             | 0        | ~20          | Condition isolation for bounds checks, gimbal lock, normalize, invertible|
| State/sequence      | ~10             | ~5       | ~5           | InitMatrix then operate, Normalize then Magnitude, self-assignment       |
| **Total**           | **~140**        | **~50**  | **~90**      |                                                                          |

### 5.2 Test Organization

Tests will be organized into Google Test fixtures and test suites:

- `FGColumnVector3Test` — vector-specific tests
- `FGMatrix33Test` — matrix-specific tests
- `MathMCDCTest` — dedicated MC/DC condition-isolation tests
- `MathRobustnessTest` — NaN/Inf/overflow tests
- `MathIntegrationTest` — cross-module tests (matrix * vector)

---

## 6. Decision Points Requiring MC/DC Analysis

### 6.1 FGColumnVector3

| # | Decision                        | Location              | Conditions | MC/DC Test Cases Required |
|---|----------------------------------|-----------------------|------------|---------------------------|
| 1 | `if (idx < 1 \|\| idx > 3)`    | `operator()`, `Entry()` | 2          | 3                         |
| 2 | `if (scalar == 0.0)`            | `operator/`, `operator/=` | 1       | 2 (DC sufficient)         |
| 3 | `if (mag != 0.0)`               | `Normalize()`         | 1          | 2 (DC sufficient)         |

**Decision 1 — Bounds check (2 conditions, 3 MC/DC test cases):**

| Test Case | idx | idx < 1 | idx > 3 | Decision | Independence Shows |
|-----------|-----|---------|---------|----------|--------------------|
| MC-V-1    | 0   | T       | F       | T        | idx < 1 independently affects outcome |
| MC-V-2    | 4   | F       | T       | T        | idx > 3 independently affects outcome |
| MC-V-3    | 2   | F       | F       | F        | Both false → decision false           |

### 6.2 FGMatrix33

| # | Decision                                            | Location              | Conditions | MC/DC Test Cases Required |
|---|-----------------------------------------------------|-----------------------|------------|---------------------------|
| 1 | `if (row < 1 \|\| row > 3 \|\| col < 1 \|\| col > 3)` | `operator()`, `Entry()` | 4       | 5                         |
| 2 | `if (sinTheta >= 1.0)`                              | `GetEuler()`          | 1          | 2 (DC sufficient)         |
| 3 | `else if (sinTheta <= -1.0)`                        | `GetEuler()`          | 1          | 2 (DC sufficient)         |
| 4 | `if (std::abs(sinTheta) < 0.9999999)`               | `GetEuler()`          | 1          | 2 (DC sufficient)         |
| 5 | `if (det == 0.0)`                                   | `Invertible()`        | 1          | 2 (DC sufficient)         |
| 6 | `if (scalar == 0.0)`                                | `operator/`, `operator/=` | 1      | 2 (DC sufficient, 2 locations) |
| 7 | `if (j > 1)` in Dump                                | `Dump()`              | 1          | 2 (DC sufficient)         |
| 8 | `if (i < 3)` in Dump                                | `Dump()`              | 1          | 2 (DC sufficient)         |

**Decision 1 — Matrix bounds check (4 conditions, 5 MC/DC test cases):**

| Test Case | row | col | row<1 | row>3 | col<1 | col>3 | Decision | Independence Shows |
|-----------|-----|-----|-------|-------|-------|-------|----------|--------------------|
| MC-M-1    | 0   | 2   | T     | F     | F     | F     | T        | row<1 independently affects outcome |
| MC-M-2    | 4   | 2   | F     | T     | F     | F     | T        | row>3 independently affects outcome |
| MC-M-3    | 2   | 0   | F     | F     | T     | F     | T        | col<1 independently affects outcome |
| MC-M-4    | 2   | 4   | F     | F     | F     | T     | T        | col>3 independently affects outcome |
| MC-M-5    | 2   | 2   | F     | F     | F     | F     | F        | All false → decision false          |

---

## 7. Deliverables

| # | Deliverable                          | Artifact                                                | Format         |
|---|--------------------------------------|---------------------------------------------------------|----------------|
| 1 | Enhanced unit test suite             | `tests/googletest/test_math_gtest.cpp`                  | C++ / GTest    |
| 2 | Catch2 parallel suite                | `tests/catch2/test_math_catch2.cpp`                     | C++ / Catch2   |
| 3 | Doctest parallel suite               | `tests/doctest/test_math_doctest.cpp`                   | C++ / Doctest  |
| 4 | SVR entry                            | Test results and coverage numbers in SVR-SF-2026-001    | Markdown       |
| 5 | Requirements traceability matrix     | Requirement → test case → result mapping                | Markdown/CSV   |
| 6 | MC/DC analysis document              | Per-decision truth tables and test case assignments     | Markdown       |
| 7 | Coverage report                      | Statement, Decision, MC/DC coverage for both modules    | lcov / HTML    |

---

## 8. Acceptance Criteria

All of the following must be satisfied before TAR-SF-001 can be closed:

1. **All tests pass** — zero failures, zero errors across all three frameworks.
2. **Statement coverage >= 100%** for `FGColumnVector3.h`, `FGColumnVector3.cpp`, `FGMatrix33.h`, `FGMatrix33.cpp`.
3. **Decision coverage >= 100%** for all four source files.
4. **MC/DC documented** for every multi-condition decision identified in Section 6.
5. **No dead code** — every reachable statement is exercised; any unreachable code is documented and justified.
6. **All source code traces to requirements** — no orphan code (code with no requirement justification).
7. **Human review completed** — independent review required per DAL A (DO-178C Section 6.3.4, Table A-7).
8. **Traceability matrix complete** — every requirement maps to at least one test case; every test case maps to at least one requirement.

---

## 9. Estimated Effort

| Phase                  | Description                                           | Effort Estimate      | Status       |
|------------------------|-------------------------------------------------------|----------------------|--------------|
| Phase 1 — Intake       | TAR creation, scope definition                        | 1 session            | **Complete** |
| Phase 2 — Plan         | Detailed test plan with MC/DC pairs, test case IDs    | 1 session            | Not Started  |
| Phase 3 — Generate     | Test code generation, compilation, initial debug      | 2–3 sessions         | Not Started  |
| Phase 4 — Validate     | Run tests, coverage analysis, gap identification      | 1 session            | Not Started  |
| Phase 5 — Document     | SVR entry, traceability matrix, MC/DC analysis report | 1 session            | Not Started  |

**Total estimated:** 6–7 sessions

---

## 10. Dependencies

| Dependency                              | Status       | Notes                                    |
|-----------------------------------------|--------------|------------------------------------------|
| PSAC baselined (PSAC-SF-2026-001)       | Complete     |                                          |
| SDP baselined (SDP-SF-2026-001)         | Complete     |                                          |
| SVP baselined (SVP-SF-2026-001)         | Complete     |                                          |
| SCMP baselined (SCMP-SF-2026-001)       | Complete     |                                          |
| SQAP baselined (SQAP-SF-2026-001)       | Complete     |                                          |
| SRS-Std baselined                       | Complete     |                                          |
| SDS baselined                           | Complete     |                                          |
| SCS baselined (SCS-SF-2026-001)         | Complete     |                                          |
| SRS requirements available              | Complete     | REQ-SF-0200, 0103, 1300, 1400, 1401     |
| Build system functional                 | Complete     | CMake + GCC toolchain                    |
| CI/CD operational                       | Complete     |                                          |
| No upstream TAR dependencies            | N/A          | This is the first TAR in the stack       |

---

## 11. Risks

| # | Risk                                                                                         | Severity | Mitigation                                                                                          |
|---|----------------------------------------------------------------------------------------------|----------|------------------------------------------------------------------------------------------------------|
| 1 | Existing tests may use 0-based indexing (found in `/mnt/f/goose/FGColumnVector3Test.cpp` — known incorrect, not in repo) | High     | All new tests will use 1-based indexing per JSBSim API; review existing tests for indexing errors    |
| 2 | `GetEuler` gimbal lock boundary at `0.9999999` needs careful tolerance analysis              | Medium   | Test at exact boundary, boundary +/- epsilon, and at +-1.0; document numerical sensitivity          |
| 3 | `initializer_list` assignment with < 3 elements leaves remaining elements unchanged          | Medium   | Verify behavior and determine if defect or design intent; document finding either way               |
| 4 | Cross-platform floating-point behavior may vary                                              | Low      | Use `EXPECT_DOUBLE_EQ` / `EXPECT_NEAR` with appropriate tolerances; document platform assumptions   |
| 5 | MC/DC test count may increase if implementation contains additional hidden decisions          | Low      | Review generated assembly or compiler output if source-level analysis is insufficient               |

---

## 12. Revision History

| Rev | Date       | Author                       | Description              |
|-----|------------|------------------------------|--------------------------|
| 1.0 | 2026-03-14 | Development Team (LLM-Assisted) | Initial TAR creation  |
