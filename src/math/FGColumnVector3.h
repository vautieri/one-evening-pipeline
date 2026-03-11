/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 Header:       FGColumnVector3.h
 Author:       Originally by Tony Peden, Jon Berndt, et al. (JSBSim project)
 Description:  Simplified, self-contained 3-element column vector class
               extracted from the JSBSim Flight Dynamics Model for unit-test
               demonstration purposes.

               JSBSim is licensed under LGPL v2+.  See:
               https://github.com/JSBSim-Team/jsbsim

 NOTE: This is a *teaching extract* — reduced to compile standalone.
       The full JSBSim library has many more features.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef FGCOLUMNVECTOR3_H
#define FGCOLUMNVECTOR3_H

#include <string>
#include <iosfwd>
#include <cmath>
#include <stdexcept>
#include <initializer_list>

namespace JSBSim {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** A 3-element column vector.
 *
 * Elements are accessed with 1-based indexing (matching aerospace convention):
 *   v(1) = X,  v(2) = Y,  v(3) = Z
 */
class FGColumnVector3 {
public:
    // --- Constructors --------------------------------------------------------
    FGColumnVector3();
    FGColumnVector3(double X, double Y, double Z);
    FGColumnVector3(const FGColumnVector3& v);
    ~FGColumnVector3() = default;

    // --- Element access (1-based) --------------------------------------------
    double  operator()(unsigned int idx) const;
    double& operator()(unsigned int idx);

    double  Entry(unsigned int idx) const { return operator()(idx); }
    double& Entry(unsigned int idx)       { return operator()(idx); }

    // --- Assignment ----------------------------------------------------------
    FGColumnVector3& operator=(const FGColumnVector3& b);
    FGColumnVector3& operator=(std::initializer_list<double> lv);

    // --- Comparison ----------------------------------------------------------
    bool operator==(const FGColumnVector3& b) const;
    bool operator!=(const FGColumnVector3& b) const;

    // --- Arithmetic ----------------------------------------------------------
    FGColumnVector3  operator+(const FGColumnVector3& B) const;
    FGColumnVector3  operator-(const FGColumnVector3& B) const;
    FGColumnVector3& operator+=(const FGColumnVector3& B);
    FGColumnVector3& operator-=(const FGColumnVector3& B);

    /// Scalar multiply / divide
    FGColumnVector3  operator*(double scalar) const;
    FGColumnVector3  operator/(double scalar) const;
    FGColumnVector3& operator*=(double scalar);
    FGColumnVector3& operator/=(double scalar);

    /// Cross product  (this x V)
    FGColumnVector3  operator*(const FGColumnVector3& V) const;

    /// Unary negation
    FGColumnVector3  operator-() const;

    // --- Initialization ------------------------------------------------------
    void InitMatrix();
    void InitMatrix(double a);
    void InitMatrix(double a, double b, double c);

    // --- Vector operations ---------------------------------------------------
    double Magnitude() const;
    double Magnitude(int idx1, int idx2) const;
    FGColumnVector3& Normalize();

    // --- Utility -------------------------------------------------------------
    std::string Dump(const std::string& delimiter) const;

private:
    double data[4]; // data[0] unused; data[1..3] = X,Y,Z
};

// --- Free functions ----------------------------------------------------------

double DotProduct(const FGColumnVector3& v1, const FGColumnVector3& v2);

FGColumnVector3 operator*(double scalar, const FGColumnVector3& A);

std::ostream& operator<<(std::ostream& os, const FGColumnVector3& col);

} // namespace JSBSim

#endif // FGCOLUMNVECTOR3_H
