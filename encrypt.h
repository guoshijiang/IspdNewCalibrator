#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <QString>

const static unsigned int g_ispd_id_map_table[34] =
{
    'Q', '9', 'Y', 'V', '2', 'F', 'N', 'Z', '5', 'U',
    'A', '6', '8', 'B', '4', 'J', '1', 'D', 'C', 'K',
    'M', 'L', 'P', '3', 'S', 'T', 'W', 'H', 'G', 'E',
    'X', '0', 'R', '7',
};

class Encrypt
{
public:
    Encrypt();
    ~Encrypt();
    QString encrypt_ispd_id(unsigned int id);
    unsigned int dencrypt_ispd_id(const std::string& id);
private:
    Encrypt(const Encrypt&);
    Encrypt(const Encrypt &&);
    const Encrypt & operator=(const Encrypt &);
    const Encrypt & operator=(const Encrypt &&);
};

#endif // ENCRYPT_H
