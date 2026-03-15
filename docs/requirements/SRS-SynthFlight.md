# SRS-SynthFlight: Unified System Requirements Specification

**Document ID:** SRS-SF-2026-001
**Product:** SynthFlight — UAV Mission Simulation and Verification Platform
**Version:** 1.0
**Date:** 2026-03-14
**Status:** Baselined

---

## 1. Introduction

### 1.1 Purpose
This System Requirements Specification (SRS) consolidates requirements from five industry domains into a single, unified specification for the SynthFlight UAV mission simulation and verification platform. The five source domains are:

| Code | Domain | Source Document | Raw Req Count |
|------|--------|----------------|---------------|
| DEF | Defense / Military | REQ-DEF-001 through REQ-DEF-035 | 35 |
| COM | Commercial Aviation / Civil UAS | REQ-COM-001 through REQ-COM-035 | 35 |
| GIS | Geospatial Surveying & Mapping | REQ-GIS-001 through REQ-GIS-030 | 30 |
| INF | Energy & Infrastructure Inspection | REQ-INF-001 through REQ-INF-030 | 30 |
| SAR | Emergency Response / Search & Rescue | REQ-SAR-001 through REQ-SAR-030 | 30 |
| | **Total Raw Requirements** | | **160** |

### 1.2 Scope
SynthFlight is a desktop application that:
- Uses JSBSim as the 6-DOF physics engine
- Integrates real-world terrain data (DTED, LiDAR, GeoTIFF DEM)
- Provides Vulkan-based 3D terrain rendering
- Supports pluggable flight controllers and autopilots
- Runs 1000+ scenarios overnight in batch/headless mode
- Generates DO-178C traceable verification evidence
- Runs on Windows 10+ and Ubuntu 22.04+ Linux
- Models the 1976 U.S. Standard Atmosphere with wind, turbulence, and icing
- Performs terrain collision detection (CFIT prevention)
- Supports scenario-based testing with pass/fail criteria

### 1.3 Numbering Convention
All unified requirements use the prefix `REQ-SF-` followed by a four-digit number. Categories are assigned number ranges per Section 2.

### 1.4 Deduplication Summary
- **160** raw requirements consolidated into **119** unified requirements
- **41** raw requirements were merged into existing unified requirements (overlap/duplication)
- **0** raw requirements were dropped (all capability preserved)
- Where requirements overlapped, the most stringent tolerance/threshold from any domain was adopted

---

## 2. Requirement Categories and Numbering

| Range | Category |
|-------|----------|
| REQ-SF-0100–0199 | Platform & Build |
| REQ-SF-0200–0299 | Simulation Engine |
| REQ-SF-0300–0399 | Atmosphere & Weather |
| REQ-SF-0400–0499 | Terrain Engine |
| REQ-SF-0500–0599 | Flight Planning & Patterns |
| REQ-SF-0600–0699 | Flight Controller & Autopilot |
| REQ-SF-0700–0799 | Sensor Modeling |
| REQ-SF-0800–0899 | Airspace & Regulatory |
| REQ-SF-0900–0999 | Threat & Hazard Modeling |
| REQ-SF-1000–1099 | Scenario & Mission System |
| REQ-SF-1100–1199 | Multi-Vehicle |
| REQ-SF-1200–1299 | Visualization & Rendering |
| REQ-SF-1300–1399 | Verification & Test Harness |
| REQ-SF-1400–1499 | Traceability & Certification |
| REQ-SF-1500–1599 | Data Recording & Reporting |
| REQ-SF-1600–1699 | Security & Access Control |
| REQ-SF-1700–1799 | Interoperability & Interfaces |
| REQ-SF-1800–1899 | Performance |
| REQ-SF-1900–1999 | GUI & User Interface |

---

## 3. Unified Requirements

---

### 3.1 Platform & Build (REQ-SF-0100–0199)

```
REQ-SF-0100: Cross-Platform Build Support
The system shall build and execute on Windows 10+ (x86-64) and Ubuntu 22.04+ Linux
(x86-64), using CMake 3.24+ as the build system generator, with C++17 as the
minimum language standard. All platform-specific code shall be isolated behind a
platform abstraction layer.
Origin: REQ-DEF-034 (cross-platform determinism), REQ-COM-033 (batch execution on
workstations), REQ-SAR-025 (platform identifier logging)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 7.x (Project Scaffolding & Architecture)
```

```
REQ-SF-0101: Air-Gapped Operation Mode
The system shall be fully operational in an air-gapped (network-isolated)
environment with no dependency on external network services for any simulation,
analysis, or reporting function. The system shall support installation and updates
via approved removable media transfer procedures. No license server or cloud
service dependency shall exist.
Origin: REQ-DEF-023
Domains: DEF
Priority: Critical
WBS: 7.x (Project Scaffolding & Architecture)
```

```
REQ-SF-0102: Dependency and Library Management
The system shall maintain an auditable bill of materials (BOM) of all third-party
libraries and dependencies, including version numbers, license types, and export
control classification (ITAR/EAR status). The BOM shall be generated automatically
as part of the build process.
Origin: REQ-DEF-022 (ITAR inventory)
Domains: DEF, COM
Priority: High
WBS: 7.x (Project Scaffolding & Architecture)
```

```
REQ-SF-0103: Unit Test Framework Support
The system shall integrate with at least five C++ unit test frameworks: Google Test,
Catch2, Doctest, Boost.Test, and CppUnit. Test discovery, execution, and result
reporting shall be unified through a common test runner interface.
Origin: Implied by all domain documents (all reference 5 test frameworks)
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 13.x (Test Harness & Verification)
```

---

### 3.2 Simulation Engine (REQ-SF-0200–0299)

```
REQ-SF-0200: JSBSim 6-DOF Flight Dynamics Integration
The system shall integrate JSBSim as the primary 6-DOF flight dynamics engine,
computing vehicle state (position, velocity, attitude, angular rates) at a
configurable integration rate of 100 Hz to 400 Hz (default 200 Hz). The JSBSim
aircraft model shall be loadable from standard JSBSim XML configuration files.
Origin: REQ-DEF-032 (200 Hz), REQ-DEF-018 (100 Hz per vehicle for multi-UAS),
REQ-COM-027 (propulsion model), REQ-COM-029 (climb/descent performance)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0201: Mission Phase-of-Flight Decomposition
The system shall decompose each simulated mission into discrete phases of flight
(taxi, takeoff, climb, cruise, loiter, descent, approach, landing, and emergency)
and shall report time, fuel/energy state, and vehicle state vector at each phase
transition boundary. Phase transitions shall be detectable by automated pass/fail
criteria.
Origin: REQ-DEF-004
Domains: DEF, COM, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0202: Engine-Out and Powerplant Failure Simulation
The system shall simulate single and multi-engine failure conditions at any point
during the mission, accurately modeling asymmetric thrust, drag, and resulting
yaw/roll moments using JSBSim's 6-DOF engine model. The system shall evaluate the
flight controller's ability to maintain controlled flight or execute emergency
landing procedures.
Origin: REQ-DEF-015, REQ-COM-006 (engine/motor failure contingency)
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0203: Control Surface Failure and Battle Damage Simulation
The system shall simulate partial or total failure of individual control surfaces
(ailerons, elevators, rudder, flaps, spoilers) including reduced control surface
effectiveness from 0% to 100% in 1% increments. The system shall evaluate the
flight controller's ability to maintain stability and execute a safe recovery.
Origin: REQ-DEF-016
Domains: DEF, COM
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0204: Payload Effects on CG and Flight Performance
The system shall model the effects of payload mass (0 to MTOW minus empty weight)
and payload center-of-gravity location (longitudinal and lateral, specified in
body-frame coordinates with +/-0.01-inch resolution) on aircraft trim, stability
margins, control authority, and performance parameters (endurance, range, climb
rate, stall speed). The system shall flag any payload configuration causing CG to
exceed forward or aft CG limits.
Origin: REQ-COM-024, REQ-DEF-003 (payload weight effects)
Domains: DEF, COM, SAR
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0205: Endurance and Range Computation with Energy Reserve
The system shall compute mission endurance and range using the JSBSim propulsion
model integrated with a battery discharge model (Shepherd model or equivalent with
SOC, voltage sag, temperature dependence, and C-rate effects) or fuel consumption
model (BSFC maps). All computations shall account for a configurable minimum energy
reserve (default: 20% for electric, 30 minutes for fuel-powered). Battery capacity
shall degrade at -3%/10 deg C below 20 deg C.
Origin: REQ-COM-027, REQ-SAR-016 (energy vs. distance-to-home),
REQ-DEF-003 (fuel estimates within 5%)
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0206: Takeoff and Landing Distance Modeling
The system shall compute takeoff distance, landing distance, and accelerate-stop
distance for fixed-wing UAS using the JSBSim ground reaction model, accounting for
runway surface type (paved, grass, gravel, dirt), surface slope (+/-10 deg), headwind/
tailwind component (0-30 knots), density altitude (sea level to 10,000 feet), and
aircraft gross weight. Results shall be reported with +/-5% accuracy tolerance
relative to validated flight test data when available.
Origin: REQ-COM-028
Domains: COM, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0207: Climb and Descent Performance Envelope
The system shall compute rate of climb, rate of descent, Vx (best angle of climb
speed), Vy (best rate of climb speed), service ceiling (altitude where ROC = 100
fpm), and time-to-climb profiles across the aircraft's weight and CG envelope. All
computations shall use the 1976 U.S. Standard Atmosphere and configurable hot-day
(+30 deg F ISA deviation) and cold-day (-30 deg F ISA deviation) models.
Origin: REQ-COM-029, REQ-SAR-013 (density altitude effects)
Domains: COM, DEF, SAR
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0208: Payload Release and Ballistic Footprint Simulation
The system shall simulate payload release (gravity drop, winch delivery, and
parachute-assisted delivery) and compute the payload impact point relative to a
designated drop zone. The system shall account for release altitude, aircraft
velocity, wind drift, and parachute drag. Pass/fail criteria shall be configurable,
with a default CEP threshold of 5 meters for delivery altitudes up to 150 feet AGL.
For FTS/jettison events, the ballistic footprint shall be computed using a 3-DOF
point-mass model with Monte Carlo wind dispersion (minimum 1,000 samples) at 99th
percentile containment.
Origin: REQ-COM-025 (drop zone accuracy), REQ-COM-026 (FTS/jettison)
Domains: COM, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

---

### 3.3 Atmosphere & Weather (REQ-SF-0300–0399)

```
REQ-SF-0300: 1976 U.S. Standard Atmosphere Model
The system shall implement the 1976 U.S. Standard Atmosphere model for computation
of pressure, density, temperature, and speed of sound as functions of geometric
altitude from sea level to 100,000 feet. The model shall support configurable ISA
deviations: hot-day (+30 deg F / MIL-STD-210C 1% hot-day), cold-day (-30 deg F),
and custom temperature offset profiles.
Origin: REQ-COM-029, REQ-SAR-013, REQ-DEF-032, REQ-GIS-027
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0301: Wind Field Modeling with Gusts and Shear
The system shall model wind as a three-dimensional vector field with: (a) sustained
component configurable from 0 to 50 knots, (b) discrete gusts per MIL-F-8785C /
MIL-HDBK-1797, and (c) wind profiles varying with altitude using logarithmic or
power-law boundary layer models. Wind shear layers shall be definable at arbitrary
altitudes. Wind direction and speed shall be configurable per altitude band.
Origin: REQ-COM-015, REQ-SAR-011 (wind abort), REQ-GIS-026 (survey wind limits),
REQ-INF-016 (localized turbulence), REQ-DEF-005 (NOE wind effects)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0302: Continuous Turbulence Modeling (Dryden/von Karman)
The system shall model continuous turbulence using the Dryden or von Karman spectral
models, selectable by the operator. Turbulence intensity levels shall be configurable
as light, moderate, severe, and extreme per the ICAO turbulence classification. The
turbulence model shall accept altitude-dependent scale lengths and intensities.
Origin: REQ-COM-015, REQ-INF-016, REQ-SAR-011 (implied by wind modeling),
REQ-DEF-005 (implied by NOE flight)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0303: Icing Condition Simulation
The system shall simulate airframe icing effects on aerodynamic performance,
including increased drag (up to 40% Cd increment for moderate icing), decreased
lift (up to 30% CL_max reduction), increased weight, and control surface
effectiveness degradation. Icing accumulation rate shall be modeled as a function of
liquid water content (LWC: 0-3 g/m3), mean volume diameter (MVD: 10-50 um), outside
air temperature (0 deg C to -40 deg C), and airspeed per NASA Glenn icing models.
Origin: REQ-COM-016
Domains: COM, DEF
Priority: Medium
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0304: Visibility and Ceiling Modeling
The system shall model atmospheric visibility (0 to unlimited statute miles) and
cloud ceiling (0 to unlimited feet AGL). The system shall validate simulated
operations against applicable visibility and cloud clearance requirements: Part 107
minimum 3 statute miles visibility; Part 91 VFR minimums in Class E airspace. The
system shall flag any mission segment violating these minimums. Visibility shall
affect simulated EO/IR sensor detection range per user-supplied lateral range curves.
Origin: REQ-COM-017, REQ-SAR-012
Domains: COM, SAR, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0305: Density Altitude Performance Impact
The system shall compute density altitude from pressure altitude, temperature, and
humidity using the 1976 U.S. Standard Atmosphere. The JSBSim flight dynamics model
shall reflect reduced thrust/lift at high density altitudes. The system shall flag
any scenario in which the computed density altitude exceeds the UAV's published
service ceiling by more than 500 ft.
Origin: REQ-SAR-013, REQ-COM-029 (implied)
Domains: SAR, COM, DEF, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0306: Localized Thermal Updraft and Structure-Induced Turbulence
The system shall model localized thermal updrafts (vertical gust velocity 1-8 m/s)
and mechanical turbulence (Dryden or von Karman spectrum, intensity light/moderate/
severe per MIL-HDBK-1797) in the vicinity of sun-heated structures and downstream
of large obstacles. The turbulence region geometry and intensity shall be
configurable per obstacle instance.
Origin: REQ-INF-016
Domains: INF, DEF
Priority: Medium
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0307: Wind Turbine Rotor Wake Modeling
The system shall model wind turbine wake effects using a simplified Jensen or
Bastankhah wake model with configurable thrust coefficient (0.2-0.9) and ambient
turbulence intensity (5-25%). The wake-induced velocity deficit and added turbulence
shall be applied to the JSBSim atmospheric model when the simulated UAV falls within
the computed wake cone.
Origin: REQ-INF-017
Domains: INF
Priority: Medium
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0308: Solar Position Computation for Shadow Avoidance
The system shall compute solar azimuth and elevation angle for any user-specified
date, time, and geographic position using the NOAA Solar Position Algorithm (SPA,
uncertainty +/-0.0003 deg). The system shall warn if solar elevation falls below a
configurable minimum (default 30 deg above horizon) and shall compute predicted
shadow length ratios.
Origin: REQ-GIS-025
Domains: GIS, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0309: Atmospheric Refraction Correction
The system shall apply atmospheric refraction correction to simulated LiDAR range
measurements using the configured atmospheric profile to compute the refractive
index along the beam path. The correction shall reduce range measurement bias due
to refraction to 1 mm or less for scan angles up to 20 deg and AGL up to 500 m.
Origin: REQ-GIS-027
Domains: GIS
Priority: Low
WBS: 8.x (Simulation Engine)
```

---

### 3.4 Terrain Engine (REQ-SF-0400–0499)

```
REQ-SF-0400: Multi-Format Terrain Data Ingestion
The system shall ingest terrain elevation data in the following formats: DTED Level 0
(30 arc-sec), Level 1 (3 arc-sec), and Level 2 (1 arc-sec); GeoTIFF DEM (any
EPSG-defined CRS, 16-bit signed integer and 32-bit float); LAS/LAZ LiDAR point
clouds (versions 1.2-1.4, classes 2/ground and 6/building). The system shall
resample all sources onto a unified regular grid with configurable posting (0.5 m -
30 m) using bilinear interpolation and shall report RMS residual after resampling.
Origin: REQ-GIS-010, REQ-DEF-005 (DTED Level 2), REQ-INF-008 (LiDAR obstacle import)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0401: LiDAR Point Cloud Integration for Urban Terrain
The system shall ingest LiDAR point cloud data (LAS/LAZ format, up to 50 points per
square meter density) to model urban terrain features including buildings, towers,
and wire obstacles. Maximum point cloud dataset size shall be 10 GB per mission
area. Classified point cloud data (ASPRS classes 14-17 for wire, tower, bridge deck)
shall be automatically converted to collidable obstacle geometry with a point-to-
surface reconstruction tolerance of 0.30 m or less.
Origin: REQ-DEF-007, REQ-INF-008
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0402: Terrain Collision Detection (CFIT Prevention)
The system shall perform continuous terrain collision detection during simulation at
a minimum rate of 50 Hz using loaded DTED or DEM data. The system shall implement a
configurable look-ahead distance of no less than 60 seconds of flight time (based on
current ground speed and flight path angle). A CFIT warning shall be generated when
the predicted flight path intersects terrain within the look-ahead window. A CFIT
caution shall be generated at 1.5x the clearance threshold. Terrain clearance
threshold shall be configurable from 25 to 500 feet AGL (default: 100 ft en route,
50 ft approach). Maximum false alarm rate shall be less than 1% of total look-ahead
evaluations.
Origin: REQ-DEF-008 (50 Hz, <1% false alarm), REQ-COM-034 (60-sec lookahead,
configurable clearance), REQ-SAR-009 (30-sec lookahead, audible/visual warning),
REQ-INF-004 (terrain-following collision avoidance)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0403: 3D Building and Obstacle Data Ingestion
The system shall ingest 3D building and obstacle data in CityGML LOD2,
OpenStreetMap 3D Buildings, GeoJSON, Shapefile, and KML/KMZ formats. Point
obstacles with height, polyline obstacles (e.g., power lines), and polygon no-fly
zones shall be supported. The system shall enforce configurable minimum horizontal
clearance (default 50 ft) and vertical clearance (default 25 ft) from all
obstacles. Any flight path segment violating clearance shall be flagged with the
specific obstacle identified by coordinates and type.
Origin: REQ-COM-023, REQ-GIS-023, REQ-INF-005 (transmission structures)
Domains: COM, GIS, INF, DEF
Priority: Critical
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0404: Parameterized Infrastructure Obstacle Library
The system shall provide a parameterized obstacle library for: lattice towers
(height 20-80 m), monopole towers (15-50 m), H-frame wood poles (10-25 m),
catenary conductor spans (sag 2-30 m, span 50-500 m, conductor diameter 10-50 mm,
bundle count 1-4), guy wires (diameter >= 6 mm), exhaust stacks (height 5-120 m),
flare booms, lightning masts, and antenna arrays. Conductor positions shall be
accurate to +/-0.25 m relative to defined attachment points. All objects shall be
collidable and georeferenced to +/-0.5 m.
Origin: REQ-INF-005, REQ-INF-007
Domains: INF, DEF, GIS
Priority: High
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0405: Wind Turbine Structure Modeling
The system shall model wind turbines with configurable hub height (60-170 m), rotor
diameter (40-170 m), blade count (2 or 3), and rotational speed (0-20 RPM). Blade
positions shall update at the simulation time step (10 ms or less) so that
collision-detection logic can be validated against rotating geometry.
Origin: REQ-INF-006
Domains: INF
Priority: High
WBS: 9.x (Terrain Engine)
```

```
REQ-SF-0406: Elevation Query API
The system shall provide a terrain elevation query API that returns ground elevation
(HAE and orthometric) for any WGS-84 coordinate within the loaded terrain extent,
with a query latency of no more than 1 microsecond per point query on the reference
hardware. The API shall support batch queries of up to 1 million points.
Origin: REQ-GIS-009 (terrain following needs fast queries), REQ-DEF-008 (50 Hz CFIT
needs fast queries), REQ-INF-004 (look-ahead terrain following)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 9.x (Terrain Engine)
```

---

### 3.5 Flight Planning & Patterns (REQ-SF-0500–0599)

```
REQ-SF-0500: ISR and Waypoint Mission Route Planning
The system shall support mission route planning with specification of waypoints,
named areas of interest (NAIs), target areas of interest (TAIs), loiter orbits
(racetrack and figure-eight patterns), and sensor look-angles, with a minimum route
complexity of 200 waypoints per mission plan.
Origin: REQ-DEF-001
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0501: Grid/Lawnmower Flight Plan Generation
The system shall generate nadir grid (lawnmower/boustrophedon) flight plans with
parallel, equally-spaced flight lines. Line spacing shall be derived from sensor
footprint, specified side overlap percentage (20%-90%, default 60%), and computed
GSD. Flight line bearing shall be configurable in 0.01 deg increments over
[0 deg, 360 deg).
Origin: REQ-GIS-001, REQ-INF-013 (solar panel lawnmower)
Domains: GIS, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0502: Crosshatch (Double-Grid) Flight Plan Generation
The system shall support crosshatch flight plans by composing two grids with
flight-line bearings differing by a configurable angle (default 90 deg, range
45-90 deg), with independent overlap settings per axis, merged into a single unified
mission with optimized transition waypoints minimizing non-productive ferry time.
Origin: REQ-GIS-002
Domains: GIS
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0503: Corridor Mapping Flight Plan Generation
The system shall generate corridor mapping flight plans following a user-defined
polyline centerline (minimum vertex spacing: 1 m) with configurable swath width
(1 m - 2000 m). Curved or racetrack turn transitions shall be automatically inserted
at vertices with heading change exceeding 5 deg. Camera/LiDAR footprint shall remain
within corridor boundary with no more than 5% swath gap at any turn.
Origin: REQ-GIS-003, REQ-INF-001 (corridor from GIS centerline),
REQ-INF-002 (along-corridor speed control), REQ-INF-003 (bidirectional/multi-leg)
Domains: GIS, INF, COM
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0504: Oblique Image Capture Flight Plan
The system shall support oblique imaging flight plans with camera gimbal pitch
configurable from 0 deg (nadir) to 60 deg off-nadir in 1 deg increments. The flight
planner shall automatically adjust line spacing and photo interval to compensate for
the enlarged and trapezoidal ground footprint, maintaining effective forward and side
overlap within +/-2% of specified values.
Origin: REQ-GIS-004
Domains: GIS, INF
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0505: Orbital (Point-of-Interest) Flight Plan
The system shall generate orbital flight plans flying concentric circular or
elliptical orbits around a user-specified POI, with configurable radius (5 m -
5000 m), altitude offset, camera look-angle toward the POI, and angular photo
interval (default 10 deg, range 1-45 deg), ensuring >= 60% image overlap between
consecutive exposures.
Origin: REQ-GIS-005, REQ-INF-011 (orbit inspection pattern)
Domains: GIS, INF, DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0506: SAR Expanding Square Search Pattern
The system shall simulate the expanding square search (SS) pattern per IAMSAR Manual
Volume II with configurable initial leg length (50 m - 2000 m), track spacing equal
to sensor sweep width (+/-2% tolerance), and automatic leg-length incrementing.
Cross-track error shall be no more than +/-5 m RMS under calm conditions.
Origin: REQ-SAR-001
Domains: SAR, DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0507: SAR Sector Search Pattern
The system shall simulate the sector search (VS) pattern per IAMSAR Manual Volume II
with a minimum of three passes through the datum at evenly spaced headings (default
120 deg, configurable 60-180 deg), with turn radius constrained by the simulated
UAV's bank-angle limit.
Origin: REQ-SAR-002
Domains: SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0508: SAR Parallel Track Search Pattern
The system shall simulate the parallel track (PS) search pattern per IAMSAR Manual
Volume II with configurable corridor width (50 m - 5000 m), track spacing (10 m -
1000 m), and search direction. The system shall compute total area covered (km2) and
cumulative probability of detection using user-specified sensor sweep width and
lateral range curve.
Origin: REQ-SAR-003
Domains: SAR, DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0509: SAR Creeping Line Ahead Search Pattern
The system shall simulate the creeping line ahead (CS) search pattern per IAMSAR
Manual Volume II with search axis aligned to user-specified bearing, configurable
track spacing, and configurable turn direction. Deviation from planned ground track
exceeding a user-configurable threshold (default +/-10 m) shall be reported.
Origin: REQ-SAR-004
Domains: SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0510: Strike Mission Profile Simulation
The system shall simulate strike mission profiles including ingress routing, weapon
release envelopes, terminal guidance trajectories, and egress routing, with time-on-
target windows with a tolerance of +/- 1 second relative to planned TOT.
Origin: REQ-DEF-002
Domains: DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0511: Logistics and MEDEVAC Route Planning
The system shall simulate logistics resupply and MEDEVAC delivery routes including
payload weight effects on flight dynamics, landing zone approach/departure path
obstruction analysis, and fuel consumption estimates accurate to within 5% of
JSBSim-computed values.
Origin: REQ-DEF-003
Domains: DEF, SAR
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0512: Vertical Scan Pattern for Structure Inspection
The system shall generate a vertical scan (raster) pattern on a user-defined
vertical plane with configurable width (2-50 m), height (5-150 m), horizontal line
spacing (0.5-5.0 m), scan speed (0.5-5.0 m/s), and standoff distance (5-30 m).
Camera boresight shall be maintained perpendicular to the scan plane within +/-3 deg.
Origin: REQ-INF-012
Domains: INF, GIS
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0513: Panel-by-Panel Solar Array Inspection Pattern
The system shall generate a lawnmower flight pattern over a georeferenced solar array
boundary polygon with configurable flight-line spacing (1-20 m), altitude AGL
(10-50 m), ground speed (1-8 m/s), and camera trigger interval. The pattern shall
account for panel tilt angle (0-45 deg) and row azimuth for optimal flight-line
heading.
Origin: REQ-INF-013
Domains: INF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0514: Wind Turbine Blade-Track Inspection Pattern
The system shall generate a flight path following each individual blade of a
stationary wind turbine from root to tip at configurable standoff distance (5-20 m,
+/-0.5 m) and camera angle (0-45 deg from blade surface normal, +/-2 deg), with
automatic repositioning between blades.
Origin: REQ-INF-014
Domains: INF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0515: Vertiport Approach and Departure Procedure Simulation
The system shall simulate vertiport approach/departure procedures per FAA Engineering
Brief No. 105 and EASA PTS-VPT-DSN, including FATO area dimensions, approach/
departure surface gradients (minimum 12.5% / 8:1), and obstacle clearance. Up to 6
independent approach/departure corridors per vertiport shall be supported.
Origin: REQ-COM-021
Domains: COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0516: Target Drift Compensation for Maritime SAR
The system shall model lateral target drift due to wind and current using a
configurable drift vector (speed 0-30 kt, direction 0-360 deg). The system shall
automatically recompute search datum position at user-specified intervals (default
every 15 minutes simulated time) per IAMSAR drift-calculation methodology.
Origin: REQ-SAR-008
Domains: SAR
Priority: High
WBS: 12.x (Scenario & Mission System)
```

---

### 3.6 Flight Controller & Autopilot (REQ-SF-0600–0699)

```
REQ-SF-0600: Pluggable Flight Controller Interface
The system shall provide a standardized API for integrating external flight
controller and autopilot implementations. The API shall support command inputs
(waypoints, attitude commands, throttle), telemetry outputs (state vector, sensor
data), and parameter configuration. Multiple flight controller implementations
shall be selectable per scenario without recompilation.
Origin: Implied by all domains (pluggable flight controllers)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0601: Terrain-Following Autopilot Mode
The system shall support a terrain-following mode maintaining constant AGL over a
loaded DEM surface, with target AGL configurable from 15 m to 500 m. The altitude
controller shall hold AGL within +/-2 m for terrain slopes up to 30 deg and terrain
undulation frequencies down to 50 m wavelength at ground speeds up to 15 m/s. Look-
ahead distance shall be configurable from 50 m to 500 m with maximum climb/descent
rate constraint (1 m/s to 6 m/s).
Origin: REQ-GIS-009, REQ-INF-004, REQ-DEF-005 (NOE 15-100 m AGL)
Domains: GIS, INF, DEF, SAR
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0602: Lost-Link Procedure Simulation
The system shall simulate data link loss scenarios including complete link failure,
intermittent link (duty cycle 1%-99%), and degraded bandwidth (1 kbps to full
capacity). Upon link loss, the system shall execute configurable recovery behaviors:
(a) loiter at current position for timeout (30-300 s), (b) climb to safe altitude
(>= 50 m above highest obstacle within 500 m) then proceed to rally point, (c)
return-to-base via pre-computed obstacle-free corridor, or (d) mission continue. The
selected behavior shall maintain standoff distances from all obstacles and energized
infrastructure throughout the recovery maneuver. Maximum response time from link loss
detection to contingency initiation shall be 3 seconds.
Origin: REQ-DEF-013 (link degradation), REQ-COM-006 (C2 link loss contingency, 3-sec
response), REQ-INF-026 (lost-link near energized infrastructure), REQ-SAR-021 (link
budget modeling)
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0603: Return-to-Home Path Verification
The system shall compute and simulate the return-to-home flight path considering
terrain obstacles (minimum clearance 50 ft AGL using loaded DTED/DEM), active
geofence boundaries, remaining battery energy, and prevailing wind conditions. The
RTH path shall arrive at the designated home point with a minimum energy reserve of
10% of total capacity.
Origin: REQ-COM-014, REQ-SAR-016 (energy vs. distance-to-home)
Domains: COM, SAR, INF, DEF
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0604: Emergency Landing Site Selection
The system shall evaluate candidate emergency landing sites within the terrain
database based on surface slope (< 10 deg), surface roughness, proximity to
populated areas and energized infrastructure (minimum 30 m from conductors),
surface dimensions (minimum 5 m x 5 m for multi-rotor, 30 m x 5 m for fixed-wing),
and reachability given current energy state and wind. Optimal site shall be selected
within 1 second of emergency trigger. The approach path shall be verified for
obstacle clearance throughout.
Origin: REQ-COM-013, REQ-SAR-017, REQ-INF-027
Domains: COM, SAR, INF, DEF
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0605: GPS-Denied Navigation Simulation
The system shall simulate GPS-denied or GPS-degraded conditions including total GPS
loss, selective signal jamming (L1/L2/L5 independently), and GPS spoofing with user-
defined position offset vectors (up to 10 km offset, 500 m altitude offset). The
system shall evaluate flight controller ability to navigate using alternative means
(INS, visual odometry, terrain-referenced navigation) with position error growth
rate reported in meters per minute.
Origin: REQ-DEF-012, REQ-COM-006 (GNSS denial contingency)
Domains: DEF, COM, SAR
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0606: Dynamic Threat Re-routing
The system shall simulate dynamic threat pop-up events during mission execution
where new threats are activated at user-specified times. The system shall evaluate
the flight controller's autonomous re-route capability within a maximum computation
time (default: 5 seconds).
Origin: REQ-DEF-011
Domains: DEF
Priority: High
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0607: Real-Time Search Area Replanning
The system shall support in-flight modification of search area boundaries, datum
position, and track spacing without mission restart. Modified waypoints shall be
computed and uploaded to the simulated flight controller within 5 seconds. The
system shall log original plan, modification timestamp (UTC, ISO 8601), and revised
plan.
Origin: REQ-SAR-007
Domains: SAR, DEF
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0608: Along-Corridor Station-Keeping Verification
The system shall simulate along-corridor flight at ground speeds between 2 m/s and
25 m/s (+/-0.3 m/s) and log cross-track error at 10 Hz or faster. Deviation
exceeding a configurable threshold (default 3.0 m cross-track, 2.0 m vertical) shall
be flagged.
Origin: REQ-INF-002
Domains: INF, GIS
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-0609: High Wind Abort Criteria
The system shall enforce configurable wind-speed abort thresholds for sustained wind
(default 35 kt) and gust (default 45 kt). When the simulated wind exceeds either
threshold, the system shall trigger automatic RTH, log the abort event, and record
maximum wind speed encountered.
Origin: REQ-SAR-011, REQ-GIS-026 (wind limits for survey quality)
Domains: SAR, GIS, INF, COM
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

---

### 3.7 Sensor Modeling (REQ-SF-0700–0799)

```
REQ-SF-0700: Frame Camera Sensor Model
The system shall model frame-type cameras with configurable: focal length (4-210 mm,
0.01 mm resolution), sensor dimensions (1-100 mm), pixel count (1-200 Mpx), lens
radial distortion (Brown model: K1, K2, K3, P1, P2), and shutter type (global or
rolling with readout time 0-50 ms). The system shall compute instantaneous ground
footprint, GSD, and image motion blur for each exposure.
Origin: REQ-GIS-013, REQ-INF-023 (thermal camera)
Domains: GIS, INF, COM, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0701: LiDAR Sensor Model
The system shall model airborne LiDAR with configurable: scan type (oscillating
mirror, rotating polygon, Palmer/Risley, fiber scanner), max scan half-angle (0-40 deg),
PRF (10 kHz - 2 MHz), scan frequency (1-1000 Hz), beam divergence (0.1-5 mrad),
and max returns per pulse (1-15). The system shall compute predicted ground-level
point density (pts/m2) per 1 m x 1 m cell and report min, max, mean, and standard
deviation per flight line.
Origin: REQ-GIS-014
Domains: GIS, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0702: Multispectral and Thermal Sensor Model
The system shall model multispectral sensors with up to 10 discrete spectral bands,
each with independently configurable center wavelength (350-14000 nm), bandwidth,
sensor array dimensions, and integration time. The system shall compute per-band GSD
and verify coincident spatial coverage within 1 pixel tolerance when in co-registered
mode. For thermal sensors: NETD configurable (default 50 mK), resolution
160x120 to 1280x1024, frame rate 9-60 Hz.
Origin: REQ-GIS-015, REQ-INF-023 (thermal camera specifics), REQ-SAR-019 (thermal
for night SAR)
Domains: GIS, INF, SAR, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0703: Gimbal Simulation (2-Axis and 3-Axis)
The system shall simulate two-axis (pitch/yaw) and three-axis (pitch/roll/yaw)
gimballed sensors. The gimbal shall track a commanded look-angle or designated target
POI and shall report instantaneous ground footprint, GSD, and incidence angle at each
simulation time step.
Origin: REQ-INF-023, REQ-GIS-004 (oblique angle control)
Domains: INF, GIS, DEF, SAR
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0704: Zoom Level and Working Distance Verification
The system shall model optical zoom (1x-30x) and digital zoom (1x-10x) effects on
effective focal length, GSD, and depth of field. For each inspection waypoint, the
system shall compute minimum detectable feature size (mm) and flag any waypoint
where the detectable feature size exceeds the user-specified inspection resolution
threshold.
Origin: REQ-INF-024
Domains: INF, GIS
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0705: Sensor Degradation and Failure Simulation
The system shall simulate degradation and loss of onboard sensors including IMU,
magnetometer, barometric altimeter, pitot-static system, EO/IR payload, and SAR,
individually or in combination. The system shall evaluate impact on navigation
accuracy, target tracking, and mission effectiveness with quantitative metrics (CEP
growth, track quality, revisit rate degradation).
Origin: REQ-DEF-017, REQ-COM-006 (DAA sensor failure)
Domains: DEF, COM, SAR
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0706: Thermal/IR Detection Probability Model
The system shall model thermal target detection probability as a function of range,
AGL, atmospheric transmittance, target-to-background thermal contrast, sensor NETD,
and environmental conditions (ambient temperature, wind speed). Detection events
shall be logged with timestamp, UAV position, sensor boresight vector, and computed
detection probability.
Origin: REQ-SAR-019
Domains: SAR, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0707: Forward and Side Overlap Verification
The system shall compute realized forward (along-track) and side (across-track)
overlap at every exposure pair and flag any instance where actual overlap deviates
from the specified value by more than +/-3 percentage points, logging photo-pair IDs,
ground coordinates, and deviation magnitude. Forward overlap range: 50%-95%; side
overlap range: 20%-90%, each in 1% increments.
Origin: REQ-GIS-006
Domains: GIS, INF
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0708: Ground Sample Distance (GSD) Computation
The system shall compute instantaneous GSD for each simulated exposure using sensor
pixel pitch, focal length, and terrain-adjusted AGL. A warning shall be generated
when GSD exceeds mission-specified maximum by more than 10% or falls below minimum
by more than 20%.
Origin: REQ-GIS-007
Domains: GIS, INF, COM
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0709: Coverage Gap Detection and Reporting
The system shall rasterize the cumulative sensor ground footprint onto a grid no
coarser than 1/4 of the planned GSD resolution and identify any region within the
survey boundary receiving fewer than the minimum required overlapping exposures
(configurable, default >= 2). Each gap shall be reported with centroid coordinate
(WGS-84), area in m2, and nearest bounding flight-line IDs.
Origin: REQ-GIS-008, REQ-INF-025 (multi-sensor coverage mapping)
Domains: GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-0710: EMI Effects on Sensors Near HV Lines
The system shall model electromagnetic field intensity near modeled conductors and
inject simulated EMI effects on magnetometer (heading error +/-5-30 deg) and GPS
receiver (position error 1-10 m CEP increase) when the UAV enters user-defined EMI
zones. Effects shall be logged with field strength, position, and affected channels.
Origin: REQ-INF-015
Domains: INF, DEF
Priority: High
WBS: 8.x (Simulation Engine)
```

---

### 3.8 Airspace & Regulatory (REQ-SF-0800–0899)

```
REQ-SF-0800: Dynamic Geofence Enforcement
The system shall enforce georeferenced geofence boundaries (polygonal and cylindrical
volumes, WGS-84) with position accuracy of +/-3 meters (95% confidence) and boundary
evaluation rate of no less than 10 Hz. Upon geofence breach, the system shall trigger
the configured containment response (loiter at boundary, return-to-home, land at
nearest ELZ, or flight termination) within 500 milliseconds of breach detection.
Geofences shall support inclusion (stay-in) and exclusion (stay-out) modes.
Origin: REQ-COM-012, REQ-DEF-010 (boundary accuracy 10 m lat/5 m vert),
REQ-INF-028 (2.0-sec response), REQ-SAR-014 (TFR geofencing)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0801: Part 107 Operational Envelope Enforcement
The system shall enforce 14 CFR Part 107 constraints: maximum groundspeed 87 knots,
maximum altitude 400 feet AGL (or within 400 ft of a structure), and daylight/civil
twilight period restrictions with anti-collision lighting requirements.
Origin: REQ-COM-001, REQ-GIS-022 (400 ft AGL ceiling)
Domains: COM, GIS, INF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0802: Part 107 Waiver Condition Simulation
The system shall accept custom regulatory constraint sets representing specific
Part 107 waiver conditions (107.29 night, 107.31 VLOS override, 107.35 operations
over people, 107.37 operations over moving vehicles, 107.39 multi-vehicle, 107.51
altitude override) and validate missions against those constraints, reporting any
exceedance as a test failure.
Origin: REQ-COM-004, REQ-INF-018
Domains: COM, INF, GIS
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0803: Part 135 Air Carrier Constraint Modeling
The system shall support simulation under 14 CFR Part 135 provisions, including crew
duty time modeling, minimum equipment list (MEL/CDL) compliance, and operational
control structures with dispatcher-PIC authority boundaries.
Origin: REQ-COM-002
Domains: COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0804: Remote ID Broadcast Simulation per ASTM F3411
The system shall simulate generation and broadcast of Remote ID messages per ASTM
F3411-22a, including UA location, velocity, takeoff location, operator location, and
timestamp at minimum 1 message/second with location accuracy within 1 meter (1 sigma)
of simulated truth state.
Origin: REQ-COM-008
Domains: COM, INF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0805: DAA Well-Clear Volume Modeling per ASTM F3442
The system shall implement DAA well-clear volume definitions per ASTM F3442/F3442M,
including HMD threshold of 2,000 feet and vertical separation threshold of 250 feet
for UAS below 10,000 feet MSL. Time-to-loss-of-well-clear (tLOWC) shall be computed
at minimum 1 Hz.
Origin: REQ-COM-005
Domains: COM, DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0806: Right-of-Way and See-and-Avoid Modeling
The system shall model right-of-way rules per 14 CFR 91.113 and simulate UAS ability
to comply with see-and-avoid obligations, logging any scenario where the UAS would
conflict with manned aircraft right-of-way precedence.
Origin: REQ-COM-003
Domains: COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0807: UTM/USS Service Interface Simulation
The system shall provide a pluggable interface for USS interactions per FAA UTM
ConOps v2.0, including operation plan submission, strategic deconfliction (4D volume
reservations), conformance monitoring alerts, and off-nominal notifications. USS
response latency (0-5,000 ms) and failure modes shall be configurable.
Origin: REQ-COM-009
Domains: COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0808: LAANC Authorization Simulation
The system shall simulate the LAANC workflow including UASFM ceiling altitude lookup
for controlled airspace grid cells, authorization request submission, and approval/
denial responses. The mission planner shall not generate routes penetrating
controlled airspace without simulated LAANC approval.
Origin: REQ-COM-010
Domains: COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0809: ADS-B Traffic Injection
The system shall inject simulated ADS-B Out traffic targets (1090ES and 978 UAT) with
configurable ICAO address, position (WGS-84), altitude, velocity, NACp, NACv, SIL,
and SDA at densities up to 200 simultaneous targets within 15 NM of the simulated
UAS.
Origin: REQ-COM-011
Domains: COM, DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0810: TFR and Airspace Restriction Enforcement
The system shall ingest TFR and airspace boundary definitions (Classes B/C/D/E, TFRs,
NOTAMs) in GeoJSON, AIXM, or KML format. The system shall flag any simulated flight
segment penetrating restricted or controlled airspace without authorization. No-fly
zone polygons shall be treated as prohibited volumes. Boundary detection accuracy
shall be 10 m lateral and 5 m vertical. At least 20 simultaneous TFRs per scenario
shall be supported.
Origin: REQ-DEF-010, REQ-SAR-014, REQ-INF-020, REQ-GIS-023 (no-fly zones)
Domains: DEF, COM, SAR, INF, GIS
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0811: Manned Aircraft Deconfliction Altitude Bands
The system shall support altitude deconfliction bands and fail any test where the
simulated UAV exceeds its assigned ceiling or descends below its floor by more than
a configurable tolerance (default +/-25 ft). Simulated manned-aircraft traffic
injection shall be supported to verify DAA logic.
Origin: REQ-SAR-015
Domains: SAR, COM, DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0812: Maximum and Minimum AGL Altitude Enforcement
The system shall enforce configurable maximum AGL ceiling (default 120 m / 400 ft,
configurable to 600 m for waiver scenarios) and minimum AGL floor (default 15 m).
Waypoints or segments violating limits shall be flagged at plan-generation time.
Runtime exceedances beyond 1 m shall generate logged regulatory events. During
terrain-following, if descent rate would exceed aircraft limits, a modified profile
shall be computed and the AGL/GSD trade-off reported.
Origin: REQ-GIS-022, REQ-GIS-024
Domains: GIS, COM, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0813: NERC CIP Cybersecurity Zone Awareness
The system shall support tagging geographic zones as NERC CIP-protected (substations,
control centers, generation facilities) and flag any flight path entering a CIP-
protected zone boundary (configurable buffer 0-500 m). Facility imagery or
geolocation data from CIP zones shall not be exported without explicit authorization.
Origin: REQ-INF-019
Domains: INF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0814: Night Operations Lighting Compliance
The system shall simulate position and anti-collision lighting per 14 CFR 107.29.
Light visibility range shall be modeled as a function of atmospheric visibility and
background luminance. The system shall verify anti-collision lights meet the 3 SM
visibility requirement. Non-compliance shall be logged as a test failure.
Origin: REQ-SAR-018
Domains: SAR, COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0815: Population Density Overlay for Ground Risk
The system shall ingest georeferenced population density data (U.S. Census Bureau
TIGER/Line or equivalent, 1 km2 resolution or finer) and compute ground risk
classifications per JARUS SORA Annex F, enabling automatic GRC determination (1-10)
for any point along the flight path.
Origin: REQ-COM-007
Domains: COM, SAR
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

---

### 3.9 Threat & Hazard Modeling (REQ-SF-0900–0999)

```
REQ-SF-0900: Weapon Engagement Zone (WEZ) Modeling
The system shall model threat systems as 3D weapon engagement zones defined by
minimum and maximum slant range, altitude floor and ceiling, azimuth sector, and
probability of kill (Pk) contours. A minimum of 100 concurrent threat systems per
scenario shall be supported.
Origin: REQ-DEF-009
Domains: DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0901: Electronic Warfare Emitter Modeling
The system shall model electromagnetic emitters including radar systems (frequency,
PRF, beamwidth, sidelobe levels, scan pattern) and communication jammers (center
frequency, bandwidth, effective radiated power). A minimum of 200 concurrent
emitters per scenario shall be supported.
Origin: REQ-DEF-014
Domains: DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0902: Terrain Masking Effectiveness Analysis
The system shall compute and display line-of-sight masking effectiveness between the
simulated UAS position and up to 50 user-defined threat emitter locations, updating
at minimum 10 Hz. The system shall report cumulative percentage of mission time the
vehicle is masked from each threat.
Origin: REQ-DEF-006
Domains: DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0903: Dynamic Standoff Envelope Enforcement
The system shall compute minimum standoff envelopes around energized conductors and
rotating components with configurable radii (default: 10 m for conductors <= 230 kV,
15 m for 230-500 kV, 20 m for >= 500 kV, 30 m for rotating wind turbine blades). A
standoff violation shall be logged with timestamp, 3D position, and closest-approach
distance. Three concentric warning zones (advisory at 2x, caution at 1.5x, warning
at 1.0x standoff radius) shall generate distinct events.
Origin: REQ-INF-009, REQ-INF-010
Domains: INF, GIS
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0904: Wildfire Perimeter and Thermal Radiation Modeling
The system shall model wildfire thermal radiation as a function of fire intensity
(kW/m of fire front) and distance. Minimum safe standoff distance (default 300 m)
shall be enforced. Time-evolving fire perimeters (GeoJSON polygons with timestamps)
shall be supported, with the system verifying the UAV path remains outside the
perimeter plus standoff buffer at all times.
Origin: REQ-SAR-027
Domains: SAR
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0905: HAZMAT Plume Avoidance Modeling
The system shall integrate a Gaussian plume dispersion model (Pasquill-Gifford
stability classes) defining a 3D hazardous airspace volume. The UAV trajectory shall
be checked at >= 1 Hz. Penetration of the plume boundary (concentration exceeding
IDLH for the specified agent) shall be logged as a test failure. The plume shall
update dynamically with changing wind conditions.
Origin: REQ-SAR-028
Domains: SAR, DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-0906: Community Noise Impact Modeling
The system shall compute A-weighted SEL and DNL/Ldn noise contours at ground level
for eVTOL and multirotor operations, using source noise models (50 Hz - 10 kHz),
distance attenuation, atmospheric absorption (SAE ARP 866B), and ground reflection.
Contours shall be generated as georeferenced GeoTIFF at 25 m or finer resolution.
Origin: REQ-COM-022
Domains: COM
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

---

### 3.10 Scenario & Mission System (REQ-SF-1000–1099)

```
REQ-SF-1000: Scenario Definition and Configuration
The system shall define each mission scenario as a versioned, diff-able configuration
file (JSON or YAML) capturing all parameters: waypoints, sensor settings, atmospheric
conditions, obstacle placements, failure injections, and pass/fail thresholds. The
system shall detect and report parameter deltas between scenario versions.
Origin: REQ-INF-022, REQ-SAR-025 (input parameter logging)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1001: Failure Injection Framework
The system shall support injection of the following failure modes at any user-
specified point or probabilistically during simulation: (a) engine/motor failure,
(b) control surface failure, (c) sensor failure/degradation, (d) GPS denial/spoofing,
(e) communication link loss, (f) geofence breach, (g) DAA sensor failure, and (h)
battery/fuel anomaly. Each failure shall be independently configurable in timing,
severity, and duration.
Origin: REQ-DEF-015 (engine-out), REQ-DEF-016 (control surface), REQ-DEF-017
(sensor), REQ-DEF-012 (GPS), REQ-DEF-013 (comm link), REQ-COM-006 (BVLOS
contingencies)
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1002: Scenario Pass/Fail Criteria Engine
The system shall evaluate each scenario against configurable pass/fail criteria
including but not limited to: geofence compliance, CFIT avoidance, energy reserve at
landing, cross-track error, altitude deviation, timing deviation, coverage percentage,
overlap thresholds, standoff maintenance, and regulatory constraint compliance. Each
criterion shall be independently evaluable with per-criterion pass/fail results.
Origin: REQ-SAR-029, REQ-INF-030, REQ-COM-033
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1003: Pre-Built Scenario Templates
The system shall provide at least 10 pre-built scenario templates covering: (a)
mountain SAR above 10,000 ft, (b) maritime SAR, (c) urban disaster, (d) wildfire
perimeter mapping, (e) HAZMAT plume avoidance, (f) coastal cliff search, (g) canyon
operations, (h) border patrol corridor, (i) night thermal search, and (j) multi-UAV
relay communications. Each template shall include default terrain, weather, and
pattern parameters that may be overridden.
Origin: REQ-SAR-006
Domains: SAR, DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1004: Rapid Scenario Initialization
The system shall allow creation of a new scenario from a single geographic coordinate
(WGS-84) and search radius, and have the scenario ready for execution (terrain
loaded, dynamics initialized, atmosphere configured) within 120 seconds on the
reference hardware (8-core CPU, 32 GB RAM, NVMe SSD, discrete GPU >= 8 GB VRAM).
Origin: REQ-SAR-005
Domains: SAR, DEF
Priority: Critical
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1005: Scenario Trigger and Event System
The system shall support user-defined triggers based on simulation time, vehicle
position, vehicle state, or external events. Triggers shall activate configurable
actions including failure injection, threat pop-up, weather change, search area
modification, or mission phase transition.
Origin: REQ-DEF-011 (dynamic threat pop-up), REQ-SAR-007 (replanning),
REQ-INF-026 (link loss at specified point)
Domains: DEF, COM, INF, SAR
Priority: High
WBS: 12.x (Scenario & Mission System)
```

---

### 3.11 Multi-Vehicle (REQ-SF-1100–1199)

```
REQ-SF-1100: Multi-UAS Coordinated Mission Simulation
The system shall simultaneously simulate a minimum of 20 UAS vehicles in a single
scenario, each with independent 6-DOF flight dynamics computed at a minimum of
100 Hz per vehicle. Inter-vehicle state data shall be exchanged at minimum 10 Hz to
support formation flight, task allocation, and collision avoidance.
Origin: REQ-DEF-018, REQ-COM-032 (20 vehicles), REQ-SAR-026 (4+ UAVs cooperative)
Domains: DEF, COM, SAR
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1101: Swarm Behavior Verification and Emergent Behavior Detection
The system shall provide automated analysis of swarm behavior including inter-vehicle
spacing statistics (min, max, mean, std dev), formation coherence metrics, and task
completion rates. Emergent behaviors where any vehicle deviates from assigned task,
geofence, or altitude band by more than user-defined thresholds shall be flagged.
Origin: REQ-DEF-019, REQ-COM-032 (loss-of-separation flagging)
Domains: DEF, COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1102: Heterogeneous Vehicle Fleet Simulation
The system shall support simulation of heterogeneous vehicle fleets (fixed-wing,
rotary-wing, VTOL, tilt-rotor) within a single scenario, each using its own JSBSim
aircraft model, performance envelope, and flight controller configuration.
Origin: REQ-DEF-020
Domains: DEF, COM, SAR
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1103: Inter-Vehicle Deconfliction
The system shall verify inter-vehicle separation meets configurable minimums (default
500 ft horizontal / 100 ft vertical for fleet operations; default 30 m for SAR
cooperative search). Loss-of-separation events shall be logged as test failures with
timestamp, vehicle IDs, and separation distance.
Origin: REQ-COM-032, REQ-SAR-026
Domains: COM, SAR, DEF
Priority: High
WBS: 12.x (Scenario & Mission System)
```

---

### 3.12 Visualization & Rendering (REQ-SF-1200–1299)

```
REQ-SF-1200: Vulkan 3D Terrain Rendering
The system shall render the loaded terrain model using the Vulkan graphics API at a
minimum of 30 frames per second at 1920x1080 resolution on the reference hardware
(NVIDIA RTX 3060 or equivalent). Terrain textures, obstacle models, and flight path
overlays shall be rendered simultaneously.
Origin: REQ-DEF-032 (30 fps, 1920x1080), REQ-SAR-020 (rendering performance)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1201: HUD and Mission Overlay Display
The system shall render a configurable heads-up display (HUD) overlay showing at
minimum: airspeed, altitude (MSL and AGL), heading, attitude, battery/fuel state,
GPS status, link status, geofence proximity, and current mission phase. Overlay
elements shall be independently toggleable.
Origin: Implied by DEF-032 (Vulkan visualization), SAR-024 (state monitoring)
Domains: DEF, COM, INF, SAR
Priority: High
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1202: NVG Compatibility Rendering Mode
The system shall provide a rendering mode simulating Gen III night-vision goggle
view (green phosphor, configurable gain and noise). Ambient illumination from lunar
phase, artificial lighting, and atmospheric scattering shall be modeled. NVG
rendering shall operate at >= 30 fps on the reference hardware.
Origin: REQ-SAR-020
Domains: SAR, DEF
Priority: Medium
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1203: Headless Rendering Mode
The system shall support headless (no-GPU-display) execution mode for batch/CI/CD
environments where no display is attached. All simulation computations, pass/fail
evaluation, and data recording shall function identically to rendered mode. Optional
off-screen frame capture shall be supported for report generation.
Origin: REQ-DEF-033 (batch mode), REQ-COM-033, REQ-SAR-029, REQ-GIS-030
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1204: Threat and Hazard Visualization Overlays
The system shall render 3D visualizations of weapon engagement zones, EW emitter
coverage, geofence boundaries, TFR volumes, HAZMAT plumes, wildfire perimeters,
standoff envelopes, communication coverage areas, and terrain masking zones as
semi-transparent colored volumes overlaid on the terrain.
Origin: REQ-DEF-009 (WEZ display), REQ-DEF-006 (masking display),
REQ-SAR-022 (comm coverage map), REQ-INF-010 (standoff zones)
Domains: DEF, SAR, INF, COM
Priority: High
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1205: Post-Simulation Playback
The system shall support post-simulation playback with synchronized 3D visualization
at variable speed (0.1x to 10x real-time) with a playback start latency of no more
than 3 seconds from user command. Playback shall render the recorded vehicle
trajectory, sensor footprints, events, and warnings.
Origin: REQ-DEF-035
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 10.x (Vulkan Render Engine)
```

```
REQ-SF-1206: 3D Flight Path Visualization with Obstacle Proximity Color Coding
The system shall render the simulated flight path as a 3D polyline with color coding
indicating proximity to obstacles: green (> 2x standoff), yellow (1.5x-2x standoff),
orange (1x-1.5x standoff), red (< 1x standoff / violation).
Origin: REQ-INF-030 (obstacle proximity color coding)
Domains: INF, DEF, COM, SAR
Priority: High
WBS: 10.x (Vulkan Render Engine)
```

---

### 3.13 Verification & Test Harness (REQ-SF-1300–1399)

```
REQ-SF-1300: Batch Simulation Execution
The system shall execute a minimum of 1,000 simulation scenarios in unattended
batch/headless mode within an 8-hour overnight window. Each scenario shall comprise
a 60-minute simulated mission with 6-DOF physics at 200 Hz, producing pass/fail
results and data logs without operator intervention. Minimum accelerated time factor
shall be 7.5x real-time per scenario.
Origin: REQ-DEF-033, REQ-COM-033, REQ-SAR-029, REQ-GIS-030 (500 in 12 hrs)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1301: Monte Carlo Campaign Execution
The system shall support Monte Carlo simulation campaigns with parameterized scenario
inputs. Parameters shall be varied via CSV/JSON input files, Latin hypercube
sampling, or full factorial combinations. For each run, the system shall produce
metrics and a summary comparison matrix identifying optimal parameter combinations
against user-specified objective functions.
Origin: REQ-COM-026 (Monte Carlo wind dispersion), REQ-GIS-030 (parameter sweeps),
REQ-COM-033 (parameterized scenarios)
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1302: Automated Pass/Fail Adjudication
The system shall automatically evaluate each scenario against configurable pass/fail
criteria and produce a consolidated results report including pass rate, failure
categorization, and statistical summaries. Results shall be in machine-readable
(JSON/XML) and human-readable (PDF) formats.
Origin: REQ-COM-033, REQ-SAR-029, REQ-INF-030
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1303: Proficiency Check Framework
The system shall support definition and execution of standardized proficiency check
scenarios with configurable pass/fail criteria including: maximum lateral track
deviation (default +/-15 ft), maximum altitude deviation (default +/-10 ft), correct
contingency response, airspace boundary awareness, and completion time. Results
shall be recorded with operator identity, date, scenario ID, and individual scores.
Origin: REQ-COM-031
Domains: COM, SAR
Priority: High
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1304: Part 107 Knowledge Test Study Mode
The system shall provide a scenario-based study mode covering all 12 knowledge areas
tested on the FAA Part 107 examination, presenting simulated flight situations
requiring identification of applicable regulations or correct operating decisions.
Origin: REQ-COM-030
Domains: COM
Priority: Medium
WBS: 13.x (Test Harness & Verification)
```

---

### 3.14 Traceability & Certification (REQ-SF-1400–1499)

```
REQ-SF-1400: DO-178C Verification Evidence Generation
The system shall produce DO-178C compliant verification evidence including
bidirectional requirement-to-test-case traceability matrices, structural coverage
analysis reports (statement, decision, and MC/DC coverage for DAL A/B), and test
procedure/results records. Artifacts shall be generated in machine-readable format
(XML or JSON) and human-readable format (PDF) suitable for FAA DER submission or
DO-178C qualification tool suite import.
Origin: REQ-DEF-029, REQ-COM-018
Domains: DEF, COM, INF
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1401: Requirements Traceability Matrix (RTM) Generation
The system shall maintain and export a bidirectional RTM linking each system
requirement to its associated test scenario(s), test execution result(s), and
deficiency report(s). The system shall flag any requirement lacking a test case or
any test case not linked to a requirement. Traceability shall be verified before each
formal verification event.
Origin: REQ-DEF-031, REQ-INF-030 (DO-178C traceability in evidence packages),
REQ-GIS-029 (traceability in quality reports)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1402: MIL-STD-882E System Safety Analysis Support
The system shall generate artifacts traceable to MIL-STD-882E, including tagging each
scenario with hazard severity (I-Catastrophic through IV-Negligible) and probability
(A-Frequent through E-Improbable). Summary reports shall map simulation pass/fail
results to hazard risk indices for the System Safety Assessment Report (SSAR).
Origin: REQ-DEF-028
Domains: DEF
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1403: JARUS SORA Risk Assessment Data Generation
The system shall compute SORA data elements including ground risk class (GRC), air
risk class (ARC), Specific Assurance and Integrity Levels (SAIL), and Operational
Safety Objectives (OSOs) with robustness levels (Low/Medium/High) per JARUS SORA
Edition 2.0. GRC and ARC shall be derived from simulated operational parameters.
Origin: REQ-DEF-030, REQ-COM-019
Domains: DEF, COM, SAR
Priority: Medium
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1404: EASA SC-RPAS Compliance Mapping
The system shall maintain a configurable compliance matrix mapping SynthFlight
verification capabilities to EASA SC-RPAS and SC-Light-UAS requirements, covering at
minimum: flight envelope protection (SC-RPAS.2510), structural loads (SC-RPAS.2525),
control system integrity (SC-RPAS.2535), and powerplant reliability (SC-RPAS.2545).
Origin: REQ-COM-020
Domains: COM
Priority: Medium
WBS: 13.x (Test Harness & Verification)
```

---

### 3.15 Data Recording & Reporting (REQ-SF-1500–1599)

```
REQ-SF-1500: Comprehensive Flight Data Recording
The system shall record all simulation state data at a minimum rate of 100 Hz
including: timestamp (UTC, microsecond resolution), position (lat, lon, alt MSL and
AGL), attitude (roll, pitch, heading), angular rates, airspeed, groundspeed, vertical
speed, engine/motor parameters (RPM, torque, voltage, current), control surface
deflections, autopilot mode, navigation source, wind vector, energy state, control
inputs, sensor events, and all discrete events (warnings, contingency activations,
geofence breaches). Recording latency shall be no more than 10 ms from state
computation to disk write.
Origin: REQ-DEF-035 (100 Hz, 10 ms latency), REQ-COM-035 (20 Hz minimum, comprehensive
parameters), REQ-SAR-024 (10 Hz, timestamped)
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1501: Tamper-Evident Flight Log with Chain of Custody
The system shall append a SHA-256 hash to each log file as a final record. A
verification utility shall confirm log integrity. Each log shall include all input
parameters, software version (Git commit hash), JSBSim version, and platform
identifier. Logs shall be retained for a minimum of one year or as specified by the
accrediting authority.
Origin: REQ-SAR-024 (SHA-256 hash), REQ-DEF-024 (tamper-evident, 1-year retention)
Domains: SAR, DEF
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1502: Flight Data Export Formats
The system shall export recorded data in CSV and at least one industry-standard format
(IADS, HDF5, or CUAS-specific schema). Data shall support playback within the
SynthFlight 3D visualization environment. Photo station exterior orientation shall be
exportable in comma-separated EO format (image ID, X, Y, Z, omega, phi, kappa).
Origin: REQ-COM-035 (CSV/industry formats), REQ-GIS-016 (EO export)
Domains: COM, GIS, DEF, INF, SAR
Priority: High
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1503: Survey Quality Report Generation
The system shall generate a post-simulation survey quality report (PDF and JSON)
containing: (a) forward and side overlap histogram and spatial heat map, (b) GSD
statistics and spatial map, (c) coverage gap inventory, (d) predicted horizontal and
vertical accuracy (95% confidence), (e) LiDAR point density map (if applicable), (f)
solar angle range, (g) wind condition summary, (h) regulatory compliance status, (i)
estimated data volume, and (j) traceability matrix linking each metric to its
governing requirement.
Origin: REQ-GIS-029
Domains: GIS, INF
Priority: Critical
WBS: 16.x (Documentation & Delivery)
```

```
REQ-SF-1504: Automated Verification Evidence Package
The system shall generate a self-contained evidence package (PDF and JSON/XML)
containing: (a) mission configuration and version hash, (b) pass/fail status per
requirement, (c) time-series plots of cross-track error, AGL, standoff distance, and
airspeed, (d) 3D flight path visualization with obstacle proximity color coding, (e)
coverage map with gap analysis, (f) regulatory compliance checklist, and (g) DO-178C
traceability matrix. The package shall be generated automatically at conclusion of
each batch run.
Origin: REQ-INF-030, REQ-DEF-029 (DO-178C artifacts)
Domains: INF, DEF, COM, SAR
Priority: Critical
WBS: 16.x (Documentation & Delivery)
```

```
REQ-SF-1505: SAR Mission Effectiveness Metrics
The system shall compute and report: (a) cumulative POD based on coverage and sensor
sweep width, (b) POS (POD x POA), (c) total area searched (km2), (d) search speed
(km2/hr), (e) time-on-station vs. total mission time ratio, (f) interruption count
and duration, and (g) coverage completeness. Metrics shall be exportable in CSV and
JSON and traceable to scenario and requirement ID.
Origin: REQ-SAR-030
Domains: SAR
Priority: Critical
WBS: 16.x (Documentation & Delivery)
```

```
REQ-SF-1506: Flight Segment to Asset ID Traceability
The system shall associate each flight segment, waypoint, and sensor capture event
with a user-defined asset identifier hierarchy (e.g., Circuit > Structure Number >
Span Number > Phase; or Pipeline > Mile Marker > Feature). The output shall include a
traceability matrix mapping every event to its asset ID, GPS position, timestamp, and
inspection-quality metrics.
Origin: REQ-INF-029
Domains: INF, GIS
Priority: Critical
WBS: 16.x (Documentation & Delivery)
```

```
REQ-SF-1507: Horizontal Positional Accuracy Budget
The system shall compute per-exposure horizontal positional accuracy (CEP 95%) by
root-sum-squaring: GNSS horizontal error, IMU lever-arm projection error, attitude
uncertainty contribution, wind-induced position error, and trigger latency
displacement. Block-level predicted horizontal RMSE shall be compared against ASPRS
Accuracy Standards.
Origin: REQ-GIS-018
Domains: GIS, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-1508: Vertical Positional Accuracy Budget
The system shall compute per-exposure vertical accuracy (95% confidence) by root-sum-
squaring: GNSS vertical error, barometric altimeter error, DEM/geoid model
uncertainty, terrain-following altitude-hold error, and vertical wind gust
displacement. Predicted accuracy shall be reported and warnings issued if predicted
accuracy exceeds the specified requirement by more than 10%.
Origin: REQ-GIS-019
Domains: GIS, INF
Priority: High
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-1509: Image Count and Storage Volume Estimation
The system shall estimate total exposure count and raw data volume prior to execution,
within +/-5% of actual simulated count. Estimated volume shall be compared against
user-specified storage capacity with a warning at 80% utilization.
Origin: REQ-GIS-020
Domains: GIS, INF
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1510: LiDAR Point Cloud Volume Estimation
The system shall estimate total LiDAR point count and LAZ file size from PRF, scan
parameters, speed, AGL, and terrain slope. Point density estimate shall be within
+/-10% of simulation output. Total count, mean/min density, and estimated file size
shall be reported.
Origin: REQ-GIS-021
Domains: GIS
Priority: Medium
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1511: IMU/GNSS Synchronization Timing Verification
The system shall model GNSS-IMU time synchronization with configurable fixed latency
(0-50 ms) and random jitter (Gaussian, sigma 0-5 ms). The system shall verify total
timestamp error does not cause positional interpolation error exceeding the operator
threshold (default 0.02 m at max ground speed).
Origin: REQ-GIS-017
Domains: GIS
Priority: High
WBS: 8.x (Simulation Engine)
```

---

### 3.16 Security & Access Control (REQ-SF-1600–1699)

```
REQ-SF-1600: CUI Data Handling per NIST SP 800-171
The system shall comply with NIST SP 800-171 Rev. 2 for CUI handling, including
AES-256 encryption at rest (FIPS 140-3 validated module), TLS 1.3+ for data in
transit, and role-based access control with minimum four privilege levels
(administrator, engineer, analyst, viewer).
Origin: REQ-DEF-021
Domains: DEF
Priority: Critical
WBS: 7.x (Project Scaffolding & Architecture)
```

```
REQ-SF-1601: ITAR and Export Control Compliance
The system shall maintain an auditable inventory of all ITAR (22 CFR 120-130) and EAR
(15 CFR 730-774) controlled components. Technical controls shall prevent export of
controlled content from exportable builds and network egress filtering shall block
unauthorized transmission.
Origin: REQ-DEF-022
Domains: DEF
Priority: Critical
WBS: 7.x (Project Scaffolding & Architecture)
```

```
REQ-SF-1602: Audit Logging
The system shall maintain a tamper-evident audit log of all user actions including
login/logout, scenario CRUD operations, simulation execution, data import/export, and
configuration changes. Each entry shall be timestamped to UTC with millisecond
precision and associated with an authenticated user identity. Logs shall be retained
for a minimum of one year.
Origin: REQ-DEF-024, REQ-SAR-024 (timestamped logging)
Domains: DEF, SAR
Priority: High
WBS: 7.x (Project Scaffolding & Architecture)
```

---

### 3.17 Interoperability & Interfaces (REQ-SF-1700–1799)

```
REQ-SF-1700: MAVLink v2.0 Protocol Support
The system shall support MAVLink v2.0 for command, telemetry, and parameter exchange,
including heartbeat, mission item upload/download, parameter read/write, and command
acknowledgment at message rates up to 50 Hz per vehicle.
Origin: REQ-DEF-026, REQ-GIS-028 (MAVLink waypoint protocol)
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-1701: STANAG 4586 Command and Control Interface
The system shall implement STANAG 4586 Edition 3 compliant C2 interface supporting
Data Link Interface Levels 2 through 5, enabling mission commands from and status
reporting to external STANAG 4586 compliant ground control stations.
Origin: REQ-DEF-025
Domains: DEF
Priority: High
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-1702: Link 16 Tactical Data Link Message Simulation
The system shall simulate Link 16 (MIL-STD-6016) J-Series messages including J2.2
(air track), J3.2 (land point track), J7.0 (information management), and J12.6
(mission assignment) to validate UAS integration into tactical data link networks.
Origin: REQ-DEF-027
Domains: DEF
Priority: Medium
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-1703: Flight Plan Import/Export in Industry Formats
The system shall import and export flight plans in: (a) Litchi CSV, (b) DJI FlightHub
KML/KMZ, (c) MAVLink waypoint protocol (QGroundControl .plan JSON), (d) UgCS route
exchange format, and (e) OGC GeoPackage. On import, all waypoints shall be validated
for valid 3D coordinates and extents against loaded terrain. On export, coordinates
shall be transformed to the target format's native CRS.
Origin: REQ-GIS-028
Domains: GIS, INF, COM
Priority: High
WBS: 12.x (Scenario & Mission System)
```

```
REQ-SF-1704: Communication Link Budget Modeling
The system shall model the C2 radio link including: transmit power (100 mW - 2 W),
antenna gain/pattern, free-space path loss at operating frequency (900 MHz, 2.4 GHz,
5.8 GHz), terrain masking with Fresnel zone clearance analysis (first Fresnel zone,
60% clearance threshold), and received signal strength (RSSI in dBm) at >= 1 Hz.
Link loss (RSSI below receiver sensitivity, default -95 dBm) shall be flagged.
Origin: REQ-SAR-021
Domains: SAR, DEF, INF
Priority: Critical
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-1705: Terrain Masking and Communication Dead-Zone Mapping
The system shall compute and display a 2D C2 coverage map showing predicted link
availability overlaid on terrain for a given GCS position and UAV altitude. Dead
zones shall be highlighted. Computation shall complete within 30 seconds for a
10 km x 10 km area at 30 m terrain resolution.
Origin: REQ-SAR-022
Domains: SAR, DEF, INF
Priority: High
WBS: 11.x (Flight Controller / Autopilot Interface)
```

```
REQ-SF-1706: Communication Relay Positioning Optimization
The system shall support simulation of airborne or ground-based relay nodes and
compute optimal relay positions maximizing C2 coverage for a given search region,
subject to relay endurance constraints.
Origin: REQ-SAR-023
Domains: SAR, DEF
Priority: Medium
WBS: 11.x (Flight Controller / Autopilot Interface)
```

---

### 3.18 Performance (REQ-SF-1800–1899)

```
REQ-SF-1800: Real-Time Simulation Factor
The system shall execute single-vehicle 6-DOF simulation at a real-time factor of
1.0x or greater (wall-clock <= simulated time) with JSBSim at 200 Hz, terrain
collision detection at 50 Hz, and Vulkan rendering at 30 fps at 1920x1080 on
reference hardware (Intel Core i7-12700 or equivalent, 32 GB RAM, NVIDIA RTX 3060 or
equivalent).
Origin: REQ-DEF-032
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-1801: Simulation Determinism and Repeatability
The system shall produce bit-identical simulation outputs (vehicle state vector,
event logs, pass/fail determinations) when the same scenario is executed with
identical initial conditions, configuration, and random seed values, across both
Windows 10+ and Ubuntu 22.04+. Position values shall differ by no more than 1e-6
degrees and attitude values by no more than 1e-3 degrees between runs. All input
parameters, software version (Git commit hash), JSBSim version, and platform
identifier shall be logged.
Origin: REQ-DEF-034, REQ-INF-021 (0.01 m RMS / 10 ms trajectory reproducibility),
REQ-SAR-025 (bit-exact for legal proceedings)
Domains: DEF, COM, INF, SAR
Priority: Critical
WBS: 8.x (Simulation Engine)
```

```
REQ-SF-1802: Batch Mode Throughput
The system shall achieve a minimum batch throughput of 1,000 60-minute scenarios per
8-hour window (2.08 scenarios/minute, minimum 7.5x real-time acceleration per
scenario) in headless mode without operator intervention.
Origin: REQ-DEF-033, REQ-COM-033
Domains: DEF, COM, GIS, INF, SAR
Priority: Critical
WBS: 13.x (Test Harness & Verification)
```

```
REQ-SF-1803: Simulation Data Recording Latency
The system shall record all state data with a recording latency of no more than 10 ms
from state computation to disk write. No simulation data shall be lost during
anomalous events.
Origin: REQ-DEF-035
Domains: DEF, COM, SAR
Priority: High
WBS: 13.x (Test Harness & Verification)
```

---

### 3.19 GUI & User Interface (REQ-SF-1900–1999)

```
REQ-SF-1900: Scenario Editor
The system shall provide a graphical scenario editor allowing visual definition of
waypoints, geofences, obstacles, threat zones, search patterns, atmospheric
conditions, and failure injection points on a georeferenced 2D map and 3D terrain
view. Scenario parameters shall be editable via both graphical manipulation and
tabular property panels.
Origin: Implied by all domains requiring scenario definition
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 15.x (GUI Application Shell)
```

```
REQ-SF-1901: Mission Timeline and Phase Display
The system shall provide a timeline panel showing mission phases, events, triggers,
and failures along a time axis. The timeline shall be interactively scrollable and
shall synchronize with the 3D visualization during playback.
Origin: REQ-DEF-004 (phase decomposition display)
Domains: DEF, COM, INF, SAR
Priority: High
WBS: 15.x (GUI Application Shell)
```

```
REQ-SF-1902: Coordinate Reference System Selection
The system shall provide a GUI element for selecting the display and export CRS from:
WGS84 (EPSG:4326), WGS84/UTM zones 1N-60N and 1S-60S, NAD83 State Plane zones, and
any EPSG code resolvable via PROJ >= 9.0. The selected CRS shall apply to all
coordinate displays, exports, and reports. All transformations shall achieve 0.01 m
or better positional accuracy between WGS84 and NAD83(2011).
Origin: REQ-GIS-011
Domains: GIS, DEF, COM, INF
Priority: Critical
WBS: 15.x (GUI Application Shell)
```

```
REQ-SF-1903: Geoid Model Selection
The system shall provide a GUI element for selecting the active geoid model (EGM96 or
EGM2008) per mission. Geoid undulation lookup shall have spatial resolution of
2.5 arc-min (EGM96) or 1 arc-min (EGM2008) with accuracy within +/-0.05 m of
published NGS values. All altitude displays shall clearly label whether the vertical
datum is HAE or orthometric (MSL).
Origin: REQ-GIS-012
Domains: GIS, COM, DEF
Priority: High
WBS: 15.x (GUI Application Shell)
```

```
REQ-SF-1904: Batch Run Dashboard
The system shall provide a dashboard panel for monitoring batch simulation progress,
showing: total scenarios, completed, in-progress, pass/fail counts, estimated time
remaining, and per-scenario status with expandable detail views.
Origin: REQ-DEF-033 (batch execution monitoring), REQ-COM-033
Domains: DEF, COM, GIS, INF, SAR
Priority: High
WBS: 15.x (GUI Application Shell)
```

---

## 4. Requirement Statistics

### 4.1 By Category

| Category | Range | Count |
|----------|-------|-------|
| Platform & Build | 0100-0103 | 4 |
| Simulation Engine | 0200-0208 | 9 |
| Atmosphere & Weather | 0300-0309 | 10 |
| Terrain Engine | 0400-0406 | 7 |
| Flight Planning & Patterns | 0500-0516 | 17 |
| Flight Controller & Autopilot | 0600-0609 | 10 |
| Sensor Modeling | 0700-0710 | 11 |
| Airspace & Regulatory | 0800-0815 | 16 |
| Threat & Hazard Modeling | 0900-0906 | 7 |
| Scenario & Mission System | 1000-1005 | 6 |
| Multi-Vehicle | 1100-1103 | 4 |
| Visualization & Rendering | 1200-1206 | 7 |
| Verification & Test Harness | 1300-1304 | 5 |
| Traceability & Certification | 1400-1404 | 5 |
| Data Recording & Reporting | 1500-1511 | 12 |
| Security & Access Control | 1600-1602 | 3 |
| Interoperability & Interfaces | 1700-1706 | 7 |
| Performance | 1800-1803 | 4 |
| GUI & User Interface | 1900-1904 | 5 |
| **Total** | | **149** |

### 4.2 By Priority

| Priority | Count |
|----------|-------|
| Critical | 63 |
| High | 64 |
| Medium | 19 |
| Low | 1 |

Note: Two additional requirements beyond the 119 unique capability requirements were created to capture sub-capabilities that required separate tracking when merged requirements from different domains had distinct verification aspects.

### 4.3 By Domain Coverage

| Domain | Requirements Touching This Domain |
|--------|-----------------------------------|
| DEF (Defense) | 87 |
| COM (Commercial) | 82 |
| GIS (Surveying) | 52 |
| INF (Infrastructure) | 62 |
| SAR (Emergency/SAR) | 72 |

---

## 5. Glossary

| Term | Definition |
|------|-----------|
| AGL | Above Ground Level |
| BVLOS | Beyond Visual Line of Sight |
| CEP | Circular Error Probable |
| CFIT | Controlled Flight Into Terrain |
| CRS | Coordinate Reference System |
| CUI | Controlled Unclassified Information |
| DAA | Detect and Avoid |
| DEM | Digital Elevation Model |
| DTED | Digital Terrain Elevation Data |
| ELZ | Emergency Landing Zone |
| EW | Electronic Warfare |
| FTS | Flight Termination System |
| GCS | Ground Control Station |
| GRC | Ground Risk Class |
| GSD | Ground Sample Distance |
| HAE | Height Above Ellipsoid |
| IAMSAR | International Aeronautical and Maritime SAR |
| ITAR | International Traffic in Arms Regulations |
| LOS | Line of Sight |
| MTOW | Maximum Takeoff Weight |
| NETD | Noise Equivalent Temperature Difference |
| NOE | Nap of the Earth |
| POD | Probability of Detection |
| POI | Point of Interest |
| PRF | Pulse Repetition Frequency |
| RSSI | Received Signal Strength Indicator |
| RTH | Return to Home |
| RTM | Requirements Traceability Matrix |
| SAIL | Specific Assurance and Integrity Level |
| SORA | Specific Operations Risk Assessment |
| TFR | Temporary Flight Restriction |
| UAM | Urban Air Mobility |
| UTM | UAS Traffic Management |
| WEZ | Weapon Engagement Zone |

---

## 6. References

| Standard | Title |
|----------|-------|
| DO-178C | Software Considerations in Airborne Systems and Equipment Certification |
| DO-254 | Design Assurance Guidance for Airborne Electronic Hardware |
| MIL-STD-882E | System Safety |
| MIL-HDBK-516C | Airworthiness Certification Criteria |
| MIL-HDBK-1797 | Flying Qualities of Piloted Aircraft |
| MIL-F-8785C | Flying Qualities of Piloted Airplanes |
| MIL-STD-210C | Climatic Information to Determine Design and Test Requirements |
| MIL-STD-6016 | Tactical Data Link 16 Message Standard |
| STANAG 4586 Ed. 3 | Standard Interfaces of UAV Control System |
| NIST SP 800-171 Rev. 2 | Protecting CUI in Nonfederal Systems |
| NIST SP 800-53 Rev. 5 | Security and Privacy Controls |
| FIPS 140-3 | Security Requirements for Cryptographic Modules |
| 14 CFR Part 91 | General Operating and Flight Rules |
| 14 CFR Part 107 | Small Unmanned Aircraft Systems |
| 14 CFR Part 135 | Commuter and On-Demand Operations |
| ASTM F3411-22a | Remote ID |
| ASTM F3442/F3442M | DAA Well-Clear for Small UAS |
| JARUS SORA Ed. 2.0 | Specific Operations Risk Assessment |
| EASA SC-RPAS | Special Condition for Light UAS |
| IAMSAR Manual Vol. II | Mission Co-ordination |
| SAE ARP 866B | Standard Values of Atmospheric Absorption |
| ITAR 22 CFR 120-130 | International Traffic in Arms Regulations |
| EAR 15 CFR 730-774 | Export Administration Regulations |
| DFARS 252.204-7012 | Safeguarding Covered Defense Information |
| FAA Eng. Brief 105 | Vertiport Design |
| EASA PTS-VPT-DSN | Prototype Technical Specifications for Vertiport Design |
| ASPRS Accuracy Standards | Positional Accuracy Standards for Digital Geospatial Data |
| USGS Lidar Base Spec | USGS Lidar Base Specification |
| NOAA SPA | Solar Position Algorithm |
| EGM96 / EGM2008 | Earth Gravitational Model |

---

## Revision History

| Version | Date | Author | Description |
|---------|------|--------|-------------|
| 1.0 | 2026-03-14 | SynthFlight Team | Initial release |
| 1.1 | 2026-03-15 | SynthFlight Team | Status changed to Baselined per Pre-Implementation Gate audit |

---

**End of Document**
