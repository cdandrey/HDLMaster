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
    QTreeWidgetItem *itemPro = new QTreeWidgetItem(m_tree);
    itemPro->setText(0,pro->proName());
    itemPro->setText(1,pro->proFile());
    QFont proFont(font());
    proFont.setWeight(QFont::Bold);
    itemPro->setFont(0,proFont);
    itemPro->setIcon(0,QIcon(":/ico/folder.png"));

    pro->toFront(CProjectSrc::Cmp);
    if (pro->hasNext()) {
        QTreeWidgetItem *itemSrc = new QTreeWidgetItem(itemPro);
        itemSrc->setText(0,folderName(CProjectSrc::Cmp));
        itemSrc->setText(1,ItemFolder);
        itemSrc->setIcon(0,QIcon(folderIco(CProjectSrc::Cmp)));

        addItem(pro,itemSrc,pro->compName(pro->proTopFile()),pro->proTopFile());
    }

    addItem(pro,itemPro,CProjectSrc::Lib);
    addItem(pro,itemPro,CProjectSrc::Mif);
    addItem(pro,itemPro,CProjectSrc::Ngc);
    addItem(pro,itemPro,CProjectSrc::Ucf);
}
//------------------------------------------------------------------


void CWidgetTree::addItem(CProject *pro,
                          QTreeWidgetItem *parent,
                          const QString& icmp,
                          const QString& file)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,icmp);
    item->setText(1,file);
    item->setIcon(0,QIcon(pro->src(CProjectSrc::Cmp,file)->suffixIco()));

    if (!pro->toFile(CProjectSrc::Cmp,file))
        return;

    QStringList lstCmp(pro->src()->list(CProjectSrc::Cmp));
    QStringList lstIcmp(pro->src()->list(CProjectSrc::Icmp));

    for (int i = 0; i < lstCmp.size(); ++i)
        addItem(pro,item,lstIcmp.at(i),pro->fileName(lstCmp.at(i)));
}
//------------------------------------------------------------------


void CWidgetTree::addItem(CProject *pro,
                          QTreeWidgetItem *parent,
                          CProjectSrc::SrcType type)
{
    pro->toFront(type);

    if (pro->hasNext()) {
        QTreeWidgetItem *itemFolder = new QTreeWidgetItem(parent);
        itemFolder->setText(0,folderName(type));
        itemFolder->setText(1,ItemFolder);
        itemFolder->setIcon(0,QIcon(folderIco(type)));

        while (pro->hasNext()) {
            QTreeWidgetItem *item = new QTreeWidgetItem(itemFolder);
            item->setText(0,pro->peekNext()->compName());
            item->setText(1,pro->peekNext()->fileName());
            item->setIcon(0,QIcon(pro->next()->suffixIco()));
        }

        itemFolder->sortChildren(0,Qt::AscendingOrder);
    }
}
//------------------------------------------------------------------


void CWidgetTree::closeProject()
{
    m_tree->clear();
}
//------------------------------------------------------------------


QString CWidgetTree::folderName(CProjectSrc::SrcType type)
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Icmp:
        return tr("Компоненты");
    case CProjectSrc::Lib:
        return tr("Библиотеки");
    case CProjectSrc::Mif:
        return tr("Файлы mif");
    case CProjectSrc::Ngc:
        return tr("Файлы ngc");
    case CProjectSrc::Ucf:
        return tr("Файлы ucf");
    default:
        break;
    }

    return CProjectSrc::Unknown;
}
//------------------------------------------------------------------


QString CWidgetTree::folderIco(CProjectSrc::SrcType type)
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Icmp:
        return "://ico/folder_src.png";
    case CProjectSrc::Lib:
        return "://ico/folder_lib.png";
    case CProjectSrc::Mif:
        return "://ico/folder_mif.png";
    case CProjectSrc::Ngc:
        return "://ico/folder_ngc.png";
    case CProjectSrc::Ucf:
        return "://ico/folder_ucf.png";
    default:
        break;
    }

    return CProjectSrc::UnknownIco;
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
