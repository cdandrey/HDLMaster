#include "cwidget_consol.h"

#include <QAction>
#include <QLabel>
#include <QPlainTextEdit>
#include <QSettings>
#include <QScrollBar>
#include <QTime>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetConsol::CWidgetConsol(QWidget *parent) :
    QWidget(parent)
{
    m_header = new CToolBarHeader(tr("Консоль"));
    m_header->insertStretch(m_header->actHint());

    m_actVisible = new QAction(tr("Отображать консоль"),this);
    m_actVisible->setCheckable(true);
    m_actVisible->setChecked(true);

    m_edit = new QPlainTextEdit();
    m_edit->setFont(QFont("Courier",12));
    m_edit->setReadOnly(true);
    m_edit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_edit->document()->setDocumentMargin(10);
    m_edit->setMaximumBlockCount(1000);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);

    connect(m_header->actHint(),SIGNAL(triggered(bool)),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_actVisible,SIGNAL(toggled(bool)),m_actVisible,SLOT(setChecked(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));

    m_time = new QTime();
}
//------------------------------------------------------------------


CWidgetConsol::~CWidgetConsol()
{
    m_actVisible->~QAction();
    m_header->~CToolBarHeader();
    m_edit->~QPlainTextEdit();
    delete m_time;
}
//------------------------------------------------------------------


void CWidgetConsol::messageAppend(const QString &message)
{
    m_actVisible->setChecked(true);
    m_edit->appendPlainText(QString("[%1] %2")
                            .arg(m_time->currentTime().toString("hh:mm:ss"))
                            .arg(message));
}
//------------------------------------------------------------------


void CWidgetConsol::messageSet(const QString &message)
{
    m_actVisible->setChecked(true);
    m_edit->setPlainText(QString("[%1] %2")
                         .arg(m_time->currentTime().toString("hh:mm:ss"))
                         .arg(message));
}
//------------------------------------------------------------------


void CWidgetConsol::executingOperation(const QString&)
{
//    if (operation == "finished") {
//        m_header->setTitle(tr("Консоль"));
//    } else {
//        m_header->setTitle(operation);
//    }
}
//------------------------------------------------------------------

