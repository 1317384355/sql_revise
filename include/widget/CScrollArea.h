#pragma once
#include <QScrollArea>
#include <QResizeEvent>
#include <QScrollBar>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QDialog>
#include <QDebug>

class CScrollArea : private QScrollArea
{
    Q_OBJECT
public:
    explicit CScrollArea(QWidget *parent = nullptr);
    ~CScrollArea();

    inline void setWidgetSize(int width, int height);

    void setPicPath(const QStringList &list_picPath);

    void clear();

protected:
    // 重写虚函数
    void resizeEvent(QResizeEvent *event) override;

private:
    inline void layoutAddWidget(QPushButton *widget);

    QScrollBar *m_pVerticalScrollBar; // y轴滑块
    QGridLayout *m_pLayout;

    QVector<QPushButton *> c_widgets;
    int c_widgetWidth;
    int c_widgetHeight;

    int minSpacing;

    int c_widgetCount;
    int c_widgetPerRowCount;

    int curRow;
    int curCol;

    QLabel *m_pLabel;
    QPixmap m_pixmap;
    float ratio;
};
