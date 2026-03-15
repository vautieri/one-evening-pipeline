/*
 * TerrainGrid.cpp
 *
 * Implementation of the SynthFlight regular-grid terrain model.
 * See TerrainGrid.h for interface documentation.
 *
 * Implements REQ-SF-0406 (Elevation Query API) and partial REQ-SF-0400
 * (terrain data model).
 *
 * Coding standard: SCS-SF-2026-001 (C++17, no raw new/delete)
 */

#include "TerrainGrid.h"

namespace SynthFlight {

// =============================================================================
//  Construction
// =============================================================================

TerrainGrid::TerrainGrid(size_t rows, size_t cols, double postingMeters,
                         double originLat, double originLon)
    : rows_(rows),
      cols_(cols),
      postingMeters_(postingMeters),
      originLat_(originLat),
      originLon_(originLon),
      metersPerDegreeLon_(0.0),
      elevations_(rows * cols, 0.0)
{
    if (rows < kMinGridDimension) {
        throw std::invalid_argument(
            "TerrainGrid: rows must be >= " + std::to_string(kMinGridDimension));
    }
    if (cols < kMinGridDimension) {
        throw std::invalid_argument(
            "TerrainGrid: cols must be >= " + std::to_string(kMinGridDimension));
    }
    if (postingMeters <= 0.0) {
        throw std::invalid_argument(
            "TerrainGrid: postingMeters must be > 0");
    }

    // Pre-compute longitude scale factor at grid origin
    const double originLatRad = originLat_ * kDegToRad;
    metersPerDegreeLon_ = kMetersPerDegreeLat * std::cos(originLatRad);
}

// =============================================================================
//  Cell-level elevation access
// =============================================================================

void TerrainGrid::SetElevation(size_t row, size_t col, double elevation)
{
    ValidateIndices(row, col);
    elevations_[Index(row, col)] = elevation;
}

double TerrainGrid::GetElevationAtCell(size_t row, size_t col) const
{
    ValidateIndices(row, col);
    return elevations_[Index(row, col)];
}

// =============================================================================
//  Grid fill utilities
// =============================================================================

void TerrainGrid::FillFlat(double elevation)
{
    std::fill(elevations_.begin(), elevations_.end(), elevation);
}

void TerrainGrid::FillSlope(double baseElevation,
                            double slopeNS_degrees, double slopeEW_degrees)
{
    // Convert slope angles to rise per posting distance
    const double risePerPostNS = postingMeters_ * std::tan(slopeNS_degrees * kDegToRad);
    const double risePerPostEW = postingMeters_ * std::tan(slopeEW_degrees * kDegToRad);

    for (size_t r = 0; r < rows_; ++r) {
        for (size_t c = 0; c < cols_; ++c) {
            elevations_[Index(r, c)] = baseElevation
                + static_cast<double>(r) * risePerPostNS
                + static_cast<double>(c) * risePerPostEW;
        }
    }
}

// =============================================================================
//  Coordinate conversion helpers
// =============================================================================

void TerrainGrid::LatLonToGrid(double lat, double lon,
                               double& row, double& col) const
{
    // Equirectangular projection: convert delta-degrees to meters,
    // then meters to grid posts.
    const double deltaLatDeg = lat - originLat_;
    const double deltaLonDeg = lon - originLon_;

    const double deltaNorthMeters = deltaLatDeg * kMetersPerDegreeLat;
    const double deltaEastMeters  = deltaLonDeg * metersPerDegreeLon_;

    row = deltaNorthMeters / postingMeters_;
    col = deltaEastMeters  / postingMeters_;
}

void TerrainGrid::GridToLatLon(double row, double col,
                               double& lat, double& lon) const
{
    const double deltaNorthMeters = row * postingMeters_;
    const double deltaEastMeters  = col * postingMeters_;

    lat = originLat_ + deltaNorthMeters / kMetersPerDegreeLat;
    lon = originLon_ + deltaEastMeters  / metersPerDegreeLon_;
}

// =============================================================================
//  Elevation query  (REQ-SF-0406)
// =============================================================================

double TerrainGrid::GetElevation(double lat, double lon) const
{
    // Step 1: Convert lat/lon to fractional grid coordinates
    double rowF = 0.0;
    double colF = 0.0;
    LatLonToGrid(lat, lon, rowF, colF);

    // Step 2: Clamp to valid grid extent [0, rows_-1] x [0, cols_-1]
    const double maxRow = static_cast<double>(rows_ - 1);
    const double maxCol = static_cast<double>(cols_ - 1);

    rowF = std::clamp(rowF, 0.0, maxRow);
    colF = std::clamp(colF, 0.0, maxCol);

    // Step 3: Integer indices of the four surrounding posts
    size_t r0 = static_cast<size_t>(rowF);
    size_t c0 = static_cast<size_t>(colF);

    // Guard against landing exactly on the upper boundary
    if (r0 >= rows_ - 1) r0 = rows_ - 2;
    if (c0 >= cols_ - 1) c0 = cols_ - 2;

    const size_t r1 = r0 + 1;
    const size_t c1 = c0 + 1;

    // Step 4: Fractional offsets within the cell
    const double fy = rowF - static_cast<double>(r0);
    const double fx = colF - static_cast<double>(c0);

    // Step 5: Fetch the four corner elevations
    const double q00 = elevations_[Index(r0, c0)];
    const double q10 = elevations_[Index(r0, c1)];
    const double q01 = elevations_[Index(r1, c0)];
    const double q11 = elevations_[Index(r1, c1)];

    // Step 6: Bilinear interpolation
    //   value = (1-fx)*(1-fy)*Q00 + fx*(1-fy)*Q10 + (1-fx)*fy*Q01 + fx*fy*Q11
    const double value = (1.0 - fx) * (1.0 - fy) * q00
                       +        fx  * (1.0 - fy) * q10
                       + (1.0 - fx) *        fy  * q01
                       +        fx  *        fy  * q11;

    return value;
}

void TerrainGrid::GetElevationBatch(const double* lats, const double* lons,
                                    double* elevations, size_t count) const
{
    for (size_t i = 0; i < count; ++i) {
        elevations[i] = GetElevation(lats[i], lons[i]);
    }
}

bool TerrainGrid::IsInsideGrid(double lat, double lon) const
{
    double rowF = 0.0;
    double colF = 0.0;
    LatLonToGrid(lat, lon, rowF, colF);

    const double maxRow = static_cast<double>(rows_ - 1);
    const double maxCol = static_cast<double>(cols_ - 1);

    return (rowF >= 0.0 && rowF <= maxRow &&
            colF >= 0.0 && colF <= maxCol);
}

// =============================================================================
//  Statistics
// =============================================================================

double TerrainGrid::GetMinElevation() const
{
    return *std::min_element(elevations_.begin(), elevations_.end());
}

double TerrainGrid::GetMaxElevation() const
{
    return *std::max_element(elevations_.begin(), elevations_.end());
}

double TerrainGrid::GetMeanElevation() const
{
    const double sum = std::accumulate(elevations_.begin(), elevations_.end(), 0.0);
    return sum / static_cast<double>(elevations_.size());
}

// =============================================================================
//  Internal helpers
// =============================================================================

void TerrainGrid::ValidateIndices(size_t row, size_t col) const
{
    if (row >= rows_) {
        throw std::out_of_range(
            "TerrainGrid: row index " + std::to_string(row)
            + " out of range [0, " + std::to_string(rows_ - 1) + "]");
    }
    if (col >= cols_) {
        throw std::out_of_range(
            "TerrainGrid: col index " + std::to_string(col)
            + " out of range [0, " + std::to_string(cols_ - 1) + "]");
    }
}

} // namespace SynthFlight
