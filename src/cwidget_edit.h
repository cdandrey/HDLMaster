#ifndef CWIDGET_EDIT_H
#define CWIDGET_EDIT_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QAction;
class QActionGroup;
class QComboBox;
class QTextEdit;
QT_END_NAMESPACE

class CWidgetEdit : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetEdit(QWidget *parent = 0);

public slots:

private:

    CToolBarHeader  *m_header;

    QComboBox       *m_combo;
    QTextEdit       *m_edit;

private slots:
    void triggered_actHide();
};

#endif // CWIDGET_EDIT_H
