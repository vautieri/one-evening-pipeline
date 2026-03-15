/*
 * TerrainGrid.h
 *
 * Regular-grid terrain elevation model for the SynthFlight terrain engine.
 * Implements REQ-SF-0406 (Elevation Query API) and partial REQ-SF-0400
 * (terrain data model).
 *
 * The grid stores elevation data (meters HAE) on a regular posting with
 * configurable spacing (0.5 m to 30 m per REQ-SF-0400).  Grid origin is
 * defined by a WGS-84 latitude/longitude pair.  Coordinate conversion
 * uses an equirectangular projection centered on the grid origin.
 *
 * Coding standard: SCS-SF-2026-001 (C++17, no raw new/delete)
 */

#ifndef SYNTHFLIGHT_TERRAIN_GRID_H
#define SYNTHFLIGHT_TERRAIN_GRID_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace SynthFlight {

// =============================================================================
//  Physical / geodetic constants
// =============================================================================

/// Approximate meters per degree of latitude on WGS-84 ellipsoid
static constexpr double kMetersPerDegreeLat = 111320.0;

/// Pi (C++17 does not guarantee M_PI everywhere)
static constexpr double kPi = 3.14159265358979323846;

/// Degrees-to-radians conversion factor
static constexpr double kDegToRad = kPi / 180.0;

/// Minimum allowed grid posting [m] per REQ-SF-0400
static constexpr double kMinPostingMeters = 0.5;

/// Maximum allowed grid posting [m] per REQ-SF-0400
static constexpr double kMaxPostingMeters = 30.0;

/// Minimum grid dimension (rows or cols) for meaningful interpolation
static constexpr size_t kMinGridDimension = 2;

// =============================================================================
//  TerrainGrid
// =============================================================================

class TerrainGrid {
public:

    // -- Construction ---------------------------------------------------------

    /// Construct a terrain grid.
    /// @param rows          Number of rows (north-south), must be >= 2
    /// @param cols          Number of columns (east-west), must be >= 2
    /// @param postingMeters Grid posting (spacing) in meters, [0.5, 30]
    /// @param originLat     Grid origin latitude  [deg, WGS-84]
    /// @param originLon     Grid origin longitude [deg, WGS-84]
    /// @throws std::invalid_argument if any parameter is out of range
    TerrainGrid(size_t rows, size_t cols, double postingMeters,
                double originLat, double originLon);

    ~TerrainGrid() = default;

    // -- Grid metadata accessors ----------------------------------------------

    /// Number of rows (north-south posts)
    size_t GetRows() const { return rows_; }

    /// Number of columns (east-west posts)
    size_t GetCols() const { return cols_; }

    /// Grid posting (spacing) in meters
    double GetPostingMeters() const { return postingMeters_; }

    /// Grid origin latitude [deg, WGS-84]
    double GetOriginLat() const { return originLat_; }

    /// Grid origin longitude [deg, WGS-84]
    double GetOriginLon() const { return originLon_; }

    // -- Cell-level elevation access ------------------------------------------

    /// Set elevation at a specific grid cell.
    /// @throws std::out_of_range if row or col exceeds grid dimensions
    void SetElevation(size_t row, size_t col, double elevation);

    /// Get raw elevation at a specific grid cell (no interpolation).
    /// @throws std::out_of_range if row or col exceeds grid dimensions
    double GetElevationAtCell(size_t row, size_t col) const;

    // -- Grid fill utilities --------------------------------------------------

    /// Fill the entire grid with a constant elevation value [m HAE].
    void FillFlat(double elevation);

    /// Fill the grid with a planar slope.
    /// @param baseElevation   Elevation at origin cell (0,0) [m HAE]
    /// @param slopeNS_degrees North-south slope angle [deg] (positive = rising northward)
    /// @param slopeEW_degrees East-west slope angle [deg] (positive = rising eastward)
    void FillSlope(double baseElevation, double slopeNS_degrees, double slopeEW_degrees);

    // -- Coordinate conversion helpers ----------------------------------------

    /// Convert WGS-84 lat/lon to fractional grid row/col.
    /// Uses equirectangular projection centered on the grid origin.
    void LatLonToGrid(double lat, double lon, double& row, double& col) const;

    /// Convert fractional grid row/col back to WGS-84 lat/lon.
    void GridToLatLon(double row, double col, double& lat, double& lon) const;

    // -- Elevation query (REQ-SF-0406) ----------------------------------------

    /// Query elevation at an arbitrary WGS-84 position using bilinear
    /// interpolation.  Points outside the grid extent are clamped to the
    /// nearest edge.
    /// Target performance: < 1 microsecond per query.
    double GetElevation(double lat, double lon) const;

    /// Batch elevation query for an array of positions.
    /// @param lats       Array of latitudes  [deg]
    /// @param lons       Array of longitudes [deg]
    /// @param elevations Output array (must be pre-allocated to at least count)
    /// @param count      Number of query points
    void GetElevationBatch(const double* lats, const double* lons,
                           double* elevations, size_t count) const;

    /// Test whether a WGS-84 position falls within the grid extent.
    bool IsInsideGrid(double lat, double lon) const;

    // -- Statistics -----------------------------------------------------------

    /// Minimum elevation across all grid cells [m HAE]
    double GetMinElevation() const;

    /// Maximum elevation across all grid cells [m HAE]
    double GetMaxElevation() const;

    /// Arithmetic mean elevation across all grid cells [m HAE]
    double GetMeanElevation() const;

private:

    // -- Grid parameters ------------------------------------------------------

    size_t rows_;
    size_t cols_;
    double postingMeters_;
    double originLat_;
    double originLon_;

    // -- Pre-computed projection factors --------------------------------------

    /// Meters per degree of longitude at the grid origin latitude
    double metersPerDegreeLon_;

    // -- Elevation storage (row-major order) ----------------------------------

    std::vector<double> elevations_;

    // -- Internal helpers -----------------------------------------------------

    /// Compute linear index from row/col (no bounds check)
    size_t Index(size_t row, size_t col) const { return row * cols_ + col; }

    /// Validate row/col and throw std::out_of_range if invalid
    void ValidateIndices(size_t row, size_t col) const;
};

} // namespace SynthFlight

#endif // SYNTHFLIGHT_TERRAIN_GRID_H
