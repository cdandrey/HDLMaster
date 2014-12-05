#ifndef CPROJECT_OBJECT_H
#define CPROJECT_OBJECT_H

#include <QObject>

#include "cproject.h"

class CProjectObject : public QObject
{
    Q_OBJECT
public:
    explicit CProjectObject(QObject *parent = 0);
    explicit CProjectObject(const QString& proFileName,QObject *parent = 0);
    ~CProjectObject();

    void setProFile(const QString& proFileName) {m_pro->setProFile(proFileName);}

private:

    CProject *m_pro;

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);

    void succefull(CProject*);
    void succefull();

private slots:

    void openXise();

};

#endif // CPROJECT_OBJECT_H
