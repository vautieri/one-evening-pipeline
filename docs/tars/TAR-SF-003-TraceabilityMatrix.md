# Requirements Traceability Matrix

| Field | Value |
|---|---|
| **Document ID** | TM-SF-TAR003-2026-001 |
| **TAR Reference** | TAR-SF-003 |
| **Date** | 2026-03-15 |
| **Status** | Baseline |
| **Test File** | `tests/googletest/test_terrain_dala_gtest.cpp` |
| **Modules Under Test** | `SynthFlight::TerrainGrid`, `SynthFlight::TerrainCollisionDetector` |
| **Coding Standard** | SCS-SF-2026-001 |
| **DAL** | A (100% Statement, Decision, and MC/DC) |

---

## Section 1: Forward Traceability (Requirement to Functions to Tests)

### REQ-SF-0400 -- Multi-Format Terrain Data Ingestion

**Implementing Functions (TerrainGrid):**

- `TerrainGrid::TerrainGrid(size_t, size_t, double, double, double)` -- constructor with validation
- `TerrainGrid::SetElevation(size_t, size_t, double)`
- `TerrainGrid::GetElevationAtCell(size_t, size_t) const`
- `TerrainGrid::FillFlat(double)`
- `TerrainGrid::FillSlope(double, double, double)`
- `TerrainGrid::GetRows() const`
- `TerrainGrid::GetCols() const`
- `TerrainGrid::GetPostingMeters() const`
- `TerrainGrid::GetOriginLat() const`
- `TerrainGrid::GetOriginLon() const`
- `TerrainGrid::GetMinElevation() const`
- `TerrainGrid::GetMaxElevation() const`
- `TerrainGrid::GetMeanElevation() const`

**Verifying Test Cases (31 tests):**

| # | Test Suite | Test Name | Result |
|---|---|---|---|
| 1 | TerrainGridDalA_MCDC | Constructor_RowsZero_D1True_Throws | PASS |
| 2 | TerrainGridDalA_MCDC | Constructor_RowsOne_D1True_Throws | PASS |
| 3 | TerrainGridDalA_MCDC | Constructor_RowsTwo_D1False_Succeeds | PASS |
| 4 | TerrainGridDalA_MCDC | Constructor_ColsZero_D2True_Throws | PASS |
| 5 | TerrainGridDalA_MCDC | Constructor_ColsOne_D2True_Throws | PASS |
| 6 | TerrainGridDalA_MCDC | Constructor_ColsTwo_D2False_Succeeds | PASS |
| 7 | TerrainGridDalA_MCDC | Constructor_PostingZero_D3True_Throws | PASS |
| 8 | TerrainGridDalA_MCDC | Constructor_PostingNegative_D3True_Throws | PASS |
| 9 | TerrainGridDalA_MCDC | Constructor_PostingSmallPositive_D3False_Succeeds | PASS |
| 10 | TerrainGridDalA_MCDC | SetElevation_RowAtMax_D4True_Throws | PASS |
| 11 | TerrainGridDalA_MCDC | SetElevation_RowAtMaxMinusOne_D4False_Succeeds | PASS |
| 12 | TerrainGridDalA_MCDC | GetElevationAtCell_ColAtMax_D5True_Throws | PASS |
| 13 | TerrainGridDalA_MCDC | GetElevationAtCell_ColAtMaxMinusOne_D5False_Succeeds | PASS |
| 14 | TerrainGridDalA_MCDC | SetElevation_BothOutOfRange_D4D5True_Throws | PASS |
| 15 | TerrainGridDalA_Boundary | MinimumPosting_HalfMeter | PASS |
| 16 | TerrainGridDalA_Boundary | MaximumPosting_ThirtyMeters | PASS |
| 17 | TerrainGridDalA_Boundary | MinimumValidGrid_2x2 | PASS |
| 18 | TerrainGridDalA_Boundary | MetadataAccessors_ReturnConstructorParams | PASS |
| 19 | TerrainGridDalA | FillSlopeNS_LinearGradient | PASS |
| 20 | TerrainGridDalA | FillSlopeEW_LinearGradient | PASS |
| 21 | TerrainGridDalA | FillSlope_Combined_BilinearGradient | PASS |
| 22 | TerrainGridDalA | Statistics_Min_KnownData | PASS |
| 23 | TerrainGridDalA | Statistics_Max_KnownData | PASS |
| 24 | TerrainGridDalA | Statistics_Mean_KnownData | PASS |
| 25 | TerrainGridDalA | Statistics_Mean_SlopedGrid | PASS |
| 26 | TerrainGridDalA_Robustness | VeryLargePosting | PASS |
| 27 | TerrainGridDalA_Robustness | GridAtEquator_MetersPerDegLonAtMax | PASS |
| 28 | TerrainGridDalA_Robustness | GridAtHighLatitude_MetersPerDegLonSmall | PASS |
| 29 | TerrainGridDalA_Robustness | NegativeElevationGrid_DeadSea | PASS |
| 30 | TerrainGridDalA_Error | Constructor_RowsZero_ThrowsInvalidArgument | PASS |
| 31 | TerrainGridDalA_Error | Constructor_RowsOne_ThrowsInvalidArgument | PASS |
| 32 | TerrainGridDalA_Error | Constructor_ColsZero_ThrowsInvalidArgument | PASS |
| 33 | TerrainGridDalA_Error | Constructor_ColsOne_ThrowsInvalidArgument | PASS |
| 34 | TerrainGridDalA_Error | Constructor_PostingZero_ThrowsInvalidArgument | PASS |
| 35 | TerrainGridDalA_Error | Constructor_PostingNegative_ThrowsInvalidArgument | PASS |
| 36 | TerrainGridDalA_Error | SetElevation_OutOfRange_ThrowsOutOfRange | PASS |
| 37 | TerrainGridDalA_Error | GetElevationAtCell_OutOfRange_ThrowsOutOfRange | PASS |

### REQ-SF-0402 -- Terrain Collision Detection (CFIT Prevention)

**Implementing Functions (TerrainCollisionDetector):**

- `TerrainCollisionDetector::TerrainCollisionDetector(const TerrainGrid&)` -- constructor
- `TerrainCollisionDetector::SetConfig(const CFITConfig&)`
- `TerrainCollisionDetector::GetConfig() const`
- `TerrainCollisionDetector::SetClearanceThreshold(double)`
- `TerrainCollisionDetector::GetClearanceThreshold() const`
- `TerrainCollisionDetector::SetLookaheadTime(double)`
- `TerrainCollisionDetector::GetLookaheadTime() const`
- `TerrainCollisionDetector::Evaluate(const AircraftState&) const`
- `TerrainCollisionDetector::PredictPosition(const AircraftState&, double, double&, double&, double&)` (static)
- `TerrainCollisionDetector::KnotsToFPS(double)` (static)
- `TerrainCollisionDetector::FeetToMeters(double)` (static)
- `TerrainCollisionDetector::MetersToFeet(double)` (static)

**Verifying Test Cases (55 tests):**

| # | Test Suite | Test Name | Result |
|---|---|---|---|
| 1 | CFITDalA_MCDC | SetClearanceThreshold_BelowMin_ClampedTo25 | PASS |
| 2 | CFITDalA_MCDC | SetClearanceThreshold_AboveMax_ClampedTo500 | PASS |
| 3 | CFITDalA_MCDC | SetClearanceThreshold_InRange_Accepted | PASS |
| 4 | CFITDalA_MCDC | SetClearanceThreshold_ExactMin_Accepted | PASS |
| 5 | CFITDalA_MCDC | SetClearanceThreshold_ExactMax_Accepted | PASS |
| 6 | CFITDalA_MCDC | SetLookaheadTime_Below60_ClampedTo60 | PASS |
| 7 | CFITDalA_MCDC | SetLookaheadTime_Exactly60_Accepted | PASS |
| 8 | CFITDalA_MCDC | SetLookaheadTime_Above60_Accepted | PASS |
| 9 | CFITDalA_MCDC | Evaluate_LoopRunsCorrectSteps_D11 | PASS |
| 10 | CFITDalA_MCDC | Evaluate_MaxStepsLimitsIteration_D11 | PASS |
| 11 | CFITDalA_MCDC | Evaluate_AircraftBelowTerrain_Warning_D12 | PASS |
| 12 | CFITDalA_MCDC | Evaluate_AircraftAtTerrainLevel_Warning_D12 | PASS |
| 13 | CFITDalA_MCDC | Evaluate_ClearanceBelowCaution_Caution_D13True | PASS |
| 14 | CFITDalA_MCDC | Evaluate_ClearanceAboveCaution_None_D13False | PASS |
| 15 | CFITDalA_MCDC | Evaluate_DescendingFlight_MinClearanceAtEnd_D14 | PASS |
| 16 | CFITDalA_MCDC | Evaluate_LevelFlight_MinClearanceConstant_D14 | PASS |
| 17 | CFITDalA_MCDC | Evaluate_ImpactDetected_WarningTakesPriority_D15True | PASS |
| 18 | CFITDalA_MCDC | Evaluate_NoImpact_CautionCheckRuns_D15False | PASS |
| 19 | CFITDalA_MCDC | PredictPosition_NorthPole_LonDisplacementZero | PASS |
| 20 | CFITDalA_MCDC | PredictPosition_SouthPole_LonDisplacementZero | PASS |
| 21 | CFITDalA_MCDC | SetConfig_ClearanceAtMin_Accepted | PASS |
| 22 | CFITDalA_MCDC | Evaluate_MultipleImpactPoints_OnlyFirstRecorded | PASS |
| 23 | CFITDalA_Boundary | LevelFlight_HighAboveTerrain_eNone | PASS |
| 24 | CFITDalA_Boundary | LevelFlight_AtCautionThreshold_eCaution | PASS |
| 25 | CFITDalA_Boundary | DescendingFlight_IntoTerrain_eWarning | PASS |
| 26 | CFITDalA_Boundary | ClimbingFlight_AwayFromTerrain_eNone | PASS |
| 27 | CFITDalA_Boundary | StationaryAircraft_SpeedZero_EvaluatesPositionOnly | PASS |
| 28 | CFITDalA_Boundary | TrackNorth_LatitudeIncreases | PASS |
| 29 | CFITDalA_Boundary | TrackEast_LongitudeIncreases | PASS |
| 30 | CFITDalA_Boundary | TrackSouth_LatitudeDecreases | PASS |
| 31 | CFITDalA_Boundary | TrackWest_LongitudeDecreases | PASS |
| 32 | CFITDalA_Boundary | SetConfig_AppliesClamping | PASS |
| 33 | CFITDalA_Boundary | GetConfig_ReturnsCurrentConfig | PASS |
| 34 | CFITDalA_Boundary | DefaultConfig_MatchesExpected | PASS |
| 35 | CFITDalA_Boundary | CFITResult_NoAlert_TimeToImpactNeg1 | PASS |
| 36 | CFITDalA_Boundary | CFITResult_Warning_ValidClosestPosition | PASS |
| 37 | CFITDalA_Robustness | ZeroGroundSpeed_NoNaN_NoCrash | PASS |
| 38 | CFITDalA_Robustness | SteepClimb_90Degrees_ValidResult | PASS |
| 39 | CFITDalA_Robustness | SteepDescent_Neg90Degrees_Warning | PASS |
| 40 | CFITDalA_Robustness | VeryHighSpeed_TrajectoryExtendsFar | PASS |
| 41 | CFITDalA_Robustness | AircraftExactlyAtTerrainElev_Warning | PASS |
| 42 | CFITDalA_Robustness | PredictPosition_TimeZero_ReturnsOriginalPosition | PASS |
| 43 | CFITDalA_Robustness | PredictPosition_HeadingNorth_LatIncreases | PASS |
| 44 | CFITDalA_Robustness | PredictPosition_HeadingEast_LonIncreases | PASS |
| 45 | CFITDalA_Robustness | PredictPosition_Climbing_AltIncreases | PASS |
| 46 | CFITDalA_Robustness | PredictPosition_Descending_AltDecreases | PASS |
| 47 | CFITDalA_Robustness | Evaluate_NorthPole_NoLonDrift | PASS |
| 48 | CFITDalA_Robustness | Evaluate_NegativeLookahead_ClampedTo60 | PASS |
| 49 | CFITDalA_Robustness | Evaluate_NegativeClearance_ClampedTo25 | PASS |
| 50 | CFITDalA_State | Integration_SlopedTerrain_LevelFlight_CFITWarning | PASS |
| 51 | CFITDalA_State | Integration_ValleyTerrain_FlyThrough_DetectsValleyFloor | PASS |
| 52 | CFITDalA_State | Integration_PeakTerrain_FlyOver_ClearanceTracking | PASS |
| 53 | CFITDalA_State | Integration_FullLifecycle_ConstructConfigureEvaluateReconfigure | PASS |
| 54 | CFITDalA_State | Integration_EvaluateWithDescendingTrajectory_TimeToImpact | PASS |
| 55 | CFITDalA_State | Integration_CFITResultFields_Warning_AllValid | PASS |

### REQ-SF-0406 -- Elevation Query API

**Implementing Functions (TerrainGrid):**

- `TerrainGrid::GetElevation(double, double) const` -- bilinear interpolation query
- `TerrainGrid::GetElevationBatch(const double*, const double*, double*, size_t) const`
- `TerrainGrid::IsInsideGrid(double, double) const`
- `TerrainGrid::LatLonToGrid(double, double, double&, double&) const`
- `TerrainGrid::GridToLatLon(double, double, double&, double&) const`

**Verifying Test Cases (38 tests):**

| # | Test Suite | Test Name | Result |
|---|---|---|---|
| 1 | TerrainGridDalA_MCDC | IsInsideGrid_AllTrue_D6True | PASS |
| 2 | TerrainGridDalA_MCDC | IsInsideGrid_RowNegative_D6False | PASS |
| 3 | TerrainGridDalA_MCDC | IsInsideGrid_RowBeyondMax_D6False | PASS |
| 4 | TerrainGridDalA_MCDC | IsInsideGrid_ColNegative_D6False | PASS |
| 5 | TerrainGridDalA_MCDC | IsInsideGrid_ColBeyondMax_D6False | PASS |
| 6 | TerrainGridDalA_MCDC | GetElevation_AtUpperRowBoundary_D7Clamped | PASS |
| 7 | TerrainGridDalA_MCDC | GetElevation_WellInsideGrid_D7NotClamped | PASS |
| 8 | TerrainGridDalA_MCDC | IsInsideGrid_NearMaxRow_D6BoundaryTrue | PASS |
| 9 | TerrainGridDalA_MCDC | IsInsideGrid_NearMaxCol_D6BoundaryTrue | PASS |
| 10 | TerrainGridDalA_MCDC | BatchQuery_EmptyInput_LoopFalseBranch | PASS |
| 11 | TerrainGridDalA_Boundary | GridCell_Origin_ExactQuery | PASS |
| 12 | TerrainGridDalA_Boundary | GridCell_LastRowLastCol_ExactQuery | PASS |
| 13 | TerrainGridDalA_Boundary | GridCell_CornerSW | PASS |
| 14 | TerrainGridDalA_Boundary | GridCell_CornerSE | PASS |
| 15 | TerrainGridDalA_Boundary | GridCell_CornerNW | PASS |
| 16 | TerrainGridDalA_Boundary | GridCell_CornerNE | PASS |
| 17 | TerrainGridDalA_Boundary | BilinearInterp_MidCell_KnownValues | PASS |
| 18 | TerrainGridDalA_Boundary | EdgeInterp_BetweenEdgeCells | PASS |
| 19 | TerrainGridDalA_Boundary | CoordinateRoundtrip_Origin | PASS |
| 20 | TerrainGridDalA_Boundary | CoordinateRoundtrip_GridCorner | PASS |
| 21 | TerrainGridDalA_Boundary | IsInsideGrid_ExactOrigin_True | PASS |
| 22 | TerrainGridDalA_Boundary | IsInsideGrid_NearFarCorner_True | PASS |
| 23 | TerrainGridDalA_Boundary | IsInsideGrid_JustOutsideNorth_False | PASS |
| 24 | TerrainGridDalA_Boundary | IsInsideGrid_JustOutsideSouth_False | PASS |
| 25 | TerrainGridDalA_Boundary | BilinearInterp_QuarterCell_CorrectWeight | PASS |
| 26 | TerrainGridDalA | FillFlat_QueryAnywhere_ReturnsConstant | PASS |
| 27 | TerrainGridDalA | FillFlat_QueryAtOrigin_ReturnsConstant | PASS |
| 28 | TerrainGridDalA | SetGetElevation_SpecificCells_InterpolatesCorrectly | PASS |
| 29 | TerrainGridDalA | CheckerboardPattern_MidCellInterpolation | PASS |
| 30 | TerrainGridDalA | BatchQuery_MatchesSingleQueries | PASS |
| 31 | TerrainGridDalA_Robustness | QueryFarOutside_ClampsToEdge_NoCrash | PASS |
| 32 | TerrainGridDalA_Robustness | QueryFarSouth_ClampsToEdge | PASS |
| 33 | TerrainGridDalA_Robustness | QueryFarWest_ClampsToEdge | PASS |
| 34 | TerrainGridDalA_Robustness | QueryNegativeLatLon_Clamps | PASS |
| 35 | TerrainGridDalA_Robustness | ZeroElevationGrid_AllQueriesReturnZero | PASS |
| 36 | TerrainGridDalA_Robustness | BatchQuery_EmptyCount_NoCrash | PASS |
| 37 | CFITDalA_State | Integration_BatchElevation_MatchesSingle | PASS |
| 38 | CFITDalA_State | Integration_CFITResultFields_None_AllValid | PASS |

### REQ-SF-1400 -- DO-178C DAL A Structural Coverage Evidence

**Implementing Functions:**

- `TerrainCollisionDetector::FeetToMeters(double)` (static constexpr)
- `TerrainCollisionDetector::MetersToFeet(double)` (static constexpr)
- `TerrainCollisionDetector::KnotsToFPS(double)` (static constexpr)

Note: REQ-SF-1400 is a process-level requirement. The tests below provide structural coverage
evidence (unit conversion verification) that supports the DAL A coverage claim. The
`Evaluate_NorthPole_NoLonDrift` test also traces to REQ-SF-1400 as polar-latitude
robustness evidence.

**Verifying Test Cases (6 tests):**

| # | Test Suite | Test Name | Result |
|---|---|---|---|
| 1 | CFITDalA_State | FeetToMeters_Roundtrip | PASS |
| 2 | CFITDalA_State | MetersToFeet_Roundtrip | PASS |
| 3 | CFITDalA_State | KnotsToFPS_KnownValue | PASS |
| 4 | CFITDalA_State | FeetToMeters_OneFootEquals0_3048m | PASS |
| 5 | CFITDalA_State | MetersToFeet_OneMeterEquals3_28084ft | PASS |
| 6 | CFITDalA_Robustness | Evaluate_NorthPole_NoLonDrift | PASS |

---

## Section 2: Reverse Traceability (Test to Function to Requirement)

Every TEST_F in `test_terrain_dala_gtest.cpp` is listed below. Total: **135 tests**.

| # | Test Suite | Test Name | Functions Tested | Requirement | Result |
|---|---|---|---|---|---|
| 1 | TerrainGridDalA_MCDC | Constructor_RowsZero_D1True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 2 | TerrainGridDalA_MCDC | Constructor_RowsOne_D1True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 3 | TerrainGridDalA_MCDC | Constructor_RowsTwo_D1False_Succeeds | TerrainGrid(), GetRows() | REQ-SF-0400 | PASS |
| 4 | TerrainGridDalA_MCDC | Constructor_ColsZero_D2True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 5 | TerrainGridDalA_MCDC | Constructor_ColsOne_D2True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 6 | TerrainGridDalA_MCDC | Constructor_ColsTwo_D2False_Succeeds | TerrainGrid(), GetCols() | REQ-SF-0400 | PASS |
| 7 | TerrainGridDalA_MCDC | Constructor_PostingZero_D3True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 8 | TerrainGridDalA_MCDC | Constructor_PostingNegative_D3True_Throws | TerrainGrid() | REQ-SF-0400 | PASS |
| 9 | TerrainGridDalA_MCDC | Constructor_PostingSmallPositive_D3False_Succeeds | TerrainGrid(), GetPostingMeters() | REQ-SF-0400 | PASS |
| 10 | TerrainGridDalA_MCDC | SetElevation_RowAtMax_D4True_Throws | SetElevation(), ValidateIndices() | REQ-SF-0400 | PASS |
| 11 | TerrainGridDalA_MCDC | SetElevation_RowAtMaxMinusOne_D4False_Succeeds | SetElevation(), ValidateIndices() | REQ-SF-0400 | PASS |
| 12 | TerrainGridDalA_MCDC | GetElevationAtCell_ColAtMax_D5True_Throws | GetElevationAtCell(), ValidateIndices() | REQ-SF-0400 | PASS |
| 13 | TerrainGridDalA_MCDC | GetElevationAtCell_ColAtMaxMinusOne_D5False_Succeeds | GetElevationAtCell(), SetElevation(), ValidateIndices() | REQ-SF-0400 | PASS |
| 14 | TerrainGridDalA_MCDC | SetElevation_BothOutOfRange_D4D5True_Throws | SetElevation(), ValidateIndices() | REQ-SF-0400 | PASS |
| 15 | TerrainGridDalA_MCDC | IsInsideGrid_AllTrue_D6True | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 16 | TerrainGridDalA_MCDC | IsInsideGrid_RowNegative_D6False | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 17 | TerrainGridDalA_MCDC | IsInsideGrid_RowBeyondMax_D6False | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 18 | TerrainGridDalA_MCDC | IsInsideGrid_ColNegative_D6False | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 19 | TerrainGridDalA_MCDC | IsInsideGrid_ColBeyondMax_D6False | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 20 | TerrainGridDalA_MCDC | GetElevation_AtUpperRowBoundary_D7Clamped | GetElevation(), FillFlat(), SetElevation() | REQ-SF-0406 | PASS |
| 21 | TerrainGridDalA_MCDC | GetElevation_WellInsideGrid_D7NotClamped | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 22 | TerrainGridDalA_MCDC | IsInsideGrid_NearMaxRow_D6BoundaryTrue | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 23 | TerrainGridDalA_MCDC | IsInsideGrid_NearMaxCol_D6BoundaryTrue | IsInsideGrid(), LatLonToGrid() | REQ-SF-0406 | PASS |
| 24 | TerrainGridDalA_MCDC | BatchQuery_EmptyInput_LoopFalseBranch | GetElevationBatch(), FillFlat() | REQ-SF-0406 | PASS |
| 25 | TerrainGridDalA_Boundary | GridCell_Origin_ExactQuery | GetElevation(), SetElevation() | REQ-SF-0406 | PASS |
| 26 | TerrainGridDalA_Boundary | GridCell_LastRowLastCol_ExactQuery | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 27 | TerrainGridDalA_Boundary | GridCell_CornerSW | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 28 | TerrainGridDalA_Boundary | GridCell_CornerSE | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 29 | TerrainGridDalA_Boundary | GridCell_CornerNW | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 30 | TerrainGridDalA_Boundary | GridCell_CornerNE | GetElevation(), FillFlat() | REQ-SF-0406 | PASS |
| 31 | TerrainGridDalA_Boundary | BilinearInterp_MidCell_KnownValues | GetElevation(), SetElevation() | REQ-SF-0406 | PASS |
| 32 | TerrainGridDalA_Boundary | EdgeInterp_BetweenEdgeCells | GetElevation(), SetElevation() | REQ-SF-0406 | PASS |
| 33 | TerrainGridDalA_Boundary | MinimumPosting_HalfMeter | TerrainGrid(), FillFlat(), GetPostingMeters(), GetElevation() | REQ-SF-0400 | PASS |
| 34 | TerrainGridDalA_Boundary | MaximumPosting_ThirtyMeters | TerrainGrid(), FillFlat(), GetPostingMeters(), GetElevation() | REQ-SF-0400 | PASS |
| 35 | TerrainGridDalA_Boundary | MinimumValidGrid_2x2 | TerrainGrid(), SetElevation(), GetRows(), GetCols(), GetElevationAtCell() | REQ-SF-0400 | PASS |
| 36 | TerrainGridDalA_Boundary | CoordinateRoundtrip_Origin | LatLonToGrid(), GridToLatLon() | REQ-SF-0406 | PASS |
| 37 | TerrainGridDalA_Boundary | CoordinateRoundtrip_GridCorner | LatLonToGrid(), GridToLatLon() | REQ-SF-0406 | PASS |
| 38 | TerrainGridDalA_Boundary | MetadataAccessors_ReturnConstructorParams | GetRows(), GetCols(), GetPostingMeters(), GetOriginLat(), GetOriginLon() | REQ-SF-0400 | PASS |
| 39 | TerrainGridDalA_Boundary | IsInsideGrid_ExactOrigin_True | IsInsideGrid() | REQ-SF-0406 | PASS |
| 40 | TerrainGridDalA_Boundary | IsInsideGrid_NearFarCorner_True | IsInsideGrid() | REQ-SF-0406 | PASS |
| 41 | TerrainGridDalA_Boundary | IsInsideGrid_JustOutsideNorth_False | IsInsideGrid() | REQ-SF-0406 | PASS |
| 42 | TerrainGridDalA_Boundary | IsInsideGrid_JustOutsideSouth_False | IsInsideGrid() | REQ-SF-0406 | PASS |
| 43 | TerrainGridDalA_Boundary | BilinearInterp_QuarterCell_CorrectWeight | GetElevation(), SetElevation() | REQ-SF-0406 | PASS |
| 44 | TerrainGridDalA | FillFlat_QueryAnywhere_ReturnsConstant | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 45 | TerrainGridDalA | FillFlat_QueryAtOrigin_ReturnsConstant | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 46 | TerrainGridDalA | FillSlopeNS_LinearGradient | FillSlope(), GetElevationAtCell() | REQ-SF-0400 | PASS |
| 47 | TerrainGridDalA | FillSlopeEW_LinearGradient | FillSlope(), GetElevationAtCell() | REQ-SF-0400 | PASS |
| 48 | TerrainGridDalA | FillSlope_Combined_BilinearGradient | FillSlope(), GetElevationAtCell() | REQ-SF-0400 | PASS |
| 49 | TerrainGridDalA | SetGetElevation_SpecificCells_InterpolatesCorrectly | SetElevation(), FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 50 | TerrainGridDalA | CheckerboardPattern_MidCellInterpolation | SetElevation(), GetElevation() | REQ-SF-0406 | PASS |
| 51 | TerrainGridDalA | BatchQuery_MatchesSingleQueries | FillSlope(), GetElevationBatch(), GetElevation() | REQ-SF-0406 | PASS |
| 52 | TerrainGridDalA | Statistics_Min_KnownData | FillFlat(), SetElevation(), GetMinElevation() | REQ-SF-0400 | PASS |
| 53 | TerrainGridDalA | Statistics_Max_KnownData | FillFlat(), SetElevation(), GetMaxElevation() | REQ-SF-0400 | PASS |
| 54 | TerrainGridDalA | Statistics_Mean_KnownData | FillFlat(), GetMeanElevation() | REQ-SF-0400 | PASS |
| 55 | TerrainGridDalA | Statistics_Mean_SlopedGrid | FillSlope(), SetElevation(), GetMeanElevation() | REQ-SF-0400 | PASS |
| 56 | TerrainGridDalA_Robustness | QueryFarOutside_ClampsToEdge_NoCrash | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 57 | TerrainGridDalA_Robustness | QueryFarSouth_ClampsToEdge | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 58 | TerrainGridDalA_Robustness | QueryFarWest_ClampsToEdge | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 59 | TerrainGridDalA_Robustness | QueryNegativeLatLon_Clamps | FillFlat(), GetElevation() | REQ-SF-0406 | PASS |
| 60 | TerrainGridDalA_Robustness | VeryLargePosting | TerrainGrid(), FillFlat(), GetElevation() | REQ-SF-0400 | PASS |
| 61 | TerrainGridDalA_Robustness | GridAtEquator_MetersPerDegLonAtMax | TerrainGrid(), FillFlat(), GetElevation() | REQ-SF-0400 | PASS |
| 62 | TerrainGridDalA_Robustness | GridAtHighLatitude_MetersPerDegLonSmall | TerrainGrid(), FillFlat(), GetElevation() | REQ-SF-0400 | PASS |
| 63 | TerrainGridDalA_Robustness | ZeroElevationGrid_AllQueriesReturnZero | FillFlat(), GetElevation(), GetMinElevation(), GetMaxElevation() | REQ-SF-0406 | PASS |
| 64 | TerrainGridDalA_Robustness | NegativeElevationGrid_DeadSea | FillFlat(), GetElevation(), GetMinElevation() | REQ-SF-0400 | PASS |
| 65 | TerrainGridDalA_Robustness | BatchQuery_EmptyCount_NoCrash | FillFlat(), GetElevationBatch() | REQ-SF-0406 | PASS |
| 66 | TerrainGridDalA_Error | Constructor_RowsZero_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 67 | TerrainGridDalA_Error | Constructor_RowsOne_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 68 | TerrainGridDalA_Error | Constructor_ColsZero_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 69 | TerrainGridDalA_Error | Constructor_ColsOne_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 70 | TerrainGridDalA_Error | Constructor_PostingZero_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 71 | TerrainGridDalA_Error | Constructor_PostingNegative_ThrowsInvalidArgument | TerrainGrid() | REQ-SF-0400 | PASS |
| 72 | TerrainGridDalA_Error | SetElevation_OutOfRange_ThrowsOutOfRange | SetElevation(), ValidateIndices() | REQ-SF-0400 | PASS |
| 73 | TerrainGridDalA_Error | GetElevationAtCell_OutOfRange_ThrowsOutOfRange | GetElevationAtCell(), ValidateIndices() | REQ-SF-0400 | PASS |
| 74 | CFITDalA_MCDC | SetClearanceThreshold_BelowMin_ClampedTo25 | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 75 | CFITDalA_MCDC | SetClearanceThreshold_AboveMax_ClampedTo500 | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 76 | CFITDalA_MCDC | SetClearanceThreshold_InRange_Accepted | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 77 | CFITDalA_MCDC | SetClearanceThreshold_ExactMin_Accepted | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 78 | CFITDalA_MCDC | SetClearanceThreshold_ExactMax_Accepted | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 79 | CFITDalA_MCDC | SetLookaheadTime_Below60_ClampedTo60 | SetLookaheadTime(), GetLookaheadTime() | REQ-SF-0402 | PASS |
| 80 | CFITDalA_MCDC | SetLookaheadTime_Exactly60_Accepted | SetLookaheadTime(), GetLookaheadTime() | REQ-SF-0402 | PASS |
| 81 | CFITDalA_MCDC | SetLookaheadTime_Above60_Accepted | SetLookaheadTime(), GetLookaheadTime() | REQ-SF-0402 | PASS |
| 82 | CFITDalA_MCDC | Evaluate_LoopRunsCorrectSteps_D11 | Evaluate(), SetConfig() | REQ-SF-0402 | PASS |
| 83 | CFITDalA_MCDC | Evaluate_MaxStepsLimitsIteration_D11 | Evaluate(), SetConfig() | REQ-SF-0402 | PASS |
| 84 | CFITDalA_MCDC | Evaluate_AircraftBelowTerrain_Warning_D12 | Evaluate() | REQ-SF-0402 | PASS |
| 85 | CFITDalA_MCDC | Evaluate_AircraftAtTerrainLevel_Warning_D12 | Evaluate() | REQ-SF-0402 | PASS |
| 86 | CFITDalA_MCDC | Evaluate_ClearanceBelowCaution_Caution_D13True | Evaluate() | REQ-SF-0402 | PASS |
| 87 | CFITDalA_MCDC | Evaluate_ClearanceAboveCaution_None_D13False | Evaluate() | REQ-SF-0402 | PASS |
| 88 | CFITDalA_MCDC | Evaluate_DescendingFlight_MinClearanceAtEnd_D14 | Evaluate() | REQ-SF-0402 | PASS |
| 89 | CFITDalA_MCDC | Evaluate_LevelFlight_MinClearanceConstant_D14 | Evaluate(), SetConfig() | REQ-SF-0402 | PASS |
| 90 | CFITDalA_MCDC | Evaluate_ImpactDetected_WarningTakesPriority_D15True | Evaluate() | REQ-SF-0402 | PASS |
| 91 | CFITDalA_MCDC | Evaluate_NoImpact_CautionCheckRuns_D15False | Evaluate() | REQ-SF-0402 | PASS |
| 92 | CFITDalA_MCDC | PredictPosition_NorthPole_LonDisplacementZero | PredictPosition() | REQ-SF-0402 | PASS |
| 93 | CFITDalA_MCDC | PredictPosition_SouthPole_LonDisplacementZero | PredictPosition() | REQ-SF-0402 | PASS |
| 94 | CFITDalA_MCDC | SetConfig_ClearanceAtMin_Accepted | SetConfig(), GetConfig() | REQ-SF-0402 | PASS |
| 95 | CFITDalA_MCDC | Evaluate_MultipleImpactPoints_OnlyFirstRecorded | Evaluate(), SetConfig() | REQ-SF-0402 | PASS |
| 96 | CFITDalA_Boundary | LevelFlight_HighAboveTerrain_eNone | Evaluate() | REQ-SF-0402 | PASS |
| 97 | CFITDalA_Boundary | LevelFlight_AtCautionThreshold_eCaution | Evaluate() | REQ-SF-0402 | PASS |
| 98 | CFITDalA_Boundary | DescendingFlight_IntoTerrain_eWarning | Evaluate() | REQ-SF-0402 | PASS |
| 99 | CFITDalA_Boundary | ClimbingFlight_AwayFromTerrain_eNone | Evaluate() | REQ-SF-0402 | PASS |
| 100 | CFITDalA_Boundary | StationaryAircraft_SpeedZero_EvaluatesPositionOnly | Evaluate() | REQ-SF-0402 | PASS |
| 101 | CFITDalA_Boundary | TrackNorth_LatitudeIncreases | PredictPosition() | REQ-SF-0402 | PASS |
| 102 | CFITDalA_Boundary | TrackEast_LongitudeIncreases | PredictPosition() | REQ-SF-0402 | PASS |
| 103 | CFITDalA_Boundary | TrackSouth_LatitudeDecreases | PredictPosition() | REQ-SF-0402 | PASS |
| 104 | CFITDalA_Boundary | TrackWest_LongitudeDecreases | PredictPosition() | REQ-SF-0402 | PASS |
| 105 | CFITDalA_Boundary | SetConfig_AppliesClamping | SetConfig(), GetConfig() | REQ-SF-0402 | PASS |
| 106 | CFITDalA_Boundary | GetConfig_ReturnsCurrentConfig | SetConfig(), GetConfig() | REQ-SF-0402 | PASS |
| 107 | CFITDalA_Boundary | DefaultConfig_MatchesExpected | GetConfig() | REQ-SF-0402 | PASS |
| 108 | CFITDalA_Boundary | CFITResult_NoAlert_TimeToImpactNeg1 | Evaluate() | REQ-SF-0402 | PASS |
| 109 | CFITDalA_Boundary | CFITResult_Warning_ValidClosestPosition | Evaluate() | REQ-SF-0402 | PASS |
| 110 | CFITDalA_Robustness | ZeroGroundSpeed_NoNaN_NoCrash | Evaluate() | REQ-SF-0402 | PASS |
| 111 | CFITDalA_Robustness | SteepClimb_90Degrees_ValidResult | Evaluate() | REQ-SF-0402 | PASS |
| 112 | CFITDalA_Robustness | SteepDescent_Neg90Degrees_Warning | Evaluate() | REQ-SF-0402 | PASS |
| 113 | CFITDalA_Robustness | VeryHighSpeed_TrajectoryExtendsFar | Evaluate() | REQ-SF-0402 | PASS |
| 114 | CFITDalA_Robustness | AircraftExactlyAtTerrainElev_Warning | Evaluate() | REQ-SF-0402 | PASS |
| 115 | CFITDalA_Robustness | PredictPosition_TimeZero_ReturnsOriginalPosition | PredictPosition() | REQ-SF-0402 | PASS |
| 116 | CFITDalA_Robustness | PredictPosition_HeadingNorth_LatIncreases | PredictPosition() | REQ-SF-0402 | PASS |
| 117 | CFITDalA_Robustness | PredictPosition_HeadingEast_LonIncreases | PredictPosition() | REQ-SF-0402 | PASS |
| 118 | CFITDalA_Robustness | PredictPosition_Climbing_AltIncreases | PredictPosition() | REQ-SF-0402 | PASS |
| 119 | CFITDalA_Robustness | PredictPosition_Descending_AltDecreases | PredictPosition() | REQ-SF-0402 | PASS |
| 120 | CFITDalA_Robustness | Evaluate_NorthPole_NoLonDrift | Evaluate(), TerrainGrid(), FillFlat() | REQ-SF-0402, REQ-SF-1400 | PASS |
| 121 | CFITDalA_Robustness | Evaluate_NegativeLookahead_ClampedTo60 | SetLookaheadTime(), GetLookaheadTime() | REQ-SF-0402 | PASS |
| 122 | CFITDalA_Robustness | Evaluate_NegativeClearance_ClampedTo25 | SetClearanceThreshold(), GetClearanceThreshold() | REQ-SF-0402 | PASS |
| 123 | CFITDalA_State | FeetToMeters_Roundtrip | FeetToMeters(), MetersToFeet() | REQ-SF-1400 | PASS |
| 124 | CFITDalA_State | MetersToFeet_Roundtrip | MetersToFeet(), FeetToMeters() | REQ-SF-1400 | PASS |
| 125 | CFITDalA_State | KnotsToFPS_KnownValue | KnotsToFPS() | REQ-SF-1400 | PASS |
| 126 | CFITDalA_State | FeetToMeters_OneFootEquals0_3048m | FeetToMeters() | REQ-SF-1400 | PASS |
| 127 | CFITDalA_State | MetersToFeet_OneMeterEquals3_28084ft | MetersToFeet() | REQ-SF-1400 | PASS |
| 128 | CFITDalA_State | Integration_SlopedTerrain_LevelFlight_CFITWarning | TerrainGrid(), SetElevation(), Evaluate(), SetConfig() | REQ-SF-0402 | PASS |
| 129 | CFITDalA_State | Integration_ValleyTerrain_FlyThrough_DetectsValleyFloor | SetElevation(), Evaluate(), GetElevation() | REQ-SF-0402 | PASS |
| 130 | CFITDalA_State | Integration_PeakTerrain_FlyOver_ClearanceTracking | FillFlat(), SetElevation(), Evaluate() | REQ-SF-0402 | PASS |
| 131 | CFITDalA_State | Integration_BatchElevation_MatchesSingle | FillSlope(), GetElevationBatch(), GetElevation() | REQ-SF-0406 | PASS |
| 132 | CFITDalA_State | Integration_FullLifecycle_ConstructConfigureEvaluateReconfigure | Evaluate(), SetConfig(), GetConfig() | REQ-SF-0402 | PASS |
| 133 | CFITDalA_State | Integration_EvaluateWithDescendingTrajectory_TimeToImpact | Evaluate(), GetConfig() | REQ-SF-0402 | PASS |
| 134 | CFITDalA_State | Integration_CFITResultFields_Warning_AllValid | Evaluate() | REQ-SF-0402 | PASS |
| 135 | CFITDalA_State | Integration_CFITResultFields_None_AllValid | Evaluate() | REQ-SF-0406 | PASS |

---

## Section 3: Function Coverage Matrix

### Module: TerrainGrid (`src/terrain/TerrainGrid.h`)

| Function | Requirement | Test Count | Covered |
|---|---|---|---|
| `TerrainGrid(size_t, size_t, double, double, double)` | REQ-SF-0400 | 18 | YES |
| `GetRows() const` | REQ-SF-0400 | 3 | YES |
| `GetCols() const` | REQ-SF-0400 | 3 | YES |
| `GetPostingMeters() const` | REQ-SF-0400 | 4 | YES |
| `GetOriginLat() const` | REQ-SF-0400 | 1 | YES |
| `GetOriginLon() const` | REQ-SF-0400 | 1 | YES |
| `SetElevation(size_t, size_t, double)` | REQ-SF-0400 | 20 | YES |
| `GetElevationAtCell(size_t, size_t) const` | REQ-SF-0400 | 10 | YES |
| `FillFlat(double)` | REQ-SF-0400, REQ-SF-0406 | 25 | YES |
| `FillSlope(double, double, double)` | REQ-SF-0400, REQ-SF-0406 | 6 | YES |
| `LatLonToGrid(double, double, double&, double&) const` | REQ-SF-0406 | 9 | YES |
| `GridToLatLon(double, double, double&, double&) const` | REQ-SF-0406 | 2 | YES |
| `GetElevation(double, double) const` | REQ-SF-0406 | 30 | YES |
| `GetElevationBatch(const double*, const double*, double*, size_t) const` | REQ-SF-0406 | 5 | YES |
| `IsInsideGrid(double, double) const` | REQ-SF-0406 | 12 | YES |
| `GetMinElevation() const` | REQ-SF-0400 | 3 | YES |
| `GetMaxElevation() const` | REQ-SF-0400 | 2 | YES |
| `GetMeanElevation() const` | REQ-SF-0400 | 2 | YES |

### Module: TerrainCollisionDetector (`src/terrain/TerrainCollisionDetector.h`)

| Function | Requirement | Test Count | Covered |
|---|---|---|---|
| `TerrainCollisionDetector(const TerrainGrid&)` | REQ-SF-0402 | 55 | YES |
| `SetConfig(const CFITConfig&)` | REQ-SF-0402 | 8 | YES |
| `GetConfig() const` | REQ-SF-0402 | 5 | YES |
| `SetClearanceThreshold(double)` | REQ-SF-0402 | 7 | YES |
| `GetClearanceThreshold() const` | REQ-SF-0402 | 7 | YES |
| `SetLookaheadTime(double)` | REQ-SF-0402 | 4 | YES |
| `GetLookaheadTime() const` | REQ-SF-0402 | 4 | YES |
| `Evaluate(const AircraftState&) const` | REQ-SF-0402 | 32 | YES |
| `PredictPosition(const AircraftState&, double, double&, double&, double&)` | REQ-SF-0402 | 10 | YES |
| `FeetToMeters(double)` | REQ-SF-1400 | 3 | YES |
| `MetersToFeet(double)` | REQ-SF-1400 | 3 | YES |
| `KnotsToFPS(double)` | REQ-SF-1400 | 1 | YES |

---

## Section 4: Summary Statistics

| Metric | Value |
|---|---|
| Total requirements in scope | 4 |
| Requirements verified | REQ-SF-0400, REQ-SF-0402, REQ-SF-0406, REQ-SF-1400 |
| Total test cases | 135 |
| Test cases PASS | 135 |
| Test cases FAIL | 0 |
| Test cases with requirement trace | 135 / 135 (100%) |
| Orphan tests (no requirement) | 0 |
| Public functions in TerrainGrid | 18 |
| Public functions in TerrainCollisionDetector | 12 |
| Total public functions | 30 |
| Functions with test coverage | 30 / 30 (100%) |
| Dead code (untested public functions) | 0 |

### Tests per Requirement

| Requirement | Test Count | Percentage |
|---|---|---|
| REQ-SF-0400 | 37 | 27.4% |
| REQ-SF-0402 | 55 | 40.7% |
| REQ-SF-0406 | 38 | 28.1% |
| REQ-SF-1400 | 6 | 4.4% |
| **Total** (one test counts toward each traced requirement) | **136** | -- |

Note: The total 136 exceeds 135 because test `Evaluate_NorthPole_NoLonDrift` traces to
both REQ-SF-0402 and REQ-SF-1400. Each unique TEST_F appears exactly once in Section 2.

### Test Suite Distribution

| Test Suite | Count |
|---|---|
| TerrainGridDalA_MCDC | 24 |
| TerrainGridDalA_Boundary | 19 |
| TerrainGridDalA | 12 |
| TerrainGridDalA_Robustness | 10 |
| TerrainGridDalA_Error | 8 |
| CFITDalA_MCDC | 22 |
| CFITDalA_Boundary | 14 |
| CFITDalA_Robustness | 13 |
| CFITDalA_State | 13 |
| **Total** | **135** |

---

*End of Traceability Matrix TM-SF-TAR003-2026-001*
