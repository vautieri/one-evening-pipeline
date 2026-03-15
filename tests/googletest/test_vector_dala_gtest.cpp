/*==============================================================================
 * TAR-SF-001 | DAL A Test Suite for FGColumnVector3
 *
 * Requirements Verified:
 *   REQ-SF-0200  Vector element access and boundary enforcement
 *   REQ-SF-0103  Arithmetic operator correctness and special-value handling
 *   REQ-SF-1300  Vector normalization and magnitude computation
 *   REQ-SF-1400  Error handling for invalid operations
 *   REQ-SF-1401  Robustness under IEEE 754 special values
 *
 * Coverage Targets: 100% Statement, Decision, and MC/DC (DAL A)
 * Standard: SCS-SF-2026-001
 * Date: 2026-03-14
 *============================================================================*/

#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <sstream>
#include <string>
#include <stdexcept>
#include "FGColumnVector3.h"

using namespace JSBSim;

// --- Named constants (SCS-SF-2026-001: no magic numbers) ---------------------

static constexpr double TOLERANCE       = 1e-10;
static constexpr double LARGE_VALUE     = 1e308;
static constexpr double SMALL_VALUE     = 1e-308;
static constexpr double ONE             = 1.0;
static constexpr double TWO             = 2.0;
static constexpr double THREE           = 3.0;
static constexpr double FOUR            = 4.0;
static constexpr double FIVE            = 5.0;
static constexpr double SIX             = 6.0;
static constexpr double SEVEN           = 7.0;
static constexpr double EIGHT           = 8.0;
static constexpr double NINE            = 9.0;
static constexpr double TEN             = 10.0;
static constexpr double ZERO            = 0.0;
static constexpr unsigned int IDX_BELOW = 0u;
static constexpr unsigned int IDX_LOWER = 1u;
static constexpr unsigned int IDX_MID   = 2u;
static constexpr unsigned int IDX_UPPER = 3u;
static constexpr unsigned int IDX_ABOVE = 4u;

// =============================================================================
// a. Boundary Value Tests
// =============================================================================

TEST(VectorDalA_BoundaryValue, ConstAccessIndex1_ReturnsFirstElement)
{
    // Verifies REQ-SF-0200 | Boundary: lower valid index (const overload)
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), ONE);
}

TEST(VectorDalA_BoundaryValue, ConstAccessIndex3_ReturnsThirdElement)
{
    // Verifies REQ-SF-0200 | Boundary: upper valid index (const overload)
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), THREE);
}

TEST(VectorDalA_BoundaryValue, MutableAccessIndex1_CanModifyFirstElement)
{
    // Verifies REQ-SF-0200 | Boundary: lower valid index, non-const overload
    FGColumnVector3 v(ONE, TWO, THREE);
    v(IDX_LOWER) = TEN;
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), TEN);
}

TEST(VectorDalA_BoundaryValue, MutableAccessIndex3_CanModifyThirdElement)
{
    // Verifies REQ-SF-0200 | Boundary: upper valid index, non-const overload
    FGColumnVector3 v(ONE, TWO, THREE);
    v(IDX_UPPER) = TEN;
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), TEN);
}

TEST(VectorDalA_BoundaryValue, ConstEntryIndex1_DelegatesToOperator)
{
    // Verifies REQ-SF-0200 | Entry() const delegates to operator() const
    const FGColumnVector3 v(FOUR, FIVE, SIX);
    EXPECT_DOUBLE_EQ(v.Entry(IDX_LOWER), FOUR);
}

TEST(VectorDalA_BoundaryValue, ConstEntryIndex3_DelegatesToOperator)
{
    // Verifies REQ-SF-0200 | Entry() const at upper boundary
    const FGColumnVector3 v(FOUR, FIVE, SIX);
    EXPECT_DOUBLE_EQ(v.Entry(IDX_UPPER), SIX);
}

TEST(VectorDalA_BoundaryValue, MutableEntryIndex2_CanModifyElement)
{
    // Verifies REQ-SF-0200 | Entry() non-const delegates to operator() non-const
    FGColumnVector3 v(ONE, TWO, THREE);
    v.Entry(IDX_MID) = NINE;
    EXPECT_DOUBLE_EQ(v(IDX_MID), NINE);
}

TEST(VectorDalA_BoundaryValue, AdditionWithZeroVector_Identity)
{
    // Verifies REQ-SF-0103 | Boundary: zero is additive identity
    const FGColumnVector3 v(ONE, TWO, THREE);
    const FGColumnVector3 zero;
    const FGColumnVector3 result = v + zero;
    EXPECT_DOUBLE_EQ(result(IDX_LOWER), ONE);
    EXPECT_DOUBLE_EQ(result(IDX_MID), TWO);
    EXPECT_DOUBLE_EQ(result(IDX_UPPER), THREE);
}

TEST(VectorDalA_BoundaryValue, SubtractionOfSelf_YieldsZero)
{
    // Verifies REQ-SF-0103 | Boundary: v - v == zero vector
    const FGColumnVector3 v(ONE, TWO, THREE);
    const FGColumnVector3 result = v - v;
    EXPECT_DOUBLE_EQ(result(IDX_LOWER), ZERO);
    EXPECT_DOUBLE_EQ(result(IDX_MID), ZERO);
    EXPECT_DOUBLE_EQ(result(IDX_UPPER), ZERO);
}

TEST(VectorDalA_BoundaryValue, CrossProductWithZeroVector_YieldsZero)
{
    // Verifies REQ-SF-0103 | Boundary: cross product with zero is zero
    const FGColumnVector3 v(ONE, TWO, THREE);
    const FGColumnVector3 zero;
    const FGColumnVector3 result = v * zero;
    EXPECT_NEAR(result(IDX_LOWER), ZERO, TOLERANCE);
    EXPECT_NEAR(result(IDX_MID), ZERO, TOLERANCE);
    EXPECT_NEAR(result(IDX_UPPER), ZERO, TOLERANCE);
}

TEST(VectorDalA_BoundaryValue, LargeValueConstruction_Preserved)
{
    // Verifies REQ-SF-0200 | Boundary: near-max double values stored correctly
    const FGColumnVector3 v(LARGE_VALUE, -LARGE_VALUE, LARGE_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), LARGE_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), -LARGE_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), LARGE_VALUE);
}

TEST(VectorDalA_BoundaryValue, SmallValueConstruction_Preserved)
{
    // Verifies REQ-SF-0200 | Boundary: near-min positive double values stored correctly
    const FGColumnVector3 v(SMALL_VALUE, SMALL_VALUE, SMALL_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), SMALL_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), SMALL_VALUE);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), SMALL_VALUE);
}

TEST(VectorDalA_BoundaryValue, NegativeZeroEqualsPositiveZero)
{
    // Verifies REQ-SF-0200 | Boundary: IEEE 754 negative zero comparison
    const FGColumnVector3 pos(ZERO, ZERO, ZERO);
    const FGColumnVector3 neg(-ZERO, -ZERO, -ZERO);
    EXPECT_TRUE(pos == neg);
}

TEST(VectorDalA_BoundaryValue, ScalarMultiplyByZero_YieldsZeroVector)
{
    // Verifies REQ-SF-0103 | Boundary: scalar zero annihilates vector
    const FGColumnVector3 v(ONE, TWO, THREE);
    const FGColumnVector3 result = v * ZERO;
    EXPECT_DOUBLE_EQ(result(IDX_LOWER), ZERO);
    EXPECT_DOUBLE_EQ(result(IDX_MID), ZERO);
    EXPECT_DOUBLE_EQ(result(IDX_UPPER), ZERO);
}

TEST(VectorDalA_BoundaryValue, ScalarMultiplyByOne_Identity)
{
    // Verifies REQ-SF-0103 | Boundary: scalar one is multiplicative identity
    const FGColumnVector3 v(FOUR, FIVE, SIX);
    const FGColumnVector3 result = v * ONE;
    EXPECT_DOUBLE_EQ(result(IDX_LOWER), FOUR);
    EXPECT_DOUBLE_EQ(result(IDX_MID), FIVE);
    EXPECT_DOUBLE_EQ(result(IDX_UPPER), SIX);
}

// =============================================================================
// b. Robustness Tests
// =============================================================================

TEST(VectorDalA_Robustness, NaNConstruction_PropagatesThroughAccess)
{
    // Verifies REQ-SF-1401 | NaN propagation on construction and access
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const FGColumnVector3 v(nan_val, ONE, TWO);
    EXPECT_TRUE(std::isnan(v(IDX_LOWER)));
}

TEST(VectorDalA_Robustness, NaNPropagation_Addition)
{
    // Verifies REQ-SF-1401 | NaN propagation through vector addition
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const FGColumnVector3 a(nan_val, ZERO, ZERO);
    const FGColumnVector3 b(ONE, ONE, ONE);
    const FGColumnVector3 result = a + b;
    EXPECT_TRUE(std::isnan(result(IDX_LOWER)));
}

TEST(VectorDalA_Robustness, NaNPropagation_Magnitude)
{
    // Verifies REQ-SF-1401 | NaN propagation through magnitude computation
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const FGColumnVector3 v(nan_val, ONE, ONE);
    EXPECT_TRUE(std::isnan(v.Magnitude()));
}

TEST(VectorDalA_Robustness, NaNPropagation_DotProduct)
{
    // Verifies REQ-SF-1401 | NaN propagation through dot product
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const FGColumnVector3 a(nan_val, ONE, ONE);
    const FGColumnVector3 b(ONE, ONE, ONE);
    EXPECT_TRUE(std::isnan(DotProduct(a, b)));
}

TEST(VectorDalA_Robustness, InfinityConstruction_Preserved)
{
    // Verifies REQ-SF-1401 | Positive infinity stored and retrievable
    const double inf = std::numeric_limits<double>::infinity();
    const FGColumnVector3 v(inf, ZERO, ZERO);
    EXPECT_TRUE(std::isinf(v(IDX_LOWER)));
    EXPECT_GT(v(IDX_LOWER), ZERO);
}

TEST(VectorDalA_Robustness, NegativeInfinityConstruction_Preserved)
{
    // Verifies REQ-SF-1401 | Negative infinity stored and retrievable
    const double neg_inf = -std::numeric_limits<double>::infinity();
    const FGColumnVector3 v(neg_inf, ZERO, ZERO);
    EXPECT_TRUE(std::isinf(v(IDX_LOWER)));
    EXPECT_LT(v(IDX_LOWER), ZERO);
}

TEST(VectorDalA_Robustness, InfinityArithmetic_Addition)
{
    // Verifies REQ-SF-1401 | Infinity propagation through addition
    const double inf = std::numeric_limits<double>::infinity();
    const FGColumnVector3 a(inf, ZERO, ZERO);
    const FGColumnVector3 b(ONE, ONE, ONE);
    const FGColumnVector3 result = a + b;
    EXPECT_TRUE(std::isinf(result(IDX_LOWER)));
}

TEST(VectorDalA_Robustness, DenormalValue_Preserved)
{
    // Verifies REQ-SF-1401 | Denormal (subnormal) value stored correctly
    const double denorm = std::numeric_limits<double>::denorm_min();
    const FGColumnVector3 v(denorm, denorm, denorm);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), denorm);
    EXPECT_DOUBLE_EQ(v(IDX_MID), denorm);
}

TEST(VectorDalA_Robustness, MagnitudeOfLargeVector_IsInfinity)
{
    // Verifies REQ-SF-1401 | Magnitude overflow to infinity for large components
    const FGColumnVector3 v(LARGE_VALUE, LARGE_VALUE, LARGE_VALUE);
    const double mag = v.Magnitude();
    EXPECT_TRUE(std::isinf(mag));
}

TEST(VectorDalA_Robustness, MagnitudeOfSmallVector_IsNonNegative)
{
    // Verifies REQ-SF-1401 | Magnitude of very small vector remains non-negative
    const FGColumnVector3 v(SMALL_VALUE, SMALL_VALUE, SMALL_VALUE);
    const double mag = v.Magnitude();
    EXPECT_GE(mag, ZERO);
}

TEST(VectorDalA_Robustness, DivisionBySmallScalar_LargeResult)
{
    // Verifies REQ-SF-0103 | Division by near-zero but non-zero scalar
    // 1.0 / 1e-300 = 1e300 — large but finite, below 1e308
    static constexpr double NEAR_ZERO_POSITIVE = 1e-300;
    static constexpr double EXPECTED_ORDER = 1e300;
    const FGColumnVector3 v(ONE, ONE, ONE);
    const FGColumnVector3 result = v / NEAR_ZERO_POSITIVE;
    EXPECT_NEAR(result(IDX_LOWER), EXPECTED_ORDER, EXPECTED_ORDER * 1e-10);
    EXPECT_TRUE(std::isfinite(result(IDX_LOWER)));
}

TEST(VectorDalA_Robustness, CrossProductOfParallelVectors_YieldsZero)
{
    // Verifies REQ-SF-0103 | Parallel vectors produce zero cross product
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(TWO, FOUR, SIX);
    const FGColumnVector3 result = a * b;
    EXPECT_NEAR(result(IDX_LOWER), ZERO, TOLERANCE);
    EXPECT_NEAR(result(IDX_MID), ZERO, TOLERANCE);
    EXPECT_NEAR(result(IDX_UPPER), ZERO, TOLERANCE);
}

TEST(VectorDalA_Robustness, DotProductOfOrthogonalVectors_IsZero)
{
    // Verifies REQ-SF-0103 | Orthogonal vectors produce zero dot product
    const FGColumnVector3 a(ONE, ZERO, ZERO);
    const FGColumnVector3 b(ZERO, ONE, ZERO);
    EXPECT_NEAR(DotProduct(a, b), ZERO, TOLERANCE);
}

TEST(VectorDalA_Robustness, NormalizeNearZeroVector_UnderflowSkipsNormalization)
{
    // Verifies REQ-SF-1300 | Normalize of denorm-range vector:
    // (1e-308)^2 = 1e-616 underflows to 0.0 in IEEE 754, so Magnitude()
    // returns 0.0, and Normalize() correctly takes the mag==0 branch.
    // Vector is unchanged. This documents the numerical limitation.
    FGColumnVector3 v(SMALL_VALUE, ZERO, ZERO);
    const FGColumnVector3 original(v);
    v.Normalize();
    EXPECT_EQ(v, original);  // unchanged because magnitude underflowed to 0
}

TEST(VectorDalA_Robustness, NormalizeModeratelySmallVector_ProducesUnitMagnitude)
{
    // Verifies REQ-SF-1300 | Normalize of small but squarable vector
    static constexpr double MODERATE_SMALL = 1e-150;
    FGColumnVector3 v(MODERATE_SMALL, ZERO, ZERO);
    v.Normalize();
    EXPECT_NEAR(v.Magnitude(), ONE, TOLERANCE);
}

TEST(VectorDalA_Robustness, NaNComparison_NotEqual)
{
    // Verifies REQ-SF-1401 | NaN != NaN per IEEE 754 in operator==
    const double nan_val = std::numeric_limits<double>::quiet_NaN();
    const FGColumnVector3 a(nan_val, ZERO, ZERO);
    const FGColumnVector3 b(nan_val, ZERO, ZERO);
    EXPECT_FALSE(a == b);
}

// =============================================================================
// c. MC/DC Tests
// =============================================================================

TEST(VectorDalA_MCDC, ConstAccess_IndexBelowRange_FirstConditionTrue)
{
    // Verifies REQ-SF-0200 | MC/DC: decision (idx < 1 || idx > 3)
    //   condition A (idx < 1) = TRUE, condition B (idx > 3) = FALSE
    //   decision = TRUE (throws)
    //   Shows condition A independently affects outcome
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_BELOW), std::range_error);
}

TEST(VectorDalA_MCDC, ConstAccess_IndexAboveRange_SecondConditionTrue)
{
    // Verifies REQ-SF-0200 | MC/DC: decision (idx < 1 || idx > 3)
    //   condition A (idx < 1) = FALSE, condition B (idx > 3) = TRUE
    //   decision = TRUE (throws)
    //   Shows condition B independently affects outcome
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_ABOVE), std::range_error);
}

TEST(VectorDalA_MCDC, ConstAccess_IndexInRange_BothConditionsFalse)
{
    // Verifies REQ-SF-0200 | MC/DC: decision (idx < 1 || idx > 3)
    //   condition A (idx < 1) = FALSE, condition B (idx > 3) = FALSE
    //   decision = FALSE (no throw, returns value)
    //   Baseline case completing MC/DC triple
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_NO_THROW(v(IDX_MID));
    EXPECT_DOUBLE_EQ(v(IDX_MID), TWO);
}

TEST(VectorDalA_MCDC, MutableAccess_IndexBelowRange_FirstConditionTrue)
{
    // Verifies REQ-SF-0200 | MC/DC: non-const operator() decision
    //   condition A (idx < 1) = TRUE, condition B (idx > 3) = FALSE
    //   decision = TRUE (throws)
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_BELOW), std::range_error);
}

TEST(VectorDalA_MCDC, MutableAccess_IndexAboveRange_SecondConditionTrue)
{
    // Verifies REQ-SF-0200 | MC/DC: non-const operator() decision
    //   condition A (idx < 1) = FALSE, condition B (idx > 3) = TRUE
    //   decision = TRUE (throws)
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_ABOVE), std::range_error);
}

TEST(VectorDalA_MCDC, MutableAccess_IndexInRange_BothConditionsFalse)
{
    // Verifies REQ-SF-0200 | MC/DC: non-const operator() decision
    //   condition A (idx < 1) = FALSE, condition B (idx > 3) = FALSE
    //   decision = FALSE (no throw, returns modifiable reference)
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_NO_THROW(v(IDX_MID) = TEN);
    EXPECT_DOUBLE_EQ(v(IDX_MID), TEN);
}

TEST(VectorDalA_MCDC, ScalarDivision_ZeroDivisor_ConditionTrue)
{
    // Verifies REQ-SF-1400 | MC/DC: decision (scalar == 0.0) in operator/
    //   condition = TRUE, decision = TRUE (throws domain_error)
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v / ZERO, std::domain_error);
}

TEST(VectorDalA_MCDC, ScalarDivision_NonZeroDivisor_ConditionFalse)
{
    // Verifies REQ-SF-1400 | MC/DC: decision (scalar == 0.0) in operator/
    //   condition = FALSE, decision = FALSE (normal division)
    const FGColumnVector3 v(TWO, FOUR, SIX);
    const FGColumnVector3 result = v / TWO;
    EXPECT_NEAR(result(IDX_LOWER), ONE, TOLERANCE);
    EXPECT_NEAR(result(IDX_MID), TWO, TOLERANCE);
    EXPECT_NEAR(result(IDX_UPPER), THREE, TOLERANCE);
}

TEST(VectorDalA_MCDC, CompoundDivision_ZeroDivisor_ConditionTrue)
{
    // Verifies REQ-SF-1400 | MC/DC: decision (scalar == 0.0) in operator/=
    //   condition = TRUE, decision = TRUE (throws domain_error)
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v /= ZERO, std::domain_error);
}

TEST(VectorDalA_MCDC, CompoundDivision_NonZeroDivisor_ConditionFalse)
{
    // Verifies REQ-SF-1400 | MC/DC: decision (scalar == 0.0) in operator/=
    //   condition = FALSE, decision = FALSE (normal compound division)
    FGColumnVector3 v(TWO, FOUR, SIX);
    v /= TWO;
    EXPECT_NEAR(v(IDX_LOWER), ONE, TOLERANCE);
    EXPECT_NEAR(v(IDX_MID), TWO, TOLERANCE);
    EXPECT_NEAR(v(IDX_UPPER), THREE, TOLERANCE);
}

TEST(VectorDalA_MCDC, Normalize_ZeroMagnitude_SkipsNormalization)
{
    // Verifies REQ-SF-1300 | MC/DC: decision (mag != 0.0) in Normalize()
    //   condition (mag != 0.0) = FALSE, decision = FALSE (skip division body)
    FGColumnVector3 v;
    v.Normalize();
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), ZERO);
    EXPECT_DOUBLE_EQ(v(IDX_MID), ZERO);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), ZERO);
}

TEST(VectorDalA_MCDC, Normalize_NonZeroMagnitude_Normalizes)
{
    // Verifies REQ-SF-1300 | MC/DC: decision (mag != 0.0) in Normalize()
    //   condition (mag != 0.0) = TRUE, decision = TRUE (performs normalization)
    FGColumnVector3 v(THREE, FOUR, ZERO);
    v.Normalize();
    EXPECT_NEAR(v.Magnitude(), ONE, TOLERANCE);
}

// =============================================================================
// d. Error Handling Tests
// =============================================================================

TEST(VectorDalA_ErrorHandling, ConstOperator_Index0_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | const operator() with idx=0 (below valid range)
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_BELOW), std::range_error);
}

TEST(VectorDalA_ErrorHandling, ConstOperator_Index4_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | const operator() with idx=4 (above valid range)
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_ABOVE), std::range_error);
}

TEST(VectorDalA_ErrorHandling, MutableOperator_Index0_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | non-const operator() with idx=0
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_BELOW), std::range_error);
}

TEST(VectorDalA_ErrorHandling, MutableOperator_Index4_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | non-const operator() with idx=4
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v(IDX_ABOVE), std::range_error);
}

TEST(VectorDalA_ErrorHandling, OperatorDivide_ZeroScalar_ThrowsDomainError)
{
    // Verifies REQ-SF-1400 | operator/ with 0.0 scalar throws domain_error
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v / ZERO, std::domain_error);
}

TEST(VectorDalA_ErrorHandling, OperatorDivideEquals_ZeroScalar_ThrowsDomainError)
{
    // Verifies REQ-SF-1400 | operator/= with 0.0 scalar throws domain_error
    FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v /= ZERO, std::domain_error);
}

TEST(VectorDalA_ErrorHandling, ConstEntry_Index0_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | const Entry() delegates to operator(), idx=0 throws
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v.Entry(IDX_BELOW), std::range_error);
}

TEST(VectorDalA_ErrorHandling, ConstEntry_Index4_ThrowsRangeError)
{
    // Verifies REQ-SF-1400 | const Entry() delegates to operator(), idx=4 throws
    const FGColumnVector3 v(ONE, TWO, THREE);
    EXPECT_THROW(v.Entry(IDX_ABOVE), std::range_error);
}

// =============================================================================
// e. State / Sequence Tests
// =============================================================================

TEST(VectorDalA_StateSequence, InitMatrixZero_ClearsAllElements)
{
    // Verifies REQ-SF-0200 | InitMatrix() zeros all three elements
    FGColumnVector3 v(SEVEN, EIGHT, NINE);
    v.InitMatrix();
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), ZERO);
    EXPECT_DOUBLE_EQ(v(IDX_MID), ZERO);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), ZERO);
}

TEST(VectorDalA_StateSequence, InitMatrixScalar_SetsAllToValue)
{
    // Verifies REQ-SF-0200 | InitMatrix(a) sets all elements to a
    FGColumnVector3 v;
    v.InitMatrix(FIVE);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), FIVE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), FIVE);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), FIVE);
}

TEST(VectorDalA_StateSequence, InitMatrixThreeArgs_SetsIndividually)
{
    // Verifies REQ-SF-0200 | InitMatrix(a, b, c) sets each element independently
    FGColumnVector3 v;
    v.InitMatrix(ONE, TWO, THREE);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), ONE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), TWO);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), THREE);
}

TEST(VectorDalA_StateSequence, SelfAssignment_PreservesValues)
{
    // Verifies REQ-SF-0200 | Self-assignment v = v preserves all elements
    FGColumnVector3 v(ONE, TWO, THREE);
    v = v;
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), ONE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), TWO);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), THREE);
}

TEST(VectorDalA_StateSequence, InitializerList_OneElement_OthersUnchanged)
{
    // Verifies REQ-SF-0200 | Partial init list: only first element modified
    FGColumnVector3 v(ONE, TWO, THREE);
    v = {TEN};
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), TEN);
    EXPECT_DOUBLE_EQ(v(IDX_MID), TWO);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), THREE);
}

TEST(VectorDalA_StateSequence, InitializerList_TwoElements_ThirdUnchanged)
{
    // Verifies REQ-SF-0200 | Partial init list: first two modified, third preserved
    FGColumnVector3 v(ONE, TWO, THREE);
    v = {TEN, TEN};
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), TEN);
    EXPECT_DOUBLE_EQ(v(IDX_MID), TEN);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), THREE);
}

TEST(VectorDalA_StateSequence, InitializerList_ThreeElements_AllSet)
{
    // Verifies REQ-SF-0200 | Full init list: all elements set to new values
    FGColumnVector3 v;
    v = {SEVEN, EIGHT, NINE};
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), SEVEN);
    EXPECT_DOUBLE_EQ(v(IDX_MID), EIGHT);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), NINE);
}

TEST(VectorDalA_StateSequence, NormalizeThenMagnitudeIsOne)
{
    // Verifies REQ-SF-1300 | Normalize of non-zero vector produces unit magnitude
    FGColumnVector3 v(THREE, FOUR, FIVE);
    v.Normalize();
    EXPECT_NEAR(v.Magnitude(), ONE, TOLERANCE);
}

TEST(VectorDalA_StateSequence, DoubleNormalize_IsIdempotent)
{
    // Verifies REQ-SF-1300 | Applying Normalize twice yields same result
    FGColumnVector3 v(THREE, FOUR, FIVE);
    v.Normalize();
    const double x_after_first = v(IDX_LOWER);
    const double y_after_first = v(IDX_MID);
    const double z_after_first = v(IDX_UPPER);
    v.Normalize();
    EXPECT_NEAR(v(IDX_LOWER), x_after_first, TOLERANCE);
    EXPECT_NEAR(v(IDX_MID), y_after_first, TOLERANCE);
    EXPECT_NEAR(v(IDX_UPPER), z_after_first, TOLERANCE);
}

TEST(VectorDalA_StateSequence, NormalizeReturnsReferenceToSelf)
{
    // Verifies REQ-SF-1300 | Normalize returns *this for method chaining
    FGColumnVector3 v(THREE, FOUR, ZERO);
    FGColumnVector3& ref = v.Normalize();
    EXPECT_EQ(&ref, &v);
}

TEST(VectorDalA_StateSequence, CopyAssignment_IndependenceVerified)
{
    // Verifies REQ-SF-0200 | Modifying assigned copy does not change original
    const FGColumnVector3 original(ONE, TWO, THREE);
    FGColumnVector3 copy;
    copy = original;
    copy(IDX_LOWER) = TEN;
    EXPECT_DOUBLE_EQ(original(IDX_LOWER), ONE);
    EXPECT_DOUBLE_EQ(copy(IDX_LOWER), TEN);
}

TEST(VectorDalA_StateSequence, CopyConstructor_IndependenceVerified)
{
    // Verifies REQ-SF-0200 | Copy-constructed object is independent of original
    const FGColumnVector3 original(FOUR, FIVE, SIX);
    FGColumnVector3 copy(original);
    copy(IDX_MID) = TEN;
    EXPECT_DOUBLE_EQ(original(IDX_MID), FIVE);
    EXPECT_DOUBLE_EQ(copy(IDX_MID), TEN);
}

// =============================================================================
// f. Additional Coverage Tests
// =============================================================================

TEST(VectorDalA_Coverage, PlusEqualsReturnsReferenceToSelf)
{
    // Verifies REQ-SF-0103 | operator+= returns *this for chaining
    FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(FOUR, FIVE, SIX);
    FGColumnVector3& ref = (a += b);
    EXPECT_EQ(&ref, &a);
}

TEST(VectorDalA_Coverage, MinusEquals_ModifiesAndReturnsReference)
{
    // Verifies REQ-SF-0103 | operator-= modifies in place and returns *this
    FGColumnVector3 a(TEN, TEN, TEN);
    const FGColumnVector3 b(ONE, TWO, THREE);
    FGColumnVector3& ref = (a -= b);
    EXPECT_EQ(&ref, &a);
    EXPECT_DOUBLE_EQ(a(IDX_LOWER), NINE);
    EXPECT_DOUBLE_EQ(a(IDX_MID), EIGHT);
    EXPECT_DOUBLE_EQ(a(IDX_UPPER), SEVEN);
}

TEST(VectorDalA_Coverage, TimesEquals_ModifiesAndReturnsReference)
{
    // Verifies REQ-SF-0103 | operator*= modifies in place and returns *this
    FGColumnVector3 v(ONE, TWO, THREE);
    FGColumnVector3& ref = (v *= THREE);
    EXPECT_EQ(&ref, &v);
    EXPECT_DOUBLE_EQ(v(IDX_LOWER), THREE);
    EXPECT_DOUBLE_EQ(v(IDX_MID), SIX);
    EXPECT_DOUBLE_EQ(v(IDX_UPPER), NINE);
}

TEST(VectorDalA_Coverage, DivideEqualsReturnsReferenceToSelf)
{
    // Verifies REQ-SF-0103 | operator/= returns *this for chaining
    FGColumnVector3 v(TWO, FOUR, SIX);
    FGColumnVector3& ref = (v /= TWO);
    EXPECT_EQ(&ref, &v);
}

TEST(VectorDalA_Coverage, CommutativeScalarMultiply_LeftAndRight)
{
    // Verifies REQ-SF-0103 | Free function: scalar * vec == vec * scalar
    const FGColumnVector3 v(ONE, TWO, THREE);
    const double scalar = FIVE;
    const FGColumnVector3 left = scalar * v;
    const FGColumnVector3 right = v * scalar;
    EXPECT_DOUBLE_EQ(left(IDX_LOWER), right(IDX_LOWER));
    EXPECT_DOUBLE_EQ(left(IDX_MID), right(IDX_MID));
    EXPECT_DOUBLE_EQ(left(IDX_UPPER), right(IDX_UPPER));
}

TEST(VectorDalA_Coverage, StreamOutput_FormattedCorrectly)
{
    // Verifies REQ-SF-0200 | operator<< produces "x , y , z" delimited format
    const FGColumnVector3 v(ONE, TWO, THREE);
    std::ostringstream oss;
    oss << v;
    const std::string output = oss.str();
    EXPECT_NE(output.find(" , "), std::string::npos);
    EXPECT_NE(output.find("1"), std::string::npos);
    EXPECT_NE(output.find("2"), std::string::npos);
    EXPECT_NE(output.find("3"), std::string::npos);
}

TEST(VectorDalA_Coverage, DumpOutput_ContainsDelimiterAndValues)
{
    // Verifies REQ-SF-0200 | Dump() produces delimited output with setw(18)
    const FGColumnVector3 v(ONE, TWO, THREE);
    const std::string delimiter = ",";
    const std::string output = v.Dump(delimiter);
    EXPECT_NE(output.find(delimiter), std::string::npos);
    EXPECT_NE(output.find("1"), std::string::npos);
    EXPECT_NE(output.find("2"), std::string::npos);
    EXPECT_NE(output.find("3"), std::string::npos);
}

TEST(VectorDalA_Coverage, TwoArgMagnitude_Indices1And2)
{
    // Verifies REQ-SF-1300 | Magnitude(1, 2) computes 2D magnitude of X,Y
    const FGColumnVector3 v(THREE, FOUR, FIVE);
    // data[1]=3, data[2]=4 -> sqrt(9+16) = 5
    const double mag12 = v.Magnitude(1, 2);
    EXPECT_NEAR(mag12, FIVE, TOLERANCE);
}

TEST(VectorDalA_Coverage, TwoArgMagnitude_Indices1And3)
{
    // Verifies REQ-SF-1300 | Magnitude(1, 3) computes 2D magnitude of X,Z
    const FGColumnVector3 v(THREE, FOUR, FOUR);
    // data[1]=3, data[3]=4 -> sqrt(9+16) = 5
    const double mag13 = v.Magnitude(1, 3);
    EXPECT_NEAR(mag13, FIVE, TOLERANCE);
}

TEST(VectorDalA_Coverage, TwoArgMagnitude_Indices2And3)
{
    // Verifies REQ-SF-1300 | Magnitude(2, 3) computes 2D magnitude of Y,Z
    const FGColumnVector3 v(FIVE, THREE, FOUR);
    // data[2]=3, data[3]=4 -> sqrt(9+16) = 5
    const double mag23 = v.Magnitude(2, 3);
    EXPECT_NEAR(mag23, FIVE, TOLERANCE);
}

TEST(VectorDalA_Coverage, Equality_DiffersOnlyInThirdElement_ReturnsFalse)
{
    // Verifies REQ-SF-0200 | operator== detects third-element-only mismatch
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(ONE, TWO, FOUR);
    EXPECT_FALSE(a == b);
}

TEST(VectorDalA_Coverage, Equality_DiffersOnlyInFirstElement_ReturnsFalse)
{
    // Verifies REQ-SF-0200 | operator== short-circuits on first element mismatch
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(NINE, TWO, THREE);
    EXPECT_FALSE(a == b);
}

TEST(VectorDalA_Coverage, Equality_DiffersOnlyInSecondElement_ReturnsFalse)
{
    // Verifies REQ-SF-0200 | operator== detects second-element-only mismatch
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(ONE, NINE, THREE);
    EXPECT_FALSE(a == b);
}

TEST(VectorDalA_Coverage, Inequality_SameVectors_ReturnsFalse)
{
    // Verifies REQ-SF-0200 | operator!= returns false for identical vectors
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(ONE, TWO, THREE);
    EXPECT_FALSE(a != b);
}

TEST(VectorDalA_Coverage, Inequality_DifferentVectors_ReturnsTrue)
{
    // Verifies REQ-SF-0200 | operator!= returns true for different vectors
    const FGColumnVector3 a(ONE, TWO, THREE);
    const FGColumnVector3 b(FOUR, FIVE, SIX);
    EXPECT_TRUE(a != b);
}

TEST(VectorDalA_Coverage, AssignmentOperatorReturnsReferenceToSelf)
{
    // Verifies REQ-SF-0200 | Copy assignment returns *this for chaining
    FGColumnVector3 a;
    const FGColumnVector3 b(ONE, TWO, THREE);
    FGColumnVector3& ref = (a = b);
    EXPECT_EQ(&ref, &a);
}

TEST(VectorDalA_Coverage, InitializerListAssignment_ReturnsReferenceToSelf)
{
    // Verifies REQ-SF-0200 | Initializer list assignment returns *this
    FGColumnVector3 v;
    FGColumnVector3& ref = (v = {ONE, TWO, THREE});
    EXPECT_EQ(&ref, &v);
}

TEST(VectorDalA_Coverage, DotProduct_SelfDot_EqualsSquaredMagnitude)
{
    // Verifies REQ-SF-0103 | v . v == |v|^2
    const FGColumnVector3 v(THREE, FOUR, FIVE);
    const double dot = DotProduct(v, v);
    const double mag = v.Magnitude();
    EXPECT_NEAR(dot, mag * mag, TOLERANCE);
}

