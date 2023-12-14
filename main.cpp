#include <iostream>
#include <math.h>
#include <iomanip>

//#include "aesTests.hpp"
#include "aes.h"


std::string hexStringToChars(const std::string& hexString) {
    if (hexString.length() % 2 != 0)
        return "";

    std::string charString;

    for (size_t i = 0; i < hexString.length(); i += 2) {
        std::string hexPair = hexString.substr(i, 2);

        unsigned int hexValue;
        std::istringstream(hexPair) >> std::hex >> hexValue;

        charString.push_back(static_cast<char>(hexValue));
    }

    return charString;
}


int main() {
    std::string plainText = "Pozdrawiamy Pana Banasika";
    std::string keyString128 = "00000000000000000000000000000000";
    std::string keyString192 = "8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b";
    std::string keyString256 = "0000000000000000000000000000000000000000000000000000000000000000";

    std::string keyString = keyString128;

    std::cout << "Plain text: " << plainText << "\n";

    std::string cipherText = aes::encrypt(plainText, keyString, aes::AES_128);

    std::cout << "Cipher text: " << cipherText << "\n";

   std::string decoded = aes::decrypt(cipherText, keyString, aes::AES_128);

    std::cout << "Decoded text: " << decoded << "\n";



    return 0;
}