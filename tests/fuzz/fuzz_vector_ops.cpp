/*
 * fuzz_vector_ops.cpp — libFuzzer target for FGColumnVector3
 *
 * Exercises constructors, arithmetic operators, normalization, cross product,
 * and magnitude with fuzzer-generated doubles. Catches undefined behavior,
 * out-of-range indexing, and floating-point exceptions via ASan + UBSan.
 *
 * Minimum input: 48 bytes (two 3-element vectors = 6 doubles)
 */

#include "FGColumnVector3.h"
#include <cstdint>
#include <cstring>
#include <cmath>

using JSBSim::FGColumnVector3;
using JSBSim::DotProduct;

static double sanitize(double v) {
    if (std::isnan(v) || std::isinf(v)) return 0.0;
    // Clamp to a range that won't cause overflow in multiplication
    if (v > 1e15) return 1e15;
    if (v < -1e15) return -1e15;
    return v;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 48) return 0;  // Need at least 6 doubles

    double vals[6];
    std::memcpy(vals, Data, sizeof(vals));

    // Sanitize to avoid trivial FP traps
    for (auto &v : vals) v = sanitize(v);

    FGColumnVector3 a(vals[0], vals[1], vals[2]);
    FGColumnVector3 b(vals[3], vals[4], vals[5]);

    // Basic arithmetic
    volatile FGColumnVector3 sum  = a + b;
    volatile FGColumnVector3 diff = a - b;
    volatile FGColumnVector3 neg  = -a;

    // Compound assignment
    FGColumnVector3 tmp = a;
    tmp += b;
    tmp -= b;

    // Scalar operations
    double scalar = (Size > 48) ? sanitize(vals[0]) : 2.0;
    if (scalar != 0.0) {
        volatile FGColumnVector3 scaled = a * scalar;
        volatile FGColumnVector3 divided = a / scalar;
        tmp *= scalar;
        tmp /= scalar;
    }

    // Cross product
    volatile FGColumnVector3 cross = a * b;

    // Dot product
    volatile double dot = DotProduct(a, b);

    // Magnitude
    volatile double mag = a.Magnitude();
    volatile double mag2d = a.Magnitude(1, 2);

    // Normalize (only if magnitude > 0)
    if (a.Magnitude() > 1e-10) {
        FGColumnVector3 normed = a;
        normed.Normalize();
    }

    // Comparison
    volatile bool eq = (a == b);
    volatile bool ne = (a != b);

    // Entry accessor
    volatile double e1 = a.Entry(1);
    volatile double e2 = a.Entry(2);
    volatile double e3 = a.Entry(3);

    // InitMatrix variants
    FGColumnVector3 c;
    c.InitMatrix();
    c.InitMatrix(vals[0]);
    c.InitMatrix(vals[0], vals[1], vals[2]);

    // Scalar left-multiply
    if (scalar != 0.0) {
        volatile FGColumnVector3 lmul = scalar * a;
    }

    // Dump
    volatile std::string dump = a.Dump(", ");

    (void)sum; (void)diff; (void)neg; (void)cross;
    (void)dot; (void)mag; (void)mag2d; (void)eq; (void)ne;
    (void)e1; (void)e2; (void)e3; (void)dump;

    return 0;
}
