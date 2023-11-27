#ifndef USERDATA_H
#define USERDATA_H

#include <QDialog>
#include "command.h"

namespace Ui
{
    class UserData;
}

class UserData : public QDialog
{
    Q_OBJECT

signals:
    void logOut();

public:
    explicit UserData(QWidget *parent = nullptr);
    ~UserData();

    void init(QStringList *curUserData);

private slots:
    void on_btnConfirm_clicked();
    void on_btnProfile_clicked();
    void on_btnLogOut_clicked();

private:
    Ui::UserData *ui;
    QString profilePath;

    QStringList *curUserData;
};

#endif // USERDATA_H
