#include "../include/widget/login.h"
#include "../include/db/cookie.h"
#include "../include/db/admin.h"
#include "../include/db/MD5.h"
#include "../include/widget/signup.h"

extern int curUserId;
extern QString curUserName;

Login::Login(QDialog *parent) : QDialog(parent), ui(new Ui::Login)
{
    ui->setupUi(this);
    /*--------------------*/
    this->setWindowTitle("登录");
    ui->lineEdit_number->setPlaceholderText("请输入用户名");
    ui->lineEdit_password->setPlaceholderText("请输入密码");
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    /*--------------------*/
    completer = new QCompleter(this);
    completer->setCaseSensitivity(Qt::CaseInsensitive); // 设置忽略大小写, Qt::CaseInSensitive不区分大小写
    completer->setMaxVisibleItems(6);                   // 设置 最大可见项目数 为6
    ui->lineEdit_number->setCompleter(completer);       // 使lineEdit提供自动完成
    connect(completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), this, QOverload<const QModelIndex &>::of(&Login::on_activatedCompletion));
    /*--------------------*/
    on_init(); // 数据初始化
}

Login::~Login()
{
    if (widget != nullptr)
        widget->deleteLater();
    delete ui;
}

// 初始化 更新用于登录账号数据
void Login::on_init()
{
    getData_formCookieList(); // 更新当前自动补全用 账户列表 以及 密码列表
    /*--------------------*/
    // 上次登录的账号
    ui->lineEdit_number->setText(number_list.at(0));

    // 查找该账号密码(曾经保存)
    int index = number_list.indexOf(number_list.at(0), 1);                     // at(0) 为上次登录账号, 故从 1 开始找
    ui->lineEdit_password->setText(password_list.at(index == -1 ? 0 : index)); // 上次登录账号的密码(曾经保存)

    number_text_size = number_list.at(0).size(); // 记录当前lineEdit中文本长度
    /*--------------------*/
    number_list.pop_front(); // front 为上次登录账号数据, 自动填充model中不显示
    password_list.pop_front();
    if (listModel == nullptr)
        listModel = new QStringListModel(number_list);
    else
        listModel->setStringList(number_list);
    completer->setModel(listModel); // 填充数据来源模型
    /*--------------------*/
    this->show();
}

// 注册
void Login::on_btnSignUp_clicked()
{
    ui->btnSignUp->setEnabled(false);
    SignUp signUp;
    if (signUp.exec() == QDialog::Accepted)
    {
        // 注册成功
        qDebug() << "---sign up success---";
        ui->lineEdit_number->setFocus();
    }
    ui->btnSignUp->setEnabled(true);
}

// 登录
void Login::on_btnSignIn_clicked()
{
    QString number = ui->lineEdit_number->text();
    QString password = ui->lineEdit_password->text();
    if (number.isEmpty() || password.isEmpty())
    {
        return;
    }
    ui->btnSignIn->setEnabled(false);
    /*--------------------*/
    if (signInCheck(number, password))
    {
        // 登录成功
        qDebug() << "---sign in success---";
        { // 更新 账号登录时间
            Admin admin(curUserId, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            AdminDao::instance()->update(admin);
        }

        { // 更新本地缓存
            Cookie cookie(0, number, ui->checkBox->isChecked() ? password : QString());
            CookieDao::instance()->update(cookie);
        }
        if (widget == nullptr)
        {
            widget = new Widget;
            QApplication::connect(this, &Login::accepted, widget, &Widget::on_init);
            QApplication::connect(widget, &Widget::logOut, this, &Login::on_init);
        }
        this->accept();
    }
    else
    {
        // 登录失败
        qDebug() << "---sign in fail---";
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
        ui->lineEdit_password->clear();
        ui->lineEdit_password->setFocus();
    }
    /*--------------------*/
    ui->btnSignIn->setEnabled(true);
}

// 当 账号 栏内容变化时, 使其仅在 text长度减少时 清空密码栏
void Login::on_lineEdit_number_textEdited(const QString &arg1)
{
    if (arg1.size() < number_text_size)
    {
        ui->lineEdit_password->clear();
    }
    number_text_size = arg1.size();
}

// 自动填充被触发时, 填充选取账号所对应密码
void Login::on_activatedCompletion(const QModelIndex &index)
{
    ui->lineEdit_password->clear();
    ui->lineEdit_password->setText(password_list.at(index.row())); // 填充当前账号所对应密码
    number_text_size = ui->lineEdit_number->text().size();
}

// 检查账号密码是否正确, 或将(非MD5)密码进行编码
inline bool Login::signInCheck(const QString &number, QString &password)
{
    Admin admin(number);
    AdminDao::instance()->find_byNumber(admin);

    curUserId = admin.id;
    curUserName = admin.user_name;

    if (admin.password == password)
        return true;
    else if (admin.password == MD5::encode(password))
    {
        password = MD5::encode(password);
        return true;
    }

    return false;
}

// 更新当前自动补全用 账户列表 以及 密码列表
inline void Login::getData_formCookieList()
{
    QList<Cookie> cookie_list;
    CookieDao::instance()->findAll(cookie_list);
    number_list.clear();
    password_list.clear();
    for (const auto &cookie : cookie_list)
    {
        number_list.append(cookie.number);
        password_list.append(cookie.password);
    }
}
