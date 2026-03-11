/*==============================================================================
 Google Test suite for JSBSim math library
 Framework #1 of 5 — https://github.com/google/googletest
==============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <sstream>
#include "FGColumnVector3.h"
#include "FGMatrix33.h"

using namespace JSBSim;

static constexpr double EPS = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// FGColumnVector3 tests
// ═══════════════════════════════════════════════════════════════════════════════

TEST(ColumnVector3, DefaultConstructorIsZero)
{
    FGColumnVector3 v;
    EXPECT_DOUBLE_EQ(v(1), 0.0);
    EXPECT_DOUBLE_EQ(v(2), 0.0);
    EXPECT_DOUBLE_EQ(v(3), 0.0);
}

TEST(ColumnVector3, ParameterizedConstructor)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v(1), 1.0);
    EXPECT_DOUBLE_EQ(v(2), 2.0);
    EXPECT_DOUBLE_EQ(v(3), 3.0);
}

TEST(ColumnVector3, CopyConstructor)
{
    FGColumnVector3 a(4.0, 5.0, 6.0);
    FGColumnVector3 b(a);
    EXPECT_EQ(a, b);
}

TEST(ColumnVector3, Addition)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    FGColumnVector3 c = a + b;
    EXPECT_DOUBLE_EQ(c(1), 5.0);
    EXPECT_DOUBLE_EQ(c(2), 7.0);
    EXPECT_DOUBLE_EQ(c(3), 9.0);
}

TEST(ColumnVector3, Subtraction)
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c = a - b;
    EXPECT_DOUBLE_EQ(c(1), 9.0);
    EXPECT_DOUBLE_EQ(c(2), 18.0);
    EXPECT_DOUBLE_EQ(c(3), 27.0);
}

TEST(ColumnVector3, ScalarMultiply)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = v * 3.0;
    EXPECT_DOUBLE_EQ(r(1), 3.0);
    EXPECT_DOUBLE_EQ(r(2), 6.0);
    EXPECT_DOUBLE_EQ(r(3), 9.0);
}

TEST(ColumnVector3, ScalarDivide)
{
    FGColumnVector3 v(4.0, 8.0, 12.0);
    FGColumnVector3 r = v / 4.0;
    EXPECT_DOUBLE_EQ(r(1), 1.0);
    EXPECT_DOUBLE_EQ(r(2), 2.0);
    EXPECT_DOUBLE_EQ(r(3), 3.0);
}

TEST(ColumnVector3, DivisionByZeroThrows)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    EXPECT_THROW(v / 0.0, std::domain_error);
}

TEST(ColumnVector3, CrossProduct)
{
    // i x j = k
    FGColumnVector3 i(1.0, 0.0, 0.0);
    FGColumnVector3 j(0.0, 1.0, 0.0);
    FGColumnVector3 k = i * j;
    EXPECT_NEAR(k(1), 0.0, EPS);
    EXPECT_NEAR(k(2), 0.0, EPS);
    EXPECT_NEAR(k(3), 1.0, EPS);
}

TEST(ColumnVector3, DotProduct)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    EXPECT_DOUBLE_EQ(DotProduct(a, b), 32.0); // 4+10+18
}

TEST(ColumnVector3, Magnitude)
{
    FGColumnVector3 v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.Magnitude(), 5.0);
}

TEST(ColumnVector3, Normalize)
{
    FGColumnVector3 v(0.0, 0.0, 5.0);
    v.Normalize();
    EXPECT_NEAR(v.Magnitude(), 1.0, EPS);
    EXPECT_NEAR(v(3), 1.0, EPS);
}

TEST(ColumnVector3, NormalizeZeroVectorIsNoOp)
{
    FGColumnVector3 v;
    v.Normalize();  // should not crash
    EXPECT_DOUBLE_EQ(v.Magnitude(), 0.0);
}

TEST(ColumnVector3, IndexOutOfRangeThrows)
{
    FGColumnVector3 v;
    EXPECT_THROW(v(0), std::range_error);
    EXPECT_THROW(v(4), std::range_error);
}

TEST(ColumnVector3, UnaryNegation)
{
    FGColumnVector3 v(1.0, -2.0, 3.0);
    FGColumnVector3 neg = -v;
    EXPECT_DOUBLE_EQ(neg(1), -1.0);
    EXPECT_DOUBLE_EQ(neg(2),  2.0);
    EXPECT_DOUBLE_EQ(neg(3), -3.0);
}

TEST(ColumnVector3, PlusEquals)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(10.0, 20.0, 30.0);
    a += b;
    EXPECT_DOUBLE_EQ(a(1), 11.0);
    EXPECT_DOUBLE_EQ(a(2), 22.0);
    EXPECT_DOUBLE_EQ(a(3), 33.0);
}

TEST(ColumnVector3, InitializerListAssignment)
{
    FGColumnVector3 v;
    v = {7.0, 8.0, 9.0};
    EXPECT_DOUBLE_EQ(v(1), 7.0);
    EXPECT_DOUBLE_EQ(v(2), 8.0);
    EXPECT_DOUBLE_EQ(v(3), 9.0);
}

// ═══════════════════════════════════════════════════════════════════════════════
// FGMatrix33 tests
// ═══════════════════════════════════════════════════════════════════════════════

TEST(Matrix33, DefaultConstructorIsZero)
{
    FGMatrix33 M;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_DOUBLE_EQ(M(i,j), 0.0);
}

TEST(Matrix33, IdentityDeterminant)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    EXPECT_DOUBLE_EQ(I.Determinant(), 1.0);
}

TEST(Matrix33, Transpose)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    EXPECT_DOUBLE_EQ(Mt(1,2), 4.0);
    EXPECT_DOUBLE_EQ(Mt(2,1), 2.0);
    EXPECT_DOUBLE_EQ(Mt(3,1), 3.0);
}

TEST(Matrix33, InverseOfIdentityIsIdentity)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 Iinv = I.Inverse();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(Iinv(i,j), I(i,j), EPS);
}

TEST(Matrix33, InverseMultiplyGivesIdentity)
{
    FGMatrix33 M(2,1,0, 0,3,1, 1,0,2);
    FGMatrix33 Minv = M.Inverse();
    FGMatrix33 product = M * Minv;

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            EXPECT_NEAR(product(i,j), expected, EPS);
        }
}

TEST(Matrix33, SingularMatrixInverseThrows)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9); // det = 0
    EXPECT_THROW(M.Inverse(), std::domain_error);
}

TEST(Matrix33, MatrixVectorMultiply)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGColumnVector3 v(7.0, 8.0, 9.0);
    FGColumnVector3 result = I * v;
    EXPECT_EQ(result, v);
}

TEST(Matrix33, MatrixMultiplication)
{
    FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 result = A * I;
    EXPECT_EQ(result, A);
}

TEST(Matrix33, ScalarMultiply)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 scaled = M * 5.0;
    EXPECT_DOUBLE_EQ(scaled(1,1), 5.0);
    EXPECT_DOUBLE_EQ(scaled(2,2), 5.0);
    EXPECT_DOUBLE_EQ(scaled(3,3), 5.0);
    EXPECT_DOUBLE_EQ(scaled(1,2), 0.0);
}

TEST(Matrix33, GetEulerRoundTrip)
{
    // Build a rotation matrix from known Euler angles, then extract them back.
    double phi = 0.3, theta = 0.2, psi = 0.1; // radians

    double cp = cos(phi), sp = sin(phi);
    double ct = cos(theta), st = sin(theta);
    double cs = cos(psi), ss = sin(psi);

    // ZYX rotation matrix
    FGMatrix33 R(
        ct*cs,              ct*ss,              -st,
        sp*st*cs - cp*ss,   sp*st*ss + cp*cs,   sp*ct,
        cp*st*cs + sp*ss,   cp*st*ss - sp*cs,   cp*ct
    );

    FGColumnVector3 euler = R.GetEuler();
    EXPECT_NEAR(euler(1), phi,   EPS);
    EXPECT_NEAR(euler(2), theta, EPS);
    EXPECT_NEAR(euler(3), psi,   EPS);
}

TEST(Matrix33, IndexOutOfRangeThrows)
{
    FGMatrix33 M;
    EXPECT_THROW(M(0, 1), std::range_error);
    EXPECT_THROW(M(1, 4), std::range_error);
}

// ═══════════════════════════════════════════════════════════════════════════════
// ADVANCED TESTS — Numerical stability, aerospace scenarios, math properties
// ═══════════════════════════════════════════════════════════════════════════════

static constexpr double LOOSE_EPS = 1e-6;

// Helper: build ZYX Tait-Bryan rotation matrix from Euler angles (phi, theta, psi)
static FGMatrix33 EulerToMatrix(double phi, double theta, double psi)
{
    double cp = cos(phi), sp = sin(phi);
    double ct = cos(theta), st = sin(theta);
    double cs = cos(psi), ss = sin(psi);
    return FGMatrix33(
        ct*cs,              ct*ss,              -st,
        sp*st*cs - cp*ss,   sp*st*ss + cp*cs,   sp*ct,
        cp*st*cs + sp*ss,   cp*st*ss - sp*cs,   cp*ct
    );
}

// --- Test 1: Gimbal lock at theta = +90 deg ---
// Euler extraction degenerates when pitch = +/-90 deg; psi is set to 0 by convention.
// Flight sims must handle this without NaN or crash during aggressive pitch maneuvers.
TEST(AdvancedAerospace, GimbalLockPositive90)
{
    double phi = 0.4;
    double theta = M_PI / 2.0;   // exactly +90 deg
    double psi = 0.6;

    FGMatrix33 R = EulerToMatrix(phi, theta, psi);
    FGColumnVector3 euler = R.GetEuler();

    // theta must be pi/2
    EXPECT_NEAR(euler(2), M_PI / 2.0, EPS);
    // in gimbal lock psi is forced to 0; phi absorbs the combined rotation
    EXPECT_NEAR(euler(3), 0.0, EPS);
    // phi should equal (phi - psi) of original, i.e. combined angle
    // Rebuild from extracted angles and verify the matrix matches
    FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(R(i,j), R2(i,j), LOOSE_EPS);
}

// --- Test 2: Gimbal lock at theta = -90 deg ---
// Symmetric case: nose-straight-down causes identical loss of yaw/roll separation.
TEST(AdvancedAerospace, GimbalLockNegative90)
{
    double phi = -0.3;
    double theta = -M_PI / 2.0;
    double psi = 0.7;

    FGMatrix33 R = EulerToMatrix(phi, theta, psi);
    FGColumnVector3 euler = R.GetEuler();

    EXPECT_NEAR(euler(2), -M_PI / 2.0, EPS);
    EXPECT_NEAR(euler(3), 0.0, EPS);
    FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(R(i,j), R2(i,j), LOOSE_EPS);
}

// --- Test 3: Rotation composition — R(a)*R(b) vs R(a+b) around same axis ---
// Composing two rotations around Z must equal a single rotation of summed angle.
// Flight sim frame transforms chain DCMs every integration step; errors here accumulate.
TEST(AdvancedAerospace, RotationCompositionSameAxis)
{
    double a = 0.3, b = 0.7;
    FGMatrix33 Ra = EulerToMatrix(0, 0, a);
    FGMatrix33 Rb = EulerToMatrix(0, 0, b);
    FGMatrix33 Rab = EulerToMatrix(0, 0, a + b);

    FGMatrix33 product = Ra * Rb;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(product(i,j), Rab(i,j), EPS);
}

// --- Test 4: DCM orthogonality — R^T * R = I for any rotation matrix ---
// Non-orthogonal DCMs cause frame drift in long flight sim runs.
TEST(AdvancedAerospace, DCMOrthogonality)
{
    FGMatrix33 R = EulerToMatrix(0.5, -0.3, 1.2);
    FGMatrix33 RtR = R.Transposed() * R;
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(RtR(i,j), I(i,j), EPS);
}

// --- Test 5: Rotation matrix determinant = +1 ---
// Proper rotations must have det=+1; det=-1 means an improper rotation (reflection).
TEST(AdvancedAerospace, RotationDeterminantIsOne)
{
    FGMatrix33 R = EulerToMatrix(1.1, -0.4, 2.3);
    EXPECT_NEAR(R.Determinant(), 1.0, EPS);
}

// --- Test 6: Near-singular matrix inverse stability ---
// An ill-conditioned matrix (high condition number) stresses the adjugate inversion method.
// Sim models with nearly coplanar inertia axes can produce such matrices.
TEST(AdvancedNumerical, NearSingularInverse)
{
    // Hilbert-like 3x3 — condition number ~ O(10^3)
    FGMatrix33 H(1.0,    1.0/2, 1.0/3,
                 1.0/2,  1.0/3, 1.0/4,
                 1.0/3,  1.0/4, 1.0/5);

    FGMatrix33 Hinv = H.Inverse();
    FGMatrix33 product = H * Hinv;

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            EXPECT_NEAR(product(i,j), expected, LOOSE_EPS);
        }
}

// --- Test 7: Cross product anti-commutativity — a x b = -(b x a) ---
// The Coriolis/centripetal terms in rotating-frame EOM rely on this property.
TEST(AdvancedMath, CrossProductAntiCommutativity)
{
    FGColumnVector3 a(1.5, -2.3, 0.7);
    FGColumnVector3 b(-0.4, 3.1, -1.8);

    FGColumnVector3 axb = a * b;
    FGColumnVector3 bxa = b * a;

    EXPECT_NEAR(axb(1), -bxa(1), EPS);
    EXPECT_NEAR(axb(2), -bxa(2), EPS);
    EXPECT_NEAR(axb(3), -bxa(3), EPS);
}

// --- Test 8: Jacobi identity — a x (b x c) + b x (c x a) + c x (a x b) = 0 ---
// Fundamental Lie algebra identity; if violated, angular momentum conservation breaks.
TEST(AdvancedMath, JacobiIdentity)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(-1.0, 0.5, 2.0);
    FGColumnVector3 c(0.3, -1.5, 0.8);

    FGColumnVector3 result = a * (b * c) + b * (c * a) + c * (a * b);

    EXPECT_NEAR(result(1), 0.0, EPS);
    EXPECT_NEAR(result(2), 0.0, EPS);
    EXPECT_NEAR(result(3), 0.0, EPS);
}

// =============================================================================
// ELITE STRESS / PROPERTY-BASED TESTS
// Aerospace-grade verification of FGMatrix33 and FGColumnVector3 numerical
// properties critical to flight dynamics model integrity.
// =============================================================================

// Helpers: elementary rotation matrices and utilities for the stress tests.
// These mirror the standard aerospace Tait-Bryan convention used throughout
// JSBSim for Direction Cosine Matrix (DCM) frame transformations in the 6-DOF
// equations of motion.

namespace {

constexpr double DEG2RAD = M_PI / 180.0;

// Rotation about the body X-axis (roll / phi)
FGMatrix33 RotX(double phi) {
    double cp = cos(phi), sp = sin(phi);
    return FGMatrix33(1,   0,   0,
                      0,  cp,  sp,
                      0, -sp,  cp);
}

// Rotation about the body Y-axis (pitch / theta)
FGMatrix33 RotY(double theta) {
    double ct = cos(theta), st = sin(theta);
    return FGMatrix33(ct,  0, -st,
                       0,  1,   0,
                      st,  0,  ct);
}

// Rotation about the body Z-axis (yaw / psi)
FGMatrix33 RotZ(double psi) {
    double cs = cos(psi), ss = sin(psi);
    return FGMatrix33( cs, ss, 0,
                      -ss, cs, 0,
                        0,  0, 1);
}

// Full ZYX DCM from Euler angles (same convention as EulerToMatrix above,
// but declared inside the anonymous namespace for stress test use)
FGMatrix33 BuildDCM(double phi, double theta, double psi) {
    double cp = cos(phi),   sp = sin(phi);
    double ct = cos(theta), st = sin(theta);
    double cs = cos(psi),   ss = sin(psi);
    return FGMatrix33(
        ct*cs,              ct*ss,              -st,
        sp*st*cs - cp*ss,   sp*st*ss + cp*cs,   sp*ct,
        cp*st*cs + sp*ss,   cp*st*ss - sp*cs,   cp*ct
    );
}

// Assert that a 3x3 matrix is identity within tolerance
void ExpectIdentity(const FGMatrix33& M, double tol, const char* ctx) {
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            EXPECT_NEAR(M(i,j), expected, tol)
                << ctx << " element (" << i << "," << j << ")";
        }
}

} // anonymous namespace

// -----------------------------------------------------------------------------
// TEST 1: Rotation matrix fundamental properties
// In flight dynamics, the DCM transforming between NED-earth and body frames
// must be a proper rotation: det(R)=1, R*R^T=I, and no reflection (det != -1).
// Violation would mean non-physical frame warping in the EOM integrator.
// Sweeps 150 attitude combinations covering the full SO(3) manifold.
// -----------------------------------------------------------------------------
TEST(EliteStress, RotationMatrixProperties_DetOne_Orthogonal_ProperRotation)
{
    double phis[]   = {0.0, 30.0, -45.0, 90.0, 179.0, -179.0};
    double thetas[] = {0.0, 15.0, -30.0, 60.0, -85.0};
    double psis[]   = {0.0, 45.0, 120.0, 270.0, 359.0};

    for (double phi_d : phis) {
        for (double theta_d : thetas) {
            for (double psi_d : psis) {
                double phi   = phi_d   * DEG2RAD;
                double theta = theta_d * DEG2RAD;
                double psi   = psi_d   * DEG2RAD;

                FGMatrix33 R = BuildDCM(phi, theta, psi);

                // Property 1: determinant of a rotation matrix is exactly +1
                EXPECT_NEAR(R.Determinant(), 1.0, 1e-12)
                    << "phi=" << phi_d << " theta=" << theta_d
                    << " psi=" << psi_d;

                // Property 2: orthogonality — R * R^T = I
                FGMatrix33 RRt = R * R.Transposed();
                ExpectIdentity(RRt, 1e-12, "R*R^T orthogonality");

                // Property 3: proper rotation (not a reflection)
                EXPECT_GT(R.Determinant(), 0.0);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// TEST 2: Successive rotations composed via elemental matrices, then
// GetEuler() recovers the original angles.
// The elemental decomposition Rx(phi)*Ry(theta)*Rz(psi) is independent of the
// monolithic BuildDCM formula — this cross-checks both construction paths.
// Covers small-angle, moderate, and extreme attitude regimes.
// -----------------------------------------------------------------------------
TEST(EliteStress, SuccessiveRotations_ComposeAndRecoverEuler)
{
    // Angles chosen so that atan2 output stays in [-pi, pi] for phi/psi,
    // and |theta| < 90 deg (non-gimbal-lock regime).
    // The GetEuler() extraction uses atan2 which returns [-pi, pi], so we
    // keep psi in [-180, 180) deg to avoid 2*pi wrapping ambiguity.
    double test_angles[][3] = {
        {  10.0,   5.0,   20.0},   // mild cruise
        { -30.0,  15.0,  100.0},   // banked turn
        {  45.0, -20.0, -160.0},   // descending spiral (200 deg wrapped to -160)
        { 120.0,  40.0,  -10.0},   // inverted steep climb (350 deg wrapped to -10)
        {-170.0, -60.0,   80.0},   // near-inverted dive
        {   0.1,   0.1,    0.1},   // near-zero — small angle linearization regime
        { 179.9,  89.0,   -0.1},   // edge of valid domain (359.9 wrapped to -0.1)
    };

    for (auto& a : test_angles) {
        double phi   = a[0] * DEG2RAD;
        double theta = a[1] * DEG2RAD;
        double psi   = a[2] * DEG2RAD;

        // Compose using three independent elemental rotation matrices
        FGMatrix33 R = RotX(phi) * RotY(theta) * RotZ(psi);

        FGColumnVector3 euler = R.GetEuler();

        EXPECT_NEAR(euler(1), phi,   1e-9)
            << "phi recovery failed (" << a[0] << "," << a[1] << "," << a[2] << ")";
        EXPECT_NEAR(euler(2), theta, 1e-9)
            << "theta recovery failed (" << a[0] << "," << a[1] << "," << a[2] << ")";
        EXPECT_NEAR(euler(3), psi,   1e-9)
            << "psi recovery failed (" << a[0] << "," << a[1] << "," << a[2] << ")";
    }
}

// -----------------------------------------------------------------------------
// TEST 3: Gimbal lock precision at theta = +/-89.99999 degrees
// At theta = +/-90 deg, the DCM loses one degree of freedom (gimbal lock).
// The Euler extraction must remain bounded (no NaN/Inf) and the DCM
// round-trip must be preserved. In real flight sims this corresponds to
// a pure nose-up/down attitude — rare but must never crash the FDM.
// Tolerance is deliberately relaxed because the decomposition is
// mathematically ambiguous at the singularity.
// -----------------------------------------------------------------------------
TEST(EliteStress, GimbalLockPrecision_NearNinetyDegPitch)
{
    double theta_up   =  89.99999 * DEG2RAD;
    double theta_down = -89.99999 * DEG2RAD;
    double phi = 25.0 * DEG2RAD;
    double psi = 60.0 * DEG2RAD;

    for (double theta : {theta_up, theta_down}) {
        FGMatrix33 R = BuildDCM(phi, theta, psi);
        FGColumnVector3 euler = R.GetEuler();

        // Theta is the unambiguous angle — must be recovered accurately
        EXPECT_NEAR(euler(2), theta, 1e-5)
            << "theta near gimbal lock: " << theta / DEG2RAD << " deg";

        // Phi and psi are coupled at gimbal lock; verify they are finite
        EXPECT_FALSE(std::isnan(euler(1))) << "phi is NaN at gimbal lock";
        EXPECT_FALSE(std::isinf(euler(1))) << "phi is Inf at gimbal lock";
        EXPECT_FALSE(std::isnan(euler(3))) << "psi is NaN at gimbal lock";
        EXPECT_FALSE(std::isinf(euler(3))) << "psi is Inf at gimbal lock";

        // The reconstructed DCM from extracted angles must match the original
        FGMatrix33 R2 = BuildDCM(euler(1), euler(2), euler(3));
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                EXPECT_NEAR(R(i,j), R2(i,j), 1e-6)
                    << "DCM round-trip at gimbal lock (" << i << "," << j << ")";
    }

    // Exact 90 degrees: enters the clamped/gimbal-lock branch in GetEuler()
    {
        FGMatrix33 R90 = BuildDCM(phi, M_PI / 2.0, psi);
        FGColumnVector3 e90 = R90.GetEuler();

        EXPECT_NEAR(e90(2), M_PI / 2.0, EPS);
        EXPECT_FALSE(std::isnan(e90(1)));
        EXPECT_FALSE(std::isnan(e90(3)));
        // In gimbal lock branch, psi is forced to 0
        EXPECT_NEAR(e90(3), 0.0, EPS);
    }
}

// -----------------------------------------------------------------------------
// TEST 4: Large iteration stress — 10,000 accumulated rotations
// In a typical 6-DOF sim running at 120 Hz for a 2-hour sortie, the DCM is
// updated ~864,000 times. Here we apply a small rotation 10,000 times and
// verify the accumulated matrix has not drifted away from SO(3). This catches
// catastrophic floating-point error accumulation in operator*= chains.
// -----------------------------------------------------------------------------
TEST(EliteStress, LargeIterationOrthogonalityDrift_10000Steps)
{
    // Small rotation per step: ~0.01 deg combined about all three axes
    double dPhi   = 0.01  * DEG2RAD;
    double dTheta = 0.005 * DEG2RAD;
    double dPsi   = 0.008 * DEG2RAD;

    FGMatrix33 dR = BuildDCM(dPhi, dTheta, dPsi);

    // Start at identity (wings-level, heading north)
    FGMatrix33 accumulated(1,0,0, 0,1,0, 0,0,1);

    constexpr int N = 10000;
    for (int i = 0; i < N; ++i) {
        accumulated = accumulated * dR;
    }

    // Determinant must still be ~1.0 after 10k multiplications
    EXPECT_NEAR(accumulated.Determinant(), 1.0, 1e-6)
        << "Determinant drifted after " << N << " iterations";

    // Orthogonality: R * R^T should still be close to I
    FGMatrix33 RRt = accumulated * accumulated.Transposed();
    ExpectIdentity(RRt, 1e-6, "10k-iteration orthogonality");

    // Column vectors of R should each have unit magnitude
    for (unsigned int col = 1; col <= 3; ++col) {
        FGColumnVector3 c(accumulated(1,col),
                          accumulated(2,col),
                          accumulated(3,col));
        EXPECT_NEAR(c.Magnitude(), 1.0, 1e-6)
            << "Column " << col << " magnitude drift after " << N << " steps";
    }
}

// -----------------------------------------------------------------------------
// TEST 5: Inverse of product property — (A*B)^-1 = B^-1 * A^-1
// Essential for frame chain transformations in multi-body dynamics (e.g.,
// aircraft -> pylon -> store -> seeker chain). If this breaks, hierarchical
// coordinate transforms silently corrupt weapons delivery solutions.
// -----------------------------------------------------------------------------
TEST(EliteStress, InverseOfProduct_AB_inv_equals_Binv_Ainv)
{
    FGMatrix33 A(2,  1,  0,
                 0,  3,  1,
                 1,  0,  2);

    FGMatrix33 B(1,  2, -1,
                 3,  0,  1,
                -1,  1,  2);

    FGMatrix33 AB = A * B;

    // (A*B)^-1 computed directly
    FGMatrix33 AB_inv = AB.Inverse();

    // B^-1 * A^-1 computed from individual inverses
    FGMatrix33 Binv_Ainv = B.Inverse() * A.Inverse();

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(AB_inv(i,j), Binv_Ainv(i,j), EPS)
                << "(A*B)^-1 vs B^-1*A^-1 at (" << i << "," << j << ")";

    // Cross-verify: (A*B) * (A*B)^-1 = I
    ExpectIdentity(AB * AB_inv, EPS, "(A*B)*(A*B)^-1");
}

// -----------------------------------------------------------------------------
// TEST 6: Frame transformation round-trip (body -> NED earth -> body)
// In a flight sim, sensor measurements in the body frame are transformed
// to earth frame for navigation, then back to body frame for control laws.
// A single-step round-trip must return the original vector exactly.
// Also cross-validates that Transposed() and Inverse() agree for DCMs.
// -----------------------------------------------------------------------------
TEST(EliteStress, FrameTransformRoundTrip_BodyEarthBody)
{
    // Aircraft attitude: 30 deg bank, 10 deg climb, heading 045
    double phi   =  30.0 * DEG2RAD;
    double theta =  10.0 * DEG2RAD;
    double psi   =  45.0 * DEG2RAD;

    FGMatrix33 T_b2e = BuildDCM(phi, theta, psi);
    FGMatrix33 T_e2b = T_b2e.Transposed();

    // Body-frame velocity: 250 kts forward, 5 kts sideslip, 2 kts vertical
    FGColumnVector3 v_body(250.0, 5.0, 2.0);

    // body -> earth -> body
    FGColumnVector3 v_earth = T_b2e * v_body;
    FGColumnVector3 v_recovered = T_e2b * v_earth;

    EXPECT_NEAR(v_recovered(1), v_body(1), EPS) << "X-body round-trip";
    EXPECT_NEAR(v_recovered(2), v_body(2), EPS) << "Y-body round-trip";
    EXPECT_NEAR(v_recovered(3), v_body(3), EPS) << "Z-body round-trip";

    // Also via explicit Inverse() to exercise both code paths
    FGMatrix33 T_b2e_inv = T_b2e.Inverse();
    FGColumnVector3 v_via_inv = T_b2e_inv * v_earth;

    EXPECT_NEAR(v_via_inv(1), v_body(1), EPS) << "X-body via Inverse()";
    EXPECT_NEAR(v_via_inv(2), v_body(2), EPS) << "Y-body via Inverse()";
    EXPECT_NEAR(v_via_inv(3), v_body(3), EPS) << "Z-body via Inverse()";

    // Transpose and Inverse must produce the same matrix for a rotation
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(T_e2b(i,j), T_b2e_inv(i,j), EPS)
                << "Transpose vs Inverse at (" << i << "," << j << ")";
}

// -----------------------------------------------------------------------------
// TEST 7: Determinant of product — det(A*B) = det(A)*det(B)
// This multiplicative property validates Determinant() and operator*() in
// concert. For general (non-rotation) matrices this ensures volume scaling
// consistency across composed transforms. Includes a triple product check
// and a hand-computed determinant cross-verification.
// -----------------------------------------------------------------------------
TEST(EliteStress, DeterminantOfProduct_Multiplicative)
{
    FGMatrix33 A(3, -1,  2,
                 0,  4,  5,
                -2,  1, -3);

    FGMatrix33 B( 1,  0, -2,
                  3, -1,  4,
                 -2,  5,  1);

    double detA  = A.Determinant();
    double detB  = B.Determinant();
    double detAB = (A * B).Determinant();

    EXPECT_NEAR(detAB, detA * detB, 1e-8)
        << "det(A*B)=" << detAB << " vs det(A)*det(B)=" << detA * detB;

    // Triple product: det(A*B*C) = det(A)*det(B)*det(C)
    FGMatrix33 C(2, 1, -1,
                 0, 3,  2,
                 1, 0,  4);

    double detC   = C.Determinant();
    double detABC = (A * B * C).Determinant();
    EXPECT_NEAR(detABC, detA * detB * detC, 1e-6)
        << "det(A*B*C) triple product";

    // Hand-computed: det(A) = 3(4*(-3)-5*1) + 1(0*(-3)-5*(-2)) + 2(0*1-4*(-2))
    //                       = 3(-17) + 1(10) + 2(8) = -51+10+16 = -25
    EXPECT_NEAR(detA, -25.0, EPS) << "Hand-computed det(A) verification";
}

// -----------------------------------------------------------------------------
// TEST 8: Near-singular matrix — condition number ~1e12
// Stress-tests Inverse() against an extremely ill-conditioned matrix.
// In real FDM work, near-singular Jacobians arise during trim convergence and
// at aerodynamic stall boundaries. Two tiers are tested:
// (a) condition ~1e8 — still invertible with useful precision
// (b) condition ~1e12 — extreme; verifies A*A^-1 is roughly I but expects
//     severe precision degradation in the determinant reciprocal
// -----------------------------------------------------------------------------
TEST(EliteStress, NearSingularMatrix_IllConditionedInverse)
{
    // --- Tier A: moderate ill-conditioning, kappa ~ 1e8 ----------------------
    {
        FGMatrix33 D(1.0,  0.0,  0.0,
                     0.0,  1.0,  0.0,
                     0.0,  0.0,  1e-8);

        FGMatrix33 R = BuildDCM(0.7, 0.3, 1.1);
        FGMatrix33 A = R.Transposed() * D * R;

        // Determinant = product of eigenvalues = 1e-8
        EXPECT_NEAR(A.Determinant(), 1e-8, 1e-10);

        FGMatrix33 Ainv = A.Inverse();

        // A * A^-1 ~ I with moderate precision loss (~8 of 16 digits)
        FGMatrix33 product = A * Ainv;
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j) {
                double expected = (i == j) ? 1.0 : 0.0;
                EXPECT_NEAR(product(i,j), expected, 1e-6)
                    << "Tier-A A*A^-1 at (" << i << "," << j << ")";
            }

        // det(A^-1) ~ 1/det(A) — still meaningful at kappa=1e8
        double detAinv = Ainv.Determinant();
        double expected_detAinv = 1.0 / A.Determinant();
        double rel_err = std::abs(detAinv - expected_detAinv)
                       / std::abs(expected_detAinv);
        EXPECT_LT(rel_err, 1e-2)
            << "Tier-A det(A^-1) relative error: " << rel_err;
    }

    // --- Tier B: extreme ill-conditioning, kappa ~ 1e12 ----------------------
    {
        FGMatrix33 D(1.0,  0.0,  0.0,
                     0.0,  1.0,  0.0,
                     0.0,  0.0,  1e-12);

        FGMatrix33 R = BuildDCM(0.7, 0.3, 1.1);
        FGMatrix33 A = R.Transposed() * D * R;

        EXPECT_NEAR(A.Determinant(), 1e-12, 1e-14);

        FGMatrix33 Ainv = A.Inverse();

        // At kappa=1e12, A*A^-1 will deviate significantly from I.
        // We verify it is at least in the right ballpark (diagonal ~1, off ~small).
        FGMatrix33 product = A * Ainv;
        for (unsigned int i = 1; i <= 3; ++i) {
            // Diagonal elements should be roughly 1
            EXPECT_NEAR(product(i,i), 1.0, 0.5)
                << "Tier-B diagonal (" << i << "," << i << ")";
        }

        // The inverse must exist and not contain NaN or Inf
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j) {
                EXPECT_FALSE(std::isnan(Ainv(i,j)))
                    << "NaN in inverse at (" << i << "," << j << ")";
                EXPECT_FALSE(std::isinf(Ainv(i,j)))
                    << "Inf in inverse at (" << i << "," << j << ")";
            }
    }
}

// -----------------------------------------------------------------------------
// TEST 9: Rotation preserves vector length — ||R*v|| = ||v||
// Defining property of orthogonal transformations. If a rotation changes the
// magnitude of a velocity vector, the aircraft magically gains or loses kinetic
// energy — a showstopper for energy-based stability analysis and fuel models.
// Tests multiple attitude/magnitude regimes from tiny perturbations to large
// vectors spanning the dynamic range of double precision.
// -----------------------------------------------------------------------------
TEST(EliteStress, RotationPreservesVectorLength)
{
    struct Case { double phi_d, theta_d, psi_d, vx, vy, vz; const char* desc; };

    Case cases[] = {
        {  0,   0,   0,  1, 0, 0,          "Identity, unit X"},
        { 45,  30,  60,  100, 200, 300,     "Typical cruise velocity"},
        { 90,   0,   0,  0, 0, 1,           "90-deg roll, unit Z"},
        {-30, -45, 120,  1e6, 1e6, 1e6,     "Large vector, steep bank"},
        {170,  80, 350,  1e-8, 1e-8, 1e-8,  "Tiny vector, extreme attitude"},
        {  0,   0, 180,  -500, 250, -100,    "180-deg heading reversal"},
    };

    for (auto& tc : cases) {
        FGMatrix33 R = BuildDCM(tc.phi_d * DEG2RAD,
                                 tc.theta_d * DEG2RAD,
                                 tc.psi_d * DEG2RAD);

        FGColumnVector3 v(tc.vx, tc.vy, tc.vz);
        FGColumnVector3 Rv = R * v;

        double origMag = v.Magnitude();
        double rotMag  = Rv.Magnitude();

        if (origMag > 1.0) {
            EXPECT_NEAR(rotMag / origMag, 1.0, 1e-12)
                << tc.desc << ": relative magnitude";
        } else {
            EXPECT_NEAR(rotMag, origMag, 1e-20)
                << tc.desc << ": absolute magnitude";
        }
    }
}

// -----------------------------------------------------------------------------
// TEST 10: Skew-symmetric cross product matrix — v x w = [v]_x * w
// The skew-symmetric form appears in Euler's rigid body equations, the
// transport theorem, and Coriolis terms. Also verifies the skew-symmetric
// property ([v]_x + [v]_x^T = 0) and that det([v]_x) = 0 (always singular).
// -----------------------------------------------------------------------------
TEST(EliteStress, SkewSymmetricCrossProductMatrix)
{
    // Angular velocity vector (rad/s) — typical for a rolling maneuver
    FGColumnVector3 omega(0.5, -0.2, 0.1);  // p, q, r body rates
    FGColumnVector3 v(150.0, 10.0, -5.0);   // body-frame velocity

    // Build [omega]_x:  [  0  -r   q ]
    //                    [  r   0  -p ]
    //                    [ -q   p   0 ]
    double p = omega(1), q = omega(2), r = omega(3);
    FGMatrix33 omega_x(  0, -r,  q,
                         r,  0, -p,
                        -q,  p,  0);

    // Cross product via vector operator vs. matrix multiply
    FGColumnVector3 cross_vec = omega * v;
    FGColumnVector3 cross_mat = omega_x * v;

    EXPECT_NEAR(cross_vec(1), cross_mat(1), EPS) << "X-component";
    EXPECT_NEAR(cross_vec(2), cross_mat(2), EPS) << "Y-component";
    EXPECT_NEAR(cross_vec(3), cross_mat(3), EPS) << "Z-component";

    // Skew-symmetric property: [omega]_x + [omega]_x^T = 0
    FGMatrix33 sum = omega_x + omega_x.Transposed();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(sum(i,j), 0.0, EPS)
                << "Skew-symmetric violation at (" << i << "," << j << ")";

    // Determinant of any 3x3 skew-symmetric matrix is always 0
    EXPECT_NEAR(omega_x.Determinant(), 0.0, EPS)
        << "Skew-symmetric matrix must be singular";
}

// -----------------------------------------------------------------------------
// TEST 11: Transpose-equals-inverse for rotation matrices across SO(3)
// For any R in SO(3), R^T = R^-1. Tests both Transposed() and Inverse()
// agree across a grid of 180 rotation matrices, catching subtle numerical
// disagreements between the adjugate-based inverse and the simple transpose.
// In production sims, transpose is used for performance in inner loops while
// Inverse() is reserved for general matrices in trim/Jacobian code.
// -----------------------------------------------------------------------------
TEST(EliteStress, TransposeEqualsInverse_GridSweep)
{
    for (double phi_d = -180.0; phi_d <= 180.0; phi_d += 72.0) {
        for (double theta_d = -80.0; theta_d <= 80.0; theta_d += 40.0) {
            for (double psi_d = 0.0; psi_d <= 350.0; psi_d += 70.0) {
                FGMatrix33 R = BuildDCM(phi_d * DEG2RAD,
                                         theta_d * DEG2RAD,
                                         psi_d * DEG2RAD);

                FGMatrix33 Rt   = R.Transposed();
                FGMatrix33 Rinv = R.Inverse();

                for (unsigned int i = 1; i <= 3; ++i)
                    for (unsigned int j = 1; j <= 3; ++j)
                        EXPECT_NEAR(Rt(i,j), Rinv(i,j), 1e-12)
                            << "R^T vs R^-1 at (" << i << "," << j
                            << ") phi=" << phi_d << " theta=" << theta_d
                            << " psi=" << psi_d;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// TEST 12: Cross product algebraic properties — anticommutativity, Jacobi,
// self-cross = 0, and scalar triple product = determinant.
// The cross product is used in computing aerodynamic moments, angular
// momentum derivatives, and Coriolis forces. A sign error would reverse
// control surface effectiveness.
// -----------------------------------------------------------------------------
TEST(EliteStress, CrossProductAlgebraicProperties)
{
    FGColumnVector3 a(3.0, -1.0, 4.0);
    FGColumnVector3 b(2.0,  5.0, -2.0);
    FGColumnVector3 c(-1.0, 3.0,  7.0);

    // Anticommutativity: a x b = -(b x a)
    FGColumnVector3 axb = a * b;
    FGColumnVector3 bxa = b * a;
    EXPECT_NEAR(axb(1), -bxa(1), EPS) << "Anticommutativity X";
    EXPECT_NEAR(axb(2), -bxa(2), EPS) << "Anticommutativity Y";
    EXPECT_NEAR(axb(3), -bxa(3), EPS) << "Anticommutativity Z";

    // Jacobi identity: a x (b x c) + b x (c x a) + c x (a x b) = 0
    FGColumnVector3 jacobi = a * (b * c) + b * (c * a) + c * (a * b);
    EXPECT_NEAR(jacobi(1), 0.0, EPS) << "Jacobi X";
    EXPECT_NEAR(jacobi(2), 0.0, EPS) << "Jacobi Y";
    EXPECT_NEAR(jacobi(3), 0.0, EPS) << "Jacobi Z";

    // Self cross product is zero: a x a = 0
    FGColumnVector3 axa = a * a;
    EXPECT_NEAR(axa(1), 0.0, EPS) << "Self-cross X";
    EXPECT_NEAR(axa(2), 0.0, EPS) << "Self-cross Y";
    EXPECT_NEAR(axa(3), 0.0, EPS) << "Self-cross Z";

    // Scalar triple product: a . (b x c) = det([a b c])
    double scalar_triple = DotProduct(a, b * c);
    FGMatrix33 M(a(1), b(1), c(1),
                 a(2), b(2), c(2),
                 a(3), b(3), c(3));
    EXPECT_NEAR(scalar_triple, M.Determinant(), EPS)
        << "Scalar triple product = det of column matrix";
}

// -----------------------------------------------------------------------------
// TEST 13: Cayley-Hamilton theorem — every matrix satisfies its own
// characteristic equation: A^3 - tr(A)*A^2 + c1*A - det(A)*I = 0
// This is a deep algebraic stress test exercising operator+, operator-,
// operator*(matrix), operator*(scalar), and Determinant() all in concert.
// Failure here indicates correlated errors across multiple operators that
// simpler tests might miss.
// -----------------------------------------------------------------------------
TEST(EliteStress, CayleyHamiltonTheorem)
{
    FGMatrix33 A(2, 1, 0,
                 0, 3, 1,
                 1, 0, 2);

    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);

    // Characteristic polynomial coefficients:
    //   p(lambda) = lambda^3 - c2*lambda^2 + c1*lambda - c0
    double trA = A(1,1) + A(2,2) + A(3,3);
    FGMatrix33 A2 = A * A;
    double trA2 = A2(1,1) + A2(2,2) + A2(3,3);

    double c2 = trA;
    double c1 = 0.5 * (trA * trA - trA2);
    double c0 = A.Determinant();

    // Cayley-Hamilton: A^3 - c2*A^2 + c1*A - c0*I = 0
    FGMatrix33 A3 = A2 * A;
    FGMatrix33 result = A3 - A2 * c2 + A * c1 - I * c0;

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_NEAR(result(i,j), 0.0, 1e-8)
                << "Cayley-Hamilton violated at (" << i << "," << j << ")";
}

// -----------------------------------------------------------------------------
// TEST 14: Rotation composition is non-commutative — Rx*Rz != Rz*Rx
// Verifies that matrix multiplication correctly captures the non-abelian
// nature of SO(3). In the flight sim, applying roll-then-yaw must produce
// a different result from yaw-then-roll. If commutativity is observed, the
// matrix multiply is broken (e.g., silently transposing).
// Also validates that both products remain valid rotations.
// -----------------------------------------------------------------------------
TEST(EliteStress, RotationNonCommutativity)
{
    double phi = 45.0 * DEG2RAD;
    double psi = 30.0 * DEG2RAD;

    FGMatrix33 Rx = RotX(phi);
    FGMatrix33 Rz = RotZ(psi);

    FGMatrix33 RxRz = Rx * Rz;
    FGMatrix33 RzRx = Rz * Rx;

    // The two products must differ
    bool allSame = true;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            if (std::abs(RxRz(i,j) - RzRx(i,j)) > 1e-14)
                allSame = false;

    EXPECT_FALSE(allSame)
        << "Rotation composition appears commutative — operator* may be broken";

    // Both products must still be valid rotations
    EXPECT_NEAR(RxRz.Determinant(), 1.0, EPS);
    EXPECT_NEAR(RzRx.Determinant(), 1.0, EPS);
    ExpectIdentity(RxRz * RxRz.Transposed(), EPS, "Rx*Rz orthogonality");
    ExpectIdentity(RzRx * RzRx.Transposed(), EPS, "Rz*Rx orthogonality");

    // Physical demonstration: the rotated vectors must differ
    FGColumnVector3 v(1.0, 0.0, 0.0);
    FGColumnVector3 v1 = RxRz * v;
    FGColumnVector3 v2 = RzRx * v;
    FGColumnVector3 diff = v1 - v2;
    EXPECT_GT(diff.Magnitude(), 0.01)
        << "Roll-then-yaw and yaw-then-roll must produce different vectors";
}

// -----------------------------------------------------------------------------
// TEST 15: DCM row/column orthonormality and right-hand rule
// Each row and column of a DCM must be a unit vector, and distinct rows/columns
// must be mutually orthogonal. Additionally, col1 x col2 = col3 enforces the
// right-hand rule (proper rotation, not reflection). This directly validates
// each of the 9 orthonormality constraints defining the SO(3) manifold —
// stricter than just det=1 or R*R^T=I, which can mask compensating errors.
// -----------------------------------------------------------------------------
TEST(EliteStress, DCM_RowColumn_Orthonormality_RightHandRule)
{
    FGMatrix33 R = BuildDCM(37.5 * DEG2RAD, -22.0 * DEG2RAD, 155.0 * DEG2RAD);

    // Extract rows
    FGColumnVector3 row1(R(1,1), R(1,2), R(1,3));
    FGColumnVector3 row2(R(2,1), R(2,2), R(2,3));
    FGColumnVector3 row3(R(3,1), R(3,2), R(3,3));

    // Extract columns
    FGColumnVector3 col1(R(1,1), R(2,1), R(3,1));
    FGColumnVector3 col2(R(1,2), R(2,2), R(3,2));
    FGColumnVector3 col3(R(1,3), R(2,3), R(3,3));

    // Unit length (6 checks)
    EXPECT_NEAR(row1.Magnitude(), 1.0, EPS) << "Row 1 not unit";
    EXPECT_NEAR(row2.Magnitude(), 1.0, EPS) << "Row 2 not unit";
    EXPECT_NEAR(row3.Magnitude(), 1.0, EPS) << "Row 3 not unit";
    EXPECT_NEAR(col1.Magnitude(), 1.0, EPS) << "Col 1 not unit";
    EXPECT_NEAR(col2.Magnitude(), 1.0, EPS) << "Col 2 not unit";
    EXPECT_NEAR(col3.Magnitude(), 1.0, EPS) << "Col 3 not unit";

    // Mutual orthogonality (6 checks)
    EXPECT_NEAR(DotProduct(row1, row2), 0.0, EPS) << "Rows 1-2 not orthogonal";
    EXPECT_NEAR(DotProduct(row1, row3), 0.0, EPS) << "Rows 1-3 not orthogonal";
    EXPECT_NEAR(DotProduct(row2, row3), 0.0, EPS) << "Rows 2-3 not orthogonal";
    EXPECT_NEAR(DotProduct(col1, col2), 0.0, EPS) << "Cols 1-2 not orthogonal";
    EXPECT_NEAR(DotProduct(col1, col3), 0.0, EPS) << "Cols 1-3 not orthogonal";
    EXPECT_NEAR(DotProduct(col2, col3), 0.0, EPS) << "Cols 2-3 not orthogonal";

    // Right-hand rule: col1 x col2 = col3
    FGColumnVector3 col1x2 = col1 * col2;
    EXPECT_NEAR(col1x2(1), col3(1), EPS) << "Right-hand rule X";
    EXPECT_NEAR(col1x2(2), col3(2), EPS) << "Right-hand rule Y";
    EXPECT_NEAR(col1x2(3), col3(3), EPS) << "Right-hand rule Z";
}

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

// --- Vector: Entry() accessor (alias for operator()) ---
TEST(CoverageGap, Vector_Entry_ReadWrite)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.Entry(1), 1.0);
    EXPECT_DOUBLE_EQ(v.Entry(2), 2.0);
    EXPECT_DOUBLE_EQ(v.Entry(3), 3.0);
    v.Entry(2) = 99.0;
    EXPECT_DOUBLE_EQ(v(2), 99.0);
}

// --- Vector: operator!= ---
TEST(CoverageGap, Vector_NotEqual)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c(1.0, 2.0, 4.0);
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

// --- Vector: operator-= ---
TEST(CoverageGap, Vector_MinusEquals)
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    a -= b;
    EXPECT_DOUBLE_EQ(a(1), 9.0);
    EXPECT_DOUBLE_EQ(a(2), 18.0);
    EXPECT_DOUBLE_EQ(a(3), 27.0);
}

// --- Vector: operator*= (scalar) ---
TEST(CoverageGap, Vector_TimesEquals)
{
    FGColumnVector3 v(2.0, 3.0, 4.0);
    v *= 5.0;
    EXPECT_DOUBLE_EQ(v(1), 10.0);
    EXPECT_DOUBLE_EQ(v(2), 15.0);
    EXPECT_DOUBLE_EQ(v(3), 20.0);
}

// --- Vector: operator/= (normal + division-by-zero) ---
TEST(CoverageGap, Vector_DivideEquals)
{
    FGColumnVector3 v(10.0, 20.0, 30.0);
    v /= 5.0;
    EXPECT_DOUBLE_EQ(v(1), 2.0);
    EXPECT_DOUBLE_EQ(v(2), 4.0);
    EXPECT_DOUBLE_EQ(v(3), 6.0);
}

TEST(CoverageGap, Vector_DivideEqualsZeroThrows)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    EXPECT_THROW(v /= 0.0, std::domain_error);
}

// --- Vector: InitMatrix() overloads ---
TEST(CoverageGap, Vector_InitMatrix_NoArg)
{
    FGColumnVector3 v(5.0, 6.0, 7.0);
    v.InitMatrix();
    EXPECT_DOUBLE_EQ(v(1), 0.0);
    EXPECT_DOUBLE_EQ(v(2), 0.0);
    EXPECT_DOUBLE_EQ(v(3), 0.0);
}

TEST(CoverageGap, Vector_InitMatrix_SingleValue)
{
    FGColumnVector3 v;
    v.InitMatrix(42.0);
    EXPECT_DOUBLE_EQ(v(1), 42.0);
    EXPECT_DOUBLE_EQ(v(2), 42.0);
    EXPECT_DOUBLE_EQ(v(3), 42.0);
}

TEST(CoverageGap, Vector_InitMatrix_ThreeValues)
{
    FGColumnVector3 v;
    v.InitMatrix(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v(1), 1.0);
    EXPECT_DOUBLE_EQ(v(2), 2.0);
    EXPECT_DOUBLE_EQ(v(3), 3.0);
}

// --- Vector: Magnitude(int, int) — 2-element partial magnitude ---
TEST(CoverageGap, Vector_Magnitude2D)
{
    FGColumnVector3 v(3.0, 4.0, 99.0);
    // Magnitude of elements 1 and 2 only: sqrt(9+16) = 5
    EXPECT_DOUBLE_EQ(v.Magnitude(1, 2), 5.0);
    // Magnitude of elements 2 and 3: sqrt(16+9801) = sqrt(9817)
    EXPECT_NEAR(v.Magnitude(2, 3), std::sqrt(16.0 + 9801.0), 1e-10);
}

// --- Vector: Dump() ---
TEST(CoverageGap, Vector_Dump)
{
    FGColumnVector3 v(1.5, 2.5, 3.5);
    std::string s = v.Dump(", ");
    EXPECT_FALSE(s.empty());
    // Should contain the values
    EXPECT_NE(s.find("1.5"), std::string::npos);
    EXPECT_NE(s.find("2.5"), std::string::npos);
    EXPECT_NE(s.find("3.5"), std::string::npos);
}

// --- Vector: operator<< (stream output) ---
TEST(CoverageGap, Vector_StreamOutput)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    std::ostringstream os;
    os << v;
    std::string s = os.str();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("1"), std::string::npos);
    EXPECT_NE(s.find("2"), std::string::npos);
    EXPECT_NE(s.find("3"), std::string::npos);
}

// --- Vector: scalar * vector (free function, left-multiply) ---
TEST(CoverageGap, Vector_ScalarLeftMultiply)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = 3.0 * v;
    EXPECT_DOUBLE_EQ(r(1), 3.0);
    EXPECT_DOUBLE_EQ(r(2), 6.0);
    EXPECT_DOUBLE_EQ(r(3), 9.0);
}

// --- Matrix: Entry() accessor ---
TEST(CoverageGap, Matrix_Entry_ReadWrite)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    EXPECT_DOUBLE_EQ(M.Entry(1,1), 1.0);
    EXPECT_DOUBLE_EQ(M.Entry(2,3), 6.0);
    M.Entry(3,3) = 99.0;
    EXPECT_DOUBLE_EQ(M(3,3), 99.0);
}

// --- Matrix: operator!= ---
TEST(CoverageGap, Matrix_NotEqual)
{
    FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 B(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 C(1,0,0, 0,1,0, 0,0,2);
    EXPECT_FALSE(A != B);
    EXPECT_TRUE(A != C);
}

// --- Matrix: T() in-place transpose ---
TEST(CoverageGap, Matrix_InPlaceTranspose)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    M.T();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_DOUBLE_EQ(M(i,j), Mt(i,j));
}

// --- Matrix: Invertible() ---
TEST(CoverageGap, Matrix_Invertible)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    EXPECT_TRUE(I.Invertible());

    FGMatrix33 singular(1,2,3, 4,5,6, 7,8,9);
    EXPECT_FALSE(singular.Invertible());
}

// --- Matrix: Rows() and Cols() ---
TEST(CoverageGap, Matrix_RowsCols)
{
    FGMatrix33 M;
    EXPECT_EQ(M.Rows(), 3u);
    EXPECT_EQ(M.Cols(), 3u);
}

// --- Matrix: InitMatrix() overloads ---
TEST(CoverageGap, Matrix_InitMatrix_NoArg)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    M.InitMatrix();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            EXPECT_DOUBLE_EQ(M(i,j), 0.0);
}

TEST(CoverageGap, Matrix_InitMatrix_NineValues)
{
    FGMatrix33 M;
    M.InitMatrix(9,8,7, 6,5,4, 3,2,1);
    EXPECT_DOUBLE_EQ(M(1,1), 9.0);
    EXPECT_DOUBLE_EQ(M(1,3), 7.0);
    EXPECT_DOUBLE_EQ(M(3,1), 3.0);
    EXPECT_DOUBLE_EQ(M(3,3), 1.0);
}

// --- Matrix: operator+= ---
TEST(CoverageGap, Matrix_PlusEquals)
{
    FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 B(1,1,1, 1,1,1, 1,1,1);
    A += B;
    EXPECT_DOUBLE_EQ(A(1,1), 2.0);
    EXPECT_DOUBLE_EQ(A(1,2), 1.0);
    EXPECT_DOUBLE_EQ(A(2,2), 2.0);
}

// --- Matrix: operator-= ---
TEST(CoverageGap, Matrix_MinusEquals)
{
    FGMatrix33 A(5,5,5, 5,5,5, 5,5,5);
    FGMatrix33 B(1,2,3, 4,5,6, 7,8,9);
    A -= B;
    EXPECT_DOUBLE_EQ(A(1,1), 4.0);
    EXPECT_DOUBLE_EQ(A(2,2), 0.0);
    EXPECT_DOUBLE_EQ(A(3,3), -4.0);
}

// --- Matrix: operator*=(double) ---
TEST(CoverageGap, Matrix_ScalarTimesEquals)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    M *= 3.0;
    EXPECT_DOUBLE_EQ(M(1,1), 3.0);
    EXPECT_DOUBLE_EQ(M(2,2), 3.0);
    EXPECT_DOUBLE_EQ(M(3,3), 3.0);
    EXPECT_DOUBLE_EQ(M(1,2), 0.0);
}

// --- Matrix: operator/(double) normal + division-by-zero ---
TEST(CoverageGap, Matrix_ScalarDivide)
{
    FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
    FGMatrix33 R = M / 10.0;
    EXPECT_DOUBLE_EQ(R(1,1), 1.0);
    EXPECT_DOUBLE_EQ(R(2,2), 5.0);
    EXPECT_DOUBLE_EQ(R(3,3), 9.0);
}

TEST(CoverageGap, Matrix_ScalarDivideZeroThrows)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    EXPECT_THROW(M / 0.0, std::domain_error);
}

// --- Matrix: operator/=(double) normal + division-by-zero ---
TEST(CoverageGap, Matrix_DivideEquals)
{
    FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
    M /= 10.0;
    EXPECT_DOUBLE_EQ(M(1,1), 1.0);
    EXPECT_DOUBLE_EQ(M(3,3), 9.0);
}

TEST(CoverageGap, Matrix_DivideEqualsZeroThrows)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    EXPECT_THROW(M /= 0.0, std::domain_error);
}

// --- Matrix: operator*=(FGMatrix33) ---
TEST(CoverageGap, Matrix_MatrixTimesEquals)
{
    FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 expected = A * I;
    A *= I;
    EXPECT_EQ(A, expected);
}

// --- Matrix: Dump() ---
TEST(CoverageGap, Matrix_Dump)
{
    FGMatrix33 M(1.5, 0, 0, 0, 2.5, 0, 0, 0, 3.5);
    std::string s = M.Dump(", ");
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("1.5"), std::string::npos);
    EXPECT_NE(s.find("2.5"), std::string::npos);
    EXPECT_NE(s.find("3.5"), std::string::npos);
}

// --- Matrix: operator<< (stream output) ---
TEST(CoverageGap, Matrix_StreamOutput)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    std::ostringstream os;
    os << M;
    std::string s = os.str();
    EXPECT_FALSE(s.empty());
    EXPECT_NE(s.find("5"), std::string::npos);
}

// --- Matrix: scalar * matrix (free function, left-multiply) ---
TEST(CoverageGap, Matrix_ScalarLeftMultiply)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 R = 7.0 * M;
    EXPECT_DOUBLE_EQ(R(1,1), 7.0);
    EXPECT_DOUBLE_EQ(R(2,2), 7.0);
    EXPECT_DOUBLE_EQ(R(3,3), 7.0);
    EXPECT_DOUBLE_EQ(R(1,2), 0.0);
}
