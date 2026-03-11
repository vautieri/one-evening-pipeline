/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 Header:       FGMatrix33.h
 Description:  Simplified 3x3 matrix class extracted from JSBSim for
               unit-test demonstration purposes.
               JSBSim is licensed under LGPL v2+.
               https://github.com/JSBSim-Team/jsbsim
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef FGMATRIX33_H
#define FGMATRIX33_H

#include "FGColumnVector3.h"
#include <string>
#include <iosfwd>
#include <initializer_list>

namespace JSBSim {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** A 3x3 matrix class.
 *
 * Elements are accessed with 1-based row, column indexing:
 *   M(1,1) .. M(3,3)
 */
class FGMatrix33 {
public:
    // --- Constructors --------------------------------------------------------
    FGMatrix33();
    FGMatrix33(const FGMatrix33& M);
    FGMatrix33(double m11, double m12, double m13,
               double m21, double m22, double m23,
               double m31, double m32, double m33);
    ~FGMatrix33() = default;

    // --- Element access (1-based) --------------------------------------------
    double  operator()(unsigned int row, unsigned int col) const;
    double& operator()(unsigned int row, unsigned int col);

    double  Entry(unsigned int r, unsigned int c) const { return operator()(r, c); }
    double& Entry(unsigned int r, unsigned int c)       { return operator()(r, c); }

    // --- Properties ----------------------------------------------------------
    unsigned int Rows() const { return 3; }
    unsigned int Cols() const { return 3; }
    double Determinant() const;
    bool Invertible() const;

    // --- Matrix operations ---------------------------------------------------
    FGMatrix33 Transposed() const;
    void T();  // in-place transpose
    FGMatrix33 Inverse() const;

    void InitMatrix();
    void InitMatrix(double m11, double m12, double m13,
                    double m21, double m22, double m23,
                    double m31, double m32, double m33);

    // --- Euler / quaternion conversions (forward-declared) -------------------
    FGColumnVector3 GetEuler() const;

    // --- Arithmetic ----------------------------------------------------------
    FGMatrix33  operator+(const FGMatrix33& B) const;
    FGMatrix33  operator-(const FGMatrix33& B) const;
    FGMatrix33& operator+=(const FGMatrix33& B);
    FGMatrix33& operator-=(const FGMatrix33& B);

    FGMatrix33  operator*(double scalar) const;
    FGMatrix33  operator/(double scalar) const;
    FGMatrix33& operator*=(double scalar);
    FGMatrix33& operator/=(double scalar);

    FGMatrix33  operator*(const FGMatrix33& B) const;
    FGMatrix33& operator*=(const FGMatrix33& B);

    FGColumnVector3 operator*(const FGColumnVector3& v) const;

    FGMatrix33& operator=(const FGMatrix33& B);

    // --- Comparison ----------------------------------------------------------
    bool operator==(const FGMatrix33& B) const;
    bool operator!=(const FGMatrix33& B) const;

    // --- Utility -------------------------------------------------------------
    std::string Dump(const std::string& delimiter) const;

private:
    // Flat storage — data[0..8]  mapped as (row-1)*3 + (col-1)
    double data[9];

    // Helper to map 1-based (r,c) to flat index
    static unsigned int idx(unsigned int r, unsigned int c) {
        return (r - 1) * 3 + (c - 1);
    }
};

// Free functions
FGMatrix33 operator*(double scalar, const FGMatrix33& M);
std::ostream& operator<<(std::ostream& os, const FGMatrix33& M);

} // namespace JSBSim

#endif // FGMATRIX33_H
