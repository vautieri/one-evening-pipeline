# CDRL Register — SynthFlight UAV Mission Simulation and Verification Platform

| Field              | Value                                                        |
|--------------------|--------------------------------------------------------------|
| **Document ID**    | CDRL-SF-2026-001                                             |
| **Version**        | 1.0                                                          |
| **Date**           | 2026-03-14                                                   |
| **Project**        | SynthFlight — UAV Mission Simulation and Verification Platform |
| **DAL**            | A (Catastrophic)                                             |
| **Applicable Standard** | DO-178C (Software Considerations in Airborne Systems and Equipment Certification) |
| **Coding Standard**| C++17 (2023), GCC                                            |
| **SRS Reference**  | SRS-SF-2026-001 (119 unified requirements, SRS-SynthFlight.md) |
| **Test Frameworks**| Google Test, Catch2, Doctest, Boost.Test, CppUnit, libFuzzer |
| **CI/CD**          | GitHub Actions (build, test, coverage, CodeQL, fuzz, dependency-review) |
| **Status**         | Baselined                                                    |
| **Prepared By**    | Software Quality Assurance                                   |
| **Approved By**    | Program Manager / DER                                        |

---

## 1. Purpose

This Contract Data Requirements List (CDRL) Register identifies and tracks all seventeen (17) DO-178C Software Life Cycle Data items defined in Table A-1 of DO-178C/ED-12C. Each entry maps the required data item to its project-specific document, current status, and delivery milestone for the SynthFlight DAL A certification program.

## 2. Scope

This register covers all software life cycle data required for DAL A certification under DO-178C. At DAL A, all seventeen data items are required, and certain items require independent review by Software Quality Assurance (SQA) per the objectives in DO-178C Annex A Tables A-1 through A-10.

## 3. References

| Reference          | Title                                                        |
|--------------------|--------------------------------------------------------------|
| DO-178C / ED-12C   | Software Considerations in Airborne Systems and Equipment Certification |
| DO-248C            | Supporting Information for DO-178C and DO-278A               |
| DO-330             | Software Tool Qualification Considerations                   |
| FAA Order 8110.49  | Software Approval Guidelines                                 |
| AC 20-115D         | Airborne Software Development Assurance Using EUROCAE ED-12 and RTCA DO-178 |
| MIL-STD-498        | Software Development and Documentation (DID references)      |
| SRS-SF-2026-001    | SynthFlight Software Requirements Specification              |

---

## 4. CDRL Summary Table

| CDRL No. | Abbr. | Data Item Title | DO-178C Ref | Document ID | Status | DAL A Applicability | Target Baseline |
|----------|-------|-----------------|-------------|-------------|--------|---------------------|-----------------|
| CDRL-SF-001 | PSAC | Plan for Software Aspects of Certification | 11.1 | PSAC-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-002 | SDP | Software Development Plan | 11.2 | SDP-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-003 | SVP | Software Verification Plan | 11.3 | SVP-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-004 | SCMP | Software Configuration Management Plan | 11.4 | SCMP-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-005 | SQAP | Software Quality Assurance Plan | 11.5 | SQAP-SF-2026-001 | Baselined | Required with Independence | 2026-05-15 |
| CDRL-SF-006 | SRS-STD | Software Requirements Standards | 11.6 | SRS-STD-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-007 | SDS | Software Design Standards | 11.7 | SDS-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-008 | SCS | Software Code Standards | 11.8 | SCS-SF-2026-001 | Baselined | Required | 2026-05-15 |
| CDRL-SF-009 | SRD | Software Requirements Data | 11.9 | SRD-SF-2026-001 | Draft | Required | 2026-07-01 |
| CDRL-SF-010 | SDD | Software Design Description | 11.10 | SDD-SF-2026-001 | Not Started | Required | 2026-08-15 |
| CDRL-SF-011 | SC | Source Code | 11.11 | SC-SF-2026-001 | Draft | Required | 2026-10-01 |
| CDRL-SF-012 | EO | Executable Object Code | 11.12 | EO-SF-2026-001 | Not Started | Required | 2026-11-01 |
| CDRL-SF-013 | SVCP | Software Verification Cases and Procedures | 11.13 | SVCP-SF-2026-001 | Draft | Required with Independence | 2026-09-15 |
| CDRL-SF-014 | SVR | Software Verification Results | 11.14 | SVR-SF-2026-001 | Draft | Required with Independence | 2026-11-15 |
| CDRL-SF-015 | SECI | Software Life Cycle Environment Configuration Index | 11.15 | SECI-SF-2026-001 | Not Started | Required | 2026-11-15 |
| CDRL-SF-016 | SCI | Software Configuration Index | 11.16 | SCI-SF-2026-001 | Not Started | Required | 2026-12-01 |
| CDRL-SF-017 | SAS | Software Accomplishment Summary | 11.17 | SAS-SF-2026-001 | Not Started | Required with Independence | 2026-12-15 |

---

## 5. Detailed CDRL Entries

---

### CDRL-SF-001 — Plan for Software Aspects of Certification (PSAC)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-001 |
| **Data Item Title** | Plan for Software Aspects of Certification |
| **Abbreviation** | PSAC |
| **DO-178C Reference** | Section 11.1 |
| **DID Reference** | DI-MISC-81466 |
| **Document ID** | PSAC-SF-2026-001 |
| **File Path** | `docs/plans/PSAC-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Project Manager |
| **Review Authority** | DER / Certification Authority |

**Description:** The PSAC is the primary means of communication between the applicant and the certification authority. It describes the software life cycle processes, software level (DAL A), and the means by which compliance with DO-178C is achieved for SynthFlight. It identifies all deviations, alternative methods, and previously developed software used in the project.

**DAL A Notes:** At DAL A, the PSAC must address all objectives in Tables A-1 through A-10. The PSAC is submitted to the certification authority for approval prior to the Stage of Involvement (SOI) 1 review.

---

### CDRL-SF-002 — Software Development Plan (SDP)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-002 |
| **Data Item Title** | Software Development Plan |
| **Abbreviation** | SDP |
| **DO-178C Reference** | Section 11.2 |
| **DID Reference** | DI-IPSC-81427 |
| **Document ID** | SDP-SF-2026-001 |
| **File Path** | `docs/plans/SDP-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Development Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The SDP defines the software development environment, development processes, coding standards (C++17/GCC), and software life cycle model used for SynthFlight. It addresses requirements development, design, coding, and integration activities and specifies the tools, methods, and procedures to be used throughout the development life cycle.

**DAL A Notes:** The SDP must describe transition criteria between life cycle processes and identify all development tools. Tool qualification per DO-330 must be addressed for tools whose output is not verified (e.g., code generators, compilers).

---

### CDRL-SF-003 — Software Verification Plan (SVP)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-003 |
| **Data Item Title** | Software Verification Plan |
| **Abbreviation** | SVP |
| **DO-178C Reference** | Section 11.3 |
| **DID Reference** | DI-IPSC-81429 |
| **Document ID** | SVP-SF-2026-001 |
| **File Path** | `docs/plans/SVP-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Verification Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The SVP describes the verification environment, activities, methods, and tools used to verify SynthFlight software. It identifies the test frameworks (Google Test, Catch2, Doctest, Boost.Test, CppUnit, libFuzzer), structural coverage analysis methods (MC/DC for DAL A), and CI/CD verification pipeline (GitHub Actions). It addresses reviews, analyses, and testing of requirements, design, code, and integration outputs.

**DAL A Notes:** At DAL A, the SVP must address Modified Condition/Decision Coverage (MC/DC). Independence between development and verification activities is required for certain objectives. The SVP must specify robustness testing and requirements-based test coverage criteria.

---

### CDRL-SF-004 — Software Configuration Management Plan (SCMP)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-004 |
| **Data Item Title** | Software Configuration Management Plan |
| **Abbreviation** | SCMP |
| **DO-178C Reference** | Section 11.4 |
| **DID Reference** | DI-IPSC-81441 |
| **Document ID** | SCMP-SF-2026-001 |
| **File Path** | `docs/plans/SCMP-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.1 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Configuration Manager |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The SCMP defines the configuration management environment, activities, and procedures for identifying, controlling, tracking, and auditing all SynthFlight configuration items. It covers baseline management, change control, problem reporting, and archive/retrieval procedures using Git and GitHub Actions.

**DAL A Notes:** At DAL A, the SCMP must address configuration identification of all software life cycle data, change control with impact analysis, configuration status accounting, and release procedures including archive and retrieval.

---

### CDRL-SF-005 — Software Quality Assurance Plan (SQAP)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-005 |
| **Data Item Title** | Software Quality Assurance Plan |
| **Abbreviation** | SQAP |
| **DO-178C Reference** | Section 11.5 |
| **DID Reference** | DI-QCIC-80572 |
| **Document ID** | SQAP-SF-2026-001 |
| **File Path** | `docs/plans/SQAP-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required with Independence |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Quality Assurance Manager |
| **Review Authority** | Program Manager / DER |

**Description:** The SQAP defines the SQA activities, authority, and independence required to provide assurance that the SynthFlight software development and verification processes conform to approved plans and standards. It establishes audit schedules, conformity review procedures, and escalation paths for process non-conformances.

**DAL A Notes:** At DAL A, SQA must have organizational independence from the development team. The SQAP must address SQA authority to ensure process compliance, including the right to halt work when deviations are found. SQA performs independent conformity reviews of all software life cycle data.

---

### CDRL-SF-006 — Software Requirements Standards (SRS-STD)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-006 |
| **Data Item Title** | Software Requirements Standards |
| **Abbreviation** | SRS-STD |
| **DO-178C Reference** | Section 11.6 |
| **DID Reference** | DI-IPSC-81431 |
| **Document ID** | SRS-STD-SF-2026-001 |
| **File Path** | `docs/standards/SRS-Standard-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Requirements Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The Software Requirements Standards define the methods, rules, and tools to be used to develop the high-level software requirements for SynthFlight. This includes requirements notation, naming conventions (as used in SRS-SF-2026-001), traceability conventions, and criteria for well-formed requirements (unambiguous, verifiable, conformant, unique, traceable).

**DAL A Notes:** At DAL A, these standards must address derived requirements identification, requirements traceability to system requirements, and criteria for verifiable requirements statements.

---

### CDRL-SF-007 — Software Design Standards (SDS-STD)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-007 |
| **Data Item Title** | Software Design Standards |
| **Abbreviation** | SDS-STD |
| **DO-178C Reference** | Section 11.7 |
| **DID Reference** | DI-IPSC-81432 |
| **Document ID** | SDS-SF-2026-001 |
| **File Path** | `docs/standards/SDS-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Architecture Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The Software Design Standards define the methods, rules, and tools used to develop the software architecture and low-level requirements for SynthFlight. This includes design description methods, naming conventions, complexity constraints, and design patterns approved for use.

**DAL A Notes:** At DAL A, design standards must address low-level requirements in addition to architecture. Standards for design description methods must enable verification of design against requirements through reviews and analyses.

---

### CDRL-SF-008 — Software Code Standards (SCS-STD)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-008 |
| **Data Item Title** | Software Code Standards |
| **Abbreviation** | SCS-STD |
| **DO-178C Reference** | Section 11.8 |
| **DID Reference** | DI-IPSC-81433 |
| **Document ID** | SCS-SF-2026-001 |
| **File Path** | `docs/standards/SCS-SynthFlight.md` |
| **Status** | Baselined |
| **DAL A Applicability** | Required |
| **Current Version** | 1.0 |
| **Target Baseline Date** | 2026-05-15 |
| **Responsible Role** | Software Development Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The Software Code Standards define the programming languages, coding rules, and conventions for SynthFlight source code. The project uses C++17 (2023 standard) compiled with GCC. Standards address naming conventions, commenting, formatting, prohibited language features, complexity limits, and static analysis rules enforced through CodeQL and CI/CD pipelines.

**DAL A Notes:** At DAL A, code standards must restrict use of features that impede verification (e.g., dynamic memory allocation after initialization, recursion, pointer arithmetic). Standards compliance is verified through code reviews and automated static analysis.

---

### CDRL-SF-009 — Software Requirements Data (SRD)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-009 |
| **Data Item Title** | Software Requirements Data |
| **Abbreviation** | SRD |
| **DO-178C Reference** | Section 11.9 |
| **DID Reference** | DI-IPSC-81430 |
| **Document ID** | SRD-SF-2026-001 |
| **File Path** | `docs/requirements/SRS-SynthFlight.md` |
| **Status** | Draft |
| **DAL A Applicability** | Required |
| **Current Version** | 0.3 |
| **Target Baseline Date** | 2026-07-01 |
| **Responsible Role** | Software Requirements Lead |
| **Review Authority** | Software Quality Assurance (SQA) / Systems Engineering |

**Description:** The Software Requirements Data contains the high-level software requirements for SynthFlight. The current SRS (SRS-SF-2026-001) contains 119 unified requirements covering functional, performance, safety, interface, and environmental requirements for the UAV Mission Simulation and Verification Platform. Requirements are traceable to system-level requirements and to verification procedures.

**DAL A Notes:** At DAL A, all high-level requirements must be traceable to system requirements. Derived requirements must be identified and provided to the system safety process. Requirements must be verifiable, conformant, and free of ambiguity. Requirements-based coverage must achieve 100%.

---

### CDRL-SF-010 — Software Design Description (SDD)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-010 |
| **Data Item Title** | Software Design Description |
| **Abbreviation** | SDD |
| **DO-178C Reference** | Section 11.10 |
| **DID Reference** | DI-IPSC-81435 |
| **Document ID** | SDD-SF-2026-001 |
| **File Path** | `docs/design/SDD-SynthFlight.md` |
| **Status** | Not Started |
| **DAL A Applicability** | Required |
| **Current Version** | — |
| **Target Baseline Date** | 2026-08-15 |
| **Responsible Role** | Software Architecture Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The Software Design Description contains the software architecture and low-level requirements for SynthFlight. It describes the software structure, data flow, control flow, resource allocation, scheduling, inter-partition communication, and the mapping of high-level requirements to software components. Low-level requirements are directly implementable in source code.

**DAL A Notes:** At DAL A, both software architecture and low-level requirements are required. The design must be traceable from high-level requirements to architecture to low-level requirements. Design must enable verification of all data and control coupling. Derived low-level requirements must be identified.

---

### CDRL-SF-011 — Source Code (SC)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-011 |
| **Data Item Title** | Source Code |
| **Abbreviation** | SC |
| **DO-178C Reference** | Section 11.11 |
| **DID Reference** | DI-IPSC-81434 |
| **Document ID** | SC-SF-2026-001 |
| **File Path** | `src/` (repository source tree) |
| **Status** | Draft |
| **DAL A Applicability** | Required |
| **Current Version** | 0.2 |
| **Target Baseline Date** | 2026-10-01 |
| **Responsible Role** | Software Development Lead |
| **Review Authority** | Software Quality Assurance (SQA) / Peer Review |

**Description:** The Source Code is the C++17 implementation of the SynthFlight UAV Mission Simulation and Verification Platform. Code is compiled using GCC and is subject to coding standards (SCS-SF-2026-001), static analysis (CodeQL), and peer review. Source code is managed under configuration control via Git.

**DAL A Notes:** At DAL A, source code must be traceable to low-level requirements. Code must comply with the approved code standards. Code reviews must verify accuracy, conformance to design, and compliance with standards. All source code is a controlled configuration item.

---

### CDRL-SF-012 — Executable Object Code (EO)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-012 |
| **Data Item Title** | Executable Object Code |
| **Abbreviation** | EO |
| **DO-178C Reference** | Section 11.12 |
| **DID Reference** | DI-IPSC-81438 |
| **Document ID** | EO-SF-2026-001 |
| **File Path** | `build/release/` (build output) |
| **Status** | Not Started |
| **DAL A Applicability** | Required |
| **Current Version** | — |
| **Target Baseline Date** | 2026-11-01 |
| **Responsible Role** | Software Build Engineer |
| **Review Authority** | Software Configuration Manager |

**Description:** The Executable Object Code is the linked, compiled output of the SynthFlight source code produced by the GCC toolchain. The EO includes all object files, libraries, and the final executable image. Build artifacts are produced via CI/CD (GitHub Actions) with reproducible build procedures and are placed under configuration control upon baselining.

**DAL A Notes:** At DAL A, the EO must be traceable to the source code from which it was built. The compilation and linking environment must be documented in the SECI. Compiler options and linker settings are configuration-controlled items.

---

### CDRL-SF-013 — Software Verification Cases and Procedures (SVCP)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-013 |
| **Data Item Title** | Software Verification Cases and Procedures |
| **Abbreviation** | SVCP |
| **DO-178C Reference** | Section 11.13 |
| **DID Reference** | DI-IPSC-81439 |
| **Document ID** | SVCP-SF-2026-001 |
| **File Path** | `tests/` (Google Test, Catch2, Doctest, Boost.Test, CppUnit, libFuzzer) |
| **Status** | Draft |
| **DAL A Applicability** | Required with Independence |
| **Current Version** | 0.3 |
| **Target Baseline Date** | 2026-09-15 |
| **Responsible Role** | Software Verification Lead |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The SVCP defines the verification cases (what to verify) and procedures (how to verify) for SynthFlight. Test cases are implemented using Google Test, Catch2, Doctest, Boost.Test, and CppUnit. Fuzz testing procedures use libFuzzer. The SVCP covers requirements-based testing, robustness testing, structural coverage analysis (MC/DC), and integration testing.

**DAL A Notes:** At DAL A, verification cases and procedures require independence between development and verification personnel for certain objectives. MC/DC coverage is required. Normal-range, robustness, and boundary-value test cases must be defined. Test procedures must be repeatable and traceable to requirements.

---

### CDRL-SF-014 — Software Verification Results (SVR)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-014 |
| **Data Item Title** | Software Verification Results |
| **Abbreviation** | SVR |
| **DO-178C Reference** | Section 11.14 |
| **DID Reference** | DI-IPSC-81440 |
| **Document ID** | SVR-SF-2026-001 |
| **File Path** | `docs/verification/SVR-SynthFlight-Increment1.md` and `docs/svr/` |
| **Status** | Draft |
| **DAL A Applicability** | Required with Independence |
| **Current Version** | 0.2 |
| **Target Baseline Date** | 2026-11-15 |
| **Responsible Role** | Software Verification Lead |
| **Review Authority** | Software Quality Assurance (SQA) / DER |

**Description:** The SVR contains the results of all verification activities for SynthFlight, including test results, review records, analysis results, and structural coverage results. It documents pass/fail status of all test cases, MC/DC coverage metrics, static analysis findings, fuzz testing outcomes, and any problem reports generated during verification.

**DAL A Notes:** At DAL A, verification results require independent review. Results must demonstrate 100% requirements-based test coverage and MC/DC structural coverage. Any coverage gaps must be justified with rationale. All verification results are configuration-controlled.

---

### CDRL-SF-015 — Software Life Cycle Environment Configuration Index (SECI)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-015 |
| **Data Item Title** | Software Life Cycle Environment Configuration Index |
| **Abbreviation** | SECI |
| **DO-178C Reference** | Section 11.15 |
| **DID Reference** | DI-IPSC-81442 |
| **Document ID** | SECI-SF-2026-001 |
| **File Path** | `docs/config/SECI-SynthFlight.md` |
| **Status** | Not Started |
| **DAL A Applicability** | Required |
| **Current Version** | — |
| **Target Baseline Date** | 2026-11-15 |
| **Responsible Role** | Software Configuration Manager |
| **Review Authority** | Software Quality Assurance (SQA) |

**Description:** The SECI identifies the software life cycle environment used for SynthFlight development and verification, including the hardware platforms, operating systems, compilers (GCC), linkers, test frameworks (Google Test, Catch2, Doctest, Boost.Test, CppUnit, libFuzzer), static analysis tools (CodeQL), CI/CD platform (GitHub Actions), and any qualified tools (per DO-330). It includes version numbers, configuration settings, and qualification status.

**DAL A Notes:** At DAL A, the SECI must identify all tools that could introduce errors into the software (development tools) and all tools used to verify the software (verification tools). Tool qualification levels per DO-330 must be documented.

---

### CDRL-SF-016 — Software Configuration Index (SCI)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-016 |
| **Data Item Title** | Software Configuration Index |
| **Abbreviation** | SCI |
| **DO-178C Reference** | Section 11.16 |
| **DID Reference** | DI-IPSC-81443 |
| **Document ID** | SCI-SF-2026-001 |
| **File Path** | `docs/config/SCI-SynthFlight.md` |
| **Status** | Not Started |
| **DAL A Applicability** | Required |
| **Current Version** | — |
| **Target Baseline Date** | 2026-12-01 |
| **Responsible Role** | Software Configuration Manager |
| **Review Authority** | Software Quality Assurance (SQA) / DER |

**Description:** The SCI identifies the configuration of the software product at the time of certification. It lists all configuration items, their versions, and their relationships, forming the definitive index of the certified SynthFlight software product. It includes references to source code baselines, executable object code, and all associated life cycle data documents.

**DAL A Notes:** At DAL A, the SCI must include identification of all software life cycle data items, source code, executable object code, and previously developed software. The SCI is a primary input to the final certification review.

---

### CDRL-SF-017 — Software Accomplishment Summary (SAS)

| Field | Value |
|-------|-------|
| **CDRL Number** | CDRL-SF-017 |
| **Data Item Title** | Software Accomplishment Summary |
| **Abbreviation** | SAS |
| **DO-178C Reference** | Section 11.17 |
| **DID Reference** | DI-MISC-81467 |
| **Document ID** | SAS-SF-2026-001 |
| **File Path** | `docs/certification/SAS-SynthFlight.md` |
| **Status** | Not Started |
| **DAL A Applicability** | Required with Independence |
| **Current Version** | — |
| **Target Baseline Date** | 2026-12-15 |
| **Responsible Role** | Software Quality Assurance Manager |
| **Review Authority** | DER / Certification Authority |

**Description:** The SAS is a summary document that provides the certification authority with assurance that the SynthFlight software life cycle processes were conducted in compliance with the approved plans and DO-178C objectives for DAL A. It summarizes compliance status, identifies all open problem reports, describes deviations from plans, and declares the software conformance status.

**DAL A Notes:** At DAL A, the SAS must be prepared with SQA independence. It must summarize compliance with each applicable DO-178C objective (Tables A-1 through A-10), identify all deviations and their resolutions, and provide the final compliance matrix. The SAS is a primary document for the final Stage of Involvement review.

---

## 6. CDRL Delivery Schedule by Program Milestone

The following table maps each CDRL to the program milestones at which it must be available for review or delivery. Statuses at each milestone are indicated as:

- **B** = Baselined (approved, under change control)
- **D** = Draft (available for review, not yet baselined)
- **U** = Updated (revised from prior baseline)
- **N/A** = Not applicable at this milestone

### 6.1 Milestone Definitions

| Milestone | Name | Target Date | Description |
|-----------|------|-------------|-------------|
| **PDR** | Preliminary Design Review | 2026-06-01 | Review of plans, standards, and preliminary requirements |
| **CDR** | Critical Design Review | 2026-09-01 | Review of detailed design, requirements baseline, test planning |
| **TRR** | Test Readiness Review | 2026-10-15 | Review of verification readiness, test cases and procedures |
| **SOI 1** | Stage of Involvement 1 | 2026-05-30 | Certification authority review of planning documents |
| **SOI 2** | Stage of Involvement 2 | 2026-08-30 | Certification authority review of development data |
| **SOI 3** | Stage of Involvement 3 | 2026-11-30 | Certification authority review of verification data |
| **SOI 4** | Stage of Involvement 4 | 2026-12-15 | Final certification authority review |

### 6.2 Delivery Matrix

| CDRL No. | Abbr. | PDR | SOI 1 | CDR | SOI 2 | TRR | SOI 3 | SOI 4 |
|----------|-------|-----|-------|-----|-------|-----|-------|-------|
| CDRL-SF-001 | PSAC | D | B | B | B | B | B | B |
| CDRL-SF-002 | SDP | D | B | B | B | B | B | B |
| CDRL-SF-003 | SVP | D | B | B | B | B | B | B |
| CDRL-SF-004 | SCMP | D | B | B | B | B | B | B |
| CDRL-SF-005 | SQAP | D | B | B | B | B | B | B |
| CDRL-SF-006 | SRS-STD | D | B | B | B | B | B | B |
| CDRL-SF-007 | SDS-STD | D | B | B | B | B | B | B |
| CDRL-SF-008 | SCS-STD | D | B | B | B | B | B | B |
| CDRL-SF-009 | SRD | D | D | B | B | B | B | B |
| CDRL-SF-010 | SDD | N/A | N/A | D | B | B | B | B |
| CDRL-SF-011 | SC | N/A | N/A | D | D | B | B | B |
| CDRL-SF-012 | EO | N/A | N/A | N/A | N/A | D | B | B |
| CDRL-SF-013 | SVCP | N/A | N/A | D | D | B | B | B |
| CDRL-SF-014 | SVR | N/A | N/A | N/A | N/A | D | B | B |
| CDRL-SF-015 | SECI | N/A | N/A | D | D | U | B | B |
| CDRL-SF-016 | SCI | N/A | N/A | N/A | N/A | N/A | D | B |
| CDRL-SF-017 | SAS | N/A | N/A | N/A | N/A | N/A | D | B |

### 6.3 Milestone Delivery Summary

**PDR (2026-06-01):**
All five plans (PSAC, SDP, SVP, SCMP, SQAP) and three standards (SRS-STD, SDS-STD, SCS-STD) must be in Draft status. Software Requirements Data (SRD) must be available in preliminary draft form. Total: 9 items in Draft.

**SOI 1 (2026-05-30):**
All plans and standards must be Baselined. The PSAC is the primary document reviewed by the certification authority. Software Requirements Data should be available in Draft. Total: 8 items Baselined, 1 item in Draft.

**CDR (2026-09-01):**
Software Requirements Data must be Baselined. Software Design Description, Source Code, Verification Cases and Procedures, and SECI must be available in Draft. Total: 9 items Baselined, 4 items in Draft.

**SOI 2 (2026-08-30):**
Software Design Description must be Baselined. Requirements data, design, and development evidence are reviewed. Total: 10 items Baselined, 3 items in Draft.

**TRR (2026-10-15):**
Source Code and Verification Cases and Procedures must be Baselined. Executable Object Code, Verification Results, and SECI should be available in Draft. Total: 12 items Baselined, 3 items in Draft.

**SOI 3 (2026-11-30):**
Executable Object Code, Verification Results, and SECI must be Baselined. SCI and SAS must be available in Draft. Total: 15 items Baselined, 2 items in Draft.

**SOI 4 (2026-12-15):**
All 17 items must be Baselined. The SAS and SCI are the final items baselined. The certification authority performs the final review. Total: 17 items Baselined.

---

## 7. Status Definitions

| Status | Definition |
|--------|------------|
| **Not Started** | Work on the data item has not yet begun. |
| **Draft** | Initial content exists and is under active development. Not yet formally reviewed. |
| **In-Review** | Content is complete and undergoing formal review per the applicable review checklist. |
| **Baselined** | Formally reviewed, approved, and placed under configuration control. Changes require a formal change request. |

---

## 8. DAL A Applicability Key

| Applicability | Meaning |
|---------------|---------|
| **Required** | The data item is required at DAL A. Standard development and review processes apply. |
| **Required with Independence** | The data item is required at DAL A and certain objectives associated with this item require independence between the person performing the activity and the person verifying or reviewing it. At DAL A, this typically means SQA or an independent verification team must review the item. |

---

## 9. DID Reference Summary

| DID Number | Title | Applicable CDRL(s) |
|------------|-------|---------------------|
| DI-MISC-81466 | Plan for Software Aspects of Certification | CDRL-SF-001 |
| DI-IPSC-81427 | Software Development Plan | CDRL-SF-002 |
| DI-IPSC-81429 | Software Verification Plan | CDRL-SF-003 |
| DI-IPSC-81441 | Software Configuration Management Plan | CDRL-SF-004 |
| DI-QCIC-80572 | Software Quality Assurance Plan | CDRL-SF-005 |
| DI-IPSC-81431 | Software Requirements Standards | CDRL-SF-006 |
| DI-IPSC-81432 | Software Design Standards | CDRL-SF-007 |
| DI-IPSC-81433 | Software Code Standards | CDRL-SF-008 |
| DI-IPSC-81430 | Software Requirements Data | CDRL-SF-009 |
| DI-IPSC-81435 | Software Design Description | CDRL-SF-010 |
| DI-IPSC-81434 | Source Code | CDRL-SF-011 |
| DI-IPSC-81438 | Executable Object Code | CDRL-SF-012 |
| DI-IPSC-81439 | Software Verification Cases and Procedures | CDRL-SF-013 |
| DI-IPSC-81440 | Software Verification Results | CDRL-SF-014 |
| DI-IPSC-81442 | Software Life Cycle Environment Configuration Index | CDRL-SF-015 |
| DI-IPSC-81443 | Software Configuration Index | CDRL-SF-016 |
| DI-MISC-81467 | Software Accomplishment Summary | CDRL-SF-017 |

---

## 10. Revision History

| Version | Date | Author | Description |
|---------|------|--------|-------------|
| 1.0 | 2026-03-14 | Software Quality Assurance | Initial CDRL Register creation |
| 1.1 | 2026-03-15 | Software Quality Assurance | Plans/standards baselined, doc ID and file path corrections, SRS count 149→119, SVCP/SVR status updated to Draft, SCMP rev bumped for TAR-SF-004 |

---

*End of CDRL Register — CDRL-SF-2026-001*
