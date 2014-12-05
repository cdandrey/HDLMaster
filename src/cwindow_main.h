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
    QAction           *m_actOpenXise;
    QAction           *m_actSave;
    QAction           *m_actSaveAs;
    QMenu             *m_menu;

    QSplitter         *m_splTree;
    QSplitter         *m_splConsol;

    CWidgetConsol     *m_widgetConsol;
    CWidgetEdit       *m_widgetEdit;
    CWidgetTree       *m_widgetTree;

    CProjectObject    *m_proObj;

    void applyStyleSheet();
    void loadSettings();
    void saveSettings();

private slots:

    void open();
    void openXise();
    void save();
    void saveAs();

signals:
    void addProject(CProject*);

    void messageAppend(const QString&);
    void messageSet(const QString&);
    void executingOperation(const QString&);
    void openSrc(CProjectSrc*);
};

#endif // CWINDOW_MAIN_H
