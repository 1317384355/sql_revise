#include "../include/widget/widget.h"
#include "../include/widget/command.h"
#include "../include/widget/group.h"
#include "../include/widget/userdata.h"
#include "../include/widget/PictureDialog.h"
#include "../include/db/admin.h"
#include "../include/db/classification.h"
#include "../include/db/picture.h"

extern int curUserId;

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(930, 775); // 927   769
    QDir currentDir = QDir::current();

    bind_btn_clicked();
#if 0 // 设置搜索框按钮
    /*--------------------*/
    将btn用QWidgetAction嵌入
    btnSearch = new QPushButton;
    // btnSearch->setIcon();
    // btnSearch->setIconSize(QSize(16, 16));
    btnSearch->setCursor(Qt::PointingHandCursor); // 指针光标变为手
    QWidgetAction *widgetAction = new QWidgetAction(ui->lineEdit);
    widgetAction->setIcon(QIcon("./icon/search.png"));
    widgetAction->setDefaultWidget(btnSearch);
    ui->lineEdit->addAction(widgetAction, QLineEdit::TrailingPosition);
    connect(btnSearch, &QPushButton::clicked, [=]() { // 按钮事件
        qDebug() << "clicked";
    });
    /*--------------------*/
#else
    /*--------------------*/
    // 用action触发作为搜索框按钮
    action.setIcon(QIcon("./icon/search.png"));
    ui->lineEdit->addAction(&action, QLineEdit::TrailingPosition);
    connect(&action, &QAction::triggered, [=]() { //
        qDebug() << "clicked";
    });
    /*--------------------*/
#endif
}

Widget::~Widget()
{
    qDebug() << "~Widget()";
    delete ui;
}

// 初始化更新用户数据 并 show()
void Widget::on_init()
{
    // curUserData 为空表示当前全部用户数据为空
    if (curGroupData.isEmpty())
    {
        { // getUserData();
            Admin admin(curUserId);
            AdminDao::instance()->find_byId(admin);
            curUserData = admin.toStringList();
        }

        { // getGroupData();
            QList<Classification> clsfc_list;
            ClassificationDao::instance()->findAll(clsfc_list, curUserId);
            for (const auto &clsfc : clsfc_list)
            {
                curGroupData.append(clsfc.toStringList());
            }
        }
        this->updateGroupData();
    }

    // 设置头像
    QPixmap pixmap(curUserData.at(UserDataSequence::USER_PROFILE_PIC));
    if (pixmap.isNull())
    {
        pixmap = QPixmap("./icon/headportrait.png");
    }
    ui->btnProfile->setIcon(QIcon(pixmap));
    // ui->btnProfile->setCursor(Qt::PointingHandCursor);               // 指针光标变为手
    // ui->btnProfile->setStyleSheet("background-color: transparent;"); // 背景透明
    /*--------------------*/
    // 设置user_name
    QString userName = curUserData.at(UserDataSequence::USER_NAME);
    ui->btnUsername->setText(userName);
    // ui->btnUsername->setCursor(Qt::PointingHandCursor);               // 指向手势
    // ui->btnUsername->setStyleSheet("background-color: transparent;"); // 背景透明
    // ui->btnUsername->setStyleSheet("border: 1px solid #000000;");     // 添加边框
    /*--------------------*/
    this->show();
}

void Widget::updateGroupData()
{
    QPushButton *btn[7];
    btn[0] = ui->pushButton_1;
    btn[1] = ui->pushButton_2;
    btn[2] = ui->pushButton_3;
    btn[3] = ui->pushButton_4;
    btn[4] = ui->pushButton_5;
    btn[5] = ui->pushButton_6;
    btn[6] = ui->pushButton_7;

    for (int i = 0; i < 7; i++)
    {
        QString picPath = curGroupData.at(i).at(GroupDataSequence::GROUP_BACKGROUND_PIC);
        if (QPixmap(picPath).isNull())
        {
            picPath = "./background/1.png";
        }
        btn[i]->setStyleSheet("QPushButton{background-color: transparent;\
                                border:1px solid #dcdcdc}\
                                QPushButton:hover{border:1px solid #4dabf7;\
                                background-color : rgba(165, 216, 255, 80)}");
        btn[i]->setIcon(QIcon(picPath));
        btn[i]->setCursor(Qt::PointingHandCursor);
        // btn[i]->setText(curGroupData.at(i).at(GroupDataSequence::GROUP_NAME));
    }
}

// 退出登录
void Widget::on_btnLogOut_clicked()
{
    curGroupData.clear();
    this->hide();
    emit this->logOut();
}

// 点击分组, 查看分组信息以及编辑
void Widget::on_btnGroup_clicked()
{
    ui->btnGroup->setEnabled(false);
    Group groupDialog(&curGroupData, this);
    if (groupDialog.exec() == QDialog::Accepted)
    {
        this->updateGroupData();
    }
    ui->btnGroup->setEnabled(true);
}

// 点击头像, 更换头像
void Widget::on_btnProfile_clicked()
{
    ui->btnProfile->setEnabled(false);
    // 得到用户选择头像相对路径
    QString filePath = currentDir.relativeFilePath(QFileDialog::getOpenFileName(this, "Open File", "./", "Images (*.png *.jpg *.bmp)"));
    if (filePath.isEmpty() != true)
    {
        ui->btnProfile->setIcon(QIcon(filePath)); // 设置头像

        { // updateUserProfile(filePath) 上传更新头像
            Admin admin = Admin(curUserId, QString(), QString(), filePath);
            AdminDao::instance()->update(admin);
        }

        this->curUserData.replace(UserDataSequence::USER_PROFILE_PIC, filePath);
    }
    ui->btnProfile->setEnabled(true);
}

// 点击用户名, 展示用户信息
void Widget::on_btnUsername_clicked()
{
    if (userDataDialog == nullptr)
    {
        userDataDialog = new UserData(this);
        connect(userDataDialog, &UserData::logOut, this, &Widget::on_btnLogOut_clicked);
    }
    userDataDialog->init(&curUserData);
    if (userDataDialog->exec() == QDialog::Accepted)
    {
        this->on_init();
    }
}

// 添加图片
void Widget::on_btnAdd_clicked()
{
    QDialog addDialog;
    QVBoxLayout *vLayout = new QVBoxLayout(&addDialog);
    QWidget *addWidget = new QWidget(&addDialog);
    vLayout->addWidget(addWidget);
    QHBoxLayout *hLayout = new QHBoxLayout(addWidget);

    QString pixPath; // 用于保存图片路径
    QPushButton *btnSelect = new QPushButton("选择图片", addWidget);
    btnSelect->setFixedSize(100, 100);
    btnSelect->setIconSize(QSize(100, 100));
    connect(btnSelect, &QPushButton::clicked, btnSelect, [btnSelect, this, &pixPath]() { // 选择图片
        btnSelect->setEnabled(false);
        QString filePath = this->currentDir.absoluteFilePath(QFileDialog::getOpenFileName(this, "Open File", "./", "Images (*.png *.jpg *.bmp)"));
        if (filePath.isEmpty() != true && QPixmap(filePath).isNull() != true)
        {
            btnSelect->setText("");
            btnSelect->setStyleSheet("background-color: transparent;");
            pixPath = filePath;
            btnSelect->setIcon(QIcon(pixPath));
        }
        btnSelect->setEnabled(true);
    });
    hLayout->addWidget(btnSelect);

    // 分组选择
    QComboBox *comboBox = new QComboBox(addWidget);
    comboBox->setEditable(false);
    QStringList curGroupName;
    for (const auto &data : curGroupData)
    {
        curGroupName.append(data.at(GroupDataSequence::GROUP_NAME));
    }
    comboBox->addItems(curGroupName); // 添加组名数据
    hLayout->addWidget(comboBox);

    // 确定上传
    QPushButton *btnOk = new QPushButton("确定", addWidget);
    connect(btnOk, &QPushButton::clicked, &addDialog, &QDialog::accept);
    hLayout->addWidget(btnOk);
    addWidget->setLayout(hLayout);

    addDialog.exec();
    if (!pixPath.isEmpty())
    {
        // 添加到选中图片到相应组
        // curPicData[comboBox->currentIndex()].append(pixPath);
        // db.addPicture(pixPath, comboBox->currentIndex());
    }
}

// 查看所有图片
void Widget::on_btnAll_clicked()
{
    QList<Picture> pic_list;
    PictureDao::instance()->find_ALL(pic_list, curUserId);
    PictureDialog picDialog(qMove(pic_list), this);
    picDialog.exec();
}

// 绑定点击事件, 查看分组图片
inline void Widget::bind_btn_clicked()
{
    QPushButton *btn[7];
    btn[0] = ui->pushButton_1;
    btn[1] = ui->pushButton_2;
    btn[2] = ui->pushButton_3;
    btn[3] = ui->pushButton_4;
    btn[4] = ui->pushButton_5;
    btn[5] = ui->pushButton_6;
    btn[6] = ui->pushButton_7;

    for (int i = 0; i < 7; i++)
    {
        connect(btn[i], &QPushButton::clicked, [this, i]() { //
            QList<Picture> pic_list;
            PictureDao::instance()->find_byGroupId(pic_list, curUserId, i);
            PictureDialog picDialog(qMove(pic_list), this);
            picDialog.exec();
        });
    }
}