# Software Code Standards

**Document ID:** SCS-SF-2026-001
**Project:** SynthFlight -- UAV Mission Simulation and Verification Platform
**DAL:** A (Catastrophic) -- DO-178C
**Language:** C++17 (ISO/IEC 14882:2017)
**Compiler:** GCC
**Build System:** CMake >= 3.16
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Draft

---

## Table of Contents

1. [Purpose](#1-purpose)
2. [Language Standard](#2-language-standard)
3. [Compiler Settings](#3-compiler-settings)
4. [Naming Conventions](#4-naming-conventions)
5. [File Organization](#5-file-organization)
6. [Code Formatting](#6-code-formatting)
7. [C++17 Feature Usage](#7-c17-feature-usage)
8. [Error Handling](#8-error-handling)
9. [Documentation](#9-documentation)
10. [Testing-Related Code Rules](#10-testing-related-code-rules)
11. [Prohibited Practices](#11-prohibited-practices)
12. [COTS/Reuse Code](#12-cotsreuse-code)
13. [Static Analysis](#13-static-analysis)
14. [Code Review Checklist](#14-code-review-checklist)
15. [References](#15-references)
16. [Revision History](#16-revision-history)

---

## 1. Purpose

This document defines the software coding standards for the SynthFlight UAV Mission Simulation and Verification Platform. It establishes mandatory rules for writing, formatting, documenting, and reviewing C++17 source code to ensure:

- Consistency and readability across the entire codebase
- Compliance with DO-178C DAL A objectives for code quality and traceability
- Safe and deterministic behavior suitable for safety-critical simulation
- Maintainability and verifiability over the product lifecycle

All developers writing, modifying, or reviewing SynthFlight source code shall comply with this standard. Deviations are permitted only for COTS/reuse code as defined in Section 12.

### 1.1 Scope

This standard applies to:

- All C++ source files (`.cpp`, `.cc`) and header files (`.h`, `.hpp`) in the SynthFlight repository
- Test source files
- Inline code in CMake-generated configuration headers

This standard does not apply to:

- Build system scripts (CMakeLists.txt, `.cmake` files) -- these follow CMake conventions
- Third-party library source code included as-is (e.g., unmodified JSBSim source)
- Generated code (e.g., moc output from Qt) -- though generated code shall be reviewed for safety impact

### 1.2 Terminology

| Term | Definition |
|---|---|
| **Shall** | Mandatory rule; violation requires correction before code acceptance |
| **Should** | Recommended practice; deviation requires documented justification |
| **May** | Optional practice; no justification required for deviation |

---

## 2. Language Standard

| Rule ID | Rule |
|---|---|
| LANG-01 | All SynthFlight source code shall conform to ISO/IEC 14882:2017 (C++17) |
| LANG-02 | No compiler-specific extensions shall be used unless abstracted behind a portability macro and documented in the SDD |
| LANG-03 | GCC-specific built-ins (e.g., `__builtin_expect`, `__attribute__`) are prohibited in application code; they may be used in platform abstraction headers with documented justification |
| LANG-04 | C language constructs that have C++ equivalents shall use the C++ form (e.g., `static_cast` instead of C-style cast, `constexpr` instead of `#define`) |

---

## 3. Compiler Settings

### 3.1 Required Compiler and Version

| Rule ID | Rule |
|---|---|
| COMP-01 | The reference compiler is GCC; the minimum supported version shall be documented in the project's CMakeLists.txt |
| COMP-02 | All code shall compile cleanly (zero warnings) with the mandatory warning flags |

### 3.2 Mandatory Warning Flags

The following flags shall be enabled for all SynthFlight build targets:

```
-std=c++17
-Wall
-Wextra
-Werror
-Wpedantic
```

| Flag | Purpose |
|---|---|
| `-std=c++17` | Enforce C++17 language standard |
| `-Wall` | Enable all common warnings |
| `-Wextra` | Enable additional warnings beyond `-Wall` |
| `-Werror` | Treat all warnings as errors; no code shall be committed with warnings |
| `-Wpedantic` | Warn on non-standard extensions |

### 3.3 Recommended Warning Flags

The following additional flags should be enabled. They are mandatory for new modules and recommended for legacy/JSBSim-derived code:

```
-Wshadow
-Wnon-virtual-dtor
-Wold-style-cast
-Wcast-align
-Woverloaded-virtual
```

| Flag | Purpose |
|---|---|
| `-Wshadow` | Warn when a local variable shadows another variable |
| `-Wnon-virtual-dtor` | Warn when a class with virtual functions has a non-virtual destructor |
| `-Wold-style-cast` | Warn on C-style casts |
| `-Wcast-align` | Warn on potentially unsafe alignment casts |
| `-Woverloaded-virtual` | Warn when a function declaration hides a virtual function from a base class |

### 3.4 Build Configuration Flags

| Configuration | Flags | Purpose |
|---|---|---|
| **Debug** | `-g -O0 -fsanitize=address,undefined` | Full debug symbols, no optimization, runtime sanitizers for memory and undefined behavior detection |
| **Release** | `-O2 -DNDEBUG` | Optimization level 2, disable assertions |
| **RelWithDebInfo** | `-O2 -g -DNDEBUG` | Release optimization with debug symbols for profiling |

| Rule ID | Rule |
|---|---|
| COMP-03 | All unit tests shall pass in both Debug and Release configurations |
| COMP-04 | AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan) builds shall be run as part of CI; any sanitizer finding is a blocking defect |
| COMP-05 | The `-DNDEBUG` flag shall only disable `assert()` macro checks; application-level validation logic shall not be guarded by `NDEBUG` |

### 3.5 CMake Integration

```cmake
# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Required compiler flags (example)
target_compile_options(${TARGET} PRIVATE
  -std=c++17
  -Wall
  -Wextra
  -Werror
  -Wpedantic
  -Wshadow
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Wcast-align
  -Woverloaded-virtual
)
```

---

## 4. Naming Conventions

### 4.1 General Principles

| Rule ID | Rule |
|---|---|
| NAME-01 | Names shall be descriptive and self-documenting; abbreviations are permitted only for widely understood domain terms (e.g., `NED`, `ECEF`, `EOM`, `DOF`) |
| NAME-02 | Single-character variable names are prohibited except for loop indices (`i`, `j`, `k`) and conventional mathematical notation in physics equations (documented in Doxygen) |
| NAME-03 | Names shall not differ only in case (e.g., `velocity` and `Velocity` in the same scope) |

### 4.2 Naming Rules by Element

| Element | Convention | Examples |
|---|---|---|
| **Classes** | `PascalCase` | `FGColumnVector3`, `FGMatrix33`, `WaypointManager`, `AtmosphereAdapter` |
| **Structs** | `PascalCase` | `AircraftState`, `ControlOutput` |
| **Member functions** | `PascalCase` | `GetMagnitude()`, `SetTemperature()`, `ComputePressure()` |
| **Free functions** | `PascalCase` | `ConvertMetersToFeet()`, `NormalizeAngle()` |
| **Member variables** | `camelCase` (no prefix for new code) | `altitude`, `airSpeed`, `rollAngle` |
| **Local variables** | `camelCase` | `deltaTime`, `pressureRatio` |
| **Function parameters** | `camelCase` | `geometricAltitude`, `machNumber` |
| **Constants** | `ALL_CAPS_WITH_UNDERSCORES` | `MAX_ALTITUDE_M`, `SPEED_OF_SOUND_MSL`, `PI` |
| **Enum types** | `PascalCase` | `CoordinateSystem`, `SensorType` |
| **Enum values** | `PascalCase` or `ALL_CAPS` | `CoordinateSystem::BodyFixed` or `BODY_FIXED` |
| **Namespaces** | `PascalCase` | `SynthFlight`, `SynthFlight::Math`, `SynthFlight::Models` |
| **Template parameters** | `PascalCase` | `typename VectorType`, `size_t N` |
| **Macros** (when unavoidable) | `ALL_CAPS_WITH_UNDERSCORES` | `SYNTHFLIGHT_VERSION_MAJOR` |

### 4.3 JSBSim Convention Retention

| Rule ID | Rule |
|---|---|
| NAME-04 | JSBSim-derived classes shall retain the `FG` prefix (e.g., `FGColumnVector3`, `FGMatrix33`, `FGAtmosphereBase`, `FGStandardAtmosphere1976`) |
| NAME-05 | JSBSim-derived code shall retain its existing naming style where modification would introduce defects or diverge from upstream |
| NAME-06 | New code added within JSBSim-derived classes shall follow the JSBSim naming conventions for consistency within that class |

### 4.4 File Names

| Rule ID | Rule |
|---|---|
| NAME-07 | Header file names shall match the primary class name with `.h` extension (e.g., `FGColumnVector3.h`, `WaypointManager.h`) |
| NAME-08 | Source file names shall match the primary class name with `.cpp` extension (e.g., `FGColumnVector3.cpp`, `WaypointManager.cpp`) |
| NAME-09 | Test file names shall follow the pattern `test_{module}_{framework}.cpp` (e.g., `test_FGColumnVector3_gtest.cpp`, `test_atmosphere_catch2.cpp`) |
| NAME-10 | File names shall not contain spaces or special characters other than underscores and hyphens |

---

## 5. File Organization

### 5.1 Header Guards

| Rule ID | Rule |
|---|---|
| FILE-01 | `#pragma once` is the preferred header guard mechanism |
| FILE-02 | As an alternative, traditional include guards are acceptable using the pattern `#ifndef CLASSNAME_H` / `#define CLASSNAME_H` / `#endif // CLASSNAME_H` |
| FILE-03 | Include guard macro names shall match the file name in `ALL_CAPS` with underscores replacing non-alphanumeric characters |

### 5.2 Include Order

| Rule ID | Rule |
|---|---|
| FILE-04 | Include statements shall be organized in the following order, separated by blank lines between groups: |

```cpp
// Group 1: Corresponding header (in .cpp files only)
#include "FGColumnVector3.h"

// Group 2: Standard library headers (alphabetical)
#include <algorithm>
#include <cmath>
#include <vector>

// Group 3: Third-party library headers (alphabetical)
#include <gtest/gtest.h>
#include <JSBSim/math/FGMatrix33.h>

// Group 4: Project headers (alphabetical)
#include "SynthFlight/Math/CoordinateTransform.h"
#include "SynthFlight/Models/AtmosphereAdapter.h"
```

| Rule ID | Rule |
|---|---|
| FILE-05 | Within each group, headers shall be sorted alphabetically |
| FILE-06 | Forward declarations shall be preferred over includes in header files where the full type definition is not required |
| FILE-07 | Every header shall be self-contained: it shall compile successfully when included in isolation |

### 5.3 File Content Rules

| Rule ID | Rule |
|---|---|
| FILE-08 | Each header file shall contain at most one primary class definition (exception: closely related utility classes such as a class and its associated iterator or builder) |
| FILE-09 | Implementation shall be in `.cpp` files; inline definitions in headers are permitted only for trivial accessors, `constexpr` functions, and template definitions |
| FILE-10 | Every source file shall begin with a copyright/license header block followed by the file-level Doxygen comment |
| FILE-11 | Anonymous namespaces shall be used for file-scope helper functions and constants (not `static` at file scope) |

### 5.4 File Template

```cpp
/// @file FGColumnVector3.h
/// @brief 3-element column vector for flight dynamics calculations.
/// @requirement REQ-SF-0001, REQ-SF-0002
///
/// Copyright (c) 2026 SynthFlight Project. All rights reserved.
/// SPDX-License-Identifier: [LICENSE]

#pragma once

#include <cmath>
#include <stdexcept>

namespace SynthFlight::Math {

/// @brief A 3-element column vector used for position, velocity, and force
///        representations in flight dynamics.
///
/// Thread safety: Thread-Compatible
///
/// @see FGMatrix33
class FGColumnVector3 {
public:
  // ... public interface
private:
  // ... private members
};

} // namespace SynthFlight::Math
```

---

## 6. Code Formatting

### 6.1 Indentation

| Rule ID | Rule |
|---|---|
| FMT-01 | Indentation shall use 2 spaces per level |
| FMT-02 | Tab characters are prohibited in source files |
| FMT-03 | Continuation lines shall be indented by 4 spaces from the start of the continued statement |

### 6.2 Braces

| Rule ID | Rule |
|---|---|
| FMT-04 | Opening braces shall appear on the same line as the statement (K&R style) |
| FMT-05 | Single-statement bodies of `if`, `for`, `while`, and `do` shall use braces |

```cpp
// Correct: K&R style, braces on same line
if (altitude > MAX_ALTITUDE_M) {
  throw std::range_error("Altitude exceeds model limit");
}

// Correct: Single-statement body with braces
for (int i = 0; i < 3; ++i) {
  result(i) = lhs(i) + rhs(i);
}

// Incorrect: Allman style (opening brace on new line)
if (altitude > MAX_ALTITUDE_M)
{
  throw std::range_error("Altitude exceeds model limit");
}

// Incorrect: No braces on single-statement body
for (int i = 0; i < 3; ++i)
  result(i) = lhs(i) + rhs(i);
```

### 6.3 Line Length

| Rule ID | Rule |
|---|---|
| FMT-06 | Lines shall not exceed 100 characters |
| FMT-07 | Exceptions to the 100-character limit: URLs in comments, include paths, and string literals that would lose clarity if broken |

### 6.4 Spacing

| Rule ID | Rule |
|---|---|
| FMT-08 | Pointer and reference declarations: the `*` or `&` shall be adjacent to the type, separated from the variable name by a space |

```cpp
// Correct
double* ptr = &value;
const FGColumnVector3& vec = GetPosition();

// Incorrect
double *ptr = &value;
const FGColumnVector3 &vec = GetPosition();
```

| Rule ID | Rule |
|---|---|
| FMT-09 | Binary operators shall be surrounded by spaces: `a + b`, `x = y`, `i < n` |
| FMT-10 | Unary operators shall not be separated from their operand: `++i`, `!flag`, `-value` |
| FMT-11 | Commas shall be followed by a space: `Foo(a, b, c)` |
| FMT-12 | No space before opening parenthesis in function calls: `Foo()`, not `Foo ()` |
| FMT-13 | No trailing whitespace on any line |

### 6.5 Blank Lines

| Rule ID | Rule |
|---|---|
| FMT-14 | Use one blank line to separate logical sections within a function |
| FMT-15 | Use two blank lines to separate class member sections (public/protected/private) or top-level definitions |
| FMT-16 | No more than two consecutive blank lines |

### 6.6 Class Layout

Members within a class definition shall be ordered as follows:

```cpp
class ClassName {
public:
  // Type aliases and nested types
  // Static constants
  // Constructors and destructor
  // Copy/move operations
  // Public member functions

protected:
  // Protected member functions
  // Protected member variables

private:
  // Private member functions
  // Private member variables
};
```

---

## 7. C++17 Feature Usage

### 7.1 Allowed Features

The following C++17 features are approved for use in SynthFlight:

| Feature | Usage Guidance |
|---|---|
| `std::optional<T>` | Preferred for representing optional values and nullable return types; use instead of sentinel values |
| `std::variant<Ts...>` | Preferred for type-safe unions; use instead of `void*` or `union` |
| `std::string_view` | Use for non-owning string references in function parameters; do not store `string_view` beyond the lifetime of the referenced string |
| `std::filesystem` | Use for all file path manipulation and directory operations |
| Structured bindings | Use for decomposing `std::pair`, `std::tuple`, and simple structs: `auto [x, y, z] = GetPosition();` |
| `if constexpr` | Use for compile-time conditional logic in templates |
| Fold expressions | Use for variadic template parameter pack operations |
| `[[nodiscard]]` | Apply to all functions where ignoring the return value is likely a programming error |
| `[[maybe_unused]]` | Apply to parameters that are intentionally unused (e.g., interface compliance) |
| `[[fallthrough]]` | Required in `switch` cases that intentionally fall through |
| `constexpr if` | Use for compile-time dispatch in generic code |
| Class template argument deduction (CTAD) | Permitted for standard library types (`std::vector v{1, 2, 3}`) |
| Inline variables | Use for header-defined constants: `inline constexpr double PI = 3.14159265358979323846;` |
| Nested namespaces | Use the `namespace A::B::C { }` syntax |
| `std::clamp` | Use for value clamping instead of manual min/max chains |
| `std::invoke` / `std::apply` | Permitted for generic callable invocation |

### 7.2 Discouraged Features

The following features should be avoided. Use requires documented justification:

| Feature | Reason | Alternative |
|---|---|---|
| `std::any` | Type-unsafe at the point of use; errors detected only at runtime | `std::variant` with a defined type set |
| `goto` | Reduces code analyzability; complicates control flow analysis | Structured control flow, RAII cleanup |
| Register variables (`register`) | Deprecated in C++17; no effect on modern compilers | Omit; trust compiler optimization |

### 7.3 Prohibited Features

The following are prohibited in SynthFlight application code. Violation is a blocking finding:

| Feature | Reason | Alternative |
|---|---|---|
| `malloc` / `free` | Bypasses C++ object lifetime management; no constructor/destructor calls | `std::make_unique`, `std::make_shared`, container classes |
| C-style casts `(int)x` | Unsafe; can perform `reinterpret_cast` silently | `static_cast`, `dynamic_cast`, `const_cast` (with justification) |
| Raw `new` / `delete` | Manual memory management is error-prone; violates RAII | `std::make_unique`, `std::make_shared` |
| `reinterpret_cast` | Unsafe; circumvents the type system | Redesign the interface; if unavoidable, document and isolate |
| `const_cast` (to remove const) | Violates const contract; undefined behavior if the original object is const | Redesign to avoid; if unavoidable, document safety argument |
| `#define` for constants | Not type-safe, not scoped, problematic in debugging | `constexpr` variables |
| `volatile` (for synchronization) | Does not provide atomicity or memory ordering guarantees | `std::atomic`, `std::mutex` |
| Exceptions from destructors | Can cause `std::terminate` during stack unwinding | Log errors in destructors; do not throw |
| `setjmp` / `longjmp` | Bypasses C++ stack unwinding; undefined behavior with destructors | Exceptions or return codes |
| C variable-length arrays (VLA) | Not standard C++; stack overflow risk | `std::vector` or `std::array` |

---

## 8. Error Handling

### 8.1 Exception Usage

| Rule ID | Rule |
|---|---|
| ERR-01 | Exceptions shall be used for exceptional conditions that indicate programming errors or violated preconditions |
| ERR-02 | Exception types shall derive from standard library exception classes |
| ERR-03 | Preferred exception types, following the existing JSBSim pattern: |

| Exception Type | Usage |
|---|---|
| `std::range_error` | Value outside valid range (e.g., altitude below model minimum) |
| `std::domain_error` | Input outside the mathematical domain (e.g., negative value for sqrt) |
| `std::invalid_argument` | Invalid parameter value or type |
| `std::runtime_error` | General runtime failure |
| `std::logic_error` | Internal logic error (invariant violation) |

| Rule ID | Rule |
|---|---|
| ERR-04 | Exception messages shall include: (1) the function or class name, (2) the condition that was violated, and (3) the actual value that caused the error where applicable |

```cpp
// Correct
if (altitude < MIN_ALTITUDE_M || altitude > MAX_ALTITUDE_M) {
  throw std::range_error(
      "FGStandardAtmosphere1976::GetTemperature: altitude " +
      std::to_string(altitude) + " m is outside valid range [" +
      std::to_string(MIN_ALTITUDE_M) + ", " +
      std::to_string(MAX_ALTITUDE_M) + "]");
}
```

### 8.2 Noexcept

| Rule ID | Rule |
|---|---|
| ERR-05 | `noexcept` shall be applied only where the function is provably non-throwing and performance benefit is measured or the function is a move constructor/assignment, swap, or destructor |
| ERR-06 | Exception specifications other than `noexcept` (e.g., `throw(...)`) are prohibited (removed in C++17) |
| ERR-07 | Destructors are implicitly `noexcept`; this shall not be overridden |

### 8.3 RAII

| Rule ID | Rule |
|---|---|
| ERR-08 | All resources shall be managed using RAII wrappers; cleanup code shall not depend on explicit `try`/`catch` blocks |
| ERR-09 | Scope guards (`std::unique_ptr` with custom deleter, or a local RAII class) shall be used for resources that do not fit standard smart pointers (e.g., C library handles) |

### 8.4 Assertions

| Rule ID | Rule |
|---|---|
| ERR-10 | `assert()` may be used for documenting invariants during development; these are disabled in Release builds via `-DNDEBUG` |
| ERR-11 | `assert()` shall not be used for input validation or runtime error checking; use exceptions or return codes for conditions that can occur in production |
| ERR-12 | Critical invariants that must be checked in Release builds shall use a project-defined `SYNTHFLIGHT_VERIFY(condition)` macro that throws on failure |

---

## 9. Documentation

### 9.1 Function Documentation

| Rule ID | Rule |
|---|---|
| DOC-01 | Every public and protected function shall have a Doxygen comment block |
| DOC-02 | The Doxygen comment shall include at minimum: |

```cpp
/// @brief Compute atmospheric temperature at a given geometric altitude.
///
/// Uses the ICAO Standard Atmosphere (1976) model to compute temperature.
/// Valid for altitudes from 0 m to 86,000 m geometric.
///
/// @param geometricAltitude_m Altitude above mean sea level in meters.
///        Valid range: [0.0, 86000.0]
/// @return Temperature in Kelvin.
/// @throws std::range_error If geometricAltitude_m is outside [0.0, 86000.0].
///
/// @requirement REQ-SF-0201
/// @see GetPressure(), GetDensity()
double GetTemperature(double geometricAltitude_m) const;
```

| Rule ID | Rule |
|---|---|
| DOC-03 | `@brief` -- One-line summary of the function's purpose |
| DOC-04 | `@param` -- For each parameter: name, description, valid range, units |
| DOC-05 | `@return` -- Description of return value, units |
| DOC-06 | `@throws` -- Each exception type and condition that triggers it |
| DOC-07 | Private functions should have Doxygen comments; they are mandatory if the function implements a non-trivial algorithm |

### 9.2 Class Documentation

| Rule ID | Rule |
|---|---|
| DOC-08 | Every class shall have a Doxygen comment block with `@brief` and design notes |
| DOC-09 | The class comment shall document the thread safety classification (per SDS-SF-2026-001 Section 5.3) |
| DOC-10 | The class comment shall reference the applicable requirements using `@requirement` |

```cpp
/// @brief ICAO Standard Atmosphere (1976) model implementation.
///
/// Computes atmospheric properties (temperature, pressure, density) as a
/// function of geometric altitude per ICAO Doc 7488/3 and US Standard
/// Atmosphere 1976.
///
/// Thread safety: Thread-Compatible (immutable after construction)
///
/// @requirement REQ-SF-0200, REQ-SF-0201, REQ-SF-0202, REQ-SF-0203
/// @see FGAtmosphereBase
class FGStandardAtmosphere1976 : public FGAtmosphereBase {
  // ...
};
```

### 9.3 Requirement Traceability in Code

| Rule ID | Rule |
|---|---|
| DOC-11 | Every function or class that implements a requirement shall include `/// @requirement REQ-SF-XXXX` in its Doxygen comment |
| DOC-12 | Every test file shall include a comment block at the top listing all requirements verified by the tests in that file |

```cpp
/// @file test_FGStandardAtmosphere1976_gtest.cpp
/// @brief Unit tests for FGStandardAtmosphere1976.
///
/// Requirements verified:
///   - REQ-SF-0200: Atmosphere model initialization
///   - REQ-SF-0201: Temperature computation
///   - REQ-SF-0202: Pressure computation
///   - REQ-SF-0203: Density computation
///   - REQ-SF-0204: Boundary condition handling
```

### 9.4 Inline Comments

| Rule ID | Rule |
|---|---|
| DOC-13 | Inline comments shall explain *why*, not *what*; the code itself should be clear enough to explain *what* |
| DOC-14 | Complex mathematical expressions shall include a comment referencing the source equation (e.g., "// ICAO Doc 7488/3, Eq. 2.11") |
| DOC-15 | TODO comments shall include the author's identifier and a tracking reference: `// TODO(jdoe): REQ-SF-0205 - implement wind model` |

---

## 10. Testing-Related Code Rules

### 10.1 Test Naming

| Rule ID | Rule |
|---|---|
| TEST-01 | Google Test fixtures shall be named `Module_Function`: `TEST_F(FGColumnVector3_Magnitude, ...)` |
| TEST-02 | Test case names shall follow the pattern `ConditionUnderTest_ExpectedBehavior` |

```cpp
// Correct naming
TEST_F(FGColumnVector3_Magnitude, UnitVector_ReturnsMagnitudeOne) {
  // ...
}

TEST_F(FGColumnVector3_Magnitude, ZeroVector_ReturnsMagnitudeZero) {
  // ...
}

TEST_F(FGStandardAtmosphere1976_Temperature,
       Alt 11000m_ReturnsStratopauseTemperature) {
  // ...
}
```

### 10.2 Magic Numbers

| Rule ID | Rule |
|---|---|
| TEST-03 | Magic numbers are prohibited in test code; use named constants with descriptive names |
| TEST-04 | Physical constants used in tests shall be defined in a shared test constants header with source references |

```cpp
// Incorrect
EXPECT_NEAR(result, 216.65, 0.01);

// Correct
constexpr double TROPOPAUSE_TEMPERATURE_K = 216.65;  // ICAO Doc 7488/3, Table 1
constexpr double TEMPERATURE_TOLERANCE_K = 0.01;     // 0.005% at tropopause
EXPECT_NEAR(result, TROPOPAUSE_TEMPERATURE_K, TEMPERATURE_TOLERANCE_K);
```

### 10.3 Floating-Point Assertions

| Rule ID | Rule |
|---|---|
| TEST-05 | Floating-point comparisons shall use `EXPECT_NEAR(actual, expected, tolerance)` or `ASSERT_NEAR(actual, expected, tolerance)` |
| TEST-06 | Exact floating-point equality (`EXPECT_EQ`, `EXPECT_DOUBLE_EQ`) is prohibited for computed results; it may be used only for comparing values that were assigned, not computed |
| TEST-07 | Every tolerance value shall be accompanied by a comment documenting its rationale |

```cpp
// Correct: tolerance documented
constexpr double PRESSURE_TOLERANCE_PA = 0.5;  // 0.001% relative at MSL
EXPECT_NEAR(pressure, SEA_LEVEL_PRESSURE_PA, PRESSURE_TOLERANCE_PA);

// Incorrect: undocumented tolerance
EXPECT_NEAR(pressure, 101325.0, 0.5);
```

### 10.4 Test Isolation

| Rule ID | Rule |
|---|---|
| TEST-08 | Each test case shall contain one logical assertion (related `EXPECT_*` calls for the same property, e.g., checking x, y, z components of a vector, count as one logical assertion) |
| TEST-09 | Tests shall be independent: no test shall depend on the execution or result of another test |
| TEST-10 | Tests shall not depend on file system state, network, or other external resources unless explicitly testing those interfaces (in which case, mock/fake objects shall be used) |
| TEST-11 | Shared test setup shall be placed in the fixture's `SetUp()` method, not in individual tests |

### 10.5 Test File Traceability

| Rule ID | Rule |
|---|---|
| TEST-12 | Every test file shall begin with a comment block listing all requirements (REQ-SF-XXXX) verified by the tests in that file (see Section 9.3) |
| TEST-13 | Each test case should reference the specific requirement it verifies using a comment: `// Verifies REQ-SF-0042` |
| TEST-14 | The requirements traceability matrix shall include test file names and test case names as verification evidence |

---

## 11. Prohibited Practices

The following practices are prohibited in SynthFlight code. These are critical for DAL A certification, and any violation is a blocking finding that shall be corrected before code acceptance.

### 11.1 Dynamic Memory in Real-Time Paths

| Rule ID | Rule |
|---|---|
| PROH-01 | Dynamic memory allocation (`new`, `malloc`, container resize, `push_back` that may reallocate) is prohibited after initialization in real-time simulation paths |
| PROH-02 | Real-time paths shall be identified and documented in the SDD; they include the main simulation loop and all functions called within a simulation time step |
| PROH-03 | Containers used in real-time paths shall be pre-allocated during initialization with `reserve()` and shall not exceed their reserved capacity |

### 11.2 Unbounded Recursion

| Rule ID | Rule |
|---|---|
| PROH-04 | Recursion is prohibited unless a bounded depth proof is provided and documented in the SDD |
| PROH-05 | The bounded depth proof shall specify the maximum recursion depth and the invariant that guarantees termination |
| PROH-06 | Iterative alternatives shall be preferred over recursive solutions |

### 11.3 Macros for Constants

| Rule ID | Rule |
|---|---|
| PROH-07 | `#define` macros shall not be used to define constants; use `constexpr` variables |
| PROH-08 | `#define` macros shall not be used to define inline functions; use `constexpr` or `inline` functions |
| PROH-09 | Permitted macro uses: include guards (if not using `#pragma once`), conditional compilation for platform abstraction, and the `SYNTHFLIGHT_VERIFY` macro |

### 11.4 Implicit Precision Loss

| Rule ID | Rule |
|---|---|
| PROH-10 | Implicit type conversions that lose precision are prohibited (e.g., `double` to `float`, `long` to `int` without explicit cast) |
| PROH-11 | All narrowing conversions shall use `static_cast` with a comment justifying that precision loss is acceptable |
| PROH-12 | Mixed-type arithmetic (`int` + `double`) shall use explicit casts to document intent |

### 11.5 Dead Code

| Rule ID | Rule |
|---|---|
| PROH-13 | Unreachable code is a DAL A finding and shall be removed |
| PROH-14 | Commented-out code is prohibited in production branches; use version control for historical code |
| PROH-15 | Unused functions, variables, and `#include` directives shall be removed |
| PROH-16 | `switch` statements shall include a `default` case that either handles the condition or explicitly documents why it is unreachable (e.g., `default: SYNTHFLIGHT_VERIFY(false && "unreachable");`) |

### 11.6 Compiler Warnings

| Rule ID | Rule |
|---|---|
| PROH-17 | All compiler warnings are treated as errors via `-Werror`; no code shall be committed that produces warnings |
| PROH-18 | Warning suppression pragmas (`#pragma GCC diagnostic ignored`) are prohibited in application code; they may be used only in COTS/reuse headers (see Section 12) |

---

## 12. COTS/Reuse Code

### 12.1 Scope

This section applies to JSBSim-derived code reused in SynthFlight, specifically:

- `FGColumnVector3` (vector mathematics)
- `FGMatrix33` (matrix mathematics)
- `FGAtmosphereBase` (atmosphere model base class)
- `FGStandardAtmosphere1976` (standard atmosphere implementation)

And any additional JSBSim modules integrated in the future.

### 12.2 Permitted Deviations

JSBSim-derived code may deviate from Sections 4 (Naming Conventions), 5 (File Organization), and 6 (Code Formatting) of this standard where modifying the code to conform would:

- Introduce defects by diverging from the well-tested upstream codebase
- Make future merges from upstream JSBSim difficult or impossible
- Require extensive retesting that outweighs the benefit of conformance

### 12.3 Deviation Documentation

| Rule ID | Rule |
|---|---|
| COTS-01 | Every deviation from this coding standard in COTS/reuse code shall be documented in the module's SDD section (per SDS-SF-2026-001 Section 6.1.1.10) |
| COTS-02 | The deviation document shall include: (1) the specific SCS rule being deviated from, (2) the reason for the deviation, (3) the risk assessment, and (4) the mitigation strategy |
| COTS-03 | Deviations shall be reviewed and approved during design review |

### 12.4 Modification Tracking

| Rule ID | Rule |
|---|---|
| COTS-04 | Any modification to JSBSim-derived code shall be marked with a comment block identifying the change, author, date, and rationale |
| COTS-05 | Modifications shall be tracked in a delta document or version control annotation to enable comparison with the upstream JSBSim version |
| COTS-06 | The baseline JSBSim version (JSBSim 6-DOF, namespace `JSBSim::`) shall be recorded in the project's configuration management plan |

### 12.5 Reuse Code Quality Requirements

| Rule ID | Rule |
|---|---|
| COTS-07 | COTS/reuse code shall comply with Sections 7 (C++17 Feature Usage), 8 (Error Handling), and 11 (Prohibited Practices) without exception |
| COTS-08 | Warning suppression for COTS code is permitted only in wrapper headers that include the COTS headers; the suppression shall be scoped as narrowly as possible |

```cpp
// Permitted: scoped warning suppression for COTS header
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <JSBSim/math/FGColumnVector3.h>
#pragma GCC diagnostic pop
```

---

## 13. Static Analysis

### 13.1 Required Analysis Tools

| Tool | Usage | Integration |
|---|---|---|
| **CodeQL** | Semantic analysis for security and correctness defects | GitHub Actions CI pipeline; runs on every pull request |
| **GCC warnings** | Compiler-based static analysis via warning flags (Section 3) | Build system; every compilation |

### 13.2 MISRA Compliance

| Rule ID | Rule |
|---|---|
| SA-01 | Additional MISRA C++:2023 compliance checkers are planned (TBD); the specific tool and rule set shall be defined before SRS baseline |
| SA-02 | When MISRA checkers are integrated, findings shall be dispositioned as: (1) Fix, (2) Deviate with justification, or (3) Not applicable with rationale |
| SA-03 | Deviations from MISRA rules shall be documented in a MISRA Deviation Record and approved by the project's safety engineer |

### 13.3 Analysis Findings

| Rule ID | Rule |
|---|---|
| SA-04 | All static analysis findings of severity "Error" or "Warning" shall be resolved before code merge |
| SA-05 | Findings of severity "Note" or "Information" shall be reviewed; resolution is recommended but not mandatory |
| SA-06 | False positives shall be suppressed with an inline annotation that references the finding ID and justification |
| SA-07 | Static analysis results shall be archived as part of the DO-178C verification evidence |

### 13.4 CI Pipeline Integration

```yaml
# Example GitHub Actions integration (conceptual)
static-analysis:
  steps:
    - name: CodeQL Analysis
      uses: github/codeql-action/analyze@v2
      with:
        languages: cpp
    - name: Build with all warnings
      run: |
        cmake -B build -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -Wpedantic"
        cmake --build build
```

---

## 14. Code Review Checklist

The following checklist shall be used during code reviews to verify compliance with this standard. Each item shall be assessed as Pass, Fail, or Not Applicable.

### 14.1 Language and Compiler

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-01 | Code compiles without warnings with mandatory flags (Section 3.2) | |
| CR-02 | No compiler-specific extensions used outside platform abstraction | |
| CR-03 | C++17 features used correctly per Section 7 | |
| CR-04 | No prohibited features (Section 7.3) are used | |

### 14.2 Naming and Organization

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-05 | Class, function, variable names follow Section 4 conventions | |
| CR-06 | File names match class names per Section 4.4 | |
| CR-07 | Include order follows Section 5.2 | |
| CR-08 | Header guards are present (Section 5.1) | |
| CR-09 | One class per header file (Section 5.3) | |

### 14.3 Formatting

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-10 | 2-space indentation, no tabs (Section 6.1) | |
| CR-11 | K&R brace style (Section 6.2) | |
| CR-12 | Line length within 100?characters (Section 6.3) | |
| CR-13 | Pointer/reference style: `Type* ptr` (Section 6.4) | |

### 14.4 Error Handling and Safety

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-14 | Exceptions derive from standard exception classes (Section 8.1) | |
| CR-15 | RAII used for all resource management (Section 8.3) | |
| CR-16 | No raw `new`/`delete` in application code | |
| CR-17 | No `malloc`/`free` | |
| CR-18 | No C-style casts | |
| CR-19 | No dynamic allocation in real-time paths (Section 11.1) | |
| CR-20 | No unbounded recursion (Section 11.2) | |
| CR-21 | No dead code (Section 11.5) | |
| CR-22 | No implicit precision-losing conversions (Section 11.4) | |

### 14.5 Documentation

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-23 | All public functions have Doxygen comments with `@brief`, `@param`, `@return`, `@throws` | |
| CR-24 | Class-level Doxygen comment with `@brief`, thread safety, and requirements | |
| CR-25 | Requirement traceability (`@requirement REQ-SF-XXXX`) present | |
| CR-26 | Inline comments explain *why*, not *what* | |
| CR-27 | No TODO comments without tracking reference | |

### 14.6 Testing

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-28 | Test names follow `TEST_F(Module_Function, Condition_Expected)` pattern | |
| CR-29 | No magic numbers in tests | |
| CR-30 | Floating-point assertions use `EXPECT_NEAR` with documented tolerance | |
| CR-31 | One logical assertion per test case | |
| CR-32 | Test file has requirements traceability comment block | |

### 14.7 COTS/Reuse

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| CR-33 | JSBSim-derived code deviations are documented per Section 12 | |
| CR-34 | Modifications to COTS code are marked with change comments | |
| CR-35 | Warning suppressions for COTS headers are scoped (Section 12.5) | |

### 14.8 Review Disposition

Each code review shall result in one of the following dispositions:

| Disposition | Action |
|---|---|
| **Approved** | Code meets all checklist criteria; merge is permitted |
| **Approved with Comments** | Code is acceptable with minor changes that do not affect functionality or safety; changes verified by original reviewer |
| **Changes Requested** | Code fails one or more checklist criteria; shall be revised and re-reviewed |
| **Rejected** | Code has fundamental issues (architecture, safety); requires significant rework |

---

## 15. References

| Reference | Title |
|---|---|
| ISO/IEC 14882:2017 | Programming Languages -- C++ (C++17) |
| DO-178C | Software Considerations in Airborne Systems and Equipment Certification (RTCA, 2011) |
| MISRA C++:2023 | Guidelines for the Use of the C++ Language in Critical Systems |
| SRS-SF-2026-001 | SynthFlight Software Requirements Specification |
| SRS-STD-SF-2026-001 | SynthFlight Software Requirements Standards |
| SDS-SF-2026-001 | SynthFlight Software Design Standards |
| Google C++ Style Guide | Google C++ Style Guide (reference, not normative) |
| JSBSim Reference Manual | JSBSim Open Source Flight Dynamics Model Documentation |
| GCC Manual | GNU Compiler Collection Documentation |

---

## 16. Revision History

| Version | Date | Author | Description |
|---|---|---|---|
| 1.0 | 2026-03-14 | SynthFlight Standards Team | Initial release |
