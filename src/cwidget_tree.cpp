#include "cwidget_tree.h"

#include <QAction>
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetTree::CWidgetTree(QWidget *parent) :
    QWidget(parent)
{
    m_header = new CToolBarHeader(tr("Дерево проекта"));
    m_header->insertStretch(m_header->actHint());

    m_tree = new QTreeWidget();
    m_tree->setMinimumWidth(150);
    m_tree->setHeaderHidden(true);
    m_tree->setColumnCount(2);
    m_tree->setColumnHidden(1,true);
    m_tree->setObjectName("widgetTreeProject");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_tree);

    connect(m_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(doubleClickedTreeItem(QTreeWidgetItem*)));

    m_actVisible = new QAction(tr("Отображать дерево проекта"),this);
    m_actVisible->setCheckable(true);
    m_actVisible->setChecked(true);
    m_actVisible->setObjectName("actVisibleTreeProject");

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
    connect(m_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(doubleClickedTreeItem(QTreeWidgetItem*)));

    setObjectName("treeProject");
}
//------------------------------------------------------------------


CWidgetTree::~CWidgetTree()
{
    m_actVisible->~QAction();
    m_header->~CToolBarHeader();
    m_tree->~QTreeWidget();
}
//------------------------------------------------------------------


void CWidgetTree::addSrc(const QString &fileName, const QString &compName)
{
    if (m_tree->findItems(fileName,Qt::MatchRecursive,1).size() > 0) {
        emit messageAppend(tr("Файл уже открыт %1").arg(fileName));
        return;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(m_tree);

    item->setText(0,compName);
    item->setText(1,fileName);

    emit messageAppend(QString("%1 %2").arg(tr("Файл успешно открыт")).arg(fileName));
}
//------------------------------------------------------------------


void CWidgetTree::disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetTree::doubleClickedTreeItem(QTreeWidgetItem* item)
{
    emit doubleClickedTreeItem(item->text(1));
}
//------------------------------------------------------------------
