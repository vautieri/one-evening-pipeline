# Software Configuration Management Plan

**Document ID:** SCMP-SF-2026-001
**Project:** SynthFlight — UAV Mission Simulation and Verification Platform
**DAL:** A (Catastrophic) — DO-178C
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Draft

---

## Revision History

| Version | Date       | Author       | Description           |
|---------|------------|--------------|-----------------------|
| 1.0     | 2026-03-14 | SynthFlight  | Initial release       |

---

## Table of Contents

1. [Purpose and Scope](#1-purpose-and-scope)
2. [Reference Documents](#2-reference-documents)
3. [CM Environment](#3-cm-environment)
4. [Configuration Identification](#4-configuration-identification)
5. [Baselines](#5-baselines)
6. [Change Control](#6-change-control)
7. [Version Control](#7-version-control)
8. [Build Control](#8-build-control)
9. [Release Process](#9-release-process)
10. [Status Accounting](#10-status-accounting)
11. [CM Audits](#11-cm-audits)
12. [Data Control](#12-data-control)

---

## 1. Purpose and Scope

### 1.1 Purpose

This Software Configuration Management Plan (SCMP) defines the configuration management activities, processes, and procedures for the SynthFlight UAV Mission Simulation and Verification Platform. It establishes the methods by which all software life cycle data is identified, controlled, tracked, and audited throughout the software development life cycle.

This plan satisfies the configuration management objectives defined in DO-178C, Section 7 — Software Configuration Management Process, and ensures that all software life cycle data produced during the development of SynthFlight is controlled with the rigor required for Design Assurance Level (DAL) A.

### 1.2 Scope

This SCMP applies to all software life cycle data associated with the SynthFlight project, including but not limited to:

- Source code (application code, library code, platform abstraction layers)
- Test code (unit tests, integration tests, fuzz tests)
- Requirements documents (system requirements, software requirements, derived requirements)
- Design documents (software architecture, detailed design)
- Plans (SDP, SVP, SCMP, SQAP, PSAC)
- Standards (SRS-Standard, SDS, SCS)
- Build system configurations (CMake files, CI/CD workflows)
- Verification results (test reports, coverage reports, analysis results)
- Traceability data (requirements-to-code, requirements-to-tests)
- Tool qualification data

All personnel involved in the SynthFlight project shall comply with the procedures defined in this plan. Configuration management activities commence at project initiation and continue through certification, delivery, and post-delivery support.

### 1.3 DAL A Implications

As a DAL A (Catastrophic) project under DO-178C, SynthFlight configuration management shall meet the most stringent objectives. All CM objectives listed in DO-178C Table A-9 are applicable without exception. This includes:

- All software life cycle data is identified and controlled
- Baselines are established and integrity is maintained
- Problem reporting, change control, and change review are formally conducted
- Configuration status accounting is maintained throughout the life cycle
- Archive and retrieval processes guarantee data integrity and availability
- Software load control ensures only authorized software is delivered

---

## 2. Reference Documents

| Document ID       | Title                                                        | Version |
|-------------------|--------------------------------------------------------------|---------|
| DO-178C           | Software Considerations in Airborne Systems and Equipment Certification, Section 7 | Rev C   |
| DO-330            | Software Tool Qualification Considerations                   | —       |
| PSAC-SF-2026-001  | Plan for Software Aspects of Certification — SynthFlight     | 1.0     |
| SDP-SF-2026-001   | Software Development Plan — SynthFlight                      | 1.0     |
| SVP-SF-2026-001   | Software Verification Plan — SynthFlight                     | 1.0     |
| SQAP-SF-2026-001  | Software Quality Assurance Plan — SynthFlight                | 1.0     |
| SRS-STD-SF-2026   | Software Requirements Standards — SynthFlight                | 1.0     |
| SDS-STD-SF-2026   | Software Design Standards — SynthFlight                      | 1.0     |
| SCS-STD-SF-2026   | Software Code Standards — SynthFlight (C++17, GCC)           | 1.0     |

---

## 3. CM Environment

### 3.1 Version Control System

- **Tool:** Git (distributed version control)
- **Hosting Platform:** GitHub — repository `github.com/vautieri/one-evening-pipeline`
- **Access Control:** GitHub organizational roles (Owner, Maintainer, Developer, Read-only) govern repository permissions. Write access to protected branches is restricted to authorized personnel.

### 3.2 CI/CD Platform

- **Platform:** GitHub Actions
- **Workflow Definitions:**

| Workflow File           | Purpose                                                |
|-------------------------|--------------------------------------------------------|
| `ci.yml`                | Primary build, test execution, and coverage reporting  |
| `codeql.yml`            | Static analysis via CodeQL for security and defect detection |
| `dependency-review.yml` | Automated review of dependency changes for vulnerabilities |
| `fuzz.yml`              | Fuzz testing execution via libFuzzer                   |
| `dependabot.yml`        | Automated dependency update monitoring                 |

All CI/CD workflow definitions are configuration items subject to change control as defined in this plan.

### 3.3 Build System

- **Build Tool:** CMake (minimum version 3.16)
- **Compiler:** GCC (version pinned per baseline — see Section 8)
- **Language Standard:** C++17 (ISO/IEC 14882:2017)
- **Coverage Tool:** lcov

### 3.4 Test Frameworks

| Framework    | Purpose                            |
|--------------|------------------------------------|
| Google Test  | Primary unit testing framework     |
| Catch2       | Header-only unit testing           |
| Doctest      | Lightweight unit testing           |
| Boost.Test   | Extended testing capabilities      |
| CppUnit      | Legacy/integration testing         |
| libFuzzer    | Fuzz testing for robustness        |

### 3.5 CM Tooling

- **Issue Tracking:** GitHub Issues (for Discrepancy Reports and Change Requests)
- **Pull Requests:** GitHub Pull Requests (for change review and approval)
- **Project Tracking:** GitHub Projects (for status accounting dashboards)

---

## 4. Configuration Identification

### 4.1 Naming Conventions

All software life cycle artifacts shall follow standardized naming conventions to ensure unique identification and traceability.

#### 4.1.1 Source Code Files

```
src/{module}/{SubModule}.cpp
src/{module}/{SubModule}.h
```

- Module names: lowercase with hyphens (e.g., `flight-dynamics`, `sensor-model`)
- Class/header names: PascalCase (e.g., `FlightController.cpp`, `SensorFusion.h`)

#### 4.1.2 Test Code Files

```
tests/unit/{module}/{SubModule}_test.cpp
tests/integration/{module}/{SubModule}_integration_test.cpp
tests/fuzz/{module}/{SubModule}_fuzz.cpp
```

- Test files mirror the source structure with a `_test`, `_integration_test`, or `_fuzz` suffix

#### 4.1.3 Documentation and Plans

```
docs/plans/{DocumentType}-SynthFlight.md
docs/requirements/{REQ-Category}-SynthFlight.md
docs/design/{DES-Category}-SynthFlight.md
docs/standards/{STD-Category}-SynthFlight.md
docs/verification/{VER-Category}-SynthFlight.md
```

Document identifiers follow the pattern: `{TYPE}-SF-{YEAR}-{SEQ}`

| Type Prefix | Document Category           |
|-------------|-----------------------------|
| PSAC        | Plan for Software Aspects of Certification |
| SDP         | Software Development Plan   |
| SVP         | Software Verification Plan  |
| SCMP        | Software Configuration Management Plan |
| SQAP        | Software Quality Assurance Plan |
| SRS         | Software Requirements Specification |
| SDS         | Software Design Specification |
| SCS         | Software Code Standard      |
| SCI         | Software Configuration Index |
| SAS         | Software Accomplishment Summary |

#### 4.1.4 Build System Files

```
CMakeLists.txt                    (root build configuration)
cmake/{module}.cmake              (module-specific build rules)
.github/workflows/{workflow}.yml  (CI/CD pipeline definitions)
```

### 4.2 Configuration Items (CI) List

The following categories of items are designated as Configuration Items and are subject to formal configuration management under this plan:

| CI Category        | Description                                   | Location in Repository          |
|--------------------|-----------------------------------------------|----------------------------------|
| Source Code         | Application source files (.cpp, .h)          | `src/`                           |
| Test Code           | Unit, integration, and fuzz test files        | `tests/`                         |
| Requirements        | Software and system requirements documents    | `docs/requirements/`             |
| Plans               | All software life cycle plans                 | `docs/plans/`                    |
| Standards           | Coding, design, and requirements standards    | `docs/standards/`                |
| Build Scripts       | CMake configurations                          | `CMakeLists.txt`, `cmake/`       |
| CI Workflows        | GitHub Actions workflow definitions           | `.github/workflows/`             |
| Traceability Data   | Requirements traceability matrices            | `docs/traceability/`             |
| Verification Results| Test reports, coverage data                   | `docs/verification/`             |
| Tool Configurations | Compiler flags, linker settings, tool configs | `cmake/`, `.github/`             |

### 4.3 Configuration Item Identification

Each configuration item shall be uniquely identified by:

1. **File path** within the repository (serves as primary identifier)
2. **Git commit hash** (40-character SHA-1, provides immutable identification)
3. **Git tag** (for baseline-level identification)
4. **Document ID** (for plans and formal documents, as specified in Section 4.1.3)

---

## 5. Baselines

### 5.1 Baseline Types

Three types of baselines are established for the SynthFlight project:

#### 5.1.1 Development Baseline

- **Definition:** The approved set of source code, design documents, and requirements at any given point during active development.
- **Contents:** Source code, software requirements (SRS), software design descriptions (SDS), coding standards, build configurations.
- **Representation:** A Git tag on the `develop` branch in the format `dev-baseline-{YYYY-MM-DD}-{seq}`.
- **Establishment Criteria:**
  - All source code compiles without errors or warnings (with `-Wall -Wextra -Werror`)
  - All existing unit tests pass
  - Code review completed for all changes since the previous development baseline
  - No unresolved Critical or Major Discrepancy Reports against baseline content
  - Requirements traceability updated

#### 5.1.2 Test Baseline

- **Definition:** The verified test suite, associated test procedures, and coverage data corresponding to a specific development baseline.
- **Contents:** Test source code, test procedures, test results, coverage reports (lcov), traceability matrices (tests-to-requirements).
- **Representation:** A Git tag in the format `test-baseline-{YYYY-MM-DD}-{seq}`.
- **Establishment Criteria:**
  - All unit tests pass (Google Test, Catch2, Doctest, Boost.Test, CppUnit)
  - Fuzz tests execute without failures (libFuzzer)
  - Statement coverage meets or exceeds DAL A threshold (per SVP)
  - Decision coverage (MC/DC) meets or exceeds DAL A threshold (per SVP)
  - All test cases trace to at least one software requirement
  - All software requirements have at least one associated test case
  - Test review completed by an independent reviewer (per DAL A requirements)

#### 5.1.3 Product Baseline

- **Definition:** The release-ready, fully verified software configuration representing a deliverable product.
- **Contents:** All development baseline items, all test baseline items, Software Configuration Index (SCI), Software Accomplishment Summary (SAS), and all supporting life cycle data.
- **Representation:** A Git tag in the format `v{major}.{minor}.{patch}` (see Section 7.4).
- **Establishment Criteria:**
  - Development baseline established and approved
  - Test baseline established and approved
  - All verification activities completed per SVP
  - No open Critical or Major Discrepancy Reports
  - All Minor Discrepancy Reports dispositioned (fixed, deferred with rationale, or waived with approval)
  - Software Configuration Index (SCI) generated and reviewed
  - Software Accomplishment Summary (SAS) completed
  - QA conformity review passed (per SQAP)
  - Functional Configuration Audit (FCA) completed
  - Physical Configuration Audit (PCA) completed
  - Change Review Board (CRB) approval obtained

### 5.2 Baseline Integrity

Once a baseline is established:

- No changes shall be made to baselined items without following the change control process (Section 6)
- The Git tag representing the baseline shall never be moved or deleted
- The baseline shall be archived per Section 9.3

---

## 6. Change Control

### 6.1 Problem Reporting (Discrepancy Reports)

#### 6.1.1 DR Process

A Discrepancy Report (DR) shall be filed whenever a problem, error, or non-conformance is identified in any configuration item. DRs are tracked via GitHub Issues with the label `discrepancy-report`.

#### 6.1.2 DR Content

Each DR shall include:

| Field              | Description                                              |
|--------------------|----------------------------------------------------------|
| DR ID              | Auto-assigned GitHub Issue number, prefixed `DR-SF-`     |
| Title              | Concise description of the discrepancy                   |
| Severity           | Critical / Major / Minor                                 |
| Category           | Requirements / Design / Code / Test / Documentation / Process |
| Affected CI        | Configuration item(s) affected (file paths, document IDs)|
| Affected Baseline  | Baseline(s) impacted, if any                             |
| Description        | Detailed description of the problem                      |
| Steps to Reproduce | For code/test defects, steps to reproduce                |
| Reported By        | Name and role of reporter                                |
| Date Reported      | Date the DR was filed                                    |

#### 6.1.3 DR Severity Definitions

| Severity  | Definition                                                                       |
|-----------|----------------------------------------------------------------------------------|
| Critical  | Prevents system operation, causes incorrect simulation results, safety impact, or prevents certification activity completion |
| Major     | Significantly degrades capability, workaround exists but is unacceptable for production |
| Minor     | Cosmetic, documentation error, or issue with acceptable workaround               |

#### 6.1.4 DR Life Cycle

```
Open -> Under Review -> Accepted -> In Work -> Resolved -> Verified -> Closed
                     -> Rejected (with rationale)
                     -> Deferred (with rationale and CRB approval)
```

- All Critical DRs must be resolved before a product baseline can be established
- All Major DRs must be resolved or formally deferred with CRB approval
- DR aging reports shall be generated weekly

### 6.2 Change Request Process

#### 6.2.1 Change Request Initiation

Any change to a baselined configuration item requires a formal Change Request (CR). CRs are tracked via GitHub Issues with the label `change-request`.

A CR shall include:

- CR ID (auto-assigned GitHub Issue number, prefixed `CR-SF-`)
- Title and description of the proposed change
- Rationale for the change
- Affected configuration items
- Affected baselines
- Associated DR(s), if applicable
- Proposed implementation approach
- Requestor name and date

#### 6.2.2 Impact Analysis

Before a CR is approved, an impact analysis shall be performed documenting:

- **Requirements Impact:** Which requirements are affected, added, modified, or deleted
- **Design Impact:** Which design elements are affected
- **Code Impact:** Which source files and modules are affected
- **Test Impact:** Which test cases need to be added, modified, or re-executed
- **Schedule Impact:** Estimated effort and schedule effect
- **Safety Impact:** Whether the change affects safety-related functionality
- **Traceability Impact:** Updates needed to traceability matrices

### 6.3 Change Review Board (CRB)

#### 6.3.1 CRB Composition

The Change Review Board for SynthFlight shall include:

| Role                        | Responsibility                                    |
|-----------------------------|---------------------------------------------------|
| Project Lead (Chair)        | Final approval authority                          |
| Software Lead               | Technical assessment of proposed changes          |
| Verification Lead           | Assessment of verification impact                 |
| QA Representative           | Independence oversight, process compliance        |
| CM Representative           | Configuration impact assessment                   |
| Safety Engineer (as needed) | Safety impact assessment for safety-related changes |

#### 6.3.2 CRB Procedures

- The CRB shall convene as needed (minimum monthly during active development)
- Quorum requires the Chair, Software Lead, and QA Representative
- CRB decisions are recorded in the CR issue with the label `crb-reviewed`
- CRB may approve, reject, defer, or request additional information

#### 6.3.3 Approval Authority

| Change Type                      | Approval Required           |
|----------------------------------|-----------------------------|
| Safety-related requirement change | CRB + Safety Engineer       |
| Non-safety requirement change     | CRB                         |
| Design change                     | CRB                         |
| Code change (baselined)           | CRB                         |
| Code change (pre-baseline)        | Software Lead + Peer Review |
| Test change                       | CRB + Verification Lead     |
| Plan/Standard change              | CRB + QA                    |
| Build/CI change                   | Software Lead + CM          |
| Documentation (editorial)         | Document Owner              |

---

## 7. Version Control

### 7.1 Git Branching Strategy

The SynthFlight project employs the following branching model:

| Branch Pattern     | Purpose                                        | Protected | Merge Target    |
|--------------------|-------------------------------------------------|-----------|-----------------|
| `main`             | Production-ready code; product baselines only   | Yes       | —               |
| `develop`          | Integration branch; development baselines       | Yes       | `main`          |
| `feature/*`        | New feature development                         | No        | `develop`       |
| `tests/*`          | Test development and enhancement                | No        | `develop`       |
| `hotfix/*`         | Critical production fixes                       | No        | `main`, `develop` |
| `release/*`        | Release preparation and stabilization           | Yes       | `main`, `develop` |

#### 7.1.1 Branch Protection Rules

The `main` and `develop` branches shall have the following protection rules enforced via GitHub:

- Direct pushes are prohibited
- Pull request required for all merges
- At least one approving review required (two for `main`)
- All CI status checks must pass before merge
- Branch must be up to date with the target before merge
- Force pushes are prohibited
- Branch deletion is prohibited

### 7.2 Branch Naming Conventions

| Branch Type | Format                                    | Example                                      |
|-------------|-------------------------------------------|----------------------------------------------|
| Feature     | `feature/{WBS}-{description}`             | `feature/WBS3.2-add-sensor-fusion`           |
| Test        | `tests/add-unit-tests-{module}`           | `tests/add-unit-tests-flight-dynamics`       |
| Hotfix      | `hotfix/{DR-ID}-{description}`            | `hotfix/DR-SF-142-fix-quaternion-overflow`   |
| Release     | `release/v{major}.{minor}.{patch}`        | `release/v1.2.0`                             |

### 7.3 Commit Message Format

All commits shall follow this format:

```
TAR-XXXX: {short description} (REQ-SF-XXXX)

{optional body with detailed explanation}

{optional footer with references}
```

Where:

- `TAR-XXXX` is the task/action request number from the project tracking system
- `{short description}` is a concise imperative-mood summary (max 72 characters in the subject line)
- `(REQ-SF-XXXX)` is the associated software requirement identifier (when applicable)

Example:

```
TAR-0047: Implement quaternion normalization for attitude propagation (REQ-SF-0112)

Added unit quaternion normalization step after each integration timestep
to prevent numeric drift in the attitude state vector. Normalization uses
the fast inverse square root method with a maximum error tolerance of 1e-12.

Resolves: DR-SF-089
Reviewed-by: J. Smith
```

Additional commit metadata for LLM-assisted development:

```
LLM Assisted: Yes
LLM Tool: {tool name and version}
```

### 7.4 Tag Format

Release tags follow semantic versioning:

```
v{major}.{minor}.{patch}
```

| Component | Increment When                                              |
|-----------|-------------------------------------------------------------|
| `major`   | Incompatible API changes or major architectural changes     |
| `minor`   | New functionality added in a backward-compatible manner     |
| `patch`   | Backward-compatible bug fixes                               |

Tags are annotated tags and shall include:

- Tag name (e.g., `v1.2.0`)
- Tagger name and date
- Release description referencing the SCI document

Pre-release tags may use suffixes: `v1.2.0-rc.1`, `v1.2.0-beta.1`

Baseline tags (non-release):

- Development baselines: `dev-baseline-{YYYY-MM-DD}-{seq}`
- Test baselines: `test-baseline-{YYYY-MM-DD}-{seq}`

### 7.5 Merge Strategy

- All merges into protected branches (`main`, `develop`) require a Pull Request
- Pull Requests require:
  - A descriptive title and body referencing the CR or DR
  - At least one approving review (two for merges into `main`)
  - All CI pipeline checks passing (`ci.yml`, `codeql.yml`, `dependency-review.yml`, `fuzz.yml`)
  - No unresolved review comments
  - Up-to-date with target branch (rebase or merge from target)
- Merge commits are used (no squash) to preserve full history for traceability
- After merge, the source branch shall be deleted (except `develop` and `main`)

---

## 8. Build Control

### 8.1 CMake Configuration Baselines

The root `CMakeLists.txt` and all module-specific CMake files are configuration items. Changes to build configuration require change control per Section 6.

Key CMake settings that are baselined:

| Setting                  | Baseline Value                |
|--------------------------|-------------------------------|
| `cmake_minimum_required` | 3.16                          |
| `CMAKE_CXX_STANDARD`    | 17                            |
| `CMAKE_CXX_STANDARD_REQUIRED` | ON                      |
| `CMAKE_CXX_EXTENSIONS`  | OFF                           |
| Warning flags            | `-Wall -Wextra -Werror -Wpedantic` |
| Optimization (Debug)     | `-O0 -g`                      |
| Optimization (Release)   | `-O2`                         |
| Coverage flags           | `--coverage -fprofile-arcs -ftest-coverage` |

### 8.2 Compiler Version Control

- The GCC compiler version shall be pinned for each product baseline
- The specific GCC version is recorded in the Software Configuration Index (SCI)
- Compiler version changes require a CR and full regression testing
- The CI/CD pipeline shall use the same compiler version as the development environment
- Compiler version is specified in the GitHub Actions workflow (`ci.yml`) via the runner image or explicit installation step

### 8.3 Dependency Management

All third-party dependencies obtained via CMake `FetchContent` or other mechanisms shall have their versions locked:

| Dependency    | Version Control Mechanism                        |
|---------------|--------------------------------------------------|
| Google Test   | FetchContent with pinned Git tag or commit hash  |
| Catch2        | FetchContent with pinned Git tag or commit hash  |
| Doctest       | FetchContent with pinned Git tag or commit hash  |
| Boost.Test    | System package with pinned version               |
| CppUnit       | System package with pinned version               |
| libFuzzer     | Bundled with compiler (version follows GCC)      |
| JSBSim        | FetchContent with pinned Git tag or commit hash  |

- Dependency updates are managed via `dependabot.yml` with automatic PR creation
- Dependency PRs are subject to `dependency-review.yml` checks
- All dependency changes require review and CI validation before merge
- The exact version (tag or commit hash) of each dependency is recorded in the SCI

### 8.4 Reproducible Build Requirements

To ensure build reproducibility:

- All builds shall be executable from the repository checkout alone (no external unversioned dependencies)
- Build environment (OS, compiler, CMake version, dependency versions) is documented in the SCI
- CI/CD builds use a fixed runner image version
- Build artifacts include build metadata (commit hash, build timestamp, compiler version)
- Archived baselines shall include sufficient information to reproduce the build environment

---

## 9. Release Process

### 9.1 Release Checklist

Before a product baseline (release) can be established, the following checklist shall be completed:

- [ ] All planned software requirements implemented and traced to design and code
- [ ] All unit tests pass (Google Test, Catch2, Doctest, Boost.Test, CppUnit)
- [ ] All integration tests pass
- [ ] All fuzz tests pass (libFuzzer)
- [ ] Statement coverage meets DAL A threshold
- [ ] Decision coverage (MC/DC) meets DAL A threshold
- [ ] All requirements traced to test cases
- [ ] All test cases traced to requirements
- [ ] No open Critical Discrepancy Reports
- [ ] No open Major Discrepancy Reports (or formally deferred by CRB)
- [ ] All Minor Discrepancy Reports dispositioned
- [ ] Code reviews completed for all changes since last baseline
- [ ] CodeQL analysis shows no unresolved findings
- [ ] Dependency review shows no unresolved vulnerabilities
- [ ] Software Configuration Index (SCI) generated and reviewed
- [ ] Software Accomplishment Summary (SAS) completed
- [ ] QA conformity review passed
- [ ] Functional Configuration Audit (FCA) completed
- [ ] Physical Configuration Audit (PCA) completed
- [ ] CRB approval obtained for release
- [ ] All CI/CD pipeline checks passing on the release branch

### 9.2 Software Configuration Index (SCI) Generation

The SCI shall be generated for each product baseline and shall contain:

- Product identification (name, version, part number)
- Complete list of all configuration items with version identification (file path and Git commit hash)
- Compiler identification (GCC version)
- Build tool identification (CMake version)
- Dependency list with exact versions
- Hardware/OS environment specifications
- List of all open Discrepancy Reports with disposition
- Identification of any deviations from plans or standards
- Archive location and retrieval instructions

### 9.3 Archive and Retrieval

- Each product baseline shall be archived as a Git tag with full repository state
- A complete archive package shall be created containing:
  - Full repository snapshot (Git bundle or archive)
  - Build environment specification
  - Pre-built binaries (if applicable)
  - All verification results (test reports, coverage reports)
  - All life cycle data documents
- Archives shall be stored in at least two geographically separate locations
- Archive integrity shall be verified via SHA-256 checksums
- Retrieval procedures shall be tested annually
- Archive retention period: life of the aircraft type plus 2 years (minimum)

---

## 10. Status Accounting

### 10.1 CM Status Tracking

Configuration status accounting provides visibility into the current state of all configuration items and baselines. The following status information is tracked and reported:

#### 10.1.1 Configuration Item Status

- Current version (Git commit hash) of each CI
- Baseline membership (which baselines include this CI version)
- Open DRs against the CI
- Pending CRs affecting the CI
- Change history (Git log)

#### 10.1.2 Baseline Status

- Baseline identifier and date established
- List of CIs and their versions in the baseline
- Open DRs against the baseline
- Approved CRs pending implementation

#### 10.1.3 Change Request Status

- Total CRs: open, in review, approved, rejected, implemented, verified
- CR aging (time in each state)
- CR by category and priority

#### 10.1.4 Discrepancy Report Status

- Total DRs: open, under review, accepted, in work, resolved, verified, closed
- DR aging (time in each state)
- DR by severity, category, and affected CI
- DR closure rate trends

### 10.2 CM Reporting

| Report                    | Frequency      | Audience                |
|---------------------------|----------------|-------------------------|
| CM Status Summary         | Weekly         | Project team            |
| DR Aging Report           | Weekly         | Project Lead, QA        |
| Baseline Status Report    | At each baseline | CRB, QA, Project Lead |
| CR Impact Summary         | Per CRB meeting | CRB members            |
| CM Metrics Dashboard      | Continuously updated | All stakeholders   |

### 10.3 CM Metrics

The following metrics are tracked to assess CM process effectiveness:

- Number of baselines established vs. planned
- Average time to resolve DRs by severity
- CR approval-to-implementation cycle time
- Number of unauthorized changes detected (target: zero)
- Build reproducibility success rate (target: 100%)

---

## 11. CM Audits

### 11.1 Functional Configuration Audit (FCA)

The FCA verifies that the software performs as documented in the requirements and that all verification activities have been completed. The FCA shall verify:

- All software requirements are tested and results documented
- Test results match expected outcomes
- Coverage analysis results meet DAL A objectives
- All DRs are closed or formally dispositioned
- Requirements traceability is complete (requirements to design, design to code, requirements to tests)
- All derived requirements have been identified and communicated to the system process

#### 11.1.1 FCA Checklist

- [ ] All software requirements verified by at least one test case
- [ ] All test cases executed and results recorded
- [ ] Statement coverage meets DAL A threshold
- [ ] MC/DC coverage meets DAL A threshold
- [ ] Requirements traceability matrix complete and reviewed
- [ ] All derived requirements identified with rationale
- [ ] No open Critical or Major DRs
- [ ] Verification results reviewed by independent QA

### 11.2 Physical Configuration Audit (PCA)

The PCA verifies that the software as built matches the documentation and that the SCI accurately represents the delivered configuration. The PCA shall verify:

- The executable code can be regenerated from the archived source
- The SCI accurately lists all configuration items and their versions
- The archived repository matches the tagged baseline
- Build environment specifications are accurate and complete
- All life cycle data documents are present and at the correct version
- Tool versions match those recorded in the SCI

#### 11.2.1 PCA Checklist

- [ ] Repository checkout from baseline tag matches archived source
- [ ] Build from archived source produces identical output (reproducible build)
- [ ] SCI lists all CIs with correct versions
- [ ] All documents referenced in SCI are present in archive
- [ ] Compiler and tool versions match SCI records
- [ ] Dependency versions match SCI records
- [ ] Archive checksums verified

### 11.3 Audit Schedule

| Audit | Timing                                      | Conducted By       |
|-------|---------------------------------------------|--------------------|
| FCA   | Before each product baseline establishment  | QA + Verification Lead |
| PCA   | Before each product baseline establishment  | QA + CM            |
| Periodic CM Process Audit | Quarterly during active development | QA |

### 11.4 Audit Records

All audit results shall be documented and retained, including:

- Audit date and participants
- Checklist results (pass/fail for each item)
- Findings and observations
- Corrective actions required
- Follow-up verification of corrective actions

---

## 12. Data Control

### 12.1 DO-178C Data Items

The following DO-178C data items are produced during the SynthFlight project. All items are configuration items subject to the controls defined in this plan:

| # | Data Item                                        | Document ID Pattern     | Storage Location     |
|---|--------------------------------------------------|-------------------------|----------------------|
| 1 | Plan for Software Aspects of Certification (PSAC)| PSAC-SF-2026-XXX        | `docs/plans/`        |
| 2 | Software Development Plan (SDP)                  | SDP-SF-2026-XXX         | `docs/plans/`        |
| 3 | Software Verification Plan (SVP)                 | SVP-SF-2026-XXX         | `docs/plans/`        |
| 4 | Software Configuration Management Plan (SCMP)    | SCMP-SF-2026-XXX        | `docs/plans/`        |
| 5 | Software Quality Assurance Plan (SQAP)           | SQAP-SF-2026-XXX        | `docs/plans/`        |
| 6 | Software Requirements Standards (SRS-Std)        | SRS-STD-SF-2026-XXX     | `docs/standards/`    |
| 7 | Software Design Standards (SDS-Std)              | SDS-STD-SF-2026-XXX     | `docs/standards/`    |
| 8 | Software Code Standards (SCS)                    | SCS-STD-SF-2026-XXX     | `docs/standards/`    |
| 9 | Software Requirements Data (SRD)                 | SRD-SF-2026-XXX         | `docs/requirements/` |
| 10| Software Design Description (SDD)                | SDD-SF-2026-XXX         | `docs/design/`       |
| 11| Source Code                                      | (File paths in `src/`)  | `src/`               |
| 12| Executable Object Code                           | (Build output)          | Build artifacts      |
| 13| Software Verification Cases and Procedures (SVCP)| SVCP-SF-2026-XXX        | `docs/verification/` |
| 14| Software Verification Results (SVR)              | SVR-SF-2026-XXX         | `docs/verification/` |
| 15| Software Configuration Index (SCI)               | SCI-SF-2026-XXX         | `docs/release/`      |
| 16| Software Accomplishment Summary (SAS)            | SAS-SF-2026-XXX         | `docs/release/`      |
| 17| Problem Reports (DRs)                            | DR-SF-XXXX              | GitHub Issues        |

### 12.2 Storage

- **Primary Storage:** GitHub repository (`github.com/vautieri/one-evening-pipeline`)
- **Backup:** Automated repository mirroring to a secondary Git hosting service (configured independently)
- **Archive Storage:** Baseline archives stored on secured, access-controlled storage with geographic redundancy

### 12.3 Backup

- GitHub provides built-in redundancy for repository data
- Additional nightly backups of the repository (full clone including all branches and tags) to secondary storage
- Backup integrity verified weekly via automated checkout and build
- Backup retention: all backups retained for the duration of the project plus the archive retention period

### 12.4 Access Control

| Role               | Repository Access | Branch Write    | Merge to `develop` | Merge to `main` | Admin |
|--------------------|-------------------|-----------------|---------------------|------------------|-------|
| Project Lead       | Read/Write        | All             | Yes (approve)       | Yes (approve)    | Yes   |
| Software Lead      | Read/Write        | All             | Yes (approve)       | Yes (approve)    | No    |
| Developer          | Read/Write        | Feature, Test   | Yes (request)       | No               | No    |
| Verification Lead  | Read/Write        | Test            | Yes (approve)       | Yes (approve)    | No    |
| QA Representative  | Read              | None            | Yes (approve)       | Yes (approve)    | No    |
| External Reviewer  | Read              | None            | No                  | No               | No    |

- Access changes require Project Lead approval
- Access reviews conducted quarterly
- All access is via individual accounts (no shared credentials)
- Two-factor authentication (2FA) is required for all accounts with write access

### 12.5 Data Integrity

- Git cryptographic hashing (SHA-1) provides inherent data integrity verification
- Signed commits are encouraged for all baselined content
- Signed tags are required for all baseline tags
- Archive packages include SHA-256 checksums for integrity verification
- Any detected data corruption shall be reported as a Critical DR

---

**End of Document — SCMP-SF-2026-001**
