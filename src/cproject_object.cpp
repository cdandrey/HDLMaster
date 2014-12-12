#include "cproject_object.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QThread>

CProjectObject::CProjectObject(QObject *parent) :
    QObject(parent),
    m_pro(NULL)
{

}
//------------------------------------------------------------------


CProjectObject::CProjectObject(const QString &proFileName, QObject *parent) :
    QObject(parent),
    m_pro(new CProject(proFileName))
{

}
//------------------------------------------------------------------


CProjectObject::~CProjectObject()
{
    if (!isEmpty())
        delete m_pro;
}
//------------------------------------------------------------------


bool CProjectObject::create(const QString &proFileName)
{
    qDebug() << "create "  << proFileName;

    if (!isEmpty())
        return false;

    QFileInfo info(proFileName);

    if (info.suffix() != CProject::SuffixMpro &&
        info.suffix() != CProject::SuffixXise)
    {
        emit messageAppend(tr("ОШИБКА! Неизвестный формат файла проекта."));
        return false;
    }

    if (!info.isReadable()) {
        emit messageAppend(QString("%1 %2")
                           .arg(tr("ОШИБКА! Невозможно открыть файл"))
                           .arg(proFileName));
        return false;
    }

    m_pro = new CProject(proFileName);

    return true;
}
//------------------------------------------------------------------


bool CProjectObject::clear()
{
    if (!isEmpty()) {
        delete m_pro;
        m_pro = NULL;
        return true;
    }

    return false;
}
//------------------------------------------------------------------


void CProjectObject::open()
{
    if (isEmpty())
        return;

    QFile proFile(m_pro->proFile());

    if (!proFile.open(QIODevice::ReadOnly)){
        emit messageAppend(QString("%1 %2").arg(tr("ОШИБКА! Невозможно открыть файл"))
                           .arg(m_pro->proFile()));
        emit finished(false);
        return;
    }

    QTextStream stream(&proFile);

    if (QFileInfo(proFile).suffix() == CProject::SuffixXise)
        if (!m_pro->parsedXise(stream.readAll())){
            emit messageAppend(tr("ОШИБКА! Возникла ошбика во время чтения файла проекта."));
            emit finished(false);
            return;
        }

    emit finished(true);
}
//------------------------------------------------------------------
