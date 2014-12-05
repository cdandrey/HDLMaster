#include "cwidget_tree.h"

#include <QDebug>
#include <QAction>
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "cproject.h"
#include "cproject_src.h"
#include "ctoolbar_header.h"

const QString CWidgetTree::ItemFolder("ItemFolder");

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
    //m_tree->setIconSize(QSize(24,24));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_tree);

    m_actVisible = new QAction(tr("Отображать дерево проекта"),this);
    m_actVisible->setCheckable(true);
    m_actVisible->setChecked(true);
    m_actVisible->setObjectName("actVisibleTreeProject");

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
    connect(m_tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(clickedTreeItem(QTreeWidgetItem*)));

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


void CWidgetTree::addSrc(CProjectSrc* src)
{
    if (m_tree->findItems(src->fileName(),Qt::MatchRecursive,1).size() > 0)
        return;

    QTreeWidgetItem *item = new QTreeWidgetItem(m_tree);

    item->setText(0,src->compName());
    item->setText(1,src->fileName());

    emit messageAppend(QString("%1 %2").arg(tr("Файл успешно открыт")).arg(src->fileName()));
}
//------------------------------------------------------------------


void CWidgetTree::addProject(CProject *pro)
{
    m_pro = pro;
    emit messageAppend(tr("Формируется дерево проекта %1").arg(m_pro->proName()));

    QTreeWidgetItem *itemPro = new QTreeWidgetItem(m_tree);
    itemPro->setText(0,m_pro->proName());
    itemPro->setText(1,m_pro->proFile());
    QFont proFont(font());
    proFont.setWeight(QFont::Bold);
    itemPro->setFont(0,proFont);
    itemPro->setIcon(0,QIcon(":/ico/folder.png"));

    QTreeWidgetItem *itemSrc = new QTreeWidgetItem(itemPro);
    itemSrc->setText(0,tr("Компоненты"));
    itemSrc->setText(1,ItemFolder);
    itemSrc->setIcon(0,QIcon(":/ico/folder.png"));

    QTreeWidgetItem *itemLib = new QTreeWidgetItem(itemPro);
    itemLib->setText(0,tr("Библиотеки"));
    itemLib->setText(1,ItemFolder);
    itemLib->setIcon(0,QIcon(":/ico/folder.png"));

    QTreeWidgetItem *itemUcf = new QTreeWidgetItem(itemPro);
    itemUcf->setText(0,tr("Файлы ucf"));
    itemUcf->setText(1,ItemFolder);
    itemUcf->setIcon(0,QIcon(":/ico/folder.png"));

    addItem(itemSrc,m_pro->proTopFile(),m_pro->compName(m_pro->proTopFile()));

    emit messageAppend(tr("Открытие проекто завершилось успешно."));
}
//------------------------------------------------------------------

void CWidgetTree::addItem(QTreeWidgetItem *parent,const QString& file,const QString& inst)
{
    //if (QRegExp("EEPROM").indexIn(file) != -1)
        qDebug() << "-- " << inst << "  -  " << file;
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,inst);
    item->setText(1,file);

    if (file == CProjectSrc::Noname || m_pro->beginInst(file) == CProjectSrc::Noname)
        return;

    do {
        addItem(item,m_pro->fileName(m_pro->instComp(file)),m_pro->inst(file));
    } while (m_pro->nextInst(file) != CProjectSrc::Noname);
}
//------------------------------------------------------------------


void CWidgetTree::disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetTree::clickedTreeItem(QTreeWidgetItem* item)
{
    if (item->text(0) == ItemFolder)
        return;

    emit clickedTreeItem(item->text(1));
}
//------------------------------------------------------------------
