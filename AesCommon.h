#pragma once
#include <stdint.h>
#include <string>
#include <sstream>
#include <math.h>
#include <stdint.h>

#include "aesLookup.hpp"

enum keyLength {
    AES_128 = 10,
    AES_192 = 12,
    AES_256 = 14
};

class AesCommon {
    private:
        // Tools
        static uint8_t gfmul(uint8_t a, uint8_t b);
        static void rotWord(uint8_t word[4]);

        // Common
        void generateKeyWord(int wordIndex);
        void generateKeyWord256(int wordIndex);

    protected:
        uint8_t ***matrix = nullptr;
        uint8_t **keyMatrix = nullptr;
        const uint8_t (*pMixMatrix)[4] = nullptr;
        const uint8_t *pSubBox = nullptr;

        keyLength version = AES_128;
        uint8_t genStart = 0;
        uint8_t subSize = 0;
        std::size_t numOfBlocks = 0;

        // Tools
        void allocateMatrixes();
        void deallocateMatrixes();

        // Common
        void mixColumns(std::size_t blockIndex);
        void splitToBlocks(std::string text, std::size_t length);
        void addKeyToBlock(std::size_t blockIndex, uint8_t roundKeyNumber);
        void stringKeyToMatrixKey(std::string key);
        void subBytes(std::size_t blockIndex);

        // New
        void generateKeyMatrix();
        AesCommon(keyLength version, const uint8_t mixMatrix[4][4], const uint8_t sBox[255]);
        ~AesCommon();

        // Specific
        virtual void shiftRows(std::size_t blockIndex);
};
