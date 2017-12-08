#ifndef AUTORECORD_H
#define AUTORECORD_H


class AutoRecord
{
public:
    AutoRecord();
    ~AutoRecord();

private:
    AutoRecord(const AutoRecord &);
    const AutoRecord & operator = (const AutoRecord &);
};

#endif // AUTORECORD_H
