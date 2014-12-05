#include "cproject_src.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

const QString CProjectSrc::SuffixNgc("ngc");
const QString CProjectSrc::SuffixThdl("thdl");
const QString CProjectSrc::SuffixVerilog("v");
const QString CProjectSrc::SuffixVhdl("vhd");
const QString CProjectSrc::SuffixUcf("ucf");

const QString CProjectSrc::Noname("<Noname>");

CProjectSrc::~CProjectSrc()
{}
//------------------------------------------------------------------


CProjectSrc* CProjectSrc::create(const QString &fileName)
{
    CProjectSrc *item = NULL;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return item;

    QTextStream stream(&file);
    QFileInfo fileInfo(file);
    QString suffix(fileInfo.suffix());

    if (suffix == SuffixNgc){
//        item = new CProjectSrcNgc(fileName);
//        item->parsed(stream.readAll());
    }
    else if (suffix == SuffixThdl) {
//        item = new CProjectSrcThdl(fileName);
//        item->parsed(stream.readAll());
    }
    else if (suffix == SuffixVerilog) {
//        item = new CProjectSrcVerilog(fileName);
//        item->parsed(stream.readAll());
    }
    else if (suffix == SuffixVhdl) {
        item = new CProjectSrcVhdl(fileName);
        item->parsed(stream.readAll());
    }
    else if (suffix == SuffixUcf) {
//        item = new CProjectSrcUcf(fileName);
//        item->parsed(stream.readAll());
    }
    else {
        return NULL;
    }

    return item;
}
//------------------------------------------------------------------

//
// NGC
//
//CProjectSrcNgc::CProjectSrcNgc(const QString &fileName)
//    : m_fileName(fileName)
//{}
//------------------------------------------------------------------

//
// THDL
//
//CProjectSrcThdl::CProjectSrcThdl(const QString &fileName)
//    : m_fileName(fileName),
//      m_compName("")
//{}
//------------------------------------------------------------------

//
// Verilog
//
//CProjectSrcVerilog::CProjectSrcVerilog(const QString &fileName)
//    : m_fileName(fileName),
//      m_compName("")
//{}
//------------------------------------------------------------------

//
// VHDL
//
CProjectSrcVhdl::CProjectSrcVhdl(const QString &fileName)
    : m_fileName(fileName),
      m_compName("")
{}
//------------------------------------------------------------------


void CProjectSrcVhdl::parsed(const QString &listing)
{
    // find include packages and begin component
    QRegExp rx("\\buse\\s+work.([\\w\\d]+).\\w+\\b|(\\bentity|\\bpackage)\\s+(\\w+)\\s+is\\b");
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;
    while ((pos = rx.indexIn(listing,pos)) != -1 && rx.cap(2)== "") {
        //qDebug() << "includ package: " << rx.cap(1);
        m_packages << rx.cap(1);
        pos += rx.matchedLength();
    }

    m_compName = Noname;
    if (pos != -1)
        m_compName = rx.cap(3);

    pos += rx.matchedLength();

    //qDebug() << "name component: " << m_compName;

    if (rx.cap(2) == "package")
        return;

    // go to block ARCHITECTURE for find contain components
    rx.setPattern("\\barchitecture\\s+[\\w\\d]+\\s+of\\s+[\\w\\d]+\\s+is\\b");

    if ((pos = rx.indexIn(listing,pos)) == -1)
        return;
    pos += rx.matchedLength();

    // find begins body architecture
    rx.setPattern("\\bbegin\\b");
    if ((pos = rx.indexIn(listing,pos)) == -1)
        return;
    pos += rx.matchedLength();

    rx.setPattern("\\b([\\w\\d]+)\\s*:\\s*(\\b[\\w\\d]+)\\s+(generic|port)\\s+map\\b"
                  "|\\b([\\w\\d]+)\\s*:\\s*entity\\s+work.([\\w\\d]+)\\b");

    while ((pos = rx.indexIn(listing,pos)) != -1) {
        if (rx.cap(4).isEmpty() && rx.cap(5).isEmpty()) {
            //m_compInst[rx.cap(2)] << rx.cap(1);
            m_inst << QString("%1 - %2").arg(rx.cap(1)).arg(rx.cap(2));
//            m_instances.insert(rx.cap(1),rx.cap(2));
        }
        else {
            //m_compInst[rx.cap(5)] << rx.cap(4);
            m_inst << QString("%1 - %2").arg(rx.cap(4)).arg(rx.cap(5));
//            m_instances.insert(rx.cap(4),rx.cap(5));
        }
        pos += rx.matchedLength();
    }
}
//------------------------------------------------------------------

//
// UCF
//
//CProjectSrcUcf::CProjectSrcUcf(const QString &fileName)
//    : m_fileName(fileName)
//{}
//------------------------------------------------------------------

