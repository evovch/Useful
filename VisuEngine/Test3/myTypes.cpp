#include "myTypes.h"

double matrix4x4_t::Det(void) const {
    matrix3x3_t m0(data[5],  data[6],  data[7],
                   data[9],  data[10], data[11],
                   data[13], data[14], data[15]);
    matrix3x3_t m1(data[4],  data[6],  data[7],
                   data[8],  data[10], data[11],
                   data[12], data[14], data[15]);
    matrix3x3_t m2(data[4],  data[5],  data[7],
                   data[8],  data[9],  data[11],
                   data[12], data[13], data[15]);
    matrix3x3_t m3(data[4],  data[5],  data[6],
                   data[8],  data[9],  data[10],
                   data[12], data[13], data[14]);
    return data[0]*m0.Det() - data[1]*m1.Det() + data[2]*m2.Det() - data[3]*m3.Det();
}

void matrix4x4_t::Inverse(void) {

    double det = this->Det();

    if (det == 0.) {
        fprintf(stderr, "[WARN] Matrix not invertable. Aborting.\n");
        return;
    }

    double detInv = 1. / det;

    matrix3x3_t m0(data[5],  data[6],  data[7],
                   data[9],  data[10], data[11],
                   data[13], data[14], data[15]);
    matrix3x3_t m1(data[4],  data[6],  data[7],
                   data[8],  data[10], data[11],
                   data[12], data[14], data[15]);
    matrix3x3_t m2(data[4],  data[5],  data[7],
                   data[8],  data[9],  data[11],
                   data[12], data[13], data[15]);
    matrix3x3_t m3(data[4],  data[5],  data[6],
                   data[8],  data[9],  data[10],
                   data[12], data[13], data[14]);

    matrix3x3_t m4(data[1],  data[2],  data[3],
                   data[9],  data[10], data[11],
                   data[13], data[14], data[15]);
    matrix3x3_t m5(data[0],  data[2],  data[3],
                   data[8],  data[10], data[11],
                   data[12], data[14], data[15]);
    matrix3x3_t m6(data[0],  data[1],  data[3],
                   data[8],  data[9],  data[11],
                   data[12], data[13], data[15]);
    matrix3x3_t m7(data[0],  data[1],  data[2],
                   data[8],  data[9],  data[10],
                   data[12], data[13], data[14]);

    matrix3x3_t m8(data[1],  data[2],  data[3],
                   data[5],  data[6],  data[7],
                   data[13], data[14], data[15]);
    matrix3x3_t m9(data[0],  data[2],  data[3],
                   data[4],  data[6],  data[7],
                   data[12], data[14], data[15]);
    matrix3x3_t m10(data[0], data[1],  data[3],
                    data[4], data[5],  data[7],
                    data[12],data[13], data[15]);
    matrix3x3_t m11(data[0], data[1],  data[2],
                    data[4], data[5],  data[6],
                    data[12],data[13], data[14]);

    matrix3x3_t m12(data[1], data[2],  data[3],
                    data[5], data[6],  data[7],
                    data[9], data[10], data[11]);
    matrix3x3_t m13(data[0], data[2],  data[3],
                    data[4], data[6],  data[7],
                    data[8], data[10], data[11]);
    matrix3x3_t m14(data[0], data[1],  data[3],
                    data[4], data[5],  data[7],
                    data[8], data[9],  data[11]);
    matrix3x3_t m15(data[0], data[1],  data[2],
                    data[4], data[5],  data[6],
                    data[8], data[9],  data[10]);

    data[0] = detInv * m0.Det();
    data[1] = -detInv * m1.Det();
    data[2] = detInv * m2.Det();
    data[3] = -detInv * m3.Det();

    data[4] = -detInv * m4.Det();
    data[5] = detInv * m5.Det();
    data[6] = -detInv * m6.Det();
    data[7] = detInv * m7.Det();

    data[8] = detInv * m8.Det();
    data[9] = -detInv * m9.Det();
    data[10] = detInv * m10.Det();
    data[11] = -detInv * m11.Det();

    data[12] = -detInv * m12.Det();
    data[13] = detInv * m13.Det();
    data[14] = -detInv * m14.Det();
    data[15] = detInv * m15.Det();
}

void crossProd(direction_t v1, direction_t v2, direction_t& res) {
	res.fPx = v1.fPy*v2.fPz - v1.fPz*v2.fPy;
	res.fPy = v1.fPz*v2.fPx - v1.fPx*v2.fPz;
	res.fPz = v1.fPx*v2.fPy - v1.fPy*v2.fPx;
}

double dotProd(direction_t v1, direction_t v2) {
	return v1.fPx*v2.fPx + v1.fPy*v2.fPy + v1.fPz*v2.fPz;
}
