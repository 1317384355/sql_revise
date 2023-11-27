#include "../include/db/cookie.h"
/*--------------------Cookie--------------------*/
QX_REGISTER_CPP_QX_PRO(Cookie)

namespace qx
{
    template <>
    void register_class(QxClass<Cookie> &t)
    {
        t.setName("cookie"); // 表名
        t.id(&Cookie::id, "id", 0);
        t.data(&Cookie::number, "number", 0);
        t.data(&Cookie::password, "password", 0);
    }
}
/*--------------------Cookie--------------------*/

/*--------------------CookieDao--------------------*/
// 新增数据
QSqlError CookieDao::insert(Cookie &cookie)
{
    return QxDao::insert(cookie, DbUtil::instance()->database());
}

// 更新数据(由number)
QSqlError CookieDao::update(Cookie &cookie)
{
    if (!cookie.password.isEmpty()) // 若密码不为空, 即勾选了保存密码, 故更新密码
    {
        Cookie tmp = Cookie(0, cookie.number);

        QSqlError error = CookieDao::find_byNumber(tmp);
        if (error.isValid())
            return error;

        if (tmp.id == 0) // 即缓存中无此账户信息, 故添加
        {
            error = CookieDao::insert(cookie);
            if (error.isValid())
                return error;
        }
        else // 即缓存中存在此账户信息, 故更新(通过id)
        {
            cookie.id = tmp.id;
            error = QxDao::update(cookie, DbUtil::instance()->database());
            if (error.isValid())
                return error;
        }
    }

    // 无论是否勾选保存密码, 都会更新最新登录账号
    // 更新最新登录账号(仅保存账户)
    cookie.id = 1;
    cookie.password.clear();
    return QxDao::update(cookie, DbUtil::instance()->database());
}

// 移除 账号为 number 的数据
QSqlError CookieDao::remove_byNumber(const QString &number)
{
    QSqlError error;
    Cookie cookie = Cookie(0, number);
    error = CookieDao::find_byNumber(cookie);
    if (error.isValid() || cookie.id == 0)
        return error;

    return QxDao::delete_by_id(cookie, DbUtil::instance()->database());
}

// 查找 账号为 number 的数据(查询结果id != 1)
QSqlError CookieDao::find_byNumber(Cookie &cookie)
{
    qx_query query;
    query.where("number").isEqualTo(cookie.number).and_("id").isNotEqualTo(1); // id == 1为上次登录账号
    return QxDao::fetch_by_query(query, cookie, DbUtil::instance()->database());
}

// 取得全部本地缓存数据, 用于自动填充 (at(0).number为上次登录账号, at(0).password为空)
QSqlError CookieDao::findAll(QList<Cookie> &cookies)
{
    return QxDao::fetch_all(cookies, DbUtil::instance()->database()); // 此时 at(0).password 无内容
}
/*--------------------CookieDao--------------------*/