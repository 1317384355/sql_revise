#pragma once

#include <QWidget>
#include <QWidgetAction>
#include <QFileDialog>
#include <QPixmap>
#include <QVector>
#include <QComboBox>
#include "ui_widget.h"

class UserData;
QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
signals:
    void logOut();
public slots:
    void on_init();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnLogOut_clicked();

    void on_btnGroup_clicked();
    void on_btnProfile_clicked();
    void on_btnUsername_clicked();

    void on_btnAdd_clicked();
    void on_btnAll_clicked();

private:
    void updateGroupData();

    inline void bind_btn_clicked();

    Ui::Widget *ui;
    QDir currentDir;
    /*--------------------*/
    QAction action; // 搜索框按钮
    /*--------------------*/
    UserData *userDataDialog = nullptr; // 用户数据展示窗体
    QStringList curUserData;            // 当前用户数据
    /*--------------------*/
    QVector<QStringList> curGroupData;
    /*--------------------*/
};
