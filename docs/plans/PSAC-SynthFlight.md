# Plan for Software Aspects of Certification (PSAC)

**Document ID:** PSAC-SF-2026-001
**Project:** SynthFlight — UAV Mission Simulation and Verification Platform
**Software Level:** A (Catastrophic)
**Applicable Standard:** RTCA DO-178C, *Software Considerations in Airborne Systems and Equipment Certification*
**Revision:** 1.0
**Date:** 2026-03-14
**Status:** Baselined

---

## Revision History

| Rev | Date       | Author         | Description             |
|-----|------------|----------------|-------------------------|
| 1.0 | 2026-03-14 | SynthFlight SE | Initial release (Draft) |
| 1.1 | 2026-03-15 | SynthFlight SE | Status changed to Baselined per Pre-Implementation Gate audit |

---

## Table of Contents

1. [System Overview](#1-system-overview)
2. [Software Overview](#2-software-overview)
3. [Certification Considerations](#3-certification-considerations)
4. [Software Life Cycle](#4-software-life-cycle)
5. [Software Life Cycle Data](#5-software-life-cycle-data)
6. [Schedule](#6-schedule)
7. [Software Life Cycle Environment](#7-software-life-cycle-environment)
8. [Software Development Processes](#8-software-development-processes)
9. [Software Verification Processes](#9-software-verification-processes)
10. [Software Configuration Management](#10-software-configuration-management)
11. [Software Quality Assurance](#11-software-quality-assurance)
12. [Certification Liaison](#12-certification-liaison)
13. [Compliance Matrix](#13-compliance-matrix)

---

## 1. System Overview

### 1.1 Purpose

This Plan for Software Aspects of Certification (PSAC) defines how the SynthFlight UAV Mission Simulation and Verification Platform will comply with the objectives of RTCA DO-178C, *Software Considerations in Airborne Systems and Equipment Certification*. This document is submitted to the certification authority as the primary means for conveying the software aspects of the certification basis and serves as the top-level roadmap for all software life cycle activities.

### 1.2 System Description

SynthFlight is a UAV Mission Simulation and Verification Platform designed to provide high-fidelity simulation, mission planning, and verification capabilities for unmanned aerial vehicle operations. The platform supports multiple industry domains:

- **Defense/Military** — Tactical mission rehearsal, threat assessment, and operational planning
- **Commercial Aviation** — Integration testing of UAV systems in controlled airspace
- **GIS/Survey** — Terrain mapping mission simulation and payload verification
- **Infrastructure** — Inspection route optimization and sensor coverage analysis
- **Search and Rescue (SAR)** — Coverage pattern simulation and time-critical mission planning

The system integrates a JSBSim 6-DOF physics engine (v1.2.4) for aerodynamic and flight dynamics modeling, the 1976 U.S. Standard Atmosphere (ISA) model for atmospheric conditions, and terrain data ingestion from DTED Level 0/1/2, LiDAR point clouds, and GeoTIFF DEM datasets. Visualization is provided through a Vulkan-based 3D rendering subsystem.

### 1.3 Operational Environment

SynthFlight operates on the following target platforms:

- **Windows 10+** (x86-64)
- **Ubuntu 22.04+** (x86-64)

The system interfaces with external UAV flight controllers, ground control stations, sensor payloads (simulated), and terrain databases. It does not operate as an embedded airborne system; rather, it serves as ground-based simulation and verification tooling whose outputs (mission plans, verified parameters, simulation results) are consumed by airborne-qualified systems.

### 1.4 Failure Condition Classification

The system-level Functional Hazard Assessment (FHA) has classified the most severe failure condition associated with SynthFlight as **Catastrophic**. This classification is based on the determination that erroneous simulation outputs — if used without detection to validate flight-critical mission parameters, flight envelopes, or obstacle clearance profiles — could lead to loss of the vehicle and potential casualties.

| Failure Condition                                  | Severity      | DAL |
|----------------------------------------------------|---------------|-----|
| Erroneous flight dynamics simulation output        | Catastrophic  | A   |
| Incorrect terrain/obstacle clearance computation   | Catastrophic  | A   |
| Undetected corruption of mission verification data | Catastrophic  | A   |
| Loss of simulation fidelity without annunciation   | Hazardous     | A   |
| Degraded rendering without misleading cues         | Major         | C   |

The assignment of **Design Assurance Level A** applies to all flight-critical software components of SynthFlight, as documented in the System Safety Assessment (SSA).

---

## 2. Software Overview

### 2.1 Software Architecture

SynthFlight is organized into the following Computer Software Configuration Items (CSCIs):

| CSCI ID       | CSCI Name                          | Description                                                                 | DAL |
|---------------|-------------------------------------|-----------------------------------------------------------------------------|-----|
| CSCI-SF-FDE   | Flight Dynamics Engine              | JSBSim 6-DOF integration, aerodynamic model execution, state propagation   | A   |
| CSCI-SF-ATM   | Atmosphere Model                    | 1976 U.S. Standard Atmosphere (ISA) computation, wind/turbulence models    | A   |
| CSCI-SF-TER   | Terrain Engine                      | DTED L0/L1/L2, LiDAR, and GeoTIFF DEM ingestion, elevation queries        | A   |
| CSCI-SF-NAV   | Navigation and Mission Planning     | Waypoint management, route computation, geofencing, mission sequencing     | A   |
| CSCI-SF-SEN   | Sensor Simulation                   | Payload modeling (EO/IR, SAR, LiDAR), FOV computation, detection models   | A   |
| CSCI-SF-VER   | Verification Engine                 | Automated mission verification, envelope checking, constraint validation   | A   |
| CSCI-SF-RND   | Rendering Subsystem                 | Vulkan-based 3D visualization, terrain rendering, HUD overlays             | C   |
| CSCI-SF-DIO   | Data I/O and Integration            | External interfaces, data import/export, GCS communication protocols       | A   |
| CSCI-SF-FWK   | Application Framework               | Core services, configuration management, logging, error handling           | A   |

### 2.2 Software Architecture Diagram

```
+------------------------------------------------------------------+
|                    SynthFlight Application Framework              |
|                         (CSCI-SF-FWK)                            |
+------------------------------------------------------------------+
|          |            |           |           |          |        |
v          v            v           v           v          v        v
+------+ +------+ +--------+ +------+ +------+ +------+ +------+
| FDE  | | ATM  | |  TER   | | NAV  | | SEN  | | VER  | | RND  |
|      |<->     |<->       |<->     |<->     |<->     | |      |
+------+ +------+ +--------+ +------+ +------+ +------+ +------+
                                                           |
                        +----------------------------------+
                        v
                  +----------+
                  |   DIO    |
                  | (Ext I/F)|
                  +----------+
```

### 2.3 Interfaces

- **CSCI-SF-FDE <-> CSCI-SF-ATM:** Atmospheric state vector (pressure, temperature, density, wind) at current altitude and position.
- **CSCI-SF-FDE <-> CSCI-SF-TER:** Ground elevation queries for ground-effect modeling and collision detection.
- **CSCI-SF-NAV <-> CSCI-SF-TER:** Terrain profile along planned routes for obstacle clearance analysis.
- **CSCI-SF-NAV <-> CSCI-SF-FDE:** Aircraft state for real-time mission progress tracking.
- **CSCI-SF-SEN <-> CSCI-SF-TER:** Terrain data for sensor footprint and line-of-sight computations.
- **CSCI-SF-VER <-> All CSCIs:** Read access to simulation state for verification constraint evaluation.
- **CSCI-SF-RND <-> CSCI-SF-TER, FDE, NAV:** Scene graph updates for 3D visualization.
- **CSCI-SF-DIO <-> External Systems:** GCS protocol interfaces, file import/export (KML, MAVLink, STANAG 4586).

### 2.4 Requirements Baseline

The software requirements are documented in **SRS-SF-2026-001** (Software Requirements Specification), which contains **149 unified requirements** organized across **19 categories**. Each requirement is traceable to system-level requirements and is assigned a verification method (test, analysis, inspection, or demonstration).

---

## 3. Certification Considerations

### 3.1 DAL Assignment Rationale

The Design Assurance Level A assignment derives from the system-level Functional Hazard Assessment (FHA) and Preliminary System Safety Assessment (PSSA). The failure condition "Erroneous flight dynamics simulation output used to validate flight-critical parameters" has been classified as Catastrophic per the criteria of 14 CFR 23.1309 / CS-23.1309 and ARP 4761. Since no architectural mitigation (e.g., dissimilar redundancy) has been applied to reduce the software level, the full DAL A rigor applies.

All CSCIs handling flight-critical data paths are assigned DAL A. The Rendering Subsystem (CSCI-SF-RND) is assigned DAL C based on the determination that rendering failures produce only misleading visual cues without affecting simulation accuracy, and the Verification Engine provides independent checking of simulation outputs.

### 3.2 Deactivated Code Policy

SynthFlight shall contain no deactivated code in the deliverable build. All compile-time feature toggles and preprocessor conditionals shall be resolved at build time, producing a single configuration per target platform. Any code paths that are not exercisable in the delivered configuration shall be removed prior to certification.

Debug and diagnostic code (assertions, logging at debug level, test harnesses) shall be excluded from the production build via the CMake build configuration. The absence of deactivated code shall be verified through structural coverage analysis and code review.

### 3.3 User-Modifiable Software

SynthFlight supports user-configurable mission parameters (waypoints, flight profiles, sensor configurations) and terrain database loading. These are treated as user-modifiable data, not user-modifiable software. The following protections apply:

- All user-supplied data shall be validated against defined schemas and range constraints prior to use by the simulation engine (see SRS-SF-2026-001, Category: Input Validation).
- User-supplied terrain data shall undergo integrity checks (checksum verification, format validation, resolution bounds) before ingestion.
- No user-modifiable code (scripts, plugins, dynamic loading) is permitted in the certified configuration.

### 3.4 COTS and Reuse Software Considerations

#### 3.4.1 JSBSim Flight Dynamics Library (v1.2.4)

JSBSim is an open-source 6-DOF flight dynamics model that constitutes Previously Developed Software (PDS). The following qualification strategy applies:

| Aspect                          | Approach                                                       |
|---------------------------------|----------------------------------------------------------------|
| Classification                  | Previously Developed Software (PDS) per DO-178C Section 12.3  |
| Service history                 | Extensive use in NASA, DoD, and academic research programs     |
| Gap analysis                    | Full gap analysis against DO-178C DAL A objectives required    |
| Requirements coverage           | JSBSim functionality mapped to SRS-SF-2026-001 requirements   |
| Verification                    | Full requirements-based testing of integrated JSBSim behavior  |
| Structural coverage             | MC/DC coverage analysis of JSBSim source code within scope     |
| Configuration management        | JSBSim v1.2.4 source code baselined under project SCM          |
| Problem reporting               | JSBSim defects tracked in project problem reporting system      |

A detailed COTS/Reuse Software Assessment Report shall be produced as a supplemental life cycle data item.

#### 3.4.2 Vulkan Graphics API

The Vulkan rendering pipeline is used exclusively by CSCI-SF-RND (DAL C). The Vulkan driver and SDK are treated as tools that do not affect the certification basis of flight-critical computations. Partitioning analysis shall demonstrate that rendering subsystem failures cannot propagate to DAL A components.

#### 3.4.3 Standard Libraries

Use of the C++ Standard Library (C++17) is permitted under the project coding standard. Standard library functions used in DAL A code paths shall be verified through requirements-based testing, and their behavior confirmed against the ISO C++17 specification (ISO/IEC 14882:2017).

### 3.5 Previously Developed Software

Beyond JSBSim (Section 3.4.1), no previously developed software is planned for inclusion. If PDS is introduced during development, a gap analysis per DO-178C Section 12.3 shall be performed and this PSAC updated accordingly.

### 3.6 Multi-Version Dissimilar Software

Multi-version dissimilar software techniques are not employed in this project.

### 3.7 Software Partitioning

Partitioning is employed to isolate the Rendering Subsystem (CSCI-SF-RND, DAL C) from all DAL A CSCIs. Partitioning is enforced through:

- Separate process address spaces (OS-level isolation)
- One-way data flow from DAL A components to the rendering subsystem
- No feedback paths from rendering to flight-critical computation
- Partitioning analysis documented in the Software Design Description (SDD)

---

## 4. Software Life Cycle

### 4.1 Life Cycle Model

SynthFlight employs an **iterative/incremental** software life cycle model, as permitted by DO-178C Section 4.0. Each iteration produces a verified increment of functionality, and the full set of DO-178C objectives is satisfied across the aggregate of all iterations.

The life cycle consists of the following integral processes applied iteratively:

```
+-----------------+     +-----------------+     +-----------------+
|   Requirements  | --> |     Design      | --> | Implementation  |
|    Definition    |     |                 |     |  (Coding)       |
+-----------------+     +-----------------+     +-----------------+
        ^                                               |
        |                                               v
+-----------------+     +-----------------+     +-----------------+
|  Configuration  |     |    Quality      |     |  Verification   |
|   Management    | <-- |   Assurance     | <-- |   (Test/Review) |
+-----------------+     +-----------------+     +-----------------+
```

### 4.2 Transition Criteria

Transitions between life cycle phases are governed by the following entry/exit criteria:

| Transition                        | Entry Criteria                                                                                      | Exit Criteria                                                                                   |
|-----------------------------------|------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------|
| System Reqs -> SW Reqs            | System requirements baselined; PSAC approved                                                        | SRS reviewed, all requirements traceable to system requirements, SQA review complete            |
| SW Reqs -> SW Design              | SRS baselined (CCB-approved); requirements review complete with all action items closed              | SDD reviewed, design traceable to requirements, no open Category 1 issues                       |
| SW Design -> Coding               | SDD baselined; design review complete                                                               | Source code reviewed, coding standard compliance verified, code traceable to design             |
| Coding -> Integration             | Unit-level verification complete; code baselined                                                     | All CSCIs integrated, integration test procedures approved                                      |
| Integration -> SW Verification    | Integrated build available; test procedures and environment qualified                                | All test cases executed, structural coverage targets met, all PRs dispositioned                 |
| SW Verification -> Certification  | All DO-178C objectives satisfied; SAS/SCI completed; all open PRs closed or dispositioned as acceptable | Certification authority acceptance                                                              |

### 4.3 Feedback Mechanisms

The iterative model incorporates the following feedback mechanisms:

- **Problem Reports (PRs):** Defects discovered during verification trigger re-entry to the appropriate upstream process.
- **Change Requests (CRs):** Requirements changes are processed through the Configuration Control Board (CCB) and may trigger re-execution of downstream activities.
- **Review Findings:** Formal review findings are tracked to closure before phase exit.

---

## 5. Software Life Cycle Data

The following data items shall be produced in accordance with DO-178C Table A-1. All documents are maintained under configuration management.

| # | Data Item                                          | Document ID         | DO-178C Reference | Status       |
|---|---------------------------------------------------|---------------------|--------------------|--------------|
| 1 | Plan for Software Aspects of Certification (PSAC) | PSAC-SF-2026-001    | 11.1               | Draft        |
| 2 | Software Development Plan (SDP)                    | SDP-SF-2026-001     | 11.2               | Planned      |
| 3 | Software Verification Plan (SVP)                   | SVP-SF-2026-001     | 11.3               | Planned      |
| 4 | Software Configuration Management Plan (SCMP)      | SCMP-SF-2026-001    | 11.4               | Planned      |
| 5 | Software Quality Assurance Plan (SQAP)             | SQAP-SF-2026-001    | 11.5               | Planned      |
| 6 | Software Requirements Standards (SRS-STD)          | SRS-STD-SF-2026-001 | 11.6               | Planned      |
| 7 | Software Design Standards (SDS-STD)                | SDS-STD-SF-2026-001 | 11.7               | Planned      |
| 8 | Software Code Standards (SCS)                      | SCS-SF-2026-001     | 11.8               | Planned      |
| 9 | Software Requirements Data (SRD) / SRS             | SRS-SF-2026-001     | 11.9               | In Progress  |
| 10| Software Design Description (SDD)                  | SDD-SF-2026-001     | 11.10              | Planned      |
| 11| Source Code                                         | N/A (repo-managed)  | 11.11              | In Progress  |
| 12| Executable Object Code                              | N/A (CI-produced)   | 11.12              | Planned      |
| 13| Software Verification Cases and Procedures (SVCP)  | SVCP-SF-2026-001    | 11.13              | Planned      |
| 14| Software Verification Results (SVR)                 | SVR-SF-2026-001     | 11.14              | Planned      |
| 15| Software Life Cycle Environment Configuration Index (SECI) | SECI-SF-2026-001 | 11.15           | Planned      |
| 16| Software Configuration Index (SCI)                  | SCI-SF-2026-001     | 11.16              | Planned      |
| 17| Software Accomplishment Summary (SAS)               | SAS-SF-2026-001     | 11.17              | Planned      |

### 5.1 Supplemental Data Items

| Data Item                              | Document ID           | Purpose                                            |
|----------------------------------------|-----------------------|----------------------------------------------------|
| COTS/Reuse Software Assessment (JSBSim)| CRSA-SF-2026-001      | Gap analysis and qualification strategy for JSBSim |
| Tool Qualification Plan                | TQP-SF-2026-001       | Qualification of verification tools per DO-330     |
| Problem Report Database                | N/A (GitHub Issues)   | Tracking of software defects and change requests   |

---

## 6. Schedule

### 6.1 Program Milestones

| Milestone                                      | Abbreviation | Target Date | Description                                                          |
|------------------------------------------------|-------------|-------------|----------------------------------------------------------------------|
| System Requirements Review                      | SRR         | 2026-Q2     | Review of system-level requirements allocation to software           |
| Software Planning Review                        | SPR         | 2026-Q2     | Review of all planning documents (PSAC, SDP, SVP, SCMP, SQAP)       |
| Preliminary Design Review                       | PDR         | 2026-Q3     | Review of software architecture and high-level design (SDD)          |
| Critical Design Review                          | CDR         | 2026-Q4     | Review of detailed design and coding standard compliance             |
| Test Readiness Review                           | TRR         | 2027-Q1     | Verification environment qualified; test procedures approved         |
| Stage of Involvement 1 (SOI-1: Planning)        | SOI-1       | 2026-Q2     | Certification authority review of plans (PSAC, SDP, SVP, SCMP, SQAP)|
| Stage of Involvement 2 (SOI-2: Development)     | SOI-2       | 2026-Q4     | Review of requirements and design data                               |
| Stage of Involvement 3 (SOI-3: Verification)    | SOI-3       | 2027-Q2     | Review of verification results, structural coverage, traceability    |
| Stage of Involvement 4 (SOI-4: Final)           | SOI-4       | 2027-Q3     | Final review of SAS, SCI, open PR disposition; certification credit  |

### 6.2 Development Iterations

| Iteration | Scope                                                    | Target Completion |
|-----------|----------------------------------------------------------|-------------------|
| I-1       | Core framework, atmosphere model, basic flight dynamics  | 2026-Q3           |
| I-2       | Terrain engine, navigation, sensor simulation            | 2026-Q4           |
| I-3       | Verification engine, data I/O, GCS integration           | 2027-Q1           |
| I-4       | Rendering subsystem, full integration, system test       | 2027-Q2           |
| I-5       | Final verification, coverage closure, certification data | 2027-Q3           |

### 6.3 Certification Data Delivery Schedule

| Delivery                    | Content                                            | Target Date |
|-----------------------------|----------------------------------------------------|-------------|
| SOI-1 Data Package          | PSAC, SDP, SVP, SCMP, SQAP, standards documents   | 2026-Q2     |
| SOI-2 Data Package          | SRS, SDD, traceability data                        | 2026-Q4     |
| SOI-3 Data Package          | SVCP, SVR, coverage analysis, PR status            | 2027-Q2     |
| SOI-4 Data Package (Final)  | SAS, SCI, SECI, final PR disposition               | 2027-Q3     |

---

## 7. Software Life Cycle Environment

### 7.1 Development Environment

#### 7.1.1 Hardware Platforms

| Platform       | OS              | Architecture | Purpose                        |
|----------------|-----------------|-------------|--------------------------------|
| Development WS | Windows 10+     | x86-64      | Primary development platform   |
| Development WS | Ubuntu 22.04+   | x86-64      | Cross-platform development     |
| CI/CD Runners  | Ubuntu (GitHub)  | x86-64      | Automated build and test       |

#### 7.1.2 Development Tools

| Tool                | Version        | Purpose                              | Qualification Required |
|---------------------|---------------|--------------------------------------|------------------------|
| GCC (g++)           | 12.x+         | C++17 compiler                       | No (Note 1)           |
| CMake               | 3.24+         | Build system generator               | No                    |
| Git                 | 2.x           | Version control                      | No                    |
| GitHub              | Cloud          | Repository hosting, issue tracking   | No                    |
| GitHub Actions      | Cloud          | CI/CD pipeline orchestration         | No                    |
| Clang-Tidy          | 15.x+         | Static analysis                      | See Section 7.3       |
| Clang-Format        | 15.x+         | Code formatting enforcement          | No                    |
| Doxygen             | 1.9+          | API documentation generation         | No                    |

**Note 1:** The compiler is a development tool whose output (object code) is verified by testing. Per DO-178C Section 12.2, tools whose output is verified by other activities do not require qualification.

### 7.2 Verification Tools

| Tool              | Version   | Purpose                                       | Qualification Required |
|-------------------|----------|-----------------------------------------------|------------------------|
| Google Test       | 1.14+    | Unit and integration testing framework         | Yes (Criteria 3)      |
| Catch2            | 3.x      | Unit and integration testing framework         | Yes (Criteria 3)      |
| Doctest           | 2.x      | Lightweight unit testing                       | Yes (Criteria 3)      |
| Boost.Test        | 1.82+    | Unit testing framework                         | Yes (Criteria 3)      |
| CppUnit           | 1.15+    | Unit testing framework                         | Yes (Criteria 3)      |
| lcov / gcov       | 2.x / 12+| Structural coverage measurement (MC/DC)        | Yes (Criteria 1/2)   |
| libFuzzer         | 15.x+    | Fuzz testing for robustness verification       | No (Note 2)          |
| Valgrind          | 3.20+    | Memory error detection                         | No (Note 2)          |

**Note 2:** libFuzzer and Valgrind are used for robustness testing and defect detection. Their outputs do not directly eliminate or automate DO-178C verification activities; they supplement requirements-based testing. Findings from these tools are recorded as problem reports and verified by requirements-based tests.

### 7.3 Tool Qualification

Per DO-178C Section 12.2 and DO-330 (*Software Tool Qualification Considerations*), tools that automate verification processes and whose output is used to satisfy DO-178C objectives require qualification. The following tool qualification criteria apply:

| Criteria | Definition                                                                                   | Applicable Tools                    |
|----------|----------------------------------------------------------------------------------------------|-------------------------------------|
| TQL-1    | Tool output is part of airborne software and could insert errors (DAL A equivalent)          | None                                |
| TQL-2    | Tool output is part of airborne software and could fail to detect errors (DAL A equivalent)  | None                                |
| TQL-3    | Tool automates verification and could fail to detect errors (DAL C equivalent)               | See below                           |
| TQL-4    | Tool automates verification and could fail to detect errors (DAL D equivalent)               | None                                |
| TQL-5    | Tool output is verified by other processes                                                    | Compiler (GCC), build tools         |

**TQL-3 Qualification Required:**

- **Test Frameworks (GTest, Catch2, Doctest, Boost.Test, CppUnit):** These tools automate test execution and produce pass/fail verdicts. A false "pass" could mask a defect. Qualification per DO-330 TQL-3 is required. The Tool Qualification Plan (TQP-SF-2026-001) shall define the operational requirements, verification activities, and tool qualification data for each framework.
- **Coverage Tools (lcov/gcov):** These tools measure structural coverage. Under-reporting of uncovered code could result in premature closure of coverage objectives. Qualification per DO-330 TQL-2 or TQL-3 is required based on the coverage criteria being satisfied (MC/DC for DAL A). The TQP shall detail the qualification approach.

### 7.4 Environment Configuration Management

The complete Software Life Cycle Environment shall be documented in the Software Life Cycle Environment Configuration Index (SECI-SF-2026-001), including:

- Tool versions and installation configurations
- OS versions and patch levels
- Compiler flags and build configurations
- CI/CD pipeline definitions (GitHub Actions workflow files)
- Library dependencies and their versions

---

## 8. Software Development Processes

### 8.1 Reference Document

The software development processes are defined in the **Software Development Plan (SDP-SF-2026-001)**, which addresses:

- Software requirements process
- Software design process
- Software coding process
- Integration process

### 8.2 Software Requirements Process

- Requirements are captured in SRS-SF-2026-001 (149 requirements across 19 categories).
- Each requirement shall be uniquely identified, unambiguous, verifiable, and traceable to system requirements.
- Requirements reviews shall be conducted with SQA independence.
- Derived requirements shall be identified and provided to the system safety assessment process.

### 8.3 Software Design Process

- The architecture shall be documented in SDD-SF-2026-001.
- Design shall be traceable to software requirements.
- CSCI partitioning and interface definitions shall be reviewed for correctness and completeness.
- Design reviews (PDR, CDR) shall include SQA participation.

### 8.4 Software Coding Process

- Source code shall conform to the Software Code Standards (SCS-SF-2026-001).
- The coding standard is based on **C++17 (ISO/IEC 14882:2017)** compiled with **GCC**.
- Coding standard compliance shall be enforced through automated static analysis (Clang-Tidy) and peer code review.
- Code shall be traceable to software design.

### 8.5 Integration Process

- CSCIs shall be integrated incrementally per the iteration plan (Section 6.2).
- Integration builds shall be produced and verified through the CI/CD pipeline (GitHub Actions).
- Integration test procedures shall verify inter-CSCI interfaces and data flows.

---

## 9. Software Verification Processes

### 9.1 Reference Document

The software verification processes are defined in the **Software Verification Plan (SVP-SF-2026-001)**.

### 9.2 Verification Methods

| Method          | Application                                                                       |
|-----------------|-----------------------------------------------------------------------------------|
| Review          | Requirements, design, source code, test procedures, traceability                  |
| Analysis        | Algorithms, timing, memory usage, worst-case execution, partitioning integrity     |
| Testing         | Requirements-based testing (normal range, robustness, boundary), integration, system |
| Inspection      | Coding standard compliance, configuration data, build outputs                      |

### 9.3 Requirements-Based Testing

- All 149 software requirements in SRS-SF-2026-001 shall have associated test cases documented in SVCP-SF-2026-001.
- Test cases shall include normal-range, robustness, and boundary value inputs.
- Test case traceability to requirements shall be verified bidirectionally.

### 9.4 Structural Coverage Requirements

For DAL A, the following structural coverage criteria shall be satisfied per DO-178C Table A-7:

| Coverage Criterion                              | Required | Analysis Method         |
|------------------------------------------------|----------|-------------------------|
| Statement Coverage                              | Yes      | gcov / lcov             |
| Decision Coverage                               | Yes      | gcov / lcov             |
| Modified Condition/Decision Coverage (MC/DC)    | Yes      | gcov / lcov + analysis  |
| Data Coupling and Control Coupling Analysis     | Yes      | Manual analysis + tools |

- Structural coverage shall be measured against requirements-based test cases.
- Coverage shortfalls shall be analyzed and resolved by one of: (a) adding test cases, (b) identifying dead code for removal, or (c) providing analysis justification (deactivated code is not permitted — see Section 3.2).

### 9.5 Verification Independence

For DAL A, verification activities require independence per DO-178C Table A-5:

- Reviews of software requirements, design, and code shall be performed by personnel who did not develop the item under review.
- Test case development and test execution may be performed by the development team, but test results review shall be performed with independence.
- Structural coverage analysis results shall be reviewed independently.
- SQA audits of verification activities shall be conducted per SQAP-SF-2026-001.

---

## 10. Software Configuration Management

### 10.1 Reference Document

Software Configuration Management activities are defined in the **Software Configuration Management Plan (SCMP-SF-2026-001)**.

### 10.2 SCM Overview

- **Repository:** Git (hosted on GitHub)
- **Branching Strategy:** Defined in SCMP; all changes to baselined items require CCB approval.
- **Configuration Control Board (CCB):** Reviews and approves all changes to baselined configuration items.
- **Baselines:** Functional baseline (requirements), allocated baseline (design), product baseline (code and test).
- **Change Control:** Problem reports and change requests tracked via GitHub Issues with classification and impact analysis.
- **Build Identification:** Each build is uniquely identified by Git commit hash and semantic version tag.
- **Archive and Retrieval:** All certification data shall be archived for the life of the product plus the duration required by the applicable airworthiness authority.

### 10.3 Configuration Items

All life cycle data items listed in Section 5 are configuration items subject to SCM control. Additionally:

- Source code (all CSCIs)
- Build configuration files (CMakeLists.txt, CI/CD workflow definitions)
- Test data and test fixtures
- Third-party source code (JSBSim v1.2.4)
- Tool configurations and qualification data

---

## 11. Software Quality Assurance

### 11.1 Reference Document

Software Quality Assurance activities are defined in the **Software Quality Assurance Plan (SQAP-SF-2026-001)**.

### 11.2 SQA Overview

The SQA function provides independent oversight of all software life cycle processes to ensure:

- Compliance with the approved plans (PSAC, SDP, SVP, SCMP)
- Compliance with applicable standards (requirements, design, coding standards)
- Adequate transition criteria are met between life cycle phases
- Deviations from processes or standards are identified, recorded, and resolved
- Software conformity review is performed on the final product

### 11.3 SQA Independence

For DAL A, the SQA function shall be organizationally independent from the software development team. The SQA authority shall have:

- Direct reporting to project management outside the development chain
- Authority to escalate unresolved quality issues
- Access to all project artifacts and processes

### 11.4 SQA Activities

| Activity                        | Frequency          | Reference           |
|---------------------------------|--------------------|---------------------|
| Process compliance audits       | Per phase/iteration| SQAP Section TBD    |
| Standards conformity reviews    | Per artifact       | SQAP Section TBD    |
| Transition criteria verification| Per phase exit     | SQAP Section TBD    |
| SCM audit                       | Quarterly          | SQAP Section TBD    |
| Verification results review     | Per test cycle     | SQAP Section TBD    |
| Software Conformity Review (SCR)| Pre-SOI-4          | SQAP Section TBD    |
| Problem report review           | Ongoing            | SQAP Section TBD    |

---

## 12. Certification Liaison

### 12.1 Certification Authority Interface

The project shall interface with the certification authority through a Designated Engineering Representative (DER) or Compliance Verification Engineer (CVE) assigned to the project. The certification liaison process ensures:

- Timely submittal of life cycle data for certification authority review
- Agreement on the means of compliance prior to commencement of each phase
- Resolution of certification authority findings and concerns
- Coordination on the interpretation of DO-178C objectives and their satisfaction

### 12.2 Stage of Involvement Audits

| Stage  | Purpose                                              | Data Submitted                                        | Timing    |
|--------|------------------------------------------------------|-------------------------------------------------------|-----------|
| SOI-1  | Review of software planning                          | PSAC, SDP, SVP, SCMP, SQAP, standards documents      | 2026-Q2   |
| SOI-2  | Review of development processes and outputs          | SRS, SDD, traceability matrices, review records       | 2026-Q4   |
| SOI-3  | Review of verification processes and results         | SVCP, SVR, coverage reports, PR status, SQA records   | 2027-Q2   |
| SOI-4  | Final review for certification credit                | SAS, SCI, SECI, final PR disposition, SCR records     | 2027-Q3   |

### 12.3 Means of Compliance

The primary means of compliance is conformance to RTCA DO-178C. No alternative methods of compliance or supplement usage (DO-331, DO-332, DO-333) are currently planned. If model-based development, object-oriented technology, or formal methods are introduced, this PSAC shall be updated and the appropriate supplement applied.

### 12.4 Issue Resolution

Issues raised by the certification authority shall be tracked as formal action items with assigned owners, due dates, and resolution status. Unresolved certification issues shall be escalated through project management.

---

## 13. Compliance Matrix

### 13.1 DO-178C Objectives Mapping

The following tables map DO-178C objectives (Tables A-1 through A-10) to project processes, activities, and evidence. For DAL A, all objectives apply, and those marked with independence requirements are identified.

#### Table A-1: Software Planning Process

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-1.1 | Software development and integral processes are defined                     | SDP-SF-2026-001, SVP, SCMP, SQAP              | N/A          | Planned |
| A-1.2 | Transition criteria, inter-relationships, and sequencing are defined         | PSAC Section 4.2; SDP                         | N/A          | Planned |
| A-1.3 | Software life cycle environment is defined                                   | PSAC Section 7; SECI                          | N/A          | Planned |
| A-1.4 | Additional considerations are addressed                                      | PSAC Section 3 (COTS, PDS, deactivated code)  | N/A          | Planned |
| A-1.5 | Software development standards are defined                                   | SRS-STD, SDS-STD, SCS                         | N/A          | Planned |
| A-1.6 | Software plans comply with this document                                     | SOI-1 review of PSAC + all plans              | N/A          | Planned |
| A-1.7 | Development and integral processes comply with plans and standards            | SQA audits per SQAP                           | Yes          | Planned |

#### Table A-2: Software Development Process (Requirements)

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-2.1 | High-level requirements are developed                                        | SRS-SF-2026-001 (149 reqs, 19 categories)     | N/A          | In Prog |
| A-2.2 | Derived high-level requirements are defined                                  | SRS review, system safety feedback             | N/A          | Planned |
| A-2.3 | High-level requirements are traceable to system requirements                 | Traceability matrix in SRS                     | N/A          | In Prog |
| A-2.4 | High-level requirements are accurate and consistent                          | Requirements review (peer + SQA)               | Yes          | Planned |
| A-2.5 | High-level requirements are verifiable                                       | Requirements review checklist                  | Yes          | Planned |
| A-2.6 | High-level requirements conform to standards                                 | SRS-STD compliance review                      | Yes          | Planned |
| A-2.7 | High-level requirements are compatible with target computer                  | Requirements review                            | Yes          | Planned |

#### Table A-3: Software Development Process (Design)

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-3.1 | Low-level requirements are developed                                         | SDD-SF-2026-001                               | N/A          | Planned |
| A-3.2 | Derived low-level requirements are defined                                   | Design review, system safety feedback          | N/A          | Planned |
| A-3.3 | Low-level requirements are traceable to high-level requirements              | Design-to-requirements traceability matrix     | N/A          | Planned |
| A-3.4 | Low-level requirements are accurate and consistent                           | Design review (peer + SQA)                     | Yes          | Planned |
| A-3.5 | Low-level requirements are verifiable                                        | Design review checklist                        | Yes          | Planned |
| A-3.6 | Low-level requirements conform to standards                                  | SDS-STD compliance review                      | Yes          | Planned |
| A-3.7 | Software architecture is compatible with high-level requirements             | Architecture review (PDR)                      | Yes          | Planned |

#### Table A-4: Software Development Process (Coding)

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-4.1 | Source code is developed                                                     | Source code repository (Git/GitHub)            | N/A          | In Prog |
| A-4.2 | Source code is traceable to low-level requirements                           | Code-to-design traceability matrix             | N/A          | Planned |
| A-4.3 | Source code is traceable to SDD and SRS                                      | Bidirectional traceability analysis             | N/A          | Planned |
| A-4.4 | Source code is accurate and consistent                                       | Code review (peer + SQA)                       | Yes          | Planned |
| A-4.5 | Source code conforms to standards                                            | Clang-Tidy + code review vs SCS               | Yes          | Planned |
| A-4.6 | Source code is compatible with target computer                               | Cross-platform build verification (Win + Linux)| Yes          | Planned |

#### Table A-5: Software Development Process (Integration)

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-5.1 | Executable object code is generated                                          | CI/CD build pipeline (GitHub Actions)          | N/A          | In Prog |
| A-5.2 | Executable is traceable to source code                                       | Build configuration, link maps, CI artifacts   | N/A          | Planned |
| A-5.3 | Integration produces correct results                                         | Integration test execution                     | N/A          | Planned |

#### Table A-6: Verification of Outputs of Software Requirements Process

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-6.1 | High-level requirements comply with system requirements                      | Requirements review records                    | Yes          | Planned |
| A-6.2 | High-level requirements are accurate and consistent                          | Review checklists, SQA audit records           | Yes          | Planned |
| A-6.3 | High-level requirements are compatible with target computer                  | Platform compatibility analysis                | Yes          | Planned |
| A-6.4 | High-level requirements are verifiable                                       | Verification method assignment per requirement | Yes          | Planned |
| A-6.5 | High-level requirements conform to standards                                 | SRS-STD compliance analysis                    | Yes          | Planned |
| A-6.6 | High-level requirements are traceable to system requirements                 | Bidirectional traceability analysis             | Yes          | Planned |
| A-6.7 | Algorithms are accurate                                                      | Algorithm analysis (flight dynamics, atm model)| Yes          | Planned |

#### Table A-7: Verification of Outputs of Software Design Process

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-7.1 | Low-level requirements comply with high-level requirements                   | Design review records                          | Yes          | Planned |
| A-7.2 | Low-level requirements are accurate and consistent                           | Review checklists, SQA audit records           | Yes          | Planned |
| A-7.3 | Low-level requirements are compatible with target computer                   | Platform compatibility analysis                | Yes          | Planned |
| A-7.4 | Low-level requirements are verifiable                                        | Verification method assignment                 | Yes          | Planned |
| A-7.5 | Low-level requirements conform to standards                                  | SDS-STD compliance analysis                    | Yes          | Planned |
| A-7.6 | Low-level requirements are traceable to high-level requirements              | Bidirectional traceability analysis             | Yes          | Planned |
| A-7.7 | Software architecture is compatible with high-level requirements             | Architecture review, partitioning analysis     | Yes          | Planned |
| A-7.8 | Software architecture is consistent                                          | Architecture review records                    | Yes          | Planned |
| A-7.9 | Software partitioning integrity is confirmed                                 | Partitioning analysis (RND vs DAL A CSCIs)     | Yes          | Planned |

#### Table A-8: Verification of Outputs of Software Coding and Integration Process

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-8.1 | Source code complies with low-level requirements                             | Code review records                            | Yes          | Planned |
| A-8.2 | Source code complies with software architecture                              | Architecture conformance review                | Yes          | Planned |
| A-8.3 | Source code is verifiable                                                    | Code review (testability assessment)           | Yes          | Planned |
| A-8.4 | Source code conforms to standards                                            | Clang-Tidy reports + code review vs SCS        | Yes          | Planned |
| A-8.5 | Source code is traceable to low-level requirements                           | Traceability analysis                          | Yes          | Planned |
| A-8.6 | Source code is accurate and consistent                                       | Code review records                            | Yes          | Planned |
| A-8.7 | Output of integration process is complete and correct                        | Integration test results (CI/CD)               | Yes          | Planned |

#### Table A-9: Verification of Verification Process Results (Testing)

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-9.1 | Test procedures are correct                                                  | Test procedure review                          | Yes          | Planned |
| A-9.2 | Test results are correct and discrepancies explained                         | SVR-SF-2026-001, PR disposition                | Yes          | Planned |
| A-9.3 | Requirements-based test coverage is achieved                                 | Requirements-to-test traceability              | Yes          | Planned |
| A-9.4 | Test coverage of high-level requirements is achieved                         | Coverage analysis in SVR                       | Yes          | Planned |
| A-9.5 | Test coverage of low-level requirements is achieved                          | Coverage analysis in SVR                       | Yes          | Planned |
| A-9.6 | Test coverage of software structure (Statement) is achieved                  | lcov/gcov statement coverage report            | Yes          | Planned |
| A-9.7 | Test coverage of software structure (Decision) is achieved                   | lcov/gcov decision coverage report             | Yes          | Planned |
| A-9.8 | Test coverage of software structure (MC/DC) is achieved                      | lcov/gcov + MC/DC analysis report              | Yes          | Planned |
| A-9.9 | Test coverage of software structure (data/control coupling) is achieved      | Data/control coupling analysis                 | Yes          | Planned |

#### Table A-10: Software Configuration Management Process

| Obj # | Objective Description                                                        | Project Activity / Evidence                    | Independence | Status  |
|-------|-----------------------------------------------------------------------------|-----------------------------------------------|--------------|---------|
| A-10.1| Configuration items are identified                                           | SCMP CI list, SCI                             | N/A          | Planned |
| A-10.2| Baselines and traceability are established                                   | Git tags, baseline records, traceability data  | N/A          | Planned |
| A-10.3| Problem reporting, tracking, and corrective action                           | GitHub Issues, PR database                     | N/A          | Planned |
| A-10.4| Change control integrity is maintained                                       | CCB records, branch protection, CI gates       | N/A          | Planned |
| A-10.5| Change review includes determination of effect on safety                     | Change impact analysis (CCB review)            | Yes          | Planned |
| A-10.6| Configuration status accounting is performed                                 | SCI-SF-2026-001                               | N/A          | Planned |
| A-10.7| Archive, retrieval, and release are controlled                               | GitHub releases, archive procedures            | N/A          | Planned |
| A-10.8| Software load control is established                                         | Build identification, version tagging          | N/A          | Planned |
| A-10.9| Software life cycle environment is controlled                                | SECI-SF-2026-001                              | N/A          | Planned |

### 13.2 Independence Summary

For DAL A, the following activities require independence (i.e., the person performing the activity is not the person who developed the item):

- All verification reviews (Tables A-6 through A-9)
- SQA process compliance audits (Table A-1, Obj A-1.7)
- Change impact analysis for safety (Table A-10, Obj A-10.5)

Independence is satisfied through organizational separation between the development team and the verification/SQA team, as defined in the SDP and SQAP.

---

## Appendix A: Acronyms and Definitions

| Acronym | Definition                                                  |
|---------|-------------------------------------------------------------|
| CCB     | Configuration Control Board                                  |
| CDR     | Critical Design Review                                       |
| COTS    | Commercial Off-The-Shelf                                     |
| CR      | Change Request                                               |
| CSCI    | Computer Software Configuration Item                         |
| CVE     | Compliance Verification Engineer                             |
| DAL     | Design Assurance Level                                       |
| DER     | Designated Engineering Representative                        |
| DTED    | Digital Terrain Elevation Data                               |
| FHA     | Functional Hazard Assessment                                 |
| ISA     | International Standard Atmosphere                            |
| MC/DC   | Modified Condition/Decision Coverage                         |
| PDR     | Preliminary Design Review                                    |
| PDS     | Previously Developed Software                                |
| PR      | Problem Report                                               |
| PSAC    | Plan for Software Aspects of Certification                   |
| PSSA    | Preliminary System Safety Assessment                         |
| SAS     | Software Accomplishment Summary                              |
| SAR     | Search and Rescue                                            |
| SCI     | Software Configuration Index                                 |
| SCMP    | Software Configuration Management Plan                       |
| SCR     | Software Conformity Review                                   |
| SDD     | Software Design Description                                  |
| SDP     | Software Development Plan                                    |
| SECI    | Software Life Cycle Environment Configuration Index          |
| SOI     | Stage of Involvement                                         |
| SQAP    | Software Quality Assurance Plan                              |
| SRR     | System Requirements Review                                   |
| SRS     | Software Requirements Specification                          |
| SSA     | System Safety Assessment                                     |
| SVCP    | Software Verification Cases and Procedures                   |
| SVP     | Software Verification Plan                                   |
| SVR     | Software Verification Results                                |
| TQP     | Tool Qualification Plan                                      |
| TRR     | Test Readiness Review                                        |
| UAV     | Unmanned Aerial Vehicle                                      |

---

## Appendix B: Referenced Documents

| Document ID          | Title                                                                     |
|----------------------|---------------------------------------------------------------------------|
| RTCA DO-178C         | Software Considerations in Airborne Systems and Equipment Certification   |
| RTCA DO-330          | Software Tool Qualification Considerations                                |
| RTCA DO-331          | Model-Based Development and Verification Supplement to DO-178C            |
| RTCA DO-332          | Object-Oriented Technology and Related Techniques Supplement to DO-178C   |
| RTCA DO-333          | Formal Methods Supplement to DO-178C                                      |
| SAE ARP 4754A        | Guidelines for Development of Civil Aircraft and Systems                  |
| SAE ARP 4761         | Guidelines and Methods for Conducting the Safety Assessment Process       |
| ISO/IEC 14882:2017   | Programming Language C++ (C++17)                                          |
| SRS-SF-2026-001      | SynthFlight Software Requirements Specification                          |
| SDP-SF-2026-001      | SynthFlight Software Development Plan                                    |
| SVP-SF-2026-001      | SynthFlight Software Verification Plan                                   |
| SCMP-SF-2026-001     | SynthFlight Software Configuration Management Plan                       |
| SQAP-SF-2026-001     | SynthFlight Software Quality Assurance Plan                              |
| SDD-SF-2026-001      | SynthFlight Software Design Description                                  |
| TQP-SF-2026-001      | SynthFlight Tool Qualification Plan                                      |
| CRSA-SF-2026-001     | SynthFlight COTS/Reuse Software Assessment (JSBSim)                      |

---

*End of Document — PSAC-SF-2026-001*
