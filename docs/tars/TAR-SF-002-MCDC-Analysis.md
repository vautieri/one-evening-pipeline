# MC/DC Structural Coverage Analysis Report

| Field              | Value                                                        |
|--------------------|--------------------------------------------------------------|
| Document ID        | MCDC-SF-TAR002-2026-001                                      |
| TAR Reference      | TAR-SF-002 — Atmosphere Model Verification & DAL A Coverage Enhancement |
| DAL                | A (Catastrophic) — 100% MC/DC Required                       |
| Standard           | DO-178C Section 6.4.4.2 — Structural Coverage Analysis       |
| SVP Reference      | SVP-SF-2026-001, Section 8                                   |
| Modules Under Test | FGAtmosphereBase (src/models/FGAtmosphereBase.h)             |
|                    | FGStandardAtmosphere1976 (src/models/FGStandardAtmosphere1976.h/.cpp) |
|                    | SimpleTable (src/models/FGStandardAtmosphere1976.h)          |
| Date               | 2026-03-14                                                   |
| Revision           | 1.0                                                          |
| Status             | Baseline                                                     |

---

## 1. Purpose

This document provides the formal Modified Condition/Decision Coverage (MC/DC) analysis for all decision points in the FGAtmosphereBase, FGStandardAtmosphere1976, and SimpleTable modules, as required by DO-178C Section 6.4.4.2 for DAL A software.

For every Boolean decision in the source code, this document:

1. Identifies the decision expression and its constituent conditions.
2. Constructs the MC/DC truth table demonstrating that each condition can independently affect the decision outcome.
3. Identifies the independence pairs that satisfy the unique-cause MC/DC criterion.
4. Specifies the minimum test cases required and maps them to test case identifiers in the test suites.
5. Documents total coverage metrics across all three modules.

This analysis covers all decisions in the `.h` header files (inline implementations) and the `.cpp` implementation file. Functions with no Boolean decisions (simple getters, delegation-only functions) are excluded from MC/DC analysis but are covered by statement/decision coverage through normal test execution.

---

## 2. Applicable Documents

| Document ID          | Title                                                         |
|----------------------|---------------------------------------------------------------|
| DO-178C              | Software Considerations in Airborne Systems and Equipment Certification |
| SVP-SF-2026-001      | Software Verification Plan                                    |
| TAR-SF-002           | Atmosphere Model Verification & DAL A Coverage Enhancement    |
| SRS-SF-2026-001      | Software Requirements Specification                           |
| SCS-SF-2026-001      | Software Coding Standards                                     |
| PSAC-SF-2026-001     | Plan for Software Aspects of Certification                    |
| NASA TM-X-74335      | U.S. Standard Atmosphere, 1976                                |

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

For switch statements:
- Each case label (including default) must be exercised at least once.
- Total test cases = number of case labels.

### 3.3 Notation

Throughout this document:
- **T** = TRUE, **F** = FALSE
- A condition labeled with its variable name (e.g., A, B) represents the truth value of that sub-expression.
- Independence pairs are written as (Test X vs Test Y) where the condition under test toggles and the decision outcome changes.

### 3.4 Loop Decision Treatment

Several functions contain `for` loops with break conditions (layer search in pressure/density altitude calculations). Each loop iteration's break condition is treated as a single-condition decision. Decision Coverage (TRUE = break, FALSE = continue) is sufficient for MC/DC. The loop's continuation condition (`b < numRows - 2`) is an implicit decision covered by ensuring the loop runs to completion at least once (highest-layer test case) and breaks early (lower-layer test case).

### 3.5 Switch Statement Treatment

Switch statements on enumeration types are treated as multi-path decisions. Each case label represents a distinct execution path. Coverage requires exercising every case label at least once, including the `default` case (which throws `std::invalid_argument`). Switch statements do not have compound Boolean conditions, so MC/DC reduces to path coverage.

---

## 4. Decision Inventory

### 4.1 Summary Table -- FGAtmosphereBase

| Decision ID | Source Location                     | Expression                                    | Type     | Conditions | MC/DC Cases |
|-------------|-------------------------------------|-----------------------------------------------|----------|------------|-------------|
| AB-D1       | `ValidatePressure()`                | `p < MinPressure`                             | if       | 1          | 2           |
| AB-D2       | `ValidateTemperature()`             | `t < minT`                                    | if       | 1          | 2           |
| AB-D3       | `ConvertToRankine()`                | `switch (unit)` — eFahrenheit, eCelsius, eRankine, eKelvin, default | switch | 5 paths | 5   |
| AB-D4       | `ConvertFromRankine()`              | `switch (unit)` — eFahrenheit, eCelsius, eRankine, eKelvin, default | switch | 5 paths | 5   |
| AB-D5       | `ConvertToPSF()`                    | `switch (unit)` — ePSF, eMillibars, ePascals, eInchesHg, default    | switch | 5 paths | 5   |
| AB-D6       | `ConvertFromPSF()`                  | `switch (unit)` — ePSF, eMillibars, ePascals, eInchesHg, default    | switch | 5 paths | 5   |
| **Subtotal**|                                     |                                               |          | **4 + 20 paths** | **24** |

### 4.2 Summary Table -- FGStandardAtmosphere1976

| Decision ID | Source Location                          | Expression                                                | Type     | Conditions | MC/DC Cases |
|-------------|------------------------------------------|-----------------------------------------------------------|----------|------------|-------------|
| SA-D1       | `GetTemperature()` line 141              | `GeoPotAlt >= 0.0`                                        | if       | 1          | 2           |
| SA-D2       | `GetTemperature()` line 144              | `GeoPotAlt <= GradientFadeoutAltitude`                    | if       | 1          | 2           |
| SA-D3       | `GetTemperature()` line 153              | `GeoPotAlt <= GradientFadeoutAltitude`                    | if       | 1          | 2           |
| SA-D4       | `GetStdTemperature()` line 163           | `GeoPotAlt >= 0.0`                                        | if       | 1          | 2           |
| SA-D5       | `GetPressure()` line 184                 | `GeoPotAlt < testAlt` (loop break)                        | if/loop  | 1 per iter | 2           |
| SA-D6       | `GetPressure()` line 192                 | `Lmb != 0.0`                                              | if       | 1          | 2           |
| SA-D7       | `GetStdPressure()` line 212             | `GeoPotAlt < testAlt` (loop break)                        | if/loop  | 1 per iter | 2           |
| SA-D8       | `GetStdPressure()` line 221             | `Lmb != 0.0`                                              | if       | 1          | 2           |
| SA-D9       | `SetTemperatureBias()` line 266          | `unit == eCelsius \|\| unit == eKelvin`                   | if       | **2**      | **3**       |
| SA-D10      | `SetTemperatureBias()` line 274          | `minStdTemp + TemperatureBias < minUniverseTemperature`   | if       | 1          | 2           |
| SA-D11      | `SetTemperatureGradedDelta()` line 290   | `unit == eCelsius \|\| unit == eKelvin`                   | if       | **2**      | **3**       |
| SA-D12      | `SetTemperatureGradedDelta()` line 296   | `deltemp <= minDelta`                                     | if       | 1          | 2           |
| SA-D13      | `CalculatePressureBreakpoints()` line 372| `LapseRates[b] != 0.0` (loop body)                       | if       | 1          | 2           |
| SA-D14      | `CalculateDensityAltitude()` line 407    | `density >= StdDensityBreakpoints[b+1]` (loop break)      | if/loop  | 1 per iter | 2           |
| SA-D15      | `CalculateDensityAltitude()` line 418    | `Lmb != 0.0`                                              | if       | 1          | 2           |
| SA-D16      | `CalculatePressureAltitude()` line 437   | `pressure >= StdPressureBreakpoints[b+1]` (loop break)    | if/loop  | 1 per iter | 2           |
| SA-D17      | `CalculatePressureAltitude()` line 449   | `Lmb != 0.0`                                              | if       | 1          | 2           |
| **Subtotal**|                                          |                                                           |          | **19**     | **36**      |

### 4.3 Summary Table -- SimpleTable

| Decision ID | Source Location                     | Expression                   | Type     | Conditions | MC/DC Cases |
|-------------|-------------------------------------|------------------------------|----------|------------|-------------|
| ST-D1       | `operator<<` line 37               | `pendingKey_`                 | if       | 1          | 2           |
| ST-D2       | `GetValue()` line 49               | `keys_.empty()`              | if       | 1          | 2           |
| ST-D3       | `GetValue()` line 50               | `key <= keys_.front()`       | if       | 1          | 2           |
| ST-D4       | `GetValue()` line 51               | `key >= keys_.back()`        | if       | 1          | 2           |
| ST-D5       | `GetValue()` line 56               | `i == 0`                     | if       | 1          | 2           |
| **Subtotal**|                                     |                              |          | **5**      | **10**      |

### 4.4 Grand Total

| Metric                                   | FGAtmosphereBase | FGStdAtmos1976 | SimpleTable | Total  |
|------------------------------------------|------------------|----------------|-------------|--------|
| Distinct decision points                 | 6                | 17             | 5           | 28     |
| Boolean conditions (non-switch)          | 2                | 19             | 5           | 26     |
| Switch paths                             | 20               | 0              | 0           | 20     |
| Multi-condition decisions                | 0                | 2              | 0           | 2      |
| Total MC/DC test cases required          | 24               | 36             | 10          | 70     |

---

## 5. Detailed Analysis -- FGAtmosphereBase

### 5.1 Decision AB-D1: ValidatePressure -- Minimum Pressure Guard

**Source:** `FGAtmosphereBase.h`, line 262, function `ValidatePressure(double p, const std::string&, bool) const`

**Decision Expression:**
```cpp
if (p < MinPressure)
```

where `MinPressure = ConvertToPSF(1E-15, ePascals)` (approximately 2.09E-17 psf).

**Conditions:**
- A: `p < MinPressure`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2 (Decision Coverage equivalent to MC/DC for single condition)

**Truth Table:**

| Test Case | p Value         | A (p < MinPressure) | Decision | Outcome                     |
|-----------|-----------------|---------------------|----------|-----------------------------|
| AB-D1-1   | 0.0             | T                   | T        | Returns MinPressure (clamped)|
| AB-D1-2   | 2116.228 (SL)   | F                   | F        | Returns p unchanged          |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- AB-D1-1: `AtmosphereMCDCTest::ValidatePressure_BelowMinimum_Clamped`
- AB-D1-2: `AtmosphereMCDCTest::ValidatePressure_AboveMinimum_Unchanged`

---

### 5.2 Decision AB-D2: ValidateTemperature -- Minimum Temperature Guard

**Source:** `FGAtmosphereBase.h`, line 267, function `ValidateTemperature(double t, const std::string&, bool) const`

**Decision Expression:**
```cpp
if (t < minT)
```

where `minT = KelvinToRankine(1.0) = 1.8` Rankine.

**Conditions:**
- A: `t < minT`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | t Value         | A (t < 1.8)  | Decision | Outcome                     |
|-----------|-----------------|--------------|----------|-----------------------------|
| AB-D2-1   | 0.0             | T            | T        | Returns 1.8 R (clamped)     |
| AB-D2-2   | 518.67 (SL)     | F            | F        | Returns t unchanged          |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- AB-D2-1: `AtmosphereMCDCTest::ValidateTemperature_BelowMinimum_Clamped`
- AB-D2-2: `AtmosphereMCDCTest::ValidateTemperature_AboveMinimum_Unchanged`

---

### 5.3 Decision AB-D3: ConvertToRankine -- Temperature Unit Switch

**Source:** `FGAtmosphereBase.h`, lines 275-281, function `ConvertToRankine(double t, eTemperature unit) const`

**Decision Expression:**
```cpp
switch (unit) {
    case eFahrenheit: return t + 459.67;
    case eCelsius:    return (t + 273.15) * 1.8;
    case eRankine:    return t;
    case eKelvin:     return t * 1.8;
    default: throw std::invalid_argument("Undefined temperature unit");
}
```

**Paths:** 5 (4 valid cases + 1 default/error)

**Path Coverage Table:**

| Test Case | unit          | Path Taken   | Input t | Expected Output          |
|-----------|---------------|--------------|---------|--------------------------|
| AB-D3-1   | eFahrenheit   | case 1       | 32.0    | 491.67                   |
| AB-D3-2   | eCelsius      | case 2       | 0.0     | 491.67                   |
| AB-D3-3   | eRankine      | case 3       | 518.67  | 518.67                   |
| AB-D3-4   | eKelvin       | case 4       | 273.15  | 491.67                   |
| AB-D3-5   | eNoTempUnit   | default      | 0.0     | throws invalid_argument  |

**Verdict:** All 5 paths exercised. Switch coverage complete.

**Test Case Mapping:**
- AB-D3-1: `FGAtmosphereBaseTest::ConvertToRankine_Fahrenheit`
- AB-D3-2: `FGAtmosphereBaseTest::ConvertToRankine_Celsius`
- AB-D3-3: `FGAtmosphereBaseTest::ConvertToRankine_Rankine`
- AB-D3-4: `FGAtmosphereBaseTest::ConvertToRankine_Kelvin`
- AB-D3-5: `FGAtmosphereBaseTest::ConvertToRankine_InvalidUnit_Throws`

---

### 5.4 Decision AB-D4: ConvertFromRankine -- Temperature Unit Switch

**Source:** `FGAtmosphereBase.h`, lines 286-292, function `ConvertFromRankine(double t, eTemperature unit) const`

**Decision Expression:**
```cpp
switch (unit) {
    case eFahrenheit: return t - 459.67;
    case eCelsius:    return t / 1.8 - 273.15;
    case eRankine:    return t;
    case eKelvin:     return t / 1.8;
    default: throw std::invalid_argument("Undefined temperature unit");
}
```

**Paths:** 5

**Path Coverage Table:**

| Test Case | unit          | Path Taken   | Input t  | Expected Output          |
|-----------|---------------|--------------|----------|--------------------------|
| AB-D4-1   | eFahrenheit   | case 1       | 491.67   | 32.0                     |
| AB-D4-2   | eCelsius      | case 2       | 491.67   | 0.0                      |
| AB-D4-3   | eRankine      | case 3       | 518.67   | 518.67                   |
| AB-D4-4   | eKelvin       | case 4       | 491.67   | 273.15                   |
| AB-D4-5   | eNoTempUnit   | default      | 0.0      | throws invalid_argument  |

**Verdict:** All 5 paths exercised. Switch coverage complete.

**Test Case Mapping:**
- AB-D4-1: `FGAtmosphereBaseTest::ConvertFromRankine_Fahrenheit`
- AB-D4-2: `FGAtmosphereBaseTest::ConvertFromRankine_Celsius`
- AB-D4-3: `FGAtmosphereBaseTest::ConvertFromRankine_Rankine`
- AB-D4-4: `FGAtmosphereBaseTest::ConvertFromRankine_Kelvin`
- AB-D4-5: `FGAtmosphereBaseTest::ConvertFromRankine_InvalidUnit_Throws`

---

### 5.5 Decision AB-D5: ConvertToPSF -- Pressure Unit Switch

**Source:** `FGAtmosphereBase.h`, lines 297-303, function `ConvertToPSF(double p, ePressure unit) const`

**Decision Expression:**
```cpp
switch (unit) {
    case ePSF:       return p;
    case eMillibars: return p * 2.08854342;
    case ePascals:   return p * 0.0208854342;
    case eInchesHg:  return p * 70.7180803;
    default: throw std::invalid_argument("Undefined pressure unit");
}
```

**Paths:** 5

**Path Coverage Table:**

| Test Case | unit          | Path Taken   | Input p   | Expected Output          |
|-----------|---------------|--------------|-----------|--------------------------|
| AB-D5-1   | ePSF          | case 1       | 2116.228  | 2116.228                 |
| AB-D5-2   | eMillibars    | case 2       | 1013.25   | ~2116.228                |
| AB-D5-3   | ePascals      | case 3       | 101325.0  | ~2116.228                |
| AB-D5-4   | eInchesHg     | case 4       | 29.9213   | ~2116.228                |
| AB-D5-5   | eNoPressUnit  | default      | 0.0       | throws invalid_argument  |

**Verdict:** All 5 paths exercised. Switch coverage complete.

**Test Case Mapping:**
- AB-D5-1: `FGAtmosphereBaseTest::ConvertToPSF_PSF`
- AB-D5-2: `FGAtmosphereBaseTest::ConvertToPSF_Millibars`
- AB-D5-3: `FGAtmosphereBaseTest::ConvertToPSF_Pascals`
- AB-D5-4: `FGAtmosphereBaseTest::ConvertToPSF_InchesHg`
- AB-D5-5: `FGAtmosphereBaseTest::ConvertToPSF_InvalidUnit_Throws`

---

### 5.6 Decision AB-D6: ConvertFromPSF -- Pressure Unit Switch

**Source:** `FGAtmosphereBase.h`, lines 308-314, function `ConvertFromPSF(double p, ePressure unit) const`

**Decision Expression:**
```cpp
switch (unit) {
    case ePSF:       return p;
    case eMillibars: return p / 2.08854342;
    case ePascals:   return p / 0.0208854342;
    case eInchesHg:  return p / 70.7180803;
    default: throw std::invalid_argument("Undefined pressure unit");
}
```

**Paths:** 5

**Path Coverage Table:**

| Test Case | unit          | Path Taken   | Input p    | Expected Output          |
|-----------|---------------|--------------|------------|--------------------------|
| AB-D6-1   | ePSF          | case 1       | 2116.228   | 2116.228                 |
| AB-D6-2   | eMillibars    | case 2       | 2116.228   | ~1013.25                 |
| AB-D6-3   | ePascals      | case 3       | 2116.228   | ~101325.0                |
| AB-D6-4   | eInchesHg     | case 4       | 2116.228   | ~29.9213                 |
| AB-D6-5   | eNoPressUnit  | default      | 0.0        | throws invalid_argument  |

**Verdict:** All 5 paths exercised. Switch coverage complete.

**Test Case Mapping:**
- AB-D6-1: `FGAtmosphereBaseTest::ConvertFromPSF_PSF`
- AB-D6-2: `FGAtmosphereBaseTest::ConvertFromPSF_Millibars`
- AB-D6-3: `FGAtmosphereBaseTest::ConvertFromPSF_Pascals`
- AB-D6-4: `FGAtmosphereBaseTest::ConvertFromPSF_InchesHg`
- AB-D6-5: `FGAtmosphereBaseTest::ConvertFromPSF_InvalidUnit_Throws`

---

## 6. Detailed Analysis -- FGStandardAtmosphere1976

### 6.1 Decision SA-D1: GetTemperature -- Sea-Level Altitude Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 141, function `GetTemperature(double altitude) const`

**Decision Expression:**
```cpp
if (GeoPotAlt >= 0.0)
```

**Conditions:**
- A: `GeoPotAlt >= 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | altitude (ft) | GeoPotAlt | A (>= 0.0) | Decision | Outcome                    |
|-----------|---------------|-----------|-------------|----------|----------------------------|
| SA-D1-1   | 10000.0       | ~9999.95  | T           | T        | Table lookup + bias/gradient|
| SA-D1-2   | -1000.0       | ~-1000.05 | F           | F        | Extrapolate below sea level |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D1-1: `FGStdAtmos1976Test::GetTemperature_AboveSeaLevel`
- SA-D1-2: `FGStdAtmos1976Test::GetTemperature_BelowSeaLevel_Extrapolated`

---

### 6.2 Decision SA-D2: GetTemperature -- Gradient Delta Application (Inside D1-TRUE)

**Source:** `FGStandardAtmosphere1976.cpp`, line 144, function `GetTemperature(double altitude) const`

**Decision Expression:**
```cpp
if (GeoPotAlt <= GradientFadeoutAltitude)
```

This decision controls whether the `TemperatureDeltaGradient` is subtracted from the table-looked-up temperature.

**Conditions:**
- A: `GeoPotAlt <= GradientFadeoutAltitude`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | GeoPotAlt          | GradientFadeoutAlt | A (<= fadeout) | Decision | Outcome                           |
|-----------|--------------------|---------------------|----------------|----------|------------------------------------|
| SA-D2-1   | 50000.0            | 298556.43           | T              | T        | Gradient delta subtracted          |
| SA-D2-2   | 300000.0 (geopot)  | 298556.43           | F              | F        | Gradient delta NOT subtracted      |

**Note:** To exercise the FALSE path, the altitude must be above the highest ISA table entry (298556.43 ft geopotential). This requires setting a non-zero `TemperatureDeltaGradient` first via `SetTemperatureGradedDelta()` so the subtraction has observable effect.

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D2-1: `AtmosphereMCDCTest::GetTemperature_GradientDelta_AppliedBelowFadeout`
- SA-D2-2: `AtmosphereMCDCTest::GetTemperature_GradientDelta_NotAppliedAboveFadeout`

---

### 6.3 Decision SA-D3: GetTemperature -- Gradient Fadeout Addition (After Bias)

**Source:** `FGStandardAtmosphere1976.cpp`, line 153, function `GetTemperature(double altitude) const`

**Decision Expression:**
```cpp
if (GeoPotAlt <= GradientFadeoutAltitude)
```

This decision controls whether the fadeout-altitude gradient correction (`TemperatureDeltaGradient * GradientFadeoutAltitude`) is added back to the temperature after the bias.

**Conditions:**
- A: `GeoPotAlt <= GradientFadeoutAltitude`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | GeoPotAlt          | A (<= fadeout) | Decision | Outcome                              |
|-----------|--------------------|----------------|----------|---------------------------------------|
| SA-D3-1   | 50000.0            | T              | T        | Fadeout correction added              |
| SA-D3-2   | 300000.0 (geopot)  | F              | F        | Fadeout correction NOT added          |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D3-1: `AtmosphereMCDCTest::GetTemperature_FadeoutCorrection_AppliedBelowFadeout`
- SA-D3-2: `AtmosphereMCDCTest::GetTemperature_FadeoutCorrection_NotAppliedAboveFadeout`

---

### 6.4 Decision SA-D4: GetStdTemperature -- Sea-Level Altitude Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 163, function `GetStdTemperature(double altitude) const`

**Decision Expression:**
```cpp
if (GeoPotAlt >= 0.0)
```

**Conditions:**
- A: `GeoPotAlt >= 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | altitude (ft)  | GeoPotAlt | A (>= 0.0) | Decision | Outcome                        |
|-----------|----------------|-----------|-------------|----------|--------------------------------|
| SA-D4-1   | 36089.0        | ~36088.8  | T           | T        | Table lookup (standard)        |
| SA-D4-2   | -500.0         | ~-500.0   | F           | F        | Extrapolate using StdLapseRates|

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D4-1: `FGStdAtmos1976Test::GetStdTemperature_AboveSeaLevel`
- SA-D4-2: `FGStdAtmos1976Test::GetStdTemperature_BelowSeaLevel_Extrapolated`

---

### 6.5 Decision SA-D5: GetPressure -- Layer Search Loop Break

**Source:** `FGStandardAtmosphere1976.cpp`, line 184, function `GetPressure(double altitude) const`

**Decision Expression:**
```cpp
for (b = 0; b < numRows - 2; ++b) {
    double testAlt = StdAtmosTemperatureTable(b + 2, 0);
    if (GeoPotAlt < testAlt) break;
    BaseAlt = testAlt;
}
```

**Loop break condition:** `GeoPotAlt < testAlt`

**Conditions:**
- A: `GeoPotAlt < testAlt` (evaluated per iteration)

**Decision:** `A`

**Number of conditions:** 1 (per iteration)
**Minimum test cases:** 2 (break early + run through multiple layers)

**Truth Table:**

| Test Case | altitude (ft) | Layer Found | A (break?) | Iterations | Outcome               |
|-----------|---------------|-------------|------------|------------|------------------------|
| SA-D5-1   | 5000.0        | Layer 0     | T (iter 0) | 1          | Breaks at first check  |
| SA-D5-2   | 200000.0      | Layer 6     | F (many)   | 7          | Iterates through most  |

**Verdict:** Single condition per iteration -- Decision Coverage sufficient. MC/DC satisfied. Additional test cases at each layer boundary improve confidence but are not strictly required for MC/DC.

**Test Case Mapping:**
- SA-D5-1: `FGStdAtmos1976Test::GetPressure_TroposphereLayer_BreaksEarly`
- SA-D5-2: `FGStdAtmos1976Test::GetPressure_HighAltitudeLayer_IteratesThrough`

---

### 6.6 Decision SA-D6: GetPressure -- Isothermal Layer Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 192, function `GetPressure(double altitude) const`

**Decision Expression:**
```cpp
if (Lmb != 0.0)
```

This selects between equation 33a (non-zero lapse rate, power law) and equation 33b (isothermal, exponential).

**Conditions:**
- A: `Lmb != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | altitude (ft) | Layer | Lmb        | A (Lmb != 0.0) | Decision | Pressure Equation |
|-----------|---------------|-------|------------|-----------------|----------|-------------------|
| SA-D6-1   | 5000.0        | 0     | -0.00356616| T               | T        | Eq. 33a (power)   |
| SA-D6-2   | 45000.0       | 1     | 0.0        | F               | F        | Eq. 33b (exp)     |

**Note:** Layer 0 (troposphere, 0-36089 ft) has a non-zero lapse rate of -6.5 K/km. Layer 1 (tropopause, 36089-65617 ft) is isothermal (Lmb = 0.0).

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D6-1: `FGStdAtmos1976Test::GetPressure_NonIsothermalLayer_PowerLaw`
- SA-D6-2: `FGStdAtmos1976Test::GetPressure_IsothermalLayer_Exponential`

---

### 6.7 Decision SA-D7: GetStdPressure -- Layer Search Loop Break

**Source:** `FGStandardAtmosphere1976.cpp`, line 212, function `GetStdPressure(double altitude) const`

**Decision Expression:**
```cpp
if (GeoPotAlt < testAlt) break;
```

Structurally identical to SA-D5 but uses `StdLapseRates` and `StdPressureBreakpoints`.

**Conditions:**
- A: `GeoPotAlt < testAlt` (per iteration)

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | altitude (ft) | Layer Found | A (break?) | Outcome               |
|-----------|---------------|-------------|------------|------------------------|
| SA-D7-1   | 5000.0        | Layer 0     | T (iter 0) | Breaks at first check  |
| SA-D7-2   | 200000.0      | Layer 6     | F (many)   | Iterates through most  |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D7-1: `FGStdAtmos1976Test::GetStdPressure_TroposphereLayer_BreaksEarly`
- SA-D7-2: `FGStdAtmos1976Test::GetStdPressure_HighAltitudeLayer_IteratesThrough`

---

### 6.8 Decision SA-D8: GetStdPressure -- Isothermal Layer Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 221, function `GetStdPressure(double altitude) const`

**Decision Expression:**
```cpp
if (Lmb != 0.0)
```

**Conditions:**
- A: `Lmb != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | altitude (ft) | Layer | StdLapseRate | A (Lmb != 0.0) | Decision | Equation |
|-----------|---------------|-------|-------------|-----------------|----------|----------|
| SA-D8-1   | 5000.0        | 0     | non-zero    | T               | T        | Eq. 33a  |
| SA-D8-2   | 45000.0       | 1     | 0.0         | F               | F        | Eq. 33b  |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D8-1: `FGStdAtmos1976Test::GetStdPressure_NonIsothermalLayer_PowerLaw`
- SA-D8-2: `FGStdAtmos1976Test::GetStdPressure_IsothermalLayer_Exponential`

---

### 6.9 Decision SA-D9: SetTemperatureBias -- Unit Conversion Check (MC/DC REQUIRED)

**Source:** `FGStandardAtmosphere1976.cpp`, line 266, function `SetTemperatureBias(eTemperature unit, double t)`

**Decision Expression:**
```cpp
if (unit == eCelsius || unit == eKelvin)
```

**Conditions:**
- A: `unit == eCelsius`
- B: `unit == eKelvin`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**Note on mutual exclusivity:** Conditions A and B are mutually exclusive (a unit enum cannot simultaneously be eCelsius and eKelvin). This means `A = T && B = T` is infeasible. Under unique-cause MC/DC with mutually exclusive conditions, the standard N+1 minimum of 3 test cases is still achievable and sufficient.

**MC/DC Truth Table:**

| Test Case | unit      | A (== eCelsius) | B (== eKelvin) | Decision (A \|\| B) | Outcome                     |
|-----------|-----------|-----------------|----------------|----------------------|-----------------------------|
| SA-D9-1   | eCelsius  | T               | F              | T                    | t *= 1.80 (scale applied)   |
| SA-D9-2   | eKelvin   | F               | T              | T                    | t *= 1.80 (scale applied)   |
| SA-D9-3   | eRankine  | F               | F              | F                    | t unchanged (no scaling)    |

**Independence Pairs:**

| Condition | Test Pair         | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|-------------------|------------------|------------------|-----------------|
| A         | SA-D9-1 vs SA-D9-3| T -> F           | B = F (fixed)    | T -> F          |
| B         | SA-D9-2 vs SA-D9-3| T -> F           | A = F (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In SA-D9-1, A=T, B=F, Decision=T. In SA-D9-3, A=F, B=F, Decision=F. Only A toggles; B remains F. Decision changes T->F. A independently affects the outcome.
- For condition B: In SA-D9-2, A=F, B=T, Decision=T. In SA-D9-3, A=F, B=F, Decision=F. Only B toggles; A remains F. Decision changes T->F. B independently affects the outcome.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- SA-D9-1: `AtmosphereMCDCTest::SetTemperatureBias_Celsius_ScaleApplied`
- SA-D9-2: `AtmosphereMCDCTest::SetTemperatureBias_Kelvin_ScaleApplied`
- SA-D9-3: `AtmosphereMCDCTest::SetTemperatureBias_Rankine_NoScale`

---

### 6.10 Decision SA-D10: SetTemperatureBias -- Absolute Zero Clamp

**Source:** `FGStandardAtmosphere1976.cpp`, line 274, function `SetTemperatureBias(eTemperature unit, double t)`

**Decision Expression:**
```cpp
if (minStdTemp + TemperatureBias < minUniverseTemperature)
```

where `minUniverseTemperature = KelvinToRankine(1.0) = 1.8` R and `minStdTemp = StdAtmosTemperatureTable.GetMinValue()` (approximately 336.5028 R, the minimum ISA temperature in the mesopause layer).

**Conditions:**
- A: `minStdTemp + TemperatureBias < minUniverseTemperature`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | TemperatureBias | minStdTemp + Bias | A (< 1.8) | Decision | Outcome                        |
|-----------|-----------------|-------------------|-----------|----------|--------------------------------|
| SA-D10-1  | -400.0 R        | ~-63.5 R          | T         | T        | Bias clamped to (1.8 - 336.5)  |
| SA-D10-2  | -10.0 R         | ~326.5 R          | F         | F        | Bias unchanged                 |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D10-1: `AtmosphereMCDCTest::SetTemperatureBias_ExtremeNegative_ClampedToAbsoluteZero`
- SA-D10-2: `AtmosphereMCDCTest::SetTemperatureBias_ModerateNegative_NotClamped`

---

### 6.11 Decision SA-D11: SetTemperatureGradedDelta -- Unit Conversion Check (MC/DC REQUIRED)

**Source:** `FGStandardAtmosphere1976.cpp`, line 290, function `SetTemperatureGradedDelta(double deltemp, double h, eTemperature unit)`

**Decision Expression:**
```cpp
if (unit == eCelsius || unit == eKelvin)
```

**Conditions:**
- A: `unit == eCelsius`
- B: `unit == eKelvin`

**Decision:** `A || B`

**Number of conditions:** 2
**Minimum MC/DC test cases:** 3 (N + 1 = 2 + 1)

**Note:** Structurally identical to SA-D9. Same mutual exclusivity applies.

**MC/DC Truth Table:**

| Test Case  | unit      | A (== eCelsius) | B (== eKelvin) | Decision (A \|\| B) | Outcome                     |
|------------|-----------|-----------------|----------------|----------------------|-----------------------------|
| SA-D11-1   | eCelsius  | T               | F              | T                    | deltemp *= 1.80 (scaled)    |
| SA-D11-2   | eKelvin   | F               | T              | T                    | deltemp *= 1.80 (scaled)    |
| SA-D11-3   | eRankine  | F               | F              | F                    | deltemp unchanged            |

**Independence Pairs:**

| Condition | Test Pair           | Condition Toggle | Other Conditions | Decision Toggle |
|-----------|---------------------|------------------|------------------|-----------------|
| A         | SA-D11-1 vs SA-D11-3| T -> F           | B = F (fixed)    | T -> F          |
| B         | SA-D11-2 vs SA-D11-3| T -> F           | A = F (fixed)    | T -> F          |

**Verification of independence:**
- For condition A: In SA-D11-1, A=T, B=F, Decision=T. In SA-D11-3, A=F, B=F, Decision=F. Only A toggles; B remains F. Decision changes T->F. A independently affects the outcome.
- For condition B: In SA-D11-2, A=F, B=T, Decision=T. In SA-D11-3, A=F, B=F, Decision=F. Only B toggles; A remains F. Decision changes T->F. B independently affects the outcome.

**Verdict:** Both conditions independently demonstrated. MC/DC satisfied with 3 test cases.

**Test Case Mapping:**
- SA-D11-1: `AtmosphereMCDCTest::SetTemperatureGradedDelta_Celsius_ScaleApplied`
- SA-D11-2: `AtmosphereMCDCTest::SetTemperatureGradedDelta_Kelvin_ScaleApplied`
- SA-D11-3: `AtmosphereMCDCTest::SetTemperatureGradedDelta_Rankine_NoScale`

---

### 6.12 Decision SA-D12: SetTemperatureGradedDelta -- Minimum Delta Clamp

**Source:** `FGStandardAtmosphere1976.cpp`, line 296, function `SetTemperatureGradedDelta(double deltemp, double h, eTemperature unit)`

**Decision Expression:**
```cpp
if (deltemp <= minDelta)
```

where `minDelta = minStdTemp - StdSLtemperature` (approximately 336.5028 - 518.67 = -182.17 R).

**Conditions:**
- A: `deltemp <= minDelta`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | deltemp (R) | minDelta (~-182.17) | A (<= minDelta) | Decision | Outcome              |
|-----------|-------------|---------------------|-----------------|----------|----------------------|
| SA-D12-1  | -200.0      | ~-182.17            | T               | T        | Clamped to minDelta  |
| SA-D12-2  | -10.0       | ~-182.17            | F               | F        | deltemp unchanged    |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D12-1: `AtmosphereMCDCTest::SetTemperatureGradedDelta_ExtremeNegative_Clamped`
- SA-D12-2: `AtmosphereMCDCTest::SetTemperatureGradedDelta_ModerateNegative_NotClamped`

---

### 6.13 Decision SA-D13: CalculatePressureBreakpoints -- Lapse Rate Check (Loop Body)

**Source:** `FGStandardAtmosphere1976.cpp`, line 372, function `CalculatePressureBreakpoints(double SLpress)`

**Decision Expression:**
```cpp
if (LapseRates[b] != 0.0)
```

This decision is evaluated once per layer boundary in a loop over all ISA layers. It selects between the power-law formula (non-zero lapse rate) and the exponential formula (isothermal).

**Conditions:**
- A: `LapseRates[b] != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Layer b | LapseRates[b] | A (!= 0.0) | Decision | Pressure Formula |
|-----------|---------|---------------|-------------|----------|------------------|
| SA-D13-1  | 0       | -0.003566     | T           | T        | Power law        |
| SA-D13-2  | 1       | 0.0           | F           | F        | Exponential      |

**Note:** Both paths are exercised during construction and `InitModel()` as the ISA profile contains both isothermal layers (layers 1 and 4) and non-isothermal layers (layers 0, 2, 3, 5, 6). The loop inherently exercises both paths.

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D13-1: `FGStdAtmos1976Test::PressureBreakpoints_NonIsothermalLayer_PowerLaw`
- SA-D13-2: `FGStdAtmos1976Test::PressureBreakpoints_IsothermalLayer_Exponential`

---

### 6.14 Decision SA-D14: CalculateDensityAltitude -- Layer Search Loop Break

**Source:** `FGStandardAtmosphere1976.cpp`, line 407, function `CalculateDensityAltitude(double density, double geometricAlt)`

**Decision Expression:**
```cpp
if (density >= StdDensityBreakpoints[b+1])
    break;
```

**Conditions:**
- A: `density >= StdDensityBreakpoints[b+1]` (per iteration)

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | density         | Layer Found | A (break?) | Outcome               |
|-----------|-----------------|-------------|------------|------------------------|
| SA-D14-1  | SL density      | Layer 0     | T (iter 0) | Breaks at first check  |
| SA-D14-2  | Very low density| Layer 6+    | F (many)   | Iterates through most  |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D14-1: `FGStdAtmos1976Test::DensityAltitude_SeaLevelDensity_BreaksEarly`
- SA-D14-2: `FGStdAtmos1976Test::DensityAltitude_HighAltDensity_IteratesThrough`

---

### 6.15 Decision SA-D15: CalculateDensityAltitude -- Isothermal Layer Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 418, function `CalculateDensityAltitude(double density, double geometricAlt)`

**Decision Expression:**
```cpp
if (Lmb != 0.0)
```

**Conditions:**
- A: `Lmb != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Layer | StdLapseRate | A (Lmb != 0.0) | Decision | Inverse Formula          |
|-----------|-------|-------------|-----------------|----------|--------------------------|
| SA-D15-1  | 0     | non-zero    | T               | T        | Power-law inverse        |
| SA-D15-2  | 1     | 0.0         | F               | F        | Logarithmic inverse      |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D15-1: `FGStdAtmos1976Test::DensityAltitude_NonIsothermalLayer_PowerInverse`
- SA-D15-2: `FGStdAtmos1976Test::DensityAltitude_IsothermalLayer_LogInverse`

---

### 6.16 Decision SA-D16: CalculatePressureAltitude -- Layer Search Loop Break

**Source:** `FGStandardAtmosphere1976.cpp`, line 437, function `CalculatePressureAltitude(double pressure, double geometricAlt)`

**Decision Expression:**
```cpp
if (pressure >= StdPressureBreakpoints[b+1])
    break;
```

**Conditions:**
- A: `pressure >= StdPressureBreakpoints[b+1]` (per iteration)

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | pressure        | Layer Found | A (break?) | Outcome               |
|-----------|-----------------|-------------|------------|------------------------|
| SA-D16-1  | SL pressure     | Layer 0     | T (iter 0) | Breaks at first check  |
| SA-D16-2  | Very low press  | Layer 6+    | F (many)   | Iterates through most  |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D16-1: `FGStdAtmos1976Test::PressureAltitude_SeaLevelPressure_BreaksEarly`
- SA-D16-2: `FGStdAtmos1976Test::PressureAltitude_HighAltPressure_IteratesThrough`

---

### 6.17 Decision SA-D17: CalculatePressureAltitude -- Isothermal Layer Check

**Source:** `FGStandardAtmosphere1976.cpp`, line 449, function `CalculatePressureAltitude(double pressure, double geometricAlt)`

**Decision Expression:**
```cpp
if (Lmb != 0.0)
```

**Conditions:**
- A: `Lmb != 0.0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Layer | StdLapseRate | A (Lmb != 0.0) | Decision | Inverse Formula          |
|-----------|-------|-------------|-----------------|----------|--------------------------|
| SA-D17-1  | 0     | non-zero    | T               | T        | Power-law inverse        |
| SA-D17-2  | 1     | 0.0         | F               | F        | Logarithmic inverse      |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- SA-D17-1: `FGStdAtmos1976Test::PressureAltitude_NonIsothermalLayer_PowerInverse`
- SA-D17-2: `FGStdAtmos1976Test::PressureAltitude_IsothermalLayer_LogInverse`

---

## 7. Detailed Analysis -- SimpleTable

### 7.1 Decision ST-D1: operator<< -- Pending Key State Toggle

**Source:** `FGStandardAtmosphere1976.h`, line 37, function `operator<<(double v)`

**Decision Expression:**
```cpp
if (pendingKey_)
```

The `operator<<` alternates between storing a key (when `pendingKey_` is FALSE) and storing a value (when `pendingKey_` is TRUE). Each call to `operator<<` toggles `pendingKey_`.

**Conditions:**
- A: `pendingKey_`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | pendingKey_ (before call) | A      | Decision | Outcome                    |
|-----------|---------------------------|--------|----------|----------------------------|
| ST-D1-1   | false (initial)           | F      | F        | v stored as key; pendingKey_ = true  |
| ST-D1-2   | true (after key stored)   | T      | T        | v stored as value; pendingKey_ = false |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- ST-D1-1: `SimpleTableTest::StreamInsert_FirstValue_StoredAsKey`
- ST-D1-2: `SimpleTableTest::StreamInsert_SecondValue_StoredAsValue`

---

### 7.2 Decision ST-D2: GetValue -- Empty Table Guard

**Source:** `FGStandardAtmosphere1976.h`, line 49, function `GetValue(double key) const`

**Decision Expression:**
```cpp
if (keys_.empty())
```

**Conditions:**
- A: `keys_.empty()`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | Table State  | A (empty?) | Decision | Outcome              |
|-----------|--------------|------------|----------|----------------------|
| ST-D2-1   | No entries   | T          | T        | Returns 0.0          |
| ST-D2-2   | Has entries  | F          | F        | Proceeds to lookup   |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- ST-D2-1: `SimpleTableTest::GetValue_EmptyTable_ReturnsZero`
- ST-D2-2: `SimpleTableTest::GetValue_PopulatedTable_ReturnsInterpolated`

---

### 7.3 Decision ST-D3: GetValue -- Below-Table Clamp

**Source:** `FGStandardAtmosphere1976.h`, line 50, function `GetValue(double key) const`

**Decision Expression:**
```cpp
if (key <= keys_.front())
```

**Conditions:**
- A: `key <= keys_.front()`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | key   | keys_.front() | A (<= front) | Decision | Outcome                   |
|-----------|-------|---------------|--------------|----------|---------------------------|
| ST-D3-1   | -100.0| 0.0           | T            | T        | Returns values_.front()   |
| ST-D3-2   | 5000.0| 0.0           | F            | F        | Proceeds to upper check   |

**Note:** The boundary case `key == keys_.front()` triggers TRUE, returning the first value without interpolation. This is a boundary value test case.

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- ST-D3-1: `SimpleTableTest::GetValue_BelowTable_ClampsToFirst`
- ST-D3-2: `SimpleTableTest::GetValue_InRange_ProceedsToInterpolation`

---

### 7.4 Decision ST-D4: GetValue -- Above-Table Clamp

**Source:** `FGStandardAtmosphere1976.h`, line 51, function `GetValue(double key) const`

**Decision Expression:**
```cpp
if (key >= keys_.back())
```

**Conditions:**
- A: `key >= keys_.back()`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | key      | keys_.back()   | A (>= back) | Decision | Outcome                   |
|-----------|----------|----------------|-------------|----------|---------------------------|
| ST-D4-1   | 500000.0 | 298556.43      | T           | T        | Returns values_.back()    |
| ST-D4-2   | 50000.0  | 298556.43      | F           | F        | Proceeds to binary search |

**Verdict:** Single condition -- Decision Coverage sufficient. MC/DC satisfied.

**Test Case Mapping:**
- ST-D4-1: `SimpleTableTest::GetValue_AboveTable_ClampsToLast`
- ST-D4-2: `SimpleTableTest::GetValue_InRange_ProceedsToBinarySearch`

---

### 7.5 Decision ST-D5: GetValue -- Binary Search Edge Guard

**Source:** `FGStandardAtmosphere1976.h`, line 56, function `GetValue(double key) const`

**Decision Expression:**
```cpp
if (i == 0) i = 1;
```

This guard ensures that after `std::lower_bound`, the index `i` is at least 1, so that `keys_[i-1]` is valid. In practice, this should not trigger because the earlier bounds checks (`key <= keys_.front()`) would have returned before reaching this point. However, the guard protects against edge cases in `std::lower_bound` behavior.

**Conditions:**
- A: `i == 0`

**Decision:** `A`

**Number of conditions:** 1
**Minimum test cases:** 2

**Truth Table:**

| Test Case | key                | i (after lower_bound) | A (i == 0) | Decision | Outcome              |
|-----------|--------------------|-----------------------|------------|----------|----------------------|
| ST-D5-1   | keys_.front()+eps  | 1                     | F          | F        | i unchanged          |
| ST-D5-2   | (see note)         | 0                     | T          | T        | i set to 1           |

**Note on ST-D5-2:** Exercising `i == 0` after `lower_bound` is difficult because the earlier clamp checks (`key <= keys_.front()`) prevent reaching this code when `key` would produce `i = 0`. In practice, this line may be defensive dead code. If analysis confirms it is unreachable, this will be documented as a finding (see Section 9). If the earlier clamp uses `<=` (which it does), then `key == keys_.front()` returns before reaching `lower_bound`, and any `key > keys_.front()` will produce `i >= 1`. This decision point requires either:
- Confirming the code is unreachable and documenting the justification, or
- Constructing a test case that reaches this code by manipulating internal state.

**Verdict:** Single condition -- subject to reachability analysis. If reachable, DC is sufficient. See Finding F1 in Section 9.

**Test Case Mapping:**
- ST-D5-1: `SimpleTableTest::GetValue_MidRange_IndexNonZero`
- ST-D5-2: Pending reachability analysis (see Finding F1)

---

## 8. Coverage Summary

### 8.1 Coverage by Module

| Module                    | Decisions | Conditions | Switch Paths | MC/DC Test Points | Multi-Condition |
|---------------------------|-----------|------------|--------------|-------------------|-----------------|
| FGAtmosphereBase          | 6         | 2          | 20           | 24                | 0               |
| FGStandardAtmosphere1976  | 17        | 19         | 0            | 36                | 2 (SA-D9, SA-D11) |
| SimpleTable               | 5         | 5          | 0            | 10                | 0               |
| **Total**                 | **28**    | **26**     | **20**       | **70**            | **2**           |

### 8.2 MC/DC Status by Decision

| Decision ID | Conditions | MC/DC Cases | Independence Pairs Demonstrated | Status    |
|-------------|------------|-------------|--------------------------------|-----------|
| AB-D1       | 1          | 2           | N/A (single condition)         | Covered   |
| AB-D2       | 1          | 2           | N/A (single condition)         | Covered   |
| AB-D3       | 5 paths    | 5           | N/A (switch)                   | Covered   |
| AB-D4       | 5 paths    | 5           | N/A (switch)                   | Covered   |
| AB-D5       | 5 paths    | 5           | N/A (switch)                   | Covered   |
| AB-D6       | 5 paths    | 5           | N/A (switch)                   | Covered   |
| SA-D1       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D2       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D3       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D4       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D5       | 1 per iter | 2           | N/A (single condition)         | Covered   |
| SA-D6       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D7       | 1 per iter | 2           | N/A (single condition)         | Covered   |
| SA-D8       | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D9       | 2          | 3           | A: (SA-D9-1 vs SA-D9-3), B: (SA-D9-2 vs SA-D9-3) | Covered |
| SA-D10      | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D11      | 2          | 3           | A: (SA-D11-1 vs SA-D11-3), B: (SA-D11-2 vs SA-D11-3) | Covered |
| SA-D12      | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D13      | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D14      | 1 per iter | 2           | N/A (single condition)         | Covered   |
| SA-D15      | 1          | 2           | N/A (single condition)         | Covered   |
| SA-D16      | 1 per iter | 2           | N/A (single condition)         | Covered   |
| SA-D17      | 1          | 2           | N/A (single condition)         | Covered   |
| ST-D1       | 1          | 2           | N/A (single condition)         | Covered   |
| ST-D2       | 1          | 2           | N/A (single condition)         | Covered   |
| ST-D3       | 1          | 2           | N/A (single condition)         | Covered   |
| ST-D4       | 1          | 2           | N/A (single condition)         | Covered   |
| ST-D5       | 1          | 2           | N/A (single condition)         | Pending   |

### 8.3 Overall MC/DC Achievement

| Metric                          | Count |
|---------------------------------|-------|
| Total decisions analyzed        | 28    |
| Decisions with MC/DC covered    | 27    |
| Decisions pending (ST-D5)       | 1     |
| Total conditions analyzed       | 26 + 20 switch paths = 46 |
| Multi-condition decisions       | 2 (SA-D9, SA-D11)          |
| Independence pairs demonstrated | 4 (2 per multi-condition decision) |
| Total MC/DC test points         | 70    |

---

## 9. Findings

### F1: SimpleTable GetValue -- Defensive Guard (ST-D5) Reachability

**Decision:** `if (i == 0) i = 1` in `SimpleTable::GetValue()`, line 56.

**Finding:** Static analysis of the control flow indicates that this decision may be unreachable under normal conditions:

1. The `keys_.empty()` check (ST-D2) returns 0.0 if the table has no entries.
2. The `key <= keys_.front()` check (ST-D3) returns `values_.front()` if the key is at or below the first entry.
3. The `key >= keys_.back()` check (ST-D4) returns `values_.back()` if the key is at or above the last entry.
4. After these three guards, `key` is strictly between `keys_.front()` and `keys_.back()`.
5. `std::lower_bound` on a sorted vector with `key > keys_.front()` will always return an iterator with `i >= 1`.

**Conclusion:** The guard `if (i == 0) i = 1` appears to be defensive dead code. Under the precondition that `keys_` is sorted in ascending order and the three earlier guards have been passed, `std::lower_bound` will never return the begin iterator (i.e., `i` will never be 0).

**Recommendation:** Document this as justified defensive code per DO-178C Section 6.4.4.3 (structural coverage analysis resolution). The code is harmless and provides additional safety against unexpected `lower_bound` behavior on degenerate inputs. No removal is required, but the FALSE path of ST-D5 may need to be marked as justified in the coverage report.

**Impact on MC/DC:** If confirmed unreachable, this decision is excluded from MC/DC requirements per DO-178C Section 6.4.4.3. The overall MC/DC percentage for SimpleTable becomes 4/4 (100%) rather than 4/5 (80%) with documented justification.

### F2: Mutually Exclusive Conditions in SA-D9 and SA-D11

**Finding:** The two conditions in decisions SA-D9 and SA-D11 (`unit == eCelsius || unit == eKelvin`) are mutually exclusive -- a variable cannot simultaneously equal two different enum values. This means the test case where both conditions are TRUE (A=T, B=T) is infeasible.

**Impact:** Under unique-cause MC/DC, mutual exclusivity does not prevent achieving coverage. The three test cases (A=T/B=F, A=F/B=T, A=F/B=F) are sufficient and represent all feasible combinations. The independence pairs are valid because each condition individually toggles the decision outcome. Some MC/DC analysis frameworks (notably CAST Position Paper 6) discuss this scenario and confirm that 3 test cases are sufficient for 2 mutually exclusive conditions in an OR decision.

### F3: Loop Decisions and Iteration Coverage

**Finding:** Decisions SA-D5, SA-D7, SA-D14, and SA-D16 are loop break conditions. Each iteration evaluates the same single-condition decision. For MC/DC purposes, exercising the TRUE path (break) and FALSE path (continue) at least once is sufficient.

**Recommendation:** For additional confidence (beyond MC/DC requirements), test at every ISA layer boundary to verify that the loop correctly identifies all 8 atmospheric layers. This provides value-domain coverage complementary to structural coverage.

### F4: GeometricAltitude Singularity

**Finding:** `GeometricAltitude(double geopotalt)` computes `(geopotalt * EarthRadius) / (EarthRadius - geopotalt)`. When `geopotalt == EarthRadius` (~20855531.5 ft), division by zero occurs. This altitude is far beyond the ISA model's valid range (~0 to 84852 m geopotential), but the function itself has no guard.

**Recommendation:** Document the valid input range constraint. Consider adding a defensive check if this function is called from contexts where the geopotential altitude is not already bounded. This is outside the MC/DC analysis scope but is noted for completeness.

---

## 10. Revision History

| Rev | Date       | Author                          | Description              |
|-----|------------|---------------------------------|--------------------------|
| 1.0 | 2026-03-14 | Development Team (LLM-Assisted) | Initial MC/DC analysis   |
