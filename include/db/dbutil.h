#pragma once

#include <QSqlDatabase>
#include "../include/db/command.h"

// 默认数据库连接名称
#define DEFAULT_DATABASE "main"

/**
 * @ brief：数据库工具
 * @ author：Jaly
 * @ date：2022-05-05
 */
class DbUtil
{
    DECLARE_SINGLETON(DbUtil)

private:
    DbUtil();
    bool create(QSqlDatabase &db, const QString &name, const QString &path);
    QMap<QString, QSqlDatabase *> dbMap;

public:
    ~DbUtil();

    /**
     * @brief 打开数据库
     * @param path
     * @param name
     * @return
     */
    bool open(const QString &path, const QString &name = DEFAULT_DATABASE);

    /**
     * @brief 按名称查找数据库
     * @param name
     * @return
     */
    QSqlDatabase *database(const QString &name = DEFAULT_DATABASE);

    /**
     * @brief 根据数据库名称启动一个事务
     * @param name
     * @return
     */
    QxTransaction *transactionStart(const QString &name = DEFAULT_DATABASE);

    /**
     * @brief 完成事务
     * @param trans
     */
    void transactionFinish(QxTransaction *trans);
};
