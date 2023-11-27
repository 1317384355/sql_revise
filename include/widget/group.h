#ifndef GROUP_H
#define GROUP_H

#include <QDialog>
#include <QLineEdit>
#include "command.h"
#include "ui_group.h"

namespace Ui
{
    class Group;
}

class Group : public QDialog
{
    Q_OBJECT

public:
    explicit Group(QVector<QStringList> *_data = nullptr, QWidget *parent = nullptr);
    ~Group();

private:
    Ui::Group *ui;
    QLineEdit *lineEdit[7];
    QLabel *lbUpdateNumber[7];
    QLabel *lbUpdateTime[7];
    QPushButton *btnBackground[7];

    QDir curDir = QDir::currentPath();
    QVector<QStringList> *data;
    QVector<QVector<QString>> data_tmp;

private slots:
    void on_btnConfirm_clicked();
    void on_btnCancel_clicked();
};

#endif // GROUP_H
