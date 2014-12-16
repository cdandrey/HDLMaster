#include "ctoolbar_header.h"

#include <QAction>
#include <QLabel>

CToolBarHeader::CToolBarHeader(QString title,QWidget *parent) :
    QToolBar(parent),
    m_actHint(NULL),
    m_label(NULL)
{
    setIconSize(QSize(16,16));
    setMinimumHeight(26);
    setMaximumHeight(26);
    setMovable(false);

    m_actHint = new QAction(QIcon(":/ico/wd_hint_d.png"),tr("Закрыть окно"),this);

    if (!title.isEmpty()) {
        m_label = new QLabel(title);
        m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
        addWidget(m_label);
    }

    addAction(m_actHint);
}
//------------------------------------------------------------------


CToolBarHeader::~CToolBarHeader()
{
    if (m_actHint != NULL)
        m_actHint->~QAction();

    if (m_label != NULL)
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
