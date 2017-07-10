#-------------------------------------------------
#
# Project created by QtCreator 2016-08-13T14:48:30
#
#-------------------------------------------------

QT       += core gui charts script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport script

TARGET = ProgaDiff
TEMPLATE = app

LIBS += -lopengl32
DEFINES += QCUSTOMPLOT_USE_OPENGL


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    dialog.cpp \
    itercore.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    dialog.h \
    comments.h \
    itercore.h \
    settings.h \
    global_variable.h

FORMS    += mainwindow.ui \
    dialog.ui \
    settings.ui

RESOURCES += \
    resourse.qrc
