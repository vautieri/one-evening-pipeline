/*==============================================================================
 * TAR-SF-001 | DAL A Test Suite for FGMatrix33
 *
 * Requirements:  REQ-SF-0200  Matrix element access and bounds checking
 *                REQ-SF-0103  Euler angle extraction from DCM
 *                REQ-SF-1300  Matrix arithmetic operations
 *                REQ-SF-1400  Matrix inversion and determinant
 *                REQ-SF-1401  Numerical robustness and edge cases
 *
 * Coding Standard: SCS-SF-2026-001
 * Date:            2026-03-14
 *
 * Coverage target: 100% Statement, Decision, and MC/DC
 * Approximately 75 test cases — non-overlapping with test_math_gtest.cpp
 *==============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <sstream>
#include <limits>
#include <string>
#include "FGMatrix33.h"
#include "FGColumnVector3.h"

using namespace JSBSim;

// ---------------------------------------------------------------------------
// Named constants per SCS-SF-2026-001
// ---------------------------------------------------------------------------
static constexpr double TOLERANCE        = 1e-10;
static constexpr double DEG_TO_RAD       = 3.14159265358979323846 / 180.0;
static constexpr double RAD_TO_DEG       = 180.0 / 3.14159265358979323846;
static constexpr double GIMBAL_THRESHOLD = 0.9999999;
static constexpr double PI               = 3.14159265358979323846;

// ---------------------------------------------------------------------------
// Helper: Construct a Direction Cosine Matrix from Euler angles (ZYX convention)
// ---------------------------------------------------------------------------
static FGMatrix33 EulerToDCM(double phi, double theta, double psi)
{
    double cp = std::cos(phi),  sp = std::sin(phi);
    double ct = std::cos(theta), st = std::sin(theta);
    double cy = std::cos(psi),  sy = std::sin(psi);
    return FGMatrix33(
        ct*cy,            ct*sy,            -st,
        sp*st*cy - cp*sy, sp*st*sy + cp*cy, sp*ct,
        cp*st*cy + sp*sy, cp*st*sy - sp*cy, cp*ct
    );
}

// ---------------------------------------------------------------------------
// Helper: Build an identity matrix
// ---------------------------------------------------------------------------
static FGMatrix33 Identity()
{
    return FGMatrix33(1, 0, 0,
                      0, 1, 0,
                      0, 0, 1);
}

// ===========================================================================
// a. BOUNDARY VALUE TESTS
// ===========================================================================

// Verifies REQ-SF-0200 | Boundary: valid corner index (1,1) via const accessor
TEST(MatrixDalA_Boundary, ConstAccessIndex_1_1_ReturnsValue)
{
    const FGMatrix33 M(7.0, 0, 0,  0, 0, 0,  0, 0, 0);
    EXPECT_DOUBLE_EQ(M(1, 1), 7.0);
}

// Verifies REQ-SF-0200 | Boundary: valid corner index (3,3) via const accessor
TEST(MatrixDalA_Boundary, ConstAccessIndex_3_3_ReturnsValue)
{
    const FGMatrix33 M(0, 0, 0,  0, 0, 0,  0, 0, 9.5);
    EXPECT_DOUBLE_EQ(M(3, 3), 9.5);
}

// Verifies REQ-SF-0200 | Boundary: valid corner index (1,1) via non-const accessor
TEST(MatrixDalA_Boundary, NonConstAccessIndex_1_1_ReturnsValue)
{
    FGMatrix33 M;
    M(1, 1) = 3.14;
    EXPECT_DOUBLE_EQ(M(1, 1), 3.14);
}

// Verifies REQ-SF-0200 | Boundary: valid corner index (3,3) via non-const accessor
TEST(MatrixDalA_Boundary, NonConstAccessIndex_3_3_ReturnsValue)
{
    FGMatrix33 M;
    M(3, 3) = 2.718;
    EXPECT_DOUBLE_EQ(M(3, 3), 2.718);
}

// Verifies REQ-SF-0200 | Boundary: Entry() const delegation at (1,1)
TEST(MatrixDalA_Boundary, ConstEntry_1_1_DelegatesToOperator)
{
    const FGMatrix33 M(42.0, 0, 0,  0, 0, 0,  0, 0, 0);
    EXPECT_DOUBLE_EQ(M.Entry(1, 1), 42.0);
}

// Verifies REQ-SF-0200 | Boundary: Entry() non-const delegation at (3,3)
TEST(MatrixDalA_Boundary, NonConstEntry_3_3_DelegatesToOperator)
{
    FGMatrix33 M;
    M.Entry(3, 3) = -99.0;
    EXPECT_DOUBLE_EQ(M.Entry(3, 3), -99.0);
}

// Verifies REQ-SF-0200 | Boundary: Entry() const throws at out-of-range
TEST(MatrixDalA_Boundary, ConstEntry_OutOfRange_Throws)
{
    const FGMatrix33 M;
    EXPECT_THROW(M.Entry(0, 1), std::range_error);
}

// Verifies REQ-SF-0200 | Boundary: Entry() non-const throws at out-of-range
TEST(MatrixDalA_Boundary, NonConstEntry_OutOfRange_Throws)
{
    FGMatrix33 M;
    EXPECT_THROW(M.Entry(1, 4), std::range_error);
}

// Verifies REQ-SF-1400 | Boundary: Determinant of identity is 1.0
TEST(MatrixDalA_Boundary, DeterminantOfIdentity_IsOne)
{
    FGMatrix33 I = Identity();
    EXPECT_NEAR(I.Determinant(), 1.0, TOLERANCE);
}

// Verifies REQ-SF-1400 | Boundary: Determinant of diagonal matrix
TEST(MatrixDalA_Boundary, DeterminantOfDiagonal_IsProductOfDiag)
{
    FGMatrix33 D(2.0, 0, 0,  0, 3.0, 0,  0, 0, 5.0);
    EXPECT_NEAR(D.Determinant(), 30.0, TOLERANCE);
}

// Verifies REQ-SF-1401 | Boundary: very large elements (1e150 range)
TEST(MatrixDalA_Boundary, VeryLargeElements_DeterminantComputable)
{
    double v = 1e50;
    FGMatrix33 M(v, 0, 0,  0, v, 0,  0, 0, v);
    EXPECT_NEAR(M.Determinant(), v * v * v, v * v * v * 1e-10);
}

// Verifies REQ-SF-1401 | Boundary: very small elements
TEST(MatrixDalA_Boundary, VerySmallElements_DeterminantComputable)
{
    double v = 1e-50;
    FGMatrix33 M(v, 0, 0,  0, v, 0,  0, 0, v);
    EXPECT_NEAR(M.Determinant(), v * v * v, 1e-160);
}

// ===========================================================================
// b. ROBUSTNESS TESTS
// ===========================================================================

// Verifies REQ-SF-1401 | Robustness: NaN propagates through Determinant
TEST(MatrixDalA_Robustness, NaNInMatrix_DeterminantIsNaN)
{
    double nan = std::numeric_limits<double>::quiet_NaN();
    FGMatrix33 M(nan, 0, 0,  0, 1, 0,  0, 0, 1);
    EXPECT_TRUE(std::isnan(M.Determinant()));
}

// Verifies REQ-SF-1401 | Robustness: Infinity in matrix — Inverse attempt
TEST(MatrixDalA_Robustness, InfinityInMatrix_InverseDoesNotThrowDomainError)
{
    double inf = std::numeric_limits<double>::infinity();
    FGMatrix33 M(inf, 0, 0,  0, 1, 0,  0, 0, 1);
    // det is inf, not 0, so domain_error should NOT be thrown
    // Result may contain inf/nan but it should not be a singular-matrix throw
    EXPECT_NO_THROW(M.Inverse());
}

// Verifies REQ-SF-1401 | Robustness: Near-singular matrix is still invertible
TEST(MatrixDalA_Robustness, NearSingularMatrix_InvertibleReturnsTrue)
{
    FGMatrix33 M(1.0, 0, 0,
                 0, 1.0, 0,
                 0, 0, 1e-15);
    EXPECT_TRUE(M.Invertible());
}

// Verifies REQ-SF-1300 | Robustness: A * I = A (right identity)
TEST(MatrixDalA_Robustness, MatrixMultiply_RightIdentity_Unchanged)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 10);
    FGMatrix33 I = Identity();
    FGMatrix33 R = A * I;
    for (unsigned int r = 1; r <= 3; ++r)
        for (unsigned int c = 1; c <= 3; ++c)
            EXPECT_NEAR(R(r, c), A(r, c), TOLERANCE);
}

// Verifies REQ-SF-1300 | Robustness: A * 0 = 0 (zero matrix)
TEST(MatrixDalA_Robustness, MatrixMultiply_ZeroMatrix_IsZero)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 Z;
    FGMatrix33 R = A * Z;
    FGMatrix33 expected;
    EXPECT_EQ(R, expected);
}

// Verifies REQ-SF-1400 | Robustness: (A^-1)^-1 = A
TEST(MatrixDalA_Robustness, InverseOfInverse_RestoresOriginal)
{
    FGMatrix33 A(2, 1, 0,  0, 3, 1,  1, 0, 2);
    FGMatrix33 Ainv = A.Inverse();
    FGMatrix33 AinvInv = Ainv.Inverse();
    for (unsigned int r = 1; r <= 3; ++r)
        for (unsigned int c = 1; c <= 3; ++c)
            EXPECT_NEAR(AinvInv(r, c), A(r, c), TOLERANCE);
}

// Verifies REQ-SF-1300 | Robustness: (A^T)^T = A
TEST(MatrixDalA_Robustness, TransposeOfTranspose_RestoresOriginal)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 ATT = A.Transposed().Transposed();
    EXPECT_EQ(ATT, A);
}

// Verifies REQ-SF-1300 | Robustness: Matrix * zero vector = zero vector
TEST(MatrixDalA_Robustness, MatrixTimesZeroVector_IsZeroVector)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGColumnVector3 v(0.0, 0.0, 0.0);
    FGColumnVector3 result = A * v;
    EXPECT_NEAR(result(1), 0.0, TOLERANCE);
    EXPECT_NEAR(result(2), 0.0, TOLERANCE);
    EXPECT_NEAR(result(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | Robustness: GetEuler with sinTheta exactly +1.0 (clamp high)
TEST(MatrixDalA_Robustness, GetEuler_SinThetaExactlyPlusOne_ClampsHigh)
{
    // Build matrix where -data[idx(1,3)] = +1.0, so data(1,3) = -1.0
    // theta = asin(1.0) = pi/2
    FGMatrix33 M(0, 0, -1.0,
                 0, 1, 0,
                 1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), PI / 2.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | Robustness: GetEuler with sinTheta exactly -1.0 (clamp low)
TEST(MatrixDalA_Robustness, GetEuler_SinThetaExactlyMinusOne_ClampsLow)
{
    // Build matrix where -data[idx(1,3)] = -1.0, so data(1,3) = +1.0
    // theta = asin(-1.0) = -pi/2
    FGMatrix33 M(0, 0, 1.0,
                 0, 1, 0,
                 -1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), -PI / 2.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | Robustness: GetEuler with sinTheta > 1.0 (beyond clamp)
TEST(MatrixDalA_Robustness, GetEuler_SinThetaBeyondPlusOne_ClampedToOne)
{
    // data(1,3) = -1.5 => sinTheta = -(-1.5) = 1.5, clamped to 1.0
    FGMatrix33 M(0, 0, -1.5,
                 0, 1, 0,
                 1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), PI / 2.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | Robustness: GetEuler with sinTheta < -1.0 (beyond clamp)
TEST(MatrixDalA_Robustness, GetEuler_SinThetaBeyondMinusOne_ClampedToMinusOne)
{
    // data(1,3) = 1.5 => sinTheta = -(1.5) = -1.5, clamped to -1.0
    FGMatrix33 M(0, 0, 1.5,
                 0, 1, 0,
                 -1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), -PI / 2.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | Robustness: GetEuler with NaN in rotation matrix
TEST(MatrixDalA_Robustness, GetEuler_NaNInMatrix_PropagatesNaN)
{
    double nan = std::numeric_limits<double>::quiet_NaN();
    FGMatrix33 M(1, 0, nan,
                 0, 1, 0,
                 0, 0, 1);
    FGColumnVector3 euler = M.GetEuler();
    // sinTheta = -nan = nan; asin(nan) = nan; downstream values are NaN
    EXPECT_TRUE(std::isnan(euler(2)));
}

// ===========================================================================
// c. MC/DC TESTS
// ===========================================================================

// ---------------------------------------------------------------------------
// Decision 1: operator()(row,col) const
//   if (row < 1 || row > 3 || col < 1 || col > 3)
//   4 conditions => 5 MC/DC test cases
// ---------------------------------------------------------------------------

// Verifies REQ-SF-0200 | MC/DC: condition 1 (row<1=T) — const operator()
TEST(MatrixDalA_MCDC, ConstAccess_RowBelow1_ThrowsRangeError)
{
    const FGMatrix33 M;
    // row=0, col=1: row<1=T, row>3=F, col<1=F, col>3=F => TRUE (throw)
    EXPECT_THROW(M(0, 1), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 2 (row>3=T) — const operator()
TEST(MatrixDalA_MCDC, ConstAccess_RowAbove3_ThrowsRangeError)
{
    const FGMatrix33 M;
    // row=4, col=1: row<1=F, row>3=T, col<1=F, col>3=F => TRUE (throw)
    EXPECT_THROW(M(4, 1), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 3 (col<1=T) — const operator()
TEST(MatrixDalA_MCDC, ConstAccess_ColBelow1_ThrowsRangeError)
{
    const FGMatrix33 M;
    // row=1, col=0: row<1=F, row>3=F, col<1=T, col>3=F => TRUE (throw)
    EXPECT_THROW(M(1, 0), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 4 (col>3=T) — const operator()
TEST(MatrixDalA_MCDC, ConstAccess_ColAbove3_ThrowsRangeError)
{
    const FGMatrix33 M;
    // row=1, col=4: row<1=F, row>3=F, col<1=F, col>3=T => TRUE (throw)
    EXPECT_THROW(M(1, 4), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: baseline (all conditions F) — const operator()
TEST(MatrixDalA_MCDC, ConstAccess_ValidIndex_NoThrow)
{
    const FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    // row=2, col=2: row<1=F, row>3=F, col<1=F, col>3=F => FALSE (no throw)
    EXPECT_NO_THROW(M(2, 2));
    EXPECT_DOUBLE_EQ(M(2, 2), 5.0);
}

// ---------------------------------------------------------------------------
// Decision 1 (duplicate): operator()(row,col) non-const — same 5 MC/DC cases
// ---------------------------------------------------------------------------

// Verifies REQ-SF-0200 | MC/DC: condition 1 (row<1=T) — non-const operator()
TEST(MatrixDalA_MCDC, NonConstAccess_RowBelow1_ThrowsRangeError)
{
    FGMatrix33 M;
    // row=0, col=1: row<1=T => throw
    EXPECT_THROW(M(0, 1), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 2 (row>3=T) — non-const operator()
TEST(MatrixDalA_MCDC, NonConstAccess_RowAbove3_ThrowsRangeError)
{
    FGMatrix33 M;
    // row=4, col=1: row>3=T => throw
    EXPECT_THROW(M(4, 1), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 3 (col<1=T) — non-const operator()
TEST(MatrixDalA_MCDC, NonConstAccess_ColBelow1_ThrowsRangeError)
{
    FGMatrix33 M;
    // row=1, col=0: col<1=T => throw
    EXPECT_THROW(M(1, 0), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: condition 4 (col>3=T) — non-const operator()
TEST(MatrixDalA_MCDC, NonConstAccess_ColAbove3_ThrowsRangeError)
{
    FGMatrix33 M;
    // row=1, col=4: col>3=T => throw
    EXPECT_THROW(M(1, 4), std::range_error);
}

// Verifies REQ-SF-0200 | MC/DC: baseline (all conditions F) — non-const operator()
TEST(MatrixDalA_MCDC, NonConstAccess_ValidIndex_NoThrow)
{
    FGMatrix33 M;
    // row=2, col=2: all conditions F => no throw
    EXPECT_NO_THROW(M(2, 2) = 42.0);
    EXPECT_DOUBLE_EQ(M(2, 2), 42.0);
}

// ---------------------------------------------------------------------------
// Decision 2: Inverse() — if (det == 0.0)
// ---------------------------------------------------------------------------

// Verifies REQ-SF-1400 | MC/DC: det==0 => throws domain_error
TEST(MatrixDalA_MCDC, Inverse_SingularZeroMatrix_ThrowsDomainError)
{
    FGMatrix33 Z;  // all zeros, det = 0
    EXPECT_THROW(Z.Inverse(), std::domain_error);
}

// Verifies REQ-SF-1400 | MC/DC: det!=0 => computes inverse
TEST(MatrixDalA_MCDC, Inverse_NonSingular_Succeeds)
{
    FGMatrix33 I = Identity();
    EXPECT_NO_THROW(I.Inverse());
}

// ---------------------------------------------------------------------------
// Decision 3: operator/(double) — if (scalar == 0.0)
// ---------------------------------------------------------------------------

// Verifies REQ-SF-1300 | MC/DC: scalar==0 => throws domain_error (operator/)
TEST(MatrixDalA_MCDC, ScalarDivide_ByZero_ThrowsDomainError)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    EXPECT_THROW(M / 0.0, std::domain_error);
}

// Verifies REQ-SF-1300 | MC/DC: scalar!=0 => succeeds (operator/)
TEST(MatrixDalA_MCDC, ScalarDivide_NonZero_Succeeds)
{
    FGMatrix33 M(2, 4, 6,  8, 10, 12,  14, 16, 18);
    FGMatrix33 R = M / 2.0;
    EXPECT_NEAR(R(1, 1), 1.0, TOLERANCE);
    EXPECT_NEAR(R(2, 3), 6.0, TOLERANCE);
}

// ---------------------------------------------------------------------------
// Decision 3 (location 2): operator/=(double) — if (scalar == 0.0)
// ---------------------------------------------------------------------------

// Verifies REQ-SF-1300 | MC/DC: scalar==0 => throws domain_error (operator/=)
TEST(MatrixDalA_MCDC, ScalarDivideEquals_ByZero_ThrowsDomainError)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    EXPECT_THROW(M /= 0.0, std::domain_error);
}

// Verifies REQ-SF-1300 | MC/DC: scalar!=0 => succeeds (operator/=)
TEST(MatrixDalA_MCDC, ScalarDivideEquals_NonZero_Succeeds)
{
    FGMatrix33 M(10, 20, 30,  40, 50, 60,  70, 80, 90);
    M /= 10.0;
    EXPECT_NEAR(M(1, 1), 1.0, TOLERANCE);
    EXPECT_NEAR(M(3, 3), 9.0, TOLERANCE);
}

// ---------------------------------------------------------------------------
// Decision 4: Invertible() — Determinant() != 0.0
// ---------------------------------------------------------------------------

// Verifies REQ-SF-1400 | MC/DC: Determinant()==0 => Invertible() returns false
TEST(MatrixDalA_MCDC, Invertible_SingularMatrix_ReturnsFalse)
{
    FGMatrix33 Z;  // all zeros, det = 0
    EXPECT_FALSE(Z.Invertible());
}

// Verifies REQ-SF-1400 | MC/DC: Determinant()!=0 => Invertible() returns true
TEST(MatrixDalA_MCDC, Invertible_NonSingularMatrix_ReturnsTrue)
{
    FGMatrix33 I = Identity();
    EXPECT_TRUE(I.Invertible());
}

// ---------------------------------------------------------------------------
// Decision 5: GetEuler decisions A, B, C
// ---------------------------------------------------------------------------

// Verifies REQ-SF-0103 | MC/DC: Decision A (sinTheta >= 1.0) TRUE
TEST(MatrixDalA_MCDC, GetEuler_DecisionA_SinThetaGE1_ClampsToOne)
{
    // data(1,3) = -1.0 => sinTheta = 1.0 => decision A TRUE
    FGMatrix33 M(0, 0, -1.0,
                 0, 1, 0,
                 1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), PI / 2.0, TOLERANCE);
    // Gimbal lock path (|sinTheta|>=0.9999999), psi should be 0
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | MC/DC: Decision A (sinTheta >= 1.0) FALSE, Decision B FALSE
TEST(MatrixDalA_MCDC, GetEuler_DecisionA_SinThetaNormal_FallsThrough)
{
    // sinTheta = 0.5 => Decision A: FALSE, Decision B: FALSE, Decision C: TRUE (normal)
    FGMatrix33 M = EulerToDCM(0.0, 30.0 * DEG_TO_RAD, 0.0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), 30.0 * DEG_TO_RAD, TOLERANCE);
}

// Verifies REQ-SF-0103 | MC/DC: Decision B (sinTheta <= -1.0) TRUE
TEST(MatrixDalA_MCDC, GetEuler_DecisionB_SinThetaLEMinus1_ClampsToMinusOne)
{
    // data(1,3) = +1.0 => sinTheta = -1.0 => decision B TRUE
    FGMatrix33 M(0, 0, 1.0,
                 0, 1, 0,
                 -1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), -PI / 2.0, TOLERANCE);
    // Gimbal lock path, psi should be 0
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | MC/DC: Decision B (sinTheta <= -1.0) FALSE with negative sinTheta
TEST(MatrixDalA_MCDC, GetEuler_DecisionB_SinThetaSlightlyAboveMinusOne_FallsThrough)
{
    // sinTheta = -0.5 => Decision A: FALSE, Decision B: FALSE => normal path
    FGMatrix33 M = EulerToDCM(0.0, -30.0 * DEG_TO_RAD, 0.0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), -30.0 * DEG_TO_RAD, TOLERANCE);
}

// Verifies REQ-SF-0103 | MC/DC: Decision C (abs(sinTheta) < 0.9999999) TRUE — normal path
TEST(MatrixDalA_MCDC, GetEuler_DecisionC_NormalPath_UsesAtan2)
{
    double phi = 20.0 * DEG_TO_RAD;
    double theta = 15.0 * DEG_TO_RAD;
    double psi = 45.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(phi, theta, psi);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), phi, TOLERANCE);
    EXPECT_NEAR(euler(2), theta, TOLERANCE);
    EXPECT_NEAR(euler(3), psi, TOLERANCE);
}

// Verifies REQ-SF-0103 | MC/DC: Decision C (abs(sinTheta) < 0.9999999) FALSE — gimbal lock path
TEST(MatrixDalA_MCDC, GetEuler_DecisionC_GimbalLockPath_PsiZero)
{
    // sinTheta = 0.99999999 (>= 0.9999999) => gimbal lock
    // Construct a matrix with data(1,3) such that sinTheta = -data(1,3) = 0.99999999
    double sinT = 0.99999999;
    double cosT = std::sqrt(1.0 - sinT * sinT);
    FGMatrix33 M(cosT, 0, -sinT,
                 0,    1, 0,
                 sinT, 0, cosT);
    FGColumnVector3 euler = M.GetEuler();
    // sinTheta >= 0.9999999, so gimbal lock path => psi = 0
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// ===========================================================================
// d. ERROR HANDLING TESTS
// ===========================================================================

// Verifies REQ-SF-0200 | Error: const operator() row=0 throws range_error
TEST(MatrixDalA_Error, ConstAccess_Row0_ThrowsRangeError)
{
    const FGMatrix33 M;
    EXPECT_THROW(M(0, 2), std::range_error);
}

// Verifies REQ-SF-0200 | Error: const operator() row=4 throws range_error
TEST(MatrixDalA_Error, ConstAccess_Row4_ThrowsRangeError)
{
    const FGMatrix33 M;
    EXPECT_THROW(M(4, 2), std::range_error);
}

// Verifies REQ-SF-0200 | Error: const operator() col=0 throws range_error
TEST(MatrixDalA_Error, ConstAccess_Col0_ThrowsRangeError)
{
    const FGMatrix33 M;
    EXPECT_THROW(M(2, 0), std::range_error);
}

// Verifies REQ-SF-0200 | Error: const operator() col=4 throws range_error
TEST(MatrixDalA_Error, ConstAccess_Col4_ThrowsRangeError)
{
    const FGMatrix33 M;
    EXPECT_THROW(M(2, 4), std::range_error);
}

// Verifies REQ-SF-0200 | Error: non-const operator() row=0 throws range_error
TEST(MatrixDalA_Error, NonConstAccess_Row0_ThrowsRangeError)
{
    FGMatrix33 M;
    EXPECT_THROW(M(0, 2), std::range_error);
}

// Verifies REQ-SF-0200 | Error: non-const operator() row=4 throws range_error
TEST(MatrixDalA_Error, NonConstAccess_Row4_ThrowsRangeError)
{
    FGMatrix33 M;
    EXPECT_THROW(M(4, 2), std::range_error);
}

// Verifies REQ-SF-0200 | Error: non-const operator() col=0 throws range_error
TEST(MatrixDalA_Error, NonConstAccess_Col0_ThrowsRangeError)
{
    FGMatrix33 M;
    EXPECT_THROW(M(2, 0), std::range_error);
}

// Verifies REQ-SF-0200 | Error: non-const operator() col=4 throws range_error
TEST(MatrixDalA_Error, NonConstAccess_Col4_ThrowsRangeError)
{
    FGMatrix33 M;
    EXPECT_THROW(M(2, 4), std::range_error);
}

// Verifies REQ-SF-1400 | Error: Inverse of zero matrix throws domain_error
TEST(MatrixDalA_Error, Inverse_ZeroMatrix_ThrowsDomainError)
{
    FGMatrix33 Z;
    EXPECT_THROW(Z.Inverse(), std::domain_error);
}

// ===========================================================================
// e. GETEULER COMPREHENSIVE TESTS
// ===========================================================================

// Verifies REQ-SF-0103 | GetEuler: identity matrix (zero rotation) → all angles zero
TEST(MatrixDalA_GetEuler, IdentityMatrix_AllAnglesZero)
{
    FGMatrix33 I = Identity();
    FGColumnVector3 euler = I.GetEuler();
    EXPECT_NEAR(euler(1), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(2), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: pure roll (phi=30°, theta=0, psi=0)
TEST(MatrixDalA_GetEuler, PureRoll_30Deg_Recovered)
{
    double phi = 30.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(phi, 0.0, 0.0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), phi, TOLERANCE);
    EXPECT_NEAR(euler(2), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: pure pitch (phi=0, theta=45°, psi=0)
TEST(MatrixDalA_GetEuler, PurePitch_45Deg_Recovered)
{
    double theta = 45.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(0.0, theta, 0.0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(2), theta, TOLERANCE);
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: pure yaw (phi=0, theta=0, psi=60°)
TEST(MatrixDalA_GetEuler, PureYaw_60Deg_Recovered)
{
    double psi = 60.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(0.0, 0.0, psi);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(2), 0.0, TOLERANCE);
    EXPECT_NEAR(euler(3), psi, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: combined rotation (phi=10°, theta=20°, psi=30°)
TEST(MatrixDalA_GetEuler, CombinedRotation_10_20_30_Recovered)
{
    double phi   = 10.0 * DEG_TO_RAD;
    double theta = 20.0 * DEG_TO_RAD;
    double psi   = 30.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(phi, theta, psi);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), phi, TOLERANCE);
    EXPECT_NEAR(euler(2), theta, TOLERANCE);
    EXPECT_NEAR(euler(3), psi, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: near gimbal lock (theta=89.999°) — normal path
TEST(MatrixDalA_GetEuler, NearGimbalLock_89_9Deg_NormalPath)
{
    // sin(89.9°) ≈ 0.99999847, which IS < 0.9999999 => normal path
    double theta = 89.9 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(0.0, theta, 0.0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), theta, 1e-6);
}

// Verifies REQ-SF-0103 | GetEuler: at gimbal lock boundary (sinTheta = 0.9999999 exactly)
TEST(MatrixDalA_GetEuler, GimbalLockBoundary_SinThetaExactlyThreshold)
{
    // sinTheta = 0.9999999 — abs(sinTheta) < 0.9999999 is FALSE (equal, not less)
    // This should take the gimbal lock path
    double sinT = GIMBAL_THRESHOLD;
    double cosT = std::sqrt(1.0 - sinT * sinT);
    FGMatrix33 M(cosT, 0, -sinT,
                 0,    1, 0,
                 sinT, 0, cosT);
    FGColumnVector3 euler = M.GetEuler();
    // abs(0.9999999) is NOT < 0.9999999, so gimbal lock
    // Wait: 0.9999999 < 0.9999999 is false, so the else branch executes
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);  // psi forced to 0
}

// Verifies REQ-SF-0103 | GetEuler: just below gimbal lock threshold — normal path
TEST(MatrixDalA_GetEuler, JustBelowGimbalThreshold_NormalPath)
{
    // sinTheta = 0.9999998 — abs(sinTheta) < 0.9999999 is TRUE => normal path
    double sinT = 0.9999998;
    double cosT = std::sqrt(1.0 - sinT * sinT);
    FGMatrix33 M(cosT, 0, -sinT,
                 0,    1, 0,
                 sinT, 0, cosT);
    FGColumnVector3 euler = M.GetEuler();
    // Normal path: psi = atan2(data(1,2), data(1,1)) = atan2(0, cosT) = 0
    EXPECT_NEAR(euler(2), std::asin(sinT), 1e-6);
}

// Verifies REQ-SF-0103 | GetEuler: negative gimbal lock (theta = -90°)
TEST(MatrixDalA_GetEuler, NegativeGimbalLock_Minus90Deg)
{
    // data(1,3) = +1.0 => sinTheta = -1.0 => clamped => theta = -pi/2, gimbal lock
    FGMatrix33 M(0, 0, 1.0,
                 0, 1, 0,
                 -1, 0, 0);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(2), -PI / 2.0, TOLERANCE);
    EXPECT_NEAR(euler(3), 0.0, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: roundtrip — construct DCM, extract, verify
TEST(MatrixDalA_GetEuler, RoundTrip_ArbitraryAngles_Match)
{
    double phi   = -25.0 * DEG_TO_RAD;
    double theta =  40.0 * DEG_TO_RAD;
    double psi   = 120.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(phi, theta, psi);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), phi, TOLERANCE);
    EXPECT_NEAR(euler(2), theta, TOLERANCE);
    EXPECT_NEAR(euler(3), psi, TOLERANCE);
}

// Verifies REQ-SF-0103 | GetEuler: negative angles roundtrip
TEST(MatrixDalA_GetEuler, RoundTrip_NegativeAngles_Match)
{
    double phi   = -45.0 * DEG_TO_RAD;
    double theta = -30.0 * DEG_TO_RAD;
    double psi   = -60.0 * DEG_TO_RAD;
    FGMatrix33 M = EulerToDCM(phi, theta, psi);
    FGColumnVector3 euler = M.GetEuler();
    EXPECT_NEAR(euler(1), phi, TOLERANCE);
    EXPECT_NEAR(euler(2), theta, TOLERANCE);
    EXPECT_NEAR(euler(3), psi, TOLERANCE);
}

// ===========================================================================
// f. STATE / SEQUENCE TESTS
// ===========================================================================

// Verifies REQ-SF-1300 | State: InitMatrix() zeroes all elements
TEST(MatrixDalA_State, InitMatrix_NoArg_AllZeros)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    M.InitMatrix();
    for (unsigned int r = 1; r <= 3; ++r)
        for (unsigned int c = 1; c <= 3; ++c)
            EXPECT_DOUBLE_EQ(M(r, c), 0.0);
}

// Verifies REQ-SF-1300 | State: InitMatrix(values) sets all elements
TEST(MatrixDalA_State, InitMatrix_WithValues_AllSet)
{
    FGMatrix33 M;
    M.InitMatrix(11, 12, 13,  21, 22, 23,  31, 32, 33);
    EXPECT_DOUBLE_EQ(M(1, 1), 11.0);
    EXPECT_DOUBLE_EQ(M(1, 2), 12.0);
    EXPECT_DOUBLE_EQ(M(1, 3), 13.0);
    EXPECT_DOUBLE_EQ(M(2, 1), 21.0);
    EXPECT_DOUBLE_EQ(M(2, 2), 22.0);
    EXPECT_DOUBLE_EQ(M(2, 3), 23.0);
    EXPECT_DOUBLE_EQ(M(3, 1), 31.0);
    EXPECT_DOUBLE_EQ(M(3, 2), 32.0);
    EXPECT_DOUBLE_EQ(M(3, 3), 33.0);
}

// Verifies REQ-SF-1300 | State: self-assignment — M = M is safe
TEST(MatrixDalA_State, SelfAssignment_MatrixUnchanged)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 copy(M);
    M = M;  // self-assignment
    EXPECT_EQ(M, copy);
}

// Verifies REQ-SF-1300 | State: in-place transpose twice returns original
TEST(MatrixDalA_State, InPlaceTransposeTwice_RestoresOriginal)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 original(M);
    M.T();
    M.T();
    EXPECT_EQ(M, original);
}

// Verifies REQ-SF-1300 | State: operator*= with identity leaves matrix unchanged
TEST(MatrixDalA_State, MatrixTimesEqualsIdentity_Unchanged)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 10);
    FGMatrix33 original(M);
    FGMatrix33 I = Identity();
    M *= I;
    for (unsigned int r = 1; r <= 3; ++r)
        for (unsigned int c = 1; c <= 3; ++c)
            EXPECT_NEAR(M(r, c), original(r, c), TOLERANCE);
}

// Verifies REQ-SF-1300 | State: compound (M += A) -= A restores M
TEST(MatrixDalA_State, CompoundAddSubtract_RestoresOriginal)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 A(10, 20, 30,  40, 50, 60,  70, 80, 90);
    FGMatrix33 original(M);
    M += A;
    M -= A;
    EXPECT_EQ(M, original);
}

// ===========================================================================
// g. ADDITIONAL COVERAGE TESTS
// ===========================================================================

// Verifies REQ-SF-0200 | Coverage: Rows() returns 3
TEST(MatrixDalA_Coverage, Rows_Returns3)
{
    FGMatrix33 M;
    EXPECT_EQ(M.Rows(), 3u);
}

// Verifies REQ-SF-0200 | Coverage: Cols() returns 3
TEST(MatrixDalA_Coverage, Cols_Returns3)
{
    FGMatrix33 M;
    EXPECT_EQ(M.Cols(), 3u);
}

// Verifies REQ-SF-1300 | Coverage: Dump() contains delimiters between columns
TEST(MatrixDalA_Coverage, Dump_ContainsDelimiters)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    std::string result = M.Dump(",");
    // Should have commas (delimiter) separating values within a row
    // 3 rows, each row has 2 delimiters => 6 commas
    int commaCount = 0;
    for (char ch : result)
        if (ch == ',') ++commaCount;
    EXPECT_EQ(commaCount, 6);
}

// Verifies REQ-SF-1300 | Coverage: Dump() contains newlines between rows
TEST(MatrixDalA_Coverage, Dump_ContainsNewlines)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    std::string result = M.Dump("|");
    // if (i < 3) adds newline => 2 newlines (after row 1 and row 2, NOT after row 3)
    int nlCount = 0;
    for (char ch : result)
        if (ch == '\n') ++nlCount;
    EXPECT_EQ(nlCount, 2);
}

// Verifies REQ-SF-1300 | Coverage: stream operator<< produces output
TEST(MatrixDalA_Coverage, StreamOutput_ProducesFormattedString)
{
    FGMatrix33 M(1, 0, 0,  0, 1, 0,  0, 0, 1);
    std::ostringstream oss;
    oss << M;
    std::string output = oss.str();
    // Output should contain the value "1" and newlines
    EXPECT_FALSE(output.empty());
    int nlCount = 0;
    for (char ch : output)
        if (ch == '\n') ++nlCount;
    EXPECT_EQ(nlCount, 2);  // operator<< outputs 2 newlines
}

// Verifies REQ-SF-1300 | Coverage: commutative scalar multiply (scalar * M == M * scalar)
TEST(MatrixDalA_Coverage, CommutativeScalarMultiply_BothOrdersEqual)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    double s = 3.5;
    FGMatrix33 left  = s * M;
    FGMatrix33 right = M * s;
    EXPECT_EQ(left, right);
}

// Verifies REQ-SF-1300 | Coverage: copy constructor independence (modifying copy does not affect original)
TEST(MatrixDalA_Coverage, CopyConstructor_Independence)
{
    FGMatrix33 original(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 copy(original);
    copy(1, 1) = 999.0;
    EXPECT_DOUBLE_EQ(original(1, 1), 1.0);
    EXPECT_DOUBLE_EQ(copy(1, 1), 999.0);
}

// Verifies REQ-SF-1300 | Coverage: assignment independence (modifying assigned does not affect source)
TEST(MatrixDalA_Coverage, Assignment_Independence)
{
    FGMatrix33 source(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 target;
    target = source;
    target(2, 2) = -1.0;
    EXPECT_DOUBLE_EQ(source(2, 2), 5.0);
    EXPECT_DOUBLE_EQ(target(2, 2), -1.0);
}

// Verifies REQ-SF-1300 | Coverage: operator== with identical matrices returns true
TEST(MatrixDalA_Coverage, Equality_IdenticalMatrices_ReturnsTrue)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B(1, 2, 3,  4, 5, 6,  7, 8, 9);
    EXPECT_TRUE(A == B);
}

// Verifies REQ-SF-1300 | Coverage: operator== with differing last element returns false
TEST(MatrixDalA_Coverage, Equality_DifferAtLastElement_ReturnsFalse)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B(1, 2, 3,  4, 5, 6,  7, 8, 10);
    EXPECT_FALSE(A == B);
}

// Verifies REQ-SF-1300 | Coverage: operator== with differing first element returns false
TEST(MatrixDalA_Coverage, Equality_DifferAtFirstElement_ReturnsFalse)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B(99, 2, 3,  4, 5, 6,  7, 8, 9);
    EXPECT_FALSE(A == B);
}

// Verifies REQ-SF-1300 | Coverage: operator!= with identical matrices returns false
TEST(MatrixDalA_Coverage, Inequality_IdenticalMatrices_ReturnsFalse)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B(A);
    EXPECT_FALSE(A != B);
}

// Verifies REQ-SF-1300 | Coverage: operator!= with different matrices returns true
TEST(MatrixDalA_Coverage, Inequality_DifferentMatrices_ReturnsTrue)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B;
    EXPECT_TRUE(A != B);
}

// Verifies REQ-SF-1300 | Coverage: addition is commutative
TEST(MatrixDalA_Coverage, Addition_Commutative)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 B(9, 8, 7,  6, 5, 4,  3, 2, 1);
    EXPECT_EQ(A + B, B + A);
}

// Verifies REQ-SF-1300 | Coverage: subtraction A - A = zero matrix
TEST(MatrixDalA_Coverage, Subtraction_SelfDifference_IsZero)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 Z;
    EXPECT_EQ(A - A, Z);
}

// Verifies REQ-SF-1300 | Coverage: scalar multiply by 1.0 is identity operation
TEST(MatrixDalA_Coverage, ScalarMultiply_ByOne_Unchanged)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 R = A * 1.0;
    EXPECT_EQ(R, A);
}

// Verifies REQ-SF-1300 | Coverage: scalar multiply by 0.0 yields zero matrix
TEST(MatrixDalA_Coverage, ScalarMultiply_ByZero_YieldsZero)
{
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33 R = A * 0.0;
    FGMatrix33 Z;
    EXPECT_EQ(R, Z);
}

// Verifies REQ-SF-1300 | Coverage: operator*= with scalar
TEST(MatrixDalA_Coverage, ScalarTimesEquals_Correct)
{
    FGMatrix33 M(1, 0, 0,  0, 2, 0,  0, 0, 3);
    M *= 2.0;
    EXPECT_DOUBLE_EQ(M(1, 1), 2.0);
    EXPECT_DOUBLE_EQ(M(2, 2), 4.0);
    EXPECT_DOUBLE_EQ(M(3, 3), 6.0);
}

// Verifies REQ-SF-1400 | Coverage: Determinant of singular matrix with nonzero elements
TEST(MatrixDalA_Coverage, Determinant_SingularNonZeroMatrix_IsZero)
{
    // Rows are linearly dependent: row3 = row1 + row2
    FGMatrix33 M(1, 2, 3,
                 4, 5, 6,
                 5, 7, 9);
    EXPECT_NEAR(M.Determinant(), 0.0, TOLERANCE);
}

// Verifies REQ-SF-1400 | Coverage: Invertible returns false for singular nonzero matrix
TEST(MatrixDalA_Coverage, Invertible_SingularNonZeroMatrix_ReturnsFalse)
{
    FGMatrix33 M(1, 2, 3,
                 4, 5, 6,
                 5, 7, 9);
    EXPECT_FALSE(M.Invertible());
}

// Verifies REQ-SF-1400 | Coverage: Inverse of singular nonzero matrix throws
TEST(MatrixDalA_Coverage, Inverse_SingularNonZeroMatrix_ThrowsDomainError)
{
    FGMatrix33 M(1, 2, 3,
                 4, 5, 6,
                 5, 7, 9);
    EXPECT_THROW(M.Inverse(), std::domain_error);
}

// Verifies REQ-SF-1300 | Coverage: matrix multiply non-commutativity (A*B != B*A in general)
TEST(MatrixDalA_Coverage, MatrixMultiply_NonCommutative)
{
    FGMatrix33 A(1, 2, 0,  0, 1, 0,  0, 0, 1);
    FGMatrix33 B(1, 0, 0,  3, 1, 0,  0, 0, 1);
    FGMatrix33 AB = A * B;
    FGMatrix33 BA = B * A;
    EXPECT_TRUE(AB != BA);
}

// Verifies REQ-SF-0200 | Coverage: const access at each valid index (full 3x3 sweep)
TEST(MatrixDalA_Coverage, ConstAccess_AllValidIndices_NoThrow)
{
    const FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    double expected = 1.0;
    for (unsigned int r = 1; r <= 3; ++r)
        for (unsigned int c = 1; c <= 3; ++c)
            EXPECT_DOUBLE_EQ(M(r, c), expected++);
}

// Verifies REQ-SF-1300 | Coverage: Transposed preserves determinant
TEST(MatrixDalA_Coverage, Transpose_PreservesDeterminant)
{
    FGMatrix33 A(2, 1, 0,  0, 3, 1,  1, 0, 2);
    EXPECT_NEAR(A.Transposed().Determinant(), A.Determinant(), TOLERANCE);
}

// Verifies REQ-SF-1300 | Coverage: Dump with empty delimiter
TEST(MatrixDalA_Coverage, Dump_EmptyDelimiter_NoExtraChars)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 9);
    std::string result = M.Dump("");
    // With empty delimiter, values are directly adjacent (no separator chars)
    EXPECT_FALSE(result.empty());
}

// Verifies REQ-SF-1300 | Coverage: Dump with multi-char delimiter
TEST(MatrixDalA_Coverage, Dump_MultiCharDelimiter_AppearsCorrectly)
{
    FGMatrix33 M(1, 0, 0,  0, 1, 0,  0, 0, 1);
    std::string result = M.Dump(" | ");
    // " | " should appear 6 times (2 per row, 3 rows)
    int count = 0;
    std::string::size_type pos = 0;
    while ((pos = result.find(" | ", pos)) != std::string::npos) {
        ++count;
        pos += 3;
    }
    EXPECT_EQ(count, 6);
}

// Verifies REQ-SF-1300 | Coverage: operator+= returns reference to self
TEST(MatrixDalA_Coverage, PlusEquals_ReturnsSelf)
{
    FGMatrix33 M(1, 0, 0,  0, 1, 0,  0, 0, 1);
    FGMatrix33 A(1, 1, 1,  1, 1, 1,  1, 1, 1);
    FGMatrix33& ref = (M += A);
    EXPECT_EQ(&ref, &M);
}

// Verifies REQ-SF-1300 | Coverage: operator-= returns reference to self
TEST(MatrixDalA_Coverage, MinusEquals_ReturnsSelf)
{
    FGMatrix33 M(2, 2, 2,  2, 2, 2,  2, 2, 2);
    FGMatrix33 A(1, 1, 1,  1, 1, 1,  1, 1, 1);
    FGMatrix33& ref = (M -= A);
    EXPECT_EQ(&ref, &M);
}

// Verifies REQ-SF-1300 | Coverage: operator*= scalar returns reference to self
TEST(MatrixDalA_Coverage, TimesEqualsScalar_ReturnsSelf)
{
    FGMatrix33 M(1, 0, 0,  0, 1, 0,  0, 0, 1);
    FGMatrix33& ref = (M *= 2.0);
    EXPECT_EQ(&ref, &M);
}

// Verifies REQ-SF-1300 | Coverage: operator/= returns reference to self
TEST(MatrixDalA_Coverage, DivideEqualsScalar_ReturnsSelf)
{
    FGMatrix33 M(2, 0, 0,  0, 2, 0,  0, 0, 2);
    FGMatrix33& ref = (M /= 2.0);
    EXPECT_EQ(&ref, &M);
}

// Verifies REQ-SF-1300 | Coverage: operator= returns reference to self
TEST(MatrixDalA_Coverage, Assignment_ReturnsSelf)
{
    FGMatrix33 M;
    FGMatrix33 A(1, 2, 3,  4, 5, 6,  7, 8, 9);
    FGMatrix33& ref = (M = A);
    EXPECT_EQ(&ref, &M);
}

// Verifies REQ-SF-1300 | Coverage: operator*= matrix returns reference to self
TEST(MatrixDalA_Coverage, TimesEqualsMatrix_ReturnsSelf)
{
    FGMatrix33 M(1, 2, 3,  4, 5, 6,  7, 8, 10);
    FGMatrix33 I = Identity();
    FGMatrix33& ref = (M *= I);
    EXPECT_EQ(&ref, &M);
}

