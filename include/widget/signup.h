#pragma once
#include "ui_signup.h"
#include "command.h"

namespace Ui
{
    class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private:
    Ui::SignUp *ui;

private slots:
    void on_btnConfirm_clicked();
};
