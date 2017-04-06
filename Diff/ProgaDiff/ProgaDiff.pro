#-------------------------------------------------
#
# Project created by QtCreator 2016-08-13T14:48:30
#
#-------------------------------------------------

QT       += core gui charts script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport script

TARGET = ProgaDiff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    dialog.cpp \
    itercore.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    dialog.h \
    comments.h \
    itercore.h

FORMS    += mainwindow.ui \
    dialog.ui
