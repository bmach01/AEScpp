#include "aes.h"

namespace aes {

void invSubBytes(uint8_t ***matrix, std::size_t blockIndex) {
    
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            matrix[blockIndex][i][j] = INV_S_BOX[matrix[blockIndex][i][j]];
        }
    }

}

void invShiftRows(uint8_t ***matrix, std::size_t blockIndex) {
    uint8_t tmp = 0;

    // row 1
    tmp = matrix[blockIndex][1][0];
    matrix[blockIndex][1][0] = matrix[blockIndex][1][3];
    matrix[blockIndex][1][3] = matrix[blockIndex][1][2];
    matrix[blockIndex][1][2] = matrix[blockIndex][1][1];
    matrix[blockIndex][1][1] = tmp;

    // row 2
    tmp = matrix[blockIndex][2][1];
    matrix[blockIndex][2][1] = matrix[blockIndex][2][3];
    matrix[blockIndex][2][3] = tmp;
    tmp = matrix[blockIndex][2][0];
    matrix[blockIndex][2][0] = matrix[blockIndex][2][2];
    matrix[blockIndex][2][2] = tmp;

    // row 3
    tmp = matrix[blockIndex][3][0];
    matrix[blockIndex][3][0] = matrix[blockIndex][3][1];
    matrix[blockIndex][3][1] = matrix[blockIndex][3][2];
    matrix[blockIndex][3][2] = matrix[blockIndex][3][3];
    matrix[blockIndex][3][3] = tmp;
}



std::string decrypt(std::string cipher, std::string key, keyLength turns) {

    std::stringstream res;

    std::size_t plainTextLen = cipher.length();
    std::size_t numOfBlocks = std::ceil((double)plainTextLen / 16);

    uint8_t ***textBlocks = create3DArray(numOfBlocks, 4, 4);

    uint8_t subSize = 0;
    uint8_t genStart = 0;
    switch (turns) {
        case AES_128: subSize = 44; genStart = 4; break;
        case AES_192: subSize = 52; genStart = 6; break;
        case AES_256: subSize = 60; genStart = 8; break;
    }

    uint8_t **extendedKey = new uint8_t *[4];
    for (uint8_t i = 0; i < 4; i++) {
        extendedKey[i] = new uint8_t[subSize];
    }


    stringKeyToMatrixKey(key, extendedKey, genStart);

    for (uint8_t i = genStart; i < subSize; i++)
        generateKeyWord(i, extendedKey, genStart);

    splitToBlocks(cipher, plainTextLen, textBlocks, numOfBlocks);

    for (std::size_t i = 0; i < numOfBlocks; i++) {
        addKeyToBlock(i, turns, textBlocks, extendedKey);

        for (uint8_t j = 1; j < turns; j++) {
            invShiftRows(textBlocks, i);
            invSubBytes(textBlocks, i);
            addKeyToBlock(i, 10 - j, textBlocks, extendedKey);
            mixColumns(textBlocks, i, INV_MIX_MATRIX);
        }

        invShiftRows(textBlocks, i);
        invSubBytes(textBlocks, i);
        addKeyToBlock(i, 0, textBlocks, extendedKey);
    }

    for (std::size_t i = 0; i < numOfBlocks; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            for (uint8_t k = 0; k < 4; k++) {
                res << textBlocks[i][k][j];
            }
        }
    }

    delete3DArray(textBlocks, numOfBlocks, 4);
    for (int i = 0; i < 4; i++) {
        delete[] extendedKey[i];
    }
    delete[] extendedKey;

    return res.str();
}

}

