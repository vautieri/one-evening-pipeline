# Software Verification Plan

**Document ID:** SVP-SF-2026-001
**Project:** SynthFlight — UAV Mission Simulation and Verification Platform
**Design Assurance Level:** A (Catastrophic)
**Applicable Standard:** DO-178C / ED-12C
**Coding Standard:** C++17 (2023), GCC
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Draft

---

## Revision History

| Version | Date       | Author            | Description               |
|---------|------------|-------------------|---------------------------|
| 1.0     | 2026-03-14 | SynthFlight Team  | Initial release            |

---

## Table of Contents

1. [Purpose and Scope](#1-purpose-and-scope)
2. [Reference Documents](#2-reference-documents)
3. [Verification Organization](#3-verification-organization)
4. [Verification Process Overview](#4-verification-process-overview)
5. [Software Reviews](#5-software-reviews)
6. [Software Testing](#6-software-testing)
7. [Requirements-Based Test Coverage](#7-requirements-based-test-coverage)
8. [Structural Coverage Analysis](#8-structural-coverage-analysis)
9. [Test Environment](#9-test-environment)
10. [Verification of Previously Developed Software](#10-verification-of-previously-developed-software)
11. [Problem Reporting](#11-problem-reporting)
12. [Verification Completion Criteria](#12-verification-completion-criteria)
13. [Verification Results Documentation](#13-verification-results-documentation)
14. [Earned Value Tracking](#14-earned-value-tracking)

---

## 1. Purpose and Scope

### 1.1 Purpose

This Software Verification Plan (SVP) defines the verification activities, methods, procedures, and tools used to verify the SynthFlight UAV Mission Simulation and Verification Platform software. It establishes the strategies, responsibilities, and criteria necessary to demonstrate that the software satisfies its requirements and that the verification process achieves the objectives mandated by DO-178C for Design Assurance Level A (Catastrophic).

At DAL A, this plan is the most critical lifecycle document because it must demonstrate that every verification objective has been met, including Modified Condition/Decision Coverage (MC/DC) — the most rigorous structural coverage criterion defined by DO-178C.

### 1.2 Scope

This plan covers all verification activities for the SynthFlight DAL A software, including:

- Verification of high-level requirements against system requirements
- Verification of low-level requirements against high-level requirements
- Verification of source code against low-level requirements
- Verification of the executable object code against requirements
- Integration testing of software components
- Requirements-based testing (normal range, boundary, robustness, error handling)
- Structural coverage analysis (statement, decision, and MC/DC)
- Verification of test cases, test procedures, and test results
- Review of verification tool qualification evidence

The scope encompasses all current source modules:

| Module                      | Description                          |
|-----------------------------|--------------------------------------|
| FGColumnVector3             | 3D vector mathematics                |
| FGMatrix33                  | 3x3 matrix operations                |
| FGAtmosphereBase            | ISA base atmosphere model            |
| FGStandardAtmosphere1976    | U.S. Standard Atmosphere 1976 model  |

All 149 requirements across 19 categories (REQ-SF-0100 through REQ-SF-1904) defined in SRS-SF-2026-001 are within scope. Additional modules introduced during development shall be subject to the same verification regime defined herein.

### 1.3 Applicability

This plan applies to all personnel performing verification activities on SynthFlight software, including developers, independent verifiers, quality assurance engineers, and any personnel operating verification tools.

---

## 2. Reference Documents

| Document ID        | Title                                                        | Version |
|--------------------|--------------------------------------------------------------|---------|
| PSAC-SF-2026-001   | Plan for Software Aspects of Certification — SynthFlight     | 1.0     |
| SDP-SF-2026-001    | Software Development Plan — SynthFlight                      | 1.0     |
| SCMP-SF-2026-001   | Software Configuration Management Plan — SynthFlight         | 1.0     |
| SQAP-SF-2026-001   | Software Quality Assurance Plan — SynthFlight                | 1.0     |
| SRS-SF-2026-001    | Software Requirements Specification — SynthFlight            | 1.0     |
| —                  | RTCA DO-178C, Software Considerations in Airborne Systems    | —       |
|                    | and Equipment Certification                                  |         |
| —                  | RTCA DO-330, Software Tool Qualification Considerations      | —       |
| —                  | RTCA DO-248C, Supporting Information for DO-178C and DO-278A | —       |
| —                  | AC 20-115D, Airborne Software Development Assurance          | —       |
|                    | Using EUROCAE ED-12 and RTCA DO-178                          |         |

---

## 3. Verification Organization

### 3.1 Roles and Responsibilities

| Role                    | Responsibilities                                                                                   |
|-------------------------|-----------------------------------------------------------------------------------------------------|
| Software Developer      | Develops source code, writes initial unit tests, performs self-review prior to submission            |
| Independent Verifier    | Reviews requirements, design, code, and test artifacts; executes verification activities             |
| Quality Assurance (QA)  | Audits verification processes, confirms compliance with this plan and DO-178C objectives            |
| Verification Lead       | Coordinates verification activities, manages verification schedule, resolves verification issues     |
| Configuration Manager   | Controls baselines of verification artifacts, manages problem report tracking                        |

### 3.2 Independence Requirements for DAL A

DO-178C Section 6.3.a mandates verification independence for DAL A software. The following independence rules apply:

1. **Code review independence:** The person reviewing source code shall not be the person who wrote the code. A developer may not approve their own code for integration.

2. **Test independence:** The person who designs and executes verification test cases for a given module shall not be the primary developer of that module. Co-developers who contributed substantive logic to the module under test are also excluded from serving as independent verifier for that module.

3. **Test result review:** An independent verifier shall review all test results, coverage reports, and verification summary reports before a module is declared verified.

4. **Requirements review independence:** High-level and low-level requirements shall be reviewed by personnel who did not author those requirements.

5. **LLM-assisted development independence:** When AI/LLM tools are used to assist in code generation or test generation, the following additional rules apply:
   - AI-generated code is treated equivalently to developer-written code for independence purposes; a human independent verifier must review all AI-generated artifacts.
   - AI-generated test cases must be reviewed by a human verifier who confirms test adequacy, correct oracle values, and proper traceability to requirements.
   - The human who prompted the AI to generate code or tests is considered the "author" for independence purposes and may not serve as the independent verifier of that artifact.
   - All AI-generated artifacts shall be annotated in configuration management records to maintain traceability of origin.

### 3.3 Independence Matrix

| Activity                     | Author/Developer | Independent Verifier | QA Audit |
|------------------------------|:----------------:|:--------------------:|:--------:|
| Requirements authoring       | X                |                      |          |
| Requirements review          |                  | X                    | X        |
| Design authoring             | X                |                      |          |
| Design review                |                  | X                    | X        |
| Code development             | X                |                      |          |
| Code review                  |                  | X                    | X        |
| Unit test development        | X                |                      |          |
| Unit test review             |                  | X                    | X        |
| Test execution               | X or IV          | X                    |          |
| Test result review           |                  | X                    | X        |
| Coverage analysis review     |                  | X                    | X        |
| Problem report disposition   | X                | X                    | X        |

---

## 4. Verification Process Overview

Verification is conducted at each phase of the software lifecycle to confirm that outputs satisfy the inputs and objectives of that phase. The five principal verification activities are:

### 4.1 Requirements Verification (Reviews)

Verification that high-level requirements (HLR) are correct, complete, consistent, traceable to system requirements, and verifiable. Verification that low-level requirements (LLR) are correct, complete, consistent, traceable to HLRs, and verifiable.

**Objectives (DO-178C Table A-3):**
- High-level requirements comply with system requirements
- High-level requirements are accurate and consistent
- High-level requirements are compatible with target computer
- High-level requirements are verifiable
- High-level requirements conform to standards
- High-level requirements are traceable to system requirements
- Algorithms are accurate

### 4.2 Design Verification (Reviews)

Verification that the software architecture and low-level design correctly and completely implement the high-level requirements.

**Objectives (DO-178C Table A-4):**
- Low-level requirements comply with high-level requirements
- Low-level requirements are accurate and consistent
- Low-level requirements are compatible with target computer
- Low-level requirements are verifiable
- Low-level requirements conform to standards
- Low-level requirements are traceable to high-level requirements
- Algorithms are accurate
- Software architecture is compatible with high-level requirements
- Software architecture is consistent
- Software architecture is compatible with target computer
- Software architecture conforms to standards

### 4.3 Code Verification (Reviews + Static Analysis)

Verification that source code correctly implements the low-level requirements, conforms to the C++17 (2023) coding standard, and contains no unintended functionality.

**Objectives (DO-178C Table A-5):**
- Source code complies with low-level requirements
- Source code complies with software architecture
- Source code is verifiable
- Source code conforms to standards (C++17 coding standard)
- Source code is traceable to low-level requirements
- Source code is accurate and consistent
- Output of software integration process is complete and correct

Static analysis tools (CodeQL via GitHub Actions, compiler warnings via GCC `-Wall -Wextra -Wpedantic`) supplement manual code reviews.

### 4.4 Integration Verification (Testing)

Verification that software components interact correctly when integrated and that the integrated software operates correctly in the target environment.

### 4.5 Software Testing (Requirements-Based + Structural Coverage)

The primary verification activity. Requirements-based testing demonstrates that the software meets its requirements. Structural coverage analysis confirms that the test cases exercise the code structure adequately for DAL A. See Sections 6, 7, and 8 for detailed testing procedures.

---

## 5. Software Reviews

### 5.1 Requirements Review

**Input:** SRS-SF-2026-001 (149 requirements, 19 categories)

**Requirements Review Checklist (SRD Review):**

| # | Criterion                                                                | Pass/Fail |
|---|--------------------------------------------------------------------------|-----------|
| 1 | Each requirement has a unique identifier (REQ-SF-XXXX)                   |           |
| 2 | Each requirement is stated in a single, unambiguous sentence             |           |
| 3 | Each requirement is verifiable (can be demonstrated by test or analysis) |           |
| 4 | Each requirement is traceable to a system-level requirement or rationale |           |
| 5 | Requirements are free of implementation bias                             |           |
| 6 | Requirements are internally consistent (no contradictions)               |           |
| 7 | Requirements use defined terminology consistently                        |           |
| 8 | Boundary conditions and tolerances are explicitly stated                 |           |
| 9 | Error handling behavior is specified for abnormal inputs                 |           |
| 10| Performance and accuracy requirements include quantitative criteria      |           |
| 11| Requirements cover normal, abnormal, and boundary operating conditions   |           |
| 12| Derived requirements are identified and justified                        |           |

**Review Process:**
1. The independent verifier completes the checklist for each requirement category (REQ-SF-0100 through REQ-SF-1904).
2. Discrepancies are recorded as Discrepancy Reports (DRs).
3. The review is documented in the Software Verification Results (SVR).

### 5.2 Design Review

**Input:** Software Design Description (SDD)

**Design Review Checklist (SDD Review):**

| # | Criterion                                                                  | Pass/Fail |
|---|----------------------------------------------------------------------------|-----------|
| 1 | Each design element traces to one or more requirements                     |           |
| 2 | The architecture supports all high-level requirements                      |           |
| 3 | Data flow is fully defined for all interfaces                              |           |
| 4 | Control flow is defined and deterministic                                  |           |
| 5 | Error handling paths are defined for all failure modes                     |           |
| 6 | Algorithms are mathematically correct and numerically stable               |           |
| 7 | Resource usage (memory, CPU) is bounded and analyzed                       |           |
| 8 | No unintended functionality is introduced                                  |           |
| 9 | Design conforms to software architecture constraints                       |           |
| 10| Derived low-level requirements are identified and traced                   |           |

### 5.3 Code Review

**Input:** Source code for each module (FGColumnVector3, FGMatrix33, FGAtmosphereBase, FGStandardAtmosphere1976)

**Code Review Criteria (SCS Compliance + Peer Review):**

| # | Criterion                                                                       | Pass/Fail |
|---|---------------------------------------------------------------------------------|-----------|
| 1 | Code conforms to C++17 (2023) coding standard                                   |           |
| 2 | Code compiles without warnings under `-Wall -Wextra -Wpedantic`                 |           |
| 3 | Each function implements exactly the behavior specified in its LLR               |           |
| 4 | No dead code (unreachable statements, unused variables, unused functions)        |           |
| 5 | No unintended functionality beyond what the requirements specify                 |           |
| 6 | Floating-point arithmetic accounts for precision limitations                     |           |
| 7 | Resource allocation (memory) is bounded; no unbounded dynamic allocation        |           |
| 8 | Error handling is consistent with requirements                                   |           |
| 9 | Naming conventions are consistent and descriptive                                |           |
| 10| No use of undefined or implementation-defined behavior                           |           |
| 11| All source code is traceable to low-level requirements                           |           |

**Peer Review Process:**
1. Developer submits code via pull request on GitHub.
2. An independent verifier (not the author) reviews the code against the checklist.
3. CodeQL static analysis results are reviewed as part of the code review.
4. All findings are dispositioned as DRs or documented as accepted.

### 5.4 Test Review

**Input:** Test cases, test procedures, test results, coverage reports

**Test Review Checklist:**

| # | Criterion                                                                                | Pass/Fail |
|---|------------------------------------------------------------------------------------------|-----------|
| 1 | Every requirement in SRS-SF-2026-001 has at least one corresponding test case            |           |
| 2 | Test cases include normal range, boundary, and robustness conditions                     |           |
| 3 | Test oracles (expected results) are independently derived, not copied from code           |           |
| 4 | Floating-point comparisons use EXPECT_NEAR with documented tolerance (never EXPECT_EQ)   |           |
| 5 | Test naming follows convention: TEST_F(Module_Function, ConditionUnderTest_ExpectedBehavior) |        |
| 6 | Each test includes traceability comment: `// Verifies REQ-SF-XXXX`                       |           |
| 7 | Structural coverage targets are met (statement, decision, MC/DC all at 100%)             |           |
| 8 | Dead code or uncovered code is justified or removed                                      |           |
| 9 | Test procedures are repeatable and deterministic                                         |           |
| 10| Test environment is documented and reproducible                                          |           |

---

## 6. Software Testing

### 6.1 Test Categories

All software testing shall be conducted in the following execution order. Each category serves a distinct verification purpose and together they provide comprehensive coverage of the requirements space.

#### a. Normal Range Tests

**Purpose:** Verify that the software produces correct outputs for inputs within the specified operating range.

**Method:** Exercise each requirement with nominal input values that represent typical operating conditions. Expected outputs are derived from the requirements specification and, where applicable, independently computed reference values.

**Examples:**
- Vector addition with typical 3D coordinates
- Matrix multiplication with well-conditioned matrices
- Atmosphere model queries at standard flight altitudes (0 m to 11,000 m)

#### b. Boundary Value Tests

**Purpose:** Verify correct behavior at the edges of input domains and at transition points in piecewise functions.

**Method:** For each input parameter, test at:
- Minimum valid value
- Just above minimum valid value
- Nominal value
- Just below maximum valid value
- Maximum valid value
- Transition boundaries (e.g., troposphere/tropopause boundary at 11,000 m)

**Examples:**
- Zero vector operations
- Matrix operations with identity matrix
- Atmosphere model at layer boundaries (0 m, 11,000 m, 20,000 m, 32,000 m)
- Temperature and pressure at sea level (h = 0)

#### c. Robustness Tests

**Purpose:** Verify that the software handles abnormal, invalid, and extreme inputs without undefined behavior.

**Method:** Exercise the software with:
- NaN (Not-a-Number) inputs
- Positive and negative infinity
- Numeric overflow / underflow conditions
- Null or uninitialized data (where applicable)
- Subnormal (denormalized) floating-point values
- Extremely large and extremely small magnitudes

**Examples:**
- Vector operations with NaN components
- Matrix inversion of singular matrix (determinant = 0)
- Atmosphere query at negative altitude
- Atmosphere query beyond model ceiling (> 84,852 m)

#### d. Error Handling Tests

**Purpose:** Verify that error detection, reporting, and recovery mechanisms function as specified in the requirements.

**Method:** Deliberately inject error conditions and verify that:
- Errors are detected
- Error indicators or return codes are set correctly
- The system transitions to a safe state or returns a defined fallback value
- No corruption of internal state occurs

#### e. DR Regression Tests

**Purpose:** Verify that defects identified via Discrepancy Reports (DRs) have been corrected and that the corrections do not introduce new defects.

**Method:** For each closed DR:
- A regression test is created that reproduces the original defect
- The test verifies the defect is corrected
- Surrounding functionality is re-verified to confirm no regression

**Traceability:** Each regression test references the DR number: `// Regression test for DR-XXXX`

#### f. State/Sequence Tests

**Purpose:** Verify correct behavior for operations that depend on state or execution order.

**Method:** Exercise sequences of operations to verify:
- Initialization sequences produce correct initial state
- State transitions follow the specified state model
- Operations applied in different orders produce correct results (or correctly detect invalid sequences)
- Accumulated state (e.g., successive atmospheric layer computations) maintains precision

### 6.2 Test Naming Convention

All test cases shall follow the naming convention:

```
TEST_F(Module_Function, ConditionUnderTest_ExpectedBehavior)
```

**Examples:**
```cpp
TEST_F(FGColumnVector3_Magnitude, UnitVectorInput_ReturnsOne)
TEST_F(FGMatrix33_Determinant, IdentityMatrix_ReturnsOne)
TEST_F(FGMatrix33_Inverse, SingularMatrix_ReturnsZeroMatrix)
TEST_F(FGStandardAtmosphere1976_Temperature, SeaLevel_Returns288Point15K)
TEST_F(FGStandardAtmosphere1976_Pressure, TropopauseBoundary_ReturnsContinuousValue)
```

For test frameworks that do not use `TEST_F` (e.g., Catch2 `TEST_CASE`, Doctest `TEST_CASE`), the equivalent naming pattern shall be applied within the test case name string:

```cpp
TEST_CASE("FGColumnVector3_Magnitude: UnitVectorInput_ReturnsOne")
```

### 6.3 Test Traceability

Every test case shall trace to at least one requirement in SRS-SF-2026-001 via a comment at the top of the test body:

```cpp
TEST_F(FGColumnVector3_Magnitude, UnitVectorInput_ReturnsOne) {
    // Verifies REQ-SF-0101
    FGColumnVector3 v(1.0, 0.0, 0.0);
    EXPECT_NEAR(v.Magnitude(), 1.0, 1e-12);
}
```

A test may verify multiple requirements:
```cpp
// Verifies REQ-SF-0101, REQ-SF-0102
```

Tests that verify derived requirements (requirements not explicitly in the SRS but necessary for implementation) shall reference the parent requirement and note the derivation:
```cpp
// Verifies REQ-SF-0101 (derived: intermediate normalization step)
```

### 6.4 Floating-Point Comparison Policy

**Mandatory rule:** Floating-point comparisons in test assertions shall never use exact equality (`EXPECT_EQ`, `ASSERT_EQ`, `==`). All floating-point comparisons shall use tolerance-based comparison with a documented rationale for the tolerance value.

**Google Test:**
```cpp
EXPECT_NEAR(actual, expected, tolerance);  // REQUIRED
// EXPECT_EQ(actual, expected);            // PROHIBITED for floating-point
```

**Catch2:**
```cpp
REQUIRE(actual == Catch::Approx(expected).margin(tolerance));
```

**Doctest:**
```cpp
CHECK(actual == doctest::Approx(expected).epsilon(tolerance));
```

**Tolerance Selection Guidance:**

| Context                                    | Typical Tolerance     | Rationale                                       |
|--------------------------------------------|-----------------------|-------------------------------------------------|
| Direct arithmetic (add, subtract)          | 1e-15 to 1e-12       | Near machine epsilon for double                  |
| Transcendental functions (exp, log, pow)   | 1e-12 to 1e-9        | Accumulated rounding in multi-step computation   |
| Iterative algorithms                       | 1e-9 to 1e-6         | Convergence tolerance of the algorithm           |
| Physical model outputs (atmosphere)        | 1e-6 to 1e-3         | Model accuracy relative to reference tables      |

Each tolerance shall be documented with its rationale in a comment or in the test documentation.

---

## 7. Requirements-Based Test Coverage

### 7.1 Coverage Objectives

Every requirement in SRS-SF-2026-001 shall have at least one test case that exercises the requirement under normal conditions and at least one test case that exercises the requirement under robustness conditions (abnormal or boundary inputs).

**Coverage targets:**
- 100% of requirements covered by at least one normal-range test
- 100% of requirements covered by at least one boundary-value test (where boundaries are defined)
- 100% of requirements covered by at least one robustness test
- 100% of error-handling requirements covered by error-injection tests

### 7.2 Requirement Coverage Matrix

The requirement coverage matrix maps each requirement to its verifying test cases. This matrix is maintained as a living artifact and is reviewed at each verification milestone.

**Matrix format:**

| Requirement   | Category        | Normal Test(s)          | Boundary Test(s)        | Robustness Test(s)     | Status     |
|---------------|-----------------|-------------------------|-------------------------|------------------------|------------|
| REQ-SF-0100   | Vector Basics   | (test case IDs)         | (test case IDs)         | (test case IDs)        | Verified   |
| REQ-SF-0101   | Vector Basics   | (test case IDs)         | (test case IDs)         | (test case IDs)        | Verified   |
| ...           | ...             | ...                     | ...                     | ...                    | ...        |
| REQ-SF-1904   | (Category 19)   | (test case IDs)         | (test case IDs)         | (test case IDs)        | Verified   |

The complete matrix spanning all 149 requirements across 19 categories (REQ-SF-0100 through REQ-SF-1904) shall be maintained in the Software Verification Results (SVR) document.

### 7.3 Traceability Analysis

Bidirectional traceability shall be established and verified:

1. **Forward traceability (requirement to test):** Every requirement shall trace forward to at least one test case. Requirements without test coverage shall be flagged as verification gaps.

2. **Backward traceability (test to requirement):** Every test case shall trace backward to at least one requirement via the `// Verifies REQ-SF-XXXX` comment. Tests without requirement traceability shall be reviewed for relevance — they may indicate a missing requirement (which must be added as a derived requirement) or an unnecessary test.

3. **Traceability verification:** An automated script shall parse test source files for `// Verifies REQ-SF-XXXX` comments and cross-reference them against the SRS requirement list to identify gaps in both directions.

---

## 8. Structural Coverage Analysis

This section defines the most critical verification activity for DAL A software. DO-178C Section 6.4.4.2 requires that structural coverage analysis be performed on the source code to confirm that requirements-based test cases have adequately exercised the code structure. For DAL A, all three levels of structural coverage are required.

### 8.1 Statement Coverage

**Objective:** Every executable statement in the source code shall be exercised by at least one test case.

**Target:** 100%

**Method:**
- The lcov tool (with lcov 2.x compatibility) instruments the compiled code via GCC's `--coverage` (`-fprofile-arcs -ftest-coverage`) flags.
- After executing all test suites, `lcov` collects execution counts for every source line.
- A line with an execution count of 0 indicates uncovered code that must be analyzed.

**Acceptance criteria:** Every executable statement in all source modules (FGColumnVector3, FGMatrix33, FGAtmosphereBase, FGStandardAtmosphere1976) shall show a non-zero execution count. Any statement with a zero count shall be dispositioned per the dead code policy (Section 8.5).

### 8.2 Decision Coverage

**Objective:** Every decision (branch point) in the source code shall be exercised with both TRUE and FALSE outcomes.

**Target:** 100%

**Decision points include:**
- `if` / `else if` / `else` statements
- `switch` / `case` statements (each case, including default)
- Ternary operators (`? :`)
- Loop conditions (`for`, `while`, `do-while`) — both entering and not entering the loop body
- Short-circuit evaluation points in logical expressions (`&&`, `||`)

**Method:**
- lcov branch coverage mode (`--rc lcov_branch_coverage=1`) collects decision-level coverage data.
- Each decision point reports the number of times each branch was taken.
- A branch with a zero-taken count indicates a coverage gap.

**Acceptance criteria:** Every branch of every decision point shall show a non-zero taken count. Any untaken branch shall be dispositioned per the dead code policy (Section 8.5).

### 8.3 MC/DC — Modified Condition/Decision Coverage

**Objective:** For every decision in the source code containing multiple conditions, each condition shall be shown to independently affect the outcome of the decision.

**Target:** 100%

This is the distinguishing structural coverage requirement for DAL A. MC/DC goes beyond decision coverage by requiring that each individual condition (Boolean sub-expression) within a compound decision be demonstrated to independently influence the decision outcome.

#### 8.3.1 MC/DC Definitions

- **Condition:** A Boolean sub-expression that cannot be decomposed further (an atomic Boolean expression). Examples: `A`, `B`, `x > 0`, `isValid()`.
- **Decision:** A Boolean expression composed of one or more conditions, connected by logical operators, that controls a program's flow. Example: `if (A && B || C)`.
- **Independence:** A condition C independently affects a decision D if there exist two test cases that differ only in the value of C and produce different outcomes for D, while all other conditions are held fixed.

#### 8.3.2 MC/DC Methodology

For a decision with N conditions, a minimum of N+1 test cases are required to demonstrate MC/DC.

**Example — Decision: `if (A && B)`**

This decision has 2 conditions (A, B), requiring a minimum of 3 test cases:

| Test Case | A     | B     | A && B (Outcome) | Purpose                                |
|-----------|-------|-------|-------------------|----------------------------------------|
| TC1       | TRUE  | TRUE  | TRUE              | Baseline: both conditions true         |
| TC2       | FALSE | TRUE  | FALSE             | Shows A independently affects outcome  |
| TC3       | TRUE  | FALSE | FALSE             | Shows B independently affects outcome  |

- **A's independence:** TC1 vs TC2 — A changes (T to F), B is fixed (T), outcome changes (T to F).
- **B's independence:** TC1 vs TC3 — B changes (T to F), A is fixed (T), outcome changes (T to F).

**Example — Decision: `if (A || B)`**

| Test Case | A     | B     | A \|\| B (Outcome) | Purpose                                |
|-----------|-------|-------|---------------------|----------------------------------------|
| TC1       | FALSE | FALSE | FALSE               | Baseline: both conditions false        |
| TC2       | TRUE  | FALSE | TRUE                | Shows A independently affects outcome  |
| TC3       | FALSE | TRUE  | TRUE                | Shows B independently affects outcome  |

**Example — Decision: `if (A && B || C)` (3 conditions, minimum 4 test cases)**

| Test Case | A     | B     | C     | Outcome | Demonstrates            |
|-----------|-------|-------|-------|---------|-------------------------|
| TC1       | TRUE  | TRUE  | FALSE | TRUE    | Baseline                |
| TC2       | FALSE | TRUE  | FALSE | FALSE   | A independently affects |
| TC3       | TRUE  | FALSE | FALSE | FALSE   | B independently affects |
| TC4       | FALSE | FALSE | TRUE  | TRUE    | C independently affects |

#### 8.3.3 MC/DC Analysis Process

1. **Identify all decisions with multiple conditions** in the source code. Document each decision with its source file, line number, and the conditions it contains.

2. **For each decision, construct the MC/DC truth table** showing the required independence pairs for each condition.

3. **Map existing test cases** to truth table rows. Determine which test cases exercise which condition combinations.

4. **Identify gaps** where a condition's independence has not been demonstrated.

5. **Add targeted test cases** to fill gaps. Each new test case shall trace to a requirement (or a derived requirement if none exists).

6. **Document the MC/DC analysis** in the coverage analysis section of the SVR, showing the independence pairs for each decision.

#### 8.3.4 MC/DC Coverage Tools

- **Primary tool:** lcov for statement and decision coverage data collection.
- **MC/DC analysis:** Manual analysis supplemented by custom scripts, or a specialized MC/DC coverage tool if qualified per DO-330. Where manual analysis is performed, the analysis shall be documented in the SVR with sufficient detail for independent review.
- **Short-circuit evaluation:** C++ short-circuit evaluation of `&&` and `||` means that not all condition combinations are reachable in a single expression evaluation. The MC/DC analysis shall account for short-circuit semantics and demonstrate independence using only reachable combinations.

### 8.4 Coverage Analysis Workflow

```
1. Compile all source modules with --coverage flags
       |
2. Execute all test suites (GTest, Catch2, Doctest, Boost.Test, CppUnit)
       |
3. Collect coverage data (lcov --capture)
       |
4. Generate coverage report (lcov / genhtml)
       |
5. Analyze statement coverage — identify lines with 0 execution count
       |
6. Analyze decision coverage — identify branches with 0 taken count
       |
7. Perform MC/DC analysis — verify independence for each multi-condition decision
       |
8. Disposition any gaps (add tests, justify, or remove dead code)
       |
9. Re-execute and re-collect until all targets are met
       |
10. Document results in SVR
```

### 8.5 Dead Code Policy

Any source code that is not executed by any test case after all requirements-based tests have been run is classified as potential dead code.

**Disposition categories:**

| Category            | Definition                                                       | Required Action                                    |
|---------------------|------------------------------------------------------------------|----------------------------------------------------|
| Unreachable code    | Code that cannot be reached by any feasible execution path       | Remove from source; document removal in DR         |
| Defensive code      | Code reachable only under hardware fault or undefined conditions | Justify retention; add analysis-based coverage note |
| Deactivated code    | Code intentionally disabled (e.g., debug-only, feature flags)   | See Section 8.6                                    |
| Coverage gap        | Code that should be reachable but is not exercised               | Add test cases to exercise the code                |

All dead code dispositions shall be reviewed by the independent verifier and documented in the SVR.

### 8.6 Deactivated Code Handling

Deactivated code is code that is present in the executable but is not intended to execute in the current configuration. For DAL A, DO-178C requires that:

1. Deactivated code shall be identified and documented.
2. The means of deactivation shall be verified (e.g., compile-time `#ifdef`, runtime configuration).
3. Deactivated code shall not be inadvertently activated.
4. If deactivated code can be activated, it shall be verified to the same DAL as the active code.
5. Structural coverage analysis shall account for deactivated code — it shall either be excluded from coverage metrics (with justification) or verified.

---

## 9. Test Environment

### 9.1 Unit Test Frameworks

SynthFlight employs five unit test frameworks to provide framework diversity, broad language feature coverage, and risk mitigation against framework-specific defects.

| Framework       | Version | Primary Use                                                      |
|-----------------|---------|------------------------------------------------------------------|
| Google Test     | 1.15.2  | Primary unit testing framework; parameterized tests, fixtures    |
| Catch2          | 3.7.1   | BDD-style tests, property-based testing, alternative assertions  |
| Doctest         | 2.4.11  | Lightweight header-only tests, rapid compile-time tests          |
| Boost.Test      | —       | Integration with Boost libraries, data-driven tests              |
| CppUnit         | —       | Legacy compatibility, JUnit-style XML reporting                  |

**Rationale for multi-framework approach:**
1. **Framework independence:** A defect in one test framework does not invalidate all test results. Cross-framework agreement on test outcomes increases confidence.
2. **Feature coverage:** Each framework offers distinct capabilities (parameterized tests, BDD sections, data generators) that together provide more thorough verification than any single framework.
3. **Tool qualification risk mitigation:** If a tool qualification concern arises for one framework, results from other frameworks provide independent corroboration.
4. **Standard compliance:** The multi-framework approach supports the DO-178C objective that verification tools shall not mask errors — diverse tools reduce the risk of systematic tool error.

### 9.2 Fuzz Testing

Fuzz testing is performed using libFuzzer with AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) enabled to discover robustness defects, memory safety violations, and undefined behavior.

**Fuzz targets:**

| Target               | Module(s) Under Test                          | Input Domain                          |
|----------------------|-----------------------------------------------|---------------------------------------|
| fuzz_vector_ops      | FGColumnVector3                                | Arbitrary 3D vector components        |
| fuzz_matrix_ops      | FGMatrix33                                     | Arbitrary 3x3 matrix elements         |
| fuzz_atmosphere      | FGAtmosphereBase, FGStandardAtmosphere1976     | Arbitrary altitude values             |

**Fuzz testing parameters:**
- Minimum corpus: 10,000 inputs per target
- Minimum execution time: 1 hour per target per CI run (extended runs for release qualification)
- Sanitizers: `-fsanitize=address,undefined -fno-sanitize-recover=all`
- Crash artifacts: Archived and converted to regression tests upon discovery

**Fuzz test results and structural coverage:** Fuzz testing contributes to structural coverage metrics. Coverage data collected during fuzz runs is merged with unit test coverage data using `lcov --add-tracefile` to produce aggregate coverage reports.

### 9.3 CI/CD Pipeline

Continuous integration and continuous delivery are implemented via GitHub Actions. The CI pipeline enforces verification gates that prevent unverified code from merging.

**Build matrix:**

| Dimension       | Values                                        |
|-----------------|-----------------------------------------------|
| Compiler         | GCC (primary)                                |
| Build type       | Debug (with coverage), Release (optimized)   |
| Sanitizer        | ASan + UBSan (Debug builds)                  |
| Coverage         | lcov (Debug builds)                          |

**CI pipeline stages:**

1. **Build:** Compile all source modules and test executables.
2. **Unit tests:** Execute all test suites (GTest, Catch2, Doctest, Boost.Test, CppUnit). Fail the pipeline if any test fails.
3. **Coverage collection:** Run lcov to collect statement and decision coverage. Fail the pipeline if coverage falls below threshold.
4. **Static analysis:** Run CodeQL. Fail the pipeline if high-severity findings are detected.
5. **Fuzz testing:** Execute fuzz targets for a minimum duration. Fail the pipeline if any crash is detected.
6. **Coverage report:** Generate HTML coverage report via genhtml. Archive as pipeline artifact.

**Coverage gate thresholds (enforced by CI):**

| Metric              | Minimum Threshold | Target   |
|---------------------|-------------------|----------|
| Statement coverage  | 95%               | 100%     |
| Decision coverage   | 95%               | 100%     |
| MC/DC coverage      | (manual analysis) | 100%     |

The CI pipeline enforces the minimum threshold as a gate; the target of 100% is enforced at verification milestones through manual review.

### 9.4 Test Tool Qualification (DO-330)

DO-178C Section 12.2 requires that verification tools whose output is used to justify elimination of, reduction of, or automation of a verification activity shall be qualified. Tool qualification level (TQL) depends on the DAL and the tool's potential impact.

**Tool qualification assessment:**

| Tool            | Role in Verification                                | Criteria | TQL  | Qualification Approach          |
|-----------------|-----------------------------------------------------|----------|------|---------------------------------|
| GCC (--coverage)| Instruments code for coverage data collection        | 5        | TQL-5| Operational history + validation |
| lcov            | Collects and reports structural coverage             | 5        | TQL-5| Operational history + validation |
| Google Test     | Executes test cases, reports pass/fail               | 5        | TQL-5| Operational history + validation |
| Catch2          | Executes test cases, reports pass/fail               | 5        | TQL-5| Operational history + validation |
| Doctest         | Executes test cases, reports pass/fail               | 5        | TQL-5| Operational history + validation |
| Boost.Test      | Executes test cases, reports pass/fail               | 5        | TQL-5| Operational history + validation |
| CppUnit         | Executes test cases, reports pass/fail               | 5        | TQL-5| Operational history + validation |
| libFuzzer       | Generates test inputs for robustness testing         | 5        | TQL-5| Operational history + validation |
| CodeQL          | Static analysis (supplements manual review)          | 5        | TQL-5| Operational history + validation |
| ASan / UBSan    | Runtime error detection during test execution        | 5        | TQL-5| Operational history + validation |

**Criteria 5 (DO-330 Table T-0):** A tool whose output is used to verify that a DO-178C objective has been met, but the tool's output can be confirmed by other means (e.g., manual review or independent tool). Errors in the tool's output could fail to detect an error in the software but would not directly introduce an error.

**Qualification evidence:** For each tool at TQL-5, the following evidence shall be maintained:
1. Tool identification (name, version, configuration)
2. Operational history demonstrating reliable use
3. Known-problem assessment (known bugs that could affect verification results)
4. Validation tests confirming correct tool operation in the project environment

---

## 10. Verification of Previously Developed Software

### 10.1 JSBSim Reuse Assessment

SynthFlight incorporates code derived from JSBSim, an open-source flight dynamics model. This code constitutes Previously Developed Software (PDS) and must be assessed per DO-178C Section 12.3.

**PDS assessment activities:**

1. **Baseline identification:** Identify the specific version, source files, and configuration of JSBSim code incorporated into SynthFlight. Establish a controlled baseline under configuration management.

2. **Service history review:** Analyze JSBSim's operational history, known defect records, and community-reported issues to assess the maturity and reliability of the reused components.

3. **Gap analysis:** Compare the verification evidence available for JSBSim against the verification objectives required by DO-178C for DAL A. Identify objectives that have not been met by existing JSBSim verification activities.

4. **Compliance determination:** For each verification objective, determine whether:
   - Existing JSBSim evidence satisfies the objective (with justification), or
   - Additional verification activities are required to close the gap.

5. **Reverse engineering (if needed):** If JSBSim code lacks requirements documentation, derive low-level requirements from the source code and verify the code against those derived requirements.

6. **Structural coverage:** JSBSim-derived code shall be subject to the same structural coverage requirements (statement, decision, MC/DC) as newly developed SynthFlight code.

### 10.2 PDS Verification Summary

The PDS verification assessment shall be documented in the SVR and shall include:
- List of all reused JSBSim source files
- Gap analysis results for each DO-178C verification objective
- Plan and schedule for closing identified gaps
- Acceptance criteria for PDS verification completion

---

## 11. Problem Reporting

### 11.1 Discrepancy Report (DR) Creation Criteria

A Discrepancy Report shall be created whenever any of the following conditions is observed during verification:

1. A test case fails (actual result differs from expected result beyond tolerance)
2. A requirement is found to be ambiguous, incomplete, incorrect, or unverifiable
3. A design element does not trace to a requirement or contradicts a requirement
4. Source code does not conform to the coding standard
5. Source code contains unintended functionality
6. Structural coverage analysis reveals unexplained gaps
7. A static analysis tool reports a finding of medium severity or higher
8. A fuzz test discovers a crash, assertion failure, or sanitizer violation
9. A verification artifact (test case, procedure, report) is found to be incorrect

### 11.2 Severity Levels

| Level    | Definition                                                                                | Response                                  |
|----------|-------------------------------------------------------------------------------------------|-------------------------------------------|
| Critical | Defect could lead to catastrophic failure or violates a safety requirement                | Immediate stop-work; resolve before any further verification |
| Major    | Defect causes incorrect functionality or violates a non-safety requirement                | Resolve before the affected module is declared verified       |
| Minor    | Defect is cosmetic, has a workaround, or affects non-functional attributes only           | Resolve before release; may defer to next verification cycle  |
| Info     | Observation or improvement suggestion; no defect in deliverable software                  | Record for consideration; no mandatory action                 |

### 11.3 Resolution Process

1. **Report:** Discoverer creates DR with description, severity, affected artifact(s), and steps to reproduce.
2. **Triage:** Verification Lead reviews and confirms severity classification.
3. **Assignment:** DR is assigned to the responsible developer or artifact owner.
4. **Investigation:** Assignee determines root cause and proposed resolution.
5. **Resolution:** Assignee implements the fix and creates a regression test (for code defects).
6. **Verification:** Independent verifier confirms the fix resolves the defect and the regression test passes.
7. **Closure:** QA reviews the DR record for completeness and closes the DR.

All DRs shall be tracked in the project's configuration management system (GitHub Issues). Open DRs shall be reviewed at each verification status meeting.

---

## 12. Verification Completion Criteria

### 12.1 Module-Level Verification Completion

A module is considered fully verified when all of the following criteria are satisfied:

1. **Requirements coverage:** Every requirement allocated to the module has at least one passing normal-range test, boundary test, and robustness test.
2. **Structural coverage (statement):** 100% statement coverage achieved, or all uncovered statements dispositioned per dead code policy.
3. **Structural coverage (decision):** 100% decision coverage achieved, or all uncovered branches dispositioned per dead code policy.
4. **Structural coverage (MC/DC):** 100% MC/DC coverage demonstrated for all multi-condition decisions, documented with independence pairs.
5. **Code review:** Independent code review completed with all findings dispositioned (no open Critical or Major DRs).
6. **Test review:** Independent test review completed confirming test adequacy.
7. **Static analysis:** No unresolved high-severity static analysis findings.
8. **Fuzz testing:** Fuzz targets executed for minimum duration with no unresolved crashes.
9. **Regression:** All DR regression tests passing.
10. **Traceability:** Bidirectional traceability verified (requirements to tests, tests to requirements).

### 12.2 Release-Level Verification Completion

A software release is considered verification-complete when:

1. All modules in the release satisfy module-level verification completion criteria.
2. Integration testing is complete with all integration tests passing.
3. All DRs are closed or deferred with documented justification and impact analysis.
4. The Software Verification Results (SVR) document is complete and reviewed by the independent verifier.
5. The requirement coverage matrix is complete with no gaps.
6. Aggregate structural coverage meets 100% for statement, decision, and MC/DC.
7. QA has audited the verification process and confirmed compliance with this plan.
8. The Verification Lead has signed the verification completion declaration.

---

## 13. Verification Results Documentation

### 13.1 Software Verification Results (SVR) Format

The SVR is the primary output of the verification process. It shall contain:

1. **Summary of verification activities:** Description of what was verified, by whom, and when.
2. **Requirements coverage matrix:** Complete mapping of requirements to test cases with pass/fail status.
3. **Structural coverage results:**
   - Statement coverage percentage and uncovered line analysis
   - Decision coverage percentage and uncovered branch analysis
   - MC/DC analysis with independence pair documentation for each multi-condition decision
4. **Test execution summary:** Total tests executed, passed, failed, skipped, with failure analysis.
5. **Static analysis results summary:** Findings categorized by severity, with disposition for each.
6. **Fuzz testing results:** Duration, corpus size, crashes found, coverage contribution.
7. **DR summary:** Open/closed counts, severity distribution, resolution status.
8. **Deviations:** Any deviations from this SVP with justification and impact analysis.
9. **Verification completion declaration:** Signed statement that verification criteria have been met.

### 13.2 Test Log Format

Each test execution shall produce a log containing:

```
Test Execution Log
==================
Date/Time:       YYYY-MM-DD HH:MM:SS UTC
Executor:        (name or CI job ID)
Test Suite:      (framework name and test suite identifier)
Build ID:        (git commit SHA)
Compiler:        GCC version
Build Type:      Debug / Release
Sanitizers:      ASan, UBSan (if enabled)

Results:
  Total:         NNN
  Passed:        NNN
  Failed:        NNN
  Skipped:       NNN
  Duration:      NNN seconds

Failed Tests:
  (list of failed test names with failure messages)
```

Test logs shall be archived as CI pipeline artifacts and retained per the SCMP retention policy.

### 13.3 Coverage Report Format

Coverage reports shall be generated in both machine-readable (lcov `.info` files) and human-readable (HTML via genhtml) formats.

**Report contents:**
- Per-file statement coverage (lines hit / lines total)
- Per-file decision (branch) coverage (branches taken / branches total)
- Per-function coverage
- Aggregate project-level coverage
- MC/DC analysis supplement (separate document or section, referencing specific source locations)

Coverage reports shall be archived as CI pipeline artifacts and included in the SVR.

---

## 14. Earned Value Tracking

### 14.1 Overview

Verification progress is tracked using Earned Value Management (EVM) methodology, aligned with the Technical Activity Record (TAR) framework. This provides objective measurement of verification progress and enables early detection of schedule or effort variances.

### 14.2 Planned Value (PV)

Planned Value represents the authorized budget assigned to scheduled verification work. PV is computed as:

```
PV = (Planned % complete of verification activities) x (Total verification budget)
```

**Verification Work Breakdown Structure (WBS) for PV calculation:**

| WBS Element                          | Weight | Description                                      |
|--------------------------------------|--------|--------------------------------------------------|
| Requirements review                  | 10%    | Review of all 149 requirements                   |
| Design review                        | 10%    | Review of SDD for all modules                    |
| Code review                          | 10%    | Independent code review of all modules            |
| Normal range test development        | 15%    | Test case development for normal conditions       |
| Boundary value test development      | 10%    | Test case development for boundary conditions     |
| Robustness test development          | 10%    | Test case development for abnormal conditions     |
| Error handling test development      | 5%     | Test case development for error paths             |
| Structural coverage analysis         | 15%    | Statement, decision, MC/DC analysis               |
| Fuzz testing                         | 5%     | Fuzz target development and execution             |
| DR resolution verification           | 5%     | Verification of defect fixes                      |
| SVR documentation                    | 5%     | Compilation of verification results               |
| **Total**                            | **100%** |                                                 |

### 14.3 Earned Value (EV)

Earned Value represents the value of verification work actually accomplished. EV is computed using the percent-complete method:

```
EV = (Actual % complete of verification activities) x (Total verification budget)
```

**Percent complete rules (objective measurement criteria):**

| Activity                        | Measurement Basis                                                    |
|---------------------------------|----------------------------------------------------------------------|
| Requirements review             | (Requirements reviewed and signed off) / (Total requirements) x 100  |
| Code review                     | (Modules reviewed and signed off) / (Total modules) x 100           |
| Test development                | (Tests written and passing) / (Total tests planned) x 100           |
| Structural coverage             | (Decisions with MC/DC demonstrated) / (Total multi-condition decisions) x 100 |
| DR resolution                   | (DRs closed) / (Total DRs opened) x 100                             |

### 14.4 Variance Analysis

The following metrics are computed at each reporting period:

| Metric                         | Formula              | Interpretation                       |
|--------------------------------|----------------------|--------------------------------------|
| Schedule Variance (SV)         | SV = EV - PV         | Positive = ahead; Negative = behind  |
| Schedule Performance Index (SPI)| SPI = EV / PV       | > 1.0 = ahead; < 1.0 = behind       |
| Cost Variance (CV)             | CV = EV - AC         | Positive = under budget; Negative = over budget |
| Cost Performance Index (CPI)   | CPI = EV / AC        | > 1.0 = under budget; < 1.0 = over budget |

Where AC = Actual Cost (effort hours expended on verification).

### 14.5 Reporting

Earned value metrics shall be reported at each TAR review cycle. Verification status shall be presented as:

```
Module: FGColumnVector3
  Requirements coverage:   100% (XX/XX requirements verified)
  Statement coverage:      XX.X%
  Decision coverage:       XX.X%
  MC/DC coverage:          XX.X%
  Open DRs:                X (Critical: X, Major: X, Minor: X)
  PV:  XX%   EV:  XX%   SPI: X.XX
```

Modules with SPI < 0.9 shall trigger a corrective action plan reviewed by the Verification Lead.

---

## Appendix A: Acronyms and Abbreviations

| Acronym   | Definition                                              |
|-----------|---------------------------------------------------------|
| ASan      | AddressSanitizer                                        |
| BDD       | Behavior-Driven Development                             |
| CI/CD     | Continuous Integration / Continuous Delivery             |
| CPI       | Cost Performance Index                                  |
| DAL       | Design Assurance Level                                  |
| DO-178C   | Software Considerations in Airborne Systems and Equipment Certification |
| DO-330    | Software Tool Qualification Considerations              |
| DR        | Discrepancy Report                                      |
| EV        | Earned Value                                            |
| EVM       | Earned Value Management                                 |
| GCC       | GNU Compiler Collection                                 |
| HLR       | High-Level Requirement                                  |
| ISA       | International Standard Atmosphere                       |
| LLM       | Large Language Model                                    |
| LLR       | Low-Level Requirement                                   |
| MC/DC     | Modified Condition/Decision Coverage                    |
| PDS       | Previously Developed Software                           |
| PSAC      | Plan for Software Aspects of Certification              |
| PV        | Planned Value                                           |
| QA        | Quality Assurance                                       |
| SCMP      | Software Configuration Management Plan                  |
| SCS       | Software Coding Standard                                |
| SDD       | Software Design Description                             |
| SDP       | Software Development Plan                               |
| SPI       | Schedule Performance Index                              |
| SQAP      | Software Quality Assurance Plan                         |
| SRS       | Software Requirements Specification                     |
| SVP       | Software Verification Plan                              |
| SVR       | Software Verification Results                           |
| TAR       | Technical Activity Record                               |
| TQL       | Tool Qualification Level                                |
| UAV       | Unmanned Aerial Vehicle                                 |
| UBSan     | UndefinedBehaviorSanitizer                              |
| WBS       | Work Breakdown Structure                                |

---

## Appendix B: DO-178C Table A-7 Verification Objectives Compliance Matrix

The following table maps each DO-178C Table A-7 verification objective to the section of this SVP that addresses it. All objectives are applicable at DAL A.

| DO-178C Objective                                                    | SVP Section | Method         |
|----------------------------------------------------------------------|-------------|----------------|
| Executable Object Code complies with HLR                             | 6, 7        | Test           |
| Executable Object Code is robust with HLR                            | 6.1c, 6.1d  | Test           |
| Executable Object Code complies with LLR                             | 6, 7        | Test           |
| Executable Object Code is robust with LLR                            | 6.1c, 6.1d  | Test           |
| Executable Object Code is compatible with target computer            | 9           | Test/Analysis  |
| Test coverage of HLR is achieved                                     | 7           | Analysis       |
| Test coverage of LLR is achieved                                     | 7           | Analysis       |
| Test coverage of software structure (statement) is achieved          | 8.1         | Analysis       |
| Test coverage of software structure (decision) is achieved           | 8.2         | Analysis       |
| Test coverage of software structure (MC/DC) is achieved              | 8.3         | Analysis       |
| Test procedures are correct                                          | 5.4         | Review         |
| Test results are correct and discrepancies explained                 | 13          | Review         |

---

*End of Document — SVP-SF-2026-001*
