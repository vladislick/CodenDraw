#-------------------------------------------------
#
# Project created by QtCreator 2019-06-06T15:16:16
#
#-------------------------------------------------

QT       += core gui serialport svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodenDraw
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/aboutlicence.cpp \
        src/aboutwindow.cpp \
        src/codeeditor.cpp \
        src/configfile.cpp \
        src/customgraphicsscene.cpp \
        src/icon.cpp \
        src/main.cpp \
        src/mainscene.cpp \
        src/mainwindow.cpp

HEADERS += \
        src/aboutlicence.h \
        src/aboutwindow.h \
        src/codeeditor.h \
        src/configfile.h \
        src/customgraphicsscene.h \
        src/icon.h \
        src/mainscene.h \
        src/mainwindow.h

FORMS += \
        ui/aboutlicence.ui \
        ui/aboutwindow.ui \
        ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target