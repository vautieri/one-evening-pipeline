/*==============================================================================
 Catch2 suite for JSBSim math library
 Framework #2 of 5 — https://github.com/catchorg/Catch2
==============================================================================*/

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include <sstream>
#include "FGColumnVector3.h"
#include "FGMatrix33.h"

using namespace JSBSim;
using Catch::Approx;

static constexpr double EPS = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// FGColumnVector3
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Vector3: default constructor is zero", "[vector]")
{
    FGColumnVector3 v;
    REQUIRE(v(1) == 0.0);
    REQUIRE(v(2) == 0.0);
    REQUIRE(v(3) == 0.0);
}

TEST_CASE("Vector3: parameterized constructor", "[vector]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    REQUIRE(v(1) == 1.0);
    REQUIRE(v(2) == 2.0);
    REQUIRE(v(3) == 3.0);
}

TEST_CASE("Vector3: copy constructor", "[vector]")
{
    FGColumnVector3 a(4.0, 5.0, 6.0);
    FGColumnVector3 b(a);
    REQUIRE(a == b);
}

TEST_CASE("Vector3: addition", "[vector][arithmetic]")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    FGColumnVector3 c = a + b;
    REQUIRE(c(1) == 5.0);
    REQUIRE(c(2) == 7.0);
    REQUIRE(c(3) == 9.0);
}

TEST_CASE("Vector3: subtraction", "[vector][arithmetic]")
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c = a - b;
    REQUIRE(c(1) == 9.0);
    REQUIRE(c(2) == 18.0);
    REQUIRE(c(3) == 27.0);
}

TEST_CASE("Vector3: scalar multiply", "[vector][arithmetic]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = v * 3.0;
    REQUIRE(r(1) == 3.0);
    REQUIRE(r(2) == 6.0);
    REQUIRE(r(3) == 9.0);
}

TEST_CASE("Vector3: scalar divide", "[vector][arithmetic]")
{
    FGColumnVector3 v(4.0, 8.0, 12.0);
    FGColumnVector3 r = v / 4.0;
    REQUIRE(r(1) == 1.0);
    REQUIRE(r(2) == 2.0);
    REQUIRE(r(3) == 3.0);
}

TEST_CASE("Vector3: division by zero throws", "[vector][error]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    REQUIRE_THROWS_AS(v / 0.0, std::domain_error);
}

TEST_CASE("Vector3: cross product i x j = k", "[vector][arithmetic]")
{
    FGColumnVector3 i(1.0, 0.0, 0.0);
    FGColumnVector3 j(0.0, 1.0, 0.0);
    FGColumnVector3 k = i * j;
    REQUIRE(k(1) == Approx(0.0).margin(EPS));
    REQUIRE(k(2) == Approx(0.0).margin(EPS));
    REQUIRE(k(3) == Approx(1.0).margin(EPS));
}

TEST_CASE("Vector3: dot product", "[vector][arithmetic]")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    REQUIRE(DotProduct(a, b) == 32.0);
}

TEST_CASE("Vector3: magnitude", "[vector]")
{
    FGColumnVector3 v(3.0, 4.0, 0.0);
    REQUIRE(v.Magnitude() == 5.0);
}

TEST_CASE("Vector3: normalize", "[vector]")
{
    FGColumnVector3 v(0.0, 0.0, 5.0);
    v.Normalize();
    REQUIRE(v.Magnitude() == Approx(1.0).margin(EPS));
    REQUIRE(v(3) == Approx(1.0).margin(EPS));
}

TEST_CASE("Vector3: normalize zero vector is no-op", "[vector]")
{
    FGColumnVector3 v;
    v.Normalize();
    REQUIRE(v.Magnitude() == 0.0);
}

TEST_CASE("Vector3: index out of range throws", "[vector][error]")
{
    FGColumnVector3 v;
    REQUIRE_THROWS_AS(v(0), std::range_error);
    REQUIRE_THROWS_AS(v(4), std::range_error);
}

TEST_CASE("Vector3: unary negation", "[vector][arithmetic]")
{
    FGColumnVector3 v(1.0, -2.0, 3.0);
    FGColumnVector3 neg = -v;
    REQUIRE(neg(1) == -1.0);
    REQUIRE(neg(2) ==  2.0);
    REQUIRE(neg(3) == -3.0);
}

TEST_CASE("Vector3: plus-equals", "[vector][arithmetic]")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(10.0, 20.0, 30.0);
    a += b;
    REQUIRE(a(1) == 11.0);
    REQUIRE(a(2) == 22.0);
    REQUIRE(a(3) == 33.0);
}

TEST_CASE("Vector3: initializer list assignment", "[vector]")
{
    FGColumnVector3 v;
    v = {7.0, 8.0, 9.0};
    REQUIRE(v(1) == 7.0);
    REQUIRE(v(2) == 8.0);
    REQUIRE(v(3) == 9.0);
}

// ═══════════════════════════════════════════════════════════════════════════════
// FGMatrix33
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Matrix33: default constructor is zero", "[matrix]")
{
    FGMatrix33 M;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(M(i,j) == 0.0);
}

TEST_CASE("Matrix33: identity determinant", "[matrix]")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    REQUIRE(I.Determinant() == 1.0);
}

TEST_CASE("Matrix33: transpose", "[matrix]")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    REQUIRE(Mt(1,2) == 4.0);
    REQUIRE(Mt(2,1) == 2.0);
    REQUIRE(Mt(3,1) == 3.0);
}

TEST_CASE("Matrix33: inverse of identity", "[matrix]")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 Iinv = I.Inverse();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(Iinv(i,j) == Approx(I(i,j)).margin(EPS));
}

TEST_CASE("Matrix33: M * M^-1 = I", "[matrix]")
{
    FGMatrix33 M(2,1,0, 0,3,1, 1,0,2);
    FGMatrix33 Minv = M.Inverse();
    FGMatrix33 product = M * Minv;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            REQUIRE(product(i,j) == Approx(expected).margin(EPS));
        }
}

TEST_CASE("Matrix33: singular matrix inverse throws", "[matrix][error]")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    REQUIRE_THROWS_AS(M.Inverse(), std::domain_error);
}

TEST_CASE("Matrix33: matrix * vector", "[matrix][vector]")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGColumnVector3 v(7.0, 8.0, 9.0);
    FGColumnVector3 result = I * v;
    REQUIRE(result == v);
}

TEST_CASE("Matrix33: matrix multiplication with identity", "[matrix]")
{
    FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 result = A * I;
    REQUIRE(result == A);
}

TEST_CASE("Matrix33: scalar multiply", "[matrix][arithmetic]")
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 scaled = M * 5.0;
    REQUIRE(scaled(1,1) == 5.0);
    REQUIRE(scaled(2,2) == 5.0);
    REQUIRE(scaled(3,3) == 5.0);
    REQUIRE(scaled(1,2) == 0.0);
}

TEST_CASE("Matrix33: Euler round-trip", "[matrix][euler]")
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
    REQUIRE(euler(1) == Approx(phi).margin(EPS));
    REQUIRE(euler(2) == Approx(theta).margin(EPS));
    REQUIRE(euler(3) == Approx(psi).margin(EPS));
}

TEST_CASE("Matrix33: index out of range throws", "[matrix][error]")
{
    FGMatrix33 M;
    REQUIRE_THROWS_AS(M(0, 1), std::range_error);
    REQUIRE_THROWS_AS(M(1, 4), std::range_error);
}

// ═══════════════════════════════════════════════════════════════════════════════
// ADVANCED TESTS — Numerical stability, aerospace scenarios, math properties
// ═══════════════════════════════════════════════════════════════════════════════

static constexpr double LOOSE_EPS = 1e-6;

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

// --- Test 9: Body-to-Earth velocity transform round trip ---
// Transform velocity body->earth then earth->body and recover original. Core of 6DOF propagation.
TEST_CASE("Advanced: body-earth velocity round trip", "[aerospace][advanced]")
{
    FGMatrix33 Tbe = EulerToMatrix(0.1, 0.2, 0.8);  // body-to-earth DCM
    FGColumnVector3 v_body(100.0, 5.0, -2.0);        // typical airspeed components (m/s)

    FGColumnVector3 v_earth = Tbe * v_body;
    FGColumnVector3 v_back  = Tbe.Transposed() * v_earth;

    REQUIRE(v_back(1) == Approx(v_body(1)).margin(EPS));
    REQUIRE(v_back(2) == Approx(v_body(2)).margin(EPS));
    REQUIRE(v_back(3) == Approx(v_body(3)).margin(EPS));
}

// --- Test 10: Accumulated rotation drift over many small steps ---
// Simulating many small yaw increments (like 1-deg steps for a full circle) and checking
// that the accumulated DCM is still orthogonal. Drift here causes position errors in sim.
TEST_CASE("Advanced: accumulated rotation drift 360 steps", "[aerospace][numerical][advanced]")
{
    int N = 360;
    double dPsi = 2.0 * M_PI / N;  // 1-degree steps

    FGMatrix33 accum(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 dR = EulerToMatrix(0, 0, dPsi);

    for (int i = 0; i < N; ++i)
        accum = accum * dR;

    // After 360 one-degree steps we should be back at identity
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(accum(i,j) == Approx(I(i,j)).margin(LOOSE_EPS));
}

// --- Test 11: Distributivity of matrix-vector multiply: M*(a+b) = M*a + M*b ---
// Any violation means force/moment summation in body frame is incorrect.
TEST_CASE("Advanced: matrix-vector distributivity", "[math][advanced]")
{
    FGMatrix33 M(2, -1, 3, 0.5, 4, -2, 1, 1, 1);
    FGColumnVector3 a(1.0, -2.0, 3.0);
    FGColumnVector3 b(4.0, 0.5, -1.0);

    FGColumnVector3 lhs = M * (a + b);
    FGColumnVector3 rhs = M * a + M * b;

    REQUIRE(lhs(1) == Approx(rhs(1)).margin(EPS));
    REQUIRE(lhs(2) == Approx(rhs(2)).margin(EPS));
    REQUIRE(lhs(3) == Approx(rhs(3)).margin(EPS));
}

// --- Test 12: Matrix multiplication associativity — (A*B)*C = A*(B*C) ---
// Chained frame transforms (e.g., body->wind->stability->earth) must associate correctly.
TEST_CASE("Advanced: matrix multiplication associativity", "[math][advanced]")
{
    FGMatrix33 A = EulerToMatrix(0.1, 0.2, 0.3);
    FGMatrix33 B = EulerToMatrix(-0.4, 0.5, -0.6);
    FGMatrix33 C = EulerToMatrix(0.7, -0.8, 0.9);

    FGMatrix33 lhs = (A * B) * C;
    FGMatrix33 rhs = A * (B * C);

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(lhs(i,j) == Approx(rhs(i,j)).margin(EPS));
}

// --- Test 13: Very large values — satellite orbit velocities ---
// At orbital speeds (~7800 m/s) with large position vectors (~6.4e6 m), arithmetic must not overflow.
TEST_CASE("Advanced: large value arithmetic stability", "[numerical][advanced]")
{
    FGColumnVector3 pos(6.378e6, 0.0, 0.0);          // ~Earth radius in meters
    FGColumnVector3 vel(0.0, 7800.0, 0.0);            // orbital speed

    FGColumnVector3 h = pos * vel;                     // angular momentum vector (cross product)
    double h_mag = h.Magnitude();

    // |h| = |r| * |v| * sin(90 deg) = 6.378e6 * 7800
    REQUIRE(h_mag == Approx(6.378e6 * 7800.0).epsilon(1e-12));

    // h should point along Z
    REQUIRE(h(1) == Approx(0.0).margin(1.0));
    REQUIRE(h(2) == Approx(0.0).margin(1.0));
    REQUIRE(h(3) == Approx(6.378e6 * 7800.0).epsilon(1e-12));
}

// --- Test 14: Very small (denormalized) float handling ---
// Denormals arise in trim convergence or near-zero control deflections; must not produce NaN.
TEST_CASE("Advanced: denormalized float arithmetic", "[numerical][advanced]")
{
    double tiny = 5e-308;  // near denorm boundary
    FGColumnVector3 v(tiny, tiny, tiny);

    FGColumnVector3 sum = v + v;
    REQUIRE(sum(1) == Approx(2.0 * tiny).margin(0.0));

    double dot = DotProduct(v, v);
    REQUIRE(dot >= 0.0);            // must not be negative
    REQUIRE(!std::isnan(dot));
    REQUIRE(!std::isinf(dot));

    // Magnitude may underflow to zero — that is acceptable, but must not be NaN
    double mag = v.Magnitude();
    REQUIRE(!std::isnan(mag));
}

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

TEST_CASE("Vector3: Entry() read/write accessor", "[vector][coverage]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    REQUIRE(v.Entry(1) == 1.0);
    REQUIRE(v.Entry(2) == 2.0);
    v.Entry(2) = 99.0;
    REQUIRE(v(2) == 99.0);
}

TEST_CASE("Vector3: operator!=", "[vector][coverage]")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c(1.0, 2.0, 4.0);
    REQUIRE_FALSE(a != b);
    REQUIRE(a != c);
}

TEST_CASE("Vector3: minus-equals", "[vector][coverage]")
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    a -= b;
    REQUIRE(a(1) == 9.0);
    REQUIRE(a(2) == 18.0);
    REQUIRE(a(3) == 27.0);
}

TEST_CASE("Vector3: times-equals", "[vector][coverage]")
{
    FGColumnVector3 v(2.0, 3.0, 4.0);
    v *= 5.0;
    REQUIRE(v(1) == 10.0);
    REQUIRE(v(2) == 15.0);
    REQUIRE(v(3) == 20.0);
}

TEST_CASE("Vector3: divide-equals", "[vector][coverage]")
{
    FGColumnVector3 v(10.0, 20.0, 30.0);
    v /= 5.0;
    REQUIRE(v(1) == 2.0);
    REQUIRE(v(2) == 4.0);
    REQUIRE(v(3) == 6.0);
}

TEST_CASE("Vector3: divide-equals by zero throws", "[vector][coverage][error]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    REQUIRE_THROWS_AS(v /= 0.0, std::domain_error);
}

TEST_CASE("Vector3: InitMatrix overloads", "[vector][coverage]")
{
    FGColumnVector3 v(5.0, 6.0, 7.0);
    v.InitMatrix();
    REQUIRE(v(1) == 0.0);
    REQUIRE(v(2) == 0.0);
    REQUIRE(v(3) == 0.0);

    v.InitMatrix(42.0);
    REQUIRE(v(1) == 42.0);
    REQUIRE(v(2) == 42.0);
    REQUIRE(v(3) == 42.0);

    v.InitMatrix(1.0, 2.0, 3.0);
    REQUIRE(v(1) == 1.0);
    REQUIRE(v(2) == 2.0);
    REQUIRE(v(3) == 3.0);
}

TEST_CASE("Vector3: 2-element magnitude", "[vector][coverage]")
{
    FGColumnVector3 v(3.0, 4.0, 99.0);
    REQUIRE(v.Magnitude(1, 2) == 5.0);
    REQUIRE(v.Magnitude(2, 3) == Approx(std::sqrt(16.0 + 9801.0)).margin(1e-10));
}

TEST_CASE("Vector3: Dump", "[vector][coverage]")
{
    FGColumnVector3 v(1.5, 2.5, 3.5);
    std::string s = v.Dump(", ");
    REQUIRE_FALSE(s.empty());
    REQUIRE(s.find("1.5") != std::string::npos);
    REQUIRE(s.find("3.5") != std::string::npos);
}

TEST_CASE("Vector3: stream output operator<<", "[vector][coverage]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    std::ostringstream os;
    os << v;
    REQUIRE_FALSE(os.str().empty());
}

TEST_CASE("Vector3: scalar left-multiply (free function)", "[vector][coverage]")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = 3.0 * v;
    REQUIRE(r(1) == 3.0);
    REQUIRE(r(2) == 6.0);
    REQUIRE(r(3) == 9.0);
}

TEST_CASE("Matrix33: Entry() read/write accessor", "[matrix][coverage]")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    REQUIRE(M.Entry(1,1) == 1.0);
    REQUIRE(M.Entry(2,3) == 6.0);
    M.Entry(3,3) = 99.0;
    REQUIRE(M(3,3) == 99.0);
}

TEST_CASE("Matrix33: operator!=", "[matrix][coverage]")
{
    FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 B(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 C(1,0,0, 0,1,0, 0,0,2);
    REQUIRE_FALSE(A != B);
    REQUIRE(A != C);
}

TEST_CASE("Matrix33: in-place transpose T()", "[matrix][coverage]")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    M.T();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(M(i,j) == Mt(i,j));
}

TEST_CASE("Matrix33: Invertible()", "[matrix][coverage]")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    REQUIRE(I.Invertible());
    FGMatrix33 singular(1,2,3, 4,5,6, 7,8,9);
    REQUIRE_FALSE(singular.Invertible());
}

TEST_CASE("Matrix33: Rows() and Cols()", "[matrix][coverage]")
{
    FGMatrix33 M;
    REQUIRE(M.Rows() == 3u);
    REQUIRE(M.Cols() == 3u);
}

TEST_CASE("Matrix33: InitMatrix overloads", "[matrix][coverage]")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    M.InitMatrix();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            REQUIRE(M(i,j) == 0.0);

    M.InitMatrix(9,8,7, 6,5,4, 3,2,1);
    REQUIRE(M(1,1) == 9.0);
    REQUIRE(M(3,3) == 1.0);
}

TEST_CASE("Matrix33: compound assignment operators", "[matrix][coverage]")
{
    SECTION("+=") {
        FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 B(1,1,1, 1,1,1, 1,1,1);
        A += B;
        REQUIRE(A(1,1) == 2.0);
        REQUIRE(A(1,2) == 1.0);
    }
    SECTION("-=") {
        FGMatrix33 A(5,5,5, 5,5,5, 5,5,5);
        FGMatrix33 B(1,2,3, 4,5,6, 7,8,9);
        A -= B;
        REQUIRE(A(1,1) == 4.0);
        REQUIRE(A(2,2) == 0.0);
    }
    SECTION("*= scalar") {
        FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
        M *= 3.0;
        REQUIRE(M(1,1) == 3.0);
        REQUIRE(M(1,2) == 0.0);
    }
    SECTION("/= scalar") {
        FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
        M /= 10.0;
        REQUIRE(M(1,1) == 1.0);
        REQUIRE(M(3,3) == 9.0);
    }
    SECTION("*= matrix") {
        FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 expected = A * I;
        A *= I;
        REQUIRE(A == expected);
    }
}

TEST_CASE("Matrix33: scalar divide and divide-by-zero", "[matrix][coverage][error]")
{
    FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
    FGMatrix33 R = M / 10.0;
    REQUIRE(R(1,1) == 1.0);
    REQUIRE_THROWS_AS(M / 0.0, std::domain_error);
    REQUIRE_THROWS_AS(M /= 0.0, std::domain_error);
}

TEST_CASE("Matrix33: Dump and stream output", "[matrix][coverage]")
{
    FGMatrix33 M(1.5, 0, 0, 0, 2.5, 0, 0, 0, 3.5);
    std::string s = M.Dump(", ");
    REQUIRE_FALSE(s.empty());
    REQUIRE(s.find("1.5") != std::string::npos);

    std::ostringstream os;
    os << M;
    REQUIRE_FALSE(os.str().empty());
}

TEST_CASE("Matrix33: scalar left-multiply (free function)", "[matrix][coverage]")
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 R = 7.0 * M;
    REQUIRE(R(1,1) == 7.0);
    REQUIRE(R(2,2) == 7.0);
    REQUIRE(R(1,2) == 0.0);
}
