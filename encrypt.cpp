#include "encrypt.h"

Encrypt::Encrypt()
{

}

Encrypt::~Encrypt()
{

}

QString Encrypt::encrypt_ispd_id(unsigned int id)
{
    QString q_str;
    std::string str;
    do
    {
        unsigned int mod = id % 34;
        str += g_ispd_id_map_table[mod];
        id = id / 34;
    }
    while(0 != id);
    std::reverse(str.begin(), str.end());
    q_str = QString::fromStdString(str);
    return q_str;
}

unsigned int Encrypt::dencrypt_ispd_id(const std::string& id)
{
    unsigned int rtn = 0;
    size_t size = id.size();
    assert(0 != size);

    for (size_t i = size; i > 0; --i)
    {
        unsigned int j = 0;
        for(; j < 34; j++)
        {
            if (id[i-1] == g_ispd_id_map_table[j])
            break;
        }
        assert(34 != j);
        rtn += j * pow((double)34, (double)size-i);
    }
    return rtn;
}




