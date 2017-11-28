#ifndef DBINFO_H
#define DBINFO_H

#include <QSqlDatabase>
#include <QDataStream>

class DBinfo
{
public:
    DBinfo();
    ~DBinfo();
    DBinfo(const QString & db_path);

    bool init(QString & err);
    bool query(const QString & sql, QVariant & res, QString & err);
    bool update(const QString & sql, QString & err);

    void set_db_path(const QString & path);
private:
    DBinfo(const DBinfo &);
    const DBinfo & operator=(const DBinfo &);

    QString m_db_path;
    QSqlDatabase m_db;
};

#endif // DBINFO_H
