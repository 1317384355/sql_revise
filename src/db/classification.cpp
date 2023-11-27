#include "../include/db/classification.h"
#include "../include/widget/command.h"
#include "../include/db/admin.h"

/*--------------------Classification--------------------*/
QX_REGISTER_CPP_QX_PRO(Classification)

namespace qx
{
    template <>
    void register_class(QxClass<Classification> &t)
    {
        t.setName("Classification");
        t.id(&Classification::id, "id", 0);
        t.data(&Classification::group_id, "group_id", 0);
        t.data(&Classification::group_name, "group_name", 0);
        t.data(&Classification::last_modifier, "last_modifier", 0);
        t.data(&Classification::last_time, "last_time", 0);
        t.data(&Classification::background, "background", 0);
        t.data(&Classification::number_id, "number_id", 0);
    }
}
QStringList Classification::toStringList() const
{
    QStringList ret;
    ret << this->group_name
        << this->last_modifier
        << this->last_time
        << this->background;
    return ret;
}
/*--------------------Classification--------------------*/

QSqlError ClassificationDao::insert_default(Admin &admin, bool *isSuccess)
{
    QList<Classification> list;
    QSqlError error = ClassificationDao::findAll(list, admin.id);

    if (isSuccess != nullptr)
        *isSuccess = (list.size() == 0); // size == 0 表示表中无该账号数据, 可以新建

    if (error.isValid() || list.size() != 0)
        return error;

    for (int i = 0; i < GROUP_NUM; i++) // 插入(group_id)0-6共7组数据,
    {
        list.append(Classification(i, QString::number(i), admin.number, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"), QString(), admin.id));
    }
    return QxDao::insert(list, DbUtil::instance()->database());
}

QSqlError ClassificationDao::update(QList<Classification> &classification_list)
{
    QList<Classification> list;
    QSqlError error = ClassificationDao::findAll(list, classification_list.first().number_id);
    if (error.isValid() || list.size() < GROUP_NUM)
        return error;

    // 由 group_id 对应以取得 最新的 id
    for (auto &classific : classification_list)
    {
        classific.id = list.at(classific.group_id).id;
    }
    return QxDao::update(classification_list, DbUtil::instance()->database());
}

QSqlError ClassificationDao::remove(Classification &classification)
{
    return QSqlError();
}

QSqlError ClassificationDao::findAll(QList<Classification> &classifications, int number_id)
{
    qx_query query;
    query.where("number_id").isEqualTo(number_id).orderAsc("group_id");
    return QxDao::fetch_by_query(query, classifications, DbUtil::instance()->database());
}
