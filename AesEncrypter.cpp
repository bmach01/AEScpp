#include "AesEncrypter.h"

AesEncrypter::AesEncrypter(keyLength version) : AesCommon(version, MIX_MATRIX, S_BOX) {}

std::string AesEncrypter::encrypt(std::string plainText, std::string key) {

    std::stringstream res;
    std::size_t plainTextLen = plainText.length();
    numOfBlocks = std::ceil((double)plainTextLen / 16);

    allocateMatrixes();
    stringKeyToMatrixKey(key);
    generateKeyMatrix();
    splitToBlocks(plainText, plainTextLen);

    for (std::size_t i = 0; i < numOfBlocks; i++) {
        addKeyToBlock(i, 0);

        for (uint8_t j = 1; j < version; j++) {

            subBytes(i);
            shiftRows(i);
            mixColumns(i);
            addKeyToBlock(i, j);
        }

        subBytes(i);
        shiftRows(i);
        addKeyToBlock(i, version);
        
    }

    for (std::size_t i = 0; i < numOfBlocks; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            for (uint8_t k = 0; k < 4; k++) {
                res << matrix[i][k][j];
            }
        }
    }
    deallocateMatrixes();
    return res.str();
}

void AesEncrypter::shiftRows(std::size_t blockIndex) {
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