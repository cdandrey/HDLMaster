#ifndef CWINDOW_MAIN_H
#define CWINDOW_MAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class CProject;
class CProjectSrc;
class CProjectObject;
class CWidgetEdit;
class CWidgetConsol;
class CWidgetTree;
class QAction;
class QFileInfo;
class QMenu;
class QSettings;
class QSplitter;
class QThread;
QT_END_NAMESPACE

class CWindowMain : public QMainWindow
{
    Q_OBJECT

public:

    explicit CWindowMain(QWidget *parent = 0);
    ~CWindowMain();

private:

    QSettings         *m_settings;

    QAction           *m_actOpen;
    QAction           *m_actOpenPro;
    QAction           *m_actClose;
    QAction           *m_actSave;
    QAction           *m_actSaveAs;
    QAction           *m_actQuit;
    QMenu             *m_menu;

    QSplitter         *m_splTree;
    QSplitter         *m_splConsol;

    CWidgetConsol     *m_widgetConsol;
    CWidgetEdit       *m_widgetEdit;
    CWidgetTree       *m_widgetTree;

    CProjectObject    *m_proObj;
    QThread           *m_thr;

    void applyStyleSheet();
    void loadSettings();
    void saveSettings();

private slots:

    void open();
    void openProject();
    void openProjectFinish(bool succes);
    void closeProject();
    void save();
    void saveAs();

signals:
    void addProject(CProject*);
    void clearTree();

    void messageAppend(const QString&);
    void messageSet(const QString&);
    void executingOperation(const QString&);
    void openSrc(CProjectSrc*);
};

#endif // CWINDOW_MAIN_H
