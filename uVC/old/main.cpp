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
	char buffer[BLOCK_SIZE];
    memset(buffer, '\0', BLOCK_SIZE);
	char *data = (char*)"test";
	ccpy(buffer, data, 0, 0, 4);
	writeBlock((char*)"data.txt", 0, buffer);
    return 0;
}