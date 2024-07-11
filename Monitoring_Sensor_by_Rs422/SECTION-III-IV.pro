#-------------------------------------------------
#
# Project created by QtCreator 2023-12-22T07:21:09
#
#-------------------------------------------------

QT       += core gui printsupport serialport
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SECTION-III-IV
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    receving_data.cpp \
    global_for_all.cpp \
    receving_for_fuze.cpp

HEADERS += \
        mainwindow.h \
    receving_data.h \
    global_for_all.h \
    receving_for_fuze.h

FORMS += \
        mainwindow.ui
