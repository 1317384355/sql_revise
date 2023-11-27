#pragma once
#include "../include/db/dbutil.h"

class QX_PRO_DLL_EXPORT Cookie
{
public:
    // 默认构造
    Cookie(int _id = 0) : id(_id) {}

    // 通过number查找
    Cookie(int _id, QString _number, QString _password = QString()) : id(_id), number(_number), password(_password) {}

    int id;
    QString number;
    QString password;
};
QX_REGISTER_PRIMARY_KEY(Cookie, int)
QX_REGISTER_HPP_QX_PRO(Cookie, qx::trait::no_base_class_defined, 0)

class CookieDao
{
    DECLARE_SINGLETON(CookieDao)
private:
    CookieDao() {}

public:
    QSqlError insert(Cookie &cookie);
    QSqlError update(Cookie &cookie);
    QSqlError remove_byNumber(const QString &number);
    QSqlError find_byNumber(Cookie &cookie);
    QSqlError findAll(QList<Cookie> &cookies);
};