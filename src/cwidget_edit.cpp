#include "cwidget_edit.h"

#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetEdit::CWidgetEdit(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QTextEdit {border: 0px;}");

    m_header = new CToolBarHeader();

    m_combo = new QComboBox();
    m_combo->addItem("<нет документа>");
    m_combo->setFixedHeight(m_header->minimumHeight() - 2);
    m_combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    m_header->insertWidget(m_header->actHint(),m_combo);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertStretch(m_header->actHint());

    m_edit = new QTextEdit();
    m_edit->setFont(QFont("Courier",12));
    m_edit->document()->setDocumentMargin(10);
    m_edit->setVisible(false);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);
    vbox->setStretch(1,100);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(triggered_actHide()));
}
//------------------------------------------------------------------


void CWidgetEdit::triggered_actHide()
{
    if (m_combo->currentIndex() > 0) {

        m_combo->removeItem(m_combo->currentIndex());

        if (m_combo->currentIndex() == 0) {
            m_edit->clear();
            m_edit->setVisible(false);
        }
    }
}
//------------------------------------------------------------------


