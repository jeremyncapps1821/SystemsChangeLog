#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T17:21:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SystemsChangeLog
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filterbydialog.cpp \
    browselogsdialog.cpp \
    eventlogviewer.cpp \
    logdetailsdialog.cpp

HEADERS  += mainwindow.h \
    filterbydialog.h \
    browselogsdialog.h \
    eventlogviewer.h \
    logdetailsdialog.h

FORMS    += mainwindow.ui \
    filterbydialog.ui \
    browselogsdialog.ui \
    eventlogviewer.ui \
    logdetailsdialog.ui

DISTFILES += \
    .gitignore \
    LICENSE \
    README.md \
    HISTORY.txt \
    INSTALL.txt
