#ifndef CWINDOW_MAIN_H
#define CWINDOW_MAIN_H

#include <QMainWindow>
#include "typedef.h"

QT_BEGIN_NAMESPACE
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
    QAction           *m_actSave;
    QAction           *m_actSaveAs;
    QMenu             *m_menu;

    QSplitter         *m_splTree;
    QSplitter         *m_splConsol;

    CWidgetConsol     *m_widgetConsol;
    CWidgetEdit       *m_widgetEdit;
    CWidgetTree       *m_widgetTree;

    void applyStyleSheet();
    void loadSettings();
    void saveSettings();

private slots:

    void open();
    void save();
    void saveAs();
    //void on_save(const TStr &fileName,CBFView *bfv);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);
    void executingOperation(const QString&);
    void openSrc(const QString&,const QString&);

};

#endif // CWINDOW_MAIN_H
