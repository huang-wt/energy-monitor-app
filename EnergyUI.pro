#-------------------------------------------------
#
# Project created by QtCreator 2023-03-10T14:21:23
#
#-------------------------------------------------

QT       += core gui

QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EnergyUI
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
    src/main.cpp \
    src/mainwindow.cpp \
    src/memory.cpp \
    src/power.cpp \
    src/power_dao.cpp \
    src/process.cpp \
    src/processor.cpp \
    src/system_parser.cpp \
    src/system.cpp \
    src/format.cpp \
    src/date_time.cpp

HEADERS += \
    headers/mainwindow.h \
    headers/command.h \
    headers/memory.h \
    headers/power.h \
    headers/power_dao.h \
    headers/process.h \
    headers/processor.h \
    headers/system_parser.h \
    headers/system.h \
    headers/format.h \
    headers/date_time.h

FORMS += \
        forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/qss.qrc \
    resources/icons.qrc

OTHER_FILES += \
    data/hours_usage.csv \
    data/days_usage.csv
