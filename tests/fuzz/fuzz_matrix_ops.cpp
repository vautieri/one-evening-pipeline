/*
 * fuzz_matrix_ops.cpp — libFuzzer target for FGMatrix33
 *
 * Exercises constructors, arithmetic, transpose, inverse, determinant,
 * Euler extraction, and matrix-vector multiply with fuzzer-generated doubles.
 *
 * Minimum input: 72 bytes (9 doubles for one matrix)
 */

#include "FGMatrix33.h"
#include "FGColumnVector3.h"
#include <cstdint>
#include <cstring>
#include <cmath>

using JSBSim::FGMatrix33;
using JSBSim::FGColumnVector3;

static double sanitize(double v) {
    if (std::isnan(v) || std::isinf(v)) return 0.0;
    if (v > 1e12) return 1e12;
    if (v < -1e12) return -1e12;
    return v;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 72) return 0;  // Need at least 9 doubles

    double vals[9];
    std::memcpy(vals, Data, sizeof(vals));
    for (auto &v : vals) v = sanitize(v);

    FGMatrix33 A(vals[0], vals[1], vals[2],
                 vals[3], vals[4], vals[5],
                 vals[6], vals[7], vals[8]);

    // Determinant and invertibility
    volatile double det = A.Determinant();
    volatile bool inv = A.Invertible();

    // Transpose
    FGMatrix33 At = A.Transposed();
    FGMatrix33 At2 = A;
    At2.T();

    // Inverse (only if invertible, to avoid expected exceptions)
    if (A.Invertible()) {
        FGMatrix33 Ainv = A.Inverse();
        // A * A^-1 should be ~identity
        volatile FGMatrix33 prod = A * Ainv;
    }

    // Arithmetic with second matrix (reuse transposed as B)
    volatile FGMatrix33 sum  = A + At;
    volatile FGMatrix33 diff = A - At;

    // Compound assignment
    FGMatrix33 tmp = A;
    tmp += At;
    tmp -= At;

    // Scalar operations
    double scalar = sanitize(vals[0]);
    if (scalar != 0.0) {
        volatile FGMatrix33 scaled = A * scalar;
        volatile FGMatrix33 divided = A / scalar;
        tmp *= scalar;
        tmp /= scalar;
    }

    // Matrix multiply
    volatile FGMatrix33 mm = A * At;
    tmp *= At;

    // Matrix-vector multiply
    FGColumnVector3 v(sanitize(vals[0]), sanitize(vals[1]), sanitize(vals[2]));
    volatile FGColumnVector3 mv = A * v;

    // Comparison
    volatile bool eq = (A == At);
    volatile bool ne = (A != At);

    // Entry accessor
    volatile double e11 = A.Entry(1, 1);
    volatile double e23 = A.Entry(2, 3);

    // Properties
    volatile unsigned int rows = A.Rows();
    volatile unsigned int cols = A.Cols();

    // InitMatrix
    FGMatrix33 Z;
    Z.InitMatrix();
    Z.InitMatrix(vals[0], vals[1], vals[2],
                 vals[3], vals[4], vals[5],
                 vals[6], vals[7], vals[8]);

    // Euler extraction (may have edge cases with gimbal lock)
    volatile FGColumnVector3 euler = A.GetEuler();

    // Scalar left-multiply
    if (scalar != 0.0) {
        volatile FGMatrix33 lmul = scalar * A;
    }

    // Dump
    volatile std::string dump = A.Dump(", ");

    (void)det; (void)inv; (void)sum; (void)diff; (void)mm;
    (void)mv; (void)eq; (void)ne; (void)e11; (void)e23;
    (void)rows; (void)cols; (void)euler; (void)dump;

    return 0;
}
