#include "cproject_src.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

const CProjectSrc CProjectSrc::m_srcUnknown = CProjectSrc();
const QString CProjectSrc::Unknown("<Unknown>");
const QString CProjectSrc::UnknownIco("://ico/doc_unknown.png");

const QString CProjectSrc::SuffixMif("mif");
const QString CProjectSrc::SuffixNgc("ngc");
const QString CProjectSrc::SuffixThdl("thdl");
const QString CProjectSrc::SuffixVerilog("v");
const QString CProjectSrc::SuffixVhdl("vhd");
const QString CProjectSrc::SuffixUcf("ucf");

const QString CProjectSrc::SuffixIcoMif("://ico/doc_mif.png");
const QString CProjectSrc::SuffixIcoNgc("://ico/doc_ngc.png");
const QString CProjectSrc::SuffixIcoThdl("://ico/doc_thd.png");
const QString CProjectSrc::SuffixIcoVerilog("://ico/doc_v.png");
const QString CProjectSrc::SuffixIcoVhdl("://ico/doc_vhd.png");
const QString CProjectSrc::SuffixIcoUcf("://ico/doc_ucf.png");


QString CProjectSrc::baseName(const QString &file) const
{
    return QFileInfo(file).baseName();
}
//------------------------------------------------------------------


CProjectSrc* CProjectSrc::create(const QString &fileName)
{
    CProjectSrc *src = const_cast<CProjectSrc*>(&CProjectSrc::m_srcUnknown);

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return src;

    QTextStream stream(&file);
    QFileInfo fileInfo(file);
    QString suffix(fileInfo.suffix());

    if (suffix == SuffixMif){
        src = new CProjectSrcMif(fileName);
    }
    else if (suffix == SuffixNgc){
        src = new CProjectSrcNgc(fileName);
    }
    else if (suffix == SuffixThdl) {
//        src = new CProjectSrcThdl(fileName);
//        src->parsed(stream.readAll());
    }
    else if (suffix == SuffixVerilog) {
//        src = new CProjectSrcVerilog(fileName);
//        src->parsed(stream.readAll());
    }
    else if (suffix == SuffixVhdl) {
        src = new CProjectSrcVhdl(fileName);
        src->parsed(stream.readAll());
    }
    else if (suffix == SuffixUcf) {
        src = new CProjectSrcUcf(fileName);
        //src->parsed(stream.readAll());
    }

    return src;
}
//------------------------------------------------------------------


//
// THDL
//

//
// Verilog
//

//
// VHDL
//
QStringList CProjectSrcVhdl::list(CProjectSrc::SrcType type) const
{
    if (type == CProjectSrc::Cmp){
        QStringList cmp;
        QRegExp rx("\\b\\w+\\s-\\s");
        foreach(QString icmp,m_icmp)
            cmp << icmp.remove(rx).toLower();
        return cmp;
    }

    switch (type) {
    case CProjectSrc::Icmp:
        return m_icmp;
    case CProjectSrc::Lib:
        return m_lib;
    case CProjectSrc::Mif:
        return m_mif;
    default:;
    }

    return QStringList() << CProjectSrc::Unknown;
}
//------------------------------------------------------------------


QString CProjectSrcVhdl::next(CProjectSrc::SrcType type) const
{
    QString str(peekNext(type));
    ++m_i;
    return str;
}
//------------------------------------------------------------------


QString CProjectSrcVhdl::peekNext(CProjectSrc::SrcType type) const
{
    int i(m_i);
    ++i;

    switch (type) {
    case CProjectSrc::Cmp:
        return i < m_icmp.size() ? QString(m_icmp.at(i)).remove(QRegExp("\\b\\w+\\s-\\s")).toLower() :
                                     CProjectSrc::Unknown;
    case CProjectSrc::Icmp:
        return i < m_icmp.size() ? m_icmp.at(i) : CProjectSrc::Unknown;
    case CProjectSrc::Lib:
        return i < m_lib.size() ? m_lib.at(i) : CProjectSrc::Unknown;
    case CProjectSrc::Mif:
        return i < m_mif.size() ? m_mif.at(i) : CProjectSrc::Unknown;
    default:;
    }

    return CProjectSrc::Unknown;
}
//------------------------------------------------------------------


bool CProjectSrcVhdl::hasNext(CProjectSrc::SrcType type) const
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Icmp:
        return (m_i + 1) < m_icmp.size() ? true : false;
    case CProjectSrc::Lib:
        return (m_i + 1) < m_lib.size() ? true : false;
    case CProjectSrc::Mif:
        return (m_i + 1) < m_mif.size() ? true : false;
    default:;
    }

    return false;
}
//------------------------------------------------------------------


void CProjectSrcVhdl::parsed(const QString &listing)
{
    // find include packages and begin component
    QRegExp rx("\\buse\\s+work.([\\w\\d]+).\\w+\\b|(\\bentity|\\bpackage)\\s+(\\w+)\\s+is\\b");
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;
    while ((pos = rx.indexIn(listing,pos)) != -1 && rx.cap(2)== "") {
        m_lib << rx.cap(1);
        pos += rx.matchedLength();
    }

    m_compName = Unknown;
    if (pos != -1)
        m_compName = rx.cap(3);

    pos += rx.matchedLength();

    if (rx.cap(2) == "package") {
        m_type = Lib;
        return;
    }

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
            m_icmp << QString("%1 - %2").arg(rx.cap(1)).arg(rx.cap(2));
        }
        else {
            m_icmp << QString("%1 - %2").arg(rx.cap(4)).arg(rx.cap(5));
        }
        pos += rx.matchedLength();
    }

    // find connect mif files
    rx.setPattern("\"([\\w\\d/.]+.mif)\"");
    pos = 0;
    while ((pos = rx.indexIn(listing,pos)) != -1) {
        if (!m_mif.contains(QFileInfo(rx.cap(1)).absoluteFilePath()))
            m_mif << QFileInfo(rx.cap(1)).absoluteFilePath();
        pos += rx.matchedLength();
    }
}
//------------------------------------------------------------------
