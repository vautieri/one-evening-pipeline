# Software Quality Assurance Plan

**Document ID:** SQAP-SF-2026-001
**Project:** SynthFlight — UAV Mission Simulation and Verification Platform
**DAL:** A (Catastrophic) — DO-178C
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Baselined

---

## Revision History

| Version | Date       | Author       | Description           |
|---------|------------|--------------|-----------------------|
| 1.0     | 2026-03-14 | SynthFlight  | Initial release       |
| 1.1     | 2026-03-15 | SynthFlight  | Status changed to Baselined per Pre-Implementation Gate audit |

---

## Table of Contents

1. [Purpose and Scope](#1-purpose-and-scope)
2. [Reference Documents](#2-reference-documents)
3. [QA Organization](#3-qa-organization)
4. [QA Authority](#4-qa-authority)
5. [Software Life Cycle Process Assurance](#5-software-life-cycle-process-assurance)
6. [Transition Criteria Assurance](#6-transition-criteria-assurance)
7. [Software Conformity Review](#7-software-conformity-review)
8. [Problem Reporting Assurance](#8-problem-reporting-assurance)
9. [QA Records](#9-qa-records)
10. [QA at Program Milestones](#10-qa-at-program-milestones)
11. [Supplier/COTS Oversight](#11-suppliercots-oversight)
12. [LLM-Assisted Development QA](#12-llm-assisted-development-qa)
13. [Non-Conformance Handling](#13-non-conformance-handling)
14. [Corrective Action Process](#14-corrective-action-process)

---

## 1. Purpose and Scope

### 1.1 Purpose

This Software Quality Assurance Plan (SQAP) defines the quality assurance activities, organizational authority, and oversight procedures for the SynthFlight UAV Mission Simulation and Verification Platform. The plan establishes the methods by which software quality assurance provides confidence that the software life cycle processes, outputs, and the resulting software product conform to applicable plans, standards, and DO-178C objectives.

This plan satisfies the software quality assurance objectives defined in DO-178C, Section 8 — Software Quality Assurance Process, and implements the heightened independence and rigor required for Design Assurance Level (DAL) A.

### 1.2 Scope

This SQAP governs quality assurance activities across all phases of the SynthFlight software life cycle:

- Software planning
- Software requirements development
- Software design
- Software coding
- Integration
- Software verification (testing, analysis, reviews)
- Software configuration management
- Certification liaison

QA oversight applies to all software life cycle data, including source code, test code, requirements, design documents, plans, standards, verification results, traceability data, and configuration management records.

### 1.3 DAL A Implications for Quality Assurance

DO-178C DAL A imposes the most stringent quality assurance requirements. Key implications include:

- **Independence:** QA personnel shall be independent from the development team. The individual performing QA review of a work product shall not be the same individual who produced that work product. This independence requirement is non-negotiable for DAL A.
- **Completeness of Coverage:** All DO-178C Table A-9 objectives are applicable, and all QA activities must cover every applicable objective.
- **Heightened Scrutiny:** QA shall apply additional scrutiny to safety-related requirements, derived requirements, and deactivated code.
- **All Objectives with Independence:** For DAL A, all objectives in DO-178C Annex A tables that are marked with independence shall have that independence provided.

---

## 2. Reference Documents

| Document ID        | Title                                                        | Version |
|--------------------|--------------------------------------------------------------|---------|
| DO-178C            | Software Considerations in Airborne Systems and Equipment Certification, Section 8 | Rev C   |
| DO-330             | Software Tool Qualification Considerations                   | —       |
| PSAC-SF-2026-001   | Plan for Software Aspects of Certification — SynthFlight     | 1.0     |
| SDP-SF-2026-001    | Software Development Plan — SynthFlight                      | 1.0     |
| SVP-SF-2026-001    | Software Verification Plan — SynthFlight                     | 1.0     |
| SCMP-SF-2026-001   | Software Configuration Management Plan — SynthFlight         | 1.0     |
| SRS-STD-SF-2026    | Software Requirements Standards — SynthFlight                | 1.0     |
| SDS-STD-SF-2026    | Software Design Standards — SynthFlight                      | 1.0     |
| SCS-STD-SF-2026    | Software Code Standards — SynthFlight (C++17, GCC)           | 1.0     |

---

## 3. QA Organization

### 3.1 Organizational Independence

For DAL A, the QA function shall be organizationally independent from the software development team. This independence ensures that QA can objectively evaluate work products, processes, and compliance without conflict of interest.

#### 3.1.1 Independence Requirements

| Principle                          | Requirement                                                   |
|------------------------------------|---------------------------------------------------------------|
| Personnel Independence             | QA reviewers shall not be the developers of the artifacts they review |
| Reporting Independence             | QA shall report to project management independently from the software development lead |
| Authority Independence             | QA shall have the authority to raise findings without requiring development approval |
| Financial Independence             | QA budget and resource allocation shall not be controlled by the development lead |

#### 3.1.2 QA Roles and Responsibilities

| Role                  | Responsibilities                                                                      |
|-----------------------|---------------------------------------------------------------------------------------|
| QA Manager            | Overall QA program management, resource allocation, QA reporting to project management, escalation authority |
| QA Engineer           | Process audits, conformity reviews, standards compliance checks, audit record maintenance |
| QA Analyst            | DR process monitoring, metrics collection and analysis, trend reporting               |
| Independent Reviewer  | Code review for SCS compliance, test review for SVP compliance, document review for completeness |

#### 3.1.3 Independence Matrix

The following matrix defines who may perform QA activities on various work products:

| Work Product           | Produced By        | QA Reviewed By       | Independence Satisfied |
|------------------------|--------------------|----------------------|------------------------|
| Source Code             | Developer A        | QA Engineer + Developer B (not A) | Yes |
| Test Code              | Developer B        | QA Engineer + Developer A (not B) | Yes |
| Requirements           | Requirements Eng.  | QA Engineer + Independent Reviewer | Yes |
| Design Documents       | Software Lead      | QA Engineer + Independent Reviewer | Yes |
| Plans                  | Project Lead       | QA Manager + Independent Reviewer  | Yes |
| Verification Results   | Verification Lead  | QA Engineer                         | Yes |

### 3.2 QA Training

All QA personnel shall be trained in:

- DO-178C objectives and processes (with emphasis on Section 8)
- SynthFlight-specific plans and standards (SDP, SVP, SCMP, SQAP, SCS, SDS, SRS-Std)
- Configuration management tools and procedures (Git, GitHub, GitHub Actions)
- C++17 coding standards (sufficient to perform SCS compliance reviews)
- Project-specific test frameworks (Google Test, Catch2, Doctest, Boost.Test, CppUnit)
- Problem reporting and change control procedures

Training records shall be maintained as QA records (Section 9).

---

## 4. QA Authority

### 4.1 Authority to Halt Releases

The QA Manager has the authority to halt any software release if:

- Unresolved non-conformances exist that affect product safety or certification
- Required verification activities are incomplete
- Configuration management processes have not been followed
- Baseline integrity has been compromised
- Open Critical or Major Discrepancy Reports exist without proper disposition
- Required audits (FCA, PCA) have not been satisfactorily completed
- Any DO-178C objective applicable to DAL A has not been met

### 4.2 Authority to Escalate

QA has direct escalation authority to:

| Escalation Level | Condition                                                    |
|------------------|--------------------------------------------------------------|
| Project Lead     | Any non-conformance not resolved within 5 business days      |
| Program Manager  | Systemic process failures or repeated non-conformances       |
| Certification Authority Liaison | Safety-related non-conformances, potential certification impact |

### 4.3 Authority to Participate

QA shall have the right to:

- Attend all technical reviews and program milestones
- Access all software life cycle data and configuration items
- Attend all CRB meetings with voting rights on process compliance matters
- Review all Pull Requests prior to merge into protected branches
- Request re-verification when changes may affect previously verified functionality
- Audit any development, verification, or CM activity without prior notice

### 4.4 Non-Overridable QA Decisions

The following QA decisions cannot be overridden without formal escalation to the Program Manager:

- QA hold on a release
- QA rejection of a baseline for non-conformance
- QA finding that a DO-178C objective has not been met
- QA determination that independence requirements have been violated

---

## 5. Software Life Cycle Process Assurance

QA shall verify that all software life cycle processes are conducted in accordance with the approved plans and standards. The following process areas are subject to QA assurance activities.

### 5.1 SDP Compliance Audits

QA shall audit the software development process for compliance with the Software Development Plan (SDP-SF-2026-001):

| Audit Area                   | Verification Method | Frequency                  |
|------------------------------|---------------------|----------------------------|
| Requirements process followed | Process audit       | Per requirements phase      |
| Design process followed       | Process audit       | Per design phase            |
| Coding standards applied       | Code review sample  | Per integration (continuous) |
| Integration process followed   | Process audit       | Per integration cycle       |
| Derived requirements identified | Review              | Per requirements update     |
| Traceability maintained        | Traceability audit  | Monthly                     |

#### 5.1.1 SDP Audit Checklist

- [ ] Requirements are documented per SRS-Standard
- [ ] Design is documented per SDS-Standard
- [ ] Code follows SCS (C++17, GCC)
- [ ] Derived requirements are flagged and communicated to the system process
- [ ] Traceability is maintained from requirements through design to code
- [ ] Reviews are conducted at each development phase transition

### 5.2 SVP Compliance Audits

QA shall audit the software verification process for compliance with the Software Verification Plan (SVP-SF-2026-001):

| Audit Area                        | Verification Method      | Frequency          |
|-----------------------------------|--------------------------|--------------------|
| Test cases trace to requirements   | Traceability review      | Per test cycle     |
| Test procedures follow SVP         | Procedure review         | Per test cycle     |
| Coverage analysis performed        | Results review           | Per test cycle     |
| Independence of verification       | Personnel audit          | Per test cycle     |
| Test environment documented        | Environment review       | Per baseline       |
| Test results recorded properly     | Results audit            | Per test execution |

#### 5.2.1 SVP Audit Checklist

- [ ] All requirements have associated test cases
- [ ] Test cases are executed per documented procedures
- [ ] Statement coverage meets DAL A threshold
- [ ] Decision coverage (MC/DC) meets DAL A threshold
- [ ] Verification of verification results by independent personnel
- [ ] Test anomalies are reported as DRs
- [ ] Regression testing performed after changes

### 5.3 SCMP Compliance Audits

QA shall audit the configuration management process for compliance with the Software Configuration Management Plan (SCMP-SF-2026-001):

| Audit Area                    | Verification Method  | Frequency          |
|-------------------------------|----------------------|--------------------|
| Baselines properly established | Baseline review      | Per baseline       |
| Change control followed        | CR/DR process audit  | Monthly            |
| Version control procedures     | Repository audit     | Monthly            |
| Branch protection enforced     | Settings review      | Quarterly          |
| Build reproducibility          | Rebuild test         | Per baseline       |
| Archive integrity              | Archive verification | Per baseline       |

#### 5.3.1 SCMP Audit Checklist

- [ ] All configuration items are identified and under version control
- [ ] Baselines are established per defined criteria
- [ ] Changes to baselined items follow the CR process
- [ ] DRs are filed for all identified problems
- [ ] CRB reviews and approves changes per defined procedures
- [ ] Branch protection rules are active and enforced
- [ ] CI/CD pipelines execute successfully for all merges
- [ ] Build can be reproduced from archived baseline

### 5.4 Standards Compliance

QA shall verify compliance with the following standards:

| Standard           | Compliance Verification Method           | Frequency           |
|--------------------|------------------------------------------|---------------------|
| SRS-Standard       | Requirements review against standard     | Per requirements update |
| SDS (Design Std.)  | Design review against standard           | Per design update   |
| SCS (Code Std.)    | Code review + static analysis (CodeQL)   | Per PR / continuous |

---

## 6. Transition Criteria Assurance

### 6.1 Phase Gate Verification

QA shall verify that entry and exit criteria are met at each software life cycle phase transition. No phase transition shall occur without QA sign-off.

### 6.2 Phase Transition Criteria

#### 6.2.1 Planning to Requirements Phase

**Entry Criteria:**
- [ ] All plans approved (SDP, SVP, SCMP, SQAP, PSAC)
- [ ] Standards baselined (SRS-Std, SDS-Std, SCS)
- [ ] Development environment established and documented
- [ ] CM environment operational (Git, GitHub, CI/CD pipelines)
- [ ] QA personnel assigned and trained

**Exit Criteria:**
- [ ] Software requirements documented per SRS-Standard
- [ ] Requirements reviewed and approved
- [ ] Derived requirements identified and communicated
- [ ] Requirements traceability to system requirements established

#### 6.2.2 Requirements to Design Phase

**Entry Criteria:**
- [ ] Software requirements baselined
- [ ] Requirements review findings resolved
- [ ] No open Critical DRs against requirements

**Exit Criteria:**
- [ ] Software architecture and detailed design documented per SDS-Standard
- [ ] Design traces to requirements (complete traceability)
- [ ] Design review completed
- [ ] Design review findings resolved

#### 6.2.3 Design to Coding Phase

**Entry Criteria:**
- [ ] Design baselined
- [ ] Design review findings resolved
- [ ] No open Critical DRs against design
- [ ] SCS (coding standards) baselined

**Exit Criteria:**
- [ ] Source code implemented per SCS
- [ ] Code review completed per SCS compliance checklist
- [ ] Code traces to design (complete traceability)
- [ ] Unit tests developed for all code modules
- [ ] All code compiles without errors or warnings

#### 6.2.4 Coding to Integration Phase

**Entry Criteria:**
- [ ] Source code reviewed and approved
- [ ] Unit tests pass for all modules
- [ ] Code coverage meets intermediate thresholds
- [ ] No open Critical DRs against code

**Exit Criteria:**
- [ ] Integrated software builds successfully
- [ ] Integration tests pass
- [ ] Integration test results documented

#### 6.2.5 Integration to Verification Phase

**Entry Criteria:**
- [ ] Integrated build stable
- [ ] Integration test results reviewed
- [ ] Test procedures documented per SVP

**Exit Criteria:**
- [ ] All verification activities completed per SVP
- [ ] Coverage analysis meets DAL A thresholds
- [ ] All verification results documented
- [ ] All test anomalies reported as DRs and dispositioned

#### 6.2.6 Verification to Release Phase

**Entry Criteria:**
- [ ] All verification objectives met
- [ ] No open Critical or Major DRs
- [ ] FCA and PCA completed
- [ ] SCI and SAS drafted

**Exit Criteria:**
- [ ] Product baseline established
- [ ] SCI finalized and approved
- [ ] SAS finalized and approved
- [ ] QA conformity review passed
- [ ] CRB release approval obtained

### 6.3 QA Phase Gate Sign-Off

QA shall provide written sign-off at each phase gate. The sign-off record shall include:

- Phase transition identifier
- Date of QA review
- Entry/exit criteria evaluation (pass/fail for each criterion)
- List of any open findings or waivers
- QA recommendation (proceed / hold / proceed with conditions)
- QA reviewer signature

---

## 7. Software Conformity Review

### 7.1 Code Reviews

QA shall ensure that all source code is reviewed for conformance to the Software Code Standards (SCS-STD-SF-2026, C++17 / GCC). Code reviews are conducted via GitHub Pull Requests and shall be completed before any merge to a protected branch.

#### 7.1.1 SCS Compliance Review Checklist

The following checklist items shall be evaluated during every code review:

**Naming and Style:**
- [ ] Class names use PascalCase
- [ ] Function names use camelCase or snake_case per project convention
- [ ] Variable names are descriptive and follow naming convention
- [ ] Constants use UPPER_SNAKE_CASE
- [ ] File names match the primary class/module they contain
- [ ] Namespace usage follows project conventions

**Prohibited Practices:**
- [ ] No magic numbers — all numeric constants are named constants with documented units
- [ ] No `goto` statements
- [ ] No implicit type conversions that may lose data
- [ ] No use of `reinterpret_cast` without documented justification
- [ ] No dynamic memory allocation in safety-critical paths (where defined by design)
- [ ] No compiler-specific extensions beyond documented exceptions
- [ ] No use of `using namespace std;` in header files

**Floating-Point Handling:**
- [ ] Floating-point comparisons use epsilon-based tolerance, never direct equality (`==`)
- [ ] Floating-point types are explicitly specified (`float` vs. `double`) with documented rationale
- [ ] Potential overflow/underflow conditions are guarded
- [ ] Division by zero is guarded
- [ ] NaN and infinity checks are implemented where applicable
- [ ] Numeric precision requirements are documented in comments referencing the requirement

**Safety and Robustness:**
- [ ] All switch statements have a `default` case
- [ ] All array/container accesses are bounds-checked
- [ ] Error handling is explicit (return codes or exceptions per project convention)
- [ ] Resource acquisition follows RAII pattern
- [ ] Smart pointers used instead of raw pointers for ownership
- [ ] Thread safety considerations documented for shared data

**C++17 Specific:**
- [ ] `std::optional` used for values that may not be present
- [ ] `[[nodiscard]]` attribute applied to functions where ignoring the return value is an error
- [ ] `constexpr` used where compile-time evaluation is possible and appropriate
- [ ] Structured bindings used appropriately
- [ ] `if constexpr` used instead of SFINAE where applicable

**Documentation:**
- [ ] All public functions have Doxygen-style documentation
- [ ] Complex algorithms have explanatory comments referencing the design or requirement
- [ ] Units are documented in comments for all physical quantities
- [ ] Assumptions and preconditions are documented

#### 7.1.2 Code Review Process

1. Developer submits a Pull Request to `develop` or `main`
2. At least one independent reviewer (not the author) reviews for technical correctness
3. QA Engineer or designated QA reviewer reviews for SCS compliance using the checklist above
4. CodeQL static analysis results are reviewed for any new findings
5. All CI checks pass (`ci.yml`, `codeql.yml`, `dependency-review.yml`, `fuzz.yml`)
6. All review comments are resolved
7. Required approvals obtained (1 for `develop`, 2 for `main`)
8. QA records the review outcome

### 7.2 Test Reviews

QA shall review test artifacts for compliance with the SVP and for adequacy of verification.

#### 7.2.1 Traceability Verification

- [ ] Every software requirement has at least one test case that verifies it
- [ ] Every test case traces to at least one software requirement
- [ ] Traceability matrix is current and reviewed
- [ ] Negative test cases exist for boundary conditions and error scenarios
- [ ] Test cases for derived requirements are identified

#### 7.2.2 Coverage Adequacy

- [ ] Statement coverage meets DAL A threshold (per SVP)
- [ ] Decision coverage meets DAL A threshold (per SVP)
- [ ] MC/DC coverage meets DAL A threshold (per SVP)
- [ ] Coverage gaps are analyzed and justified or additional tests created
- [ ] Dead code and deactivated code are identified and justified
- [ ] lcov reports are generated and archived with each test baseline

#### 7.2.3 Test Quality

- [ ] Test cases are deterministic (no random/non-deterministic behavior without seed control)
- [ ] Test cases are independent (no inter-test dependencies)
- [ ] Test assertions are specific and meaningful
- [ ] Test failure messages clearly identify the failing condition
- [ ] Fuzz test harnesses (libFuzzer) cover security-sensitive and input-parsing code

### 7.3 Documentation Reviews

QA shall review all life cycle documents for completeness per the Contract Data Requirements List (CDRL) and DO-178C data item descriptions.

#### 7.3.1 Documentation Completeness Checklist

- [ ] Document has a unique identifier and version
- [ ] Document has a revision history
- [ ] All sections required by DO-178C or the CDRL are present
- [ ] Cross-references to other documents are correct and current
- [ ] Terminology is consistent with the project glossary
- [ ] Document has been reviewed and approved by the designated authority
- [ ] Document is under configuration management (per SCMP)

---

## 8. Problem Reporting Assurance

### 8.1 DR Process Monitoring

QA shall monitor the Discrepancy Report (DR) process defined in the SCMP to ensure:

- All identified problems are reported as DRs
- DRs are properly categorized by severity (Critical / Major / Minor) and category
- DRs are assigned and worked in a timely manner
- DR resolutions are technically adequate
- Closed DRs have been verified (fix confirmed)
- DR process metrics are collected and trended

### 8.2 DR Metrics and Reporting

QA shall produce the following DR-related reports:

| Report                  | Content                                           | Frequency |
|-------------------------|---------------------------------------------------|-----------|
| DR Status Summary       | Open/closed counts by severity and category       | Weekly    |
| DR Aging Report         | DRs exceeding target resolution times             | Weekly    |
| DR Trend Analysis       | Discovery rate, closure rate, backlog trends       | Monthly   |
| DR Severity Distribution| Distribution of DRs by severity over time         | Monthly   |
| Escaped Defect Analysis | Defects found in later phases that should have been caught earlier | Per phase |

### 8.3 DR Process Compliance Checks

QA shall verify:

- [ ] All DRs have required fields completed (per SCMP Section 6.1.2)
- [ ] DRs follow the defined life cycle states (per SCMP Section 6.1.4)
- [ ] Critical DRs are escalated per defined timelines
- [ ] DR closures include verification evidence
- [ ] No problems are resolved outside the DR process ("side-channel fixes")
- [ ] CRB reviews all CRs associated with DRs per defined procedures

---

## 9. QA Records

### 9.1 Records to be Maintained

QA shall create and maintain the following records throughout the SynthFlight project:

| Record Type                   | Description                                                | Retention       |
|-------------------------------|------------------------------------------------------------|-----------------|
| Process Audit Reports         | Results of SDP, SVP, SCMP compliance audits               | Life of type + 2 years |
| Conformity Review Records     | Code review, test review, documentation review results     | Life of type + 2 years |
| Non-Conformance Reports (NCR) | Documented QA findings of non-conformance                  | Life of type + 2 years |
| Corrective Action Records     | Root cause analysis, corrective actions, verification      | Life of type + 2 years |
| Phase Gate Sign-Off Records   | QA transition criteria verification results                | Life of type + 2 years |
| Milestone Audit Records       | SRR, PDR, CDR, TRR, SOI audit results                     | Life of type + 2 years |
| QA Meeting Minutes            | Records of QA-related discussions and decisions            | Life of type + 2 years |
| Training Records              | QA personnel training and qualification evidence           | Life of type + 2 years |
| DR Monitoring Reports         | Weekly/monthly DR status and trend reports                 | Life of type + 2 years |
| Supplier Oversight Records    | Assessment and monitoring records for COTS/suppliers       | Life of type + 2 years |
| LLM QA Assessment Records     | QA reviews of AI-generated artifacts                       | Life of type + 2 years |

### 9.2 Record Storage and Access

- QA records shall be stored in the project repository under `docs/qa/` or in the designated project records management system
- QA records are configuration items subject to the SCMP
- Access to QA records shall be available to all project personnel in a read-only capacity
- QA records shall be included in baseline archives

### 9.3 Record Format

All QA records shall include:

- Record identifier (unique ID)
- Date of the activity
- Personnel involved (names and roles)
- Description of the activity performed
- Findings and observations
- Disposition (conforming / non-conforming / observation)
- Required actions (if any)
- Follow-up status
- QA reviewer signature/approval

---

## 10. QA at Program Milestones

### 10.1 System Requirements Review (SRR) Checklist

QA shall participate in the SRR and verify:

- [ ] System requirements allocated to software are documented
- [ ] Software requirements are derived from and traceable to system requirements
- [ ] Software requirements conform to the SRS-Standard
- [ ] Each software requirement is verifiable (testable)
- [ ] Derived requirements are identified and flagged
- [ ] Requirements are unambiguous, consistent, and complete
- [ ] Safety-related requirements are identified
- [ ] Interface requirements are fully defined
- [ ] Performance requirements include measurable criteria
- [ ] The SDP, SVP, SCMP, and SQAP are approved

### 10.2 Preliminary Design Review (PDR) Checklist

QA shall participate in the PDR and verify:

- [ ] Software architecture is documented per SDS-Standard
- [ ] Architecture traces to software requirements
- [ ] All software requirements are addressed by the design
- [ ] Design partitioning and modularity are adequate
- [ ] Interface definitions are complete and consistent
- [ ] Design addresses safety-related requirements with appropriate rigor
- [ ] Resource budgets (memory, CPU, timing) are established
- [ ] Error handling strategy is defined
- [ ] Derived requirements from the design are identified
- [ ] Design review action items from SRR are resolved
- [ ] CM baselines are established per SCMP

### 10.3 Critical Design Review (CDR) Checklist

QA shall participate in the CDR and verify:

- [ ] Detailed design is documented per SDS-Standard
- [ ] Detailed design traces to software architecture and requirements
- [ ] All software requirements are allocated to specific code modules
- [ ] Algorithm descriptions are complete with references
- [ ] Data structures are defined
- [ ] Data flow and control flow are documented
- [ ] Timing and sizing estimates are updated
- [ ] Design complexity is assessed and manageable
- [ ] Coding standards (SCS) are ready for application
- [ ] Test procedures and test cases are in development per SVP
- [ ] PDR action items are resolved

### 10.4 Test Readiness Review (TRR) Checklist

QA shall participate in the TRR and verify:

- [ ] All test cases are documented and trace to requirements
- [ ] Test procedures are complete and reviewed
- [ ] Test environment is established and documented
- [ ] Test tools are qualified per DO-330 (if applicable)
- [ ] Test data is prepared and validated
- [ ] Pass/fail criteria are defined for each test case
- [ ] Coverage analysis approach is defined and tools are operational
- [ ] Regression test suite is identified
- [ ] Test schedule is feasible
- [ ] CDR action items are resolved
- [ ] All frameworks operational: Google Test, Catch2, Doctest, Boost.Test, CppUnit, libFuzzer
- [ ] CI/CD pipeline executes all test suites successfully

### 10.5 Stage of Involvement (SOI) Audit Support

QA shall support SOI audits conducted by the certification authority or their delegates:

#### 10.5.1 SOI #1 — Planning Review

- Ensure all plans are available, approved, and baselined
- Demonstrate CM environment (Git, GitHub, CI/CD)
- Present QA organization and independence evidence

#### 10.5.2 SOI #2 — Development Review

- Demonstrate requirements and design process compliance
- Present traceability evidence
- Show code review and standards compliance records

#### 10.5.3 SOI #3 — Verification Review

- Present verification results and coverage analysis
- Demonstrate test environment and tool qualification evidence
- Show DR status and disposition evidence

#### 10.5.4 SOI #4 — Final Review

- Present SCI and SAS
- Demonstrate FCA and PCA results
- Show all open item dispositions
- Present conformity review summary

---

## 11. Supplier/COTS Oversight

### 11.1 JSBSim Integration QA

JSBSim (an open-source flight dynamics model) is integrated into SynthFlight. QA shall provide the following oversight for JSBSim integration:

#### 11.1.1 Assessment

- [ ] JSBSim version is identified and documented in the SCI
- [ ] JSBSim is obtained from its official source and integrity verified (hash comparison)
- [ ] JSBSim license terms are compatible with SynthFlight project requirements
- [ ] JSBSim interfaces to SynthFlight are clearly defined and documented
- [ ] JSBSim functionality used by SynthFlight is identified and documented

#### 11.1.2 Verification

- [ ] Integration tests verify JSBSim behavior within SynthFlight context
- [ ] JSBSim output values are validated against known reference data
- [ ] Boundary conditions at JSBSim interfaces are tested
- [ ] Error handling for JSBSim interface failures is tested
- [ ] Performance impact of JSBSim is measured and within budgets

#### 11.1.3 Ongoing Monitoring

- JSBSim version updates are managed through the change control process (SCMP Section 6)
- `dependabot.yml` monitors JSBSim for updates
- `dependency-review.yml` evaluates security implications of updates
- Any JSBSim update requires regression testing of all integration points

### 11.2 Third-Party Library Assessment

QA shall assess all third-party libraries used in SynthFlight:

| Library       | Assessment Areas                                                    |
|---------------|---------------------------------------------------------------------|
| Google Test   | Version pinned, no impact on target code, test-only dependency      |
| Catch2        | Version pinned, no impact on target code, test-only dependency      |
| Doctest       | Version pinned, no impact on target code, test-only dependency      |
| Boost.Test    | Version pinned, no impact on target code, test-only dependency      |
| CppUnit       | Version pinned, no impact on target code, test-only dependency      |
| libFuzzer     | Compiler-bundled, no impact on target code, test-only dependency    |

For test-only dependencies (those not linked into the deliverable executable), the assessment focuses on:

- [ ] The library does not affect the target code under test
- [ ] The library version is pinned and reproducible
- [ ] Known vulnerabilities are reviewed (via `dependency-review.yml`)

For libraries linked into the deliverable (if any are added in the future), full DO-178C COTS assessment criteria apply, including:

- Service history assessment
- Additional verification to cover COTS functionality used
- COTS problem reporting integration with SynthFlight DR process

### 11.3 Supplier QA Records

QA shall maintain records of all supplier/COTS assessments, including:

- Assessment date and reviewer
- Library name, version, and source
- Assessment results and any conditions of use
- Ongoing monitoring plan
- Re-assessment triggers (version change, vulnerability disclosure)

---

## 12. LLM-Assisted Development QA

### 12.1 Overview

SynthFlight permits the use of Large Language Model (LLM) tools to assist in software development activities, including code generation, test generation, documentation drafting, and requirements analysis. Due to the DAL A classification, LLM-assisted artifacts require special QA procedures to ensure safety, correctness, and compliance with DO-178C objectives.

### 12.2 Artifact Marking

All artifacts generated with LLM assistance shall be clearly identified:

- **Commit Messages:** Commits containing LLM-assisted content shall include the metadata field `LLM Assisted: Yes` in the commit message body
- **Pull Requests:** PRs containing LLM-assisted content shall include the label `llm-assisted` and a statement in the PR description identifying which artifacts were LLM-assisted
- **Inline Comments:** LLM-generated code blocks may include a comment `// LLM-generated` at the boundary, though this is not a substitute for commit/PR-level marking

QA shall verify that LLM-assisted artifacts are properly marked during conformity reviews.

### 12.3 Human Review Requirement

**For DAL A, human review of all LLM-generated artifacts is mandatory before merge.** This requirement provides the independence assurance required by DO-178C:

- The human reviewer shall be a qualified engineer (not a different LLM instance)
- The human reviewer shall evaluate the artifact against the same standards as human-generated artifacts (SCS for code, SRS-Std for requirements, SDS-Std for design)
- The human reviewer shall verify correctness, not merely plausibility
- The human review is documented in the PR review record

#### 12.3.1 LLM-Assisted Code Review Checklist (in addition to Section 7.1.1)

- [ ] Code logic is verified to be correct by a human reviewer
- [ ] Code does not contain hallucinated APIs, functions, or libraries
- [ ] Code does not contain plausible but incorrect algorithms
- [ ] Code does not introduce subtle behavioral differences from the requirement intent
- [ ] Code handles edge cases that LLMs commonly miss (overflow, underflow, null, empty)
- [ ] Generated unit tests actually test the stated requirement (not just appear to)
- [ ] Generated comments accurately describe the code behavior
- [ ] No sensitive data, model artifacts, or training data remnants are present

### 12.4 AI Tool Version Tracking

The following information shall be tracked for each LLM usage and recorded in the Software Environment Configuration Index (SECI):

| Field                | Description                                          | Example                    |
|----------------------|------------------------------------------------------|----------------------------|
| LLM Model            | Model name and version identifier                   | Claude Opus 4.6         |
| LLM Tool             | Tool/interface used to invoke the model              | Claude Code v1.x           |
| Tool Version          | Specific version of the tool                        | 1.0.0                      |
| Date of Use           | When the LLM was used                               | 2026-03-14                 |
| Artifacts Produced    | List of files or content generated                  | `SensorFusion_test.cpp`    |
| Prompt Summary        | High-level summary of what was requested             | "Generate unit tests for SensorFusion class" |

The SECI shall be updated whenever a new LLM tool or version is introduced into the development process.

### 12.5 Traceability of AI-Generated Tests

When LLMs generate test cases, human verification of requirement traceability is mandatory:

- [ ] Each LLM-generated test case is mapped to a specific software requirement by a human reviewer
- [ ] The mapping is verified to be correct (the test actually verifies the requirement, not just references it)
- [ ] LLM-generated tests do not create a false sense of coverage (testing trivial properties while missing the requirement intent)
- [ ] Coverage attributed to LLM-generated tests is validated through manual inspection of coverage reports

### 12.6 Prohibited LLM Uses

The following uses of LLMs are prohibited for DAL A without explicit CRB approval:

- Generating safety analysis or hazard assessment content
- Making disposition decisions on DRs
- Performing independent QA reviews (LLM output may inform but not replace human QA)
- Generating certification authority correspondence
- Making airworthiness determinations

---

## 13. Non-Conformance Handling

### 13.1 Non-Conformance Identification

A non-conformance is any deviation from approved plans, standards, or DO-178C objectives. Non-conformances may be identified through:

- Process audits (Section 5)
- Conformity reviews (Section 7)
- Phase gate reviews (Section 6)
- Milestone audits (Section 10)
- DR process monitoring (Section 8)
- Ad hoc observation by any project team member

### 13.2 Non-Conformance Report (NCR)

When a non-conformance is identified, QA shall issue a Non-Conformance Report containing:

| Field                | Description                                                |
|----------------------|------------------------------------------------------------|
| NCR ID               | Unique identifier: `NCR-SF-{YYYY}-{SEQ}`                  |
| Date Identified      | Date the non-conformance was discovered                    |
| Identified By        | Name and role of the person identifying the non-conformance|
| Category             | Process / Product / Documentation / Standards              |
| Severity             | Critical / Major / Minor                                   |
| Affected Plan/Std    | Which plan or standard was not followed                    |
| Affected CI          | Configuration items affected (if applicable)               |
| Description          | Detailed description of the non-conformance                |
| Evidence             | Evidence supporting the finding                            |
| Required Response By | Date by which a response/corrective action plan is required|

### 13.3 NCR Severity Definitions

| Severity  | Definition                                                                       |
|-----------|----------------------------------------------------------------------------------|
| Critical  | Non-conformance that may affect airworthiness, safety, or prevents a DO-178C objective from being met. Requires immediate corrective action. |
| Major     | Non-conformance to a plan or standard that does not directly affect safety but could lead to a defective product if not corrected. Requires corrective action within the current phase. |
| Minor     | Non-conformance that is administrative or editorial in nature and does not affect the product. Requires correction but does not block progress. |

### 13.4 NCR Life Cycle

```
Issued -> Acknowledged -> Corrective Action Planned -> Corrective Action Implemented -> Verified -> Closed
       -> Disputed (escalated to QA Manager / Program Manager)
```

- Critical NCRs shall have a corrective action plan within 2 business days
- Major NCRs shall have a corrective action plan within 5 business days
- Minor NCRs shall have a corrective action plan within 10 business days
- All NCRs shall be closed before the affected baseline can be established

### 13.5 NCR Tracking

QA shall maintain a log of all NCRs with current status. NCR metrics are reported as part of QA status reporting:

- Total NCRs: open / closed
- NCRs by severity and category
- Average time to closure
- Repeat non-conformances (same root cause recurring)

---

## 14. Corrective Action Process

### 14.1 Corrective Action Initiation

Corrective action is required for all Critical and Major NCRs. Minor NCRs require correction but may not require formal root cause analysis unless a pattern of recurring minor issues is identified.

### 14.2 Root Cause Analysis

For Critical and Major NCRs, a root cause analysis shall be performed:

#### 14.2.1 Root Cause Analysis Methods

The following methods may be used (individually or in combination):

| Method                | Application                                          |
|-----------------------|------------------------------------------------------|
| 5 Whys               | Simple process failures with clear causal chains     |
| Fishbone (Ishikawa)  | Multi-factor analysis (people, process, tools, environment) |
| Fault Tree Analysis   | Complex failures with multiple contributing causes   |

#### 14.2.2 Root Cause Categories

Root causes shall be categorized as:

| Category        | Examples                                                     |
|-----------------|--------------------------------------------------------------|
| Process         | Inadequate procedure, missing process step, unclear guidance |
| Training        | Insufficient training, unfamiliarity with tools or standards |
| Tool            | Tool defect, tool misconfiguration, inadequate tool support  |
| Communication   | Miscommunication, missing information transfer               |
| Resource        | Insufficient time, personnel, or equipment                   |
| Human Error     | Mistake despite adequate process and training                |

### 14.3 Corrective Action Implementation

The corrective action plan shall document:

- **Immediate Correction:** What is done to fix the specific non-conformance (e.g., rework the code, update the document)
- **Systemic Corrective Action:** What is done to prevent recurrence (e.g., update the process, add a checklist item, enhance training, improve tooling)
- **Responsible Person:** Who is accountable for implementing each action
- **Target Date:** When each action will be completed
- **Affected Scope:** Other areas that may be affected by the same root cause (lateral sweep)

### 14.4 Effectiveness Verification

After corrective actions are implemented, QA shall verify their effectiveness:

- [ ] The immediate correction resolves the specific non-conformance
- [ ] The systemic corrective action is implemented (process updated, training delivered, tool configured)
- [ ] A follow-up audit is scheduled to verify the corrective action prevents recurrence
- [ ] If the same non-conformance recurs, the corrective action is deemed ineffective and the NCR is reopened with a requirement for enhanced corrective action

### 14.5 Corrective Action Records

All corrective action records shall include:

- NCR reference
- Root cause analysis results
- Corrective action plan (immediate and systemic)
- Implementation evidence
- Effectiveness verification results
- QA sign-off

### 14.6 Trend Analysis

QA shall perform trend analysis on non-conformances and corrective actions to identify systemic issues:

- Recurring root causes
- Process areas with high non-conformance rates
- Effectiveness of corrective actions over time
- Correlation between project phases and non-conformance types

Trend analysis results shall be reported to the Project Lead monthly and presented at program milestone reviews.

---

**End of Document — SQAP-SF-2026-001**
