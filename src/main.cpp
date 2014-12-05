#include "cwindow_main.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CWindowMain w;
    w.setWindowIcon(QIcon(":/ico/logo.png"));
    w.showMaximized();

    return a.exec();
}
