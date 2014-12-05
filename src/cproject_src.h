#ifndef CPROJECT_SRC_H
#define CPROJECT_SRC_H

#include <QMap>
#include <QString>
#include <QStringList>

class CProjectSrc
{
public:
    CProjectSrc(){}
    virtual ~CProjectSrc();

    virtual QString compName() const  {return Noname;}
    virtual QString fileName() const  {return Noname;}

    virtual QString comp() const {return Noname;}

    virtual QString inst() const {return Noname;}
    virtual QString beginInst() {return inst();}
    virtual QString nextInst() {return inst();}
    virtual QString prevInst() {return inst();}

    virtual void parsed(const QString&) {};

    static CProjectSrc* create(const QString& fileName);

    static const QString SuffixNgc;
    static const QString SuffixThdl;
    static const QString SuffixVerilog;
    static const QString SuffixVhdl;
    static const QString SuffixUcf;

    static const QString Noname;
};
//--------------------------------------------------

//class CProjectSrcNgc : public CProjectSrc
//{
//public:
//    explicit CProjectSrcNgc(const QString& fileName);

//    QString nameComp(){return UnknownComponent;}
//    QString nameFile(){return m_fileName;}

//    const QString& nameComp() const {return UnknownComponent;}
//    const QString& nameFile() const {return m_fileName;}

//    QMap<QString,QStringList> componets() {return QMap<QString,QStringList>();}
//    const QMap<QString,QStringList> componets() const {return QMap<QString,QStringList>();}

//    void parsed(const QString&){}

//private:

//    CProjectSrcNgc(){}

//    QString m_fileName;
//};
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
    explicit CProjectSrcVhdl(const QString& fileName = "");

    QString compName() const {return m_compName;}
    QString fileName() const {return m_fileName;}

//    QString comp() const {return m_it != m_instances.end() ? m_it.value() : Noname;}

//    QString inst() const {return m_it != m_instances.end() ? mergeName(m_it.key()) : Noname;}
//    QString beginInst()  {return m_instances.isEmpty() ? Noname : mergeName((m_it = m_instances.begin()).key());}
//    QString nextInst()   {m_it != m_instances.end()   ? ++m_it : m_it; return inst();}
//    QString prevInst()   {m_it != m_instances.begin() ? --m_it : m_it = m_instances.end(); return inst();}

    QString comp() const {return m_i < m_inst.size() ? comp(m_i) : Noname;}

    QString inst() const {return m_i < m_inst.size() ? m_inst.at(m_i) : Noname;}
    QString beginInst()  {return m_inst.isEmpty() ? Noname : m_inst.at(m_i = 0);}
    QString nextInst()   {m_i < m_inst.size() ? ++m_i : m_i; return inst();}
    QString prevInst()   {m_i > 0 ? --m_i : m_i = m_inst.size(); return inst();}

    void parsed(const QString& listing);

private:

    //CProjectSrcVhdl(){}

    QString m_fileName;                 //
    QString m_compName;

    //QMap<QString,QStringList> m_compInst;   // first - name comp,
    QStringList m_packages;                 // list include packages

    QStringList m_inst;
    QMap<QString,QString> m_instances;      // first - names instances of comp, second - name comp

    QMap<QString,QString>::iterator m_it;
    int m_i;

    QString mergeName(const QString& instName) const {
        return QString("%1 - %2").arg(instName)
                                 .arg(m_instances.value(instName,Noname));
    }

    QString comp(int i) const {
        return (QString(m_inst.at(i)).remove(QRegExp("\\b\\w+\\s-\\s"))).toLower();
    }
};
//--------------------------------------------------


//class CProjectSrcUcf : public CProjectSrc
//{
//public:
//    explicit CProjectSrcUcf(const QString& fileName);

//    QString nameComp(){return UnknownComponent;}
//    QString nameFile(){return m_fileName;}

//    const QString& nameComp() const {return UnknownComponent;}
//    const QString& nameFile() const {return m_fileName;}

//    QMap<QString,QStringList> componets() {return QMap<QString,QStringList>();}
//    const QMap<QString,QStringList> componets() const {return QMap<QString,QStringList>();}

//    void parsed(const QString&){}

//private:

//    CProjectSrcUcf(){}

//    QString m_fileName;
//};
//--------------------------------------------------

Q_DECLARE_METATYPE(CProjectSrc)

#endif // CPROJECT_SRC_H
