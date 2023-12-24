#include <iostream>

#include "AesEncrypter.h"
#include "AesDecrypter.h"

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