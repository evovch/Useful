#-------------------------------------------------
#
# Project created by QtCreator 2017-12-18T13:17:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

LIBS    += -lGLEW

SOURCES += main.cpp\
           mainwindow.cpp \
           cls_myglwidget.cpp \
    cls_displaymodel.cpp \
    support.cpp

HEADERS += mainwindow.h \
           cls_myglwidget.h \
    cls_displaymodel.h \
    support.h

FORMS   += mainwindow.ui
