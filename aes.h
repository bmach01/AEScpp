#include <stdint.h>
#include <iostream>
#include <math.h>
#include <iomanip>

#include "aesLookup.hpp"

namespace aes {

    enum keyLength {
        AES_128 = 10,
        AES_192 = 12,
        AES_256 = 14
    };

    // Cipher
    void shiftRows(uint8_t ***matrix, std::size_t blockIndex);
    void subBytes(uint8_t ***matrix, std::size_t blockIndex);
    uint8_t gfmul(uint8_t a, uint8_t b);
    std::string encrypt(std::string plainText, std::string key, keyLength turns);

    // Decipher
    void InvSubBytes(uint8_t ***matrix, std::size_t blockIndex);
    void InvShiftRows(uint8_t ***matrix, std::size_t blockIndex);
    std::string decrypt(std::string cipher, std::string key, keyLength turns);

    // Common
    void mixColumns(uint8_t ***matrix, std::size_t blockIndex, const uint8_t mixMatrix[4][4]);
    void generateKeyWord(int wordIndex, uint8_t **keyMatrix, uint8_t fpk);
    void addKeyToBlock(std::size_t blockIndex, uint8_t roundKeyNumber, uint8_t ***matrix, uint8_t **keys);
    void splitToBlocks(std::string text, std::size_t length, uint8_t ***matrix, std::size_t numberOfBlocks);
    void stringKeyToMatrixKey(std::string key, uint8_t **keyMatrix, uint8_t genStart);

    // Extra
    uint8_t*** create3DArray(int dim1, int dim2, int dim3);
    void delete3DArray(uint8_t*** array, int dim1, int dim2);
}

