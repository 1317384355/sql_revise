#include "../include/db/admin.h"
#include "../include/db/classification.h"
/*--------------------Admin--------------------*/
QX_REGISTER_CPP_QX_PRO(Admin)

namespace qx
{
    template <>
    void register_class(QxClass<Admin> &t)
    {
        t.setName("admin"); // 表名
        t.id(&Admin::id, "id", 0);
        t.data(&Admin::user_name, "user_name", 0);
        t.data(&Admin::number, "number", 0);
        t.data(&Admin::password, "password", 0);
        t.data(&Admin::describe, "describe", 0);
        t.data(&Admin::login_time, "login_time", 0);
        t.data(&Admin::profile, "profile", 0);
        t.data(&Admin::role_id, "role_id", 0);
    }
}

bool Admin::operator==(const Admin &admin) const
{
    if (this->number == admin.number && this->password == admin.password)
    {
        return true;
    }
    return false;
}

// 必定修改内容: role_id; 可能(若为空)修改内容: user_name, password, describe, login_time, profile
void Admin::copy_from(const Admin &src)
{
    this->role_id = role_id;

    if (this->user_name.isEmpty())
        this->user_name = src.user_name;

    if (this->password.isEmpty())
        this->password = src.password;

    if (this->describe.isEmpty())
        this->describe = src.describe;

    if (this->login_time.isEmpty())
        this->login_time = src.login_time;

    if (this->profile.isEmpty())
        this->profile = src.profile;
}
/*--------------------Admin--------------------*/

QStringList Admin::toStringList()
{
    QStringList ret;
    ret << this->user_name
        << this->number
        << this->describe
        << this->login_time
        << this->profile
        << QString::number(this->role_id);
    return ret;
}

/*--------------------AdminDao--------------------*/

QSqlError AdminDao::insert(Admin &admin, bool *isSuccess)
{
    QSqlError error;
    error = AdminDao::find_byNumber(admin); // 先查找账号是否存在

    if (isSuccess != nullptr)
        *isSuccess = (admin.id == 0); // id == 0, 说明为查找到账号, 即可以注册

    if (error.isValid() || admin.id != 0)
        return error;

    error = QxDao::insert(admin, DbUtil::instance()->database()); // 先添加账号信息数据
    if (error.isValid())
        return error;

    return ClassificationDao::instance()->insert_default(admin, isSuccess); // 再添加账号群组数据
}

QSqlError AdminDao::update(Admin &admin, bool *isSuccess)
{
    QSqlError error;
    Admin admin_temp(admin.number);
    error = AdminDao::find_byNumber(admin_temp); // 先查找到该账号数据

    if (isSuccess != nullptr)
        *isSuccess = (admin_temp.id != 0); // id != 0, 即查找到账号数据, 否则说明账号不存在

    if (error.isValid() || admin_temp.id == 0)
        return error;

    admin.copy_from(admin_temp); // 将空的数据项填充为查找到旧数据, 并替换role_id
    return QxDao::update(admin, DbUtil::instance()->database());
}

QSqlError AdminDao::remove_byNumber(const QString &number, bool *isSuccess)
{
    QSqlError error;
    Admin admin(number);
    error = find_byNumber(admin); // 先查找到该账号数据

    if (isSuccess != nullptr)
        *isSuccess = (admin.id != 0); // id != 0, 即查找到账号数据, 否则说明账号不存在

    if (error.isValid() || admin.id == 0) // 若不存在, 则不删除, 直接返回
        return error;

    return QxDao::delete_by_id(admin, DbUtil::instance()->database());
}

QSqlError AdminDao::find_byNumber(Admin &admin)
{
    qx_query query;
    query.where("number").isEqualTo(admin.number);
    return QxDao::fetch_by_query(query, admin, DbUtil::instance()->database());
}

QSqlError AdminDao::find_byId(Admin &admin)
{
    return QxDao::fetch_by_id(admin, DbUtil::instance()->database());
}
/*--------------------AdminDao--------------------*/
