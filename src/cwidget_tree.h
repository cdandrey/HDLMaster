#ifndef CWIDGET_TREE_H
#define CWIDGET_TREE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CProject;
class CProjectSrc;
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

    CToolBarHeader      *m_header;
    QAction             *m_actVisible;
    QTreeWidget         *m_tree;

    CProject            *m_pro;
    QList<CProjectSrc*>  m_src;

    static const QString ItemFolder;

    void addItem(QTreeWidgetItem *parent,const QString& file,const QString& inst);

private slots:

    void addSrc(CProjectSrc* src);
    void addProject(CProject* pro);
    void disabledHide(bool disabled);
    void clickedTreeItem(QTreeWidgetItem*item);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);

    void clickedTreeItem(const QString &fileName);

};

#endif // CWIDGET_TREE_H
