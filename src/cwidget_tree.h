#ifndef CWIDGET_TREE_H
#define CWIDGET_TREE_H

#include <QWidget>
#include "typedef.h"

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QAction;
class QThread;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class CWidgetTree : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetTree(QWidget *parent = 0);

    QAction *actVisible(){return m_actVisible;}

public slots:

    void on_disabledHide(bool disabled);

private:

    CToolBarHeader  *m_header;
    QAction         *m_actVisible;
    QTreeWidget     *m_tree;

private slots:

    void clicked_treeWidgetItem(QTreeWidgetItem*);

signals:

};

#endif // CWIDGET_TREE_H
