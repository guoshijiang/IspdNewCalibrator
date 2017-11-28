#include "dbinfo.h"

DBinfo::DBinfo(): m_db_path("../database/IspdNewCalibrator.db")
  , m_db(QSqlDatabase::contains()? QSqlDatabase::database("qt_sql_default_connection")
          : QSqlDatabase::addDatabase("QSQLITE"))
{

}

DBinfo::DBinfo(const QString &db_path) : m_db_path(db_path), m_db(QSqlDatabase::contains()
            ? QSqlDatabase::database("qt_sql_default_connection")
            : QSqlDatabase::addDatabase("QSQLITE"))
{

}

DBinfo::~DBinfo()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DBinfo::init(QString & err)
{
    m_db.setDatabaseName(m_db_path);
    if (!m_db.open())
    {
        QMessageBox::critical(0, qApp->tr("数据库打开失败"),
                              qApp->tr("请检查数据库文件存放位置，"),
                              QMessageBox::Abort);
        err = m_db.lastError().text();
        qCritical() << err;
        return false;
    }
    else
    {
        return true;
    }
}

bool DBinfo::query(const QString & sql, QVariant & res, QString & err)
{
    QSqlQuery query;
    if (!query.exec(sql))
    {
        err = query.lastError().text();
        return false;
    }
    while (query.next())
    {
        res = query.value(0).toString();
    }
    return true;
}

bool DBinfo::update(const QString &sql, QString &err)
{
    QSqlQuery query;
    if (!query.prepare(sql))
    {
        err = query.lastError().text();
        return false;
    }
    if (!query.exec())
    {
        err = query.lastError().text();
        return false;
    }
    return true;
}

void DBinfo::set_db_path(const QString &path)
{
    m_db_path = path;
}



