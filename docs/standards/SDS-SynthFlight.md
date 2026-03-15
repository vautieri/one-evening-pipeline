# Software Design Standards

**Document ID:** SDS-SF-2026-001
**Project:** SynthFlight -- UAV Mission Simulation and Verification Platform
**DAL:** A (Catastrophic) -- DO-178C
**Language:** C++17 (ISO/IEC 14882:2017)
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Draft

---

## Table of Contents

1. [Purpose](#1-purpose)
2. [Architecture Conventions](#2-architecture-conventions)
3. [Class Design Rules](#3-class-design-rules)
4. [Data Design](#4-data-design)
5. [Interface Design](#5-interface-design)
6. [Design Documentation](#6-design-documentation)
7. [Design Review Checklist](#7-design-review-checklist)
8. [References](#8-references)
9. [Revision History](#9-revision-history)

---

## 1. Purpose

This document defines the software design standards for the SynthFlight UAV Mission Simulation and Verification Platform. It establishes the conventions, rules, and quality criteria that govern the design of all SynthFlight software components.

These standards ensure that:

- Software architecture is consistent, maintainable, and analyzable for DO-178C DAL A certification
- Design artifacts are sufficient for independent verification
- Design decisions are traceable to requirements in SRS-SF-2026-001
- Integration of JSBSim 6-DOF physics engine components follows a controlled and documented process

### 1.1 Scope

This standard applies to:

- All new SynthFlight software modules and components
- Modifications to existing SynthFlight code
- Integration wrappers for JSBSim-derived modules (FGColumnVector3, FGMatrix33, FGAtmosphereBase, FGStandardAtmosphere1976)
- Test harness and verification framework design

This standard does not apply to:

- Unmodified JSBSim source code used as-is (governed by JSBSim's own conventions; deviations documented per Section 2.3)
- Third-party tools (CMake scripts, CI/CD configuration) unless they directly implement a design decision

### 1.2 Relationship to Other Standards

| Standard | Relationship |
|---|---|
| SRS-STD-SF-2026-001 | Requirements that drive design decisions |
| SCS-SF-2026-001 | Coding rules that implement these design conventions |
| DO-178C Section 6.3.2 | Design process objectives this standard supports |

---

## 2. Architecture Conventions

### 2.1 Layered Architecture

SynthFlight shall follow a layered architecture with strictly defined dependency rules. Each layer may depend only on layers below it. No circular dependencies between layers are permitted.

```
+-------------------------------------------------------+
|  Layer 5: GUI / Visualization                         |
|  (Qt/OpenGL interfaces, operator displays)            |
+-------------------------------------------------------+
        |
        v
+-------------------------------------------------------+
|  Layer 4: Application                                 |
|  (Mission management, scenario execution, logging)    |
+-------------------------------------------------------+
        |
        v
+-------------------------------------------------------+
|  Layer 3: Systems                                     |
|  (Sensor models, actuator models, communication,      |
|   flight controllers, navigation)                     |
+-------------------------------------------------------+
        |
        v
+-------------------------------------------------------+
|  Layer 2: Models                                      |
|  (Atmosphere, aerodynamics, propulsion, 6-DOF EOM,    |
|   FGAtmosphereBase, FGStandardAtmosphere1976)         |
+-------------------------------------------------------+
        |
        v
+-------------------------------------------------------+
|  Layer 1: Math                                        |
|  (FGColumnVector3, FGMatrix33, coordinate transforms,  |
|   numerical integrators, interpolation)               |
+-------------------------------------------------------+
```

#### 2.1.1 Layer Dependency Rules

| Rule ID | Rule |
|---|---|
| ARCH-01 | A module in Layer N shall only depend on modules in Layers 1 through N-1 |
| ARCH-02 | A module shall not depend on modules in the same layer unless they are in the same functional group |
| ARCH-03 | No module shall have a dependency on a module in a higher layer (no upward dependencies) |
| ARCH-04 | Cross-layer callbacks shall use the Observer pattern or dependency injection to avoid upward coupling |
| ARCH-05 | Layer boundaries shall be enforced by namespace and directory structure |

#### 2.1.2 Layer Directory Mapping

| Layer | Directory | Namespace |
|---|---|---|
| Layer 1: Math | `src/math/` | `SynthFlight::Math` |
| Layer 2: Models | `src/models/` | `SynthFlight::Models` |
| Layer 3: Systems | `src/systems/` | `SynthFlight::Systems` |
| Layer 4: Application | `src/app/` | `SynthFlight::App` |
| Layer 5: GUI | `src/gui/` | `SynthFlight::Gui` |

### 2.2 Namespace Usage

| Rule ID | Rule |
|---|---|
| NS-01 | All new SynthFlight code shall reside within the `SynthFlight::` namespace hierarchy |
| NS-02 | JSBSim-derived code that is reused without modification shall retain the `JSBSim::` namespace |
| NS-03 | JSBSim-derived code that is modified for SynthFlight shall be placed in `SynthFlight::` with a documented mapping to the original `JSBSim::` class |
| NS-04 | The global namespace shall not be used for any SynthFlight symbols |
| NS-05 | `using namespace` directives are prohibited in header files |
| NS-06 | `using namespace` directives in source files shall be limited to the innermost practical scope |

### 2.3 Module Naming

| Rule ID | Rule |
|---|---|
| MN-01 | JSBSim-derived classes shall retain the `FG` prefix to indicate heritage (e.g., `FGColumnVector3`, `FGMatrix33`) |
| MN-02 | New SynthFlight classes shall not use the `FG` prefix |
| MN-03 | Module names shall clearly indicate the module's responsibility (e.g., `WaypointManager`, `SensorModel`, `AtmosphereAdapter`) |
| MN-04 | Adapter or wrapper classes for JSBSim modules shall use the suffix `Adapter` (e.g., `AtmosphereAdapter` wrapping `FGStandardAtmosphere1976`) |

---

## 3. Class Design Rules

### 3.1 Single Responsibility Principle

| Rule ID | Rule |
|---|---|
| CLS-01 | Each class shall have a single, well-defined responsibility documented in its class-level Doxygen comment |
| CLS-02 | If a class description requires the word "and" to describe its responsibility, the class is a candidate for decomposition |
| CLS-03 | Utility classes (e.g., unit conversion helpers) are permitted but shall be stateless |

### 3.2 Composition Over Inheritance

| Rule ID | Rule |
|---|---|
| CLS-04 | Prefer composition over inheritance for code reuse |
| CLS-05 | Inheritance shall be used only to model genuine "is-a" relationships |
| CLS-06 | Inheritance hierarchies shall not exceed three levels of depth (base + two derived) unless justified in the SDD |
| CLS-07 | Multiple inheritance is prohibited except for interface (pure virtual) mix-ins |
| CLS-08 | Diamond inheritance is prohibited |

### 3.3 Interface Design for Pluggable Components

| Rule ID | Rule |
|---|---|
| CLS-09 | Pluggable components (flight controllers, sensor models, actuator models, atmosphere models) shall define a pure virtual interface class |
| CLS-10 | Interface classes shall contain only pure virtual methods and a virtual destructor |
| CLS-11 | Concrete implementations shall inherit from exactly one interface class (plus any pure interface mix-ins) |
| CLS-12 | Selection of concrete implementation shall be performed via factory or dependency injection, not compile-time conditional compilation |

Example interface pattern:

```cpp
namespace SynthFlight::Systems {

/// @brief Interface for all flight controller implementations.
class IFlightController {
public:
  virtual ~IFlightController() = default;

  /// @brief Compute control outputs for the current state.
  /// @param state Current aircraft state vector.
  /// @param dt Time step in seconds.
  /// @return Control surface deflections and throttle commands.
  virtual ControlOutput ComputeControl(const AircraftState& state, double dt) = 0;

  /// @brief Reset controller to initial state.
  virtual void Reset() = 0;
};

} // namespace SynthFlight::Systems
```

### 3.4 Resource Management (RAII)

| Rule ID | Rule |
|---|---|
| CLS-13 | All resources (memory, file handles, sockets, mutex locks) shall be managed using RAII |
| CLS-14 | Raw `new` and `delete` are prohibited in application code; use `std::make_unique` or `std::make_shared` |
| CLS-15 | `std::unique_ptr` is the default smart pointer; `std::shared_ptr` shall be used only when shared ownership is genuinely required and documented |
| CLS-16 | Classes managing resources shall either implement the Rule of Five or explicitly delete copy/move operations |
| CLS-17 | Resource acquisition during object construction shall be exception-safe (strong exception guarantee preferred) |

### 3.5 Const Correctness

| Rule ID | Rule |
|---|---|
| CLS-18 | Member functions that do not modify object state shall be declared `const` |
| CLS-19 | Function parameters that are not modified shall be passed by `const` reference (for non-trivial types) or by value (for trivial types) |
| CLS-20 | Return values that should not be modified by the caller shall be returned as `const` reference where object lifetime permits |
| CLS-21 | Use of `mutable` shall be limited to implementation-detail caching and shall be documented |

---

## 4. Data Design

### 4.1 Physical Units

| Rule ID | Rule |
|---|---|
| DAT-01 | All internal representations of physical quantities shall use SI units unless explicitly exempted |
| DAT-02 | SI base units for SynthFlight: meters (length), kilograms (mass), seconds (time), Kelvin (temperature), radians (angle) |
| DAT-03 | JSBSim-derived modules (FGColumnVector3, FGMatrix33, FGAtmosphereBase, FGStandardAtmosphere1976) may retain imperial units internally where changing units would introduce defects; such deviations shall be documented in the module's SDD section |
| DAT-04 | All unit conversions shall be performed through dedicated conversion functions, not inline magic numbers |
| DAT-05 | Conversion functions shall be implemented as `constexpr` functions in a `UnitConversion` utility class |
| DAT-06 | Variable and parameter names should encode units where ambiguity is possible (e.g., `altitude_m`, `pressure_Pa`, `temperature_K`) |

### 4.2 Coordinate Systems

| Rule ID | Rule |
|---|---|
| DAT-07 | Every module's SDD section shall document which coordinate system(s) it uses |
| DAT-08 | Coordinate system conventions shall be documented using the following standard identifiers: |

| Identifier | Coordinate System | Convention |
|---|---|---|
| `NED` | North-East-Down | Local tangent plane; X=North, Y=East, Z=Down |
| `ECEF` | Earth-Centered Earth-Fixed | X through prime meridian/equator, Z through North Pole |
| `BODY` | Body-Fixed | X=Forward, Y=Starboard, Z=Down (aircraft convention) |
| `WIND` | Wind Axes | X=Along relative wind, Z=Down in stability plane |
| `STAB` | Stability Axes | Body axes rotated by angle of attack |
| `LLA` | Geodetic | Latitude, Longitude, Altitude (WGS-84) |

| Rule ID | Rule |
|---|---|
| DAT-09 | Coordinate transformations shall be implemented as named functions, not inline matrix operations |
| DAT-10 | Every coordinate transformation function shall document the source and target coordinate systems in its Doxygen comment |
| DAT-11 | Transformation matrices shall be orthogonal (for rotation-only transforms); this property shall be verified by unit tests |

### 4.3 Floating-Point Arithmetic

| Rule ID | Rule |
|---|---|
| DAT-12 | All physics calculations shall use `double` (IEEE 754 double-precision, 64-bit); `float` is prohibited for physics computations |
| DAT-13 | `float` may be used only for GPU rendering data or external interface compliance where `double` is not supported; such usage shall be documented |
| DAT-14 | Floating-point equality comparisons (`==`, `!=`) are prohibited for physics values; use tolerance-based comparison functions |
| DAT-15 | Tolerance values for comparisons shall be documented with their rationale (e.g., based on measurement precision, algorithm stability analysis) |
| DAT-16 | Division operations shall include a guard against division by zero; the guard behavior (exception, clamp, default value) shall be documented per module |
| DAT-17 | `NaN` and `Inf` values shall be treated as error conditions; modules shall validate outputs and report or handle these values explicitly |

### 4.4 Data Structures

| Rule ID | Rule |
|---|---|
| DAT-18 | Standard library containers (`std::vector`, `std::array`, `std::map`, `std::unordered_map`) shall be used in preference to custom containers |
| DAT-19 | `std::array` shall be used for fixed-size collections where the size is known at compile time |
| DAT-20 | Container sizes for real-time paths shall be pre-allocated during initialization; dynamic resizing during real-time execution is prohibited |
| DAT-21 | Serialization formats (for data recording, configuration) shall be documented in the SDD |

---

## 5. Interface Design

### 5.1 Public API Documentation

| Rule ID | Rule |
|---|---|
| INT-01 | Every public class, method, and function shall have a Doxygen comment block |
| INT-02 | Doxygen comments shall include at minimum: `@brief`, `@param` (for each parameter), `@return`, and `@throws` (if exceptions are possible) |
| INT-03 | API documentation shall specify preconditions and postconditions where applicable |
| INT-04 | API documentation shall specify the valid range of each parameter |
| INT-05 | Breaking API changes shall be documented in the module's SDD revision history and communicated to all dependent module owners |

### 5.2 Error Handling Strategy

SynthFlight uses a dual error handling strategy:

#### 5.2.1 Exceptions for Programming Errors

| Rule ID | Rule |
|---|---|
| INT-06 | Exceptions shall be used for conditions that indicate programming errors or contract violations (e.g., out-of-range arguments, null pointer dereference attempts) |
| INT-07 | Exception types shall derive from standard library exception classes: `std::runtime_error`, `std::domain_error`, `std::range_error`, `std::invalid_argument` |
| INT-08 | Exception messages shall include: the function name, the violated condition, and the actual value that caused the violation |
| INT-09 | Custom exception classes are permitted when additional context (e.g., module ID, requirement ID) is needed; they shall derive from a standard exception class |

#### 5.2.2 Return Codes for Runtime Conditions

| Rule ID | Rule |
|---|---|
| INT-10 | Return codes or `std::optional` shall be used for expected runtime conditions (e.g., sensor data unavailable, configuration file not found) |
| INT-11 | `std::optional` is preferred over sentinel values (e.g., returning -1 for "not found") |
| INT-12 | Error enumerations shall be scoped enums (`enum class`) with descriptive names |
| INT-13 | Every function that returns an error indicator shall document all possible error conditions |

#### 5.2.3 Error Handling Boundaries

| Rule ID | Rule |
|---|---|
| INT-14 | Exception propagation shall not cross layer boundaries (see Section 2.1); each layer boundary shall catch, log, and either handle or translate exceptions |
| INT-15 | JSBSim-derived code exceptions shall be caught at the SynthFlight integration boundary and translated to SynthFlight exception types |

### 5.3 Thread Safety

| Rule ID | Rule |
|---|---|
| INT-16 | Every class shall document its thread safety guarantee using one of the following classifications: |

| Classification | Meaning |
|---|---|
| **Thread-Safe** | All public methods may be called concurrently from multiple threads without external synchronization |
| **Thread-Compatible** | Concurrent reads are safe; concurrent writes or mixed read/write require external synchronization |
| **Thread-Unsafe** | No concurrent access is safe; caller must ensure single-threaded access |

| Rule ID | Rule |
|---|---|
| INT-17 | Thread safety classification shall appear in the class-level Doxygen comment |
| INT-18 | Mutable shared state shall be protected by `std::mutex` or `std::shared_mutex`; lock-free alternatives shall be justified by performance analysis |
| INT-19 | Lock ordering shall be documented to prevent deadlocks; a global lock ordering document shall be maintained in the SDD |
| INT-20 | Simulation time-step execution shall be single-threaded within a given simulation instance; parallelism is permitted only for independent simulation instances or offline analysis |

---

## 6. Design Documentation

### 6.1 Software Design Description (SDD) Format

Each module shall have a corresponding section in the SynthFlight Software Design Description (SDD) document. The SDD section shall contain the following subsections:

#### 6.1.1 Required SDD Content Per Module

| Section | Content |
|---|---|
| **6.1.1.1 Module Overview** | Brief description of the module's purpose, its layer assignment (per Section 2.1), and its role in the overall architecture |
| **6.1.1.2 Requirements Traceability** | Table mapping module functionality to requirement IDs (REQ-SF-XXXX) from SRS-SF-2026-001 |
| **6.1.1.3 Class Diagram** | UML class diagram showing classes, inheritance, composition, and dependencies |
| **6.1.1.4 Data Dictionary** | Table of key data items: name, type, units, valid range, description |
| **6.1.1.5 Algorithm Description** | Mathematical formulation and algorithmic steps for each significant computation; references to source material (e.g., ICAO Doc 7488/3, MIL-STD-1797A) |
| **6.1.1.6 Coordinate Systems** | Coordinate systems used by this module (per Section 4.2) |
| **6.1.1.7 Interface Specification** | Public API with parameter descriptions, preconditions, postconditions, and error behavior |
| **6.1.1.8 Error Handling** | Module-specific error conditions and handling strategy |
| **6.1.1.9 Thread Safety** | Thread safety classification and synchronization approach |
| **6.1.1.10 COTS/Reuse Deviations** | Deviations from this standard for JSBSim-derived code, with justification |
| **6.1.1.11 Design Decisions** | Record of significant design decisions with rationale and alternatives considered |
| **6.1.1.12 Revision History** | Change log for the module's design section |

### 6.2 Design Diagrams

| Rule ID | Rule |
|---|---|
| DOC-01 | Architecture diagrams shall use UML 2.x notation |
| DOC-02 | Sequence diagrams shall be provided for all inter-module interactions involving more than two modules |
| DOC-03 | State machine diagrams shall be provided for all modules that exhibit stateful behavior (e.g., mission state machine, simulation state machine) |
| DOC-04 | Diagrams shall be version-controlled alongside the SDD document |
| DOC-05 | Diagrams shall be generated from text-based tools (e.g., PlantUML, Mermaid) where practical to support version control and diffing |

### 6.3 Design Constraints

| Rule ID | Rule |
|---|---|
| DOC-06 | The design shall support compilation with CMake >= 3.16 |
| DOC-07 | The design shall not depend on platform-specific features unless abstracted behind a platform abstraction layer |
| DOC-08 | The design shall support deterministic simulation replay (given identical inputs and configuration, outputs shall be bit-identical) |
| DOC-09 | The design shall separate configuration data from code; all tunable parameters shall be loadable from configuration files |

---

## 7. Design Review Checklist

The following checklist shall be applied during design reviews per DO-178C Section 6.3.4. Each item shall be assessed as Pass, Fail, or Not Applicable.

### 7.1 Architecture Compliance

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DA-01 | Module is assigned to the correct architectural layer | |
| DA-02 | Module dependencies comply with layer dependency rules (Section 2.1.1) | |
| DA-03 | No circular dependencies exist | |
| DA-04 | Namespace usage complies with Section 2.2 | |
| DA-05 | Module naming complies with Section 2.3 | |

### 7.2 Class Design Compliance

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DC-01 | Each class has a single, documented responsibility | |
| DC-02 | Inheritance is used only for genuine "is-a" relationships | |
| DC-03 | Inheritance depth does not exceed three levels | |
| DC-04 | Pluggable components use pure virtual interfaces | |
| DC-05 | Resource management uses RAII; no raw `new`/`delete` | |
| DC-06 | Const correctness is applied consistently | |
| DC-07 | Classes that manage resources implement Rule of Five or delete copy/move | |

### 7.3 Data Design Compliance

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DD-01 | Physical quantities use SI units (or documented exemption for JSBSim heritage) | |
| DD-02 | Coordinate systems are documented per module | |
| DD-03 | All physics calculations use `double`, not `float` | |
| DD-04 | Floating-point comparisons use tolerance-based functions | |
| DD-05 | Unit conversions use dedicated conversion functions | |
| DD-06 | Division by zero is guarded | |
| DD-07 | NaN/Inf detection is implemented for outputs | |

### 7.4 Interface Design Compliance

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DI-01 | All public APIs have complete Doxygen documentation | |
| DI-02 | Error handling follows the dual strategy (exceptions vs. return codes) | |
| DI-03 | Thread safety classification is documented for each class | |
| DI-04 | Exception propagation does not cross layer boundaries uncaught | |
| DI-05 | Preconditions and postconditions are documented | |

### 7.5 Design Documentation Compliance

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DM-01 | SDD section contains all required content per Section 6.1.1 | |
| DM-02 | Requirements traceability table is complete and bidirectional | |
| DM-03 | Class diagrams are current and match implementation | |
| DM-04 | Algorithm descriptions reference authoritative source material | |
| DM-05 | COTS/Reuse deviations are documented with justification | |
| DM-06 | Design decisions are recorded with rationale | |

### 7.6 Traceability

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| DT-01 | Every design element traces to at least one requirement in SRS-SF-2026-001 | |
| DT-02 | Every requirement allocated to this module has a corresponding design element | |
| DT-03 | Derived requirements are identified and traced per SRS-STD-SF-2026-001 Section 5 | |
| DT-04 | Design-to-code traceability is maintained | |

### 7.7 Review Disposition

Each design element reviewed shall receive one of the following dispositions:

| Disposition | Action |
|---|---|
| **Approved** | Design meets all checklist criteria; proceed to implementation |
| **Approved with Comment** | Design is acceptable with minor changes that do not affect architecture or interfaces |
| **Disapproved** | Design fails one or more checklist criteria; shall be revised and re-reviewed |
| **Deferred** | Design review is deferred pending resolution of an open action item |

---

## 8. References

| Reference | Title |
|---|---|
| DO-178C | Software Considerations in Airborne Systems and Equipment Certification (RTCA, 2011) |
| DO-330 | Software Tool Qualification Considerations (RTCA, 2011) |
| SRS-SF-2026-001 | SynthFlight Software Requirements Specification |
| SRS-STD-SF-2026-001 | SynthFlight Software Requirements Standards |
| SCS-SF-2026-001 | SynthFlight Software Code Standards |
| ISO/IEC 14882:2017 | Programming Languages -- C++ (C++17) |
| ICAO Doc 7488/3 | Manual of the ICAO Standard Atmosphere |
| JSBSim Reference Manual | JSBSim Open Source Flight Dynamics Model |

---

## 9. Revision History

| Version | Date | Author | Description |
|---|---|---|---|
| 1.0 | 2026-03-14 | SynthFlight Standards Team | Initial release |
