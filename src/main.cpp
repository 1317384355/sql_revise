#include "../include/db/dbutil.h"
#include "../include/widget/login.h"
#include <QApplication>

#define DB_PATH "C:/Users/hanxu/Documents/SQLite/sqlite1.db"

int curUserId = 0;
QString curUserName;

int main(int argc, char *argv[])
{
    // 初始化数据库
    DbUtil::instance()->open(DB_PATH);
    QApplication a(argc, argv);
    Login l;
    l.show();
    return a.exec();
}
