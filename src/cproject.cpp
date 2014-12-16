#include "cproject.h"

#include <QDebug>
#include <QFileInfo>

const QString CProject::SuffixMpro("mpro");
const QString CProject::SuffixXise("xise");

CProject::CProject(const QString &fileName)
    : m_file(fileName),
      m_name(CProjectSrc::Unknown),
      m_top(CProjectSrc::Unknown),
      m_it(m_fileCmp)
{

}
//------------------------------------------------------------------


CProject::~CProject()
{
    qDeleteAll(m_fileCmp);
    qDeleteAll(m_fileLib);
    qDeleteAll(m_fileMif);
    qDeleteAll(m_fileNgc);
    qDeleteAll(m_fileUcf);
}
//------------------------------------------------------------------


QString CProject::compName(const QString &file) const
{
    return m_fileCmp.constFind(file) != m_fileCmp.cend() ?
            m_fileCmp.value(file)->compName() : CProjectSrc::Unknown;
}
//------------------------------------------------------------------


QString CProject::fileName(const QString &comp) const
{
    return m_cmpFile.value(comp.toLower(),CProjectSrc::Unknown);
}
//------------------------------------------------------------------


QStringList CProject::includes(const QString &cmp,CProjectSrc::SrcType type) const
{
    switch (type) {
    case CProjectSrc::Cmp:
        return m_cmpInc.value(cmp.toLower());
    case CProjectSrc::Lib:
        return m_libInc.value(cmp.toLower());
    case CProjectSrc::Mif:
        return m_mifInc.value(cmp);
    default:;
    }

    return QStringList();
}
//------------------------------------------------------------------


void CProject::toFront(CProjectSrc::SrcType type) const
{
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Mod:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileCmp);
        break;
    case CProjectSrc::Lib:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileLib);
        break;
    case CProjectSrc::Mif:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileMif);
        break;
    case CProjectSrc::Ngc:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileNgc);
        break;
    case CProjectSrc::Ucf:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileUcf);
        break;
    default:
        m_it = QMapIterator<QString,CProjectSrc*>(QMap<QString,CProjectSrc*>());
    }
}
//------------------------------------------------------------------


bool CProject::toFile(CProjectSrc::SrcType type, const QString &file) const
{
    QMap<QString,CProjectSrc*>::const_iterator itEnd;
    QMap<QString,CProjectSrc*>::const_iterator itFind;
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Mod:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileCmp);
        itEnd = m_fileCmp.cend();
        itFind = m_fileCmp.constFind(file);
        break;
    case CProjectSrc::Lib:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileLib);
        itEnd = m_fileLib.cend();
        itFind = m_fileLib.constFind(file);
        break;
    case CProjectSrc::Mif:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileMif);
        itEnd = m_fileMif.cend();
        itFind = m_fileMif.constFind(file);
        break;
    case CProjectSrc::Ngc:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileNgc);
        itEnd = m_fileNgc.cend();
        itFind = m_fileNgc.constFind(file);
        break;
    case CProjectSrc::Ucf:
        m_it = QMapIterator<QString,CProjectSrc*>(m_fileUcf);
        itEnd = m_fileUcf.cend();
        itFind = m_fileUcf.constFind(file);
        break;
    default:
        return false;
    }

    if (itEnd != itFind) {
        m_it.findNext(itFind.value());
        return true;
    }

    return false;
}
//------------------------------------------------------------------


const CProjectSrc* CProject::src(CProjectSrc::SrcType type, const QString &file)
{
    QMap<QString,CProjectSrc*>::const_iterator it;
    QMap<QString,CProjectSrc*>::const_iterator itEnd;
    switch (type) {
    case CProjectSrc::Cmp:
    case CProjectSrc::Mod:
        itEnd = m_fileCmp.cend();
        it = m_fileCmp.constFind(file);
        break;
    case CProjectSrc::Lib:
        itEnd = m_fileLib.cend();
        it = m_fileLib.constFind(file);
        break;
    case CProjectSrc::Mif:
        itEnd = m_fileMif.cend();
        it = m_fileMif.constFind(file);
        break;
    case CProjectSrc::Ngc:
        itEnd = m_fileNgc.cend();
        it = m_fileNgc.constFind(file);
        break;
    case CProjectSrc::Ucf:
        itEnd = m_fileUcf.cend();
        it = m_fileUcf.constFind(file);
        break;
    default:
        return &CProjectSrc::m_srcUnknown;
    }

    return it != itEnd ? it.value() : &CProjectSrc::m_srcUnknown;
}
//------------------------------------------------------------------


bool CProject::open(const QString&)
{
    return true;
}
//------------------------------------------------------------------


bool CProject::parsedXise(const QString& listing)
{
    // determ is xise project
    QRegExp rx("<version\\s+xil_pn\\s*:\\s*ise_version\\s*=\\s*\"\\s*[\\d.]+\\s*\"\\s+"
               "xil_pn\\s*:\\s*schema_version\\s*=\\s*\"\\s*[\\d.]+\\s*\"\\/>");

    int pos = 0;
    if ((pos = rx.indexIn(listing,pos)) == -1)
        return false;

    pos += rx.matchedLength();

    rx.setPattern("<file\\s+xil_pn:name=\"([\\w\\d-/.]+)\"\\s+xil_pn:type=\"\\w+\">"
                  "[\\w\\d:=\"/<>\\s]+Implementation[\\w\\d:=\"\\/<>\\s]+<\\/file>");

    QFileInfo fileInfoItem;
    CProjectSrc *src;

    while ((pos = rx.indexIn(listing,pos)) != -1) {

        fileInfoItem.setFile(rx.cap(1));
        src = CProjectSrc::create(fileInfoItem.absoluteFilePath());

        if (src != &CProjectSrc::m_srcUnknown) {

            if (src->suffix() == CProjectSrc::SuffixNgc) {
                m_fileNgc.insert(src->fileName(),src);
            } else if (src->suffix() == CProjectSrc::SuffixVhdl) {

                if (src->type() == CProjectSrc::Cmp) {
                    m_fileCmp.insert(src->fileName(),src);
                    m_cmpFile.insert(src->compName().toLower(),src->fileName());
                } else if (src->type() == CProjectSrc::Lib) {
                    m_fileLib.insert(src->fileName(),src);
                    m_libFile.insert(src->compName().toLower(),src->fileName());
                }

                src->toFront(CProjectSrc::Mif);
                while (src->hasNext(CProjectSrc::Mif)){
                    QString fileMif(src->next(CProjectSrc::Mif));
                    if (m_fileMif.constFind(fileMif) == m_fileMif.cend())
                        m_fileMif.insert(fileMif,new CProjectSrcMif(fileMif));
                }

            } else if (src->suffix() == CProjectSrc::SuffixUcf) {
                m_fileUcf.insert(src->fileName(),src);
            }
        }

        pos += rx.matchedLength();
    }

    if (m_fileCmp.isEmpty())
        return false;

    // create list includes for components, libreris and mif

    toFront(CProjectSrc::Cmp);
    while (hasNext()){
        QString nameCmp(peekNext()->compName());
        foreach (QString val, peekNext()->list(CProjectSrc::Cmp))
            m_cmpInc[val.toLower()] << nameCmp;
        foreach (QString val, peekNext()->list(CProjectSrc::Lib))
            m_libInc[val.toLower()] << nameCmp;
        foreach (QString val, next()->list(CProjectSrc::Mif))
            m_mifInc[QFileInfo(val).baseName()] << nameCmp;
    }

    toFront(CProjectSrc::Lib);
    while (hasNext()){
        QString nameCmp(peekNext()->compName());
        foreach (QString cmp, next()->list(CProjectSrc::Lib))
            m_libInc[cmp.toLower()] << nameCmp;
    }

    // get top file
    pos = 0;
    rx.setPattern("<property\\s+xil_pn:name=\"Implementation Top File\"\\s+xil_pn:value=\"([\\w\\d/.]+)\"\\s+"
                  "xil_pn:valueState=\"[\\w\\d-]+\"\\/>");

    if ((pos = rx.indexIn(listing,pos)) > -1){
        fileInfoItem.setFile(rx.cap(1));
        m_top = fileInfoItem.absoluteFilePath();
        pos += rx.matchedLength();
    } else {
        m_top = m_cmpFile.begin().value();
        pos = 0;
    }

    // get name project
    rx.setPattern("<property\\s+xil_pn:name=\"PROP_DesignName\"\\s+xil_pn:value=\"([\\w\\d]+)\"\\s+"
                  "xil_pn:valueState=\"[\\w\\d-]+\"\\/>");

    m_name = rx.indexIn(listing,pos) > -1 ? rx.cap(1) : CProjectSrc::Unknown;

    return true;
}
//------------------------------------------------------------------


//void CProject::setProFile(const QString &fileName)
//{
//    m_file = fileName;
//    m_name = CProjectSrc::Unknown;
//    m_top = CProjectSrc::Unknown;

//    m_cmpFile.clear();
//    m_fileCmp.clear();
//}
//------------------------------------------------------------------

