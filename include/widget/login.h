#pragma once
#include <QApplication>
#include "ui_login.h"
#include "command.h"
#include "widget.h"

namespace Ui
{
    class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public slots:
    void on_init();

public:
    explicit Login(QDialog *parent = nullptr);
    ~Login();

private slots:
    void on_btnSignUp_clicked();
    void on_btnSignIn_clicked();

    void on_lineEdit_number_textEdited(const QString &arg1);
    void on_activatedCompletion(const QModelIndex &index);

private:
    Ui::Login *ui;
    Widget *widget = nullptr;

    QStringList number_list;   // 自动补全用账号
    QStringList password_list; // 自动补全用密码

    QCompleter *completer; // 提供自动补全的对象可用于lineEdit和combobox
    QStringListModel *listModel = nullptr;

    int number_text_size; // 目的: 保存line_number每次编辑text长度, 使其仅在长度减少时清空密码栏

    inline bool signInCheck(const QString &number, QString &password);
    inline void getData_formCookieList();
};
