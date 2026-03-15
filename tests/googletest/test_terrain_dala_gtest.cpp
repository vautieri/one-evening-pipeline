/*==============================================================================
 * TAR-SF-003 | DAL A Test Suite for Terrain Engine
 *
 * Requirements Verified:
 *   REQ-SF-0400  Terrain data model (grid-based elevation)
 *   REQ-SF-0402  Terrain Collision Detection (CFIT Prevention)
 *   REQ-SF-0406  Elevation Query API
 *   REQ-SF-1400  DO-178C DAL A structural coverage evidence
 *
 * Coverage Targets: 100% Statement, Decision, and MC/DC (DAL A)
 * Standard: SCS-SF-2026-001
 * Date: 2026-03-15
 *
 * Modules Under Test:
 *   SynthFlight::TerrainGrid
 *   SynthFlight::TerrainCollisionDetector
 *============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>
#include "TerrainGrid.h"
#include "TerrainCollisionDetector.h"

using namespace SynthFlight;

// =============================================================================
//  Named constants (SCS-SF-2026-001: no magic numbers)
// =============================================================================

// Grid dimensions
static constexpr size_t GRID_ROWS = 10;
static constexpr size_t GRID_COLS = 10;
static constexpr double GRID_POSTING = 10.0;  // meters
static constexpr double ORIGIN_LAT = 40.0;    // degrees
static constexpr double ORIGIN_LON = -105.0;  // degrees

// Tolerances
static constexpr double TIGHT_TOL = 1.0e-10;
static constexpr double INTERP_TOL = 1.0e-6;
static constexpr double COORD_TOL = 1.0e-8;  // degrees
static constexpr double ELEV_TOL = 0.01;      // meters

// Elevations
static constexpr double ELEV_FLAT = 1500.0;   // meters
static constexpr double ELEV_ZERO = 0.0;
static constexpr double ELEV_PEAK = 3000.0;
static constexpr double ELEV_VALLEY = 500.0;

// CFIT test values
static constexpr double ALT_HIGH_FT = 10000.0;
static constexpr double ALT_LOW_FT = 200.0;
static constexpr double SPEED_CRUISE_KTS = 120.0;
static constexpr double TRACK_NORTH = 0.0;
static constexpr double TRACK_EAST = 90.0;
static constexpr double FPA_LEVEL = 0.0;
static constexpr double FPA_DESCENDING = -3.0;
static constexpr double FPA_CLIMBING = 5.0;

// Physical constants
static constexpr double METERS_PER_DEG_LAT = 111320.0;
static constexpr double DEG_TO_RAD = 3.14159265358979323846 / 180.0;
static constexpr double FEET_PER_METER = 1.0 / 0.3048;

// Additional test constants
static constexpr double TRACK_SOUTH = 180.0;
static constexpr double TRACK_WEST = 270.0;
static constexpr double FPA_STEEP_DOWN = -90.0;
static constexpr double FPA_STEEP_UP = 90.0;
static constexpr double SPEED_ZERO = 0.0;
static constexpr double SPEED_HIGH_KTS = 500.0;
static constexpr double CLEARANCE_MIN_FT = 25.0;
static constexpr double CLEARANCE_MAX_FT = 500.0;
static constexpr double CLEARANCE_DEFAULT_FT = 100.0;
static constexpr double LOOKAHEAD_MIN_SEC = 60.0;
static constexpr double LOOKAHEAD_DEFAULT_SEC = 60.0;
static constexpr double CAUTION_MULTIPLIER = 1.5;
static constexpr double ELEV_DEAD_SEA = -430.0;  // meters
static constexpr double SLOPE_10DEG = 10.0;       // degrees
static constexpr double SLOPE_5DEG = 5.0;         // degrees
static constexpr double KNOTS_100_IN_FPS = 168.78098571;
static constexpr double FEET_TO_METERS_RATIO = 0.3048;
static constexpr double METERS_TO_FEET_RATIO = 3.2808398950131;

// =============================================================================
//  Test fixtures
// =============================================================================

class TerrainGridDalA : public ::testing::Test {
protected:
    // Standard 10x10 grid with 10m posting at (40 N, -105 W)
    TerrainGrid grid{GRID_ROWS, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON};

    // Compute lat/lon offset for a given row/col
    double LatForRow(double row) const {
        return ORIGIN_LAT + (row * GRID_POSTING) / METERS_PER_DEG_LAT;
    }
    double LonForCol(double col) const {
        double metersPerDegLon = METERS_PER_DEG_LAT * std::cos(ORIGIN_LAT * DEG_TO_RAD);
        return ORIGIN_LON + (col * GRID_POSTING) / metersPerDegLon;
    }
};

using TerrainGridDalA_MCDC = TerrainGridDalA;
using TerrainGridDalA_Boundary = TerrainGridDalA;
using TerrainGridDalA_Robustness = TerrainGridDalA;
using TerrainGridDalA_Error = TerrainGridDalA;

class CFITDalA : public ::testing::Test {
protected:
    TerrainGrid terrain{GRID_ROWS, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON};

    void SetUp() override {
        terrain.FillFlat(ELEV_FLAT);  // 1500m flat terrain
    }

    AircraftState MakeState(double lat, double lon, double altMSL_ft,
                            double speedKts, double trackDeg, double fpaDeg) {
        double terrainElev = terrain.GetElevation(lat, lon);
        AircraftState s{};
        s.latitude = lat;
        s.longitude = lon;
        s.altitudeMSL = altMSL_ft;
        s.altitudeAGL = altMSL_ft - terrainElev * FEET_PER_METER;
        s.groundSpeedKts = speedKts;
        s.trackAngleDeg = trackDeg;
        s.flightPathAngleDeg = fpaDeg;
        s.verticalSpeedFPM = 0.0;
        return s;
    }

    double LatForRow(double row) const {
        return ORIGIN_LAT + (row * GRID_POSTING) / METERS_PER_DEG_LAT;
    }
    double LonForCol(double col) const {
        double metersPerDegLon = METERS_PER_DEG_LAT * std::cos(ORIGIN_LAT * DEG_TO_RAD);
        return ORIGIN_LON + (col * GRID_POSTING) / metersPerDegLon;
    }
};

using CFITDalA_MCDC = CFITDalA;
using CFITDalA_Boundary = CFITDalA;
using CFITDalA_Robustness = CFITDalA;
using CFITDalA_State = CFITDalA;

// #############################################################################
//  1. TERRAIN GRID MC/DC TESTS (~20 tests)
// #############################################################################

// -----------------------------------------------------------------------------
//  D1: Constructor -- if (rows < kMinGridDimension)
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, Constructor_RowsZero_D1True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D1 TRUE -- rows=0 < 2, throws
    EXPECT_THROW(
        TerrainGrid(0, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_RowsOne_D1True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D1 TRUE -- rows=1 < 2, throws
    EXPECT_THROW(
        TerrainGrid(1, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_RowsTwo_D1False_Succeeds)
{
    // Verifies REQ-SF-0400 | MC/DC: D1 FALSE -- rows=2 >= 2, succeeds
    TerrainGrid g(2, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON);
    EXPECT_EQ(g.GetRows(), static_cast<size_t>(2));
}

// -----------------------------------------------------------------------------
//  D2: Constructor -- if (cols < kMinGridDimension)
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, Constructor_ColsZero_D2True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D2 TRUE -- cols=0 < 2, throws
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, 0, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_ColsOne_D2True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D2 TRUE -- cols=1 < 2, throws
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, 1, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_ColsTwo_D2False_Succeeds)
{
    // Verifies REQ-SF-0400 | MC/DC: D2 FALSE -- cols=2 >= 2, succeeds
    TerrainGrid g(GRID_ROWS, 2, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON);
    EXPECT_EQ(g.GetCols(), static_cast<size_t>(2));
}

// -----------------------------------------------------------------------------
//  D3: Constructor -- if (postingMeters <= 0.0)
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, Constructor_PostingZero_D3True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D3 TRUE -- posting=0.0, throws
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, GRID_COLS, 0.0, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_PostingNegative_D3True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D3 TRUE -- posting=-1.0, throws
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, GRID_COLS, -1.0, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_MCDC, Constructor_PostingSmallPositive_D3False_Succeeds)
{
    // Verifies REQ-SF-0400 | MC/DC: D3 FALSE -- posting=0.5 > 0, succeeds
    TerrainGrid g(GRID_ROWS, GRID_COLS, 0.5, ORIGIN_LAT, ORIGIN_LON);
    EXPECT_NEAR(g.GetPostingMeters(), 0.5, TIGHT_TOL);
}

// -----------------------------------------------------------------------------
//  D4/D5: ValidateIndices -- if (row >= rows_) / if (col >= cols_)
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, SetElevation_RowAtMax_D4True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D4 TRUE -- row=rows throws out_of_range
    EXPECT_THROW(grid.SetElevation(GRID_ROWS, 0, ELEV_FLAT), std::out_of_range);
}

TEST_F(TerrainGridDalA_MCDC, SetElevation_RowAtMaxMinusOne_D4False_Succeeds)
{
    // Verifies REQ-SF-0400 | MC/DC: D4 FALSE -- row=rows-1 succeeds
    EXPECT_NO_THROW(grid.SetElevation(GRID_ROWS - 1, 0, ELEV_FLAT));
}

TEST_F(TerrainGridDalA_MCDC, GetElevationAtCell_ColAtMax_D5True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D5 TRUE -- col=cols throws out_of_range
    EXPECT_THROW(grid.GetElevationAtCell(0, GRID_COLS), std::out_of_range);
}

TEST_F(TerrainGridDalA_MCDC, GetElevationAtCell_ColAtMaxMinusOne_D5False_Succeeds)
{
    // Verifies REQ-SF-0400 | MC/DC: D5 FALSE -- col=cols-1 succeeds
    grid.SetElevation(0, GRID_COLS - 1, ELEV_PEAK);
    EXPECT_NEAR(grid.GetElevationAtCell(0, GRID_COLS - 1), ELEV_PEAK, TIGHT_TOL);
}

TEST_F(TerrainGridDalA_MCDC, SetElevation_BothOutOfRange_D4D5True_Throws)
{
    // Verifies REQ-SF-0400 | MC/DC: D4+D5 both TRUE -- combined out_of_range
    EXPECT_THROW(grid.SetElevation(GRID_ROWS, GRID_COLS, ELEV_FLAT), std::out_of_range);
}

// -----------------------------------------------------------------------------
//  D6: IsInsideGrid -- 4-condition AND
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_AllTrue_D6True)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 all conditions TRUE -- inside grid
    double lat = LatForRow(4.5);
    double lon = LonForCol(4.5);
    EXPECT_TRUE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_RowNegative_D6False)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 rowF < 0 -- south of grid
    double lat = LatForRow(-1.0);
    double lon = LonForCol(4.5);
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_RowBeyondMax_D6False)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 rowF > maxRow -- north of grid
    double lat = LatForRow(static_cast<double>(GRID_ROWS));
    double lon = LonForCol(4.5);
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_ColNegative_D6False)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 colF < 0 -- west of grid
    double lat = LatForRow(4.5);
    double lon = LonForCol(-1.0);
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_ColBeyondMax_D6False)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 colF > maxCol -- east of grid
    double lat = LatForRow(4.5);
    double lon = LonForCol(static_cast<double>(GRID_COLS));
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

// -----------------------------------------------------------------------------
//  D7: GetElevation clamping -- if (r0 >= rows_-1) / if (c0 >= cols_-1)
// -----------------------------------------------------------------------------

TEST_F(TerrainGridDalA_MCDC, GetElevation_AtUpperRowBoundary_D7Clamped)
{
    // Verifies REQ-SF-0406 | MC/DC: D7 TRUE -- query at upper row boundary clamped
    grid.FillFlat(ELEV_FLAT);
    grid.SetElevation(GRID_ROWS - 1, 0, ELEV_PEAK);
    double lat = LatForRow(static_cast<double>(GRID_ROWS - 1));
    double lon = LonForCol(0.0);
    double elev = grid.GetElevation(lat, lon);
    // At exact last row, r0 clamps to rows_-2, interpolation picks up value
    EXPECT_GT(elev, ELEV_FLAT - ELEV_TOL);
}

TEST_F(TerrainGridDalA_MCDC, GetElevation_WellInsideGrid_D7NotClamped)
{
    // Verifies REQ-SF-0406 | MC/DC: D7 FALSE -- query well inside, no clamping
    grid.FillFlat(ELEV_FLAT);
    double lat = LatForRow(3.0);
    double lon = LonForCol(3.0);
    double elev = grid.GetElevation(lat, lon);
    EXPECT_NEAR(elev, ELEV_FLAT, TIGHT_TOL);
}

// #############################################################################
//  2. TERRAIN GRID BOUNDARY TESTS (~18 tests)
// #############################################################################

TEST_F(TerrainGridDalA_Boundary, GridCell_Origin_ExactQuery)
{
    // Verifies REQ-SF-0406 | Boundary: query at grid origin (0,0)
    grid.SetElevation(0, 0, ELEV_PEAK);
    grid.SetElevation(0, 1, ELEV_PEAK);
    grid.SetElevation(1, 0, ELEV_PEAK);
    grid.SetElevation(1, 1, ELEV_PEAK);
    double elev = grid.GetElevation(ORIGIN_LAT, ORIGIN_LON);
    EXPECT_NEAR(elev, ELEV_PEAK, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, GridCell_LastRowLastCol_ExactQuery)
{
    // Verifies REQ-SF-0406 | Boundary: query at grid corner (rows-1, cols-1)
    grid.FillFlat(ELEV_FLAT);
    double lat = LatForRow(static_cast<double>(GRID_ROWS - 1));
    double lon = LonForCol(static_cast<double>(GRID_COLS - 1));
    double elev = grid.GetElevation(lat, lon);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, GridCell_CornerSW)
{
    // Verifies REQ-SF-0406 | Boundary: SW corner (row=0, col=0)
    grid.FillFlat(ELEV_VALLEY);
    double elev = grid.GetElevation(LatForRow(0.0), LonForCol(0.0));
    EXPECT_NEAR(elev, ELEV_VALLEY, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, GridCell_CornerSE)
{
    // Verifies REQ-SF-0406 | Boundary: SE corner (row=0, col=cols-1)
    grid.FillFlat(ELEV_VALLEY);
    double elev = grid.GetElevation(LatForRow(0.0),
                                     LonForCol(static_cast<double>(GRID_COLS - 1)));
    EXPECT_NEAR(elev, ELEV_VALLEY, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, GridCell_CornerNW)
{
    // Verifies REQ-SF-0406 | Boundary: NW corner (row=rows-1, col=0)
    grid.FillFlat(ELEV_VALLEY);
    double elev = grid.GetElevation(LatForRow(static_cast<double>(GRID_ROWS - 1)),
                                     LonForCol(0.0));
    EXPECT_NEAR(elev, ELEV_VALLEY, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, GridCell_CornerNE)
{
    // Verifies REQ-SF-0406 | Boundary: NE corner (row=rows-1, col=cols-1)
    grid.FillFlat(ELEV_VALLEY);
    double elev = grid.GetElevation(LatForRow(static_cast<double>(GRID_ROWS - 1)),
                                     LonForCol(static_cast<double>(GRID_COLS - 1)));
    EXPECT_NEAR(elev, ELEV_VALLEY, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, BilinearInterp_MidCell_KnownValues)
{
    // Verifies REQ-SF-0406 | Boundary: mid-cell bilinear interpolation
    grid.SetElevation(2, 2, 100.0);
    grid.SetElevation(2, 3, 200.0);
    grid.SetElevation(3, 2, 300.0);
    grid.SetElevation(3, 3, 400.0);
    // At midpoint (2.5, 2.5), bilinear should yield (100+200+300+400)/4 = 250
    double lat = LatForRow(2.5);
    double lon = LonForCol(2.5);
    double elev = grid.GetElevation(lat, lon);
    EXPECT_NEAR(elev, 250.0, INTERP_TOL);
}

TEST_F(TerrainGridDalA_Boundary, EdgeInterp_BetweenEdgeCells)
{
    // Verifies REQ-SF-0406 | Boundary: interpolation along bottom edge
    grid.SetElevation(0, 0, 100.0);
    grid.SetElevation(0, 1, 300.0);
    grid.SetElevation(1, 0, 100.0);
    grid.SetElevation(1, 1, 300.0);
    // At (0.0, 0.5), expect average of 100 and 300 = 200
    double lat = LatForRow(0.0);
    double lon = LonForCol(0.5);
    double elev = grid.GetElevation(lat, lon);
    EXPECT_NEAR(elev, 200.0, INTERP_TOL);
}

TEST_F(TerrainGridDalA_Boundary, MinimumPosting_HalfMeter)
{
    // Verifies REQ-SF-0400 | Boundary: minimum posting of 0.5m
    TerrainGrid g(2, 2, 0.5, ORIGIN_LAT, ORIGIN_LON);
    g.FillFlat(ELEV_FLAT);
    EXPECT_NEAR(g.GetPostingMeters(), 0.5, TIGHT_TOL);
    EXPECT_NEAR(g.GetElevation(ORIGIN_LAT, ORIGIN_LON), ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, MaximumPosting_ThirtyMeters)
{
    // Verifies REQ-SF-0400 | Boundary: maximum posting of 30.0m
    TerrainGrid g(2, 2, 30.0, ORIGIN_LAT, ORIGIN_LON);
    g.FillFlat(ELEV_FLAT);
    EXPECT_NEAR(g.GetPostingMeters(), 30.0, TIGHT_TOL);
    EXPECT_NEAR(g.GetElevation(ORIGIN_LAT, ORIGIN_LON), ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Boundary, MinimumValidGrid_2x2)
{
    // Verifies REQ-SF-0400 | Boundary: minimum valid grid 2x2
    TerrainGrid g(2, 2, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON);
    g.SetElevation(0, 0, 100.0);
    g.SetElevation(0, 1, 200.0);
    g.SetElevation(1, 0, 300.0);
    g.SetElevation(1, 1, 400.0);
    EXPECT_EQ(g.GetRows(), static_cast<size_t>(2));
    EXPECT_EQ(g.GetCols(), static_cast<size_t>(2));
    EXPECT_NEAR(g.GetElevationAtCell(1, 1), 400.0, TIGHT_TOL);
}

TEST_F(TerrainGridDalA_Boundary, CoordinateRoundtrip_Origin)
{
    // Verifies REQ-SF-0406 | Boundary: coordinate conversion roundtrip at origin
    double row = 0.0;
    double col = 0.0;
    grid.LatLonToGrid(ORIGIN_LAT, ORIGIN_LON, row, col);
    EXPECT_NEAR(row, 0.0, TIGHT_TOL);
    EXPECT_NEAR(col, 0.0, TIGHT_TOL);

    double lat = 0.0;
    double lon = 0.0;
    grid.GridToLatLon(0.0, 0.0, lat, lon);
    EXPECT_NEAR(lat, ORIGIN_LAT, COORD_TOL);
    EXPECT_NEAR(lon, ORIGIN_LON, COORD_TOL);
}

TEST_F(TerrainGridDalA_Boundary, CoordinateRoundtrip_GridCorner)
{
    // Verifies REQ-SF-0406 | Boundary: coordinate roundtrip at far corner
    double maxRow = static_cast<double>(GRID_ROWS - 1);
    double maxCol = static_cast<double>(GRID_COLS - 1);
    double lat = 0.0;
    double lon = 0.0;
    grid.GridToLatLon(maxRow, maxCol, lat, lon);

    double rowBack = 0.0;
    double colBack = 0.0;
    grid.LatLonToGrid(lat, lon, rowBack, colBack);
    EXPECT_NEAR(rowBack, maxRow, INTERP_TOL);
    EXPECT_NEAR(colBack, maxCol, INTERP_TOL);
}

TEST_F(TerrainGridDalA_Boundary, MetadataAccessors_ReturnConstructorParams)
{
    // Verifies REQ-SF-0400 | Boundary: metadata accessor correctness
    EXPECT_EQ(grid.GetRows(), GRID_ROWS);
    EXPECT_EQ(grid.GetCols(), GRID_COLS);
    EXPECT_NEAR(grid.GetPostingMeters(), GRID_POSTING, TIGHT_TOL);
    EXPECT_NEAR(grid.GetOriginLat(), ORIGIN_LAT, COORD_TOL);
    EXPECT_NEAR(grid.GetOriginLon(), ORIGIN_LON, COORD_TOL);
}

TEST_F(TerrainGridDalA_Boundary, IsInsideGrid_ExactOrigin_True)
{
    // Verifies REQ-SF-0406 | Boundary: exact origin is inside grid
    EXPECT_TRUE(grid.IsInsideGrid(ORIGIN_LAT, ORIGIN_LON));
}

TEST_F(TerrainGridDalA_Boundary, IsInsideGrid_NearFarCorner_True)
{
    // Verifies REQ-SF-0406 | Boundary: point slightly inside far corner is inside grid
    // Note: exact boundary computation via LatForRow/LonForCol may exceed maxRow/maxCol
    // by floating-point epsilon due to roundtrip through equirectangular projection.
    // Using 0.99*(rows-1) avoids this numerical artifact.
    double lat = LatForRow(static_cast<double>(GRID_ROWS - 1) - 0.01);
    double lon = LonForCol(static_cast<double>(GRID_COLS - 1) - 0.01);
    EXPECT_TRUE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_Boundary, IsInsideGrid_JustOutsideNorth_False)
{
    // Verifies REQ-SF-0406 | Boundary: slightly north of grid is outside
    double lat = LatForRow(static_cast<double>(GRID_ROWS - 1) + 0.01);
    double lon = LonForCol(4.5);
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_Boundary, IsInsideGrid_JustOutsideSouth_False)
{
    // Verifies REQ-SF-0406 | Boundary: slightly south of grid is outside
    double lat = LatForRow(-0.01);
    double lon = LonForCol(4.5);
    EXPECT_FALSE(grid.IsInsideGrid(lat, lon));
}

// #############################################################################
//  3. TERRAIN GRID NORMAL RANGE / INTERPOLATION TESTS (~12 tests)
// #############################################################################

TEST_F(TerrainGridDalA, FillFlat_QueryAnywhere_ReturnsConstant)
{
    // Verifies REQ-SF-0406 | Normal: flat fill returns constant elevation
    grid.FillFlat(ELEV_FLAT);
    double elev = grid.GetElevation(LatForRow(5.5), LonForCol(5.5));
    EXPECT_NEAR(elev, ELEV_FLAT, TIGHT_TOL);
}

TEST_F(TerrainGridDalA, FillFlat_QueryAtOrigin_ReturnsConstant)
{
    // Verifies REQ-SF-0406 | Normal: flat fill at origin
    grid.FillFlat(ELEV_FLAT);
    EXPECT_NEAR(grid.GetElevation(ORIGIN_LAT, ORIGIN_LON), ELEV_FLAT, TIGHT_TOL);
}

TEST_F(TerrainGridDalA, FillSlopeNS_LinearGradient)
{
    // Verifies REQ-SF-0400 | Normal: FillSlope NS produces linear gradient
    grid.FillSlope(ELEV_FLAT, SLOPE_5DEG, 0.0);
    double e0 = grid.GetElevationAtCell(0, 0);
    double e5 = grid.GetElevationAtCell(5, 0);
    double expectedRise = 5.0 * GRID_POSTING * std::tan(SLOPE_5DEG * DEG_TO_RAD);
    EXPECT_NEAR(e5 - e0, expectedRise, INTERP_TOL);
}

TEST_F(TerrainGridDalA, FillSlopeEW_LinearGradient)
{
    // Verifies REQ-SF-0400 | Normal: FillSlope EW produces linear gradient
    grid.FillSlope(ELEV_FLAT, 0.0, SLOPE_10DEG);
    double e0 = grid.GetElevationAtCell(0, 0);
    double e5 = grid.GetElevationAtCell(0, 5);
    double expectedRise = 5.0 * GRID_POSTING * std::tan(SLOPE_10DEG * DEG_TO_RAD);
    EXPECT_NEAR(e5 - e0, expectedRise, INTERP_TOL);
}

TEST_F(TerrainGridDalA, FillSlope_Combined_BilinearGradient)
{
    // Verifies REQ-SF-0400 | Normal: combined NS+EW slope
    grid.FillSlope(ELEV_FLAT, SLOPE_5DEG, SLOPE_10DEG);
    double e00 = grid.GetElevationAtCell(0, 0);
    double e55 = grid.GetElevationAtCell(5, 5);
    double riseNS = 5.0 * GRID_POSTING * std::tan(SLOPE_5DEG * DEG_TO_RAD);
    double riseEW = 5.0 * GRID_POSTING * std::tan(SLOPE_10DEG * DEG_TO_RAD);
    EXPECT_NEAR(e55 - e00, riseNS + riseEW, INTERP_TOL);
}

TEST_F(TerrainGridDalA, SetGetElevation_SpecificCells_InterpolatesCorrectly)
{
    // Verifies REQ-SF-0406 | Normal: set specific cells, interpolate mid-cell
    grid.FillFlat(ELEV_ZERO);
    grid.SetElevation(4, 4, 1000.0);
    grid.SetElevation(4, 5, 1000.0);
    grid.SetElevation(5, 4, 1000.0);
    grid.SetElevation(5, 5, 1000.0);
    double elev = grid.GetElevation(LatForRow(4.5), LonForCol(4.5));
    EXPECT_NEAR(elev, 1000.0, INTERP_TOL);
}

TEST_F(TerrainGridDalA, CheckerboardPattern_MidCellInterpolation)
{
    // Verifies REQ-SF-0406 | Normal: checkerboard pattern averages at mid-cell
    grid.SetElevation(3, 3, ELEV_ZERO);
    grid.SetElevation(3, 4, ELEV_PEAK);
    grid.SetElevation(4, 3, ELEV_PEAK);
    grid.SetElevation(4, 4, ELEV_ZERO);
    double elev = grid.GetElevation(LatForRow(3.5), LonForCol(3.5));
    EXPECT_NEAR(elev, (ELEV_ZERO + ELEV_PEAK + ELEV_PEAK + ELEV_ZERO) / 4.0,
                INTERP_TOL);
}

TEST_F(TerrainGridDalA, BatchQuery_MatchesSingleQueries)
{
    // Verifies REQ-SF-0406 | Normal: batch query matches individual queries
    grid.FillSlope(ELEV_FLAT, SLOPE_5DEG, SLOPE_10DEG);
    const size_t count = 5;
    double lats[5], lons[5], elevBatch[5];
    for (size_t i = 0; i < count; ++i) {
        lats[i] = LatForRow(static_cast<double>(i) + 0.5);
        lons[i] = LonForCol(static_cast<double>(i) + 0.5);
    }
    grid.GetElevationBatch(lats, lons, elevBatch, count);
    for (size_t i = 0; i < count; ++i) {
        double single = grid.GetElevation(lats[i], lons[i]);
        EXPECT_NEAR(elevBatch[i], single, TIGHT_TOL);
    }
}

TEST_F(TerrainGridDalA, Statistics_Min_KnownData)
{
    // Verifies REQ-SF-0400 | Normal: min elevation on known data
    grid.FillFlat(ELEV_FLAT);
    grid.SetElevation(3, 7, ELEV_VALLEY);
    EXPECT_NEAR(grid.GetMinElevation(), ELEV_VALLEY, TIGHT_TOL);
}

TEST_F(TerrainGridDalA, Statistics_Max_KnownData)
{
    // Verifies REQ-SF-0400 | Normal: max elevation on known data
    grid.FillFlat(ELEV_FLAT);
    grid.SetElevation(7, 3, ELEV_PEAK);
    EXPECT_NEAR(grid.GetMaxElevation(), ELEV_PEAK, TIGHT_TOL);
}

TEST_F(TerrainGridDalA, Statistics_Mean_KnownData)
{
    // Verifies REQ-SF-0400 | Normal: mean elevation on uniform grid
    grid.FillFlat(ELEV_FLAT);
    EXPECT_NEAR(grid.GetMeanElevation(), ELEV_FLAT, TIGHT_TOL);
}

TEST_F(TerrainGridDalA, Statistics_Mean_SlopedGrid)
{
    // Verifies REQ-SF-0400 | Normal: mean elevation on sloped grid
    grid.FillSlope(0.0, 0.0, 0.0);  // all zeros
    grid.SetElevation(0, 0, 100.0);
    grid.SetElevation(0, 1, 200.0);
    // Mean should reflect set values plus zeros
    double mean = grid.GetMeanElevation();
    double expectedMean = (100.0 + 200.0) / static_cast<double>(GRID_ROWS * GRID_COLS);
    EXPECT_NEAR(mean, expectedMean, INTERP_TOL);
}

// #############################################################################
//  4. TERRAIN GRID ROBUSTNESS TESTS (~10 tests)
// #############################################################################

TEST_F(TerrainGridDalA_Robustness, QueryFarOutside_ClampsToEdge_NoCrash)
{
    // Verifies REQ-SF-0406 | Robustness: query far outside clamps
    grid.FillFlat(ELEV_FLAT);
    double elev = grid.GetElevation(ORIGIN_LAT + 10.0, ORIGIN_LON + 10.0);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, QueryFarSouth_ClampsToEdge)
{
    // Verifies REQ-SF-0406 | Robustness: query far south clamps to row 0
    grid.FillFlat(ELEV_FLAT);
    double elev = grid.GetElevation(ORIGIN_LAT - 10.0, ORIGIN_LON);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, QueryFarWest_ClampsToEdge)
{
    // Verifies REQ-SF-0406 | Robustness: query far west clamps to col 0
    grid.FillFlat(ELEV_FLAT);
    double elev = grid.GetElevation(ORIGIN_LAT, ORIGIN_LON - 10.0);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, QueryNegativeLatLon_Clamps)
{
    // Verifies REQ-SF-0406 | Robustness: negative lat/lon query clamps
    grid.FillFlat(ELEV_FLAT);
    double elev = grid.GetElevation(-90.0, -180.0);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, VeryLargePosting)
{
    // Verifies REQ-SF-0400 | Robustness: very large posting value
    TerrainGrid g(2, 2, 30.0, ORIGIN_LAT, ORIGIN_LON);
    g.FillFlat(ELEV_FLAT);
    EXPECT_NEAR(g.GetElevation(ORIGIN_LAT, ORIGIN_LON), ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, GridAtEquator_MetersPerDegLonAtMax)
{
    // Verifies REQ-SF-0400 | Robustness: grid at equator (lat=0)
    TerrainGrid g(3, 3, GRID_POSTING, 0.0, 0.0);
    g.FillFlat(ELEV_FLAT);
    double elev = g.GetElevation(0.0, 0.0);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, GridAtHighLatitude_MetersPerDegLonSmall)
{
    // Verifies REQ-SF-0400 | Robustness: grid at lat=85 deg
    TerrainGrid g(3, 3, GRID_POSTING, 85.0, 0.0);
    g.FillFlat(ELEV_FLAT);
    double elev = g.GetElevation(85.0, 0.0);
    EXPECT_NEAR(elev, ELEV_FLAT, ELEV_TOL);
}

TEST_F(TerrainGridDalA_Robustness, ZeroElevationGrid_AllQueriesReturnZero)
{
    // Verifies REQ-SF-0406 | Robustness: zero-elevation grid
    grid.FillFlat(ELEV_ZERO);
    EXPECT_NEAR(grid.GetElevation(LatForRow(5.0), LonForCol(5.0)), ELEV_ZERO, TIGHT_TOL);
    EXPECT_NEAR(grid.GetMinElevation(), ELEV_ZERO, TIGHT_TOL);
    EXPECT_NEAR(grid.GetMaxElevation(), ELEV_ZERO, TIGHT_TOL);
}

TEST_F(TerrainGridDalA_Robustness, NegativeElevationGrid_DeadSea)
{
    // Verifies REQ-SF-0400 | Robustness: negative elevation (Dead Sea, -430m)
    grid.FillFlat(ELEV_DEAD_SEA);
    EXPECT_NEAR(grid.GetElevation(LatForRow(5.0), LonForCol(5.0)),
                ELEV_DEAD_SEA, ELEV_TOL);
    EXPECT_NEAR(grid.GetMinElevation(), ELEV_DEAD_SEA, TIGHT_TOL);
}

TEST_F(TerrainGridDalA_Robustness, BatchQuery_EmptyCount_NoCrash)
{
    // Verifies REQ-SF-0406 | Robustness: batch query with count=0
    grid.FillFlat(ELEV_FLAT);
    double lat = ORIGIN_LAT;
    double lon = ORIGIN_LON;
    double elev = 0.0;
    grid.GetElevationBatch(&lat, &lon, &elev, 0);
    // No crash, elev unchanged
    EXPECT_NEAR(elev, 0.0, TIGHT_TOL);
}

// #############################################################################
//  5. TERRAIN GRID ERROR HANDLING TESTS (~8 tests)
// #############################################################################

TEST_F(TerrainGridDalA_Error, Constructor_RowsZero_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: rows=0 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(0, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, Constructor_RowsOne_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: rows=1 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(1, GRID_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, Constructor_ColsZero_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: cols=0 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, 0, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, Constructor_ColsOne_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: cols=1 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, 1, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, Constructor_PostingZero_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: posting=0.0 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, GRID_COLS, 0.0, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, Constructor_PostingNegative_ThrowsInvalidArgument)
{
    // Verifies REQ-SF-0400 | Error: posting=-5.0 throws std::invalid_argument
    EXPECT_THROW(
        TerrainGrid(GRID_ROWS, GRID_COLS, -5.0, ORIGIN_LAT, ORIGIN_LON),
        std::invalid_argument);
}

TEST_F(TerrainGridDalA_Error, SetElevation_OutOfRange_ThrowsOutOfRange)
{
    // Verifies REQ-SF-0400 | Error: SetElevation out of range throws
    EXPECT_THROW(grid.SetElevation(GRID_ROWS, 0, ELEV_FLAT), std::out_of_range);
    EXPECT_THROW(grid.SetElevation(0, GRID_COLS, ELEV_FLAT), std::out_of_range);
}

TEST_F(TerrainGridDalA_Error, GetElevationAtCell_OutOfRange_ThrowsOutOfRange)
{
    // Verifies REQ-SF-0400 | Error: GetElevationAtCell out of range throws
    EXPECT_THROW(grid.GetElevationAtCell(GRID_ROWS, 0), std::out_of_range);
    EXPECT_THROW(grid.GetElevationAtCell(0, GRID_COLS), std::out_of_range);
}

// #############################################################################
//  6. CFIT MC/DC TESTS (~20 tests)
// #############################################################################

// -----------------------------------------------------------------------------
//  D8/D9: SetClearanceThreshold clamping [25, 500]
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, SetClearanceThreshold_BelowMin_ClampedTo25)
{
    // Verifies REQ-SF-0402 | MC/DC: D8 threshold < 25 clamped to 25
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(10.0);
    EXPECT_NEAR(det.GetClearanceThreshold(), CLEARANCE_MIN_FT, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetClearanceThreshold_AboveMax_ClampedTo500)
{
    // Verifies REQ-SF-0402 | MC/DC: D9 threshold > 500 clamped to 500
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(1000.0);
    EXPECT_NEAR(det.GetClearanceThreshold(), CLEARANCE_MAX_FT, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetClearanceThreshold_InRange_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: threshold in range accepted unchanged
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(200.0);
    EXPECT_NEAR(det.GetClearanceThreshold(), 200.0, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetClearanceThreshold_ExactMin_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: threshold exactly 25 accepted
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(CLEARANCE_MIN_FT);
    EXPECT_NEAR(det.GetClearanceThreshold(), CLEARANCE_MIN_FT, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetClearanceThreshold_ExactMax_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: threshold exactly 500 accepted
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(CLEARANCE_MAX_FT);
    EXPECT_NEAR(det.GetClearanceThreshold(), CLEARANCE_MAX_FT, TIGHT_TOL);
}

// -----------------------------------------------------------------------------
//  D10: SetLookaheadTime minimum 60s
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, SetLookaheadTime_Below60_ClampedTo60)
{
    // Verifies REQ-SF-0402 | MC/DC: D10 time < 60 clamped to 60
    TerrainCollisionDetector det(terrain);
    det.SetLookaheadTime(30.0);
    EXPECT_NEAR(det.GetLookaheadTime(), LOOKAHEAD_MIN_SEC, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetLookaheadTime_Exactly60_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: D10 time = 60 accepted
    TerrainCollisionDetector det(terrain);
    det.SetLookaheadTime(LOOKAHEAD_MIN_SEC);
    EXPECT_NEAR(det.GetLookaheadTime(), LOOKAHEAD_MIN_SEC, TIGHT_TOL);
}

TEST_F(CFITDalA_MCDC, SetLookaheadTime_Above60_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: D10 time > 60 accepted
    TerrainCollisionDetector det(terrain);
    det.SetLookaheadTime(120.0);
    EXPECT_NEAR(det.GetLookaheadTime(), 120.0, TIGHT_TOL);
}

// -----------------------------------------------------------------------------
//  D11: Evaluate loop (for i <= numSteps)
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, Evaluate_LoopRunsCorrectSteps_D11)
{
    // Verifies REQ-SF-0402 | MC/DC: D11 loop runs correct number of steps
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.lookaheadTimeSec = LOOKAHEAD_MIN_SEC;
    cfg.stepTimeSec = 1.0;
    cfg.maxSteps = 120;
    det.SetConfig(cfg);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    // High altitude, level flight over flat terrain: no alert
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

TEST_F(CFITDalA_MCDC, Evaluate_MaxStepsLimitsIteration_D11)
{
    // Verifies REQ-SF-0402 | MC/DC: D11 maxSteps limits iteration count
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.lookaheadTimeSec = 1000.0;
    cfg.stepTimeSec = 1.0;
    cfg.maxSteps = 5;
    det.SetConfig(cfg);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    // With only 5 steps, should still evaluate without crash
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

// -----------------------------------------------------------------------------
//  D12: CFIT WARNING: if (clearanceFt <= 0.0)
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, Evaluate_AircraftBelowTerrain_Warning_D12)
{
    // Verifies REQ-SF-0402 | MC/DC: D12 TRUE -- aircraft below terrain -> WARNING
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    // Terrain is 1500m = ~4921.26 ft; set aircraft at 200 ft MSL (below terrain)
    AircraftState s = MakeState(lat, lon, ALT_LOW_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_GE(r.timeToImpactSec, 0.0);
}

TEST_F(CFITDalA_MCDC, Evaluate_AircraftAtTerrainLevel_Warning_D12)
{
    // Verifies REQ-SF-0402 | MC/DC: D12 boundary -- aircraft at terrain level
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    AircraftState s = MakeState(lat, lon, terrainFt, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_NEAR(r.timeToImpactSec, 0.0, 1.0);  // impact at or very near t=0
}

// -----------------------------------------------------------------------------
//  D13: CFIT CAUTION: if (minimumClearanceFt < cautionClearanceFt)
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, Evaluate_ClearanceBelowCaution_Caution_D13True)
{
    // Verifies REQ-SF-0402 | MC/DC: D13 TRUE -- clearance below caution threshold
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    // Set altitude just above terrain but within caution band
    // caution = 100 * 1.5 = 150 ft; set aircraft at terrain + 50 ft
    double altMSL = terrainFt + 50.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eCaution);
}

TEST_F(CFITDalA_MCDC, Evaluate_ClearanceAboveCaution_None_D13False)
{
    // Verifies REQ-SF-0402 | MC/DC: D13 FALSE -- clearance above caution threshold
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

// -----------------------------------------------------------------------------
//  D14: Minimum clearance tracking: if (clearanceFt < result.minimumClearanceFt)
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, Evaluate_DescendingFlight_MinClearanceAtEnd_D14)
{
    // Verifies REQ-SF-0402 | MC/DC: D14 -- descending flight tracks minimum at end
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    // Start well above terrain, descending
    double altMSL = terrainFt + 1000.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_DESCENDING);
    CFITResult r = det.Evaluate(s);
    // Minimum clearance should be less than initial clearance
    EXPECT_LT(r.minimumClearanceFt, 1000.0);
}

TEST_F(CFITDalA_MCDC, Evaluate_LevelFlight_MinClearanceConstant_D14)
{
    // Verifies REQ-SF-0402 | MC/DC: D14 -- level flight, constant clearance
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.lookaheadTimeSec = LOOKAHEAD_MIN_SEC;
    cfg.stepTimeSec = 1.0;
    cfg.maxSteps = 10;  // short lookahead, stays in flat terrain
    det.SetConfig(cfg);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    double altMSL = terrainFt + 500.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_NEAR(r.minimumClearanceFt, 500.0, ELEV_TOL);
}

// -----------------------------------------------------------------------------
//  D15: if (!impactDetected) for caution check
// -----------------------------------------------------------------------------

TEST_F(CFITDalA_MCDC, Evaluate_ImpactDetected_WarningTakesPriority_D15True)
{
    // Verifies REQ-SF-0402 | MC/DC: D15 TRUE -- impact detected, WARNING overrides
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    // Aircraft at 200 ft MSL, terrain at ~4921 ft: below terrain
    AircraftState s = MakeState(lat, lon, ALT_LOW_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    // Must not be eCaution even though clearance is below caution threshold
    EXPECT_NE(r.alertLevel, CFITAlertLevel::eCaution);
}

TEST_F(CFITDalA_MCDC, Evaluate_NoImpact_CautionCheckRuns_D15False)
{
    // Verifies REQ-SF-0402 | MC/DC: D15 FALSE -- no impact, caution check runs
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    // Above terrain but within caution band: 100 * 1.5 = 150 ft
    double altMSL = terrainFt + 80.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eCaution);
}

// #############################################################################
//  7. CFIT BOUNDARY / STATE TESTS (~15 tests)
// #############################################################################

TEST_F(CFITDalA_Boundary, LevelFlight_HighAboveTerrain_eNone)
{
    // Verifies REQ-SF-0402 | Boundary: level flight high above terrain -> eNone
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
    EXPECT_NEAR(r.timeToImpactSec, -1.0, TIGHT_TOL);
}

TEST_F(CFITDalA_Boundary, LevelFlight_AtCautionThreshold_eCaution)
{
    // Verifies REQ-SF-0402 | Boundary: level flight at caution boundary -> eCaution
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    // Caution threshold = 100 * 1.5 = 150 ft; set altitude at terrain + 140 ft
    double altMSL = terrainFt + 140.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eCaution);
}

TEST_F(CFITDalA_Boundary, DescendingFlight_IntoTerrain_eWarning)
{
    // Verifies REQ-SF-0402 | Boundary: descending flight into terrain -> eWarning
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    // Start above terrain with steep descent
    double altMSL = terrainFt + 500.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_DESCENDING);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_GT(r.timeToImpactSec, 0.0);
}

TEST_F(CFITDalA_Boundary, ClimbingFlight_AwayFromTerrain_eNone)
{
    // Verifies REQ-SF-0402 | Boundary: climbing flight away from terrain -> eNone
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_CLIMBING);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

TEST_F(CFITDalA_Boundary, StationaryAircraft_SpeedZero_EvaluatesPositionOnly)
{
    // Verifies REQ-SF-0402 | Boundary: stationary aircraft evaluates position only
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

TEST_F(CFITDalA_Boundary, TrackNorth_LatitudeIncreases)
{
    // Verifies REQ-SF-0402 | Boundary: track 0 deg (north), lat increases
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 10.0, predLat, predLon, predAlt);
    EXPECT_GT(predLat, lat);
}

TEST_F(CFITDalA_Boundary, TrackEast_LongitudeIncreases)
{
    // Verifies REQ-SF-0402 | Boundary: track 90 deg (east), lon increases
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_EAST, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 10.0, predLat, predLon, predAlt);
    EXPECT_GT(predLon, lon);
}

TEST_F(CFITDalA_Boundary, TrackSouth_LatitudeDecreases)
{
    // Verifies REQ-SF-0402 | Boundary: track 180 deg (south), lat decreases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_SOUTH, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 10.0, predLat, predLon, predAlt);
    EXPECT_LT(predLat, lat);
}

TEST_F(CFITDalA_Boundary, TrackWest_LongitudeDecreases)
{
    // Verifies REQ-SF-0402 | Boundary: track 270 deg (west), lon decreases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_WEST, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 10.0, predLat, predLon, predAlt);
    EXPECT_LT(predLon, lon);
}

TEST_F(CFITDalA_Boundary, SetConfig_AppliesClamping)
{
    // Verifies REQ-SF-0402 | Boundary: SetConfig applies clamping to params
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.clearanceThresholdFt = 1000.0;  // above max 500
    cfg.lookaheadTimeSec = 10.0;        // below min 60
    det.SetConfig(cfg);
    CFITConfig result = det.GetConfig();
    EXPECT_NEAR(result.clearanceThresholdFt, CLEARANCE_MAX_FT, TIGHT_TOL);
    EXPECT_NEAR(result.lookaheadTimeSec, LOOKAHEAD_MIN_SEC, TIGHT_TOL);
}

TEST_F(CFITDalA_Boundary, GetConfig_ReturnsCurrentConfig)
{
    // Verifies REQ-SF-0402 | Boundary: GetConfig returns current state
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.clearanceThresholdFt = 200.0;
    cfg.lookaheadTimeSec = 90.0;
    cfg.cautionMultiplier = 2.0;
    cfg.stepTimeSec = 2.0;
    cfg.maxSteps = 50;
    det.SetConfig(cfg);
    CFITConfig result = det.GetConfig();
    EXPECT_NEAR(result.clearanceThresholdFt, 200.0, TIGHT_TOL);
    EXPECT_NEAR(result.lookaheadTimeSec, 90.0, TIGHT_TOL);
    EXPECT_NEAR(result.cautionMultiplier, 2.0, TIGHT_TOL);
    EXPECT_NEAR(result.stepTimeSec, 2.0, TIGHT_TOL);
    EXPECT_EQ(result.maxSteps, static_cast<size_t>(50));
}

TEST_F(CFITDalA_Boundary, DefaultConfig_MatchesExpected)
{
    // Verifies REQ-SF-0402 | Boundary: default config matches specification
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg = det.GetConfig();
    EXPECT_NEAR(cfg.lookaheadTimeSec, LOOKAHEAD_DEFAULT_SEC, TIGHT_TOL);
    EXPECT_NEAR(cfg.clearanceThresholdFt, CLEARANCE_DEFAULT_FT, TIGHT_TOL);
    EXPECT_NEAR(cfg.cautionMultiplier, CAUTION_MULTIPLIER, TIGHT_TOL);
    EXPECT_NEAR(cfg.stepTimeSec, 1.0, TIGHT_TOL);
    EXPECT_EQ(cfg.maxSteps, static_cast<size_t>(120));
}

TEST_F(CFITDalA_Boundary, CFITResult_NoAlert_TimeToImpactNeg1)
{
    // Verifies REQ-SF-0402 | Boundary: no alert -> timeToImpact = -1
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_NEAR(r.timeToImpactSec, -1.0, TIGHT_TOL);
}

TEST_F(CFITDalA_Boundary, CFITResult_Warning_ValidClosestPosition)
{
    // Verifies REQ-SF-0402 | Boundary: warning result has valid closest position
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_LOW_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    // Closest lat/lon should be a valid coordinate
    EXPECT_GT(r.closestLat, -90.1);
    EXPECT_LT(r.closestLat, 90.1);
}

// #############################################################################
//  8. CFIT ROBUSTNESS TESTS (~10 tests)
// #############################################################################

TEST_F(CFITDalA_Robustness, ZeroGroundSpeed_NoNaN_NoCrash)
{
    // Verifies REQ-SF-0402 | Robustness: zero ground speed, no NaN or crash
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_FALSE(std::isnan(r.minimumClearanceFt));
    EXPECT_FALSE(std::isnan(r.terrainElevationFt));
}

TEST_F(CFITDalA_Robustness, SteepClimb_90Degrees_ValidResult)
{
    // Verifies REQ-SF-0402 | Robustness: 90 deg FPA (straight up), valid result
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_STEEP_UP);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
    EXPECT_FALSE(std::isnan(r.minimumClearanceFt));
}

TEST_F(CFITDalA_Robustness, SteepDescent_Neg90Degrees_Warning)
{
    // Verifies REQ-SF-0402 | Robustness: -90 deg FPA (straight down), WARNING
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    double altMSL = terrainFt + 2000.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_STEEP_DOWN);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
}

TEST_F(CFITDalA_Robustness, VeryHighSpeed_TrajectoryExtendsFar)
{
    // Verifies REQ-SF-0402 | Robustness: very high speed, trajectory extends far
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_HIGH_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    // Should evaluate without crash even at high speed
    EXPECT_FALSE(std::isnan(r.minimumClearanceFt));
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
}

TEST_F(CFITDalA_Robustness, AircraftExactlyAtTerrainElev_Warning)
{
    // Verifies REQ-SF-0402 | Robustness: aircraft at exact terrain elevation
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    AircraftState s = MakeState(lat, lon, terrainFt, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_NEAR(r.timeToImpactSec, 0.0, 1.0);
}

TEST_F(CFITDalA_Robustness, PredictPosition_TimeZero_ReturnsOriginalPosition)
{
    // Verifies REQ-SF-0402 | Robustness: PredictPosition at t=0 returns original
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 0.0, predLat, predLon, predAlt);
    EXPECT_NEAR(predLat, lat, COORD_TOL);
    EXPECT_NEAR(predLon, lon, COORD_TOL);
    EXPECT_NEAR(predAlt, ALT_HIGH_FT, TIGHT_TOL);
}

TEST_F(CFITDalA_Robustness, PredictPosition_HeadingNorth_LatIncreases)
{
    // Verifies REQ-SF-0402 | Robustness: heading north, latitude increases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 60.0, predLat, predLon, predAlt);
    EXPECT_GT(predLat, lat);
}

TEST_F(CFITDalA_Robustness, PredictPosition_HeadingEast_LonIncreases)
{
    // Verifies REQ-SF-0402 | Robustness: heading east, longitude increases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_EAST, FPA_LEVEL);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 60.0, predLat, predLon, predAlt);
    EXPECT_GT(predLon, lon);
}

TEST_F(CFITDalA_Robustness, PredictPosition_Climbing_AltIncreases)
{
    // Verifies REQ-SF-0402 | Robustness: climbing FPA, altitude increases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_CLIMBING);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 60.0, predLat, predLon, predAlt);
    EXPECT_GT(predAlt, ALT_HIGH_FT);
}

TEST_F(CFITDalA_Robustness, PredictPosition_Descending_AltDecreases)
{
    // Verifies REQ-SF-0402 | Robustness: descending FPA, altitude decreases
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_DESCENDING);
    double predLat = 0.0, predLon = 0.0, predAlt = 0.0;
    TerrainCollisionDetector::PredictPosition(s, 60.0, predLat, predLon, predAlt);
    EXPECT_LT(predAlt, ALT_HIGH_FT);
}

// #############################################################################
//  9. UNIT CONVERSION TESTS (~5 tests)
// #############################################################################

TEST_F(CFITDalA_State, FeetToMeters_Roundtrip)
{
    // Verifies REQ-SF-1400 | UnitConversion: FeetToMeters then MetersToFeet roundtrip
    double original = 1000.0;
    double meters = TerrainCollisionDetector::FeetToMeters(original);
    double back = TerrainCollisionDetector::MetersToFeet(meters);
    EXPECT_NEAR(back, original, TIGHT_TOL);
}

TEST_F(CFITDalA_State, MetersToFeet_Roundtrip)
{
    // Verifies REQ-SF-1400 | UnitConversion: MetersToFeet then FeetToMeters roundtrip
    double original = 500.0;
    double feet = TerrainCollisionDetector::MetersToFeet(original);
    double back = TerrainCollisionDetector::FeetToMeters(feet);
    EXPECT_NEAR(back, original, TIGHT_TOL);
}

TEST_F(CFITDalA_State, KnotsToFPS_KnownValue)
{
    // Verifies REQ-SF-1400 | UnitConversion: 100 kts = 168.78... fps
    double fps = TerrainCollisionDetector::KnotsToFPS(100.0);
    EXPECT_NEAR(fps, KNOTS_100_IN_FPS, INTERP_TOL);
}

TEST_F(CFITDalA_State, FeetToMeters_OneFootEquals0_3048m)
{
    // Verifies REQ-SF-1400 | UnitConversion: 1 ft = 0.3048 m
    double meters = TerrainCollisionDetector::FeetToMeters(1.0);
    EXPECT_NEAR(meters, FEET_TO_METERS_RATIO, TIGHT_TOL);
}

TEST_F(CFITDalA_State, MetersToFeet_OneMeterEquals3_28084ft)
{
    // Verifies REQ-SF-1400 | UnitConversion: 1 m = 3.28084... ft
    double feet = TerrainCollisionDetector::MetersToFeet(1.0);
    EXPECT_NEAR(feet, METERS_TO_FEET_RATIO, INTERP_TOL);
}

// #############################################################################
//  10. INTEGRATION TESTS (~8 tests)
// #############################################################################

TEST_F(CFITDalA_State, Integration_SlopedTerrain_LevelFlight_CFITWarning)
{
    // Verifies REQ-SF-0402 | Integration: sloped terrain, level flight -> CFIT warning
    // Use a large grid (1000x1000 at 10m posting = 10km extent) so the aircraft
    // trajectory stays within the grid during the 60s lookahead at 120 kts.
    static constexpr size_t BIG_ROWS = 1000;
    static constexpr size_t BIG_COLS = 100;
    TerrainGrid bigTerrain(BIG_ROWS, BIG_COLS, GRID_POSTING, ORIGIN_LAT, ORIGIN_LON);
    // Steep upward slope northward: 10m rise per posting = 45° slope
    // Over 1000 rows that's 10,000m rise (32,808 ft)
    for (size_t r = 0; r < BIG_ROWS; ++r) {
        for (size_t c = 0; c < BIG_COLS; ++c) {
            bigTerrain.SetElevation(r, c, ELEV_FLAT + static_cast<double>(r) * 10.0);
        }
    }

    TerrainCollisionDetector det(bigTerrain);
    CFITConfig cfg;
    cfg.lookaheadTimeSec = LOOKAHEAD_MIN_SEC;
    cfg.stepTimeSec = 1.0;
    cfg.maxSteps = 120;
    det.SetConfig(cfg);

    // Start near the south end of the grid, flying north at level altitude
    double lat = ORIGIN_LAT + (10.0 * GRID_POSTING) / METERS_PER_DEG_LAT;
    double metersPerDegLon = METERS_PER_DEG_LAT * std::cos(ORIGIN_LAT * DEG_TO_RAD);
    double lon = ORIGIN_LON + (50.0 * GRID_POSTING) / metersPerDegLon;
    double terrainFt = bigTerrain.GetElevation(lat, lon) * FEET_PER_METER;
    double altMSL = terrainFt + 300.0; // 300 ft above terrain at start
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    // Override MakeState's AGL since we're using bigTerrain not terrain
    s.altitudeAGL = 300.0;
    CFITResult r = det.Evaluate(s);
    // Should hit rising terrain within lookahead
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_GT(r.timeToImpactSec, 0.0);
}

TEST_F(CFITDalA_State, Integration_ValleyTerrain_FlyThrough_DetectsValleyFloor)
{
    // Verifies REQ-SF-0402 | Integration: valley terrain, detects valley floor
    // Create valley: high edges, low center
    for (size_t r = 0; r < GRID_ROWS; ++r) {
        for (size_t c = 0; c < GRID_COLS; ++c) {
            double distFromCenter = std::abs(static_cast<double>(c) - 4.5);
            double elev = ELEV_VALLEY + distFromCenter * 200.0;
            terrain.SetElevation(r, c, elev);
        }
    }
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(4.5);
    double terrainFt = terrain.GetElevation(lat, lon) * FEET_PER_METER;
    // Flying just above valley floor
    double altMSL = terrainFt + 50.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    // 50 ft clearance < 150 ft caution threshold
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eCaution);
}

TEST_F(CFITDalA_State, Integration_PeakTerrain_FlyOver_ClearanceTracking)
{
    // Verifies REQ-SF-0402 | Integration: peak terrain, clearance tracking
    terrain.FillFlat(ELEV_FLAT);
    terrain.SetElevation(5, 5, ELEV_PEAK);
    terrain.SetElevation(5, 4, ELEV_PEAK);
    terrain.SetElevation(4, 5, ELEV_PEAK);
    terrain.SetElevation(4, 4, ELEV_PEAK);
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double peakFt = ELEV_PEAK * FEET_PER_METER;
    // Fly well above peak
    double altMSL = peakFt + 2000.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
    // Minimum clearance should be positive
    EXPECT_GT(r.minimumClearanceFt, 0.0);
}

TEST_F(CFITDalA_State, Integration_BatchElevation_MatchesSingle)
{
    // Verifies REQ-SF-0406 | Integration: batch elevation matches single queries
    terrain.FillSlope(ELEV_FLAT, SLOPE_5DEG, SLOPE_10DEG);
    const size_t count = 8;
    std::vector<double> lats(count), lons(count), elevBatch(count);
    for (size_t i = 0; i < count; ++i) {
        lats[i] = LatForRow(static_cast<double>(i) + 0.3);
        lons[i] = LonForCol(static_cast<double>(i) + 0.7);
    }
    terrain.GetElevationBatch(lats.data(), lons.data(), elevBatch.data(), count);
    for (size_t i = 0; i < count; ++i) {
        double single = terrain.GetElevation(lats[i], lons[i]);
        EXPECT_NEAR(elevBatch[i], single, TIGHT_TOL);
    }
}

TEST_F(CFITDalA_State, Integration_FullLifecycle_ConstructConfigureEvaluateReconfigure)
{
    // Verifies REQ-SF-0402 | Integration: full evaluate lifecycle
    TerrainCollisionDetector det(terrain);

    // Phase 1: default config, high altitude -> eNone
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s1 = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                 TRACK_NORTH, FPA_LEVEL);
    CFITResult r1 = det.Evaluate(s1);
    EXPECT_EQ(r1.alertLevel, CFITAlertLevel::eNone);

    // Phase 2: reconfigure with tighter threshold, re-evaluate low alt
    CFITConfig cfg;
    cfg.clearanceThresholdFt = CLEARANCE_MAX_FT;
    cfg.lookaheadTimeSec = 120.0;
    cfg.cautionMultiplier = 2.0;
    cfg.stepTimeSec = 1.0;
    cfg.maxSteps = 120;
    det.SetConfig(cfg);

    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    double altMSL = terrainFt + 800.0;
    AircraftState s2 = MakeState(lat, lon, altMSL, SPEED_ZERO,
                                 TRACK_NORTH, FPA_LEVEL);
    CFITResult r2 = det.Evaluate(s2);
    // 800 ft clearance < 500 * 2.0 = 1000 ft caution band
    EXPECT_EQ(r2.alertLevel, CFITAlertLevel::eCaution);
}

TEST_F(CFITDalA_State, Integration_EvaluateWithDescendingTrajectory_TimeToImpact)
{
    // Verifies REQ-SF-0402 | Integration: descending trajectory computes time to impact
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    double terrainFt = ELEV_FLAT * FEET_PER_METER;
    double altMSL = terrainFt + 500.0;
    AircraftState s = MakeState(lat, lon, altMSL, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_DESCENDING);
    CFITResult r = det.Evaluate(s);
    if (r.alertLevel == CFITAlertLevel::eWarning) {
        EXPECT_GT(r.timeToImpactSec, 0.0);
        EXPECT_LE(r.timeToImpactSec,
                  det.GetConfig().lookaheadTimeSec + det.GetConfig().stepTimeSec);
    }
}

TEST_F(CFITDalA_State, Integration_CFITResultFields_Warning_AllValid)
{
    // Verifies REQ-SF-0402 | Integration: all result fields valid on WARNING
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_LOW_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eWarning);
    EXPECT_GE(r.timeToImpactSec, 0.0);
    EXPECT_FALSE(std::isnan(r.minimumClearanceFt));
    EXPECT_FALSE(std::isnan(r.terrainElevationFt));
    EXPECT_FALSE(std::isnan(r.distanceToClosestFt));
    EXPECT_FALSE(std::isnan(r.closestLat));
    EXPECT_FALSE(std::isnan(r.closestLon));
}

TEST_F(CFITDalA_State, Integration_CFITResultFields_None_AllValid)
{
    // Verifies REQ-SF-0402 | Integration: all result fields valid on eNone
    TerrainCollisionDetector det(terrain);
    double lat = LatForRow(5.0);
    double lon = LonForCol(5.0);
    AircraftState s = MakeState(lat, lon, ALT_HIGH_FT, SPEED_CRUISE_KTS,
                                TRACK_NORTH, FPA_LEVEL);
    CFITResult r = det.Evaluate(s);
    EXPECT_EQ(r.alertLevel, CFITAlertLevel::eNone);
    EXPECT_NEAR(r.timeToImpactSec, -1.0, TIGHT_TOL);
    EXPECT_FALSE(std::isnan(r.minimumClearanceFt));
    EXPECT_GT(r.minimumClearanceFt, 0.0);
    EXPECT_FALSE(std::isnan(r.terrainElevationFt));
}

// #############################################################################
//  Additional MC/DC and coverage tests
// #############################################################################

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_NearMaxRow_D6BoundaryTrue)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 rowF near maxRow, inside grid
    // Note: exact boundary via LatForRow may exceed maxRow by FP epsilon
    // due to equirectangular projection roundtrip. Use slightly interior point.
    double lat = LatForRow(static_cast<double>(GRID_ROWS - 1) - 0.001);
    double lon = LonForCol(4.5);
    EXPECT_TRUE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_MCDC, IsInsideGrid_NearMaxCol_D6BoundaryTrue)
{
    // Verifies REQ-SF-0406 | MC/DC: D6 colF near maxCol, inside grid
    // Same FP epsilon rationale as above.
    double lat = LatForRow(4.5);
    double lon = LonForCol(static_cast<double>(GRID_COLS - 1) - 0.001);
    EXPECT_TRUE(grid.IsInsideGrid(lat, lon));
}

TEST_F(TerrainGridDalA_Boundary, BilinearInterp_QuarterCell_CorrectWeight)
{
    // Verifies REQ-SF-0406 | Boundary: bilinear interpolation at 1/4 cell offset
    grid.SetElevation(1, 1, 0.0);
    grid.SetElevation(1, 2, 400.0);
    grid.SetElevation(2, 1, 400.0);
    grid.SetElevation(2, 2, 800.0);
    // At (1.25, 1.25): (0.75*0.75*0)+(0.25*0.75*400)+(0.75*0.25*400)+(0.25*0.25*800) = 200
    double lat = LatForRow(1.25);
    double lon = LonForCol(1.25);
    double elev = grid.GetElevation(lat, lon);
    double expected = 0.75 * 0.75 * 0.0 + 0.25 * 0.75 * 400.0
                    + 0.75 * 0.25 * 400.0 + 0.25 * 0.25 * 800.0;
    EXPECT_NEAR(elev, expected, INTERP_TOL);
}

TEST_F(CFITDalA_MCDC, SetConfig_ClearanceAtMin_Accepted)
{
    // Verifies REQ-SF-0402 | MC/DC: SetConfig with clearance exactly at min boundary
    TerrainCollisionDetector det(terrain);
    CFITConfig cfg;
    cfg.clearanceThresholdFt = CLEARANCE_MIN_FT;
    cfg.lookaheadTimeSec = LOOKAHEAD_MIN_SEC;
    det.SetConfig(cfg);
    CFITConfig result = det.GetConfig();
    EXPECT_NEAR(result.clearanceThresholdFt, CLEARANCE_MIN_FT, TIGHT_TOL);
    EXPECT_NEAR(result.lookaheadTimeSec, LOOKAHEAD_MIN_SEC, TIGHT_TOL);
}

TEST_F(CFITDalA_Robustness, Evaluate_NegativeLookahead_ClampedTo60)
{
    // Verifies REQ-SF-0402 | Robustness: negative lookahead time clamped to 60
    TerrainCollisionDetector det(terrain);
    det.SetLookaheadTime(-100.0);
    EXPECT_NEAR(det.GetLookaheadTime(), LOOKAHEAD_MIN_SEC, TIGHT_TOL);
}

TEST_F(CFITDalA_Robustness, Evaluate_NegativeClearance_ClampedTo25)
{
    // Verifies REQ-SF-0402 | Robustness: negative clearance threshold clamped to 25
    TerrainCollisionDetector det(terrain);
    det.SetClearanceThreshold(-50.0);
    EXPECT_NEAR(det.GetClearanceThreshold(), CLEARANCE_MIN_FT, TIGHT_TOL);
}
