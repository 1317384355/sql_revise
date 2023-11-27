#pragma once
#include <QDialog>
#include <QLabel>

class ShowPic : public QDialog
{
private:
    QLabel *label;
    QPixmap pix;
    double ratio;

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

public:
    ShowPic(QWidget *parent = nullptr);
    ~ShowPic();

    int showPic(const QString &path);
};
