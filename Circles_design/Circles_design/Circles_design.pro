QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circles_design
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cls_DrawWidget.cpp \
    cls_Circle.cpp

HEADERS  += mainwindow.h \
    cls_DrawWidget.h \
    cls_Circle.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic
