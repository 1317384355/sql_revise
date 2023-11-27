#include "../include/widget/signup.h"
#include "../include/db/admin.h"
#include "../include/db/MD5.h"

SignUp::SignUp(QWidget *parent) : QDialog(parent), ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->le_name->setPlaceholderText("请输入用户名");
    ui->le_number->setValidator(new QRegExpValidator(QRegExp("[0-9]+"))); // 设置仅能输入数字
    ui->le_number->setPlaceholderText("请输入账号");
    ui->le_password->setPlaceholderText("请输入密码");
    ui->le_confirm->setPlaceholderText("请再次输入密码");
    ui->le_password->setEchoMode(QLineEdit::Password);
    ui->le_confirm->setEchoMode(QLineEdit::Password);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_btnConfirm_clicked()
{
    QString name = ui->le_name->text();
    QString numb = ui->le_number->text();
    QString pwd = ui->le_password->text();
    QString pwd_2 = ui->le_confirm->text();
    int level = ui->cbLevel->currentIndex();
    if (name.isEmpty() || numb.isEmpty() || pwd.isEmpty() || pwd_2.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please enter your name / number / password!");
        return;
    }
    if (pwd != pwd_2)
    {
        QMessageBox::warning(this, "Warning", "Password is not the same!");
        return;
    }
    ui->btnConfirm->setEnabled(false);
    /*--------------------*/
    Admin admin(name, numb, MD5::encode(pwd), level + 1);
    bool isSuccess = false;
    QSqlError error = AdminDao::instance()->insert(admin, &isSuccess);
    if (error.isValid() == false && isSuccess)
    {
        QMessageBox::information(this, "Information", "Sign up successfully!");
        QDialog::accept();
    }
    /*--------------------*/
    ui->btnConfirm->setEnabled(true);
}
