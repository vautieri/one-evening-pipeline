# Software Requirements Standards

**Document ID:** SRS-STD-SF-2026-001
**Project:** SynthFlight -- UAV Mission Simulation and Verification Platform
**DAL:** A (Catastrophic) -- DO-178C
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Baselined

---

## Table of Contents

1. [Purpose](#1-purpose)
2. [Requirement Identification](#2-requirement-identification)
3. [Requirement Attributes](#3-requirement-attributes)
4. [Writing Rules](#4-writing-rules)
5. [Derived Requirements](#5-derived-requirements)
6. [Requirements Review Checklist](#6-requirements-review-checklist)
7. [References](#7-references)
8. [Revision History](#8-revision-history)

---

## 1. Purpose

This document defines the rules, conventions, and quality criteria for writing software requirements for the SynthFlight UAV Mission Simulation and Verification Platform. It governs the authoring, structure, and review of all requirements contained in the Software Requirements Specification (SRS-SF-2026-001) and any subsequent SRS revisions.

This standard ensures that all SynthFlight requirements are:

- Unambiguous and testable
- Traceable to system-level objectives and DO-178C certification evidence
- Consistent in format and terminology across all 149 requirements (REQ-SF-0001 through REQ-SF-0149) and any future additions
- Compliant with DO-178C Section 6.3.1 objectives for DAL A software

All personnel authoring, reviewing, or approving SynthFlight requirements shall follow this standard.

### 1.1 Scope

This standard applies to:

- High-level requirements (HLR) in SRS-SF-2026-001
- Low-level requirements (LLR) derived during detailed design
- Derived requirements identified during design or implementation
- Interface requirements between SynthFlight modules and external systems

### 1.2 Applicable Documents

| Document ID | Title |
|---|---|
| SRS-SF-2026-001 | SynthFlight Software Requirements Specification |
| DO-178C | Software Considerations in Airborne Systems and Equipment Certification |
| DO-330 | Software Tool Qualification Considerations |
| DO-331 | Model-Based Development and Verification Supplement |

---

## 2. Requirement Identification

### 2.1 Numbering Scheme

All SynthFlight requirements shall use the identifier format:

```
REQ-SF-XXXX
```

Where:

- `REQ` -- Indicates a software requirement
- `SF` -- SynthFlight project identifier
- `XXXX` -- Four-digit sequential number, zero-padded

### 2.2 Category Ranges

Requirement numbers are allocated by functional category as defined in SRS-SF-2026-001 Section 2. The following ranges are reserved:

| Range | Category | Description |
|---|---|---|
| REQ-SF-0001 -- REQ-SF-0199 | Core Mathematics | Vector/matrix operations, coordinate transforms, numerical methods |
| REQ-SF-0200 -- REQ-SF-0399 | Atmospheric Models | Standard atmosphere, wind models, environmental conditions |
| REQ-SF-0400 -- REQ-SF-0599 | Flight Dynamics | 6-DOF equations of motion, aerodynamics, propulsion |
| REQ-SF-0600 -- REQ-SF-0799 | Navigation and Guidance | Waypoint management, path planning, GPS/INS models |
| REQ-SF-0800 -- REQ-SF-0999 | Systems and Sensors | Sensor models, actuator models, communication links |
| REQ-SF-1000 -- REQ-SF-1199 | Mission Management | Mission planning, execution, monitoring |
| REQ-SF-1200 -- REQ-SF-1399 | Verification and Validation | Built-in test, simulation harness, data recording |
| REQ-SF-1400 -- REQ-SF-1599 | Infrastructure | Configuration, logging, data persistence, IPC |
| REQ-SF-1600 -- REQ-SF-1799 | User Interface | GUI, visualization, operator controls |
| REQ-SF-1800 -- REQ-SF-1999 | Reserved | Future expansion |

### 2.3 Requirement Versioning

When a requirement is modified after baseline, the requirement ID shall remain unchanged. The revision shall be tracked in the requirement's revision history attribute. Deleted requirements shall be marked as "Deleted" with rationale; the ID shall not be reused.

---

## 3. Requirement Attributes

Every requirement in SRS-SF-2026-001 shall include the following attributes:

### 3.1 Mandatory Attributes

| Attribute | Description | Example |
|---|---|---|
| **Unique ID** | Identifier per Section 2.1 numbering scheme | `REQ-SF-0042` |
| **Text** | A single "shall" statement defining the requirement | "The system shall compute the atmospheric pressure..." |
| **Rationale** | Justification for why the requirement exists | "Required to model ISA conditions per ICAO Doc 7488/3" |
| **Source Domain(s)** | One or more of the defined source domains (see 3.2) | `DEF, SAR` |
| **Verification Method** | The method by which the requirement will be verified (see 3.3) | `Test` |
| **DAL Applicability** | The Design Assurance Level to which this requirement applies | `A` |
| **Traceability** | Parent requirement ID, or "System-Level" if top-level; for derived requirements, the parent requirement from which it was derived | `REQ-SF-0040` or `System-Level` |

### 3.2 Source Domains

Each requirement shall be tagged with one or more source domains indicating the operational context:

| Code | Domain | Description |
|---|---|---|
| `DEF` | Defense | Military UAV operations, tactical mission profiles |
| `COM` | Commercial | Commercial UAV operations, civil airspace integration |
| `GIS` | Geospatial | Mapping, surveying, terrain analysis missions |
| `INF` | Infrastructure | Infrastructure inspection, monitoring missions |
| `SAR` | Search and Rescue | SAR mission profiles, emergency response operations |

A requirement may apply to multiple domains. If a requirement applies to all domains, it shall be tagged with all five codes.

### 3.3 Verification Methods

Each requirement shall specify exactly one primary verification method:

| Method | Code | Description | When to Use |
|---|---|---|---|
| **Test** | T | Execution of software against defined test cases with measurable pass/fail criteria | Quantitative requirements with observable outputs |
| **Analysis** | A | Examination of design artifacts, mathematical proof, or simulation results | Performance bounds, algorithmic correctness, resource usage |
| **Inspection** | I | Visual examination of code, documentation, or configuration | Coding standards compliance, documentation completeness |
| **Demonstration** | D | Operation of the software to show capability without formal measurement | Operational scenarios, user interface behavior |

For DAL A requirements, verification method "Test" is preferred. Use of "Analysis," "Inspection," or "Demonstration" as the sole verification method for a DAL A requirement shall be justified in the verification plan.

### 3.4 Optional Attributes

The following attributes may be included when applicable:

| Attribute | Description |
|---|---|
| **Priority** | Implementation priority (P1 = Must Have, P2 = Should Have, P3 = Nice to Have) |
| **Stability** | Likelihood of change (Stable, Evolving, Volatile) |
| **Safety Impact** | Description of safety consequence if requirement is not met |
| **Interface** | Identifies external interface if applicable |
| **Allocation** | Target module or component |

---

## 4. Writing Rules

### 4.1 Mandatory Language

- The keyword **"shall"** indicates a mandatory requirement. Every requirement statement shall contain exactly one "shall."
- The keyword **"should"** indicates a recommendation. Statements using "should" are not testable requirements and shall be flagged for review. "Should" statements shall either be promoted to "shall" statements with quantified criteria or removed before SRS baseline.
- The keyword **"will"** describes facts or declarations of purpose (e.g., system context). "Will" statements are informational and are not requirements.

### 4.2 One Requirement Per Statement

Each requirement shall express a single testable obligation. Compound requirements (multiple "shall" clauses connected by "and," "or," "also," or semicolons) are prohibited.

**Non-compliant:**

> The system shall compute atmospheric temperature and shall compute atmospheric pressure at the given altitude.

**Compliant:**

> REQ-SF-0201: The system shall compute atmospheric temperature at the given geometric altitude.
>
> REQ-SF-0202: The system shall compute atmospheric pressure at the given geometric altitude.

### 4.3 Quantitative Specification

Requirements shall be quantitative wherever the nature of the requirement permits. Quantitative requirements shall include:

- **Tolerance:** Acceptable deviation from the nominal value
- **Units:** Measurement units (see Section 4.7)
- **Range:** Valid input and output ranges

**Non-compliant:**

> The system shall compute pressure accurately.

**Compliant:**

> The system shall compute atmospheric pressure with a maximum error of 0.1% relative to the ICAO Standard Atmosphere model over the altitude range 0 m to 86,000 m (geometric).

### 4.4 No Implementation-Specific Language

Requirements shall describe **what** the system must do, not **how** it does it. Implementation details (algorithms, data structures, specific function calls) belong in the Software Design Description (SDD), not the SRS.

**Non-compliant:**

> The system shall use a fourth-order Runge-Kutta integrator to solve the equations of motion.

**Compliant:**

> The system shall integrate the 6-DOF equations of motion with a maximum local truncation error of 1e-6 per time step.

### 4.5 No Ambiguous Terms

The following terms are considered ambiguous and shall not appear in requirement text:

| Prohibited Term | Replacement Guidance |
|---|---|
| "adequate" | Specify the quantitative threshold |
| "appropriate" | State the specific condition or value |
| "as applicable" | State the explicit conditions of applicability |
| "but not limited to" | Provide the complete list or reference a defined set |
| "easy" / "user-friendly" | Define measurable usability criteria |
| "efficient" | Specify performance bounds (time, memory, throughput) |
| "flexible" | Define the specific configurability or extensibility |
| "fast" / "rapid" | Specify time constraints in seconds or milliseconds |
| "minimize" / "maximize" | Specify the quantitative target and tolerance |
| "normal" | Define the specific operating conditions |
| "robust" | Define failure modes and required behavior |
| "significant" | Specify the threshold value |
| "sufficient" | Specify the quantitative threshold |
| "TBD" | Must be resolved before SRS baseline; open TBDs are tracked as action items |

### 4.6 Completeness

Every requirement shall be self-contained. If a requirement depends on definitions, constants, or conditions defined elsewhere, it shall include an explicit cross-reference (e.g., "as defined in REQ-SF-0040" or "per Table 3.2 of this document").

### 4.7 Units

- **Primary units:** SI (International System of Units) -- meters, kilograms, seconds, Kelvin, radians
- **Secondary units:** Imperial/aviation-conventional units may be used parenthetically where aviation convention dictates (e.g., feet, knots, nautical miles)
- When both unit systems appear, SI shall be stated first: "11,000 m (36,089 ft)"
- Angular measurements: Radians are the primary unit; degrees may appear parenthetically
- All unit abbreviations shall follow ISO 80000 conventions

### 4.8 Requirement Boundaries

Each requirement shall define behavior at boundary conditions where applicable:

- Minimum and maximum valid input values
- Behavior at zero, negative, and overflow conditions
- Behavior when optional inputs are absent

---

## 5. Derived Requirements

### 5.1 Definition

A derived requirement is a requirement that is not directly traceable to a higher-level requirement but arises from the design or implementation process. Derived requirements emerge from:

- Design decisions (e.g., choice of algorithm imposes precision constraints)
- Implementation constraints (e.g., compiler or platform limitations)
- Integration considerations (e.g., interface timing between modules)
- Safety analysis (e.g., additional checks identified by FHA/FMEA)

### 5.2 Identification

Derived requirements shall use the identifier format:

```
DR-SF-XXXX
```

Where:

- `DR` -- Indicates a derived requirement
- `SF` -- SynthFlight project identifier
- `XXXX` -- Four-digit sequential number, zero-padded, allocated from a separate sequence starting at DR-SF-0001

### 5.3 Justification Requirements

Every derived requirement shall include the following additional attributes beyond those in Section 3.1:

| Attribute | Description |
|---|---|
| **Derivation Source** | The design decision, analysis, or implementation constraint that generated this requirement |
| **Safety Impact Assessment** | Analysis of whether the derived requirement introduces a new failure condition or modifies an existing one |
| **Review Authority** | The name or role of the person who approved the derived requirement |
| **Date Identified** | The date the derived requirement was identified |

### 5.4 Safety Assessment

Per DO-178C Section 6.3.3, all derived requirements shall be evaluated for their impact on safety. The evaluation shall determine:

1. Whether the derived requirement could introduce new hazards
2. Whether the derived requirement could affect existing safety requirements
3. Whether the derived requirement needs to be fed back to the system safety assessment process

Derived requirements that affect safety shall be communicated to the system safety assessment process (per DO-178C objective 6.3.3.a) and documented in the safety assessment report.

### 5.5 Traceability

Derived requirements shall trace to:

- The design artifact or analysis that generated them (forward traceability)
- The verification evidence that confirms their implementation (verification traceability)

Derived requirements that cannot be justified shall be escalated to the Certification Liaison for disposition.

---

## 6. Requirements Review Checklist

The following checklist shall be applied during requirements review (per DO-178C Section 6.3.4). Each item shall be assessed as Pass, Fail, or Not Applicable for every requirement.

### 6.1 Completeness

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| C-01 | The requirement has all mandatory attributes defined in Section 3.1 | |
| C-02 | The requirement defines behavior for all valid input ranges | |
| C-03 | The requirement defines behavior at boundary conditions | |
| C-04 | The requirement defines error/exception behavior for invalid inputs | |
| C-05 | All referenced documents, tables, and cross-references exist | |
| C-06 | No TBD items remain in the requirement text | |
| C-07 | Units are specified for all quantitative values | |

### 6.2 Correctness

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| R-01 | The requirement accurately reflects the intended system behavior | |
| R-02 | Numerical values, tolerances, and ranges are technically correct | |
| R-03 | Referenced standards and source documents are correctly cited | |
| R-04 | The requirement does not contradict physics or mathematical constraints | |
| R-05 | The requirement is achievable within the system's design constraints | |

### 6.3 Consistency

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| S-01 | The requirement does not conflict with any other requirement in the SRS | |
| S-02 | Terminology is consistent with the SRS glossary | |
| S-03 | Units are consistent with Section 4.7 of this standard | |
| S-04 | The requirement ID follows the numbering scheme in Section 2 | |
| S-05 | Source domain tags are consistent with the requirement's scope | |

### 6.4 Verifiability

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| V-01 | The requirement contains a "shall" statement (not "should" or "will") | |
| V-02 | The requirement is testable -- a test case can be defined that objectively determines pass/fail | |
| V-03 | The verification method is appropriate for the requirement type | |
| V-04 | Quantitative criteria (tolerances, ranges, timing) are specified where applicable | |
| V-05 | The requirement does not use ambiguous terms listed in Section 4.5 | |
| V-06 | The requirement expresses a single obligation (no compound "shall" statements) | |

### 6.5 Traceability

| # | Check Item | Pass/Fail/NA |
|---|---|---|
| T-01 | The requirement traces to a parent requirement or is identified as system-level | |
| T-02 | Derived requirements include derivation source and safety impact assessment | |
| T-03 | The requirement is allocated to at least one module or component | |
| T-04 | Bidirectional traceability exists (parent to child and child to parent) | |

### 6.6 Review Disposition

Each requirement reviewed shall receive one of the following dispositions:

| Disposition | Action |
|---|---|
| **Approved** | Requirement meets all checklist criteria; no changes required |
| **Approved with Comment** | Requirement is acceptable with minor editorial changes that do not affect intent |
| **Disapproved** | Requirement fails one or more checklist criteria; shall be revised and re-reviewed |
| **Deferred** | Requirement review is deferred pending resolution of an open action item |

---

## 7. References

| Reference | Title |
|---|---|
| DO-178C | Software Considerations in Airborne Systems and Equipment Certification (RTCA, 2011) |
| DO-330 | Software Tool Qualification Considerations (RTCA, 2011) |
| DO-331 | Model-Based Development and Verification Supplement to DO-178C (RTCA, 2011) |
| IEEE 830-1998 | IEEE Recommended Practice for Software Requirements Specifications |
| ISO 80000 | Quantities and Units |
| ICAO Doc 7488/3 | Manual of the ICAO Standard Atmosphere |
| SRS-SF-2026-001 | SynthFlight Software Requirements Specification |

---

## 8. Revision History

| Version | Date | Author | Description |
|---|---|---|---|
| 1.0 | 2026-03-14 | SynthFlight Standards Team | Initial release |
| 1.1 | 2026-03-15 | SynthFlight Standards Team | Status changed to Baselined per Pre-Implementation Gate audit |
