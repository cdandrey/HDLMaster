#ifndef CPROJECT_H
#define CPROJECT_H

#include "cproject_src.h"

class CProject
{
public:

    explicit CProject(const QString& fileName = CProjectSrc::Unknown);

    ~CProject();

    QString proFile()    const {return m_file;}
    QString proName()    const {return m_name;}
    QString proTopFile() const {return m_top;}

    QString compName(const QString& file);
    QString fileName(const QString& comp);

    const CProjectSrc* next() const {return m_it.next().value();}
    const CProjectSrc* peekNext() const {return m_it.peekNext().value();}
    bool hasNext() const {return m_it.hasNext();}
    bool toFile(CProjectSrc::SrcType type,const QString& file) const;
    void toFront(CProjectSrc::SrcType type) const;

    const CProjectSrc *src(CProjectSrc::SrcType type,const QString& file);
    const CProjectSrc *src() const {return m_it.value();}

    bool open(const QString& fileName);
    bool parsedXise(const QString& listing);

    static const QString SuffixMpro;
    static const QString SuffixXise;

private:

    QString m_file;
    QString m_name;
    QString m_top;

    QMap<QString,CProjectSrc*> m_fileCmp;
    QMap<QString,QString> m_cmpFile;

    QMap<QString,CProjectSrc*> m_fileLib;
    QMap<QString,QString> m_libFile;

    QMap<QString,CProjectSrc*> m_fileMif;
    QMap<QString,CProjectSrc*> m_fileNgc;
    QMap<QString,CProjectSrc*> m_fileUcf;

    mutable QMapIterator<QString,CProjectSrc*> m_it;
};

Q_DECLARE_METATYPE(CProject)

#endif // CPROJECT_H
