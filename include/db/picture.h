#pragma once
#include "../include/db/dbutil.h"

class QX_PRO_DLL_EXPORT Picture
{
public:
    // 默认构造
    Picture() : id(0) {}

    // 查找全部用
    Picture(int _number_id) : number_id(_number_id) {}

    // 新增, 查找用
    Picture(int _group_id, int _number_id)
        : group_id(_group_id), number_id(_number_id) {}

    int id;
    int group_id;
    QString address;
    int number_id;
};
QX_REGISTER_PRIMARY_KEY(Picture, int)
QX_REGISTER_HPP_QX_PRO(Picture, qx::trait::no_base_class_defined, 0)

class PictureDao
{
    DECLARE_SINGLETON(PictureDao)
private:
    PictureDao() {}

public:
    //
    QSqlError insert(Picture &picture);

    QSqlError remove(Picture &picture, bool *isSuccess = nullptr);

    // 查找(通过number_id)账户 特定组(group_id) 图片
    QSqlError find_byGroupId(QList<Picture> &pictures, int group_id, int number_id);

    // 查找(通过number_id)账号全部图片
    QSqlError find_ALL(QList<Picture> &pictures, int number_id);
};