#pragma once
#include "../include/db/dbutil.h"

class QX_PRO_DLL_EXPORT Admin
{
public:
    // 默认构造
    Admin(int _id = 0) : id(_id), role_id(1) {}

    // 修改登录时间
    Admin(int _id, QString _login_time) : id(_id), login_time(_login_time) {}

    // 查找用
    Admin(QString _number) : id(0), number(_number) {}

    // 注册用
    Admin(QString _user_name, QString _number, QString _password, int _role_id)
        : id(0), user_name(_user_name), number(_number), password(_password), role_id(_role_id) {}

    // 修改个人数据用
    Admin(int _id, QString _user_name, QString _describe, QString _profile)
        : id(_id), user_name(_user_name), describe(_describe), profile(_profile) {}

    int id;             // id
    QString user_name;  // 用户名
    QString number;     // 账号
    QString password;   // 密码
    QString describe;   // 用户简介
    QString login_time; // 用户上次登录时间
    QString profile;    // 用户头像路径
    int role_id;        // 用户权限

    void copy_from(const Admin &src);
    QStringList toStringList();
    bool operator==(const Admin &admin) const;
};
QX_REGISTER_PRIMARY_KEY(Admin, int)
QX_REGISTER_HPP_QX_PRO(Admin, qx::trait::no_base_class_defined, 0)

class AdminDao
{
    DECLARE_SINGLETON(AdminDao)
private:
    AdminDao() {}

public:
    // 注册
    QSqlError insert(Admin &admin, bool *isSuccess = nullptr);

    // 更新用户数据(通过number)
    QSqlError update(Admin &admin, bool *isSuccess = nullptr);

    // 删除账号(通过number)
    QSqlError remove_byNumber(const QString &number, bool *isSuccess = nullptr);

    // 通过number查找
    QSqlError find_byNumber(Admin &admin);

    // 通过id查找
    QSqlError find_byId(Admin &admin);
};