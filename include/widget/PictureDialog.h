#pragma once
#include <QDialog>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>

class Picture;

class PictureDialog : public QDialog
{
    Q_OBJECT
public:
    PictureDialog(QList<Picture> _pictures, QWidget *parent = nullptr);
    ~PictureDialog();

protected:
    // 重写虚函数
    void resizeEvent(QResizeEvent *event) override;

private:
    QList<Picture> pictures;
    QVector<QPushButton *> btns;
    QScrollArea *scrollArea;
    QGridLayout *m_pLayout;
};
