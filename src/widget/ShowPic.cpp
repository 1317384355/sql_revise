#include "../include/widget/ShowPic.h"
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QDebug>

using namespace cv;

ShowPic::ShowPic(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("图片预览");
    this->resize(300, 300);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    QWidget *contenter = new QWidget(scrollArea);
    scrollArea->setWidget(contenter);

    QHBoxLayout *m_layout = new QHBoxLayout(contenter);
    scrollArea->setLayout(m_layout);
    m_layout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 255);}");
    m_layout->addWidget(label);
}

ShowPic::~ShowPic()
{
}

int ShowPic::showPic(const QString &path)
{

    Mat img = imread(path.toStdString());
    if (img.empty())
    {
        QMessageBox::warning(this, "警告", "图片不存在");
        return QDialog::Rejected;
    }
    cvtColor(img, img, COLOR_BGR2RGB);

    pix = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888));
    ratio = (double)pix.width() / pix.height();
    update();
    return this->exec();
}

void ShowPic::resizeEvent(QResizeEvent *event)
{
    double widgetRatio = label->width() / 1.0 / label->height();
    if (widgetRatio > ratio)
    {
        label->setPixmap(pix.scaled((label->height()) * ratio, (label->height())));
    }
    else
    {
        label->setPixmap(pix.scaled((label->width()), (label->width()) / ratio));
    }
}

void ShowPic::paintEvent(QPaintEvent *event)
{
    double widgetRatio = label->width() / 1.0 / label->height();
    if (widgetRatio > ratio)
    {
        label->setPixmap(pix.scaled((label->height()) * ratio, (label->height())));
    }
    else
    {
        label->setPixmap(pix.scaled((label->width()), (label->width()) / ratio));
    }
}