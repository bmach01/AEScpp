#include <iostream>
#include <math.h>
#include <iomanip>

#include "AesEncrypter.h"
#include "AesDecrypter.h"

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
    std::string plainText = "Lorem ipsum sit dolor et amet???";
    std::string keyString128 = "00000000000000000000000000000000";
    std::string keyString192 = "000000000000000000000000000000000000000000000000";
    std::string keyString256 = "0000000000000000000000000000000000000000000000000000000000000000";

    AesEncrypter encrypter(keyLength::AES_256);
    AesDecrypter decrypter(keyLength::AES_256);

    std::cout << "plain text: " << plainText << "\n";

    std::string cipherText = encrypter.encrypt(plainText, keyString256);
    std::cout << "cipher text: " << cipherText << "\n";

    std::string decryptedText = decrypter.decrypt(cipherText, keyString256);
    std::cout << "decrypted: " << decryptedText;

    return 0;
}