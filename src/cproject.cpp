#include "cproject.h"

#include <QDebug>
#include <QFileInfo>

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

    rx.setPattern("<file\\s+xil_pn:name=\"([\\w\\d-/.]+)\"\\s+xil_pn:type=\"\\w+\">");

    QFileInfo fileInfoItem;
    CProjectSrc *proSrc;

    while ((pos = rx.indexIn(listing,pos)) != -1) {
        fileInfoItem.setFile(rx.cap(1));
        proSrc = CProjectSrc::create(fileInfoItem.absoluteFilePath());
        if (proSrc != NULL) {
            m_fileComp.insert(proSrc->fileName(),proSrc);
            m_compFile.insert(proSrc->compName().toLower(),proSrc->fileName());
        }
        proSrc = NULL;
        pos += rx.matchedLength();
    }

    if (m_fileComp.isEmpty())
        return false;

    // get top file
    pos = 0;
    rx.setPattern("<property\\s+xil_pn:name=\"Implementation Top File\"\\s+xil_pn:value=\"([\\w\\d/.]+)\"\\s+"
                  "xil_pn:valueState=\"[\\w\\d-]+\"\\/>");

    if ((pos = rx.indexIn(listing,pos)) > -1){
        fileInfoItem.setFile(rx.cap(1));
        m_top = fileInfoItem.absoluteFilePath();
        pos += rx.matchedLength();
    } else {
        m_top = m_compFile.begin().value();
        pos = 0;
    }

    // get name project
    rx.setPattern("<property\\s+xil_pn:name=\"PROP_DesignName\"\\s+xil_pn:value=\"([\\w\\d]+)\"\\s+"
                  "xil_pn:valueState=\"[\\w\\d-]+\"\\/>");

    m_name = rx.indexIn(listing,pos) > -1 ? rx.cap(1) : CProjectSrc::Noname;

    return true;
}
//------------------------------------------------------------------


void CProject::setProFile(const QString &fileName)
{
    m_file = fileName;
    m_name = CProjectSrc::Noname;
    m_top = CProjectSrc::Noname;
    m_compFile.clear();
    m_fileComp.clear();
}
//------------------------------------------------------------------
