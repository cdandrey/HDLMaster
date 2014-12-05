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
    ~CWidgetEdit();

private:

    CToolBarHeader  *m_header;

    QComboBox       *m_combo;
    QTextEdit       *m_edit;

    int comboCurrentIndex;
    QMap<QString,QString> files;

private slots:

    void view(const QString &fileName);
    void comboCurrentIndexChanged(int newCurrentIndex);
    void editTextChanged();
    void triggeredActHide();

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);
};

#endif // CWIDGET_EDIT_H
