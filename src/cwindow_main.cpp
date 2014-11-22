#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>
#include <QSplitter>
#include <QTextStream>
#include <QTimerEvent>

#include "cwidget_edit.h"
#include "cwidget_consol.h"
#include "cwidget_tree.h"
#include "cwindow_main.h"

CWindowMain::CWindowMain(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("QSplitter::handle:vertical {height: 0px;}"
                  "QSplitter::handle:horizontal {width: 0px;}");

    m_widgetEdit      = new CWidgetEdit(this);
    m_widgetConsol      = new CWidgetConsol(this);
    m_widgetConsol->setVisible(false);
    m_widgetTree     = new CWidgetTree(this);

    QSplitter *spTreeEdit = new QSplitter(Qt::Horizontal);
    spTreeEdit->addWidget(m_widgetTree);
    spTreeEdit->addWidget(m_widgetEdit);
    spTreeEdit->setStretchFactor(1,100);

    QSplitter *spMain = new QSplitter(Qt::Vertical);
    spMain->addWidget(spTreeEdit);
    spMain->addWidget(m_widgetConsol);
    spMain->setStretchFactor(0,100);

    QWidget *widgetCenter = new QWidget(this);
    setCentralWidget(widgetCenter);

    QHBoxLayout *hbox = new QHBoxLayout();
    centralWidget()->setLayout(hbox);

    hbox->addWidget(spMain);
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->setStretch(0,100);

    m_actOpen = new QAction(QIcon(":/ico/main_open.png"),tr("&Открыть"),this);
    m_actOpen->setShortcut(QKeySequence::Open);
    connect(m_actOpen,SIGNAL(triggered()),this,SLOT(on_open()));

    m_actSave = new QAction(QIcon(":/ico/main_save.png"),tr("&Сохранить"),this);
    m_actSave->setShortcut(QKeySequence::Save);
    connect(m_actSave,SIGNAL(triggered()),this,SLOT(on_save()));

    m_actSaveAs = new QAction(QIcon(":/ico/main_save_as.png"),tr("&Сохранить как"),this);
    m_actSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(m_actSaveAs,SIGNAL(triggered()),this,SLOT(on_save_as()));

    mainMenu = menuBar()->addMenu(tr("Файл"));
    mainMenu->addAction(m_actOpen);
    mainMenu->addAction(m_actSave);
    mainMenu->addAction(m_actSaveAs);

    mainMenu = menuBar()->addMenu(tr("Вид"));
    mainMenu->addAction(m_widgetConsol->actVisible());
    mainMenu->addAction(m_widgetTree->actVisible());

    mainMenu = menuBar()->addMenu(tr("Операции"));
    mainMenu = menuBar()->addMenu(tr("Помощь"));

    // main
    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));

    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));

    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
}
//-------------------------------------------------------------------


void CWindowMain::on_open()
{
    TLStr lstFiles =  QFileDialog::getOpenFileNames(this,tr("Открыть проект"),"",
                                                    tr("Проект HDL(*.%1)").arg("xise"));

    foreach (TStr fileName, lstFiles) {

        QFile file(fileName);
        QTextStream stream(&file);

        if (!file.open(QIODevice::ReadOnly)){
            emit messageAppend(TStr("%1 : %2")
                               .arg(tr("Невозможно открыть файл"))
                               .arg(fileName));
            continue;
        }

        QRegExp regDIMACS("^\\s*p\\s+xise\\s+\\d+\\s+\\d+\\s*$");
        regDIMACS.setCaseSensitivity(Qt::CaseInsensitive);

        TStr line = "";
        int pos = -1;

        while (pos == -1 && !stream.atEnd()) {
            line = stream.readLine();
            pos = regDIMACS.indexIn(line);
        }

        if (pos == -1) {
            emit messageAppend(TStr("%1 : %2")
                               .arg(tr("ОШИБКА! Неизвестный формат файла"))
                               .arg(fileName));
            continue;
        }

        TLLst cnf;

        QRegExp regDigit("\\d+|-\\d+");

        while (!stream.atEnd()) {

            line = stream.readLine();

            int pos = 0;
            TLst icnf;
            while (((pos = regDigit.indexIn(line, pos)) != -1)
                   && (regDigit.cap(0) != "0"))
            {
                icnf << regDigit.cap(0).toInt();
                pos += regDigit.matchedLength();
            }

            if (icnf.size() > 1)
                cnf << icnf;
            else
                emit messageAppend(TStr("%1 : %2")
                                   .arg(tr("ОШИБКА! Дизъюнкт сожержит меньше двух переменных."))
                                   .arg(fileName));
        }

        /*if (cnf.isEmpty())
            continue;

        CBoolFunction *bf = new CBoolFunction(cnf);
        QFileInfo fileInfo(file);
        CBFView *bfv = new CBFView(bf,fileInfo.baseName(),fileInfo.absolutePath());

        m_widgetListBF->on_append(bfv);
        m_widgetBFGenerate->on_set(bfv);*/

        file.close();
    }
}
//------------------------------------------------------------------


void CWindowMain::on_save()
{
    /*CBFView *bfv = m_widgetListBF->bfCurrent();

    if (bfv == NULL)
        return;

    TStr fileName = "";
    if (bfv->pathFile().isEmpty()){
        fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    } else {
        fileName = bfv->absoluteFileName();
    }

    on_save(fileName,bfv);*/
}
//------------------------------------------------------------------


void CWindowMain::on_save_as()
{
    /*CBFView *bfv = m_widgetListBF->bfCurrent();

    TStr fileName = "";
    fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    on_save(fileName,bfv);*/
}
//------------------------------------------------------------------


/*void CWindowMain::on_save(const TStr &fileName, CBFView *bfv)
{
    QFile file(fileName);
    QTextStream stream(&file);

    if (!file.open(QIODevice::WriteOnly)){
        emit messageAppend(TStr("%1 : %2")
                           .arg(tr("Невозможно открыть файл"))
                           .arg(fileName));
        return;
    }

    stream << bfv->viewDIMACS();
    QFileInfo fileInfo(file);
    bfv->setBoolFunctionName(fileInfo.baseName());
    bfv->setPathFile(fileInfo.absolutePath());
    m_widgetListBF->on_updateCurrentItem();

    file.close();
}*/
//------------------------------------------------------------------


