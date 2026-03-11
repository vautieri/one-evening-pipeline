/*==============================================================================
 doctest suite for JSBSim math library
 Framework #3 of 5 — https://github.com/doctest/doctest
==============================================================================*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cmath>
#include <sstream>
#include "FGColumnVector3.h"
#include "FGMatrix33.h"

using namespace JSBSim;

static constexpr double EPS = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// FGColumnVector3
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("ColumnVector3") {

TEST_CASE("default constructor is zero")
{
    FGColumnVector3 v;
    CHECK(v(1) == 0.0);
    CHECK(v(2) == 0.0);
    CHECK(v(3) == 0.0);
}

TEST_CASE("parameterized constructor")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    CHECK(v(1) == 1.0);
    CHECK(v(2) == 2.0);
    CHECK(v(3) == 3.0);
}

TEST_CASE("copy constructor")
{
    FGColumnVector3 a(4.0, 5.0, 6.0);
    FGColumnVector3 b(a);
    CHECK(a == b);
}

TEST_CASE("addition")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    FGColumnVector3 c = a + b;
    CHECK(c(1) == 5.0);
    CHECK(c(2) == 7.0);
    CHECK(c(3) == 9.0);
}

TEST_CASE("subtraction")
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c = a - b;
    CHECK(c(1) == 9.0);
    CHECK(c(2) == 18.0);
    CHECK(c(3) == 27.0);
}

TEST_CASE("scalar multiply")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = v * 3.0;
    CHECK(r(1) == 3.0);
    CHECK(r(2) == 6.0);
    CHECK(r(3) == 9.0);
}

TEST_CASE("scalar divide")
{
    FGColumnVector3 v(4.0, 8.0, 12.0);
    FGColumnVector3 r = v / 4.0;
    CHECK(r(1) == 1.0);
    CHECK(r(2) == 2.0);
    CHECK(r(3) == 3.0);
}

TEST_CASE("division by zero throws")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    CHECK_THROWS_AS(v / 0.0, std::domain_error);
}

TEST_CASE("cross product i x j = k")
{
    FGColumnVector3 i(1.0, 0.0, 0.0);
    FGColumnVector3 j(0.0, 1.0, 0.0);
    FGColumnVector3 k = i * j;
    CHECK(k(1) == doctest::Approx(0.0).epsilon(EPS));
    CHECK(k(2) == doctest::Approx(0.0).epsilon(EPS));
    CHECK(k(3) == doctest::Approx(1.0).epsilon(EPS));
}

TEST_CASE("dot product")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(4.0, 5.0, 6.0);
    CHECK(DotProduct(a, b) == 32.0);
}

TEST_CASE("magnitude")
{
    FGColumnVector3 v(3.0, 4.0, 0.0);
    CHECK(v.Magnitude() == 5.0);
}

TEST_CASE("normalize")
{
    FGColumnVector3 v(0.0, 0.0, 5.0);
    v.Normalize();
    CHECK(v.Magnitude() == doctest::Approx(1.0).epsilon(EPS));
    CHECK(v(3) == doctest::Approx(1.0).epsilon(EPS));
}

TEST_CASE("normalize zero vector is no-op")
{
    FGColumnVector3 v;
    v.Normalize();
    CHECK(v.Magnitude() == 0.0);
}

TEST_CASE("index out of range throws")
{
    FGColumnVector3 v;
    CHECK_THROWS_AS(v(0), std::range_error);
    CHECK_THROWS_AS(v(4), std::range_error);
}

TEST_CASE("unary negation")
{
    FGColumnVector3 v(1.0, -2.0, 3.0);
    FGColumnVector3 neg = -v;
    CHECK(neg(1) == -1.0);
    CHECK(neg(2) ==  2.0);
    CHECK(neg(3) == -3.0);
}

TEST_CASE("plus-equals")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(10.0, 20.0, 30.0);
    a += b;
    CHECK(a(1) == 11.0);
    CHECK(a(2) == 22.0);
    CHECK(a(3) == 33.0);
}

TEST_CASE("initializer list assignment")
{
    FGColumnVector3 v;
    v = {7.0, 8.0, 9.0};
    CHECK(v(1) == 7.0);
    CHECK(v(2) == 8.0);
    CHECK(v(3) == 9.0);
}

} // TEST_SUITE

// ═══════════════════════════════════════════════════════════════════════════════
// FGMatrix33
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("Matrix33") {

TEST_CASE("default constructor is zero")
{
    FGMatrix33 M;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            CHECK(M(i,j) == 0.0);
}

TEST_CASE("identity determinant")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    CHECK(I.Determinant() == 1.0);
}

TEST_CASE("transpose")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    CHECK(Mt(1,2) == 4.0);
    CHECK(Mt(2,1) == 2.0);
    CHECK(Mt(3,1) == 3.0);
}

TEST_CASE("inverse of identity")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 Iinv = I.Inverse();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            CHECK(Iinv(i,j) == doctest::Approx(I(i,j)).epsilon(EPS));
}

TEST_CASE("M * M^-1 = I")
{
    FGMatrix33 M(2,1,0, 0,3,1, 1,0,2);
    FGMatrix33 Minv = M.Inverse();
    FGMatrix33 product = M * Minv;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            CHECK(product(i,j) == doctest::Approx(expected).epsilon(EPS));
        }
}

TEST_CASE("singular matrix inverse throws")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    CHECK_THROWS_AS(M.Inverse(), std::domain_error);
}

TEST_CASE("matrix * vector")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGColumnVector3 v(7.0, 8.0, 9.0);
    FGColumnVector3 result = I * v;
    CHECK(result == v);
}

TEST_CASE("matrix multiplication with identity")
{
    FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 result = A * I;
    CHECK(result == A);
}

TEST_CASE("scalar multiply")
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 scaled = M * 5.0;
    CHECK(scaled(1,1) == 5.0);
    CHECK(scaled(2,2) == 5.0);
    CHECK(scaled(3,3) == 5.0);
    CHECK(scaled(1,2) == 0.0);
}

TEST_CASE("Euler round-trip")
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
    CHECK(euler(1) == doctest::Approx(phi).epsilon(EPS));
    CHECK(euler(2) == doctest::Approx(theta).epsilon(EPS));
    CHECK(euler(3) == doctest::Approx(psi).epsilon(EPS));
}

TEST_CASE("index out of range throws")
{
    FGMatrix33 M;
    CHECK_THROWS_AS(M(0, 1), std::range_error);
    CHECK_THROWS_AS(M(1, 4), std::range_error);
}

} // TEST_SUITE

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

TEST_SUITE("AdvancedAerospace") {

// --- Test 15: Full Euler sweep — round trip over grid of angles ---
// Verifies GetEuler correctness across the non-singular attitude envelope.
// Sims encounter all attitude combinations; a single failing quadrant corrupts guidance.
TEST_CASE("Euler round trip over full attitude grid")
{
    double phis[]   = {-2.5, -1.0, 0.0, 0.5, 2.8};
    double thetas[] = {-1.2, -0.3, 0.0, 0.4, 1.2};
    double psis[]   = {-3.0, -0.5, 0.0, 1.0, 3.0};

    for (double phi : phis) {
        for (double theta : thetas) {
            for (double psi : psis) {
                FGMatrix33 R = EulerToMatrix(phi, theta, psi);
                FGColumnVector3 euler = R.GetEuler();
                FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));

                for (unsigned int i = 1; i <= 3; ++i)
                    for (unsigned int j = 1; j <= 3; ++j)
                        CHECK(R(i,j) == doctest::Approx(R2(i,j)).epsilon(LOOSE_EPS));
            }
        }
    }
}

// --- Test 16: Inverse rotation undoes rotation — R^-1 * R * v = v ---
// If the inverse DCM does not perfectly cancel the forward DCM, nav errors accumulate.
TEST_CASE("inverse rotation recovers original vector")
{
    FGMatrix33 R = EulerToMatrix(0.6, -0.25, 1.8);
    FGColumnVector3 v(100.0, -50.0, 25.0);

    FGColumnVector3 result = R.Inverse() * (R * v);

    CHECK(result(1) == doctest::Approx(v(1)).epsilon(EPS));
    CHECK(result(2) == doctest::Approx(v(2)).epsilon(EPS));
    CHECK(result(3) == doctest::Approx(v(3)).epsilon(EPS));
}

// --- Test 17: Cross product magnitude = |a||b|sin(theta) ---
// Moment = r x F; incorrect magnitude means wrong torque on the airframe.
TEST_CASE("cross product magnitude equals |a||b|sin(theta)")
{
    FGColumnVector3 a(3.0, 0.0, 0.0);
    FGColumnVector3 b(0.0, 4.0, 0.0);

    FGColumnVector3 c = a * b;
    double expected = a.Magnitude() * b.Magnitude(); // sin(90) = 1
    CHECK(c.Magnitude() == doctest::Approx(expected).epsilon(EPS));

    // Non-orthogonal case
    FGColumnVector3 d(1.0, 1.0, 0.0);
    FGColumnVector3 e(1.0, 0.0, 0.0);
    FGColumnVector3 f = d * e;
    double cos_angle = DotProduct(d, e) / (d.Magnitude() * e.Magnitude());
    double sin_angle = std::sqrt(1.0 - cos_angle * cos_angle);
    double expected2 = d.Magnitude() * e.Magnitude() * sin_angle;
    CHECK(f.Magnitude() == doctest::Approx(expected2).epsilon(EPS));
}

// --- Test 18: Scalar triple product — a . (b x c) = det([a b c]) ---
// This identity is used in volume calculations and wind-axis transforms.
TEST_CASE("scalar triple product equals determinant")
{
    FGColumnVector3 a(1.0, 4.0, -2.0);
    FGColumnVector3 b(3.0, -1.0, 0.5);
    FGColumnVector3 c(-2.0, 5.0, 3.0);

    double tripleProduct = DotProduct(a, b * c);

    // Build matrix with a,b,c as rows and compute determinant
    FGMatrix33 M(a(1), a(2), a(3),
                 b(1), b(2), b(3),
                 c(1), c(2), c(3));

    CHECK(tripleProduct == doctest::Approx(M.Determinant()).epsilon(EPS));
}

// --- Test 19: NaN propagation — any NaN input must propagate, never silently vanish ---
// A sensor returning NaN (e.g., GPS dropout) must not be masked by matrix ops.
TEST_CASE("NaN propagation through arithmetic")
{
    double nan = std::numeric_limits<double>::quiet_NaN();

    FGColumnVector3 v(nan, 1.0, 2.0);
    FGColumnVector3 w(1.0, 2.0, 3.0);

    FGColumnVector3 sum = v + w;
    CHECK(std::isnan(sum(1)));  // NaN must survive addition

    double dot = DotProduct(v, w);
    CHECK(std::isnan(dot));     // NaN must survive dot product

    FGColumnVector3 cross = v * w;
    // At least one component of the cross product must be NaN
    bool anyNan = std::isnan(cross(1)) || std::isnan(cross(2)) || std::isnan(cross(3));
    CHECK(anyNan);

    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGColumnVector3 mv = M * v;
    CHECK(std::isnan(mv(1)));   // NaN must survive matrix-vector multiply
}

// --- Test 20: Inf propagation ---
// Overflow (e.g., integrator blowup) produces Inf; it must not become finite silently.
TEST_CASE("Inf propagation through arithmetic")
{
    double inf = std::numeric_limits<double>::infinity();

    FGColumnVector3 v(inf, 0.0, 0.0);
    FGColumnVector3 w(1.0, 1.0, 1.0);

    CHECK(std::isinf((v + w)(1)));
    CHECK(std::isinf(DotProduct(v, w)));
    CHECK(std::isinf(v.Magnitude()));

    FGColumnVector3 scaled = v * 2.0;
    CHECK(std::isinf(scaled(1)));
}

// --- Test 21: (A*B)^T = B^T * A^T ---
// Transpose reversal law; violated implementation would break DCM update equations.
TEST_CASE("transpose of product equals product of transposes reversed")
{
    FGMatrix33 A = EulerToMatrix(0.3, -0.5, 1.0);
    FGMatrix33 B = EulerToMatrix(-0.7, 0.2, -0.4);

    FGMatrix33 lhs = (A * B).Transposed();
    FGMatrix33 rhs = B.Transposed() * A.Transposed();

    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            CHECK(lhs(i,j) == doctest::Approx(rhs(i,j)).epsilon(EPS));
}

// --- Test 22: Specific moment calculation — r x F for a wing-mounted engine ---
// A real scenario: engine at r=(2,5,0)m producing thrust F=(1000,0,0)N.
// Moment M = r x F must give correct yaw/pitch torques on the airframe.
TEST_CASE("moment from offset force - wing engine scenario")
{
    FGColumnVector3 r(2.0, 5.0, 0.0);       // engine position in body frame (m)
    FGColumnVector3 F(1000.0, 0.0, 0.0);    // thrust along body X (N)

    FGColumnVector3 M = r * F;  // moment = r x F

    // r x F = (5*0 - 0*0, 0*1000 - 2*0, 2*0 - 5*1000) = (0, 0, -5000)
    CHECK(M(1) == doctest::Approx(0.0).epsilon(EPS));
    CHECK(M(2) == doctest::Approx(0.0).epsilon(EPS));
    CHECK(M(3) == doctest::Approx(-5000.0).epsilon(EPS));
}

} // TEST_SUITE

// ═══════════════════════════════════════════════════════════════════════════════
// COVERAGE GAP TESTS — Exercise every remaining untested public API method
// ═══════════════════════════════════════════════════════════════════════════════

TEST_SUITE("CoverageGaps") {

TEST_CASE("Vector: Entry() read/write accessor")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    CHECK(v.Entry(1) == 1.0);
    CHECK(v.Entry(2) == 2.0);
    v.Entry(2) = 99.0;
    CHECK(v(2) == 99.0);
}

TEST_CASE("Vector: operator!=")
{
    FGColumnVector3 a(1.0, 2.0, 3.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    FGColumnVector3 c(1.0, 2.0, 4.0);
    CHECK_FALSE(a != b);
    CHECK(a != c);
}

TEST_CASE("Vector: minus-equals")
{
    FGColumnVector3 a(10.0, 20.0, 30.0);
    FGColumnVector3 b(1.0, 2.0, 3.0);
    a -= b;
    CHECK(a(1) == 9.0);
    CHECK(a(2) == 18.0);
    CHECK(a(3) == 27.0);
}

TEST_CASE("Vector: times-equals")
{
    FGColumnVector3 v(2.0, 3.0, 4.0);
    v *= 5.0;
    CHECK(v(1) == 10.0);
    CHECK(v(2) == 15.0);
    CHECK(v(3) == 20.0);
}

TEST_CASE("Vector: divide-equals and divide-equals by zero")
{
    FGColumnVector3 v(10.0, 20.0, 30.0);
    v /= 5.0;
    CHECK(v(1) == 2.0);
    CHECK(v(2) == 4.0);
    CHECK(v(3) == 6.0);

    FGColumnVector3 w(1.0, 2.0, 3.0);
    CHECK_THROWS_AS(w /= 0.0, std::domain_error);
}

TEST_CASE("Vector: InitMatrix overloads")
{
    FGColumnVector3 v(5.0, 6.0, 7.0);
    v.InitMatrix();
    CHECK(v(1) == 0.0);
    CHECK(v(2) == 0.0);
    CHECK(v(3) == 0.0);

    v.InitMatrix(42.0);
    CHECK(v(1) == 42.0);
    CHECK(v(2) == 42.0);
    CHECK(v(3) == 42.0);

    v.InitMatrix(1.0, 2.0, 3.0);
    CHECK(v(1) == 1.0);
    CHECK(v(2) == 2.0);
    CHECK(v(3) == 3.0);
}

TEST_CASE("Vector: 2-element magnitude")
{
    FGColumnVector3 v(3.0, 4.0, 99.0);
    CHECK(v.Magnitude(1, 2) == 5.0);
    CHECK(v.Magnitude(2, 3) == doctest::Approx(std::sqrt(16.0 + 9801.0)).epsilon(1e-10));
}

TEST_CASE("Vector: Dump")
{
    FGColumnVector3 v(1.5, 2.5, 3.5);
    std::string s = v.Dump(", ");
    CHECK_FALSE(s.empty());
    CHECK(s.find("1.5") != std::string::npos);
    CHECK(s.find("3.5") != std::string::npos);
}

TEST_CASE("Vector: stream output operator<<")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    std::ostringstream os;
    os << v;
    CHECK_FALSE(os.str().empty());
}

TEST_CASE("Vector: scalar left-multiply free function")
{
    FGColumnVector3 v(1.0, 2.0, 3.0);
    FGColumnVector3 r = 3.0 * v;
    CHECK(r(1) == 3.0);
    CHECK(r(2) == 6.0);
    CHECK(r(3) == 9.0);
}

TEST_CASE("Matrix: Entry() read/write accessor")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    CHECK(M.Entry(1,1) == 1.0);
    CHECK(M.Entry(2,3) == 6.0);
    M.Entry(3,3) = 99.0;
    CHECK(M(3,3) == 99.0);
}

TEST_CASE("Matrix: operator!=")
{
    FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 B(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 C(1,0,0, 0,1,0, 0,0,2);
    CHECK_FALSE(A != B);
    CHECK(A != C);
}

TEST_CASE("Matrix: in-place transpose T()")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    FGMatrix33 Mt = M.Transposed();
    M.T();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            CHECK(M(i,j) == Mt(i,j));
}

TEST_CASE("Matrix: Invertible()")
{
    FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
    CHECK(I.Invertible());
    FGMatrix33 singular(1,2,3, 4,5,6, 7,8,9);
    CHECK_FALSE(singular.Invertible());
}

TEST_CASE("Matrix: Rows() and Cols()")
{
    FGMatrix33 M;
    CHECK(M.Rows() == 3u);
    CHECK(M.Cols() == 3u);
}

TEST_CASE("Matrix: InitMatrix overloads")
{
    FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
    M.InitMatrix();
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j)
            CHECK(M(i,j) == 0.0);

    M.InitMatrix(9,8,7, 6,5,4, 3,2,1);
    CHECK(M(1,1) == 9.0);
    CHECK(M(3,3) == 1.0);
}

TEST_CASE("Matrix: compound assignment operators")
{
    SUBCASE("+=") {
        FGMatrix33 A(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 B(1,1,1, 1,1,1, 1,1,1);
        A += B;
        CHECK(A(1,1) == 2.0);
        CHECK(A(1,2) == 1.0);
    }
    SUBCASE("-=") {
        FGMatrix33 A(5,5,5, 5,5,5, 5,5,5);
        FGMatrix33 B(1,2,3, 4,5,6, 7,8,9);
        A -= B;
        CHECK(A(1,1) == 4.0);
        CHECK(A(2,2) == 0.0);
    }
    SUBCASE("*= scalar") {
        FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
        M *= 3.0;
        CHECK(M(1,1) == 3.0);
        CHECK(M(1,2) == 0.0);
    }
    SUBCASE("/= scalar") {
        FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
        M /= 10.0;
        CHECK(M(1,1) == 1.0);
        CHECK(M(3,3) == 9.0);
    }
    SUBCASE("*= matrix") {
        FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 expected = A * I;
        A *= I;
        CHECK(A == expected);
    }
}

TEST_CASE("Matrix: scalar divide and divide-by-zero")
{
    FGMatrix33 M(10,20,30, 40,50,60, 70,80,90);
    FGMatrix33 R = M / 10.0;
    CHECK(R(1,1) == 1.0);
    CHECK_THROWS_AS(M / 0.0, std::domain_error);
    CHECK_THROWS_AS(M /= 0.0, std::domain_error);
}

TEST_CASE("Matrix: Dump and stream output")
{
    FGMatrix33 M(1.5, 0, 0, 0, 2.5, 0, 0, 0, 3.5);
    std::string s = M.Dump(", ");
    CHECK_FALSE(s.empty());
    CHECK(s.find("1.5") != std::string::npos);

    std::ostringstream os;
    os << M;
    CHECK_FALSE(os.str().empty());
}

TEST_CASE("Matrix: scalar left-multiply free function")
{
    FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
    FGMatrix33 R = 7.0 * M;
    CHECK(R(1,1) == 7.0);
    CHECK(R(2,2) == 7.0);
    CHECK(R(1,2) == 0.0);
}

} // TEST_SUITE CoverageGaps
