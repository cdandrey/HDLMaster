#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T14:28:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HDLMaster
TEMPLATE = app


SOURCES += main.cpp \
    cwindow_main.cpp \
    ctoolbar_header.cpp \
    cwidget_consol.cpp \
    cwidget_edit.cpp \
    cwidget_tree.cpp \
    cproject.cpp \
    cparser_vhdl.cpp

HEADERS  += \
    cwindow_main.h \
    ctoolbar_header.h \
    cwidget_consol.h \
    cwidget_edit.h \
    cwidget_tree.h \
    cproject.h \
    typedef.h \
    cparser_vhdl.h

RESOURCES += \
    ico.qrc
