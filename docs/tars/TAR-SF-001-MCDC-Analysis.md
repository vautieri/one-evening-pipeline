# MC/DC Structural Coverage Analysis Report

| Field              | Value                                                        |
|--------------------|--------------------------------------------------------------|
| Document ID        | MCDC-SF-TAR001-2026-001                                      |
| TAR Reference      | TAR-SF-001 — Math Library Verification & DAL A Coverage Enhancement |
| DAL                | A (Catastrophic) — 100% MC/DC Required                       |
| Standard           | DO-178C Section 6.4.4.2 — Structural Coverage Analysis       |
| SVP Reference      | SVP-SF-2026-001, Section 8                                   |
| Modules Under Test | FGColumnVector3 (src/math/FGColumnVector3.cpp)                |
|                    | FGMatrix33 (src/math/FGMatrix33.cpp)                         |
| Date               | 2026-03-14                                                   |
| Revision           | 1.0                                                          |
| Status             | Baseline                                                     |

---

## 1. Purpose

This document provides the formal Modified Condition/Decision Coverage (MC/DC) analysis for all decision points in the FGColumnVector3 and FGMatrix33 modules, as required by DO-178C Section 6.4.4.2 for DAL A software.

For every Boolean decision in the source code, this document:

1. Identifies the decision expression and its constituent conditions.
2. Constructs the MC/DC truth table demonstrating that each condition can independently affect the decision outcome.
3. Identifies the independence pairs that satisfy the unique-cause MC/DC criterion.
4. Specifies the minimum test cases required and maps them to test case identifiers in the test suites.
5. Documents total coverage metrics across both modules.

This analysis covers all decisions in the `.cpp` implementation files. Inline functions defined in header files (e.g., `Rows()`, `Cols()`, `Entry()`, `idx()`) contain no Boolean decisions and are therefore excluded from MC/DC analysis but are covered by statement/decision coverage through delegation to analyzed functions.

---

## 2. Applicable Documents

| Document ID          | Title                                                         |
|----------------------|---------------------------------------------------------------|
| DO-178C              | Software Considerations in Airborne Systems and Equipment Certification |
| SVP-SF-2026-001      | Software Verification Plan                                    |
| TAR-SF-001           | Math Library Verification & DAL A Coverage Enhancement        |
| SRS-SF-2026-001      | Software Requirements Specification                           |
| SCS-SF-2026-001      | Software Coding Standards                                     |
| PSAC-SF-2026-001     | Plan for Software Aspects of Certification                    |

---

## 3. Methodology

### 3.1 MC/DC Variant

This analysis uses **Unique-Cause MC/DC** as defined in DO-178C Section 6.4.4.2. Under unique-cause MC/DC, for each condition in a decision:

- There must exist a pair of test cases where:
  - The condition under test toggles (TRUE to FALSE or FALSE to TRUE).
  - All other conditions in the decision hold fixed values.
  - The overall decision outcome changes.

Masking MC/DC is **not** used in this analysis. Unique-cause MC/DC is the stricter form and provides a clearer demonstration of condition independence.

### 3.2 Minimum Test Case Count

For a decision with N Boolean conditions:
- Minimum MC/DC test cases: **N + 1**
- Single-condition decisions: Decision Coverage (2 test cases) is equivalent to MC/DC.

### 3.3 Notation

Throughout this document:
- **T** = TRUE, **F** = FALSE
- A condition labeled with its variable name (e.g., A, B, C, D) represents the truth value of that sub-expression.
- Independence pairs are written as (Test X vs Test Y) where the condition under test toggles and the decision outcome changes.

### 3.4 Unsigned Integer Consideration

The parameters `idx`, `row`, and `col` in the element access operators are declared as `unsigned int`. The value `0` is a valid `unsigned int` and triggers the `idx < 1` / `row < 1` / `col < 1` condition as TRUE. Negative integer arguments undergo implicit conversion to large unsigned values (wrap-around), which would trigger the `> 3` condition. This analysis uses only non-negative test inputs to avoid reliance on implementation-defined wrap-around behavior. See Section 9 (Findings) for further discussion.

---

## 4. Decision Inventory

### 4.1 Summary Table — FGColumnVector3

| Decision ID | Source Location                          | Line(s)  | Expression                                    | Conditions | MC/DC Cases | Instances |
|-------------|------------------------------------------|----------|-----------------------------------------------|------------|-------------|-----------|
| V1a         | `operator()` const                       | 41       | `idx < 1 \|\| idx > 3`                       | 2          | 3           | 1         |
| V1b         | `operator()` non-const                   | 48       | `idx < 1 \|\| idx > 3`                       | 2          | 3           | 1         |
| V2          | `operator/` (scalar)                     | 129      | `scalar == 0.0`                               | 1          | 2           | 1         |
| V3          | `operator/=` (scalar)                    | 145      | `scalar == 0.0`                               | 1          | 2           | 1         |
| V4          | `Normalize()`                            | 205      | `mag != 0.0`                                  | 1          | 2           | 1         |
| V5a         | `operator=` (initializer_list)           | 66       | `lv.size() >= 1`                              | 1          | 2           | 1         |
| V5b         | `operator=` (initializer_list)           | 67       | `lv.size() >= 2`                              | 1          | 2           | 1         |
| V5c         | `operator=` (initializer_list)           | 68       | `lv.size() >= 3`                              | 1          | 2           | 1         |
| V6          | `operator==`                             | 76-78    | `data[1]==b.data[1] && data[2]==b.data[2] && data[3]==b.data[3]` | 3 | 4  | 1         |
| **Total**   |                                          |          |                                               | **13**     | **23**      | **9**     |

### 4.2 Summary Table — FGMatrix33

| Decision ID | Source Location                          | Line(s)  | Expression                                                     | Conditions | MC/DC Cases | Instances |
|-------------|------------------------------------------|----------|----------------------------------------------------------------|------------|-------------|-----------|
| M1a         | `operator()` const                       | 40       | `row < 1 \|\| row > 3 \|\| col < 1 \|\| col > 3`            | 4          | 5           | 1         |
| M1b         | `operator()` non-const                   | 47       | `row < 1 \|\| row > 3 \|\| col < 1 \|\| col > 3`            | 4          | 5           | 1         |
| M2          | `Inverse()`                              | 83       | `det == 0.0`                                                   | 1          | 2           | 1         |
| M3          | `operator/` (scalar)                     | 188      | `scalar == 0.0`                                                | 1          | 2           | 1         |
| M4          | `operator/=` (scalar)                    | 204      | `scalar == 0.0`                                                | 1          | 2           | 1         |
| M5          | `Invertible()`                           | 63       | `Determinant() != 0.0`                                         | 1          | 2           | 1         |
| M6          | `GetEuler()` — clamp high               | 127      | `sinTheta >= 1.0`                                              | 1          | 2           | 1         |
| M7          | `GetEuler()` — clamp low                | 129      | `sinTheta <= -1.0`                                             | 1          | 2           | 1         |
| M8          | `GetEuler()` — gimbal lock              | 134      | `std::abs(sinTheta) < 0.9999999`                               | 1          | 2           | 1         |
| M9          | `Dump()` — delimiter                    | 273      | `j > 1`                                                        | 1          | 2           | 1         |
| M10         | `Dump()` — newline                      | 276      | `i < 3`                                                        | 1          | 2           | 1         |
| M11         | `operator==` — element inequality       | 256      | `data[i] != B.data[i]`                                         | 1          | 2           | 1         |
| **Total**   |                                          |          |                                                                | **18**     | **30**      | **12**    |

### 4.3 Grand Total

| Metric                                   | FGColumnVector3 | FGMatrix33 | Total |
|------------------------------------------|-----------------|------------|-------|
| Distinct decision points                 | 9               | 12         | 21    |
| Total Boolean conditions                 | 13              | 18         | 31    |
| Total MC/DC test cases required          | 23              | 30         | 53    |

---

## 5. Detailed Analysis — FGColumnVector3

### 5.1 Decision V1a: Const Element Access Bounds Check

**Source:** `FGColumnVector3.cpp`, line 41, function `operator()(unsigned int idx) const`

**Decision Expression:**
```cpp
if (idx < 1 || idx > 3)
```

**Conditions:**
- A: `idx < 1`
- B: `idx > 3`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**MC/DC Truth Table:**

| Test Case | idx Value | A (idx < 1) | B (idx > 3) | Decision (A \|\| B) | Purpose                   |
|-----------|-----------|-------------|-------------|----------------------|---------------------------|
| V1a-1     | 0         | T           | F           | T (throws)           | A independently forces T  |
| V1a-2     | 2         | F           | F           | F (returns element)  | Baseline — both false     |
| V1a-3     | 4         | F           | T           | T (throws)           | B independently forces T  |

**Independence Pairs:**

| Condition | Test Pair       | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-----------------|------------------|------------------|-----------------|
| A         | V1a-1 vs V1a-2  | T -> F           | B = F (fixed)    | T -> F          |
| B         | V1a-3 vs V1a-2  | T -> F           | A = F (fixed)    | T -> F          |

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- V1a-1: `FGColumnVector3Test::ConstElementAccess_IndexZero_Throws`
- V1a-2: `FGColumnVector3Test::ConstElementAccess_ValidIndex_ReturnsElement`
- V1a-3: `FGColumnVector3Test::ConstElementAccess_IndexFour_Throws`

---

### 5.2 Decision V1b: Non-Const Element Access Bounds Check

**Source:** `FGColumnVector3.cpp`, line 48, function `operator()(unsigned int idx)`

**Decision Expression:**
```cpp
if (idx < 1 || idx > 3)
```

**Conditions:**
- A: `idx < 1`
- B: `idx > 3`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3

**MC/DC Truth Table:**

| Test Case | idx Value | A (idx < 1) | B (idx > 3) | Decision (A \|\| B) | Purpose                   |
|-----------|-----------|-------------|-------------|----------------------|---------------------------|
| V1b-1     | 0         | T           | F           | T (throws)           | A independently forces T  |
| V1b-2     | 2         | F           | F           | F (returns ref)      | Baseline — both false     |
| V1b-3     | 4         | F           | T           | T (throws)           | B independently forces T  |

**Independence Pairs:**

| Condition | Test Pair       | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-----------------|------------------|------------------|-----------------|
| A         | V1b-1 vs V1b-2  | T -> F           | B = F (fixed)    | T -> F          |
| B         | V1b-3 vs V1b-2  | T -> F           | A = F (fixed)    | T -> F          |

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- V1b-1: `FGColumnVector3Test::NonConstElementAccess_IndexZero_Throws`
- V1b-2: `FGColumnVector3Test::NonConstElementAccess_ValidIndex_ReturnsRef`
- V1b-3: `FGColumnVector3Test::NonConstElementAccess_IndexFour_Throws`

---

### 5.3 Decision V2: Division by Zero — operator/

**Source:** `FGColumnVector3.cpp`, line 129, function `operator/(double scalar) const`

**Decision Expression:**
```cpp
if (scalar == 0.0)
```

**Conditions:**
- A: `scalar == 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2 (Decision Coverage equivalent to MC/DC for single condition)

**Truth Table:**

| Test Case | scalar Value | A (scalar == 0.0) | Decision | Outcome              |
|-----------|--------------|--------------------|----------|----------------------|
| V2-1      | 0.0          | T                  | T        | Throws domain_error  |
| V2-2      | 2.0          | F                  | F        | Returns scaled vector |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V2-1: `FGColumnVector3Test::ScalarDivision_ByZero_Throws`
- V2-2: `FGColumnVector3Test::ScalarDivision_ValidScalar_Succeeds`

---

### 5.4 Decision V3: Division by Zero — operator/=

**Source:** `FGColumnVector3.cpp`, line 145, function `operator/=(double scalar)`

**Decision Expression:**
```cpp
if (scalar == 0.0)
```

**Conditions:**
- A: `scalar == 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | scalar Value | A (scalar == 0.0) | Decision | Outcome                    |
|-----------|--------------|--------------------|----------|----------------------------|
| V3-1      | 0.0          | T                  | T        | Throws domain_error        |
| V3-2      | 4.0          | F                  | F        | Modifies vector in-place   |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V3-1: `FGColumnVector3Test::ScalarDivAssign_ByZero_Throws`
- V3-2: `FGColumnVector3Test::ScalarDivAssign_ValidScalar_Succeeds`

---

### 5.5 Decision V4: Normalize — Zero-Magnitude Guard

**Source:** `FGColumnVector3.cpp`, line 205, function `Normalize()`

**Decision Expression:**
```cpp
if (mag != 0.0)
```

**Conditions:**
- A: `mag != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Input Vector    | mag    | A (mag != 0.0) | Decision | Outcome                  |
|-----------|-----------------|--------|-----------------|----------|--------------------------|
| V4-1      | (0.0, 0.0, 0.0) | 0.0   | F               | F        | Vector unchanged (zero)  |
| V4-2      | (3.0, 4.0, 0.0) | 5.0   | T               | T        | Vector normalized to unit |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V4-1: `FGColumnVector3Test::Normalize_ZeroVector_Unchanged`
- V4-2: `FGColumnVector3Test::Normalize_NonZeroVector_UnitLength`

---

### 5.6 Decision V5a: Initializer List Size Check — Element 1

**Source:** `FGColumnVector3.cpp`, line 66, function `operator=(std::initializer_list<double> lv)`

**Decision Expression:**
```cpp
if (lv.size() >= 1)
```

**Conditions:**
- A: `lv.size() >= 1`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | lv             | lv.size() | A (size >= 1) | Decision | Outcome               |
|-----------|----------------|-----------|---------------|----------|------------------------|
| V5a-1     | `{}`           | 0         | F             | F        | data[1] unchanged      |
| V5a-2     | `{1.0}`        | 1         | T             | T        | data[1] = 1.0          |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V5a-1: `FGColumnVector3Test::InitializerList_Empty_NoChange`
- V5a-2: `FGColumnVector3Test::InitializerList_OneElement_SetsFirst`

---

### 5.7 Decision V5b: Initializer List Size Check — Element 2

**Source:** `FGColumnVector3.cpp`, line 67, function `operator=(std::initializer_list<double> lv)`

**Decision Expression:**
```cpp
if (lv.size() >= 2)
```

**Conditions:**
- A: `lv.size() >= 2`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | lv             | lv.size() | A (size >= 2) | Decision | Outcome               |
|-----------|----------------|-----------|---------------|----------|------------------------|
| V5b-1     | `{1.0}`        | 1         | F             | F        | data[2] unchanged      |
| V5b-2     | `{1.0, 2.0}`   | 2         | T             | T        | data[2] = 2.0          |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V5b-1: `FGColumnVector3Test::InitializerList_OneElement_SecondUnchanged`
- V5b-2: `FGColumnVector3Test::InitializerList_TwoElements_SetsBoth`

---

### 5.8 Decision V5c: Initializer List Size Check — Element 3

**Source:** `FGColumnVector3.cpp`, line 68, function `operator=(std::initializer_list<double> lv)`

**Decision Expression:**
```cpp
if (lv.size() >= 3)
```

**Conditions:**
- A: `lv.size() >= 3`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | lv                  | lv.size() | A (size >= 3) | Decision | Outcome               |
|-----------|---------------------|-----------|---------------|----------|------------------------|
| V5c-1     | `{1.0, 2.0}`       | 2         | F             | F        | data[3] unchanged      |
| V5c-2     | `{1.0, 2.0, 3.0}`  | 3         | T             | T        | data[3] = 3.0          |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- V5c-1: `FGColumnVector3Test::InitializerList_TwoElements_ThirdUnchanged`
- V5c-2: `FGColumnVector3Test::InitializerList_ThreeElements_SetsAll`

---

### 5.9 Decision V6: Vector Equality — Compound AND

**Source:** `FGColumnVector3.cpp`, lines 76-78, function `operator==(const FGColumnVector3& b) const`

**Decision Expression:**
```cpp
return data[1] == b.data[1]
    && data[2] == b.data[2]
    && data[3] == b.data[3];
```

**Conditions:**
- A: `data[1] == b.data[1]`
- B: `data[2] == b.data[2]`
- C: `data[3] == b.data[3]`

**Decision:** `A && B && C`

**Number of conditions:** 3
**Minimum MC/DC test cases:** 4 (N + 1 = 3 + 1)

**MC/DC Truth Table:**

| Test Case | this         | b            | A    | B    | C    | Decision | Purpose                    |
|-----------|--------------|--------------|------|------|------|----------|----------------------------|
| V6-1      | (1, 2, 3)    | (1, 2, 3)    | T    | T    | T    | T (true) | Baseline — all match       |
| V6-2      | (9, 2, 3)    | (1, 2, 3)    | F    | T    | T    | F (false)| A independently forces F   |
| V6-3      | (1, 9, 3)    | (1, 2, 3)    | T    | F    | T    | F (false)| B independently forces F   |
| V6-4      | (1, 2, 9)    | (1, 2, 3)    | T    | T    | F    | F (false)| C independently forces F   |

**Independence Pairs:**

| Condition | Test Pair     | Condition Toggle | Other Conditions      | Decision Toggle |
|-----------|---------------|------------------|-----------------------|-----------------|
| A         | V6-1 vs V6-2  | T -> F           | B = T, C = T (fixed)  | T -> F          |
| B         | V6-1 vs V6-3  | T -> F           | A = T, C = T (fixed)  | T -> F          |
| C         | V6-1 vs V6-4  | T -> F           | A = T, B = T (fixed)  | T -> F          |

**Verdict:** All three conditions independently demonstrated. MC/DC satisfied with 4 test cases.

**Note on short-circuit evaluation:** C++ guarantees left-to-right short-circuit evaluation for `&&`. When A is FALSE (V6-2), conditions B and C are not evaluated at runtime. However, for MC/DC analysis purposes, the logical values of B and C are still defined by the test inputs. The independence pair (V6-1 vs V6-2) is valid because A toggles, B and C have the same logical truth values, and the decision changes. Short-circuit evaluation does not invalidate unique-cause MC/DC analysis.

**Test Case Mapping:**
- V6-1: `FGColumnVector3Test::Equality_IdenticalVectors_True`
- V6-2: `FGColumnVector3Test::Equality_DifferAtX_False`
- V6-3: `FGColumnVector3Test::Equality_DifferAtY_False`
- V6-4: `FGColumnVector3Test::Equality_DifferAtZ_False`

---

## 6. Detailed Analysis — FGMatrix33

### 6.1 Decision M1a: Const Element Access Bounds Check

**Source:** `FGMatrix33.cpp`, line 40, function `operator()(unsigned int row, unsigned int col) const`

**Decision Expression:**
```cpp
if (row < 1 || row > 3 || col < 1 || col > 3)
```

**Conditions:**
- A: `row < 1`
- B: `row > 3`
- C: `col < 1`
- D: `col > 3`

**Decision:** `A || B || C || D`

**Number of conditions:** 4
**Minimum MC/DC test cases:** 5 (N + 1 = 4 + 1)

**MC/DC Truth Table:**

| Test Case | row | col | A (row<1) | B (row>3) | C (col<1) | D (col>3) | Decision | Purpose                    |
|-----------|-----|-----|-----------|-----------|-----------|-----------|----------|----------------------------|
| M1a-1     | 2   | 2   | F         | F         | F         | F         | F        | Baseline — all false       |
| M1a-2     | 0   | 2   | T         | F         | F         | F         | T        | A independently forces T   |
| M1a-3     | 4   | 2   | F         | T         | F         | F         | T        | B independently forces T   |
| M1a-4     | 2   | 0   | F         | F         | T         | F         | T        | C independently forces T   |
| M1a-5     | 2   | 4   | F         | F         | F         | T         | T        | D independently forces T   |

**Independence Pairs:**

| Condition | Test Pair       | Condition Toggle | Other Conditions           | Decision Toggle |
|-----------|-----------------|------------------|----------------------------|-----------------|
| A         | M1a-2 vs M1a-1  | T -> F           | B=F, C=F, D=F (fixed)     | T -> F          |
| B         | M1a-3 vs M1a-1  | T -> F           | A=F, C=F, D=F (fixed)     | T -> F          |
| C         | M1a-4 vs M1a-1  | T -> F           | A=F, B=F, D=F (fixed)     | T -> F          |
| D         | M1a-5 vs M1a-1  | T -> F           | A=F, B=F, C=F (fixed)     | T -> F          |

**Note on independence pair consistency:** For conditions A and B, the row value changes while col is held at 2 (ensuring C=F, D=F). For conditions C and D, the col value changes while row is held at 2 (ensuring A=F, B=F). All other conditions remain at FALSE in every pair, satisfying unique-cause requirements.

**Verdict:** All four conditions independently demonstrated. MC/DC satisfied with 5 test cases.

**Test Case Mapping:**
- M1a-1: `FGMatrix33Test::ConstElementAccess_ValidIndex_ReturnsElement`
- M1a-2: `FGMatrix33Test::ConstElementAccess_RowZero_Throws`
- M1a-3: `FGMatrix33Test::ConstElementAccess_RowFour_Throws`
- M1a-4: `FGMatrix33Test::ConstElementAccess_ColZero_Throws`
- M1a-5: `FGMatrix33Test::ConstElementAccess_ColFour_Throws`

---

### 6.2 Decision M1b: Non-Const Element Access Bounds Check

**Source:** `FGMatrix33.cpp`, line 47, function `operator()(unsigned int row, unsigned int col)`

**Decision Expression:**
```cpp
if (row < 1 || row > 3 || col < 1 || col > 3)
```

**Conditions:**
- A: `row < 1`
- B: `row > 3`
- C: `col < 1`
- D: `col > 3`

**Decision:** `A || B || C || D`

**Number of conditions:** 4
**Minimum MC/DC test cases:** 5

**MC/DC Truth Table:**

| Test Case | row | col | A (row<1) | B (row>3) | C (col<1) | D (col>3) | Decision | Purpose                    |
|-----------|-----|-----|-----------|-----------|-----------|-----------|----------|----------------------------|
| M1b-1     | 2   | 2   | F         | F         | F         | F         | F        | Baseline — all false       |
| M1b-2     | 0   | 2   | T         | F         | F         | F         | T        | A independently forces T   |
| M1b-3     | 4   | 2   | F         | T         | F         | F         | T        | B independently forces T   |
| M1b-4     | 2   | 0   | F         | F         | T         | F         | T        | C independently forces T   |
| M1b-5     | 2   | 4   | F         | F         | F         | T         | T        | D independently forces T   |

**Independence Pairs:**

| Condition | Test Pair       | Condition Toggle | Other Conditions           | Decision Toggle |
|-----------|-----------------|------------------|----------------------------|-----------------|
| A         | M1b-2 vs M1b-1  | T -> F           | B=F, C=F, D=F (fixed)     | T -> F          |
| B         | M1b-3 vs M1b-1  | T -> F           | A=F, C=F, D=F (fixed)     | T -> F          |
| C         | M1b-4 vs M1b-1  | T -> F           | A=F, B=F, D=F (fixed)     | T -> F          |
| D         | M1b-5 vs M1b-1  | T -> F           | A=F, B=F, C=F (fixed)     | T -> F          |

**Verdict:** All four conditions independently demonstrated. MC/DC satisfied with 5 test cases.

**Test Case Mapping:**
- M1b-1: `FGMatrix33Test::NonConstElementAccess_ValidIndex_ReturnsRef`
- M1b-2: `FGMatrix33Test::NonConstElementAccess_RowZero_Throws`
- M1b-3: `FGMatrix33Test::NonConstElementAccess_RowFour_Throws`
- M1b-4: `FGMatrix33Test::NonConstElementAccess_ColZero_Throws`
- M1b-5: `FGMatrix33Test::NonConstElementAccess_ColFour_Throws`

---

### 6.3 Decision M2: Inverse — Singularity Check

**Source:** `FGMatrix33.cpp`, line 83, function `Inverse() const`

**Decision Expression:**
```cpp
if (det == 0.0)
```

**Conditions:**
- A: `det == 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Matrix                | det   | A (det == 0.0) | Decision | Outcome                   |
|-----------|-----------------------|-------|-----------------|----------|---------------------------|
| M2-1      | Singular (all zeros)  | 0.0   | T               | T        | Throws domain_error       |
| M2-2      | Identity              | 1.0   | F               | F        | Returns inverse matrix    |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M2-1: `FGMatrix33Test::Inverse_SingularMatrix_Throws`
- M2-2: `FGMatrix33Test::Inverse_IdentityMatrix_ReturnsIdentity`

---

### 6.4 Decision M3: Division by Zero — operator/

**Source:** `FGMatrix33.cpp`, line 188, function `operator/(double scalar) const`

**Decision Expression:**
```cpp
if (scalar == 0.0)
```

**Conditions:**
- A: `scalar == 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | scalar Value | A (scalar == 0.0) | Decision | Outcome                  |
|-----------|--------------|--------------------|----------|--------------------------|
| M3-1      | 0.0          | T                  | T        | Throws domain_error      |
| M3-2      | 2.0          | F                  | F        | Returns scaled matrix    |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M3-1: `FGMatrix33Test::ScalarDivision_ByZero_Throws`
- M3-2: `FGMatrix33Test::ScalarDivision_ValidScalar_Succeeds`

---

### 6.5 Decision M4: Division by Zero — operator/=

**Source:** `FGMatrix33.cpp`, line 204, function `operator/=(double scalar)`

**Decision Expression:**
```cpp
if (scalar == 0.0)
```

**Conditions:**
- A: `scalar == 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | scalar Value | A (scalar == 0.0) | Decision | Outcome                  |
|-----------|--------------|--------------------|----------|--------------------------|
| M4-1      | 0.0          | T                  | T        | Throws domain_error      |
| M4-2      | 4.0          | F                  | F        | Modifies matrix in-place |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M4-1: `FGMatrix33Test::ScalarDivAssign_ByZero_Throws`
- M4-2: `FGMatrix33Test::ScalarDivAssign_ValidScalar_Succeeds`

---

### 6.6 Decision M5: Invertible — Determinant Check

**Source:** `FGMatrix33.cpp`, line 63, function `Invertible() const`

**Decision Expression:**
```cpp
return Determinant() != 0.0;
```

**Conditions:**
- A: `Determinant() != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Matrix                | det   | A (det != 0.0) | Decision | Return Value |
|-----------|-----------------------|-------|-----------------|----------|--------------|
| M5-1      | Singular (all zeros)  | 0.0   | F               | F        | false        |
| M5-2      | Identity              | 1.0   | T               | T        | true         |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M5-1: `FGMatrix33Test::Invertible_SingularMatrix_ReturnsFalse`
- M5-2: `FGMatrix33Test::Invertible_IdentityMatrix_ReturnsTrue`

---

### 6.7 Decision M6: GetEuler — sinTheta Clamp High

**Source:** `FGMatrix33.cpp`, line 127, function `GetEuler() const`

**Decision Expression:**
```cpp
if (sinTheta >= 1.0)
```

Where `sinTheta = -data[idx(1,3)]`.

**Conditions:**
- A: `sinTheta >= 1.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | data(1,3) | sinTheta | A (sinTheta >= 1.0) | Decision | Outcome              |
|-----------|-----------|----------|----------------------|----------|----------------------|
| M6-1      | -1.0      | 1.0      | T                    | T        | sinTheta clamped to 1.0 |
| M6-2      | -0.5      | 0.5      | F                    | F        | No clamping, falls through |

**Note:** When `data(1,3) = -1.5`, `sinTheta = 1.5 >= 1.0` is also TRUE. The test case M6-1 uses the exact boundary value. An additional boundary test with `sinTheta = 0.9999999` (FALSE) is recommended for boundary value analysis but is not strictly required for MC/DC.

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M6-1: `FGMatrix33Test::GetEuler_SinThetaAtPositiveOne_ClampHigh`
- M6-2: `FGMatrix33Test::GetEuler_NormalAngle_NoClamp`

---

### 6.8 Decision M7: GetEuler — sinTheta Clamp Low

**Source:** `FGMatrix33.cpp`, line 129, function `GetEuler() const`

**Decision Expression:**
```cpp
else if (sinTheta <= -1.0)
```

This decision is only evaluated when Decision M6 is FALSE (i.e., `sinTheta < 1.0`).

**Conditions:**
- A: `sinTheta <= -1.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | data(1,3) | sinTheta | M6 Result | A (sinTheta <= -1.0) | Decision | Outcome              |
|-----------|-----------|----------|-----------|----------------------|----------|----------------------|
| M7-1      | 1.0       | -1.0     | F         | T                    | T        | sinTheta clamped to -1.0 |
| M7-2      | 0.5       | -0.5     | F         | F                    | F        | No clamping          |

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M7-1: `FGMatrix33Test::GetEuler_SinThetaAtNegativeOne_ClampLow`
- M7-2: `FGMatrix33Test::GetEuler_NegativeAngle_NoClamp`

---

### 6.9 Decision M8: GetEuler — Gimbal Lock Detection

**Source:** `FGMatrix33.cpp`, line 134, function `GetEuler() const`

**Decision Expression:**
```cpp
if (std::abs(sinTheta) < 0.9999999)
```

**Conditions:**
- A: `std::abs(sinTheta) < 0.9999999`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | sinTheta      | abs(sinTheta) | A (abs < 0.9999999) | Decision | Outcome            |
|-----------|---------------|---------------|----------------------|----------|--------------------|
| M8-1      | 0.5           | 0.5           | T                    | T        | Normal Euler path  |
| M8-2      | 0.99999999    | 0.99999999    | F                    | F        | Gimbal lock path   |

**Note on threshold:** The threshold `0.9999999` (seven 9s) is a design choice to detect near-gimbal-lock conditions. Test case M8-2 uses `0.99999999` (eight 9s), which exceeds the threshold and triggers the gimbal lock branch. The boundary behavior should also be verified with values at and near the exact threshold; see Section 9 (Findings).

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M8-1: `FGMatrix33Test::GetEuler_NormalAngle_StandardPath`
- M8-2: `FGMatrix33Test::GetEuler_NearGimbalLock_GimbalPath`

---

### 6.10 Decision M9: Dump — Delimiter Condition

**Source:** `FGMatrix33.cpp`, line 273, function `Dump(const std::string& delimiter) const`

**Decision Expression:**
```cpp
if (j > 1)
```

This decision is evaluated in the inner loop for each column index j (1, 2, 3).

**Conditions:**
- A: `j > 1`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | j Value | A (j > 1) | Decision | Outcome              |
|-----------|---------|-----------|----------|----------------------|
| M9-1      | 1       | F         | F        | No delimiter printed |
| M9-2      | 2       | T         | T        | Delimiter printed    |

**Note:** This decision is intrinsically exercised by any call to `Dump()` on a valid matrix, since the loop iterates j from 1 to 3. Both branches (j=1 and j=2,3) are exercised in a single function call. Nevertheless, the test must verify the output format to confirm both branches produce correct output.

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M9-1/M9-2: `FGMatrix33Test::Dump_ValidMatrix_CorrectFormat` (single test exercises both branches via loop iteration; output string verified for correct delimiter placement)

---

### 6.11 Decision M10: Dump — Newline Condition

**Source:** `FGMatrix33.cpp`, line 276, function `Dump(const std::string& delimiter) const`

**Decision Expression:**
```cpp
if (i < 3)
```

This decision is evaluated in the outer loop for each row index i (1, 2, 3).

**Conditions:**
- A: `i < 3`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | i Value | A (i < 3) | Decision | Outcome                   |
|-----------|---------|-----------|----------|---------------------------|
| M10-1     | 1       | T         | T        | Newline appended after row |
| M10-2     | 3       | F         | F        | No newline after last row  |

**Note:** As with M9, both branches are intrinsically exercised by any call to `Dump()`. The test verifies the output contains exactly 2 newline characters (after rows 1 and 2, but not after row 3).

**Verdict:** Single condition — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M10-1/M10-2: `FGMatrix33Test::Dump_ValidMatrix_CorrectNewlines` (single test exercises both branches; newline count verified)

---

### 6.12 Decision M11: Equality — Element Inequality in Loop

**Source:** `FGMatrix33.cpp`, line 256, function `operator==(const FGMatrix33& B) const`

**Decision Expression:**
```cpp
if (data[i] != B.data[i])
```

This decision is evaluated inside a loop iterating `i` from 0 to 8. Each iteration constitutes a separate decision instance (same expression, same conditions, but different data values). For MC/DC, we must demonstrate both TRUE and FALSE outcomes for this decision.

**Conditions:**
- A: `data[i] != B.data[i]`

**Decision:** `A`

**Number of conditions per iteration:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Matrices          | A (data[i] != B.data[i]) | Decision | Outcome               |
|-----------|-------------------|--------------------------|----------|------------------------|
| M11-1     | Identical         | F (all iterations)       | F        | Returns true           |
| M11-2     | Differ at (1,1)   | T (iteration i=0)        | T        | Returns false (early)  |

**Additional coverage test cases:**

| Test Case | Matrices            | Notes                                                |
|-----------|---------------------|------------------------------------------------------|
| M11-3     | Differ at (3,3) only | Exercises all iterations i=0..7 as FALSE, i=8 as TRUE |

Test case M11-3 ensures that the loop body is exercised for all 9 element positions and that a difference at the last element is detected.

**Verdict:** Single condition per iteration — Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- M11-1: `FGMatrix33Test::Equality_IdenticalMatrices_True`
- M11-2: `FGMatrix33Test::Equality_DifferAtFirstElement_False`
- M11-3: `FGMatrix33Test::Equality_DifferAtLastElement_False`

---

## 7. Coverage Summary

### 7.1 Per-Module Summary

#### FGColumnVector3

| Decision ID | Expression                          | Conditions | MC/DC Test Cases | Independence Pairs Verified |
|-------------|-------------------------------------|------------|------------------|-----------------------------|
| V1a         | `idx < 1 \|\| idx > 3` (const)     | 2          | 3                | 2 of 2                      |
| V1b         | `idx < 1 \|\| idx > 3` (non-const) | 2          | 3                | 2 of 2                      |
| V2          | `scalar == 0.0` (operator/)        | 1          | 2                | 1 of 1                      |
| V3          | `scalar == 0.0` (operator/=)       | 1          | 2                | 1 of 1                      |
| V4          | `mag != 0.0`                        | 1          | 2                | 1 of 1                      |
| V5a         | `lv.size() >= 1`                    | 1          | 2                | 1 of 1                      |
| V5b         | `lv.size() >= 2`                    | 1          | 2                | 1 of 1                      |
| V5c         | `lv.size() >= 3`                    | 1          | 2                | 1 of 1                      |
| V6          | `data[1]==b[1] && data[2]==b[2] && data[3]==b[3]` | 3 | 4       | 3 of 3                      |
| **Totals**  |                                     | **13**     | **23**           | **13 of 13 (100%)**         |

#### FGMatrix33

| Decision ID | Expression                                        | Conditions | MC/DC Test Cases | Independence Pairs Verified |
|-------------|---------------------------------------------------|------------|------------------|-----------------------------|
| M1a         | `row<1 \|\| row>3 \|\| col<1 \|\| col>3` (const) | 4          | 5                | 4 of 4                      |
| M1b         | `row<1 \|\| row>3 \|\| col<1 \|\| col>3` (non-const) | 4      | 5                | 4 of 4                      |
| M2          | `det == 0.0`                                       | 1          | 2                | 1 of 1                      |
| M3          | `scalar == 0.0` (operator/)                        | 1          | 2                | 1 of 1                      |
| M4          | `scalar == 0.0` (operator/=)                       | 1          | 2                | 1 of 1                      |
| M5          | `Determinant() != 0.0`                             | 1          | 2                | 1 of 1                      |
| M6          | `sinTheta >= 1.0`                                  | 1          | 2                | 1 of 1                      |
| M7          | `sinTheta <= -1.0`                                 | 1          | 2                | 1 of 1                      |
| M8          | `std::abs(sinTheta) < 0.9999999`                   | 1          | 2                | 1 of 1                      |
| M9          | `j > 1`                                            | 1          | 2                | 1 of 1                      |
| M10         | `i < 3`                                            | 1          | 2                | 1 of 1                      |
| M11         | `data[i] != B.data[i]`                             | 1          | 2                | 1 of 1                      |
| **Totals**  |                                                    | **18**     | **30**           | **18 of 18 (100%)**         |

### 7.2 Grand Total

| Metric                                    | Value   |
|-------------------------------------------|---------|
| Total distinct decision points            | 21      |
| Total Boolean conditions                  | 31      |
| Total MC/DC test cases required           | 53      |
| Total independence pairs required         | 31      |
| Total independence pairs verified         | 31      |
| **MC/DC Coverage**                        | **100%** |

---

## 8. Decision-to-Test-Case Traceability Matrix

This section maps each MC/DC test case to its corresponding test function in the Google Test suite files.

### 8.1 FGColumnVector3 Test Cases

| MC/DC Test ID | Decision | Test Suite / Test Name                                               | Source File                        |
|---------------|----------|----------------------------------------------------------------------|------------------------------------|
| V1a-1         | V1a      | `FGColumnVector3Test::ConstElementAccess_IndexZero_Throws`           | test_vector_dala_gtest.cpp         |
| V1a-2         | V1a      | `FGColumnVector3Test::ConstElementAccess_ValidIndex_ReturnsElement`  | test_vector_dala_gtest.cpp         |
| V1a-3         | V1a      | `FGColumnVector3Test::ConstElementAccess_IndexFour_Throws`           | test_vector_dala_gtest.cpp         |
| V1b-1         | V1b      | `FGColumnVector3Test::NonConstElementAccess_IndexZero_Throws`        | test_vector_dala_gtest.cpp         |
| V1b-2         | V1b      | `FGColumnVector3Test::NonConstElementAccess_ValidIndex_ReturnsRef`   | test_vector_dala_gtest.cpp         |
| V1b-3         | V1b      | `FGColumnVector3Test::NonConstElementAccess_IndexFour_Throws`        | test_vector_dala_gtest.cpp         |
| V2-1          | V2       | `FGColumnVector3Test::ScalarDivision_ByZero_Throws`                  | test_vector_dala_gtest.cpp         |
| V2-2          | V2       | `FGColumnVector3Test::ScalarDivision_ValidScalar_Succeeds`           | test_vector_dala_gtest.cpp         |
| V3-1          | V3       | `FGColumnVector3Test::ScalarDivAssign_ByZero_Throws`                 | test_vector_dala_gtest.cpp         |
| V3-2          | V3       | `FGColumnVector3Test::ScalarDivAssign_ValidScalar_Succeeds`          | test_vector_dala_gtest.cpp         |
| V4-1          | V4       | `FGColumnVector3Test::Normalize_ZeroVector_Unchanged`                | test_vector_dala_gtest.cpp         |
| V4-2          | V4       | `FGColumnVector3Test::Normalize_NonZeroVector_UnitLength`            | test_vector_dala_gtest.cpp         |
| V5a-1         | V5a      | `FGColumnVector3Test::InitializerList_Empty_NoChange`                | test_vector_dala_gtest.cpp         |
| V5a-2         | V5a      | `FGColumnVector3Test::InitializerList_OneElement_SetsFirst`          | test_vector_dala_gtest.cpp         |
| V5b-1         | V5b      | `FGColumnVector3Test::InitializerList_OneElement_SecondUnchanged`    | test_vector_dala_gtest.cpp         |
| V5b-2         | V5b      | `FGColumnVector3Test::InitializerList_TwoElements_SetsBoth`          | test_vector_dala_gtest.cpp         |
| V5c-1         | V5c      | `FGColumnVector3Test::InitializerList_TwoElements_ThirdUnchanged`    | test_vector_dala_gtest.cpp         |
| V5c-2         | V5c      | `FGColumnVector3Test::InitializerList_ThreeElements_SetsAll`         | test_vector_dala_gtest.cpp         |
| V6-1          | V6       | `FGColumnVector3Test::Equality_IdenticalVectors_True`                | test_vector_dala_gtest.cpp         |
| V6-2          | V6       | `FGColumnVector3Test::Equality_DifferAtX_False`                      | test_vector_dala_gtest.cpp         |
| V6-3          | V6       | `FGColumnVector3Test::Equality_DifferAtY_False`                      | test_vector_dala_gtest.cpp         |
| V6-4          | V6       | `FGColumnVector3Test::Equality_DifferAtZ_False`                      | test_vector_dala_gtest.cpp         |

### 8.2 FGMatrix33 Test Cases

| MC/DC Test ID | Decision | Test Suite / Test Name                                               | Source File                        |
|---------------|----------|----------------------------------------------------------------------|------------------------------------|
| M1a-1         | M1a      | `FGMatrix33Test::ConstElementAccess_ValidIndex_ReturnsElement`        | test_matrix_dala_gtest.cpp         |
| M1a-2         | M1a      | `FGMatrix33Test::ConstElementAccess_RowZero_Throws`                  | test_matrix_dala_gtest.cpp         |
| M1a-3         | M1a      | `FGMatrix33Test::ConstElementAccess_RowFour_Throws`                  | test_matrix_dala_gtest.cpp         |
| M1a-4         | M1a      | `FGMatrix33Test::ConstElementAccess_ColZero_Throws`                  | test_matrix_dala_gtest.cpp         |
| M1a-5         | M1a      | `FGMatrix33Test::ConstElementAccess_ColFour_Throws`                  | test_matrix_dala_gtest.cpp         |
| M1b-1         | M1b      | `FGMatrix33Test::NonConstElementAccess_ValidIndex_ReturnsRef`        | test_matrix_dala_gtest.cpp         |
| M1b-2         | M1b      | `FGMatrix33Test::NonConstElementAccess_RowZero_Throws`               | test_matrix_dala_gtest.cpp         |
| M1b-3         | M1b      | `FGMatrix33Test::NonConstElementAccess_RowFour_Throws`               | test_matrix_dala_gtest.cpp         |
| M1b-4         | M1b      | `FGMatrix33Test::NonConstElementAccess_ColZero_Throws`               | test_matrix_dala_gtest.cpp         |
| M1b-5         | M1b      | `FGMatrix33Test::NonConstElementAccess_ColFour_Throws`               | test_matrix_dala_gtest.cpp         |
| M2-1          | M2       | `FGMatrix33Test::Inverse_SingularMatrix_Throws`                      | test_matrix_dala_gtest.cpp         |
| M2-2          | M2       | `FGMatrix33Test::Inverse_IdentityMatrix_ReturnsIdentity`             | test_matrix_dala_gtest.cpp         |
| M3-1          | M3       | `FGMatrix33Test::ScalarDivision_ByZero_Throws`                       | test_matrix_dala_gtest.cpp         |
| M3-2          | M3       | `FGMatrix33Test::ScalarDivision_ValidScalar_Succeeds`                | test_matrix_dala_gtest.cpp         |
| M4-1          | M4       | `FGMatrix33Test::ScalarDivAssign_ByZero_Throws`                      | test_matrix_dala_gtest.cpp         |
| M4-2          | M4       | `FGMatrix33Test::ScalarDivAssign_ValidScalar_Succeeds`               | test_matrix_dala_gtest.cpp         |
| M5-1          | M5       | `FGMatrix33Test::Invertible_SingularMatrix_ReturnsFalse`             | test_matrix_dala_gtest.cpp         |
| M5-2          | M5       | `FGMatrix33Test::Invertible_IdentityMatrix_ReturnsTrue`              | test_matrix_dala_gtest.cpp         |
| M6-1          | M6       | `FGMatrix33Test::GetEuler_SinThetaAtPositiveOne_ClampHigh`           | test_matrix_dala_gtest.cpp         |
| M6-2          | M6       | `FGMatrix33Test::GetEuler_NormalAngle_NoClamp`                       | test_matrix_dala_gtest.cpp         |
| M7-1          | M7       | `FGMatrix33Test::GetEuler_SinThetaAtNegativeOne_ClampLow`            | test_matrix_dala_gtest.cpp         |
| M7-2          | M7       | `FGMatrix33Test::GetEuler_NegativeAngle_NoClamp`                     | test_matrix_dala_gtest.cpp         |
| M8-1          | M8       | `FGMatrix33Test::GetEuler_NormalAngle_StandardPath`                  | test_matrix_dala_gtest.cpp         |
| M8-2          | M8       | `FGMatrix33Test::GetEuler_NearGimbalLock_GimbalPath`                 | test_matrix_dala_gtest.cpp         |
| M9-1/M9-2    | M9       | `FGMatrix33Test::Dump_ValidMatrix_CorrectFormat`                     | test_matrix_dala_gtest.cpp         |
| M10-1/M10-2  | M10      | `FGMatrix33Test::Dump_ValidMatrix_CorrectNewlines`                   | test_matrix_dala_gtest.cpp         |
| M11-1         | M11      | `FGMatrix33Test::Equality_IdenticalMatrices_True`                    | test_matrix_dala_gtest.cpp         |
| M11-2         | M11      | `FGMatrix33Test::Equality_DifferAtFirstElement_False`                | test_matrix_dala_gtest.cpp         |
| M11-3         | M11      | `FGMatrix33Test::Equality_DifferAtLastElement_False`                 | test_matrix_dala_gtest.cpp         |

---

## 9. Findings and Observations

### 9.1 Finding F-001: Unsigned Integer Wrap-Around for idx = 0

**Severity:** Observation (no code change required)

**Description:** The element access operators in both FGColumnVector3 and FGMatrix33 accept `unsigned int` parameters. The condition `idx < 1` (or `row < 1`, `col < 1`) evaluates to TRUE when `idx = 0`, which is the only unsigned integer value that satisfies this condition without wrap-around. If a caller passes a negative `int` (e.g., `-1`), implicit conversion to `unsigned int` produces a large value (e.g., `4294967295`), which would trigger the `idx > 3` condition rather than `idx < 1`.

**Impact on MC/DC:** The MC/DC test cases use `idx = 0` for the `< 1` condition, which is the only natural unsigned value that makes this condition TRUE. This is correct and complete. The wrap-around behavior is an interface design consideration, not an MC/DC gap.

**Recommendation:** Document in the SRS that negative index values are not a valid input for unsigned int parameters. Consider adding a static assertion or API note to prevent misuse.

### 9.2 Finding F-002: Exact Floating-Point Equality in Invertible()

**Severity:** Observation (design review recommended)

**Description:** The `Invertible()` function (line 63) uses exact floating-point comparison `Determinant() != 0.0`. Due to floating-point rounding, a matrix that is mathematically singular may have a computed determinant very close to (but not exactly) zero, causing `Invertible()` to return `true` when the matrix is effectively singular.

**Impact on MC/DC:** MC/DC is fully satisfied for this decision. The finding relates to the functional correctness of the threshold choice, not to structural coverage.

**Recommendation:** Consider whether a tolerance-based check (e.g., `std::abs(Determinant()) > epsilon`) is more appropriate for production use. Document the design rationale for exact comparison.

### 9.3 Finding F-003: Exact Floating-Point Equality in Division-by-Zero Checks

**Severity:** Observation

**Description:** The division operators in both modules guard against division by zero using `scalar == 0.0`. This exact comparison will not catch near-zero denominators that could produce overflow (e.g., `scalar = 1e-308`).

**Impact on MC/DC:** No impact. The decision has a single condition and MC/DC is satisfied.

**Recommendation:** Consider whether near-zero denominators should be treated as error conditions. If so, a tolerance-based guard would be needed and the decision expression would remain a single condition.

### 9.4 Finding F-004: GetEuler Gimbal Lock Threshold Boundary

**Severity:** Observation (boundary analysis recommended)

**Description:** The gimbal lock detection threshold `0.9999999` (seven 9s after decimal) is a magic number. The MC/DC analysis verifies that both branches (normal path and gimbal lock path) are exercised, but the exact boundary value should be tested:
- `sinTheta = 0.9999998` (just below threshold) -- should take normal path
- `sinTheta = 0.9999999` (at threshold) -- should take normal path (strict `<`)
- `sinTheta = 0.99999991` (just above threshold) -- should take gimbal lock path

**Impact on MC/DC:** No impact. The decision has a single condition and MC/DC is satisfied regardless of threshold value. However, boundary value analysis (a separate DO-178C requirement) should verify behavior at the threshold.

**Recommendation:** Add boundary value test cases at and near the threshold. Document the rationale for the `0.9999999` value.

### 9.5 Finding F-005: Short-Circuit Evaluation in operator== (FGColumnVector3)

**Severity:** Observation (no action required)

**Description:** The equality operator in FGColumnVector3 uses a compound `&&` expression (line 76-78). C++ guarantees left-to-right short-circuit evaluation: if `data[1] != b.data[1]`, conditions B and C are never evaluated at runtime. This does not affect MC/DC validity because:
1. The test inputs define the logical values of all conditions regardless of evaluation order.
2. The independence pairs (V6-1 vs V6-2, V6-1 vs V6-3, V6-1 vs V6-4) are all valid under unique-cause MC/DC.

For condition B (V6-1 vs V6-3) and condition C (V6-1 vs V6-4), the baseline test case V6-1 evaluates all three conditions, so the short-circuit path does not prevent the independence demonstration.

**Impact on MC/DC:** None. Analysis is valid.

### 9.6 Finding F-006: Loop-Based Equality in operator== (FGMatrix33)

**Severity:** Observation

**Description:** The equality operator in FGMatrix33 uses a loop with an early-return pattern (lines 255-257). Each loop iteration contains the same single-condition decision `data[i] != B.data[i]`. Under MC/DC, this is treated as a single decision with both TRUE and FALSE outcomes required. Test case M11-3 (differ at last element only) provides additional assurance that the loop executes all iterations before finding a mismatch.

**Impact on MC/DC:** The loop structure means the decision `data[i] != B.data[i]` is executed up to 9 times per function call. MC/DC requires demonstrating TRUE and FALSE outcomes for this decision, which test cases M11-1 (all FALSE) and M11-2 (TRUE at first iteration) achieve. M11-3 provides defense-in-depth by exercising TRUE at the last iteration.

---

## 10. Conclusion

This analysis demonstrates that **100% unique-cause MC/DC coverage** is achievable for all 21 decision points (containing 31 total Boolean conditions) across the FGColumnVector3 and FGMatrix33 modules. A minimum of **53 MC/DC-specific test cases** are required, with all 31 independence pairs formally identified and documented.

All multi-condition decisions (V1a, V1b, V6, M1a, M1b) have complete truth tables with verified independence pairs. All single-condition decisions are covered by Decision Coverage, which is equivalent to MC/DC for single-condition decisions.

No structural coverage gaps were identified. Six observations (F-001 through F-006) are documented for design review consideration but do not represent MC/DC deficiencies.

This analysis satisfies the requirements of:
- DO-178C Section 6.4.4.2 (Structural Coverage Analysis — MC/DC)
- SVP-SF-2026-001 Section 8 (Verification Approach — Structural Coverage)
- TAR-SF-001 Acceptance Criterion 4 (MC/DC documented for every multi-condition decision)

---

## 11. Revision History

| Rev | Date       | Author                          | Description                                      |
|-----|------------|---------------------------------|--------------------------------------------------|
| 1.0 | 2026-03-14 | Development Team (LLM-Assisted) | Initial MC/DC analysis — all 21 decisions in FGColumnVector3 and FGMatrix33 |
