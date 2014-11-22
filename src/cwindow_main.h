#ifndef CWINDOW_MAIN_H
#define CWINDOW_MAIN_H

#include <QMainWindow>
#include "typedef.h"

QT_BEGIN_NAMESPACE
class CWidgetEdit;
class CWidgetConsol;
class CWidgetTree;
class QAction;
class QMenu;
QT_END_NAMESPACE

class CWindowMain : public QMainWindow
{
    Q_OBJECT

public:

    explicit CWindowMain(QWidget *parent = 0);
    ~CWindowMain();

private:

    QAction             *m_actOpen;
    QAction             *m_actSave;
    QAction             *m_actSaveAs;
    QMenu               *mainMenu;

    CWidgetEdit       *m_widgetEdit;
    CWidgetConsol     *m_widgetConsol;
    CWidgetTree       *m_widgetTree;

private slots:

    void on_open();
    void on_save();
    void on_save_as();
    //void on_save(const TStr &fileName,CBFView *bfv);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);
    void executingOperation(const QString&);

};

#endif // CWINDOW_MAIN_H
