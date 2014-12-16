#include "cwidget_edit.h"

#include <QDebug>

#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QFile>
#include <QFileInfo>
#include <QPlainTextEdit>
//#include <QSettings>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetEdit::CWidgetEdit(QWidget *parent) :
    QWidget(parent),
    comboCurrentIndex(0)
{
    m_header = new CToolBarHeader();

    m_combo = new QComboBox();
    m_combo->addItem(tr("<нет документа>"));
    m_combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_header->insertWidget(m_header->actHint(),m_combo);
    m_header->insertSpace(m_header->actHint(),0);
    //m_header->insertStretch(m_header->actHint());

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
    connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboCurrentIndexChanged(int)));
    connect(m_edit,SIGNAL(textChanged()),this,SLOT(editTextChanged()));

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
        disconnect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboCurrentIndexChanged(int)));
        disconnect(m_edit,SIGNAL(textChanged()),this,SLOT(editTextChanged()));

        m_edit->setVisible(true);
        m_edit->setPlainText(stream.readAll());

        files.insert(fileName,m_edit->toPlainText());

        QFileInfo fileInfo(fileName);
        QString fileNameBase(fileInfo.fileName().remove(fileInfo.filePath(),Qt::CaseInsensitive));

        m_combo->addItem(fileNameBase,fileName);
        m_combo->setCurrentText(fileNameBase);
        comboCurrentIndex = m_combo->currentIndex();

        // return default connected
        connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(comboCurrentIndexChanged(int)));
        connect(m_edit,SIGNAL(textChanged()),this,SLOT(editTextChanged()));
    }
}
//------------------------------------------------------------------


void CWidgetEdit::comboCurrentIndexChanged(int newCurrentIndex)
{
    if (newCurrentIndex == comboCurrentIndex)
        return;

    if (newCurrentIndex > 0) {
        files[m_combo->itemData(comboCurrentIndex).toString()] = m_edit->toPlainText();
        m_edit->setPlainText(files[m_combo->currentData().toString()]);
    }
    else {
        if (m_combo->count() > 1)
            m_combo->setCurrentIndex(comboCurrentIndex);
    }

    comboCurrentIndex = newCurrentIndex;
}
//------------------------------------------------------------------


void CWidgetEdit::editTextChanged()
{
    if (m_combo->currentIndex() > 0) {

        QChar lastChar(m_combo->currentText().at(m_combo->currentText().length() - 1));

        if ((lastChar != '*') && (m_edit->toPlainText() != files[m_combo->currentData().toString()]))
            m_combo->setItemText(m_combo->currentIndex(),m_combo->currentText() + "*");
    }
}
//------------------------------------------------------------------


void CWidgetEdit::triggeredActHide()
{
    if (m_combo->currentIndex() > 0) {

        files.erase(files.find(m_combo->currentData().toString()));
        m_combo->removeItem(m_combo->currentIndex());

        if (m_combo->currentIndex() == 0) {
            m_edit->clear();
            m_edit->setVisible(false);
        }
    }
}
//------------------------------------------------------------------


