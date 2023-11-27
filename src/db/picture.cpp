#include "../include/db/picture.h"
/*--------------------Picture--------------------*/
QX_REGISTER_CPP_QX_PRO(Picture)
namespace qx
{
    template <>
    void register_class(QxClass<Picture> &t)
    {
        t.setName("picture");
        t.id(&Picture::id, "id", 0);
        t.data(&Picture::group_id, "group_id", 0);
        t.data(&Picture::address, "address", 0);
        t.data(&Picture::number_id, "number_id", 0);
    }
}
/*--------------------Picture--------------------*/

QSqlError PictureDao::insert(Picture &picture)
{
    return QxDao::insert(picture, DbUtil::instance()->database());
}

QSqlError PictureDao::remove(Picture &picture, bool *isSuccess)
{
    // Picture tmp(id);
    // QSqlError error = QxDao::fetch_by_id(tmp, DbUtil::instance()->database());
    // if (isSuccess != nullptr)
    //     (*isSuccess) = !tmp.number.isEmpty(); // 若为空, 即未找到该id数据, 故删除失败
    // if (error.isValid() || tmp.number.isEmpty())
    //     return error;

    // return QxDao::delete_by_id(tmp, DbUtil::instance()->database());
}

QSqlError PictureDao::find_byGroupId(QList<Picture> &pictures, int group_id, int number_id)
{
    qx_query query;
    query.where("group_id").isEqualTo(group_id).and_("number_id").isEqualTo(number_id);
    return QxDao::fetch_by_query(query, pictures, DbUtil::instance()->database());
}

QSqlError PictureDao::find_ALL(QList<Picture> &pictures, int number_id)
{
    qx_query query;
    query.where("number_id").isEqualTo(number_id).orderAsc("group_id");
    return QxDao::fetch_by_query(query, pictures, DbUtil::instance()->database());
}
