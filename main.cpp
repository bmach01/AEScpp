#include <iostream>

#include "AesEncrypter.h"
#include "AesDecrypter.h"

int main() {

    std::string plainText = "Lorem ipsum sit dolor et amet???";
    std::string keyString128 = "00000000000000000000000000000000";
    std::string keyString192 = "000000000000000000000000000000000000000000000000";
    std::string keyString256 = "0000000000000000000000000000000000000000000000000000000000000000";

    AesEncrypter encrypter(keyLength::AES_192);
    AesDecrypter decrypter(keyLength::AES_192);

    std::cout << "plain text: " << plainText << "\n";

    std::string cipherText = encrypter.encrypt(plainText, keyString192);
    std::cout << "cipher text: " << cipherText << "\n";

    std::string decryptedText = decrypter.decrypt(cipherText, keyString192);
    std::cout << "decrypted: " << decryptedText << "\n";

    return 0;
}