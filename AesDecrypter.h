#include "AesCommon.h"

class AesDecrypter : protected AesCommon {
    private:
        void shiftRows(std::size_t blockIndex) override;
    public:
        AesDecrypter(keyLength ver);
        std::string decrypt(std::string cipherText, std::string key);

};