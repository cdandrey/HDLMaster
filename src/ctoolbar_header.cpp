#include "ctoolbar_header.h"

#include <QAction>
#include <QLabel>

CToolBarHeader::CToolBarHeader(QString title,QWidget *parent) :
    QToolBar(parent)
{
    setIconSize(QSize(16,16));
    setMinimumHeight(28);
    setMovable(false);
    setObjectName("Title");

    m_label = new QLabel(title);
    m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_label->setMargin(5);
    m_label->setObjectName("Title");

    m_actHint = new QAction(QIcon(":/ico/wd_hint_d.png"),tr("Закрыть окно"),this);
    m_actHint->setObjectName("Title");

    addWidget(m_label);
    addAction(m_actHint);
}
//------------------------------------------------------------------


CToolBarHeader::~CToolBarHeader()
{
    m_actHint->~QAction();
    m_label->~QLabel();
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
