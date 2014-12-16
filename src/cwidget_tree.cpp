#include "cwidget_tree.h"
#include "cproject.h"
#include "ctoolbar_header.h"

#include <QDebug>
#include <QAction>
#include <QComboBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

const QString CWidgetTree::ItemFolder("ItemFolder");

CWidgetTree::CWidgetTree(QWidget *parent) :
    QWidget(parent)
{
    m_actVisible = new QAction(tr("Окно обозревателя проекта"),this);
    m_actVisible->setCheckable(true);
    m_actVisible->setChecked(true);

    m_combo = new QComboBox();
    m_combo->addItem(tr("Проект"));
    m_combo->addItem(tr("Зависимости"));
    m_combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_header = new CToolBarHeader();

    m_header->insertWidget(m_header->actHint(),m_combo);
    m_header->insertSpace(m_header->actHint(),0);

    m_tree = new QTreeWidget();
    m_tree->setMinimumWidth(150);
    m_tree->setHeaderHidden(true);
    m_tree->setColumnCount(2);
    m_tree->setColumnHidden(1,true);

    m_treeIndep = new QTreeWidget();
    m_treeIndep->setMinimumWidth(150);
    m_treeIndep->setHeaderHidden(true);
    m_treeIndep->setColumnCount(2);
    m_treeIndep->setColumnHidden(1,true);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_tree);
    vbox->addWidget(m_treeIndep);

    connect(m_combo,SIGNAL(currentIndexChanged(int)),this,SLOT(currentComboIndexChange(int)));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
    connect(m_tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(clickedTreeItem(QTreeWidgetItem*)));
    connect(m_treeIndep,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(clickedTreeItem(QTreeWidgetItem*)));
    connect(this,SIGNAL(clearTree()),m_tree,SLOT(clear()));

    m_fontBold = font();
    m_fontBold.setBold(true);

    m_fontBoldItalic = font();
    m_fontBoldItalic.setBold(true);
    m_fontBoldItalic.setItalic(true);

    emit m_combo->currentIndexChanged(0);
}
//------------------------------------------------------------------


CWidgetTree::~CWidgetTree()
{
    m_actVisible->~QAction();
    m_combo->~QComboBox();
    m_header->~CToolBarHeader();
    m_tree->~QTreeWidget();
    m_treeIndep->~QTreeWidget();
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
    // create project tree
    QTreeWidgetItem *itPro = new QTreeWidgetItem(m_tree);
    itPro->setText(0,pro->proName());
    itPro->setText(1,pro->proFile());
    itPro->setFont(0,m_fontBold);
    itPro->setIcon(0,QIcon(":/ico/folder.png"));

    pro->toFront(CProjectSrc::Cmp);
    if (pro->hasNext()) {
        QTreeWidgetItem *itSrc = new QTreeWidgetItem(itPro);
        itSrc->setText(0,folderName(CProjectSrc::Cmp));
        itSrc->setText(1,ItemFolder);
        itSrc->setIcon(0,QIcon(folderIco(CProjectSrc::Cmp)));

        addItem(itSrc,pro,pro->compName(pro->proTopFile()),
                          pro->proTopFile());
    }

    addItem(itPro,pro,CProjectSrc::Lib);
    addItem(itPro,pro,CProjectSrc::Mif);
    addItem(itPro,pro,CProjectSrc::Ngc);
    addItem(itPro,pro,CProjectSrc::Ucf);

    // create independ tree
    QTreeWidgetItem *itCmp = new QTreeWidgetItem(m_treeIndep);
    itCmp->setText(0,folderName(CProjectSrc::Cmp));
    itCmp->setText(1,ItemFolder);
    itCmp->setIcon(0,QIcon(folderIco(CProjectSrc::Cmp)));

    pro->toFront(CProjectSrc::Cmp);
    while (pro->hasNext()) {

        const CProjectSrc *src = pro->next();
        QTreeWidgetItem *it = new QTreeWidgetItem(itCmp);
        it->setText(0,src->compName());
        it->setText(1,src->fileName());
        it->setIcon(0,QIcon(src->suffixIco()));

        addItem(it,pro,tr("Библиотеки"),src->list(CProjectSrc::Lib));
        addItem(it,pro,tr("Входящие в состав компоненты"),src->list(CProjectSrc::Cmp));
        addItem(it,pro,tr("Входит в состав компонентов"),pro->includes(src->compName(),CProjectSrc::Cmp));
    }

    itCmp->sortChildren(0,Qt::AscendingOrder);

    QTreeWidgetItem *itLib = new QTreeWidgetItem(m_treeIndep);
    itLib->setText(0,folderName(CProjectSrc::Lib));
    itLib->setText(1,ItemFolder);
    itLib->setIcon(0,QIcon(folderIco(CProjectSrc::Lib)));

    pro->toFront(CProjectSrc::Lib);
    while (pro->hasNext()) {

        const CProjectSrc *src = pro->next();
        QTreeWidgetItem *it = new QTreeWidgetItem(itLib);
        it->setText(0,src->compName());
        it->setText(1,src->fileName());
        it->setIcon(0,QIcon(src->suffixIco()));

        addItem(it,pro,tr("Библиотеки"),src->list(CProjectSrc::Lib));
        addItem(it,pro,tr("Входит в состав компонентов"),pro->includes(src->compName(),CProjectSrc::Lib));
    }

    itLib->sortChildren(0,Qt::AscendingOrder);

    QTreeWidgetItem *itMif = new QTreeWidgetItem(m_treeIndep);
    itMif->setText(0,folderName(CProjectSrc::Mif));
    itMif->setText(1,ItemFolder);
    itMif->setIcon(0,QIcon(folderIco(CProjectSrc::Mif)));

    pro->toFront(CProjectSrc::Mif);
    while (pro->hasNext()) {

        const CProjectSrc *src = pro->next();
        QTreeWidgetItem *it = new QTreeWidgetItem(itMif);
        it->setText(0,src->compName());
        it->setText(1,src->fileName());
        it->setIcon(0,QIcon(src->suffixIco()));

        //addItem(it,pro,tr("Библиотеки"),src->list(CProjectSrc::Lib));
        addItem(it,pro,tr("Входит в состав компонентов"),pro->includes(src->compName(),CProjectSrc::Mif));
    }

    itMif->sortChildren(0,Qt::AscendingOrder);
}
//------------------------------------------------------------------


void CWidgetTree::addItem(QTreeWidgetItem *parent,
                          CProject *pro,
                          const QString& mod,
                          const QString& file)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,mod);
    item->setText(1,file);
    item->setIcon(0,QIcon(pro->src(CProjectSrc::Cmp,file)->suffixIco()));

    if (!pro->toFile(CProjectSrc::Cmp,file))
        return;

    foreach (QString imod, pro->src()->list(CProjectSrc::Mod)) {
        QString cmp(pro->src(CProjectSrc::Cmp,file)->modCmp(imod));
        addItem(item,pro,QString("%1 - %2").arg(imod).arg(cmp),pro->fileName(cmp));
    }
}
//------------------------------------------------------------------


void CWidgetTree::addItem(QTreeWidgetItem *parent, CProject *pro,
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


void CWidgetTree::addItem(QTreeWidgetItem *parent,
                          CProject *pro,
                          const QString &textItem,
                          const QStringList &lst)
{
    if (!lst.isEmpty()) {
        QTreeWidgetItem *itSub = new QTreeWidgetItem(parent);
        itSub->setText(0,textItem);
        itSub->setText(1,ItemFolder);
        itSub->setFont(0,m_fontBoldItalic);

        foreach (QString cmp, lst) {
            QTreeWidgetItem *it = new QTreeWidgetItem(itSub);
            it->setText(0,cmp);
            it->setText(1,pro->fileName(cmp));
        }

        itSub->sortChildren(0,Qt::AscendingOrder);
    }
}
//------------------------------------------------------------------


QString CWidgetTree::folderName(CProjectSrc::SrcType type)
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Mod:
        return tr("Компоненты");
    case CProjectSrc::Lib:
        return tr("Библиотеки");
    case CProjectSrc::Mif:
        return tr("Файлы mif");
    case CProjectSrc::Ngc:
        return tr("Файлы ngc");
    case CProjectSrc::Ucf:
        return tr("Файлы ucf");
    default:;
    }

    return CProjectSrc::Unknown;
}
//------------------------------------------------------------------


QString CWidgetTree::folderIco(CProjectSrc::SrcType type)
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Mod:
        return "://ico/folder_src.png";
    case CProjectSrc::Lib:
        return "://ico/folder_lib.png";
    case CProjectSrc::Mif:
        return "://ico/folder_mif.png";
    case CProjectSrc::Ngc:
        return "://ico/folder_ngc.png";
    case CProjectSrc::Ucf:
        return "://ico/folder_ucf.png";
    default:;
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


void CWidgetTree::currentComboIndexChange(int index)
{
    switch (index) {
    case 0:
        m_tree->setVisible(true);
        m_treeIndep->setVisible(false);
        break;
    case 1:
        m_tree->setVisible(false);
        m_treeIndep->setVisible(true);
        break;
    default:;
    }
}
//------------------------------------------------------------------
