# MC/DC Structural Coverage Analysis Report

| Field              | Value                                                        |
|--------------------|--------------------------------------------------------------|
| Document ID        | MCDC-SF-TAR003-2026-001                                      |
| TAR Reference      | TAR-SF-003 — Terrain Engine Core Verification & DAL A Coverage Enhancement |
| DAL                | A (Catastrophic) — 100% MC/DC Required                       |
| Standard           | DO-178C Section 6.4.4.2 — Structural Coverage Analysis       |
| SVP Reference      | SVP-SF-2026-001, Section 8                                   |
| Modules Under Test | TerrainGrid (src/terrain/TerrainGrid.h, TerrainGrid.cpp)     |
|                    | TerrainCollisionDetector (src/terrain/TerrainCollisionDetector.h/.cpp) |
| Date               | 2026-03-14                                                   |
| Revision           | 1.0                                                          |
| Status             | Baseline                                                     |

---

## 1. Purpose

This document provides the formal Modified Condition/Decision Coverage (MC/DC) analysis for all decision points in the TerrainGrid and TerrainCollisionDetector modules, as required by DO-178C Section 6.4.4.2 for DAL A software.

For every Boolean decision in the source code, this document:

1. Identifies the decision expression and its constituent conditions.
2. Constructs the MC/DC truth table demonstrating that each condition can independently affect the decision outcome.
3. Identifies the independence pairs that satisfy the unique-cause MC/DC criterion.
4. Specifies the minimum test cases required and maps them to test case identifiers in the test suites.
5. Documents total coverage metrics across both modules.

This analysis covers all decisions in the `.h` header files (inline implementations) and the `.cpp` implementation files. Functions with no Boolean decisions (simple getters, delegation-only functions) are excluded from MC/DC analysis but are covered by statement/decision coverage through normal test execution.

---

## 2. Applicable Documents

| Document ID          | Title                                                         |
|----------------------|---------------------------------------------------------------|
| DO-178C              | Software Considerations in Airborne Systems and Equipment Certification |
| SVP-SF-2026-001      | Software Verification Plan                                    |
| TAR-SF-003           | Terrain Engine Core Verification & DAL A Coverage Enhancement |
| SRS-SF-2026-001      | Software Requirements Specification                           |
| SCS-SF-2026-001      | Software Coding Standards                                     |
| PSAC-SF-2026-001     | Plan for Software Aspects of Certification                    |
| TAR-SF-001           | Math Library Verification & DAL A Coverage Enhancement        |
| TAR-SF-002           | Atmosphere Model Verification & DAL A Coverage Enhancement    |

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
- A condition labeled with its variable name (e.g., A, B) represents the truth value of that sub-expression.
- Independence pairs are written as (Test X vs Test Y) where the condition under test toggles and the decision outcome changes.

### 3.4 Loop Decision Treatment

Several functions contain `for` loops with compound continuation conditions. Each loop's continuation condition is treated as a decision. For single-condition loop decisions, Decision Coverage (TRUE = continue, FALSE = exit) is sufficient for MC/DC. For multi-condition loop decisions (e.g., `step <= maxSteps && t <= lookaheadTime`), full MC/DC analysis is performed.

### 3.5 Clamping Decision Treatment

The `GetElevation()` function contains four sequential clamping decisions (`if (r0 < 0)`, `if (r0 >= maxRow)`, `if (c0 < 0)`, `if (c0 >= maxCol)`). Each is a single-condition decision and is analyzed independently. Together they ensure that interpolation coordinates are clamped to the valid grid range.

---

## 4. Decision Inventory

### 4.1 Summary Table -- TerrainGrid

| Decision ID | Source Location                     | Expression                                    | Type     | Conditions | MC/DC Cases |
|-------------|-------------------------------------|-----------------------------------------------|----------|------------|-------------|
| TG-D1       | Constructor                         | `rows < 2 \|\| cols < 2`                     | if       | **2**      | **3**       |
| TG-D2       | Constructor                         | `postingMeters <= 0.0`                        | if       | 1          | 2           |
| TG-D3       | `SetElevation()`                    | `row >= rows_ \|\| col >= cols_`              | if       | **2**      | **3**       |
| TG-D4       | `GetElevationAtCell()`              | `row >= rows_ \|\| col >= cols_`              | if       | **2**      | **3**       |
| TG-D5       | `IsInsideGrid()`                    | `gridRow >= 0.0 && gridRow <= maxRow && gridCol >= 0.0 && gridCol <= maxCol` | if | **4** | **5** |
| TG-D6a      | `GetElevation()` clamping           | `r0 < 0`                                      | if       | 1          | 2           |
| TG-D6b      | `GetElevation()` clamping           | `r0 >= maxRow`                                 | if       | 1          | 2           |
| TG-D6c      | `GetElevation()` clamping           | `c0 < 0`                                      | if       | 1          | 2           |
| TG-D6d      | `GetElevation()` clamping           | `c0 >= maxCol`                                 | if       | 1          | 2           |
| TG-D7a      | `FillSlope()` outer loop            | `row < rows_`                                  | for      | 1          | 2           |
| TG-D7b      | `FillSlope()` inner loop            | `col < cols_`                                  | for      | 1          | 2           |
| **Subtotal**|                                     |                                               |          | **15**     | **28**      |

### 4.2 Summary Table -- TerrainCollisionDetector

| Decision ID | Source Location                          | Expression                                                | Type     | Conditions | MC/DC Cases |
|-------------|------------------------------------------|-----------------------------------------------------------|----------|------------|-------------|
| CD-D8       | `SetClearanceThreshold()`                | `threshold < 25.0`                                        | if       | 1          | 2           |
| CD-D9       | `SetClearanceThreshold()`                | `threshold > 500.0`                                       | if       | 1          | 2           |
| CD-D10      | `SetLookaheadTime()`                     | `time < 60.0`                                             | if       | 1          | 2           |
| CD-D11      | `Evaluate()` loop                        | `step <= maxSteps && t <= lookaheadTime`                  | for      | **2**      | **3**       |
| CD-D12      | `Evaluate()` WARNING                     | `clearance <= 0.0`                                        | if       | 1          | 2           |
| CD-D13      | `Evaluate()` CAUTION                     | `clearance < threshold * cautionMultiplier`               | if       | 1          | 2           |
| CD-D14      | `Evaluate()` tracking minimum            | `clearance < minClearance`                                | if       | 1          | 2           |
| **Subtotal**|                                          |                                                           |          | **9**      | **16**      |

### 4.3 Grand Total

| Metric                                   | TerrainGrid | TerrainCollisionDetector | Total  |
|------------------------------------------|-------------|--------------------------|--------|
| Distinct decision points                 | 11          | 7                        | 18     |
| Boolean conditions (non-switch)          | 15          | 9                        | 24     |
| Switch paths                             | 0           | 0                        | 0      |
| Multi-condition decisions                | 3           | 1                        | 4      |
| Total MC/DC test cases required          | 28          | 16                       | 44     |

---

## 5. Detailed Analysis -- TerrainGrid

### 5.1 Decision TG-D1: Constructor -- Minimum Dimension Validation

**Source:** `TerrainGrid.cpp`, Constructor `TerrainGrid(size_t rows, size_t cols, double postingMeters)`

**Decision Expression:**
```cpp
if (rows < 2 || cols < 2)
```

This decision validates that the grid has at least 2 rows and 2 columns, which is the minimum required for bilinear interpolation (at least one complete cell).

**Conditions:**
- A: `rows < 2`
- B: `cols < 2`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**MC/DC Truth Table:**

| Test Case | rows | cols | A (rows < 2) | B (cols < 2) | Decision (A \|\| B) | Outcome                     |
|-----------|------|------|---------------|--------------|----------------------|-----------------------------|
| TG-D1-1   | 1    | 10   | T             | F            | T                    | throws std::invalid_argument|
| TG-D1-2   | 10   | 1    | F             | T            | T                    | throws std::invalid_argument|
| TG-D1-3   | 10   | 10   | F             | F            | F                    | Grid constructed normally   |

**Independence Pairs:**

| Condition | Test Pair         | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-------------------|------------------|------------------|-----------------|
| A         | TG-D1-1 vs TG-D1-3| T -> F           | B = F (fixed)    | T -> F          |
| B         | TG-D1-2 vs TG-D1-3| T -> F           | A = F (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In TG-D1-1, A=T, B=F, Decision=T. In TG-D1-3, A=F, B=F, Decision=F. Only A toggles; B remains F. Decision changes T->F. A independently affects the outcome.
- For condition B: In TG-D1-2, A=F, B=T, Decision=T. In TG-D1-3, A=F, B=F, Decision=F. Only B toggles; A remains F. Decision changes T->F. B independently affects the outcome.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- TG-D1-1: `TerrainGridConstructionTest::Constructor_RowsTooSmall_Throws`
- TG-D1-2: `TerrainGridConstructionTest::Constructor_ColsTooSmall_Throws`
- TG-D1-3: `TerrainGridConstructionTest::Constructor_ValidDimensions_Succeeds`

---

### 5.2 Decision TG-D2: Constructor -- Posting Validation

**Source:** `TerrainGrid.cpp`, Constructor `TerrainGrid(size_t rows, size_t cols, double postingMeters)`

**Decision Expression:**
```cpp
if (postingMeters <= 0.0)
```

This decision validates that the grid posting spacing is positive.

**Conditions:**
- A: `postingMeters <= 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2 (Decision Coverage equivalent to MC/DC for single condition)

**Truth Table:**

| Test Case | postingMeters | A (postingMeters <= 0.0) | Decision | Outcome                      |
|-----------|---------------|--------------------------|----------|------------------------------|
| TG-D2-1   | 0.0           | T                        | T        | throws std::invalid_argument |
| TG-D2-2   | 30.0          | F                        | F        | Grid constructed normally    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- TG-D2-1: `TerrainGridConstructionTest::Constructor_ZeroPosting_Throws`
- TG-D2-2: `TerrainGridConstructionTest::Constructor_ValidPosting_Succeeds`

---

### 5.3 Decision TG-D3: SetElevation -- Bounds Check

**Source:** `TerrainGrid.cpp`, function `SetElevation(size_t row, size_t col, double elevation)`

**Decision Expression:**
```cpp
if (row >= rows_ || col >= cols_)
```

This decision validates that the row and column indices are within the grid dimensions before writing an elevation value.

**Conditions:**
- A: `row >= rows_`
- B: `col >= cols_`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**MC/DC Truth Table:**

| Test Case | row    | col    | A (row >= rows_) | B (col >= cols_) | Decision (A \|\| B) | Outcome                      |
|-----------|--------|--------|------------------|------------------|----------------------|------------------------------|
| TG-D3-1   | rows_  | 0      | T                | F                | T                    | throws std::out_of_range     |
| TG-D3-2   | 0      | cols_  | F                | T                | T                    | throws std::out_of_range     |
| TG-D3-3   | 0      | 0      | F                | F                | F                    | Elevation set successfully   |

**Independence Pairs:**

| Condition | Test Pair         | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-------------------|------------------|------------------|-----------------|
| A         | TG-D3-1 vs TG-D3-3| T -> F           | B = F (fixed)    | T -> F          |
| B         | TG-D3-2 vs TG-D3-3| T -> F           | A = F (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In TG-D3-1, A=T, B=F, Decision=T. In TG-D3-3, A=F, B=F, Decision=F. Only A toggles; B remains F. Decision changes T->F. A independently affects the outcome.
- For condition B: In TG-D3-2, A=F, B=T, Decision=T. In TG-D3-3, A=F, B=F, Decision=F. Only B toggles; A remains F. Decision changes T->F. B independently affects the outcome.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- TG-D3-1: `TerrainGridDataEntryTest::SetElevation_RowOutOfBounds_Throws`
- TG-D3-2: `TerrainGridDataEntryTest::SetElevation_ColOutOfBounds_Throws`
- TG-D3-3: `TerrainGridDataEntryTest::SetElevation_ValidIndex_Succeeds`

---

### 5.4 Decision TG-D4: GetElevationAtCell -- Bounds Check

**Source:** `TerrainGrid.cpp`, function `GetElevationAtCell(size_t row, size_t col) const`

**Decision Expression:**
```cpp
if (row >= rows_ || col >= cols_)
```

Structurally identical to TG-D3 but guards the read (getter) path.

**Conditions:**
- A: `row >= rows_`
- B: `col >= cols_`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**MC/DC Truth Table:**

| Test Case | row    | col    | A (row >= rows_) | B (col >= cols_) | Decision (A \|\| B) | Outcome                      |
|-----------|--------|--------|------------------|------------------|----------------------|------------------------------|
| TG-D4-1   | rows_  | 0      | T                | F                | T                    | throws std::out_of_range     |
| TG-D4-2   | 0      | cols_  | F                | T                | T                    | throws std::out_of_range     |
| TG-D4-3   | 0      | 0      | F                | F                | F                    | Returns elevation value      |

**Independence Pairs:**

| Condition | Test Pair         | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-------------------|------------------|------------------|-----------------|
| A         | TG-D4-1 vs TG-D4-3| T -> F           | B = F (fixed)    | T -> F          |
| B         | TG-D4-2 vs TG-D4-3| T -> F           | A = F (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In TG-D4-1, A=T, B=F, Decision=T. In TG-D4-3, A=F, B=F, Decision=F. Only A toggles; B remains F. Decision changes T->F. A independently affects the outcome.
- For condition B: In TG-D4-2, A=F, B=T, Decision=T. In TG-D4-3, A=F, B=F, Decision=F. Only B toggles; A remains F. Decision changes T->F. B independently affects the outcome.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- TG-D4-1: `TerrainGridDataEntryTest::GetElevationAtCell_RowOutOfBounds_Throws`
- TG-D4-2: `TerrainGridDataEntryTest::GetElevationAtCell_ColOutOfBounds_Throws`
- TG-D4-3: `TerrainGridDataEntryTest::GetElevationAtCell_ValidIndex_ReturnsValue`

---

### 5.5 Decision TG-D5: IsInsideGrid -- 4-Condition Bounds Check (MC/DC REQUIRED)

**Source:** `TerrainGrid.cpp`, function `IsInsideGrid(double northMeters, double eastMeters) const`

**Decision Expression:**
```cpp
if (gridRow >= 0.0 && gridRow <= maxRow && gridCol >= 0.0 && gridCol <= maxCol)
```

where `maxRow = static_cast<double>(rows_ - 1)` and `maxCol = static_cast<double>(cols_ - 1)`, and `gridRow` / `gridCol` are derived from the input world coordinates via `ToGridCoordinates()`.

**Conditions:**
- A: `gridRow >= 0.0`
- B: `gridRow <= maxRow`
- C: `gridCol >= 0.0`
- D: `gridCol <= maxCol`

**Decision:** `A && B && C && D`

**Number of conditions:** 4
**Minimum MC/DC test cases:** 5 (N + 1 = 4 + 1)

**MC/DC Truth Table:**

| Test Case | gridRow | gridCol | A (>= 0) | B (<= maxRow) | C (>= 0) | D (<= maxCol) | Decision | Outcome       |
|-----------|---------|---------|-----------|----------------|-----------|----------------|----------|---------------|
| TG-D5-1   | mid     | mid     | T         | T              | T         | T              | T        | Returns true  |
| TG-D5-2   | -1.0    | mid     | F         | T              | T         | T              | F        | Returns false |
| TG-D5-3   | maxR+1  | mid     | T         | F              | T         | T              | F        | Returns false |
| TG-D5-4   | mid     | -1.0    | T         | T              | F         | T              | F        | Returns false |
| TG-D5-5   | mid     | maxC+1  | T         | T              | T         | F              | F        | Returns false |

**Concrete values (assuming 10x10 grid with 30m posting, maxRow=9, maxCol=9):**

| Test Case | northMeters | eastMeters | gridRow | gridCol | A   | B   | C   | D   | Decision |
|-----------|-------------|------------|---------|---------|-----|-----|-----|-----|----------|
| TG-D5-1   | 135.0       | 135.0      | 4.5     | 4.5     | T   | T   | T   | T   | T        |
| TG-D5-2   | -30.0       | 135.0      | -1.0    | 4.5     | F   | T   | T   | T   | F        |
| TG-D5-3   | 300.0       | 135.0      | 10.0    | 4.5     | T   | F   | T   | T   | F        |
| TG-D5-4   | 135.0       | -30.0      | 4.5     | -1.0    | T   | T   | F   | T   | F        |
| TG-D5-5   | 135.0       | 300.0      | 4.5     | 10.0    | T   | T   | T   | F   | F        |

**Independence Pairs:**

| Condition | Test Pair         | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-------------------|------------------|------------------|-----------------|
| A         | TG-D5-1 vs TG-D5-2| T -> F           | B=T, C=T, D=T   | T -> F          |
| B         | TG-D5-1 vs TG-D5-3| T -> F           | A=T, C=T, D=T   | T -> F          |
| C         | TG-D5-1 vs TG-D5-4| T -> F           | A=T, B=T, D=T   | T -> F          |
| D         | TG-D5-1 vs TG-D5-5| T -> F           | A=T, B=T, C=T   | T -> F          |

**Verification of independence:**
- For condition A: In TG-D5-1, all conditions T, Decision=T. In TG-D5-2, A=F, B/C/D remain T, Decision=F. Only A toggles. Decision changes T->F. A independently affects the outcome.
- For condition B: In TG-D5-1, all conditions T, Decision=T. In TG-D5-3, B=F, A/C/D remain T, Decision=F. Only B toggles. Decision changes T->F. B independently affects the outcome.
- For condition C: In TG-D5-1, all conditions T, Decision=T. In TG-D5-4, C=F, A/B/D remain T, Decision=F. Only C toggles. Decision changes T->F. C independently affects the outcome.
- For condition D: In TG-D5-1, all conditions T, Decision=T. In TG-D5-5, D=F, A/B/C remain T, Decision=F. Only D toggles. Decision changes T->F. D independently affects the outcome.

**Verdict:** All four conditions independently demonstrated. MC/DC satisfied with 5 test cases.

**Test Case Mapping:**
- TG-D5-1: `TerrainMCDCTest::IsInsideGrid_Center_ReturnsTrue`
- TG-D5-2: `TerrainMCDCTest::IsInsideGrid_NorthBelowZero_ReturnsFalse`
- TG-D5-3: `TerrainMCDCTest::IsInsideGrid_NorthAboveMax_ReturnsFalse`
- TG-D5-4: `TerrainMCDCTest::IsInsideGrid_EastBelowZero_ReturnsFalse`
- TG-D5-5: `TerrainMCDCTest::IsInsideGrid_EastAboveMax_ReturnsFalse`

---

### 5.6 Decision TG-D6a: GetElevation -- Row Lower Clamp

**Source:** `TerrainGrid.cpp`, function `GetElevation(double northMeters, double eastMeters) const`

**Decision Expression:**
```cpp
if (r0 < 0)
```

This clamps the lower row index for bilinear interpolation to 0 when the query point is at or below the southern grid edge.

**Conditions:**
- A: `r0 < 0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | northMeters       | r0 (computed) | A (r0 < 0) | Decision | Outcome                       |
|-----------|-------------------|---------------|------------|----------|-------------------------------|
| TG-D6a-1  | -15.0 (below grid)| -0.5          | T          | T        | r0 clamped to 0               |
| TG-D6a-2  | 60.0 (inside grid)| 2.0           | F          | F        | r0 unchanged, normal interp   |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- TG-D6a-1: `TerrainMCDCTest::GetElevation_BelowSouthEdge_ClampedRow`
- TG-D6a-2: `TerrainMCDCTest::GetElevation_InsideGrid_NoClamp`

---

### 5.7 Decision TG-D6b: GetElevation -- Row Upper Clamp

**Source:** `TerrainGrid.cpp`, function `GetElevation(double northMeters, double eastMeters) const`

**Decision Expression:**
```cpp
if (r0 >= maxRow)
```

where `maxRow = rows_ - 2` (the last valid interpolation base row, ensuring `r0 + 1` remains within bounds).

**Conditions:**
- A: `r0 >= maxRow`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | northMeters          | r0 (computed) | maxRow | A (r0 >= maxRow) | Decision | Outcome                        |
|-----------|----------------------|---------------|--------|------------------|----------|---------------------------------|
| TG-D6b-1  | 285.0 (above grid)   | 9.5           | 8      | T                | T        | r0 clamped to maxRow            |
| TG-D6b-2  | 60.0 (inside grid)   | 2.0           | 8      | F                | F        | r0 unchanged, normal interp    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- TG-D6b-1: `TerrainMCDCTest::GetElevation_AboveNorthEdge_ClampedRow`
- TG-D6b-2: `TerrainMCDCTest::GetElevation_InsideGrid_NoRowClamp`

---

### 5.8 Decision TG-D6c: GetElevation -- Column Lower Clamp

**Source:** `TerrainGrid.cpp`, function `GetElevation(double northMeters, double eastMeters) const`

**Decision Expression:**
```cpp
if (c0 < 0)
```

This clamps the lower column index for bilinear interpolation to 0 when the query point is at or below the western grid edge.

**Conditions:**
- A: `c0 < 0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | eastMeters         | c0 (computed) | A (c0 < 0) | Decision | Outcome                       |
|-----------|--------------------|---------------|------------|----------|-------------------------------|
| TG-D6c-1  | -15.0 (west of grid)| -0.5         | T          | T        | c0 clamped to 0               |
| TG-D6c-2  | 60.0 (inside grid) | 2.0           | F          | F        | c0 unchanged, normal interp   |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- TG-D6c-1: `TerrainMCDCTest::GetElevation_WestOfGrid_ClampedCol`
- TG-D6c-2: `TerrainMCDCTest::GetElevation_InsideGrid_NoColClamp`

---

### 5.9 Decision TG-D6d: GetElevation -- Column Upper Clamp

**Source:** `TerrainGrid.cpp`, function `GetElevation(double northMeters, double eastMeters) const`

**Decision Expression:**
```cpp
if (c0 >= maxCol)
```

where `maxCol = cols_ - 2` (the last valid interpolation base column, ensuring `c0 + 1` remains within bounds).

**Conditions:**
- A: `c0 >= maxCol`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | eastMeters           | c0 (computed) | maxCol | A (c0 >= maxCol) | Decision | Outcome                        |
|-----------|----------------------|---------------|--------|------------------|----------|---------------------------------|
| TG-D6d-1  | 285.0 (east of grid) | 9.5           | 8      | T                | T        | c0 clamped to maxCol            |
| TG-D6d-2  | 60.0 (inside grid)   | 2.0           | 8      | F                | F        | c0 unchanged, normal interp    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- TG-D6d-1: `TerrainMCDCTest::GetElevation_EastOfGrid_ClampedCol`
- TG-D6d-2: `TerrainMCDCTest::GetElevation_InsideGrid_NoColUpperClamp`

---

### 5.10 Decision TG-D7a: FillSlope -- Outer Loop (Row Iteration)

**Source:** `TerrainGrid.cpp`, function `FillSlope(double baseElevation, double slopeNorth, double slopeEast)`

**Decision Expression:**
```cpp
for (size_t row = 0; row < rows_; ++row)
```

**Loop continuation condition:** `row < rows_`

**Conditions:**
- A: `row < rows_` (evaluated per iteration)

**Decision:** `A`

**Number of conditions:** 1 (per iteration)
**Minimum test cases:** 2 (loop executes at least once + loop terminates)

**Truth Table:**

| Test Case | rows_ | Iterations | A (continue?) | Outcome                       |
|-----------|-------|------------|---------------|-------------------------------|
| TG-D7a-1  | 2     | 2          | T then F      | Loop body executes for rows 0, 1; terminates at row 2 |
| TG-D7a-2  | 10    | 10         | T (9x) then F | Loop body executes for rows 0-9; terminates at row 10 |

**Note:** Because the constructor requires `rows_ >= 2`, the loop always executes at least twice. Both the TRUE path (continue iteration) and FALSE path (exit loop) are exercised in every valid invocation.

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied by any call to `FillSlope()` on a valid grid.

**Test Case Mapping:**
- TG-D7a-1: `TerrainGridDataEntryTest::FillSlope_MinGrid_LoopExecutes`
- TG-D7a-2: `TerrainGridDataEntryTest::FillSlope_LargerGrid_LoopExecutes`

---

### 5.11 Decision TG-D7b: FillSlope -- Inner Loop (Column Iteration)

**Source:** `TerrainGrid.cpp`, function `FillSlope(double baseElevation, double slopeNorth, double slopeEast)`

**Decision Expression:**
```cpp
for (size_t col = 0; col < cols_; ++col)
```

**Loop continuation condition:** `col < cols_`

**Conditions:**
- A: `col < cols_` (evaluated per iteration)

**Decision:** `A`

**Number of conditions:** 1 (per iteration)
**Minimum test cases:** 2 (loop executes at least once + loop terminates)

**Truth Table:**

| Test Case | cols_ | Iterations | A (continue?) | Outcome                       |
|-----------|-------|------------|---------------|-------------------------------|
| TG-D7b-1  | 2     | 2          | T then F      | Loop body executes for cols 0, 1; terminates at col 2 |
| TG-D7b-2  | 10    | 10         | T (9x) then F | Loop body executes for cols 0-9; terminates at col 10 |

**Note:** Because the constructor requires `cols_ >= 2`, the loop always executes at least twice. Both TRUE and FALSE paths are exercised in every valid invocation.

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied by any call to `FillSlope()` on a valid grid.

**Test Case Mapping:**
- TG-D7b-1: `TerrainGridDataEntryTest::FillSlope_MinGrid_InnerLoopExecutes`
- TG-D7b-2: `TerrainGridDataEntryTest::FillSlope_LargerGrid_InnerLoopExecutes`

---

## 6. Detailed Analysis -- TerrainCollisionDetector

### 6.1 Decision CD-D8: SetClearanceThreshold -- Lower Bound Clamp

**Source:** `TerrainCollisionDetector.cpp`, function `SetClearanceThreshold(double threshold)`

**Decision Expression:**
```cpp
if (threshold < 25.0)
```

This clamps the clearance threshold to a minimum of 25 meters, ensuring a minimum safety margin is always maintained for CFIT alerting.

**Conditions:**
- A: `threshold < 25.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | threshold | A (< 25.0) | Decision | Outcome                          |
|-----------|-----------|------------|----------|----------------------------------|
| CD-D8-1   | 10.0      | T          | T        | Threshold clamped to 25.0 meters |
| CD-D8-2   | 100.0     | F          | F        | Threshold set to 100.0 meters    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D8-1: `TerrainCollisionDetectorConfigTest::SetClearanceThreshold_BelowMinimum_Clamped`
- CD-D8-2: `TerrainCollisionDetectorConfigTest::SetClearanceThreshold_ValidValue_Set`

---

### 6.2 Decision CD-D9: SetClearanceThreshold -- Upper Bound Clamp

**Source:** `TerrainCollisionDetector.cpp`, function `SetClearanceThreshold(double threshold)`

**Decision Expression:**
```cpp
if (threshold > 500.0)
```

This clamps the clearance threshold to a maximum of 500 meters, preventing excessive false-alarm generation.

**Conditions:**
- A: `threshold > 500.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | threshold | A (> 500.0) | Decision | Outcome                           |
|-----------|-----------|-------------|----------|------------------------------------|
| CD-D9-1   | 600.0     | T           | T        | Threshold clamped to 500.0 meters  |
| CD-D9-2   | 100.0     | F           | F        | Threshold set to 100.0 meters      |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D9-1: `TerrainCollisionDetectorConfigTest::SetClearanceThreshold_AboveMaximum_Clamped`
- CD-D9-2: `TerrainCollisionDetectorConfigTest::SetClearanceThreshold_ValidValue_Set`

---

### 6.3 Decision CD-D10: SetLookaheadTime -- Lower Bound Clamp

**Source:** `TerrainCollisionDetector.cpp`, function `SetLookaheadTime(double time)`

**Decision Expression:**
```cpp
if (time < 60.0)
```

This clamps the lookahead time to a minimum of 60 seconds, ensuring the CFIT prediction window is long enough to provide actionable alerts.

**Conditions:**
- A: `time < 60.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | time  | A (< 60.0) | Decision | Outcome                            |
|-----------|-------|------------|----------|------------------------------------|
| CD-D10-1  | 30.0  | T          | T        | Lookahead clamped to 60.0 seconds  |
| CD-D10-2  | 120.0 | F          | F        | Lookahead set to 120.0 seconds     |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D10-1: `TerrainCollisionDetectorConfigTest::SetLookaheadTime_BelowMinimum_Clamped`
- CD-D10-2: `TerrainCollisionDetectorConfigTest::SetLookaheadTime_ValidValue_Set`

---

### 6.4 Decision CD-D11: Evaluate -- Loop Continuation Condition (MC/DC REQUIRED)

**Source:** `TerrainCollisionDetector.cpp`, function `Evaluate(Position pos, Velocity vel, double altitude)`

**Decision Expression:**
```cpp
for (int step = 0; step <= maxSteps && t <= lookaheadTime_; ++step)
```

The loop continuation is a compound condition: the loop continues only if both the step count has not exceeded the maximum AND the projected time has not exceeded the lookahead window.

**Conditions:**
- A: `step <= maxSteps`
- B: `t <= lookaheadTime_`

**Decision:** `A && B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**MC/DC Truth Table:**

| Test Case  | step vs maxSteps | t vs lookaheadTime | A (step <= max) | B (t <= lookahead) | Decision (A && B) | Outcome              |
|------------|------------------|--------------------|-----------------|--------------------|--------------------|----------------------|
| CD-D11-1   | step < maxSteps  | t < lookaheadTime  | T               | T                  | T                  | Loop continues       |
| CD-D11-2   | step > maxSteps  | t < lookaheadTime  | F               | T                  | F                  | Loop exits (step limit reached) |
| CD-D11-3   | step < maxSteps  | t > lookaheadTime  | T               | F                  | F                  | Loop exits (time limit reached) |

**Independence Pairs:**

| Condition | Test Pair           | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|---------------------|------------------|------------------|-----------------|
| A         | CD-D11-1 vs CD-D11-2| T -> F           | B = T (fixed)    | T -> F          |
| B         | CD-D11-1 vs CD-D11-3| T -> F           | A = T (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In CD-D11-1, A=T, B=T, Decision=T. In CD-D11-2, A=F, B=T, Decision=F. Only A toggles; B remains T. Decision changes T->F. A independently affects the outcome.
- For condition B: In CD-D11-1, A=T, B=T, Decision=T. In CD-D11-3, A=F (note: at termination, t has exceeded but step has not), B=F, Decision=F. Only B toggles; A remains T. Decision changes T->F. B independently affects the outcome.

**Note on test construction:** CD-D11-2 is exercised by configuring a very short time step relative to maxSteps (or equivalently, a small grid with many steps). CD-D11-3 is exercised by configuring a long time step so that `t` exceeds `lookaheadTime_` before `step` reaches `maxSteps`. Both termination modes must be observed to achieve MC/DC.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- CD-D11-1: `TerrainMCDCTest::Evaluate_LoopContinues_BothConditionsTrue`
- CD-D11-2: `TerrainMCDCTest::Evaluate_LoopExits_StepLimitReached`
- CD-D11-3: `TerrainMCDCTest::Evaluate_LoopExits_TimeLimitReached`

---

### 6.5 Decision CD-D12: Evaluate -- WARNING Alert (Terrain Penetration)

**Source:** `TerrainCollisionDetector.cpp`, function `Evaluate(Position pos, Velocity vel, double altitude)`

**Decision Expression:**
```cpp
if (clearance <= 0.0)
```

This decision triggers a WARNING-level CFIT alert when the projected aircraft altitude at a future time step is at or below the terrain elevation (zero or negative clearance indicates terrain penetration).

**Conditions:**
- A: `clearance <= 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | projected altitude | terrain elevation | clearance | A (<= 0.0) | Decision | Outcome              |
|-----------|-------------------|-------------------|-----------|------------|----------|----------------------|
| CD-D12-1  | 500.0             | 550.0             | -50.0     | T          | T        | Alert = WARNING      |
| CD-D12-2  | 500.0             | 100.0             | 400.0     | F          | F        | No WARNING (check CAUTION next) |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D12-1: `TerrainCollisionDetectorEvaluateTest::Evaluate_TerrainPenetration_WarningAlert`
- CD-D12-2: `TerrainCollisionDetectorEvaluateTest::Evaluate_PositiveClearance_NoWarning`

---

### 6.6 Decision CD-D13: Evaluate -- CAUTION Alert (Low Clearance)

**Source:** `TerrainCollisionDetector.cpp`, function `Evaluate(Position pos, Velocity vel, double altitude)`

**Decision Expression:**
```cpp
if (clearance < threshold * cautionMultiplier)
```

This decision triggers a CAUTION-level CFIT alert when the projected clearance falls below the clearance threshold multiplied by a caution multiplier (typically 1.0 to 2.0, yielding a wider alert envelope). This fires only when CD-D12 was FALSE (clearance > 0, no terrain penetration).

**Conditions:**
- A: `clearance < threshold * cautionMultiplier`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | clearance | threshold | cautionMult | threshold * cautionMult | A (< product) | Decision | Outcome       |
|-----------|-----------|-----------|-------------|-------------------------|----------------|----------|---------------|
| CD-D13-1  | 30.0      | 50.0      | 1.5         | 75.0                    | T              | T        | Alert = CAUTION |
| CD-D13-2  | 200.0     | 50.0      | 1.5         | 75.0                    | F              | F        | Alert = NONE    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D13-1: `TerrainCollisionDetectorEvaluateTest::Evaluate_LowClearance_CautionAlert`
- CD-D13-2: `TerrainCollisionDetectorEvaluateTest::Evaluate_HighClearance_NoAlert`

---

### 6.7 Decision CD-D14: Evaluate -- Minimum Clearance Tracking

**Source:** `TerrainCollisionDetector.cpp`, function `Evaluate(Position pos, Velocity vel, double altitude)`

**Decision Expression:**
```cpp
if (clearance < minClearance)
```

This decision updates the tracked minimum clearance value across all time steps in the evaluation loop. `minClearance` is initialized to a large value (e.g., `std::numeric_limits<double>::max()`) at the start of `Evaluate()` and is updated whenever a smaller clearance is found.

**Conditions:**
- A: `clearance < minClearance`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | clearance | minClearance (current) | A (< minClearance) | Decision | Outcome                        |
|-----------|-----------|------------------------|---------------------|----------|--------------------------------|
| CD-D14-1  | 100.0     | MAX_DOUBLE             | T                   | T        | minClearance updated to 100.0  |
| CD-D14-2  | 200.0     | 100.0                  | F                   | F        | minClearance unchanged (100.0) |

**Note:** CD-D14-1 is always exercised on the first iteration of the evaluation loop (since `minClearance` starts at MAX_DOUBLE). CD-D14-2 is exercised when a subsequent time step has a larger clearance than a previous step, which occurs on terrain that initially descends then rises (valley terrain profile).

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- CD-D14-1: `TerrainCollisionDetectorEvaluateTest::Evaluate_FirstStep_UpdatesMinClearance`
- CD-D14-2: `TerrainCollisionDetectorEvaluateTest::Evaluate_ValleyTerrain_MinClearanceNotUpdatedOnRise`

---

## 7. Coverage Summary

### 7.1 Coverage by Module

| Module                    | Decisions | Conditions | Switch Paths | MC/DC Test Points | Multi-Condition |
|---------------------------|-----------|------------|--------------|-------------------|-----------------|
| TerrainGrid               | 11        | 15         | 0            | 28                | 3 (TG-D1, TG-D3, TG-D5) |
| TerrainCollisionDetector  | 7         | 9          | 0            | 16                | 1 (CD-D11)      |
| **Total**                 | **18**    | **24**     | **0**        | **44**            | **4**           |

### 7.2 MC/DC Status by Decision

| Decision ID | Conditions | MC/DC Cases | Independence Pairs Demonstrated | Status    |
|-------------|------------|-------------|--------------------------------|-----------|
| TG-D1       | 2          | 3           | A: (TG-D1-1 vs TG-D1-3), B: (TG-D1-2 vs TG-D1-3) | Covered |
| TG-D2       | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D3       | 2          | 3           | A: (TG-D3-1 vs TG-D3-3), B: (TG-D3-2 vs TG-D3-3) | Covered |
| TG-D4       | 2          | 3           | A: (TG-D4-1 vs TG-D4-3), B: (TG-D4-2 vs TG-D4-3) | Covered |
| TG-D5       | 4          | 5           | A: (TG-D5-1 vs TG-D5-2), B: (TG-D5-1 vs TG-D5-3), C: (TG-D5-1 vs TG-D5-4), D: (TG-D5-1 vs TG-D5-5) | Covered |
| TG-D6a      | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D6b      | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D6c      | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D6d      | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D7a      | 1          | 2           | N/A (single condition)         | Covered   |
| TG-D7b      | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D8       | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D9       | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D10      | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D11      | 2          | 3           | A: (CD-D11-1 vs CD-D11-2), B: (CD-D11-1 vs CD-D11-3) | Covered |
| CD-D12      | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D13      | 1          | 2           | N/A (single condition)         | Covered   |
| CD-D14      | 1          | 2           | N/A (single condition)         | Covered   |

### 7.3 Overall MC/DC Achievement

| Metric                          | Count |
|---------------------------------|-------|
| Total decisions analyzed        | 18    |
| Decisions with MC/DC covered    | 18    |
| Decisions pending               | 0     |
| Total conditions analyzed       | 24    |
| Multi-condition decisions       | 4 (TG-D1, TG-D3, TG-D4 [2-cond OR]; TG-D5 [4-cond AND]; CD-D11 [2-cond AND]) |
| Independence pairs demonstrated | 12 (2+2+2+4+2 for multi-condition decisions) |
| Total MC/DC test points         | 44    |

---

## 8. Findings

### F1: IsInsideGrid Boundary Inclusivity (TG-D5)

**Decision:** `if (gridRow >= 0.0 && gridRow <= maxRow && gridCol >= 0.0 && gridCol <= maxCol)` in `TerrainGrid::IsInsideGrid()`.

**Finding:** The decision uses `>=` and `<=` comparisons, making the grid boundaries inclusive. A point exactly on the boundary (e.g., `gridRow == 0.0` or `gridCol == maxCol`) is considered inside the grid. However, in `GetElevation()`, the clamping logic handles points at or beyond boundaries by clamping interpolation indices. This means a point reported as "inside" by `IsInsideGrid()` at the exact boundary will use clamped interpolation in `GetElevation()`, which is correct behavior (returning the boundary elevation value).

**Recommendation:** Document that `IsInsideGrid()` boundaries are inclusive and that boundary points produce valid clamped interpolation results. Add explicit boundary tests at `gridRow == 0.0`, `gridRow == maxRow`, `gridCol == 0.0`, and `gridCol == maxCol` to verify consistent behavior between `IsInsideGrid()` and `GetElevation()`.

**Impact on MC/DC:** None. The boundary condition is correctly handled by the existing test cases.

### F2: GetElevation Clamping Chain Independence (TG-D6a through TG-D6d)

**Finding:** The four clamping decisions in `GetElevation()` (TG-D6a through TG-D6d) are independent sequential checks. Each clamp operates on a different variable (`r0` for rows, `c0` for columns) and in a different direction (lower bound vs. upper bound). A single query point can trigger at most one row clamp (lower OR upper, not both) and at most one column clamp (lower OR upper, not both). The corner cases (e.g., below-south AND west-of-grid) trigger both TG-D6a and TG-D6c simultaneously.

**Recommendation:** Add corner-case tests where two clamps fire simultaneously (e.g., southwest corner, northeast corner) to verify that the sequential clamping produces the correct bilinear interpolation result for corner extrapolation.

**Impact on MC/DC:** None. Each decision is single-condition and independently coverable by DC. The simultaneous-clamp corner cases provide additional value-domain coverage beyond MC/DC requirements.

### F3: Evaluate Loop Dual Termination Modes (CD-D11)

**Finding:** The `Evaluate()` loop can terminate by either exceeding `maxSteps` (condition A becomes FALSE) or exceeding `lookaheadTime_` (condition B becomes FALSE). The two termination modes represent different operational scenarios:
- Step-limit termination: the time step is very small relative to lookahead time, so the maximum iteration count is reached first. This is an efficiency guard.
- Time-limit termination: the normal case where the full lookahead window is scanned.

Both modes must be tested to achieve MC/DC. In practice, time-limit termination is the common mode, while step-limit termination is a protective measure against excessively fine time steps.

**Recommendation:** Verify that the alert level and minimum clearance results are consistent regardless of which termination mode triggers. Add a test case where both limits are reached simultaneously (e.g., `maxSteps * timeStep == lookaheadTime`) to verify the boundary behavior.

**Impact on MC/DC:** Both termination modes are covered by the CD-D11 truth table. The simultaneous-termination boundary case is recommended for additional confidence.

### F4: Clearance Threshold Clamping Range (CD-D8, CD-D9)

**Finding:** The clearance threshold is clamped to the range [25.0, 500.0] meters. The lower bound (25 meters) represents a minimum safety margin, and the upper bound (500 meters) prevents excessive false alarms. These values should be traceable to REQ-SF-0402 (Terrain Collision Detection). If the SRS specifies different limits, the implementation and this analysis must be updated accordingly.

**Recommendation:** Cross-reference the clamping constants (25.0, 500.0) against REQ-SF-0402 to confirm they match the specified safety requirements. Add boundary tests at the exact clamp values (25.0 and 500.0) to verify that boundary values are not clamped.

**Impact on MC/DC:** None. Each clamping decision is single-condition and fully covered.

### F5: FillSlope Loop Decisions (TG-D7a, TG-D7b) -- Always Exercised

**Finding:** The `FillSlope()` loop decisions are inherently exercised by any valid call to `FillSlope()`, since the constructor guarantees `rows_ >= 2` and `cols_ >= 2`. Both the TRUE path (loop body executes) and FALSE path (loop terminates) are exercised in every call. This means any test case that calls `FillSlope()` on a valid grid automatically provides DC (and MC/DC, since these are single-condition decisions) for TG-D7a and TG-D7b.

**Recommendation:** No additional dedicated test cases needed beyond what is already planned in the Normal Range and Boundary Value test categories. Document that these loop decisions are implicitly covered.

**Impact on MC/DC:** None. Coverage is automatic for any valid `FillSlope()` invocation.

---

## 9. Traceability to Test Suites

### 9.1 MC/DC Test Case to Test Suite Mapping

| Decision ID | MC/DC Test Case | Test Suite                          | Test Name                                      |
|-------------|-----------------|-------------------------------------|-------------------------------------------------|
| TG-D1       | TG-D1-1         | TerrainGridConstructionTest         | Constructor_RowsTooSmall_Throws                 |
| TG-D1       | TG-D1-2         | TerrainGridConstructionTest         | Constructor_ColsTooSmall_Throws                 |
| TG-D1       | TG-D1-3         | TerrainGridConstructionTest         | Constructor_ValidDimensions_Succeeds            |
| TG-D2       | TG-D2-1         | TerrainGridConstructionTest         | Constructor_ZeroPosting_Throws                  |
| TG-D2       | TG-D2-2         | TerrainGridConstructionTest         | Constructor_ValidPosting_Succeeds               |
| TG-D3       | TG-D3-1         | TerrainGridDataEntryTest            | SetElevation_RowOutOfBounds_Throws              |
| TG-D3       | TG-D3-2         | TerrainGridDataEntryTest            | SetElevation_ColOutOfBounds_Throws              |
| TG-D3       | TG-D3-3         | TerrainGridDataEntryTest            | SetElevation_ValidIndex_Succeeds                |
| TG-D4       | TG-D4-1         | TerrainGridDataEntryTest            | GetElevationAtCell_RowOutOfBounds_Throws        |
| TG-D4       | TG-D4-2         | TerrainGridDataEntryTest            | GetElevationAtCell_ColOutOfBounds_Throws        |
| TG-D4       | TG-D4-3         | TerrainGridDataEntryTest            | GetElevationAtCell_ValidIndex_ReturnsValue       |
| TG-D5       | TG-D5-1         | TerrainMCDCTest                     | IsInsideGrid_Center_ReturnsTrue                  |
| TG-D5       | TG-D5-2         | TerrainMCDCTest                     | IsInsideGrid_NorthBelowZero_ReturnsFalse         |
| TG-D5       | TG-D5-3         | TerrainMCDCTest                     | IsInsideGrid_NorthAboveMax_ReturnsFalse          |
| TG-D5       | TG-D5-4         | TerrainMCDCTest                     | IsInsideGrid_EastBelowZero_ReturnsFalse          |
| TG-D5       | TG-D5-5         | TerrainMCDCTest                     | IsInsideGrid_EastAboveMax_ReturnsFalse           |
| TG-D6a      | TG-D6a-1        | TerrainMCDCTest                     | GetElevation_BelowSouthEdge_ClampedRow           |
| TG-D6a      | TG-D6a-2        | TerrainMCDCTest                     | GetElevation_InsideGrid_NoClamp                  |
| TG-D6b      | TG-D6b-1        | TerrainMCDCTest                     | GetElevation_AboveNorthEdge_ClampedRow           |
| TG-D6b      | TG-D6b-2        | TerrainMCDCTest                     | GetElevation_InsideGrid_NoRowClamp               |
| TG-D6c      | TG-D6c-1        | TerrainMCDCTest                     | GetElevation_WestOfGrid_ClampedCol               |
| TG-D6c      | TG-D6c-2        | TerrainMCDCTest                     | GetElevation_InsideGrid_NoColClamp               |
| TG-D6d      | TG-D6d-1        | TerrainMCDCTest                     | GetElevation_EastOfGrid_ClampedCol               |
| TG-D6d      | TG-D6d-2        | TerrainMCDCTest                     | GetElevation_InsideGrid_NoColUpperClamp          |
| TG-D7a      | TG-D7a-1        | TerrainGridDataEntryTest            | FillSlope_MinGrid_LoopExecutes                   |
| TG-D7a      | TG-D7a-2        | TerrainGridDataEntryTest            | FillSlope_LargerGrid_LoopExecutes                |
| TG-D7b      | TG-D7b-1        | TerrainGridDataEntryTest            | FillSlope_MinGrid_InnerLoopExecutes              |
| TG-D7b      | TG-D7b-2        | TerrainGridDataEntryTest            | FillSlope_LargerGrid_InnerLoopExecutes           |
| CD-D8       | CD-D8-1         | TerrainCollisionDetectorConfigTest  | SetClearanceThreshold_BelowMinimum_Clamped       |
| CD-D8       | CD-D8-2         | TerrainCollisionDetectorConfigTest  | SetClearanceThreshold_ValidValue_Set             |
| CD-D9       | CD-D9-1         | TerrainCollisionDetectorConfigTest  | SetClearanceThreshold_AboveMaximum_Clamped       |
| CD-D9       | CD-D9-2         | TerrainCollisionDetectorConfigTest  | SetClearanceThreshold_ValidValue_Set             |
| CD-D10      | CD-D10-1        | TerrainCollisionDetectorConfigTest  | SetLookaheadTime_BelowMinimum_Clamped            |
| CD-D10      | CD-D10-2        | TerrainCollisionDetectorConfigTest  | SetLookaheadTime_ValidValue_Set                  |
| CD-D11      | CD-D11-1        | TerrainMCDCTest                     | Evaluate_LoopContinues_BothConditionsTrue         |
| CD-D11      | CD-D11-2        | TerrainMCDCTest                     | Evaluate_LoopExits_StepLimitReached               |
| CD-D11      | CD-D11-3        | TerrainMCDCTest                     | Evaluate_LoopExits_TimeLimitReached               |
| CD-D12      | CD-D12-1        | TerrainCollisionDetectorEvaluateTest| Evaluate_TerrainPenetration_WarningAlert          |
| CD-D12      | CD-D12-2        | TerrainCollisionDetectorEvaluateTest| Evaluate_PositiveClearance_NoWarning              |
| CD-D13      | CD-D13-1        | TerrainCollisionDetectorEvaluateTest| Evaluate_LowClearance_CautionAlert                |
| CD-D13      | CD-D13-2        | TerrainCollisionDetectorEvaluateTest| Evaluate_HighClearance_NoAlert                    |
| CD-D14      | CD-D14-1        | TerrainCollisionDetectorEvaluateTest| Evaluate_FirstStep_UpdatesMinClearance            |
| CD-D14      | CD-D14-2        | TerrainCollisionDetectorEvaluateTest| Evaluate_ValleyTerrain_MinClearanceNotUpdatedOnRise|

---

## 10. Revision History

| Rev | Date       | Author                          | Description              |
|-----|------------|---------------------------------|--------------------------|
| 1.0 | 2026-03-14 | LLM-Assisted (Claude Opus 4.6) | Initial MC/DC analysis   |
