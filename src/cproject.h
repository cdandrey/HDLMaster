#ifndef CPROJECT_H
#define CPROJECT_H

#include "cproject_src.h"

class CProject
{
public:

    explicit CProject(const QString& fileName = CProjectSrc::Noname)
                : m_file(fileName),
                  m_name(CProjectSrc::Noname),
                  m_top(CProjectSrc::Noname){}

    QString proFile()    const {return m_file;}
    QString proName()    const {return m_name;}
    QString proTopFile() const {return m_top;}

    QString compName(const QString& file) {return m_fileComp.value(file)->compName();}
    QString fileName(const QString& comp) {return m_compFile.value(comp,CProjectSrc::Noname);}

    QString inst(const QString& file) const {return m_fileComp.value(file)->inst();}
    QString instComp(const QString& file) const {return m_fileComp.value(file)->comp();}
    QString beginInst(const QString& file) const {return m_fileComp.value(file)->beginInst();}
    QString nextInst(const QString& file) const {return m_fileComp.value(file)->nextInst();}
    QString prevInst(const QString& file) const {return m_fileComp.value(file)->prevInst();}

    bool open(const QString& fileName);
    bool parsedXise(const QString& listing);

    void setProFile(const QString& fileName);

private:

    QString m_file;
    QString m_name;
    QString m_top;

    QMap<QString,CProjectSrc*> m_fileComp;
    QMap<QString,QString> m_compFile;
};

Q_DECLARE_METATYPE(CProject)

#endif // CPROJECT_H
