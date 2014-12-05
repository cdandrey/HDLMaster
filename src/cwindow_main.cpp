#include <QtDebug>
#include <QDir>
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
#include <QThread>
#include <QTimerEvent>

#include "cproject_object.h"
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
    m_actOpen = new QAction(QIcon(":/ico/main_open.png"),tr("Открыть"),this);
    m_actOpen->setShortcut(QKeySequence::Open);
    connect(m_actOpen,SIGNAL(triggered()),this,SLOT(open()));

    m_actOpenXise = new QAction(tr("Открыть ISE проект"),this);
    connect(m_actOpenXise,SIGNAL(triggered()),this,SLOT(openXise()));

    m_actSave = new QAction(QIcon(":/ico/main_save.png"),tr("Сохранить"),this);
    m_actSave->setShortcut(QKeySequence::Save);
    connect(m_actSave,SIGNAL(triggered()),this,SLOT(save()));

    m_actSaveAs = new QAction(QIcon(":/ico/main_save_as.png"),tr("Сохранить как"),this);
    m_actSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(m_actSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));

    // create main menu
    m_menu = menuBar()->addMenu(tr("Файл"));
    m_menu->addAction(m_actOpen);
    m_menu->addAction(m_actOpenXise);
    m_menu->addAction(m_actSave);
    m_menu->addAction(m_actSaveAs);

    // ceate widget consol
    m_widgetConsol = new CWidgetConsol(this);
    m_widgetConsol->actVisible()->setChecked(m_settings->value("state/consol_visible").toBool());

    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));
    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));
    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));

    // create widget edit sources
    m_widgetEdit = new CWidgetEdit(this);
    connect(m_widgetEdit,SIGNAL(messageAppend(QString)),m_widgetConsol,SLOT(messageAppend(QString)));
    connect(m_widgetEdit,SIGNAL(messageSet(QString)),m_widgetConsol,SLOT(messageSet(QString)));

    // create widget tree project
    m_widgetTree     = new CWidgetTree(this);
    m_widgetTree->actVisible()->setChecked(m_settings->value("state/tree_visible").toBool());
    connect(m_widgetTree,SIGNAL(messageAppend(QString)),m_widgetConsol,SLOT(messageAppend(QString)));
    connect(m_widgetTree,SIGNAL(clickedTreeItem(QString)),m_widgetEdit,SLOT(view(QString)));
    connect(this,SIGNAL(openSrc(CProjectSrc*)),m_widgetTree,SLOT(addSrc(CProjectSrc*)));

    // placement widgets
    m_splTree = new QSplitter(Qt::Horizontal);
    m_splTree->addWidget(m_widgetTree);
    m_splTree->addWidget(m_widgetEdit);
    m_splTree->restoreState(m_settings->value("state/splTree").toByteArray());

    m_splConsol = new QSplitter(Qt::Vertical);
    m_splConsol->addWidget(m_splTree);
    m_splConsol->addWidget(m_widgetConsol);
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

    m_proObj = new CProjectObject();
    connect(m_proObj,SIGNAL(succefull(CProject*)),m_widgetTree,SLOT(addProject(CProject*)));
    connect(m_proObj,SIGNAL(messageAppend(QString)),m_widgetConsol,SLOT(messageAppend(QString)));
    connect(m_proObj,SIGNAL(messageSet(QString)),m_widgetConsol,SLOT(messageSet(QString)));

    applyStyleSheet();
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
    saveSettings();

    m_settings->~QSettings();

    m_actOpen->~QAction();
    m_actOpenXise->~QAction();
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
}
//-------------------------------------------------------------------


void CWindowMain::saveSettings()
{
    QFile fileSettings("settings.ini");

    if (!fileSettings.open(QIODevice::Append)) {
        emit messageAppend(QString("%1 %2")
                           .arg(tr("ОШИБКА! Невозможно открыть файл"))
                           .arg(fileSettings.fileName()));
        return;
    }

    QSettings *settings = new QSettings(fileSettings.fileName(),QSettings::IniFormat);

    QStringList keys(m_settings->allKeys());

    foreach (QString key, keys)
        settings->setValue(key,m_settings->value(key));

    settings->setValue("state/consol_visible",m_widgetConsol->actVisible()->isChecked());
    settings->setValue("state/tree_visible",m_widgetTree->actVisible()->isChecked());
    settings->setValue("state/splConsol",m_splConsol->saveState());
    settings->setValue("state/splTree",m_splTree->saveState());

    delete settings;
}
//-------------------------------------------------------------------


void CWindowMain::open()
{
    QStringList lstFiles =  QFileDialog::getOpenFileNames(this,tr("Открыть"),
                                                    QDir::currentPath(),
                                                    tr("Файл VHDL(*.vhd)"));

    foreach (QString srcFileName, lstFiles)
        emit openSrc(CProjectSrc::create(srcFileName));

}
//------------------------------------------------------------------


void CWindowMain::openXise()
{
    QString proFileName(QFileDialog::getOpenFileName(this,m_actOpenXise->text(),
                                               QDir::currentPath(),
                                               tr("Проект ISE(*.xise)")));

    if (proFileName.isEmpty())
        return;

    if (!QFileInfo(proFileName).isReadable()) {
        emit messageAppend(QString("%1 %2")
                           .arg(tr("ОШИБКА! Невозможно открыть файл"))
                           .arg(proFileName));
        return;
    }

    QDir::setCurrent(QFileInfo(proFileName).absolutePath());

    QThread *thr = new QThread(this);

    m_proObj->setProFile(proFileName);
    m_proObj->moveToThread(thr);

    connect(thr,SIGNAL(started()),m_proObj,SLOT(openXise()));
    connect(m_proObj,SIGNAL(succefull()),thr,SLOT(quit()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));

    thr->start(QThread::HighPriority);
}
//------------------------------------------------------------------


void CWindowMain::save()
{
    /*CBFView *bfv = m_widgetListBF->bfCurrent();

    if (bfv == NULL)
        return;

    QString fileName = "";
    if (bfv->pathFile().isEmpty()){
        fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    } else {
        fileName = bfv->absoluteFileName();
    }

    on_save(fileName,bfv);*/
}
//------------------------------------------------------------------


void CWindowMain::saveAs()
{
    /*CBFView *bfv = m_widgetListBF->bfCurrent();

    QString fileName = "";
    fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    on_save(fileName,bfv);*/
}
//------------------------------------------------------------------


/*void CWindowMain::on_save(const QString &fileName, CBFView *bfv)
{
    QFile file(fileName);
    QTextStream stream(&file);

    if (!file.open(QIODevice::WriteOnly)){
        emit messageAppend(QString("%1 : %2")
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


