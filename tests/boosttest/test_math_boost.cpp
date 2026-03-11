/*==============================================================================
 Boost.Test suite for JSBSim math library
 Framework #4 of 5 — https://www.boost.org/doc/libs/release/libs/test/
==============================================================================*/

#define BOOST_TEST_MODULE JSBSimMathTests
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <limits>
#include "FGColumnVector3.h"
#include "FGMatrix33.h"

using namespace JSBSim;

static constexpr double EPS = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// FGColumnVector3
// ═══════════════════════════════════════════════════════════════════════════════

BOOST_AUTO_TEST_SUITE(ColumnVector3)

BOOST_AUTO_TEST_CASE(default_constructor_is_zero)
{
    FGColumnVector3 v;
    BOOST_CHECK_EQUAL(v(1), 0.0);
    BOOST_CHECK_EQUAL(v(2), 0.0);
    BOOST_CHECK_EQUAL(v(3), 0.0);
}

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    BOOST_CHECK_EQUAL(v(1), 1.0);
    BOOST_CHECK_EQUAL(v(2), 2.0);
    BOOST_CHECK_EQUAL(v(3), 3.0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    FGColumnVector3 a(4.0, 5.0, 6.0);
    FGColumnVector3 b(a);
    BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(addition)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    FGColumnVector3 c = a + b;
    BOOST_CHECK_EQUAL(c(1), 5.0);
    BOOST_CHECK_EQUAL(c(2), 7.0);
    BOOST_CHECK_EQUAL(c(3), 9.0);
}

BOOST_AUTO_TEST_CASE(subtraction)
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c = a - b;
    BOOST_CHECK_EQUAL(c(1), 9.0);
    BOOST_CHECK_EQUAL(c(2), 18.0);
    BOOST_CHECK_EQUAL(c(3), 27.0);
}

BOOST_AUTO_TEST_CASE(scalar_multiply)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = v * 3.0;
    BOOST_CHECK_EQUAL(r(1), 3.0);
    BOOST_CHECK_EQUAL(r(2), 6.0);
    BOOST_CHECK_EQUAL(r(3), 9.0);
}

BOOST_AUTO_TEST_CASE(scalar_divide)
{
    FGColumnVector3 v(4.0, 8.0, 12.0);
    FGColumnVector3 r = v / 4.0;
    BOOST_CHECK_EQUAL(r(1), 1.0);
    BOOST_CHECK_EQUAL(r(2), 2.0);
    BOOST_CHECK_EQUAL(r(3), 3.0);
}

BOOST_AUTO_TEST_CASE(division_by_zero_throws)
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    BOOST_CHECK_THROW(v / 0.0, std::domain_error);
}

BOOST_AUTO_TEST_CASE(cross_product)
{
    FGColumnVector3 i(1.0, 0.0, 0.0);
    FGColumnVector3 j(0.0, 1.0, 0.0);
    FGColumnVector3 k = i * j;
    BOOST_CHECK_CLOSE(k(3), 1.0, EPS);
    BOOST_CHECK_SMALL(k(1), EPS);
    BOOST_CHECK_SMALL(k(2), EPS);
}

BOOST_AUTO_TEST_CASE(dot_product)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    BOOST_CHECK_EQUAL(DotProduct(a, b), 32.0);
}

BOOST_AUTO_TEST_CASE(magnitude)
{
    FGColumnVector3 v(3.0, 4.0, 0.0);
    BOOST_CHECK_EQUAL(v.Magnitude(), 5.0);
}

BOOST_AUTO_TEST_CASE(normalize)
{
    FGColumnVector3 v(0.0, 0.0, 5.0);
    v.Normalize();
    BOOST_CHECK_CLOSE(v.Magnitude(), 1.0, EPS);
    BOOST_CHECK_CLOSE(v(3), 1.0, EPS);
}

BOOST_AUTO_TEST_CASE(normalize_zero_vector)
{
    FGColumnVector3 v;
    v.Normalize();
    BOOST_CHECK_EQUAL(v.Magnitude(), 0.0);
}

BOOST_AUTO_TEST_CASE(index_out_of_range)
{
    FGColumnVector3 v;
    BOOST_CHECK_THROW(v(0), std::range_error);
    BOOST_CHECK_THROW(v(4), std::range_error);
}

BOOST_AUTO_TEST_CASE(unary_negation)
{
    FGColumnVector3 v(1.0, -2.0, 3.0);
    FGColumnVector3 neg = -v;
    BOOST_CHECK_EQUAL(neg(1), -1.0);
    BOOST_CHECK_EQUAL(neg(2),  2.0);
    BOOST_CHECK_EQUAL(neg(3), -3.0);
}

BOOST_AUTO_TEST_CASE(plus_equals)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(10.0, 20.0, 30.0);
    a += b;
    BOOST_CHECK_EQUAL(a(1), 11.0);
    BOOST_CHECK_EQUAL(a(2), 22.0);
    BOOST_CHECK_EQUAL(a(3), 33.0);
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    FGColumnVector3 v;
    v = {7.0, 8.0, 9.0};
    BOOST_CHECK_EQUAL(v(1), 7.0);
    BOOST_CHECK_EQUAL(v(2), 8.0);
    BOOST_CHECK_EQUAL(v(3), 9.0);
}

BOOST_AUTO_TEST_SUITE_END()

// ═══════════════════════════════════════════════════════════════════════════════
// FGMatrix33
// ═══════════════════════════════════════════════════════════════════════════════

BOOST_AUTO_TEST_SUITE(Matrix33)

BOOST_AUTO_TEST_CASE(default_constructor_is_zero)
{
    FGMatrix33 M;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_EQUAL(M(i,j), 0.0);
}

BOOST_AUTO_TEST_CASE(identity_determinant)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    BOOST_CHECK_EQUAL(I.Determinant(), 1.0);
}

BOOST_AUTO_TEST_CASE(transpose)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    BOOST_CHECK_EQUAL(Mt(1,2), 4.0);
    BOOST_CHECK_EQUAL(Mt(2,1), 2.0);
    BOOST_CHECK_EQUAL(Mt(3,1), 3.0);
}

BOOST_AUTO_TEST_CASE(inverse_of_identity)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 Iinv = I.Inverse();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_CLOSE(Iinv(i,j) + 1.0, I(i,j) + 1.0, EPS);
}

BOOST_AUTO_TEST_CASE(inverse_multiply_gives_identity)
{
    FGMatrix33 M(2,1,0, 0,3,1, 1,0,2);
    FGMatrix33 Minv = M.Inverse();
    FGMatrix33 product = M * Minv;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            BOOST_CHECK_SMALL(product(i,j) - expected, EPS);
        }
}

BOOST_AUTO_TEST_CASE(singular_inverse_throws)
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    BOOST_CHECK_THROW(M.Inverse(), std::domain_error);
}

BOOST_AUTO_TEST_CASE(matrix_vector_multiply)
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGColumnVector3 v(7.0, 8.0, 9.0);
    FGColumnVector3 result = I * v;
    BOOST_CHECK(result == v);
}

BOOST_AUTO_TEST_CASE(matrix_multiplication)
{
    FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 result = A * I;
    BOOST_CHECK(result == A);
}

BOOST_AUTO_TEST_CASE(scalar_multiply)
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 scaled = M * 5.0;
    BOOST_CHECK_EQUAL(scaled(1,1), 5.0);
    BOOST_CHECK_EQUAL(scaled(2,2), 5.0);
    BOOST_CHECK_EQUAL(scaled(3,3), 5.0);
    BOOST_CHECK_EQUAL(scaled(1,2), 0.0);
}

BOOST_AUTO_TEST_CASE(euler_round_trip)
{
    double phi = 0.3, theta = 0.2, psi = 0.1;
    double cp = cos(phi), sp = sin(phi);
    double ct = cos(theta), st = sin(theta);
    double cs = cos(psi), ss = sin(psi);

    FGMatrix33 R(
        ct*cs,              ct*ss,              -st,
        sp*st*cs - cp*ss,   sp*st*ss + cp*cs,   sp*ct,
        cp*st*cs + sp*ss,   cp*st*ss - sp*cs,   cp*ct
    );

    FGColumnVector3 euler = R.GetEuler();
    BOOST_CHECK_CLOSE(euler(1) + 1.0, phi + 1.0, EPS);
    BOOST_CHECK_CLOSE(euler(2) + 1.0, theta + 1.0, EPS);
    BOOST_CHECK_CLOSE(euler(3) + 1.0, psi + 1.0, EPS);
}

BOOST_AUTO_TEST_CASE(index_out_of_range)
{
    FGMatrix33 M;
    BOOST_CHECK_THROW(M(0, 1), std::range_error);
    BOOST_CHECK_THROW(M(1, 4), std::range_error);
}

BOOST_AUTO_TEST_SUITE_END()

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

BOOST_AUTO_TEST_SUITE(AdvancedAerospace)

// --- Test 1: Gimbal lock at theta = +90 deg ---
// Euler extraction degenerates when pitch = +/-90 deg; psi is set to 0 by convention.
// Flight sims must handle this without NaN or crash during aggressive pitch maneuvers.
BOOST_AUTO_TEST_CASE(gimbal_lock_positive_90)
{
    double phi = 0.4;
    double theta = M_PI / 2.0;   // exactly +90 deg
    double psi = 0.6;

    FGMatrix33 R = EulerToMatrix(phi, theta, psi);
    FGColumnVector3 euler = R.GetEuler();

    // theta must be pi/2
    BOOST_CHECK_SMALL(euler(2) - M_PI / 2.0, EPS);
    // in gimbal lock psi is forced to 0; phi absorbs the combined rotation
    BOOST_CHECK_SMALL(euler(3), EPS);
    // Rebuild from extracted angles and verify the matrix matches
    FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_SMALL(R(i,j) - R2(i,j), LOOSE_EPS);
}

// --- Test 2: Gimbal lock at theta = -90 deg ---
// Symmetric case: nose-straight-down causes identical loss of yaw/roll separation.
BOOST_AUTO_TEST_CASE(gimbal_lock_negative_90)
{
    double phi = -0.3;
    double theta = -M_PI / 2.0;
    double psi = 0.7;

    FGMatrix33 R = EulerToMatrix(phi, theta, psi);
    FGColumnVector3 euler = R.GetEuler();

    BOOST_CHECK_SMALL(euler(2) - (-M_PI / 2.0), EPS);
    BOOST_CHECK_SMALL(euler(3), EPS);
    FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_SMALL(R(i,j) - R2(i,j), LOOSE_EPS);
}

// --- Test 3: Rotation composition — R(a)*R(b) vs R(a+b) around same axis ---
// Composing two rotations around Z must equal a single rotation of summed angle.
// Flight sim frame transforms chain DCMs every integration step; errors here accumulate.
BOOST_AUTO_TEST_CASE(rotation_composition_same_axis)
{
    double a = 0.3, b = 0.7;
    FGMatrix33 Ra = EulerToMatrix(0, 0, a);
    FGMatrix33 Rb = EulerToMatrix(0, 0, b);
    FGMatrix33 Rab = EulerToMatrix(0, 0, a + b);

    FGMatrix33 product = Ra * Rb;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_SMALL(product(i,j) - Rab(i,j), EPS);
}

// --- Test 4: DCM orthogonality — R^T * R = I for any rotation matrix ---
// Non-orthogonal DCMs cause frame drift in long flight sim runs.
BOOST_AUTO_TEST_CASE(dcm_orthogonality)
{
    FGMatrix33 R = EulerToMatrix(0.5, -0.3, 1.2);
    FGMatrix33 RtR = R.Transposed() * R;
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_SMALL(RtR(i,j) - I(i,j), EPS);
}

// --- Test 5: Rotation matrix determinant = +1 ---
// Proper rotations must have det=+1; det=-1 means an improper rotation (reflection).
BOOST_AUTO_TEST_CASE(rotation_determinant_is_one)
{
    FGMatrix33 R = EulerToMatrix(1.1, -0.4, 2.3);
    BOOST_CHECK_CLOSE(R.Determinant(), 1.0, EPS);
}

// --- Test 6: Cross product anti-commutativity — a x b = -(b x a) ---
// The Coriolis/centripetal terms in rotating-frame EOM rely on this property.
BOOST_AUTO_TEST_CASE(cross_product_anticommutativity)
{
    FGColumnVector3 a(1.5, -2.3, 0.7);
    FGColumnVector3 b(-0.4, 3.1, -1.8);

    FGColumnVector3 axb = a * b;
    FGColumnVector3 bxa = b * a;

    BOOST_CHECK_SMALL(axb(1) + bxa(1), EPS);
    BOOST_CHECK_SMALL(axb(2) + bxa(2), EPS);
    BOOST_CHECK_SMALL(axb(3) + bxa(3), EPS);
}

// --- Test 7: Jacobi identity — a x (b x c) + b x (c x a) + c x (a x b) = 0 ---
// Fundamental Lie algebra identity; if violated, angular momentum conservation breaks.
BOOST_AUTO_TEST_CASE(jacobi_identity)
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(-1.0, 0.5, 2.0);
    FGColumnVector3 c(0.3, -1.5, 0.8);

    FGColumnVector3 result = a * (b * c) + b * (c * a) + c * (a * b);

    BOOST_CHECK_SMALL(result(1), EPS);
    BOOST_CHECK_SMALL(result(2), EPS);
    BOOST_CHECK_SMALL(result(3), EPS);
}

// --- Test 8: Body-earth round trip — transform velocity body->earth->body ---
// In a flight sim, sensor measurements in the body frame are transformed
// to earth frame for navigation, then back to body frame for control laws.
BOOST_AUTO_TEST_CASE(body_earth_round_trip)
{
    double phi   =  30.0 * M_PI / 180.0;
    double theta =  10.0 * M_PI / 180.0;
    double psi   =  45.0 * M_PI / 180.0;

    FGMatrix33 T_b2e = EulerToMatrix(phi, theta, psi);
    FGMatrix33 T_e2b = T_b2e.Transposed();

    // Body-frame velocity: 250 kts forward, 5 kts sideslip, 2 kts vertical
    FGColumnVector3 v_body(250.0, 5.0, 2.0);

    // body -> earth -> body
    FGColumnVector3 v_earth = T_b2e * v_body;
    FGColumnVector3 v_recovered = T_e2b * v_earth;

    BOOST_CHECK_SMALL(v_recovered(1) - v_body(1), EPS);
    BOOST_CHECK_SMALL(v_recovered(2) - v_body(2), EPS);
    BOOST_CHECK_SMALL(v_recovered(3) - v_body(3), EPS);
}

// --- Test 9: Rotation preserves vector length — ||R*v|| = ||v|| ---
// Defining property of orthogonal transformations. If a rotation changes the
// magnitude of a velocity vector, the aircraft magically gains or loses energy.
BOOST_AUTO_TEST_CASE(rotation_preserves_vector_length)
{
    FGMatrix33 R = EulerToMatrix(0.5, -0.3, 1.2);
    FGColumnVector3 v(100.0, 200.0, 300.0);
    FGColumnVector3 Rv = R * v;

    BOOST_CHECK_CLOSE(Rv.Magnitude(), v.Magnitude(), EPS);
}

// --- Test 10: Matrix-vector distributivity — M*(a+b) = M*a + M*b ---
// Linearity of matrix-vector multiply is fundamental to the EOM formulation.
BOOST_AUTO_TEST_CASE(matrix_vector_distributivity)
{
    FGMatrix33 M(2, 1, 0, 0, 3, 1, 1, 0, 2);
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, -1.0, 0.5);

    FGColumnVector3 lhs = M * (a + b);
    FGColumnVector3 rhs = M * a + M * b;

    BOOST_CHECK_SMALL(lhs(1) - rhs(1), EPS);
    BOOST_CHECK_SMALL(lhs(2) - rhs(2), EPS);
    BOOST_CHECK_SMALL(lhs(3) - rhs(3), EPS);
}

// --- Test 11: Matrix associativity — (A*B)*C = A*(B*C) ---
// Matrix multiplication must be associative for chained frame transforms.
BOOST_AUTO_TEST_CASE(matrix_associativity)
{
    FGMatrix33 A(2, 1, 0, 0, 3, 1, 1, 0, 2);
    FGMatrix33 B(1, 2, -1, 3, 0, 1, -1, 1, 2);
    FGMatrix33 C(0, 1, 3, 2, -1, 0, 1, 2, -1);

    FGMatrix33 lhs = (A * B) * C;
    FGMatrix33 rhs = A * (B * C);

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            BOOST_CHECK_SMALL(lhs(i,j) - rhs(i,j), EPS);
}

// --- Test 12: NaN propagation ---
// Verify that NaN inputs propagate through arithmetic without silently disappearing.
BOOST_AUTO_TEST_CASE(nan_propagation)
{
    double nan = std::numeric_limits<double>::quiet_NaN();
    FGColumnVector3 v(nan, 1.0, 2.0);
    FGColumnVector3 w(3.0, 4.0, 5.0);

    FGColumnVector3 sum = v + w;
    BOOST_CHECK(std::isnan(sum(1)));

    FGColumnVector3 cross = v * w;
    // NaN in component 1 of v propagates into cross product components
    BOOST_CHECK(std::isnan(cross(2)) || std::isnan(cross(3)));

    FGMatrix33 M(nan, 0, 0, 0, 1, 0, 0, 0, 1);
    BOOST_CHECK(std::isnan(M.Determinant()));
}

// --- Test 13: Inf propagation ---
// Verify that infinity inputs propagate through arithmetic correctly.
BOOST_AUTO_TEST_CASE(inf_propagation)
{
    double inf = std::numeric_limits<double>::infinity();
    FGColumnVector3 v(inf, 1.0, 2.0);
    FGColumnVector3 w(3.0, 4.0, 5.0);

    FGColumnVector3 sum = v + w;
    BOOST_CHECK(std::isinf(sum(1)));

    FGColumnVector3 scaled = v * 2.0;
    BOOST_CHECK(std::isinf(scaled(1)));

    FGMatrix33 M(inf, 0, 0, 0, 1, 0, 0, 0, 1);
    BOOST_CHECK(std::isinf(M.Determinant()));
}

BOOST_AUTO_TEST_SUITE_END()
