#include "aes.h"

namespace aes {

void shiftRows(uint8_t ***matrix, std::size_t blockIndex) {
    uint8_t tmp = 0;
    
    // row 1
    tmp = matrix[blockIndex][1][0];
    matrix[blockIndex][1][0] = matrix[blockIndex][1][1];
    matrix[blockIndex][1][1] = matrix[blockIndex][1][2];
    matrix[blockIndex][1][2] = matrix[blockIndex][1][3];
    matrix[blockIndex][1][3] = tmp;

    // row 2
    tmp = matrix[blockIndex][2][0];
    matrix[blockIndex][2][0] = matrix[blockIndex][2][2];
    matrix[blockIndex][2][2] = tmp;
    tmp = matrix[blockIndex][2][1];
    matrix[blockIndex][2][1] = matrix[blockIndex][2][3];
    matrix[blockIndex][2][3] = tmp;

    // row 3
    tmp = matrix[blockIndex][3][0];
    matrix[blockIndex][3][0] = matrix[blockIndex][3][3];
    matrix[blockIndex][3][3] = matrix[blockIndex][3][2];
    matrix[blockIndex][3][2] = matrix[blockIndex][3][1];
    matrix[blockIndex][3][1] = tmp; 
    
}

void subBytes(uint8_t ***matrix, std::size_t blockIndex) {
    
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            matrix[blockIndex][i][j] = S_BOX[matrix[blockIndex][i][j]];
        }
    }
    
}


std::string encrypt(std::string plainText, std::string key, keyLength turns) {

    std::stringstream res;

    std::size_t plainTextLen = plainText.length();
    std::size_t numOfBlocks = std::ceil((double)plainTextLen / 16);

    uint8_t ***textBlocks = create3DArray(numOfBlocks, 4, 4);

    uint8_t subSize = 0;
    uint8_t genStart = 0;
    switch (turns) {
        case AES_128: subSize = 44; genStart = 4; break;
        case AES_192: subSize = 52; genStart = 6; break;
        case AES_256: subSize = 60; genStart = 8; break;
    }

    uint8_t **extendedKey = new uint8_t*[4];
    for (uint8_t i = 0; i < 4; i++) {
        extendedKey[i] = new uint8_t[subSize];
    }

    stringKeyToMatrixKey(key, extendedKey, genStart);

    if (genStart == 8) {
        for (uint8_t i = genStart; i < subSize; i++) {
            generateKeyWord256(i, extendedKey, genStart);
        }
    }
    else {
        for (uint8_t i = genStart; i < subSize; i++) {
            generateKeyWord(i, extendedKey, genStart);
        }
    }


    for (int i = 0; i < subSize; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << std::hex << (int)extendedKey[j][i] << " ";
        }
        std::cout << " ";
    }
    std::cout << "\n";


    splitToBlocks(plainText, plainTextLen, textBlocks, numOfBlocks);

    for (std::size_t i = 0; i < numOfBlocks; i++) {
        addKeyToBlock(i, 0, textBlocks, extendedKey);

        for (uint8_t j = 1; j < turns; j++) {

            subBytes(textBlocks, i);
            shiftRows(textBlocks, i);
            mixColumns(textBlocks, i, MIX_MATRIX);
            addKeyToBlock(i, j, textBlocks, extendedKey);
        }

        subBytes(textBlocks, i);
        shiftRows(textBlocks, i);
        addKeyToBlock(i, turns, textBlocks, extendedKey);
        
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




