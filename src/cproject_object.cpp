#include "cproject_object.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QThread>

CProjectObject::CProjectObject(QObject *parent) :
    QObject(parent),
    m_pro(new CProject())
{ }
//------------------------------------------------------------------


CProjectObject::CProjectObject(const QString &proFileName, QObject *parent) :
    QObject(parent),
    m_pro(new CProject(proFileName))
{ }
//------------------------------------------------------------------


CProjectObject::~CProjectObject()
{
    delete m_pro;
}
//------------------------------------------------------------------


void CProjectObject::openXise()
{
    emit messageSet(tr("Читаются файлы проекта %1").arg(m_pro->proFile()));

    QFile proFile(m_pro->proFile());

    if (!proFile.open(QIODevice::ReadOnly)){
        emit messageAppend(QString("%1 %2")
                           .arg(tr("ОШИБКА! Невозможно открыть файл"))
                           .arg(m_pro->proFile()));
        emit succefull();
        return;
    }

    QTextStream stream(&proFile);

    QString listing(stream.readAll());

    if (!m_pro->parsedXise(listing)){
        emit succefull();
        return;
    }

    emit succefull();
    emit messageAppend(tr("Чтение файлов успешно завершенно."));
    emit succefull(m_pro);
}
//------------------------------------------------------------------
