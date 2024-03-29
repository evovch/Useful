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

OTHER_FILES += shaders/vertSh_shading.vp \
               shaders/geomSh_shading.gp \
               shaders/frSh_shading.fp \
               shaders/vertSh_wire.vp \
               shaders/geomSh_wire.gp \
               shaders/frSh_wire.fp \
               shaders/vertSh_points.vp \
               shaders/geomSh_points.gp \
               shaders/frSh_points.fp

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

LIBS    += -lGLEW

SOURCES += main.cpp\
           support.cpp \
           mainwindow.cpp \
           cls_myglwidget.cpp \
           cls_displaymodel.cpp \
           cls_mycamera.cpp

HEADERS += mainwindow.h \
           support.h \
           cls_myglwidget.h \
           cls_displaymodel.h \
           cls_mycamera.h

FORMS   += mainwindow.ui
