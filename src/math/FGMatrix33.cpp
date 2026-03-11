/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 Source:        FGMatrix33.cpp  (teaching extract from JSBSim)
 See header for license information.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "FGMatrix33.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace JSBSim {

// --- Constructors ------------------------------------------------------------

FGMatrix33::FGMatrix33()
{
    for (int i = 0; i < 9; ++i) data[i] = 0.0;
}

FGMatrix33::FGMatrix33(const FGMatrix33& M)
{
    for (int i = 0; i < 9; ++i) data[i] = M.data[i];
}

FGMatrix33::FGMatrix33(double m11, double m12, double m13,
                       double m21, double m22, double m23,
                       double m31, double m32, double m33)
{
    data[idx(1,1)] = m11; data[idx(1,2)] = m12; data[idx(1,3)] = m13;
    data[idx(2,1)] = m21; data[idx(2,2)] = m22; data[idx(2,3)] = m23;
    data[idx(3,1)] = m31; data[idx(3,2)] = m32; data[idx(3,3)] = m33;
}

// --- Element access ----------------------------------------------------------

double FGMatrix33::operator()(unsigned int row, unsigned int col) const
{
    if (row < 1 || row > 3 || col < 1 || col > 3)
        throw std::range_error("FGMatrix33: index out of range");
    return data[idx(row, col)];
}

double& FGMatrix33::operator()(unsigned int row, unsigned int col)
{
    if (row < 1 || row > 3 || col < 1 || col > 3)
        throw std::range_error("FGMatrix33: index out of range");
    return data[idx(row, col)];
}

// --- Properties --------------------------------------------------------------

double FGMatrix33::Determinant() const
{
    return data[idx(1,1)] * (data[idx(2,2)] * data[idx(3,3)] - data[idx(2,3)] * data[idx(3,2)])
         - data[idx(1,2)] * (data[idx(2,1)] * data[idx(3,3)] - data[idx(2,3)] * data[idx(3,1)])
         + data[idx(1,3)] * (data[idx(2,1)] * data[idx(3,2)] - data[idx(2,2)] * data[idx(3,1)]);
}

bool FGMatrix33::Invertible() const
{
    return Determinant() != 0.0;
}

// --- Matrix operations -------------------------------------------------------

FGMatrix33 FGMatrix33::Transposed() const
{
    return FGMatrix33(data[idx(1,1)], data[idx(2,1)], data[idx(3,1)],
                      data[idx(1,2)], data[idx(2,2)], data[idx(3,2)],
                      data[idx(1,3)], data[idx(2,3)], data[idx(3,3)]);
}

void FGMatrix33::T()
{
    *this = Transposed();
}

FGMatrix33 FGMatrix33::Inverse() const
{
    double det = Determinant();
    if (det == 0.0)
        throw std::domain_error("FGMatrix33::Inverse: singular matrix");

    double invDet = 1.0 / det;

    // Cofactor / adjugate method (Cramer's rule)
    return FGMatrix33(
        invDet * (data[idx(2,2)] * data[idx(3,3)] - data[idx(2,3)] * data[idx(3,2)]),
        invDet * (data[idx(1,3)] * data[idx(3,2)] - data[idx(1,2)] * data[idx(3,3)]),
        invDet * (data[idx(1,2)] * data[idx(2,3)] - data[idx(1,3)] * data[idx(2,2)]),

        invDet * (data[idx(2,3)] * data[idx(3,1)] - data[idx(2,1)] * data[idx(3,3)]),
        invDet * (data[idx(1,1)] * data[idx(3,3)] - data[idx(1,3)] * data[idx(3,1)]),
        invDet * (data[idx(1,3)] * data[idx(2,1)] - data[idx(1,1)] * data[idx(2,3)]),

        invDet * (data[idx(2,1)] * data[idx(3,2)] - data[idx(2,2)] * data[idx(3,1)]),
        invDet * (data[idx(1,2)] * data[idx(3,1)] - data[idx(1,1)] * data[idx(3,2)]),
        invDet * (data[idx(1,1)] * data[idx(2,2)] - data[idx(1,2)] * data[idx(2,1)])
    );
}

void FGMatrix33::InitMatrix()
{
    for (int i = 0; i < 9; ++i) data[i] = 0.0;
}

void FGMatrix33::InitMatrix(double m11, double m12, double m13,
                            double m21, double m22, double m23,
                            double m31, double m32, double m33)
{
    data[idx(1,1)] = m11; data[idx(1,2)] = m12; data[idx(1,3)] = m13;
    data[idx(2,1)] = m21; data[idx(2,2)] = m22; data[idx(2,3)] = m23;
    data[idx(3,1)] = m31; data[idx(3,2)] = m32; data[idx(3,3)] = m33;
}

// --- Euler extraction --------------------------------------------------------

FGColumnVector3 FGMatrix33::GetEuler() const
{
    // Extract Euler angles (phi, theta, psi) from a direction cosine matrix.
    // Follows the aerospace Tait-Bryan ZYX convention.
    FGColumnVector3 euler;

    double sinTheta = -data[idx(1,3)];
    if (sinTheta >= 1.0)
        sinTheta = 1.0;
    else if (sinTheta <= -1.0)
        sinTheta = -1.0;

    double theta = std::asin(sinTheta);

    if (std::abs(sinTheta) < 0.9999999) {
        // Normal case — no gimbal lock
        euler(1) = std::atan2(data[idx(2,3)], data[idx(3,3)]);  // phi
        euler(2) = theta;                                         // theta
        euler(3) = std::atan2(data[idx(1,2)], data[idx(1,1)]);  // psi
    } else {
        // Gimbal lock
        euler(1) = std::atan2(-data[idx(3,2)], data[idx(2,2)]); // phi
        euler(2) = theta;                                         // theta
        euler(3) = 0.0;                                           // psi (arbitrary)
    }

    return euler;
}

// --- Arithmetic (matrix + matrix) --------------------------------------------

FGMatrix33 FGMatrix33::operator+(const FGMatrix33& B) const
{
    FGMatrix33 result;
    for (int i = 0; i < 9; ++i) result.data[i] = data[i] + B.data[i];
    return result;
}

FGMatrix33 FGMatrix33::operator-(const FGMatrix33& B) const
{
    FGMatrix33 result;
    for (int i = 0; i < 9; ++i) result.data[i] = data[i] - B.data[i];
    return result;
}

FGMatrix33& FGMatrix33::operator+=(const FGMatrix33& B)
{
    for (int i = 0; i < 9; ++i) data[i] += B.data[i];
    return *this;
}

FGMatrix33& FGMatrix33::operator-=(const FGMatrix33& B)
{
    for (int i = 0; i < 9; ++i) data[i] -= B.data[i];
    return *this;
}

// --- Arithmetic (scalar) -----------------------------------------------------

FGMatrix33 FGMatrix33::operator*(double scalar) const
{
    FGMatrix33 result;
    for (int i = 0; i < 9; ++i) result.data[i] = data[i] * scalar;
    return result;
}

FGMatrix33 FGMatrix33::operator/(double scalar) const
{
    if (scalar == 0.0)
        throw std::domain_error("FGMatrix33: division by zero");
    double inv = 1.0 / scalar;
    FGMatrix33 result;
    for (int i = 0; i < 9; ++i) result.data[i] = data[i] * inv;
    return result;
}

FGMatrix33& FGMatrix33::operator*=(double scalar)
{
    for (int i = 0; i < 9; ++i) data[i] *= scalar;
    return *this;
}

FGMatrix33& FGMatrix33::operator/=(double scalar)
{
    if (scalar == 0.0)
        throw std::domain_error("FGMatrix33: division by zero");
    double inv = 1.0 / scalar;
    for (int i = 0; i < 9; ++i) data[i] *= inv;
    return *this;
}

// --- Arithmetic (matrix * matrix) --------------------------------------------

FGMatrix33 FGMatrix33::operator*(const FGMatrix33& B) const
{
    FGMatrix33 result;
    for (unsigned int i = 1; i <= 3; ++i)
        for (unsigned int j = 1; j <= 3; ++j) {
            double sum = 0.0;
            for (unsigned int k = 1; k <= 3; ++k)
                sum += data[idx(i,k)] * B.data[idx(k,j)];
            result.data[idx(i,j)] = sum;
        }
    return result;
}

FGMatrix33& FGMatrix33::operator*=(const FGMatrix33& B)
{
    *this = *this * B;
    return *this;
}

// --- Matrix * Vector ---------------------------------------------------------

FGColumnVector3 FGMatrix33::operator*(const FGColumnVector3& v) const
{
    return FGColumnVector3(
        data[idx(1,1)] * v(1) + data[idx(1,2)] * v(2) + data[idx(1,3)] * v(3),
        data[idx(2,1)] * v(1) + data[idx(2,2)] * v(2) + data[idx(2,3)] * v(3),
        data[idx(3,1)] * v(1) + data[idx(3,2)] * v(2) + data[idx(3,3)] * v(3)
    );
}

// --- Assignment --------------------------------------------------------------

FGMatrix33& FGMatrix33::operator=(const FGMatrix33& B)
{
    for (int i = 0; i < 9; ++i) data[i] = B.data[i];
    return *this;
}

// --- Comparison --------------------------------------------------------------

bool FGMatrix33::operator==(const FGMatrix33& B) const
{
    for (int i = 0; i < 9; ++i)
        if (data[i] != B.data[i]) return false;
    return true;
}

bool FGMatrix33::operator!=(const FGMatrix33& B) const
{
    return !(*this == B);
}

// --- Utility -----------------------------------------------------------------

std::string FGMatrix33::Dump(const std::string& delimiter) const
{
    std::ostringstream buf;
    buf << std::setprecision(16);
    for (unsigned int i = 1; i <= 3; ++i) {
        for (unsigned int j = 1; j <= 3; ++j) {
            if (j > 1) buf << delimiter;
            buf << std::setw(18) << data[idx(i,j)];
        }
        if (i < 3) buf << "\n";
    }
    return buf.str();
}

// --- Free functions ----------------------------------------------------------

FGMatrix33 operator*(double scalar, const FGMatrix33& M)
{
    return M * scalar;
}

std::ostream& operator<<(std::ostream& os, const FGMatrix33& M)
{
    os << M(1,1) << " " << M(1,2) << " " << M(1,3) << "\n"
       << M(2,1) << " " << M(2,2) << " " << M(2,3) << "\n"
       << M(3,1) << " " << M(3,2) << " " << M(3,3);
    return os;
}

} // namespace JSBSim
