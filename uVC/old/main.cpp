#include "uVC.h"

#include <iostream>
using namespace std;

// Encrypt/Decrypt strings using a key
void encrypt(char toEncrypt[512], std::string key, int size) 
{
    // Get size of std::string and code key
    int keyLength = key.size();
    int r = 1;
    
    for (unsigned int i = 0; i < keyLength; i++)
        key[i] += 3*i;
    
    // Perform encryption/decryption for this std::string
    for (unsigned int i = 0, j = 0; i < size; i++)
    {
        toEncrypt[i] ^= (++key[j] + i) ^ pepper[j];
        if (j >= keyLength || j <= 0)
            r = -r;
        j += r;
    }
}

int main()
{
    uint64_t i = 42;
    std::string s = compress(i);
    cout << s << endl;
    cout << decompress(s) << endl;
    return 0;
}