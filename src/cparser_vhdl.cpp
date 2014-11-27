#include "cparser_vhdl.h"

#include <QRegExp>

CParserVhdl::CParserVhdl(const QString &text)
    : m_text(text)
{
}
//------------------------------------------------------------------


QString CParserVhdl::componentName()
{
    QRegExp rxCompName("(\\bentity|\\bpackage)\\s+\\w+(?=\\s+\\is\\b)");
    rxCompName.setCaseSensitivity(Qt::CaseInsensitive);

    if (rxCompName.indexIn(m_text,0) > -1) {

        QString compName(rxCompName.cap());
        int len = compName.length();

        compName = compName.remove("entity",Qt::CaseInsensitive).trimmed();

        if (compName.length() < len)
            return compName;
        else
            return compName.remove("package",Qt::CaseInsensitive).trimmed();
    }

    return "<unknown>";
}
//------------------------------------------------------------------


