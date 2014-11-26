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
    m_tree->setObjectName("widgetTreeProject");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_tree);

    connect(m_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(clicked_treeWidgetItem(QTreeWidgetItem*)));

    m_actVisible = new QAction(tr("Отображать дерево проекта"),this);
    m_actVisible->setCheckable(true);
    m_actVisible->setChecked(true);
    m_actVisible->setObjectName("actVisibleTreeProject");

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));

    setObjectName("treeProject");
}
//------------------------------------------------------------------


void CWidgetTree::on_disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetTree::clicked_treeWidgetItem(QTreeWidgetItem*)
{
}
//------------------------------------------------------------------
