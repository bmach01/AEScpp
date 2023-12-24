Academic project C++ AES128/192/256 ECB

Allows encryption and decryption in AES ECB with 128/192/256-bit key.
Written entirely in C++ using only standard library.

To encrypt use the AesEncrypter class object provided with key length (eg. AES_128),
then provide the text and the key to the .encrypt method which will return
encrypted string.

Similarly, use the AesDecrypter class object with the .decrypt method to decrypt
a given string.

The project was created with learning purposes in mind.
