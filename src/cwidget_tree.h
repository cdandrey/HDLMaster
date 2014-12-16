#ifndef CWIDGET_TREE_H
#define CWIDGET_TREE_H

#include <QWidget>
#include "cproject_src.h"

QT_BEGIN_NAMESPACE
class CProject;
class CToolBarHeader;
class QAction;
class QComboBox;
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

    QAction             *m_actVisible;
    QComboBox           *m_combo;
    CToolBarHeader      *m_header;
    QTreeWidget         *m_tree;
    QTreeWidget         *m_treeIndep;

    QFont                m_fontBold;
    QFont                m_fontBoldItalic;

    static const QString ItemFolder;


    void addItem(QTreeWidgetItem *parent,
                 CProject *pro,
                 const QString& mod,
                 const QString& file);

    void addItem(QTreeWidgetItem *parent,
                 CProject *pro,
                 CProjectSrc::SrcType type);

    void addItem(QTreeWidgetItem *parent,
                 CProject *pro,
                 const QString& textItem,
                 const QStringList& lst);

    static QString folderName(CProjectSrc::SrcType type);
    static QString folderIco(CProjectSrc::SrcType type);

private slots:

    void addSrc(CProjectSrc* src);
    void addProject(CProject* pro);
    void disabledHide(bool disabled);
    void clickedTreeItem(QTreeWidgetItem*item);
    void currentComboIndexChange(int);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);

    void clickedTreeItem(const QString &fileName);
    void clearTree();

};

#endif // CWIDGET_TREE_H
