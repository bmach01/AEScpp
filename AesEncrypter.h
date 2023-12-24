#include "AesCommon.h"

class AesEncrypter : protected AesCommon {
    private:
        void shiftRows(std::size_t blockIndex) override;
    public:
        AesEncrypter(keyLength ver);
        std::string encrypt(std::string plainText, std::string key);

};