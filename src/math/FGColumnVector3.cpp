/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 Source:        FGColumnVector3.cpp  (teaching extract from JSBSim)
 See header for license information.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "FGColumnVector3.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace JSBSim {

// --- Constructors ------------------------------------------------------------

FGColumnVector3::FGColumnVector3()
{
    data[0] = 0.0; data[1] = 0.0; data[2] = 0.0; data[3] = 0.0;
}

FGColumnVector3::FGColumnVector3(double X, double Y, double Z)
{
    data[0] = 0.0;
    data[1] = X;
    data[2] = Y;
    data[3] = Z;
}

FGColumnVector3::FGColumnVector3(const FGColumnVector3& v)
{
    data[0] = 0.0;
    data[1] = v.data[1];
    data[2] = v.data[2];
    data[3] = v.data[3];
}

// --- Element access ----------------------------------------------------------

double FGColumnVector3::operator()(unsigned int idx) const
{
    if (idx < 1 || idx > 3)
        throw std::range_error("FGColumnVector3: index out of range");
    return data[idx];
}

double& FGColumnVector3::operator()(unsigned int idx)
{
    if (idx < 1 || idx > 3)
        throw std::range_error("FGColumnVector3: index out of range");
    return data[idx];
}

// --- Assignment --------------------------------------------------------------

FGColumnVector3& FGColumnVector3::operator=(const FGColumnVector3& b)
{
    data[1] = b.data[1];
    data[2] = b.data[2];
    data[3] = b.data[3];
    return *this;
}

FGColumnVector3& FGColumnVector3::operator=(std::initializer_list<double> lv)
{
    auto it = lv.begin();
    if (lv.size() >= 1) data[1] = *it++;
    if (lv.size() >= 2) data[2] = *it++;
    if (lv.size() >= 3) data[3] = *it;
    return *this;
}

// --- Comparison --------------------------------------------------------------

bool FGColumnVector3::operator==(const FGColumnVector3& b) const
{
    return data[1] == b.data[1]
        && data[2] == b.data[2]
        && data[3] == b.data[3];
}

bool FGColumnVector3::operator!=(const FGColumnVector3& b) const
{
    return !(*this == b);
}

// --- Arithmetic (vector + vector) --------------------------------------------

FGColumnVector3 FGColumnVector3::operator+(const FGColumnVector3& B) const
{
    return FGColumnVector3(data[1] + B.data[1],
                           data[2] + B.data[2],
                           data[3] + B.data[3]);
}

FGColumnVector3 FGColumnVector3::operator-(const FGColumnVector3& B) const
{
    return FGColumnVector3(data[1] - B.data[1],
                           data[2] - B.data[2],
                           data[3] - B.data[3]);
}

FGColumnVector3& FGColumnVector3::operator+=(const FGColumnVector3& B)
{
    data[1] += B.data[1];
    data[2] += B.data[2];
    data[3] += B.data[3];
    return *this;
}

FGColumnVector3& FGColumnVector3::operator-=(const FGColumnVector3& B)
{
    data[1] -= B.data[1];
    data[2] -= B.data[2];
    data[3] -= B.data[3];
    return *this;
}

// --- Arithmetic (scalar) -----------------------------------------------------

FGColumnVector3 FGColumnVector3::operator*(double scalar) const
{
    return FGColumnVector3(scalar * data[1],
                           scalar * data[2],
                           scalar * data[3]);
}

FGColumnVector3 FGColumnVector3::operator/(double scalar) const
{
    if (scalar == 0.0)
        throw std::domain_error("FGColumnVector3: division by zero");
    double inv = 1.0 / scalar;
    return FGColumnVector3(inv * data[1], inv * data[2], inv * data[3]);
}

FGColumnVector3& FGColumnVector3::operator*=(double scalar)
{
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;
    return *this;
}

FGColumnVector3& FGColumnVector3::operator/=(double scalar)
{
    if (scalar == 0.0)
        throw std::domain_error("FGColumnVector3: division by zero");
    double inv = 1.0 / scalar;
    data[1] *= inv;
    data[2] *= inv;
    data[3] *= inv;
    return *this;
}

// --- Cross product -----------------------------------------------------------

FGColumnVector3 FGColumnVector3::operator*(const FGColumnVector3& V) const
{
    return FGColumnVector3(data[2] * V.data[3] - data[3] * V.data[2],
                           data[3] * V.data[1] - data[1] * V.data[3],
                           data[1] * V.data[2] - data[2] * V.data[1]);
}

// --- Unary negation ----------------------------------------------------------

FGColumnVector3 FGColumnVector3::operator-() const
{
    return FGColumnVector3(-data[1], -data[2], -data[3]);
}

// --- Initialization ----------------------------------------------------------

void FGColumnVector3::InitMatrix()
{
    data[1] = data[2] = data[3] = 0.0;
}

void FGColumnVector3::InitMatrix(double a)
{
    data[1] = data[2] = data[3] = a;
}

void FGColumnVector3::InitMatrix(double a, double b, double c)
{
    data[1] = a; data[2] = b; data[3] = c;
}

// --- Vector operations -------------------------------------------------------

double FGColumnVector3::Magnitude() const
{
    return std::sqrt(data[1] * data[1] +
                     data[2] * data[2] +
                     data[3] * data[3]);
}

double FGColumnVector3::Magnitude(int idx1, int idx2) const
{
    return std::sqrt(data[idx1] * data[idx1] +
                     data[idx2] * data[idx2]);
}

FGColumnVector3& FGColumnVector3::Normalize()
{
    double mag = Magnitude();
    if (mag != 0.0) {
        double inv = 1.0 / mag;
        data[1] *= inv;
        data[2] *= inv;
        data[3] *= inv;
    }
    return *this;
}

// --- Utility -----------------------------------------------------------------

std::string FGColumnVector3::Dump(const std::string& delimiter) const
{
    std::ostringstream buf;
    buf << std::setw(18) << std::setprecision(16) << data[1] << delimiter;
    buf << std::setw(18) << std::setprecision(16) << data[2] << delimiter;
    buf << std::setw(18) << std::setprecision(16) << data[3];
    return buf.str();
}

// --- Free functions ----------------------------------------------------------

double DotProduct(const FGColumnVector3& v1, const FGColumnVector3& v2)
{
    return v1(1) * v2(1) + v1(2) * v2(2) + v1(3) * v2(3);
}

FGColumnVector3 operator*(double scalar, const FGColumnVector3& A)
{
    return A * scalar;
}

std::ostream& operator<<(std::ostream& os, const FGColumnVector3& col)
{
    os << col(1) << " , " << col(2) << " , " << col(3);
    return os;
}

} // namespace JSBSim
