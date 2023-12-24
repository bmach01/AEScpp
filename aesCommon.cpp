#include "AesCommon.h"
#include <iostream>

/*
TOOLS
*/

uint8_t AesCommon::gfmul(uint8_t a, uint8_t b) {
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

void AesCommon::allocateMatrixes() {
    matrix = new uint8_t**[numOfBlocks];
 
    for (int i = 0; i < numOfBlocks; i++) {
        matrix[i] = new uint8_t*[4];

        for (int j = 0; j < 4; j++) {
            matrix[i][j] = new uint8_t[4];

        }
    }

    keyMatrix = new uint8_t*[4];
    for (uint8_t i = 0; i < 4; i++) {
        keyMatrix[i] = new uint8_t[subSize];
    }

}

void AesCommon::deallocateMatrixes() {

    for (int i = 0; i < numOfBlocks; i++) {
        for (int j = 0; j < 4; j++) {
            delete[] matrix[i][j];
        }
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;

    for (int i = 0; i < 4; i++) {
        delete[] keyMatrix[i];
    }
    delete[] keyMatrix;
    keyMatrix = nullptr;
}

void AesCommon::rotWord(uint8_t word[4]) {
    uint8_t tmp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;
}

/*
COMMON
*/

void AesCommon::mixColumns(std::size_t blockIndex) {
    uint8_t output[4][4]{}; 

    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            output[i][j] = 0;
            for (uint8_t k = 0; k < 4; k++) {
                //output[i][j] ^= GFMUL_LOOKUP[MIX_MATRIX[i][k]][matrix[blockIndex][k][j]];
                output[i][j] ^= gfmul(pMixMatrix[i][k], matrix[blockIndex][k][j]);
            }
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            matrix[blockIndex][i][j] = output[i][j];
        }
    }
}

void AesCommon::generateKeyWord(int wordIndex) {
    uint8_t word[4] = { 0, 0, 0, 0 };
    uint8_t word_1[4] = {
        keyMatrix[0][wordIndex - 1],
        keyMatrix[1][wordIndex - 1],
        keyMatrix[2][wordIndex - 1],
        keyMatrix[3][wordIndex - 1]
    };

    if (wordIndex % genStart == 0) {
        
        rotWord(word_1);
        word[0] = keyMatrix[0][wordIndex - genStart] ^ S_BOX[word_1[0]] ^ RC[(int)(wordIndex / genStart) - 1];
        word[1] = keyMatrix[1][wordIndex - genStart] ^ S_BOX[word_1[1]];
        word[2] = keyMatrix[2][wordIndex - genStart] ^ S_BOX[word_1[2]];
        word[3] = keyMatrix[3][wordIndex - genStart] ^ S_BOX[word_1[3]];

    }
    else {
        for (uint8_t i = 0; i < 4; i++) {
            word[i] = keyMatrix[i][wordIndex - genStart] ^ word_1[i];
        }
    }

    keyMatrix[0][wordIndex] = word[0];
    keyMatrix[1][wordIndex] = word[1];
    keyMatrix[2][wordIndex] = word[2];
    keyMatrix[3][wordIndex] = word[3];
}

void AesCommon::generateKeyWord256(int wordIndex) {
    uint8_t word[4] = { 0, 0, 0, 0 };
    uint8_t word_1[4] = {
        keyMatrix[0][wordIndex - 1],
        keyMatrix[1][wordIndex - 1],
        keyMatrix[2][wordIndex - 1],
        keyMatrix[3][wordIndex - 1]
    };

    if (wordIndex % genStart == 0) {
        
        rotWord(word_1);
            word[0] = keyMatrix[0][wordIndex - genStart] ^ S_BOX[word_1[0]] ^ RC[(int)(wordIndex / genStart) - 1];
            word[1] = keyMatrix[1][wordIndex - genStart] ^ S_BOX[word_1[1]];
            word[2] = keyMatrix[2][wordIndex - genStart] ^ S_BOX[word_1[2]];
            word[3] = keyMatrix[3][wordIndex - genStart] ^ S_BOX[word_1[3]];

    }
    else if (wordIndex % genStart == 4) {
        for (uint8_t i = 0; i < 4; i++) {
            word[i] = word_1[i] ^ S_BOX[word_1[i]];
        }
    }
    else {
        for (uint8_t i = 0; i < 4; i++) {
            word[i] = keyMatrix[i][wordIndex - genStart] ^ word_1[i];
        }
    }

    keyMatrix[0][wordIndex] = word[0];
    keyMatrix[1][wordIndex] = word[1];
    keyMatrix[2][wordIndex] = word[2];
    keyMatrix[3][wordIndex] = word[3];
}

void AesCommon::addKeyToBlock(std::size_t blockIndex, uint8_t roundKeyNumber){
    for(uint8_t i = 0; i < 4; i++){
        for(uint8_t j = 0; j < 4; j++){
            matrix[blockIndex][i][j] ^= keyMatrix[i][j + (4 * roundKeyNumber)];
        }
    }
}   

void AesCommon::splitToBlocks(std::string text, std::size_t length) {
    std::size_t currentChar = 0;

    for (std::size_t i = 0; i < numOfBlocks; i++){

        for(uint8_t j = 0; j < 4; j++){

            for (uint8_t n = 0; n < 4; n++){

                if (currentChar/*n+j4+i16*/ < length) {
                    matrix[i][n][j] = text[/*n+j4+i16*/currentChar]; 
                    currentChar++;
                }

                else
                    matrix[i][n][j] = 0;
            }
        }
    }
}

void AesCommon::stringKeyToMatrixKey(std::string key) {
    uint32_t indexInKey = 0; 

    for (uint8_t i = 0; i < genStart; i++){
        for(uint8_t j = 0; j < 4; j++){
            keyMatrix[j][i] = std::stoi(key.substr(indexInKey, 2), 0, 16);
            indexInKey += 2;
        }

    }
}

AesCommon::AesCommon(keyLength version, const uint8_t mixMatrix[4][4], const uint8_t sBox[255]) {
    this->version = version;
    pMixMatrix = mixMatrix;
    pSubBox = sBox;

    switch (version) {
        case AES_128: subSize = 44; genStart = 4; break;
        case AES_192: subSize = 52; genStart = 6; break;
        case AES_256: subSize = 60; genStart = 8; break;
    }
}

AesCommon::~AesCommon() {
    if (keyMatrix != nullptr) // && matrix != nullptr
        deallocateMatrixes();
}

void AesCommon::generateKeyMatrix() {
    if (version == AES_256) {
        for (uint8_t i = genStart; i < subSize; i++) {
            generateKeyWord256(i);
        }
    }
    else {
        for (uint8_t i = genStart; i < subSize; i++) {
            generateKeyWord(i);
        }
    }
}

void AesCommon::subBytes(std::size_t blockIndex) {
    
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            matrix[blockIndex][i][j] = pSubBox[matrix[blockIndex][i][j]];
        }
    }
    
}

void AesCommon::shiftRows(std::size_t blockIndex) {}
