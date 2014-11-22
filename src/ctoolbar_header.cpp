#include "ctoolbar_header.h"

#include <QAction>
#include <QLabel>

//const QString CToolBarHeader::StyleSheet = "";

CToolBarHeader::CToolBarHeader(QString title,QWidget *parent) :
    QToolBar(parent)
{
    setIconSize(QSize(16,16));
    setMinimumHeight(28);
    setMovable(false);
    setStyleSheet(styleBlackGradient());

    m_label = new QLabel(title);
    m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_label->setMargin(5);

    m_actHint = new QAction(QIcon(":/ico/wd_hint_d.png"),tr("Закрыть окно"),this);

    addWidget(m_label);
    addAction(m_actHint);
}
//------------------------------------------------------------------


void CToolBarHeader::insertSpace(QAction *before,int width)
{
    QWidget *space = new QWidget();
    space->setFixedWidth(width);
    space->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    insertWidget(before,space);
}
//------------------------------------------------------------------


void CToolBarHeader::insertStretch(QAction *before)
{
    QWidget *stretch = new QWidget();
    stretch->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    insertWidget(before,stretch);
}
//------------------------------------------------------------------


QString CToolBarHeader::styleBlackGradient(int top,int right,int bottom,int left){
    return QString(
           "QToolBar {"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                "border-width: %1px %2px %3px %4px;"
                "border-style: solid;"
                "border-color: #000;"
            "}"
            "QToolButton:hover,QToolButton:checked {"
                "border: 0px;"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ccc, stop:1.0 #888);"
            "}"
            "QLabel {"
                "color: white; font-size: 12px;"
            "}"
            "QComboBox {"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                "border-radius: 0px;"
                "color: #fff;"
            "}"
            "QComboBox:hover {"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #bbb, stop:1.0 #777);"
                "border-radius: 0px;"
                "color: #fff;"
            "}"
            "QComboBox::drop-down {"
                "subcontrol-origin: padding;"
                "subcontrol-position: top right;"
                "width: 12px; "
                "border-left-width: 0px;"
                "border-left-color: #ccc;"
                "border-left-style: solid;"
                "border-top-right-radius: 3px;"
                "border-bottom-right-radius: 3px;"
                "}"
            "QComboBox::down-arrow {"
                "image: url(:/ico/arrow_combo_box.png);"
                "width: 12;"
                "height: 12;"
            "}"
                ).arg(top).arg(right).arg(bottom).arg(left);
}
//------------------------------------------------------------------


QString CToolBarHeader::title() const
{
    return m_label->text();
}
//------------------------------------------------------------------


void CToolBarHeader::setTitle(const QString &title)
{
    m_label->setText(title);
}
//------------------------------------------------------------------
