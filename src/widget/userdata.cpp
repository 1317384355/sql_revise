#include "../include/widget/userdata.h"
#include "../include/db/admin.h"
#include "ui_userdata.h"

extern int curUserId;

UserData::UserData(QWidget *parent) : QDialog(parent), ui(new Ui::UserData)
{
    ui->setupUi(this);
    connect(ui->btnCancel, &QPushButton::clicked, this, &UserData::reject);
}

UserData::~UserData()
{
    delete ui;
}

// 初始化 更新用户数据
void UserData::init(QStringList *_curUserData)
{
    curUserData = _curUserData;
    // 设置头像
    QPixmap pixmap(curUserData->at(UserDataSequence::USER_PROFILE_PIC));
    if (pixmap.isNull())
    {
        pixmap = QPixmap("./icon/headportrait.png");
    }
    ui->btnProfile->setIcon(QIcon(pixmap));

    ui->lineEdit_username->setText(curUserData->at(UserDataSequence::USER_NAME));
    ui->lineEdit_number->setText(curUserData->at(UserDataSequence::USER_NUMBER));
    ui->lineEdit_describe->setText(curUserData->at(UserDataSequence::USER_DESCRIBE));
    ui->lineEdit_lastLoginTime->setText(curUserData->at(UserDataSequence::USER_LAST_LOGIN_TIME));

    // ui->lineEdit_role->setText(curUserData->at(UserDataSequence::USER_ROLE));
    int role_ID = curUserData->at(UserDataSequence::USER_ROLE).toInt();
    QString role;
    switch (role_ID)
    {
    case 1:
        role = "会员";
        break;
    case 2:
        role = "管理员";
        break;
    case 3:
        role = "超级管理员";
        break;
    }
    ui->lineEdit_role->setText(role);
}

// 头像点击事件, 用于更换头像
void UserData::on_btnProfile_clicked()
{
    ui->btnProfile->setEnabled(false);
    // 得到用户选择头像相对路径
    QDir currentDir = QDir::currentPath();
    QString pixPath = currentDir.relativeFilePath(QFileDialog::getOpenFileName(this, "Open File", "./", "Images (*.png *.jpg *.bmp)"));
    QPixmap pixmap(pixPath);
    if (pixmap.isNull() != true)
    {
        profilePath = pixPath;
        ui->btnProfile->setIcon(QIcon(pixmap));
    }
    ui->btnProfile->setEnabled(true);
}

// 退出登录按钮的点击事件将发出 logOut 信号
void UserData::on_btnLogOut_clicked()
{
    this->reject();
    emit this->logOut();
}

// 确认保存当前修改, 将上传数据并accpet()
void UserData::on_btnConfirm_clicked()
{
    { // updateUserData
        Admin admin(curUserId, ui->lineEdit_username->text(), ui->lineEdit_describe->text(), profilePath);
        AdminDao::instance()->update(admin);
    }
    curUserData->replace(UserDataSequence::USER_PROFILE_PIC, profilePath);
    curUserData->replace(UserDataSequence::USER_NAME, ui->lineEdit_username->text());
    curUserData->replace(UserDataSequence::USER_DESCRIBE, ui->lineEdit_describe->text());

    this->accept();
}
