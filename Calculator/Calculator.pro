QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calculator
TEMPLATE = app


SOURCES += main.cpp\
    eqcalc.cpp \
    interface.cpp

HEADERS  += \
    eqcalc.h \
    interface.h

FORMS    +=
