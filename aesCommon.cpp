#include "aes.h"

namespace aes {

void mixColumns(uint8_t ***matrix, std::size_t blockIndex, const uint8_t mixMatrix[4][4]) {
    uint8_t output[4][4]{}; 

    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            output[i][j] = 0;
            for (uint8_t k = 0; k < 4; k++) {
                //output[i][j] ^= GFMUL_LOOKUP[MIX_MATRIX[i][k]][matrix[blockIndex][k][j]];
                output[i][j] ^= gfmul(mixMatrix[i][k], matrix[blockIndex][k][j]);
            }
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            matrix[blockIndex][i][j] = output[i][j];
        }
    }
}


void stringKeyToMatrixKey(std::string key, uint8_t **keys, uint8_t genStart) {
    //wpisanie klucza
    uint32_t indexInKey = 0; 
    for (uint8_t i = 0; i < genStart; i++){
        for(uint8_t j = 0; j < 4; j++){
            //dzieli stringa key na fragmenty po 2 znaki i konwertuje na system szesnastkowy
            keys[j][i] = std::stoi(key.substr(indexInKey, 2), 0, 16);
            indexInKey += 2;
        }

    }
}

void generateKeyWord(int wordIndex, uint8_t **keys, uint8_t fpk){
    uint32_t i_4 = wordIndex - fpk, i_1 = wordIndex - 1;
    uint8_t word[4] = { 0, 0, 0, 0 };


    if (wordIndex % fpk == 0){
        word[0] = keys[1][i_1];
        word[1] = keys[2][i_1];
        word[2] = keys[3][i_1];
        word[3] = keys[0][i_1];

        for (uint8_t i = 0; i < 4; i++){
            word[i] = S_BOX[word[i]];
        }

        word[0] ^= RC[(wordIndex / fpk) - 1];
    }
    else {
        for (uint8_t i = 0; i < 4; i++) {
            word[i] = keys[i][i_1];
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        word[i] ^= keys[i][i_4];
    }

    for (uint8_t i = 0; i < 4; i++) {
        keys[i][wordIndex] = word[i];
    }
}

void addKeyToBlock(std::size_t blockIndex, uint8_t roundKeyNumber, uint8_t ***matrix, uint8_t **keys){
    for(uint8_t i = 0; i < 4; i++){
        for(uint8_t j = 0; j < 4; j++){
            matrix[blockIndex][i][j] ^= keys[i][j + (4 * roundKeyNumber)];
        }
    }
}   

void splitToBlocks(std::string text, std::size_t length, uint8_t ***matrix, std::size_t numberOfBlocks) {
    std::size_t currentChar = 0;

    for (std::size_t i = 0; i < numberOfBlocks; i++){

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

}