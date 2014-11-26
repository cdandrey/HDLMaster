#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>
#include <QSettings>
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
    // create settings
    loadSettings();

    // create actions
    m_actOpen = new QAction(QIcon(":/ico/main_open.png"),tr("&Открыть"),this);
    m_actOpen->setShortcut(QKeySequence::Open);
    connect(m_actOpen,SIGNAL(triggered()),this,SLOT(on_open()));

    m_actSave = new QAction(QIcon(":/ico/main_save.png"),tr("&Сохранить"),this);
    m_actSave->setShortcut(QKeySequence::Save);
    connect(m_actSave,SIGNAL(triggered()),this,SLOT(on_save()));

    m_actSaveAs = new QAction(QIcon(":/ico/main_save_as.png"),tr("&Сохранить как"),this);
    m_actSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(m_actSaveAs,SIGNAL(triggered()),this,SLOT(on_save_as()));

    // create main menu
    m_menu = menuBar()->addMenu(tr("Файл"));
    m_menu->addAction(m_actOpen);
    m_menu->addAction(m_actSave);
    m_menu->addAction(m_actSaveAs);

    // ceate widget consol
    m_widgetConsol = new CWidgetConsol(this);
    qDebug() << "state/consol_visible = " << m_settings->value("state/consol_visible").toBool();
    m_widgetConsol->actVisible()->setChecked(m_settings->value("state/consol_visible").toBool());

    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));
    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));
    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));

    // create widget edit sources
    m_widgetEdit = new CWidgetEdit(this);

    // create widget tree project
    m_widgetTree     = new CWidgetTree(this);
    qDebug() << "state/tree_visible = " << m_settings->value("state/tree_visible").toBool();
    m_widgetTree->actVisible()->setChecked(m_settings->value("state/tree_visible").toBool());

    // placement widgets
    m_splTree = new QSplitter(Qt::Horizontal);
    m_splTree->addWidget(m_widgetTree);
    m_splTree->addWidget(m_widgetEdit);
    qDebug() << "state/splTree = " << m_settings->value("state/splTree");
    m_splTree->restoreState(m_settings->value("state/splTree").toByteArray());

    m_splConsol = new QSplitter(Qt::Vertical);
    m_splConsol->addWidget(m_splTree);
    m_splConsol->addWidget(m_widgetConsol);
    qDebug() << "state/splConsol = " << m_settings->value("state/splConsol");
    m_splConsol->restoreState(m_settings->value("state/splConsol").toByteArray());

    QWidget *widgetCenter = new QWidget(this);
    setCentralWidget(widgetCenter);

    QHBoxLayout *hbox = new QHBoxLayout();
    centralWidget()->setLayout(hbox);

    hbox->addWidget(m_splConsol);
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->setStretch(0,100);

    m_menu = menuBar()->addMenu(tr("Вид"));
    m_menu->addAction(m_widgetConsol->actVisible());
    m_menu->addAction(m_widgetTree->actVisible());

    applyStyleSheet();
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
    saveSettings();

    m_settings->~QSettings();

    m_actOpen->~QAction();
    m_actSave->~QAction();
    m_actSaveAs->~QAction();

    m_menu->~QMenu();

    m_widgetConsol->~QWidget();
    m_widgetEdit->~QWidget();
    m_widgetTree->~QWidget();

    m_splTree->~QSplitter();
    m_splConsol->~QSplitter();
}
//-------------------------------------------------------------------


void CWindowMain::applyStyleSheet()
{
    QString style(m_settings->value("style/combobox").toString());
    style += m_settings->value("style/lable").toString();
    style += m_settings->value("style/plain_edit").toString();
    style += m_settings->value("style/splitter").toString();
    style += m_settings->value("style/text_edit").toString();
    style += m_settings->value("style/tool_bar").toString();
    style += m_settings->value("style/tool_button").toString();
    style += m_settings->value("style/widget_tree").toString();

    setStyleSheet(style);
}
//-------------------------------------------------------------------


void CWindowMain::loadSettings()
{
    QFileInfo fileInfoSettings("settings.ini");

    if (fileInfoSettings.isReadable())
        m_settings = new QSettings("settings.ini",QSettings::IniFormat);
    else
        m_settings = new QSettings(":/ini/settings_default.ini",QSettings::IniFormat);

    qDebug() << m_settings->fileName();
}
//-------------------------------------------------------------------


void CWindowMain::saveSettings()
{
    QFile fileSettings("settings.ini");

    if (!fileSettings.open(QIODevice::Append)) {
        emit messageAppend(TStr("%1 : %2")
                           .arg(tr("Невозможно открыть файл"))
                           .arg(fileSettings.fileName()));
        return;
    }

    QSettings *settings = new QSettings(fileSettings.fileName(),QSettings::IniFormat);

    QStringList keys(m_settings->allKeys());
    qDebug() << settings->fileName();

    foreach (QString key, keys)
        settings->setValue(key,m_settings->value(key));

    settings->setValue("state/consol_visible",m_widgetConsol->actVisible()->isChecked());
    settings->setValue("state/tree_visible",m_widgetTree->actVisible()->isChecked());
    settings->setValue("state/splConsol",m_splConsol->saveState());
    settings->setValue("state/splTree",m_splTree->saveState());

    delete settings;
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


