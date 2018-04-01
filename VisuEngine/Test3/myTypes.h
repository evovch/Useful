#ifndef MYTYPES_H
#define MYTYPES_H

#include <cfloat>
#include <cmath>
#include <cstdio>

/**
    0 1 2
    3 4 5
    6 7 8
**/
class matrix3x3_t
{
public:
    matrix3x3_t() {
        // Init with E matrix (all zero, diagonal elements 1)
        for (unsigned int i=0; i<9; i++) data[i] = 0.;
        data[0] = 1.;
        data[4] = 1.;
        data[8] = 1.;
    }
    matrix3x3_t(double d0, double d1, double d2,
                double d3, double d4, double d5,
                double d6, double d7, double d8) {
        data[0] = d0;
        data[1] = d1;
        data[2] = d2;
        data[3] = d3;
        data[4] = d4;
        data[5] = d5;
        data[6] = d6;
        data[7] = d7;
        data[8] = d8;
    }
    ~matrix3x3_t() {}

    matrix3x3_t& operator=(const matrix3x3_t& right) {
        if (this == &right) {
            return *this;
        }
        for (unsigned int i=0; i<9; i++) data[i] = right.data[i];
        return *this;
    }

    /*
        0 11
        1 12
        2 13
        3 21
        4 22
        5 23
        6 31
        7 32
        8 33
        det = 11*22*33 - 11*23*32 - 12*21*33 + 12*23*31 + 13*21*32 - 13*22*31
        det = 0*4*8    - 0*5*7    - 1*3*8    + 1*5*6    + 2*3*7    - 2*4*6
    */
    double Det() const {
        return data[0]*data[4]*data[8] -
               data[0]*data[5]*data[7] -
               data[1]*data[3]*data[8] +
               data[1]*data[5]*data[6] +
               data[2]*data[3]*data[7] -
               data[2]*data[4]*data[6];
    }

public:
    double data[9];

};

/**
    0  1  2  3
    4  5  6  7
    8  9  10 11
    12 13 14 15
**/
class matrix4x4_t
{
public:
    matrix4x4_t() {
        // Init with E matrix (all zero, diagonal elements 1)
        for (unsigned int i=0; i<16; i++) data[i] = 0.;
        data[0] = 1.;
        data[5] = 1.;
        data[10] = 1.;
        data[15] = 1.;
    }
    ~matrix4x4_t() {}

    matrix4x4_t& operator=(const matrix4x4_t& right) {
        if (this == &right) {
            return *this;
        }
        for (unsigned int i=0; i<16; i++) data[i] = right.data[i];
        return *this;
    }

    double Det(void) const;

    void Inverse(void);

public:
    double data[16];

};

class point_t
{
public:
    point_t() {
        fX = -DBL_MAX;
        fY = -DBL_MAX;
        fZ = -DBL_MAX;
    }
    point_t(double p_x, double p_y, double p_z) {
        fX = p_x;
        fY = p_y;
        fZ = p_z;
    }
    ~point_t() {}

    point_t& operator=(const point_t& right) {
        if (this == &right) {
            return *this;
        }
        this->fX = right.fX;
        this->fY = right.fY;
        this->fZ = right.fZ;
        return *this;
    }

    point_t& operator+=(const point_t& right) {
        this->fX += right.fX;
        this->fY += right.fY;
        this->fZ += right.fZ;
        return *this;
    }

    // Multiply by matrix (matrix on the left)
    void MultiplyLeft(matrix4x4_t leftM) {
        double tmpX = leftM.data[0]*fX + leftM.data[1]*fY + leftM.data[2]*fZ + leftM.data[3];
        double tmpY = leftM.data[4]*fX + leftM.data[5]*fY + leftM.data[6]*fZ + leftM.data[7];
        double tmpZ = leftM.data[8]*fX + leftM.data[9]*fY + leftM.data[10]*fZ + leftM.data[11];
        this->fX = tmpX;
        this->fY = tmpY;
        this->fZ = tmpZ;
    }

public:
    double fX;
    double fY;
    double fZ;

};

class direction_t
{
public:
    direction_t() {
        fPx = 0.;
        fPy = 0.;
        fPz = 1.;
    }
    direction_t(double p_px, double p_py, double p_pz) {
        fPx = p_px;
        fPy = p_py;
        fPz = p_pz;
    }
    ~direction_t() {}

    direction_t& operator=(const direction_t& right) {
        if (this == &right) {
            return *this;
        }
        this->fPx = right.fPx;
        this->fPy = right.fPy;
        this->fPz = right.fPz;
        return *this;
    }

    direction_t operator-(const direction_t& right) {
        direction_t res;
        res.fPx = this->fPx - right.fPx;
        res.fPy = this->fPy - right.fPy;
        res.fPz = this->fPz - right.fPz;
        return res;
    }

    void Normalize() {
        double len = sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
        fPx /= len;
        fPy /= len;
        fPz /= len;
        /*len = sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
        fprintf(stderr, "[DEBUG] len=%f\n", len);*/
    }

    void Multiply(double scalar) {
        fPx *= scalar;
        fPy *= scalar;
        fPz *= scalar;
    }

    // Multyply by matrix (matrix on the left)
    void MultiplyLeft(matrix4x4_t leftM) {
        double tmpX = leftM.data[0]*fPx + leftM.data[1]*fPy + leftM.data[2]*fPz + leftM.data[3];
        double tmpY = leftM.data[4]*fPx + leftM.data[5]*fPy + leftM.data[6]*fPz + leftM.data[7];
        double tmpZ = leftM.data[8]*fPx + leftM.data[9]*fPy + leftM.data[10]*fPz + leftM.data[11];
        this->fPx = tmpX;
        this->fPy = tmpY;
        this->fPz = tmpZ;
    }

public:
    double fPx;
    double fPy;
    double fPz;

};

void crossProd(direction_t v1, direction_t v2, direction_t& res);
double dotProd(direction_t v1, direction_t v2);

#endif // MYTYPES_H
