#ifndef CPROJECT_SRC_H
#define CPROJECT_SRC_H

#include <QMap>
#include <QString>
#include <QStringList>

class CProjectSrc
{
public:

    enum SrcType {Cmp,Icmp,Lib,Mif,Ngc,Ucf};

    CProjectSrc(){}
    virtual ~CProjectSrc() {}

    virtual QString compName() const {return Unknown;}
    virtual QString fileName() const  {return Unknown;}
    virtual CProjectSrc::SrcType type() const {return CProjectSrc::Cmp;}

    virtual QStringList list(CProjectSrc::SrcType) const {return QStringList();}

    virtual QString next(CProjectSrc::SrcType) const {return Unknown;}
    virtual QString peekNext(CProjectSrc::SrcType) const {return Unknown;}
    virtual void toFront() const {return;}
    virtual bool hasNext(CProjectSrc::SrcType) const {return false;}

    virtual void parsed(const QString&) {}

    virtual QString suffix() const {return Unknown;}
    virtual QString suffixIco() const {return UnknownIco;}

    static CProjectSrc* create(const QString& fileName);

    static const CProjectSrc m_srcUnknown;
    static const QString Unknown;
    static const QString UnknownIco;

    static const QString SuffixMif;
    static const QString SuffixNgc;
    static const QString SuffixThdl;
    static const QString SuffixVerilog;
    static const QString SuffixVhdl;
    static const QString SuffixUcf;

    static const QString SuffixIcoMif;
    static const QString SuffixIcoNgc;
    static const QString SuffixIcoThdl;
    static const QString SuffixIcoVerilog;
    static const QString SuffixIcoVhdl;
    static const QString SuffixIcoUcf;

protected:

    QString baseName(const QString& file) const;

};
//--------------------------------------------------


//
// Delivered classes
//


class CProjectSrcMif : public CProjectSrc
{
public:

    explicit CProjectSrcMif(const QString& fileName = "") :
          m_fileName(fileName)
    {}

    QString compName() const {return CProjectSrc::baseName(m_fileName);}
    QString fileName() const {return m_fileName;}
    CProjectSrc::SrcType type() const {return CProjectSrc::Mif;}

    virtual QString suffix() const {return CProjectSrc::SuffixMif;}
    virtual QString suffixIco() const {return CProjectSrc::SuffixIcoMif;}

private:

    QString m_fileName;
};
//--------------------------------------------------


class CProjectSrcNgc : public CProjectSrc
{
public:

    explicit CProjectSrcNgc(const QString& fileName = "") :
          m_fileName(fileName)
    {}

    QString compName() const {return CProjectSrc::baseName(m_fileName);}
    QString fileName() const {return m_fileName;}
    CProjectSrc::SrcType type() const {return CProjectSrc::Ngc;}

    virtual QString suffix() const {return CProjectSrc::SuffixNgc;}
    virtual QString suffixIco() const {return CProjectSrc::SuffixIcoNgc;}

private:

    QString m_fileName;
};
//--------------------------------------------------


//class CProjectSrcThdl : public CProjectSrc
//{
//public:
//    explicit CProjectSrcThdl(const QString& fileName);

//    QString nameComp(){return m_compName;}
//    QString nameFile(){return m_fileName;}

//    const QString& nameComp() const {return m_compName;}
//    const QString& nameFile() const {return m_fileName;}

//    QMap<QString,QStringList> componets() {return QMap<QString,QStringList>();}
//    const QMap<QString,QStringList> componets() const {return QMap<QString,QStringList>();}

//    void parsed(const QString&){}

//private:

//    CProjectSrcThdl(){}

//    QString m_fileName;
//    QString m_compName;

//    QStringList m_containComps;
//    QStringList m_includPackages;

//};
//--------------------------------------------------


//class CProjectSrcVerilog : public CProjectSrc
//{
//public:
//    explicit CProjectSrcVerilog(const QString& fileName);

//    QString nameComp(){return m_compName;}
//    QString nameFile(){return m_fileName;}

//    const QString& nameComp() const {return m_compName;}
//    const QString& nameFile() const {return m_fileName;}

//    QMap<QString,QStringList> componets() {return QMap<QString,QStringList>();}
//    const QMap<QString,QStringList> componets() const {return QMap<QString,QStringList>();}

//    void parsed(const QString&){}

//private:

//    CProjectSrcVerilog(){}

//    QString m_fileName;
//    QString m_compName;

//    QStringList m_containComps;
//};
//--------------------------------------------------


class CProjectSrcVhdl : public CProjectSrc
{
public:

    explicit CProjectSrcVhdl(const QString& fileName = "") :
          m_fileName(fileName),
          m_compName(""),
          m_type(Cmp),
          m_i(-1)
    {}

    QString compName() const {return m_compName;}
    QString fileName() const {return m_fileName;}
    CProjectSrc::SrcType type() const {return m_type;}

    QStringList list(CProjectSrc::SrcType type) const;
    QString next(CProjectSrc::SrcType type) const;
    QString peekNext(CProjectSrc::SrcType type) const;
    void toFront() const {m_i = -1;}
    bool hasNext(CProjectSrc::SrcType type) const;

    void parsed(const QString& listing);

    QString suffix() const {return CProjectSrc::SuffixVhdl;}
    QString suffixIco() const {return CProjectSrc::SuffixIcoVhdl;}

private:

    QString m_fileName;
    QString m_compName;
    CProjectSrc::SrcType m_type;

    QStringList m_icmp;
    QStringList m_lib;
    QStringList m_mif;

    mutable int m_i;
};
//--------------------------------------------------


class CProjectSrcUcf : public CProjectSrc
{
public:

    explicit CProjectSrcUcf(const QString& fileName = "") :
          m_fileName(fileName)
    {}

    QString compName() const {return CProjectSrc::baseName(m_fileName);}
    QString fileName() const {return m_fileName;}
    CProjectSrc::SrcType type() const {return CProjectSrc::Ucf;}

    virtual QString suffix() const {return CProjectSrc::SuffixUcf;}
    virtual QString suffixIco() const {return CProjectSrc::SuffixIcoUcf;}

private:

    QString m_fileName;
};
//--------------------------------------------------

Q_DECLARE_METATYPE(CProjectSrc)

#endif // CPROJECT_SRC_H
