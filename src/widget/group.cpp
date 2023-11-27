#include "../include/widget/group.h"
#include "../include/db/classification.h"
#include <QDateTime>
#define WIDGET_RELATIVE_LOCTION 3

extern int curUserId;
extern QString curUserName;

Group::Group(QVector<QStringList> *_data, QWidget *parent) : QDialog(parent), ui(new Ui::Group), data(_data)
{
    ui->setupUi(this);
    data_tmp = QVector<QVector<QString>>(GROUP_NUM, QVector<QString>(GROUP_DATA_NUM));

    for (int i = 0; i < GROUP_NUM; i++)
    {
        // 组名称
        lineEdit[i] = new QLineEdit(this);
        lineEdit[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        lineEdit[i]->setAlignment(Qt::AlignCenter);
        connect(lineEdit[i], &QLineEdit::textEdited, this, [this, i](const QString &arg) { // 修改组名称
            if (arg.isEmpty() == false)
            {
                data_tmp[i][GroupDataSequence::GROUP_NAME] = arg;
                data_tmp[i][GroupDataSequence::GROUP_LAST_MODIFIER] = curUserName;
                data_tmp[i][GroupDataSequence::GROUP_LAST_UPDATE_TIME] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                if (data_tmp[i][GroupDataSequence::GROUP_BACKGROUND_PIC].isEmpty())
                {
                    data_tmp[i][GroupDataSequence::GROUP_BACKGROUND_PIC] = data->at(i).at(GroupDataSequence::GROUP_BACKGROUND_PIC);
                }
            }
        });
        ui->g_layout->addWidget(lineEdit[i], i + WIDGET_RELATIVE_LOCTION, 0);
        // i + 3 是因为在该layout中, 前三行分别是 line , label(标题) , line

        // 组更新人
        lbUpdateNumber[i] = new QLabel(this);
        lbUpdateNumber[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        lbUpdateNumber[i]->setAlignment(Qt::AlignCenter);
        ui->g_layout->addWidget(lbUpdateNumber[i], i + WIDGET_RELATIVE_LOCTION, 1);

        // 组更新事件
        lbUpdateTime[i] = new QLabel(this);
        lbUpdateTime[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        lbUpdateTime[i]->setAlignment(Qt::AlignCenter);
        ui->g_layout->addWidget(lbUpdateTime[i], i + WIDGET_RELATIVE_LOCTION, 2);

        // 组背景
        btnBackground[i] = new QPushButton(this);
        btnBackground[i]->setFixedSize(64, 64);
        btnBackground[i]->setStyleSheet("background-color: transparent;");
        btnBackground[i]->setIconSize(QSize(64, 64));
        connect(btnBackground[i], &QPushButton::clicked, this, [this, i]() { // 修改组背景图
            btnBackground[i]->setEnabled(false);
            QString bg_path = curDir.absoluteFilePath(QFileDialog::getOpenFileName(this, "选择背景图片", ".", "图片(*.png *.jpg *.bmp)"));
            if (bg_path.isEmpty())
                return;

            QPixmap bg_pixmap(bg_path);
            if (bg_pixmap.isNull() == false)
            {
                if (data_tmp[i][GroupDataSequence::GROUP_NAME].isEmpty())
                {
                    data_tmp[i][GroupDataSequence::GROUP_NAME] = data->at(i).at(GroupDataSequence::GROUP_NAME);
                }
                data_tmp[i][GroupDataSequence::GROUP_LAST_MODIFIER] = curUserName;
                data_tmp[i][GroupDataSequence::GROUP_LAST_UPDATE_TIME] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                data_tmp[i][GroupDataSequence::GROUP_BACKGROUND_PIC] = bg_path;

                btnBackground[i]->setStyleSheet("background-color: transparent;");
                btnBackground[i]->setIcon(QIcon(bg_path));
            }
            btnBackground[i]->setEnabled(true);
        });
        ui->g_layout->addWidget(btnBackground[i], i + WIDGET_RELATIVE_LOCTION, 3);
    }

    for (int i = 0; i < GROUP_NUM; i++)
    {
        lineEdit[i]->setText(data->at(i)[GroupDataSequence::GROUP_NAME]);
        lbUpdateNumber[i]->setText(data->at(i)[GroupDataSequence::GROUP_LAST_MODIFIER]);
        lbUpdateTime[i]->setText(data->at(i)[GroupDataSequence::GROUP_LAST_UPDATE_TIME]);
        QPixmap pixmap(data->at(i)[GroupDataSequence::GROUP_BACKGROUND_PIC]);
        if (pixmap.isNull() != true)
        {
            btnBackground[i]->setIcon(QIcon(pixmap));
        }
        else
        {
            btnBackground[i]->setStyleSheet("border: 1px solid #000000;");
        }
    }
}

Group::~Group()
{
    delete ui;
}

void Group::on_btnConfirm_clicked()
{
    QList<Classification> classification_list;
    for (int i = 0; i < GROUP_NUM; i++) // 更新本地数据
    {
        if (data_tmp[i][GroupDataSequence::GROUP_NAME].isEmpty() != true)
        {
            (*data)[i][GroupDataSequence::GROUP_NAME] = data_tmp[i][GroupDataSequence::GROUP_NAME];
            (*data)[i][GroupDataSequence::GROUP_LAST_MODIFIER] = data_tmp[i][GroupDataSequence::GROUP_LAST_MODIFIER];
            (*data)[i][GroupDataSequence::GROUP_LAST_UPDATE_TIME] = data_tmp[i][GroupDataSequence::GROUP_LAST_UPDATE_TIME];
            (*data)[i][GroupDataSequence::GROUP_BACKGROUND_PIC] = data_tmp[i][GroupDataSequence::GROUP_BACKGROUND_PIC];

            classification_list.append(Classification(i, data_tmp[i][GROUP_NAME], data_tmp[i][GROUP_LAST_MODIFIER], data_tmp[i][GROUP_LAST_UPDATE_TIME], data_tmp[i][GROUP_BACKGROUND_PIC], curUserId));
        }
    }
    ClassificationDao::instance()->update(classification_list); // 上传更新数据
    this->accept();
}

void Group::on_btnCancel_clicked()
{
    this->reject();
}
