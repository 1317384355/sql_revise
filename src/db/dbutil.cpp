#include "../include/db/dbutil.h"

DbUtil::DbUtil()
{
}

DbUtil::~DbUtil()
{
    for (auto it = dbMap.begin(); it != dbMap.end(); it++)
    {
        it.value()->close();
        delete it.value();
    }
}

bool DbUtil::create(QSqlDatabase &db, const QString &name, const QString &path)
{
    db = QSqlDatabase::addDatabase("QSQLITE", name);
    db.setDatabaseName(path);
    if (db.open())
    {
        QSqlQuery query(db);
        return query.exec("PRAGMA foreign_keys = ON"); // 使外键功能生效
    }
    return false;
}

bool DbUtil::open(const QString &path, const QString &name)
{
    QSqlDatabase db;
    if (create(db, name, path))
    {
        dbMap[name] = new QSqlDatabase(db);
        return true;
    }
    return false;
}

QSqlDatabase *DbUtil::database(const QString &name)
{
    return dbMap[name];
}

QxTransaction *DbUtil::transactionStart(const QString &name)
{
    QxTransaction *tran = new QxTransaction(*dbMap[name], true);
    tran->open();
    return tran;
}

void DbUtil::transactionFinish(QxTransaction *trans)
{
    if (trans != NULL)
    {
        trans->commit();
        trans->close();
    }
}
