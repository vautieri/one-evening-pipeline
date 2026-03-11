/*==============================================================================
 CppUnit suite for JSBSim math library
 Framework #5 of 5 — https://freedesktop.org/wiki/Software/cppunit/
==============================================================================*/

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cmath>
#include <limits>
#include "FGColumnVector3.h"
#include "FGMatrix33.h"

using namespace JSBSim;

static constexpr double EPS = 1e-10;

// ═══════════════════════════════════════════════════════════════════════════════
// FGColumnVector3
// ═══════════════════════════════════════════════════════════════════════════════

class ColumnVector3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(ColumnVector3Test);
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testParameterizedConstructor);
    CPPUNIT_TEST(testCopyConstructor);
    CPPUNIT_TEST(testAddition);
    CPPUNIT_TEST(testSubtraction);
    CPPUNIT_TEST(testScalarMultiply);
    CPPUNIT_TEST(testScalarDivide);
    CPPUNIT_TEST(testDivisionByZeroThrows);
    CPPUNIT_TEST(testCrossProduct);
    CPPUNIT_TEST(testDotProduct);
    CPPUNIT_TEST(testMagnitude);
    CPPUNIT_TEST(testNormalize);
    CPPUNIT_TEST(testNormalizeZeroVector);
    CPPUNIT_TEST(testIndexOutOfRange);
    CPPUNIT_TEST(testUnaryNegation);
    CPPUNIT_TEST(testPlusEquals);
    CPPUNIT_TEST(testInitializerList);
    CPPUNIT_TEST_SUITE_END();

public:
    void testDefaultConstructor()
    {
        FGColumnVector3 v;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, v(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, v(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, v(3), EPS);
    }

    void testParameterizedConstructor()
    {
        FGColumnVector3 v(1.0, 2.0, 3.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, v(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, v(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, v(3), EPS);
    }

    void testCopyConstructor()
    {
        FGColumnVector3 a(4.0, 5.0, 6.0);
        FGColumnVector3 b(a);
        CPPUNIT_ASSERT(a == b);
    }

    void testAddition()
    {
        FGColumnVector3 a(1.0, 2.0, 3.0);
        FGColumnVector3 b(4.0, 5.0, 6.0);
        FGColumnVector3 c = a + b;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, c(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, c(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, c(3), EPS);
    }

    void testSubtraction()
    {
        FGColumnVector3 a(10.0, 20.0, 30.0);
        FGColumnVector3 b(1.0, 2.0, 3.0);
        FGColumnVector3 c = a - b;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0,  c(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(18.0, c(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(27.0, c(3), EPS);
    }

    void testScalarMultiply()
    {
        FGColumnVector3 v(1.0, 2.0, 3.0);
        FGColumnVector3 r = v * 3.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, r(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, r(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, r(3), EPS);
    }

    void testScalarDivide()
    {
        FGColumnVector3 v(4.0, 8.0, 12.0);
        FGColumnVector3 r = v / 4.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, r(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, r(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, r(3), EPS);
    }

    void testDivisionByZeroThrows()
    {
        FGColumnVector3 v(1.0, 2.0, 3.0);
        CPPUNIT_ASSERT_THROW(v / 0.0, std::domain_error);
    }

    void testCrossProduct()
    {
        FGColumnVector3 i(1.0, 0.0, 0.0);
        FGColumnVector3 j(0.0, 1.0, 0.0);
        FGColumnVector3 k = i * j;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, k(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, k(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, k(3), EPS);
    }

    void testDotProduct()
    {
        FGColumnVector3 a(1.0, 2.0, 3.0);
        FGColumnVector3 b(4.0, 5.0, 6.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(32.0, JSBSim::DotProduct(a, b), EPS);
    }

    void testMagnitude()
    {
        FGColumnVector3 v(3.0, 4.0, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, v.Magnitude(), EPS);
    }

    void testNormalize()
    {
        FGColumnVector3 v(0.0, 0.0, 5.0);
        v.Normalize();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, v.Magnitude(), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, v(3), EPS);
    }

    void testNormalizeZeroVector()
    {
        FGColumnVector3 v;
        v.Normalize();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, v.Magnitude(), EPS);
    }

    void testIndexOutOfRange()
    {
        FGColumnVector3 v;
        CPPUNIT_ASSERT_THROW(v(0), std::range_error);
        CPPUNIT_ASSERT_THROW(v(4), std::range_error);
    }

    void testUnaryNegation()
    {
        FGColumnVector3 v(1.0, -2.0, 3.0);
        FGColumnVector3 neg = -v;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, neg(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, neg(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.0, neg(3), EPS);
    }

    void testPlusEquals()
    {
        FGColumnVector3 a(1.0, 2.0, 3.0);
        FGColumnVector3 b(10.0, 20.0, 30.0);
        a += b;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(11.0, a(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(22.0, a(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(33.0, a(3), EPS);
    }

    void testInitializerList()
    {
        FGColumnVector3 v;
        v = {7.0, 8.0, 9.0};
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, v(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, v(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, v(3), EPS);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(ColumnVector3Test);

// ═══════════════════════════════════════════════════════════════════════════════
// FGMatrix33
// ═══════════════════════════════════════════════════════════════════════════════

class Matrix33Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Matrix33Test);
    CPPUNIT_TEST(testDefaultConstructor);
    CPPUNIT_TEST(testIdentityDeterminant);
    CPPUNIT_TEST(testTranspose);
    CPPUNIT_TEST(testInverseOfIdentity);
    CPPUNIT_TEST(testInverseMultiply);
    CPPUNIT_TEST(testSingularInverseThrows);
    CPPUNIT_TEST(testMatrixVectorMultiply);
    CPPUNIT_TEST(testMatrixMultiplication);
    CPPUNIT_TEST(testScalarMultiply);
    CPPUNIT_TEST(testEulerRoundTrip);
    CPPUNIT_TEST(testIndexOutOfRange);
    CPPUNIT_TEST_SUITE_END();

public:
    void testDefaultConstructor()
    {
        FGMatrix33 M;
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, M(i,j), EPS);
    }

    void testIdentityDeterminant()
    {
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, I.Determinant(), EPS);
    }

    void testTranspose()
    {
        FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
        FGMatrix33 Mt = M.Transposed();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, Mt(1,2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, Mt(2,1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, Mt(3,1), EPS);
    }

    void testInverseOfIdentity()
    {
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 Iinv = I.Inverse();
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(I(i,j), Iinv(i,j), EPS);
    }

    void testInverseMultiply()
    {
        FGMatrix33 M(2,1,0, 0,3,1, 1,0,2);
        FGMatrix33 Minv = M.Inverse();
        FGMatrix33 product = M * Minv;
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j) {
                double expected = (i == j) ? 1.0 : 0.0;
                CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, product(i,j), EPS);
            }
    }

    void testSingularInverseThrows()
    {
        FGMatrix33 M(1,2,3, 4,5,6, 7,8,9);
        CPPUNIT_ASSERT_THROW(M.Inverse(), std::domain_error);
    }

    void testMatrixVectorMultiply()
    {
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        FGColumnVector3 v(7.0, 8.0, 9.0);
        FGColumnVector3 result = I * v;
        CPPUNIT_ASSERT(result == v);
    }

    void testMatrixMultiplication()
    {
        FGMatrix33 A(1,2,3, 4,5,6, 7,8,9);
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 result = A * I;
        CPPUNIT_ASSERT(result == A);
    }

    void testScalarMultiply()
    {
        FGMatrix33 M(1,0,0, 0,1,0, 0,0,1);
        FGMatrix33 scaled = M * 5.0;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, scaled(1,1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, scaled(2,2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, scaled(3,3), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, scaled(1,2), EPS);
    }

    void testEulerRoundTrip()
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
        CPPUNIT_ASSERT_DOUBLES_EQUAL(phi,   euler(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(theta, euler(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(psi,   euler(3), EPS);
    }

    void testIndexOutOfRange()
    {
        FGMatrix33 M;
        CPPUNIT_ASSERT_THROW(M(0, 1), std::range_error);
        CPPUNIT_ASSERT_THROW(M(1, 4), std::range_error);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(Matrix33Test);

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

class AdvancedAerospaceTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AdvancedAerospaceTest);
    CPPUNIT_TEST(testGimbalLockPositive90);
    CPPUNIT_TEST(testGimbalLockNegative90);
    CPPUNIT_TEST(testRotationCompositionSameAxis);
    CPPUNIT_TEST(testDCMOrthogonality);
    CPPUNIT_TEST(testRotationDeterminantIsOne);
    CPPUNIT_TEST(testCrossProductAnticommutativity);
    CPPUNIT_TEST(testJacobiIdentity);
    CPPUNIT_TEST(testBodyEarthRoundTrip);
    CPPUNIT_TEST(testRotationPreservesVectorLength);
    CPPUNIT_TEST(testMatrixVectorDistributivity);
    CPPUNIT_TEST(testMatrixAssociativity);
    CPPUNIT_TEST(testNaNPropagation);
    CPPUNIT_TEST(testInfPropagation);
    CPPUNIT_TEST_SUITE_END();

public:
    // --- Test 1: Gimbal lock at theta = +90 deg ---
    // Euler extraction degenerates when pitch = +/-90 deg; psi is set to 0 by convention.
    // Flight sims must handle this without NaN or crash during aggressive pitch maneuvers.
    void testGimbalLockPositive90()
    {
        double phi = 0.4;
        double theta = M_PI / 2.0;   // exactly +90 deg
        double psi = 0.6;

        FGMatrix33 R = EulerToMatrix(phi, theta, psi);
        FGColumnVector3 euler = R.GetEuler();

        // theta must be pi/2
        CPPUNIT_ASSERT_DOUBLES_EQUAL(M_PI / 2.0, euler(2), EPS);
        // in gimbal lock psi is forced to 0; phi absorbs the combined rotation
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, euler(3), EPS);
        // Rebuild from extracted angles and verify the matrix matches
        FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(R(i,j), R2(i,j), LOOSE_EPS);
    }

    // --- Test 2: Gimbal lock at theta = -90 deg ---
    // Symmetric case: nose-straight-down causes identical loss of yaw/roll separation.
    void testGimbalLockNegative90()
    {
        double phi = -0.3;
        double theta = -M_PI / 2.0;
        double psi = 0.7;

        FGMatrix33 R = EulerToMatrix(phi, theta, psi);
        FGColumnVector3 euler = R.GetEuler();

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-M_PI / 2.0, euler(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, euler(3), EPS);
        FGMatrix33 R2 = EulerToMatrix(euler(1), euler(2), euler(3));
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(R(i,j), R2(i,j), LOOSE_EPS);
    }

    // --- Test 3: Rotation composition — R(a)*R(b) vs R(a+b) around same axis ---
    // Composing two rotations around Z must equal a single rotation of summed angle.
    void testRotationCompositionSameAxis()
    {
        double a = 0.3, b = 0.7;
        FGMatrix33 Ra = EulerToMatrix(0, 0, a);
        FGMatrix33 Rb = EulerToMatrix(0, 0, b);
        FGMatrix33 Rab = EulerToMatrix(0, 0, a + b);

        FGMatrix33 product = Ra * Rb;
        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(Rab(i,j), product(i,j), EPS);
    }

    // --- Test 4: DCM orthogonality — R^T * R = I for any rotation matrix ---
    void testDCMOrthogonality()
    {
        FGMatrix33 R = EulerToMatrix(0.5, -0.3, 1.2);
        FGMatrix33 RtR = R.Transposed() * R;
        FGMatrix33 I(1,0,0, 0,1,0, 0,0,1);

        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(I(i,j), RtR(i,j), EPS);
    }

    // --- Test 5: Rotation matrix determinant = +1 ---
    void testRotationDeterminantIsOne()
    {
        FGMatrix33 R = EulerToMatrix(1.1, -0.4, 2.3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, R.Determinant(), EPS);
    }

    // --- Test 6: Cross product anti-commutativity — a x b = -(b x a) ---
    void testCrossProductAnticommutativity()
    {
        FGColumnVector3 a(1.5, -2.3, 0.7);
        FGColumnVector3 b(-0.4, 3.1, -1.8);

        FGColumnVector3 axb = a * b;
        FGColumnVector3 bxa = b * a;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(-bxa(1), axb(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-bxa(2), axb(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-bxa(3), axb(3), EPS);
    }

    // --- Test 7: Jacobi identity — a x (b x c) + b x (c x a) + c x (a x b) = 0 ---
    void testJacobiIdentity()
    {
        FGColumnVector3 a(1.0, 2.0, 3.0);
        FGColumnVector3 b(-1.0, 0.5, 2.0);
        FGColumnVector3 c(0.3, -1.5, 0.8);

        FGColumnVector3 result = a * (b * c) + b * (c * a) + c * (a * b);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result(3), EPS);
    }

    // --- Test 8: Body-earth round trip — transform velocity body->earth->body ---
    void testBodyEarthRoundTrip()
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

        CPPUNIT_ASSERT_DOUBLES_EQUAL(v_body(1), v_recovered(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(v_body(2), v_recovered(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(v_body(3), v_recovered(3), EPS);
    }

    // --- Test 9: Rotation preserves vector length — ||R*v|| = ||v|| ---
    void testRotationPreservesVectorLength()
    {
        FGMatrix33 R = EulerToMatrix(0.5, -0.3, 1.2);
        FGColumnVector3 v(100.0, 200.0, 300.0);
        FGColumnVector3 Rv = R * v;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(v.Magnitude(), Rv.Magnitude(), EPS);
    }

    // --- Test 10: Matrix-vector distributivity — M*(a+b) = M*a + M*b ---
    void testMatrixVectorDistributivity()
    {
        FGMatrix33 M(2, 1, 0, 0, 3, 1, 1, 0, 2);
        FGColumnVector3 a(1.0, 2.0, 3.0);
        FGColumnVector3 b(4.0, -1.0, 0.5);

        FGColumnVector3 lhs = M * (a + b);
        FGColumnVector3 rhs = M * a + M * b;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(rhs(1), lhs(1), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(rhs(2), lhs(2), EPS);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(rhs(3), lhs(3), EPS);
    }

    // --- Test 11: Matrix associativity — (A*B)*C = A*(B*C) ---
    void testMatrixAssociativity()
    {
        FGMatrix33 A(2, 1, 0, 0, 3, 1, 1, 0, 2);
        FGMatrix33 B(1, 2, -1, 3, 0, 1, -1, 1, 2);
        FGMatrix33 C(0, 1, 3, 2, -1, 0, 1, 2, -1);

        FGMatrix33 lhs = (A * B) * C;
        FGMatrix33 rhs = A * (B * C);

        for (unsigned int i = 1; i <= 3; ++i)
            for (unsigned int j = 1; j <= 3; ++j)
                CPPUNIT_ASSERT_DOUBLES_EQUAL(rhs(i,j), lhs(i,j), EPS);
    }

    // --- Test 12: NaN propagation ---
    void testNaNPropagation()
    {
        double nan = std::numeric_limits<double>::quiet_NaN();
        FGColumnVector3 v(nan, 1.0, 2.0);
        FGColumnVector3 w(3.0, 4.0, 5.0);

        FGColumnVector3 sum = v + w;
        CPPUNIT_ASSERT(std::isnan(sum(1)));

        FGColumnVector3 cross = v * w;
        // NaN in component 1 of v propagates into cross product components
        CPPUNIT_ASSERT(std::isnan(cross(2)) || std::isnan(cross(3)));

        FGMatrix33 M(nan, 0, 0, 0, 1, 0, 0, 0, 1);
        CPPUNIT_ASSERT(std::isnan(M.Determinant()));
    }

    // --- Test 13: Inf propagation ---
    void testInfPropagation()
    {
        double inf = std::numeric_limits<double>::infinity();
        FGColumnVector3 v(inf, 1.0, 2.0);
        FGColumnVector3 w(3.0, 4.0, 5.0);

        FGColumnVector3 sum = v + w;
        CPPUNIT_ASSERT(std::isinf(sum(1)));

        FGColumnVector3 scaled = v * 2.0;
        CPPUNIT_ASSERT(std::isinf(scaled(1)));

        FGMatrix33 M(inf, 0, 0, 0, 1, 0, 0, 0, 1);
        CPPUNIT_ASSERT(std::isinf(M.Determinant()));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(AdvancedAerospaceTest);

// ═══════════════════════════════════════════════════════════════════════════════
// Main runner
// ═══════════════════════════════════════════════════════════════════════════════

int main()
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    return runner.run() ? 0 : 1;
}
