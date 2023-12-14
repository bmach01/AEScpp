#include "sstream"
#include <cassert>

#include "aesLookup.hpp"
#include "aesCipher.h"

void testKeyExtension128(std::string key) {

    uint8_t keyBlocks[4][44];
    std::stringstream generatedKey;

    // as in code
    aes::stringKeyToMatrixKey(key, keyBlocks);
        for (uint8_t i = 4; i < 44; i++)
            aes::generateKeyWord128(i, keyBlocks);
    
    for (uint8_t i = 0; i < 44; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            generatedKey << std::hex << (int)keyBlocks[j][i] << " ";
        }
        std::cout << generatedKey.str() << "\n";
        generatedKey.str("");
    }

    std::cout << generatedKey.str() << "\n";
    
    // if (generatedKey.str().compare(extendedKey))
    //     std::cout << "assert successful\n";
    // else
    //     std::cout << "assert failed\n";

}

void testShiftRows() {
    uint8_t ***testBox = aes::create3DArray(1, 4, 4);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            testBox[0][i][j] = j;
        }
    }

    aes::shiftRows(testBox, 0);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << (int)testBox[0][i][j] << " ";
        }
        std::cout << "\n";
    }

    aes::delete3DArray(testBox, 1, 4);

}

void mul(const uint8_t matrix1[4][4], const uint8_t matrix2[4], uint8_t result[4]) {
    // Matrix multiplication
    for (int i = 0; i < 4; ++i) {
        result[i] = 0;
        for (int j = 0; j < 4; ++j) {
            result[i] ^= GFMUL_LOOKUP[matrix1[i][j]][matrix2[j]];
        }
    }
}

void testMixColumns() {
    uint8_t ***matrix = aes::create3DArray(1, 4, 4);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[0][i][j] = i;
            std::cout << (int)matrix[0][i][j] << " ";
        }
        std::cout << "\n";
    }
    
    aes::mixColumns(matrix, 0);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << (int)matrix[0][i][j] << " ";
        }
        std::cout << "\n";
    }

    aes::delete3DArray(matrix, 1, 1);
    return;
}