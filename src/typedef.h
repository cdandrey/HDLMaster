#ifndef CBFTYPEDEF_H
#define CBFTYPEDEF_H


#include <qmath.h>

#include <QDebug>

#include <QList>
#include <QMap>
#include <QPair>
#include <QSet>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVector>

typedef QByteArray         TByt;

typedef QString            TStr;
typedef QStringList        TSLst;

typedef QLinkedList<int>   TLnk;

typedef QList<int>         TLst;
typedef QList<TLst>        TLLst;

typedef QVector<int>       TVec;
typedef QVector<TVec>      TVVec;
typedef QVector<double>    TVecd;
typedef QVector<TVecd>     TVVecd;
typedef QVector<TStr>      TVStr;

typedef QSet<int>          TSet;
typedef QSet<TSet>         TSSet;

typedef QList<TByt>        TLByt;
typedef QList<TLnk>        TLLnk;
typedef QList<TSet>        TLSet;

typedef QVector<TLst>      TVLst;
typedef QVector<TSet>      TVSet;

typedef QPair<TSet,TSet>    TPSet;

#endif // CBFTYPEDEF_H
