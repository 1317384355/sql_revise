#pragma once
#include "../include/db/dbutil.h"

class QX_PRO_DLL_EXPORT Classification
{
public:
    // 默认构造
    Classification() : id(0), group_id(1) {}

    // 修改用户数据用
    Classification(int _group_id, QString _group_name, QString _last_modifier, QString _last_time, QString _background, int _number_id)
        : group_id(_group_id), group_name(_group_name), last_modifier(_last_modifier), last_time(_last_time), background(_background), number_id(_number_id) {}

    int id;                // 主键,唯一
    int group_id;          // 分组id,用于区分分组
    QString group_name;    // 分组名
    QString last_modifier; // 最新修改时间
    QString last_time;     // 最新修改时间
    QString background;    // 分组背景图片
    int number_id;         // 外键, 绑定Admin.id, 查找用

    // 将类数据(group_name, last_modifier, last_time, background)导出到stringlist
    QStringList toStringList() const;
};
QX_REGISTER_PRIMARY_KEY(Classification, int);
QX_REGISTER_HPP_QX_PRO(Classification, qx::trait::no_base_class_defined, 0)

class Admin;

class ClassificationDao
{
    DECLARE_SINGLETON(ClassificationDao)
private:
    ClassificationDao() {}

public:
    // 注册账号时注册 GROUP_NUM(7) 个分组数据
    QSqlError insert_default(Admin &admin, bool *isSuccess = nullptr);

    // 更新数据(通过group_id, number_id)
    QSqlError update(QList<Classification> &classification_list);

    // 暂无法使用
    QSqlError remove(Classification &classification);

    // 查找(通过number_id)账号全部数据
    QSqlError findAll(QList<Classification> &classifications, int number_id);
};
