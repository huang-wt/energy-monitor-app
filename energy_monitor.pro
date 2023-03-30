QT       += core gui

QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = energy_monitor
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

CONFIG += c++17

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
    include/mainwindow.h \
    include/command.h \
    include/memory.h \
    include/power.h \
    include/power_dao.h \
    include/process.h \
    include/processor.h \
    include/system_parser.h \
    include/system.h \
    include/format.h \
    include/date_time.h

FORMS += \
        forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/qss.qrc \
    resources/icons.qrc
