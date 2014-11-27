#ifndef CPARSER_VHDL_H
#define CPARSER_VHDL_H

#include <QString>

class CParserVhdl
{
public:
    explicit CParserVhdl(const QString &text = "");

    QString componentName();

private:
    QString m_text;
};

#endif // CPARSER_VHDL_H
