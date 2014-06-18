#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T16:53:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CsvExport
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csvpcbfile.cpp

HEADERS  += mainwindow.h \
    csvpcbfile.h

FORMS    += mainwindow.ui

LIBS +=  -L/usr/local/lib -lcsv
