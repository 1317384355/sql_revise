#include "../include/widget/PictureDialog.h"
#include "../include/widget/ShowPic.h"
#include "../include/db/picture.h"
#include <qmath.h>
#include <QScrollBar>
#define GAP 10
#define MIN_SPACING 2
#define PIC_WIDTH 100
#define PIC_HEIGHT 100

PictureDialog::PictureDialog(QList<Picture> _pictures, QWidget *parent) : QDialog(parent)
{
    this->pictures = qMove(_pictures);
    this->setMinimumSize(150, 150);
    // 不显示边框
    // this->setWindowFlags(Qt::FramelessWindowHint);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWindowFlags(Qt::FramelessWindowHint);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 始终不显示水平滑块
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    QWidget *m_pContainer = new QWidget(this);
    m_pContainer->setWindowFlags(Qt::FramelessWindowHint);
    m_pContainer->setStyleSheet("background-color : #343a40;");
    // m_pContainer->setStyleSheet("background-color: rgb(0,255,0);");
    scrollArea->setWidget(m_pContainer);

    m_pLayout = new QGridLayout(m_pContainer);
    m_pLayout->setAlignment(Qt::AlignTop);
    m_pContainer->setLayout(m_pLayout);

    for (const auto &pic : pictures)
    {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(PIC_WIDTH, PIC_HEIGHT);
        btn->setIconSize(QSize(PIC_WIDTH, PIC_HEIGHT));
        btn->setStyleSheet("QPushButton{background-color: transparent;}\
                                    QPushButton:hover{border:1px solid #4dabf7;\
                                    background-color : rgba(165, 216, 255, 80)}");
        btn->setCursor(Qt::PointingHandCursor);
        QPixmap pix(pic.address);
        if (pix.isNull())
        {
            pix.load("./icon/error.png");
        }
        else
        {
            connect(btn, &QPushButton::clicked, [this, pic] { //
                ShowPic showDialog;
                showDialog.showPic(pic.address);
            });
        }
        btn->setIcon(QIcon(pix));
        btns.append(btn);
    }

    int size = qMax(qCeil(qSqrt(pictures.size())), 2) * PIC_WIDTH;
    this->resize(size, size);
}

PictureDialog::~PictureDialog()
{
}

void PictureDialog::resizeEvent(QResizeEvent *event)
{
    // 计算 可放置控件 区域宽度
    int screenWidth = scrollArea->width() - GAP;
    if (scrollArea->verticalScrollBar()->isVisible())
    {
        screenWidth -= scrollArea->verticalScrollBar()->width();
    }
    // 计算每行按钮的数量
    int newPerRowCount = (screenWidth + MIN_SPACING) / (PIC_WIDTH + MIN_SPACING);

    // 如果每行控件数量发生变化, 则清空并重新填入
    if (newPerRowCount != qMin(m_pLayout->columnCount(), m_pLayout->count()))
    {
        // 清空lyout
        while (m_pLayout->takeAt(0) != nullptr)
            ;

        // 重新填入
        int curRow = 0;
        int curCol = 0;
        for (auto btn : btns)
        {
            m_pLayout->addWidget(btn, curRow, curCol++);
            if (curCol == newPerRowCount)
            {
                curCol = 0;
                curRow++;
            }
        }
    }

    // 计算间隙
    int spacing = (screenWidth - newPerRowCount * PIC_WIDTH) / qMax(newPerRowCount, 1);
    m_pLayout->setHorizontalSpacing(spacing);
}