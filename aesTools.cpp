#include "aes.h"

namespace aes {
    uint8_t gfmul(uint8_t a, uint8_t b) {
        uint8_t res = 0;
        while (a != 0 && b != 0) {
            if (b & 1)
                res ^= a;

            if (a & 0x80)
                a = (a << 1) ^ 0x11b;
            else
                a <<= 1;
            b >>= 1;
        }

	return res;
}

uint8_t*** create3DArray(int dim1, int dim2, int dim3) {
    uint8_t ***array = new uint8_t**[dim1];
 
    for (int i = 0; i < dim1; i++) {
        array[i] = new uint8_t*[dim2];

        for (int j = 0; j < dim2; j++) {
            array[i][j] = new uint8_t[dim3];

        }
    }
    return array;
}

void delete3DArray(uint8_t*** array, int dim1, int dim2) {

    for (int i = 0; i < dim1; i++) {
        for (int j = 0; j < dim2; j++) {
            delete[] array[i][j];
        }
        delete[] array[i];
    }
    delete[] array;
}

}
