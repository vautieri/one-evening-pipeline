# SynthFlight Requirement Cross-Reference and Deduplication Summary

**Document ID:** SRS-SF-XREF-2026-001
**Date:** 2026-03-14
**Companion to:** SRS-SynthFlight.md

---

## 1. Deduplication Summary

| Metric | Value |
|--------|-------|
| Total raw requirements across 5 domains | 160 |
| Unified SRS requirements produced | 149 |
| Raw requirements merged into shared unified requirements | 41 |
| Raw requirements that are unique to a single domain | 52 |
| Raw requirements dropped (lost) | 0 |
| Domains consolidated | 5 (DEF, COM, GIS, INF, SAR) |

**Key merging decisions:**
- Where multiple domains required the same capability (e.g., terrain collision detection), the most stringent threshold from any domain was adopted (e.g., 50 Hz from DEF, 60-sec lookahead from COM, <1% false alarm from DEF).
- Where domains required domain-specific variants of a common pattern (e.g., SAR search patterns vs. GIS grid patterns), separate requirements were retained since the functionality is genuinely distinct.
- Where domains overlapped on infrastructure-level requirements (batch execution, determinism, DO-178C), a single merged requirement captures all domains' needs.

---

## 2. Complete Cross-Reference Table: Raw Requirement to Unified Requirement

### 2.1 DEF (Defense/Military) — 35 Raw Requirements

| Raw ID | Raw Title | Unified ID | Unified Title | Merged With |
|--------|-----------|------------|---------------|-------------|
| REQ-DEF-001 | ISR Mission Route Simulation | REQ-SF-0500 | ISR and Waypoint Mission Route Planning | — |
| REQ-DEF-002 | Strike Mission Profile Simulation | REQ-SF-0510 | Strike Mission Profile Simulation | — |
| REQ-DEF-003 | Logistics and MEDEVAC Route Planning | REQ-SF-0511, REQ-SF-0204, REQ-SF-0205 | Logistics/MEDEVAC; Payload Effects; Endurance | REQ-COM-024, REQ-COM-027 |
| REQ-DEF-004 | Mission Timeline and Phase Decomposition | REQ-SF-0201 | Mission Phase-of-Flight Decomposition | — |
| REQ-DEF-005 | Nap-of-the-Earth Flight Simulation | REQ-SF-0601 | Terrain-Following Autopilot Mode | REQ-GIS-009, REQ-INF-004 |
| REQ-DEF-006 | Terrain Masking Effectiveness Analysis | REQ-SF-0902 | Terrain Masking Effectiveness Analysis | — |
| REQ-DEF-007 | LiDAR Point Cloud Integration for Urban Terrain | REQ-SF-0401 | LiDAR Point Cloud Integration for Urban Terrain | REQ-INF-008 |
| REQ-DEF-008 | Terrain Collision Detection and CFIT Warning | REQ-SF-0402 | Terrain Collision Detection (CFIT Prevention) | REQ-COM-034, REQ-SAR-009, REQ-INF-004 |
| REQ-DEF-009 | Threat Ring and WEZ Modeling | REQ-SF-0900 | Weapon Engagement Zone (WEZ) Modeling | — |
| REQ-DEF-010 | No-Fly Zone and Airspace Restriction Enforcement | REQ-SF-0810, REQ-SF-0800 | TFR/Airspace Enforcement; Geofence | REQ-SAR-014, REQ-INF-020, REQ-COM-012 |
| REQ-DEF-011 | Dynamic Threat Re-routing Simulation | REQ-SF-0606 | Dynamic Threat Re-routing | — |
| REQ-DEF-012 | GPS-Denied Navigation Simulation | REQ-SF-0605 | GPS-Denied Navigation Simulation | REQ-COM-006 |
| REQ-DEF-013 | Communications Link Loss and Degradation | REQ-SF-0602 | Lost-Link Procedure Simulation | REQ-COM-006, REQ-INF-026, REQ-SAR-021 |
| REQ-DEF-014 | EW Environment Emitter Modeling | REQ-SF-0901 | Electronic Warfare Emitter Modeling | — |
| REQ-DEF-015 | Engine-Out Flight Dynamics Simulation | REQ-SF-0202 | Engine-Out and Powerplant Failure Simulation | REQ-COM-006 |
| REQ-DEF-016 | Control Surface Failure and Battle Damage | REQ-SF-0203 | Control Surface Failure and Battle Damage Simulation | — |
| REQ-DEF-017 | Sensor Degradation and Failure Simulation | REQ-SF-0705 | Sensor Degradation and Failure Simulation | REQ-COM-006 |
| REQ-DEF-018 | Multi-UAS Coordinated Mission Simulation | REQ-SF-1100 | Multi-UAS Coordinated Mission Simulation | REQ-COM-032, REQ-SAR-026 |
| REQ-DEF-019 | Swarm Behavior Verification | REQ-SF-1101 | Swarm Behavior Verification | REQ-COM-032 |
| REQ-DEF-020 | Heterogeneous Vehicle Fleet Simulation | REQ-SF-1102 | Heterogeneous Vehicle Fleet Simulation | — |
| REQ-DEF-021 | CUI Data Handling | REQ-SF-1600 | CUI Data Handling per NIST SP 800-171 | — |
| REQ-DEF-022 | ITAR and Export Control Compliance | REQ-SF-1601, REQ-SF-0102 | ITAR Compliance; Dependency Management | — |
| REQ-DEF-023 | Air-Gapped Operation Mode | REQ-SF-0101 | Air-Gapped Operation Mode | — |
| REQ-DEF-024 | Audit Logging and Forensic Traceability | REQ-SF-1602, REQ-SF-1501 | Audit Logging; Tamper-Evident Logs | REQ-SAR-024 |
| REQ-DEF-025 | STANAG 4586 C2 Interface | REQ-SF-1701 | STANAG 4586 Command and Control Interface | — |
| REQ-DEF-026 | MAVLink Protocol Support | REQ-SF-1700 | MAVLink v2.0 Protocol Support | REQ-GIS-028 |
| REQ-DEF-027 | Link 16 Tactical Data Link | REQ-SF-1702 | Link 16 Tactical Data Link Message Simulation | — |
| REQ-DEF-028 | MIL-STD-882E Safety Analysis Support | REQ-SF-1402 | MIL-STD-882E System Safety Analysis Support | — |
| REQ-DEF-029 | DO-178C Verification Evidence | REQ-SF-1400 | DO-178C Verification Evidence Generation | REQ-COM-018 |
| REQ-DEF-030 | JARUS SORA Risk Assessment | REQ-SF-1403 | JARUS SORA Risk Assessment Data Generation | REQ-COM-019 |
| REQ-DEF-031 | Requirement Traceability Matrix | REQ-SF-1401 | Requirements Traceability Matrix Generation | REQ-INF-030, REQ-GIS-029 |
| REQ-DEF-032 | Real-Time Simulation Performance | REQ-SF-1800, REQ-SF-0200 | Real-Time Factor; JSBSim Integration | — |
| REQ-DEF-033 | Batch Mode Throughput | REQ-SF-1300, REQ-SF-1802 | Batch Execution; Batch Throughput | REQ-COM-033, REQ-SAR-029, REQ-GIS-030 |
| REQ-DEF-034 | Simulation Determinism and Repeatability | REQ-SF-1801 | Simulation Determinism and Repeatability | REQ-INF-021, REQ-SAR-025 |
| REQ-DEF-035 | Simulation Data Recording and Playback | REQ-SF-1500, REQ-SF-1205, REQ-SF-1803 | Flight Data Recording; Playback; Latency | REQ-COM-035, REQ-SAR-024 |

### 2.2 COM (Commercial Aviation) — 35 Raw Requirements

| Raw ID | Raw Title | Unified ID | Unified Title | Merged With |
|--------|-----------|------------|---------------|-------------|
| REQ-COM-001 | Part 107 Operational Envelope | REQ-SF-0801 | Part 107 Operational Envelope Enforcement | REQ-GIS-022 |
| REQ-COM-002 | Part 135 Cargo Drone Constraints | REQ-SF-0803 | Part 135 Air Carrier Constraint Modeling | — |
| REQ-COM-003 | 14 CFR 91.113 Right-of-Way | REQ-SF-0806 | Right-of-Way and See-and-Avoid Modeling | — |
| REQ-COM-004 | Part 107 Waiver Condition Simulation | REQ-SF-0802 | Part 107 Waiver Condition Simulation | REQ-INF-018 |
| REQ-COM-005 | DAA Well-Clear per ASTM F3442 | REQ-SF-0805 | DAA Well-Clear Volume Modeling | — |
| REQ-COM-006 | BVLOS Contingency Action Simulation | REQ-SF-0602, REQ-SF-0605, REQ-SF-1001 | Lost-Link; GPS-Denied; Failure Injection | REQ-DEF-012, REQ-DEF-013, REQ-DEF-015, REQ-DEF-017 |
| REQ-COM-007 | Ground Risk Mitigation — Population Density | REQ-SF-0815 | Population Density Overlay for Ground Risk | — |
| REQ-COM-008 | Remote ID per ASTM F3411 | REQ-SF-0804 | Remote ID Broadcast Simulation | — |
| REQ-COM-009 | UTM/USS Service Interface | REQ-SF-0807 | UTM/USS Service Interface Simulation | — |
| REQ-COM-010 | LAANC Authorization Simulation | REQ-SF-0808 | LAANC Authorization Simulation | — |
| REQ-COM-011 | ADS-B Traffic Injection | REQ-SF-0809 | ADS-B Traffic Injection | — |
| REQ-COM-012 | Dynamic Geofence Enforcement | REQ-SF-0800 | Dynamic Geofence Enforcement | REQ-DEF-010, REQ-INF-028, REQ-SAR-014 |
| REQ-COM-013 | Emergency Landing Site Selection | REQ-SF-0604 | Emergency Landing Site Selection | REQ-SAR-017, REQ-INF-027 |
| REQ-COM-014 | Return-to-Home Path Verification | REQ-SF-0603 | Return-to-Home Path Verification | REQ-SAR-016 |
| REQ-COM-015 | Wind Field Modeling with Gusts | REQ-SF-0301, REQ-SF-0302 | Wind Field; Turbulence Modeling | REQ-SAR-011, REQ-GIS-026, REQ-INF-016 |
| REQ-COM-016 | Icing Condition Simulation | REQ-SF-0303 | Icing Condition Simulation | — |
| REQ-COM-017 | Visibility and Ceiling Minimums | REQ-SF-0304 | Visibility and Ceiling Modeling | REQ-SAR-012 |
| REQ-COM-018 | DO-178C Traceability Evidence | REQ-SF-1400 | DO-178C Verification Evidence Generation | REQ-DEF-029 |
| REQ-COM-019 | JARUS SORA Risk Assessment | REQ-SF-1403 | JARUS SORA Risk Assessment Data | REQ-DEF-030 |
| REQ-COM-020 | EASA SC-RPAS Compliance Mapping | REQ-SF-1404 | EASA SC-RPAS Compliance Mapping | — |
| REQ-COM-021 | Vertiport Approach/Departure | REQ-SF-0515 | Vertiport Approach/Departure Procedure | — |
| REQ-COM-022 | Community Noise Impact Modeling | REQ-SF-0906 | Community Noise Impact Modeling | — |
| REQ-COM-023 | Urban Obstacle Clearance | REQ-SF-0403 | 3D Building and Obstacle Data Ingestion | REQ-GIS-023, REQ-INF-005 |
| REQ-COM-024 | Payload Effects on CG | REQ-SF-0204 | Payload Effects on CG and Flight Performance | REQ-DEF-003 |
| REQ-COM-025 | Drop Zone Accuracy Validation | REQ-SF-0208 | Payload Release and Ballistic Footprint | REQ-COM-026 |
| REQ-COM-026 | Payload Jettison and FTS Simulation | REQ-SF-0208 | Payload Release and Ballistic Footprint | REQ-COM-025 |
| REQ-COM-027 | Endurance and Range with Energy Reserve | REQ-SF-0205 | Endurance and Range Computation | REQ-DEF-003, REQ-SAR-016 |
| REQ-COM-028 | Takeoff and Landing Distance | REQ-SF-0206 | Takeoff and Landing Distance Modeling | — |
| REQ-COM-029 | Climb and Descent Performance | REQ-SF-0207 | Climb and Descent Performance Envelope | REQ-SAR-013 |
| REQ-COM-030 | Part 107 Knowledge Test Study Mode | REQ-SF-1304 | Part 107 Knowledge Test Study Mode | — |
| REQ-COM-031 | Proficiency Check Framework | REQ-SF-1303 | Proficiency Check Framework | — |
| REQ-COM-032 | Multi-Vehicle Fleet Operations Training | REQ-SF-1100, REQ-SF-1101, REQ-SF-1103 | Multi-UAS; Swarm Verification; Deconfliction | REQ-DEF-018, REQ-DEF-019, REQ-SAR-026 |
| REQ-COM-033 | Batch Simulation Execution | REQ-SF-1300, REQ-SF-1302, REQ-SF-1802 | Batch Execution; Pass/Fail; Throughput | REQ-DEF-033, REQ-SAR-029, REQ-GIS-030 |
| REQ-COM-034 | CFIT Prevention with Regulatory Margins | REQ-SF-0402 | Terrain Collision Detection | REQ-DEF-008, REQ-SAR-009 |
| REQ-COM-035 | DFDR Equivalent Logging | REQ-SF-1500, REQ-SF-1502 | Flight Data Recording; Export Formats | REQ-DEF-035, REQ-SAR-024 |

### 2.3 GIS (Surveying/Mapping) — 30 Raw Requirements

| Raw ID | Raw Title | Unified ID | Unified Title | Merged With |
|--------|-----------|------------|---------------|-------------|
| REQ-GIS-001 | Grid/Lawnmower Flight Plan | REQ-SF-0501 | Grid/Lawnmower Flight Plan Generation | REQ-INF-013 |
| REQ-GIS-002 | Crosshatch Flight Plan | REQ-SF-0502 | Crosshatch (Double-Grid) Flight Plan | — |
| REQ-GIS-003 | Corridor Mapping Flight Plan | REQ-SF-0503 | Corridor Mapping Flight Plan Generation | REQ-INF-001, REQ-INF-002, REQ-INF-003 |
| REQ-GIS-004 | Oblique Image Capture | REQ-SF-0504 | Oblique Image Capture Flight Plan | — |
| REQ-GIS-005 | Orbital (POI) Flight Plan | REQ-SF-0505 | Orbital (Point-of-Interest) Flight Plan | REQ-INF-011 |
| REQ-GIS-006 | Forward and Side Overlap Verification | REQ-SF-0707 | Forward and Side Overlap Verification | — |
| REQ-GIS-007 | GSD Computation and Constraint | REQ-SF-0708 | Ground Sample Distance Computation | — |
| REQ-GIS-008 | Coverage Gap Detection | REQ-SF-0709 | Coverage Gap Detection and Reporting | REQ-INF-025 |
| REQ-GIS-009 | Constant-AGL Terrain Following | REQ-SF-0601 | Terrain-Following Autopilot Mode | REQ-DEF-005, REQ-INF-004 |
| REQ-GIS-010 | Terrain Data Ingestion | REQ-SF-0400 | Multi-Format Terrain Data Ingestion | REQ-DEF-005, REQ-INF-008 |
| REQ-GIS-011 | Coordinate Reference System Support | REQ-SF-1902 | Coordinate Reference System Selection | — |
| REQ-GIS-012 | Geoid Model Application | REQ-SF-1903 | Geoid Model Selection | — |
| REQ-GIS-013 | Frame Camera Sensor Model | REQ-SF-0700 | Frame Camera Sensor Model | REQ-INF-023 |
| REQ-GIS-014 | LiDAR Sensor Model | REQ-SF-0701 | LiDAR Sensor Model | — |
| REQ-GIS-015 | Multispectral and Thermal Sensor | REQ-SF-0702 | Multispectral and Thermal Sensor Model | REQ-INF-023, REQ-SAR-019 |
| REQ-GIS-016 | Photo Station Prediction | REQ-SF-1502 | Flight Data Export Formats (EO export) | — |
| REQ-GIS-017 | IMU/GNSS Sync Timing Verification | REQ-SF-1511 | IMU/GNSS Synchronization Timing Verification | — |
| REQ-GIS-018 | Horizontal Positional Accuracy Budget | REQ-SF-1507 | Horizontal Positional Accuracy Budget | — |
| REQ-GIS-019 | Vertical Positional Accuracy Budget | REQ-SF-1508 | Vertical Positional Accuracy Budget | — |
| REQ-GIS-020 | Image Count and Storage Estimation | REQ-SF-1509 | Image Count and Storage Volume Estimation | — |
| REQ-GIS-021 | LiDAR Point Cloud Volume Estimation | REQ-SF-1510 | LiDAR Point Cloud Volume Estimation | — |
| REQ-GIS-022 | Maximum AGL Altitude Enforcement | REQ-SF-0812, REQ-SF-0801 | AGL Enforcement; Part 107 Envelope | REQ-COM-001 |
| REQ-GIS-023 | Obstacle Clearance and No-Fly Zones | REQ-SF-0403, REQ-SF-0810 | 3D Obstacle Data; TFR Enforcement | REQ-COM-023, REQ-INF-005 |
| REQ-GIS-024 | Minimum AGL Altitude Enforcement | REQ-SF-0812 | Max/Min AGL Altitude Enforcement | REQ-GIS-022 |
| REQ-GIS-025 | Solar Angle for Shadow Avoidance | REQ-SF-0308 | Solar Position Computation | — |
| REQ-GIS-026 | Wind Speed and Gust Limits for Survey | REQ-SF-0609, REQ-SF-0301 | Wind Abort; Wind Field Modeling | REQ-SAR-011, REQ-COM-015 |
| REQ-GIS-027 | Atmospheric Refraction Correction | REQ-SF-0309 | Atmospheric Refraction Correction | — |
| REQ-GIS-028 | Flight Plan Import/Export | REQ-SF-1703, REQ-SF-1700 | Flight Plan Import/Export; MAVLink | REQ-DEF-026 |
| REQ-GIS-029 | Survey Quality Report | REQ-SF-1503, REQ-SF-1401 | Survey Quality Report; RTM | REQ-DEF-031, REQ-INF-030 |
| REQ-GIS-030 | Batch Scenario for Parameter Sweeps | REQ-SF-1300, REQ-SF-1301 | Batch Execution; Monte Carlo | REQ-DEF-033, REQ-COM-033, REQ-SAR-029 |

### 2.4 INF (Energy/Infrastructure) — 30 Raw Requirements

| Raw ID | Raw Title | Unified ID | Unified Title | Merged With |
|--------|-----------|------------|---------------|-------------|
| REQ-INF-001 | Corridor Path from GIS Centerline | REQ-SF-0503 | Corridor Mapping Flight Plan Generation | REQ-GIS-003 |
| REQ-INF-002 | Along-Corridor Station-Keeping | REQ-SF-0608 | Along-Corridor Station-Keeping Verification | — |
| REQ-INF-003 | Bidirectional Multi-Leg Corridor | REQ-SF-0503 | Corridor Mapping Flight Plan Generation | REQ-GIS-003 |
| REQ-INF-004 | Terrain-Following Along Corridor | REQ-SF-0601, REQ-SF-0402 | Terrain-Following; CFIT Detection | REQ-GIS-009, REQ-DEF-005, REQ-DEF-008 |
| REQ-INF-005 | Transmission Tower and Conductor Modeling | REQ-SF-0404, REQ-SF-0403 | Infrastructure Obstacle Library; 3D Obstacles | REQ-COM-023, REQ-GIS-023 |
| REQ-INF-006 | Wind Turbine Rotor and Nacelle Modeling | REQ-SF-0405 | Wind Turbine Structure Modeling | — |
| REQ-INF-007 | Guy Wire, Stack, Ancillary Obstacles | REQ-SF-0404 | Parameterized Infrastructure Obstacle Library | REQ-INF-005 |
| REQ-INF-008 | Obstacle Import from LiDAR Point Cloud | REQ-SF-0401, REQ-SF-0400 | LiDAR Point Cloud Integration; Terrain Ingestion | REQ-DEF-007, REQ-GIS-010 |
| REQ-INF-009 | Dynamic Standoff Envelope Enforcement | REQ-SF-0903 | Dynamic Standoff Envelope Enforcement | REQ-INF-010 |
| REQ-INF-010 | Graded Standoff Warning Zones | REQ-SF-0903 | Dynamic Standoff Envelope Enforcement | REQ-INF-009 |
| REQ-INF-011 | Orbit Inspection Pattern | REQ-SF-0505 | Orbital (Point-of-Interest) Flight Plan | REQ-GIS-005 |
| REQ-INF-012 | Vertical Scan Pattern | REQ-SF-0512 | Vertical Scan Pattern for Structure Inspection | — |
| REQ-INF-013 | Panel-by-Panel Solar Array Pattern | REQ-SF-0513, REQ-SF-0501 | Solar Array Pattern; Grid/Lawnmower | REQ-GIS-001 |
| REQ-INF-014 | Blade-Track Inspection Pattern | REQ-SF-0514 | Wind Turbine Blade-Track Inspection Pattern | — |
| REQ-INF-015 | EMI Modeling Near HV Lines | REQ-SF-0710 | EMI Effects on Sensors Near HV Lines | — |
| REQ-INF-016 | Thermal Updraft/Structure Turbulence | REQ-SF-0306 | Localized Thermal Updraft and Turbulence | REQ-COM-015 |
| REQ-INF-017 | Wind Shear and Rotor Wake | REQ-SF-0307 | Wind Turbine Rotor Wake Modeling | — |
| REQ-INF-018 | FAA Part 107 Waiver Verification | REQ-SF-0802 | Part 107 Waiver Condition Simulation | REQ-COM-004 |
| REQ-INF-019 | NERC CIP Cybersecurity | REQ-SF-0813 | NERC CIP Cybersecurity Zone Awareness | — |
| REQ-INF-020 | Airspace and NOTAMs for Corridors | REQ-SF-0810 | TFR and Airspace Restriction Enforcement | REQ-DEF-010, REQ-SAR-014 |
| REQ-INF-021 | Deterministic Mission Replay | REQ-SF-1801 | Simulation Determinism and Repeatability | REQ-DEF-034, REQ-SAR-025 |
| REQ-INF-022 | Mission Template Versioning | REQ-SF-1000 | Scenario Definition and Configuration | REQ-SAR-025 |
| REQ-INF-023 | Thermal Camera Gimbal and FOV | REQ-SF-0702, REQ-SF-0703, REQ-SF-0700 | Thermal/Multispectral; Gimbal; Camera | REQ-GIS-013, REQ-GIS-015, REQ-SAR-019 |
| REQ-INF-024 | Zoom Level and Working Distance | REQ-SF-0704 | Zoom Level and Working Distance Verification | — |
| REQ-INF-025 | Multi-Sensor Synchronization | REQ-SF-0709 | Coverage Gap Detection and Reporting | REQ-GIS-008 |
| REQ-INF-026 | Lost-Link Near Infrastructure | REQ-SF-0602 | Lost-Link Procedure Simulation | REQ-DEF-013, REQ-COM-006 |
| REQ-INF-027 | Emergency Landing Away from Conductors | REQ-SF-0604 | Emergency Landing Site Selection | REQ-COM-013, REQ-SAR-017 |
| REQ-INF-028 | Geofence in Infrastructure Environments | REQ-SF-0800 | Dynamic Geofence Enforcement | REQ-COM-012, REQ-DEF-010 |
| REQ-INF-029 | Flight Segment to Asset ID Traceability | REQ-SF-1506 | Flight Segment to Asset ID Traceability | — |
| REQ-INF-030 | Automated Verification Evidence Package | REQ-SF-1504, REQ-SF-1401 | Evidence Package; RTM | REQ-DEF-029, REQ-DEF-031, REQ-GIS-029 |

### 2.5 SAR (Emergency/Search & Rescue) — 30 Raw Requirements

| Raw ID | Raw Title | Unified ID | Unified Title | Merged With |
|--------|-----------|------------|---------------|-------------|
| REQ-SAR-001 | Expanding Square Search Pattern | REQ-SF-0506 | SAR Expanding Square Search Pattern | — |
| REQ-SAR-002 | Sector Search Pattern | REQ-SF-0507 | SAR Sector Search Pattern | — |
| REQ-SAR-003 | Parallel Track Search Pattern | REQ-SF-0508 | SAR Parallel Track Search Pattern | — |
| REQ-SAR-004 | Creeping Line Ahead Search Pattern | REQ-SF-0509 | SAR Creeping Line Ahead Search Pattern | — |
| REQ-SAR-005 | Rapid Scenario Initialization | REQ-SF-1004 | Rapid Scenario Initialization | — |
| REQ-SAR-006 | Pre-Built SAR Scenario Templates | REQ-SF-1003 | Pre-Built Scenario Templates | — |
| REQ-SAR-007 | Real-Time Search Area Replanning | REQ-SF-0607 | Real-Time Search Area Replanning | — |
| REQ-SAR-008 | Target Drift Compensation | REQ-SF-0516 | Target Drift Compensation for Maritime SAR | — |
| REQ-SAR-009 | DTED/LiDAR Terrain Collision Warning | REQ-SF-0402 | Terrain Collision Detection (CFIT Prevention) | REQ-DEF-008, REQ-COM-034 |
| REQ-SAR-010 | Canyon and Urban Canyon Verification | REQ-SF-0403 | 3D Building and Obstacle Data Ingestion (implied canyon geometry support) | REQ-COM-023 |
| REQ-SAR-011 | High Wind Abort Criteria | REQ-SF-0609, REQ-SF-0301 | Wind Abort; Wind Field Modeling | REQ-COM-015, REQ-GIS-026 |
| REQ-SAR-012 | Visibility and Ceiling Minimums | REQ-SF-0304 | Visibility and Ceiling Modeling | REQ-COM-017 |
| REQ-SAR-013 | Density Altitude Performance Degradation | REQ-SF-0305, REQ-SF-0207 | Density Altitude; Climb/Descent Performance | REQ-COM-029 |
| REQ-SAR-014 | TFR Geofence Enforcement | REQ-SF-0810, REQ-SF-0800 | TFR Enforcement; Geofence | REQ-DEF-010, REQ-COM-012, REQ-INF-020 |
| REQ-SAR-015 | Manned Aircraft Deconfliction | REQ-SF-0811 | Manned Aircraft Deconfliction Altitude Bands | — |
| REQ-SAR-016 | Battery/Fuel vs. Distance-to-Home | REQ-SF-0205, REQ-SF-0603 | Endurance Computation; RTH Verification | REQ-COM-027, REQ-COM-014 |
| REQ-SAR-017 | Safe Landing Zone Identification | REQ-SF-0604 | Emergency Landing Site Selection | REQ-COM-013, REQ-INF-027 |
| REQ-SAR-018 | Night Operations Lighting Compliance | REQ-SF-0814 | Night Operations Lighting Compliance | — |
| REQ-SAR-019 | Thermal/IR Sensor for Night SAR | REQ-SF-0702, REQ-SF-0706 | Thermal Sensor; Detection Probability | REQ-GIS-015, REQ-INF-023 |
| REQ-SAR-020 | NVG Compatibility Rendering | REQ-SF-1202 | NVG Compatibility Rendering Mode | — |
| REQ-SAR-021 | Communication Link Budget Modeling | REQ-SF-1704 | Communication Link Budget Modeling | REQ-DEF-013 |
| REQ-SAR-022 | Terrain Masking Dead-Zone Mapping | REQ-SF-1705 | Terrain Masking and Communication Dead-Zone Mapping | — |
| REQ-SAR-023 | Communication Relay Positioning | REQ-SF-1706 | Communication Relay Positioning Optimization | — |
| REQ-SAR-024 | Timestamped Tamper-Evident Flight Log | REQ-SF-1501, REQ-SF-1500 | Tamper-Evident Log; Flight Data Recording | REQ-DEF-024, REQ-DEF-035, REQ-COM-035 |
| REQ-SAR-025 | Simulation Reproducibility for Legal | REQ-SF-1801, REQ-SF-1000 | Determinism; Scenario Configuration | REQ-DEF-034, REQ-INF-021, REQ-INF-022 |
| REQ-SAR-026 | Multi-UAV Swarm Search | REQ-SF-1100, REQ-SF-1103 | Multi-UAS Simulation; Deconfliction | REQ-DEF-018, REQ-COM-032 |
| REQ-SAR-027 | Wildfire Perimeter with Thermal Hazard | REQ-SF-0904 | Wildfire Perimeter and Thermal Radiation | — |
| REQ-SAR-028 | HAZMAT Plume Avoidance | REQ-SF-0905 | HAZMAT Plume Avoidance Modeling | — |
| REQ-SAR-029 | Batch Scenario Execution for SAR | REQ-SF-1300, REQ-SF-1302 | Batch Execution; Pass/Fail Adjudication | REQ-DEF-033, REQ-COM-033, REQ-GIS-030 |
| REQ-SAR-030 | SAR Mission Effectiveness Metrics | REQ-SF-1505 | SAR Mission Effectiveness Metrics | — |

---

## 3. Overlap Heatmap — Requirements Shared Across Domains

The following table shows the major cross-domain overlaps that were consolidated. Each row represents a capability area where two or more domains had separate but overlapping requirements.

| Capability | DEF | COM | GIS | INF | SAR | Unified ID(s) | Raw IDs Merged |
|------------|-----|-----|-----|-----|-----|---------------|----------------|
| Terrain Collision / CFIT | X | X | | X | X | REQ-SF-0402 | DEF-008, COM-034, SAR-009, INF-004 |
| Geofencing | X | X | X | X | X | REQ-SF-0800 | DEF-010, COM-012, GIS-023, INF-028, SAR-014 |
| Wind/Turbulence Modeling | X | X | X | X | X | REQ-SF-0301, 0302 | COM-015, SAR-011, GIS-026, INF-016, DEF-005 |
| Batch Execution | X | X | X | X | X | REQ-SF-1300 | DEF-033, COM-033, GIS-030, SAR-029 |
| DO-178C Traceability | X | X | | X | | REQ-SF-1400 | DEF-029, COM-018, INF-030 |
| Determinism / Repeatability | X | X | | X | X | REQ-SF-1801 | DEF-034, INF-021, SAR-025 |
| Endurance / Energy Planning | X | X | | X | X | REQ-SF-0205 | DEF-003, COM-027, SAR-016 |
| Multi-Vehicle Simulation | X | X | | | X | REQ-SF-1100 | DEF-018, COM-032, SAR-026 |
| Lost-Link Procedures | X | X | | X | X | REQ-SF-0602 | DEF-013, COM-006, INF-026, SAR-021 |
| Terrain Following | X | | X | X | | REQ-SF-0601 | DEF-005, GIS-009, INF-004 |
| Emergency Landing | | X | | X | X | REQ-SF-0604 | COM-013, INF-027, SAR-017 |
| Corridor Flight Planning | | | X | X | | REQ-SF-0503 | GIS-003, INF-001, INF-002, INF-003 |
| Orbital/POI Patterns | | | X | X | | REQ-SF-0505 | GIS-005, INF-011 |
| Sensor Modeling (Thermal) | | | X | X | X | REQ-SF-0702 | GIS-015, INF-023, SAR-019 |
| Obstacle Modeling (3D) | X | X | X | X | | REQ-SF-0403 | COM-023, GIS-023, INF-005, DEF-007 |
| TFR/Airspace Enforcement | X | X | X | X | X | REQ-SF-0810 | DEF-010, SAR-014, INF-020, GIS-023 |
| Audit Logging | X | | | | X | REQ-SF-1602 | DEF-024, SAR-024 |
| Flight Data Recording | X | X | | | X | REQ-SF-1500 | DEF-035, COM-035, SAR-024 |
| Report Generation | X | X | X | X | X | REQ-SF-1503, 1504, 1505 | GIS-029, INF-030, SAR-030, DEF-029 |
| JARUS SORA | X | X | | | | REQ-SF-1403 | DEF-030, COM-019 |
| RTM Generation | X | X | X | X | X | REQ-SF-1401 | DEF-031, INF-030, GIS-029 |
| GPS-Denied Navigation | X | X | | | X | REQ-SF-0605 | DEF-012, COM-006 |
| CRS / Coordinate Systems | | X | X | X | | REQ-SF-1902 | GIS-011 (primary, others implied) |
| Part 107 Waiver | | X | | X | | REQ-SF-0802 | COM-004, INF-018 |
| Visibility/Ceiling | | X | | | X | REQ-SF-0304 | COM-017, SAR-012 |
| Payload/CG Effects | X | X | | | | REQ-SF-0204 | COM-024, DEF-003 |
| Engine-Out Simulation | X | X | | | | REQ-SF-0202 | DEF-015, COM-006 |
| Sensor Failure | X | X | | | X | REQ-SF-0705 | DEF-017, COM-006 |

---

## 4. Requirements Unique to a Single Domain

The following raw requirements are unique to their originating domain and were not merged with any requirement from another domain:

### DEF-Only (12 unique)
| Raw ID | Title | Unified ID |
|--------|-------|------------|
| REQ-DEF-002 | Strike Mission Profile | REQ-SF-0510 |
| REQ-DEF-004 | Mission Timeline/Phase Decomposition | REQ-SF-0201 |
| REQ-DEF-006 | Terrain Masking Effectiveness | REQ-SF-0902 |
| REQ-DEF-009 | WEZ Modeling | REQ-SF-0900 |
| REQ-DEF-011 | Dynamic Threat Re-routing | REQ-SF-0606 |
| REQ-DEF-014 | EW Emitter Modeling | REQ-SF-0901 |
| REQ-DEF-016 | Control Surface Failure/Battle Damage | REQ-SF-0203 |
| REQ-DEF-021 | CUI Data Handling | REQ-SF-1600 |
| REQ-DEF-023 | Air-Gapped Operation | REQ-SF-0101 |
| REQ-DEF-025 | STANAG 4586 | REQ-SF-1701 |
| REQ-DEF-027 | Link 16 | REQ-SF-1702 |
| REQ-DEF-028 | MIL-STD-882E Safety | REQ-SF-1402 |

### COM-Only (10 unique)
| Raw ID | Title | Unified ID |
|--------|-------|------------|
| REQ-COM-002 | Part 135 Constraints | REQ-SF-0803 |
| REQ-COM-003 | Right-of-Way/See-and-Avoid | REQ-SF-0806 |
| REQ-COM-005 | DAA Well-Clear (ASTM F3442) | REQ-SF-0805 |
| REQ-COM-008 | Remote ID (ASTM F3411) | REQ-SF-0804 |
| REQ-COM-009 | UTM/USS Interface | REQ-SF-0807 |
| REQ-COM-010 | LAANC Authorization | REQ-SF-0808 |
| REQ-COM-016 | Icing Simulation | REQ-SF-0303 |
| REQ-COM-020 | EASA SC-RPAS Compliance | REQ-SF-1404 |
| REQ-COM-021 | Vertiport Procedures | REQ-SF-0515 |
| REQ-COM-022 | Community Noise | REQ-SF-0906 |

### GIS-Only (12 unique)
| Raw ID | Title | Unified ID |
|--------|-------|------------|
| REQ-GIS-002 | Crosshatch Flight Plan | REQ-SF-0502 |
| REQ-GIS-004 | Oblique Image Capture | REQ-SF-0504 |
| REQ-GIS-007 | GSD Computation | REQ-SF-0708 |
| REQ-GIS-012 | Geoid Model | REQ-SF-1903 |
| REQ-GIS-014 | LiDAR Sensor Model | REQ-SF-0701 |
| REQ-GIS-016 | Photo Station Prediction | REQ-SF-1502 |
| REQ-GIS-017 | IMU/GNSS Sync Timing | REQ-SF-1511 |
| REQ-GIS-018 | Horizontal Accuracy Budget | REQ-SF-1507 |
| REQ-GIS-019 | Vertical Accuracy Budget | REQ-SF-1508 |
| REQ-GIS-020 | Image Count Estimation | REQ-SF-1509 |
| REQ-GIS-021 | LiDAR Volume Estimation | REQ-SF-1510 |
| REQ-GIS-027 | Atmospheric Refraction | REQ-SF-0309 |

### INF-Only (8 unique)
| Raw ID | Title | Unified ID |
|--------|-------|------------|
| REQ-INF-006 | Wind Turbine Rotor Modeling | REQ-SF-0405 |
| REQ-INF-012 | Vertical Scan Pattern | REQ-SF-0512 |
| REQ-INF-014 | Blade-Track Inspection | REQ-SF-0514 |
| REQ-INF-015 | EMI Near HV Lines | REQ-SF-0710 |
| REQ-INF-017 | Wind Shear/Rotor Wake | REQ-SF-0307 |
| REQ-INF-019 | NERC CIP Awareness | REQ-SF-0813 |
| REQ-INF-024 | Zoom/Working Distance | REQ-SF-0704 |
| REQ-INF-029 | Asset ID Traceability | REQ-SF-1506 |

### SAR-Only (10 unique)
| Raw ID | Title | Unified ID |
|--------|-------|------------|
| REQ-SAR-001 | Expanding Square Search | REQ-SF-0506 |
| REQ-SAR-002 | Sector Search | REQ-SF-0507 |
| REQ-SAR-004 | Creeping Line Ahead | REQ-SF-0509 |
| REQ-SAR-005 | Rapid Scenario Init | REQ-SF-1004 |
| REQ-SAR-006 | Pre-Built Templates | REQ-SF-1003 |
| REQ-SAR-007 | Real-Time Replanning | REQ-SF-0607 |
| REQ-SAR-008 | Target Drift Compensation | REQ-SF-0516 |
| REQ-SAR-020 | NVG Rendering | REQ-SF-1202 |
| REQ-SAR-022 | Dead-Zone Mapping | REQ-SF-1705 |
| REQ-SAR-030 | Mission Effectiveness Metrics | REQ-SF-1505 |

**Total unique (single-domain) requirements: 52 out of 160 raw (32.5%)**
**Total merged (multi-domain) requirements: 108 out of 160 raw (67.5%)**

---

## 5. Merging Rules Applied

1. **Most Stringent Threshold**: When multiple domains specified different thresholds for the same capability, the most stringent was adopted. Examples:
   - CFIT detection rate: 50 Hz (DEF) adopted over 10 Hz (SAR)
   - Geofence response time: 500 ms (COM) adopted over 2.0 s (INF)
   - Flight data recording rate: 100 Hz (DEF) adopted over 20 Hz (COM) and 10 Hz (SAR)
   - Determinism tolerance: bit-identical (DEF) adopted as the baseline, with INF's 0.01 m RMS as a secondary verification metric

2. **Union of Features**: When domains specified different features for the same system component, all features were included. Examples:
   - Lost-link behaviors: union of loiter, climb-and-proceed, RTB, and mission-continue from all domains
   - Emergency landing criteria: union of slope, roughness, populated-area proximity (COM), energized-conductor distance (INF), and reachability (SAR)

3. **Preserved Domain-Specific Patterns**: Search patterns (IAMSAR), inspection patterns (vertical scan, blade-track), and military-specific patterns (strike, NOE) were kept as separate requirements because they represent genuinely distinct flight geometries.

4. **Regulatory Consolidation**: Part 107 base rules and waiver conditions were consolidated into two requirements (REQ-SF-0801, 0802) despite appearing across COM, GIS, and INF independently.

---

**End of Cross-Reference Document**
