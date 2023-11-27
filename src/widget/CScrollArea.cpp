#include "../include/widget/CScrollArea.h"
#define GAP 10

CScrollArea::CScrollArea(QWidget *parent) : QScrollArea(parent)
{
    this->setMinimumWidth(150);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background-color: rgb(255,0,0);"); // 设置暗色背景
    this->setWidgetResizable(true);

    m_pVerticalScrollBar = this->verticalScrollBar();

    QWidget *m_pContainer = new QWidget(this);
    // m_pContainer->setStyleSheet("background-color: rgb(0,255,0);");
    this->setWidget(m_pContainer);

    m_pLayout = new QGridLayout(m_pContainer);
    m_pLayout->setAlignment(Qt::AlignTop);
    m_pContainer->setLayout(m_pLayout);

    m_pLabel = new QLabel(m_pContainer);
    m_pLabel->setAlignment(Qt::AlignCenter);
    // m_pLabel->setStyleSheet("QLabel{background-color: rgba(0, 0, 0, 255);}");
    m_pLabel->hide();

    c_widgetWidth = 100;
    c_widgetHeight = 100;

    minSpacing = 2;

    c_widgetCount = 0;

    curRow = 0;
    curCol = 0;
}

CScrollArea::~CScrollArea()
{
}

inline void CScrollArea::setWidgetSize(int width, int height)
{
    c_widgetWidth = width;
    c_widgetHeight = height;
    this->resizeEvent(nullptr); // 设置控件宽高后刷新窗口
}

void CScrollArea::setPicPath(const QStringList &list_picPath)
{
    for (const auto &picPath : list_picPath)
    {
        c_widgets.append(new QPushButton(this));
        c_widgets.last()->setFixedSize(c_widgetWidth, c_widgetHeight);
        c_widgets.last()->setIconSize(QSize(c_widgetWidth, c_widgetHeight));
        c_widgets.last()->setStyleSheet("background:transparent;");
        c_widgets.last()->setCursor(Qt::PointingHandCursor);
        QPixmap pix(picPath);
        if (pix.isNull())
        {
            pix.load("./icon/error.png");
        }
        c_widgets.last()->setIcon(QIcon(pix));
        connect(c_widgets.last(), &QPushButton::clicked, [this, pix]() { //
            this->m_pixmap = pix;
            ratio = this->m_pixmap.width() / 1.0 / this->m_pixmap.height();
            this->m_pLabel->setPixmap(this->m_pixmap);
            for (auto widget : this->c_widgets)
            {
                widget->hide();
            }

            m_pLayout->setAlignment(Qt::AlignmentFlag::AlignCenter);

            m_pLayout->addWidget(this->m_pLabel, 0, 0, 0, 0);
            this->resizeEvent(nullptr);
        });
    }
    c_widgetPerRowCount = -1;
    this->resizeEvent(nullptr);
}

void CScrollArea::clear()
{
    for (auto c_widget : c_widgets)
    {
        c_widget->deleteLater();
    }
    c_widgets.clear();
    m_pLayout->setAlignment(Qt::AlignTop);
    m_pLabel->hide();
}

void CScrollArea::resizeEvent(QResizeEvent *event)
{
    if (m_pLabel->isVisible())
    {
        m_pLabel->resize(this->size());
    }
    else
    { // 计算 可放置控件 区域宽度
        int screenWidth = this->width() - GAP;
        if (m_pVerticalScrollBar->isVisible())
        {
            screenWidth -= m_pVerticalScrollBar->width();
        }
        // 计算每行按钮的数量
        int newPerRowCount = (screenWidth + minSpacing) / ((c_widgetWidth ? c_widgetWidth : 1) + minSpacing);

        // 如果每行控件数量发生变化, 则清空并重新填入
        if (newPerRowCount != c_widgetPerRowCount)
        {
            c_widgetPerRowCount = newPerRowCount;
            // 清空lyout
            while (m_pLayout->takeAt(0) != nullptr)
                ;

            // 重新填入
            curRow = 0;
            curCol = 0;
            for (auto widget : c_widgets)
            {
                this->layoutAddWidget(widget);
            }
        }

        // 计算间隙
        int spacing = (screenWidth - c_widgetPerRowCount * c_widgetWidth) / (c_widgetPerRowCount ? c_widgetPerRowCount : 1);
        m_pLayout->setHorizontalSpacing(spacing);
    }
}

inline void CScrollArea::layoutAddWidget(QPushButton *btn)
{
    m_pLayout->addWidget(btn, curRow, curCol++);
    if (curCol == c_widgetPerRowCount)
    {
        curCol = 0;
        curRow++;
    }
}
