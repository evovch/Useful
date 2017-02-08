QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtRootTreeTest

TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           classes.cpp \
           Go4QtRoot/QRootApplication.cpp \
           Go4QtRoot/QRootCanvas.cpp \
           Go4QtRoot/QRootDialog.cpp \
           Go4QtRoot/QRootWindow.cpp \
           gui/GraphWidget.cpp \
           gui/HistoWidget.cpp \
           ../../build-qtRootTreeTest-Desktop_Qt_5_7_0_GCC_64bit-Debug/classesdict.cxx

HEADERS += mainwindow.h \
           classes.h \
           Go4QtRoot/QRootApplication.h \
           Go4QtRoot/QRootCanvas.h \
           Go4QtRoot/QRootDialog.h \
           Go4QtRoot/QRootWindow.h \
           Go4QtRoot/TGo4LockGuard.h \
           gui/GraphWidget.h \
           gui/HistoWidget.h

FORMS   += mainwindow.ui \
           gui/GraphWidget.ui \
           gui/HistoWidget.ui

# Set correct path to your ROOT include directory

INCLUDEPATH += /opt/root/root_build/include \
    Go4QtRoot/ \
    gui/

# Set correct path to your ROOT libs directory

LIBS += -lX11 \
        -L/opt/root/root_build/lib \
        -lGui -lCore -lRIO -lNet -lHist -lGraf -lGraf3d \
        -lGpad -lTree -lRint -lPostscript -lMatrix \
        -lPhysics -lMathCore -lThread -lMultiProc -lCling \
        -lGed -lTreePlayer \
        -pthread -lm -ldl -rdynamic

# Use also -lCint for ROOT5

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic
