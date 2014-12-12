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

    bool create(const QString& proFileName);
    bool clear();

    QString fileName() {return isEmpty() ?  CProjectSrc::Unknown:m_pro->proFile();}
    QString projectName() {return isEmpty() ? CProjectSrc::Unknown:m_pro->proName();}

    CProject* project() {return m_pro;}

    bool isEmpty() {return m_pro == NULL ? true:false;}

private:

    CProject *m_pro;

signals:

    void messageAppend(QString);
    void messageSet(QString);

    void finished(bool);

private slots:

    void open();

};

#endif // CPROJECT_OBJECT_H
