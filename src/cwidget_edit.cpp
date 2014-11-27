#include "cwidget_edit.h"

#include <QDebug>

#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QFile>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QSettings>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetEdit::CWidgetEdit(QWidget *parent) :
    QWidget(parent)
{
    m_header = new CToolBarHeader();

    m_combo = new QComboBox();
    m_combo->addItem("<нет документа>");
    m_combo->setFixedHeight(m_header->minimumHeight() - 4);
    m_combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    m_header->insertWidget(m_header->actHint(),m_combo);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertStretch(m_header->actHint());

    m_edit = new QTextEdit();
    m_edit->setFont(QFont("Courier",12));
    m_edit->document()->setDocumentMargin(10);
    m_edit->setAcceptRichText(true);
    m_edit->setVisible(false);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);
    vbox->setStretch(1,100);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(triggeredActHide()));
    connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(viewChanged(int)));
    connect(m_edit,SIGNAL(textChanged()),this,SLOT(textChanged()));

    setObjectName("editSources");
}
//------------------------------------------------------------------


CWidgetEdit::~CWidgetEdit()
{
    m_combo->~QComboBox();
    m_header->~CToolBarHeader();
    m_edit->~QTextEdit();
}
//------------------------------------------------------------------

void CWidgetEdit::view(const QString &fileName)
{
    if (files.contains(fileName)) {
        if (fileName != m_combo->currentData().toString())
            m_combo->setCurrentIndex(m_combo->findData(fileName));
    }
    else {
        QFile file(fileName);
        QTextStream stream(&file);

        if (!file.open(QIODevice::ReadOnly)){
            emit messageAppend(QString("%1 : %2")
                               .arg(tr("ОШИБКА! Невозможно открыть файл"))
                               .arg(fileName));
            return;
        }

        // disconnected first open so not set flag of modification
        disconnect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(viewChanged(int)));
        disconnect(m_edit,SIGNAL(textChanged()),this,SLOT(textChanged()));

        m_edit->setVisible(true);
        m_edit->setPlainText(stream.readAll());

        files.insert(fileName,m_edit->toPlainText());

        QFileInfo fileInfo(fileName);
        QString fileNameBase(fileInfo.fileName().remove(fileInfo.filePath(),Qt::CaseInsensitive));

        m_combo->addItem(fileNameBase,fileName);
        m_combo->setCurrentText(fileNameBase);

        // return default connected
        connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(viewChanged(int)));
        connect(m_edit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    }
}
//------------------------------------------------------------------


void CWidgetEdit::viewChanged(int)
{
    m_edit->setPlainText(files[m_combo->currentData().toString()]);
}
//------------------------------------------------------------------


void CWidgetEdit::textChanged()
{
    if (m_edit->toPlainText() != files[m_combo->currentData().toString()] &&
            m_combo->currentText().at(m_combo->currentText().length() - 1) != '*')
        m_combo->setItemText(m_combo->currentIndex(),m_combo->currentText() + "*");

    files[m_combo->currentData().toString()] = m_edit->toPlainText();
}
//------------------------------------------------------------------


void CWidgetEdit::triggeredActHide()
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


