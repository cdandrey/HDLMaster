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
    ~CWidgetTree();

    QAction *actVisible(){return m_actVisible;}

private:

    CToolBarHeader  *m_header;
    QAction         *m_actVisible;
    QTreeWidget     *m_tree;

    QStringList files;

private slots:

    void addSrc(const QString& fileName, const QString& compName);
    void disabledHide(bool disabled);
    void doubleClickedTreeItem(QTreeWidgetItem*item);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);

    void doubleClickedTreeItem(const QString &fileName);

};

#endif // CWIDGET_TREE_H
