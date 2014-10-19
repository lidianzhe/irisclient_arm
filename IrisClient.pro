#-------------------------------------------------
#
# Project created by QtCreator 2011-02-18T17:05:07
#
#-------------------------------------------------

QT       += core gui sql

TARGET = IrisClient
TEMPLATE = app

# Change the following version number
CMIRIS_VERSION = 2.1.3
CMICOLOR_VERSION = 0.9.0 # cmicolorLib version
EMA_VERSION = 1.0.1
# End of Change the following version number

SOURCES += mainwindow.cpp \
    main.cpp \
    cdatabase.cpp \
    dialogname.cpp \
    eventthread.cpp \
    dmeventthread.cpp \
    dbrecord.cpp

HEADERS  += mainwindow.h \
    cdatabase.h \
    dialogname.h \
    eventthread.h \
    dmeventthread.h \
    dbrecord.h

FORMS    += mainwindow.ui \
    dialogname.ui

OTHER_FILES += captureSound.wav

RESOURCES +=

# Naming Convention
# libCategory_OS_LIB_DIR, LIB_NAME, INCLUDE_DIR
# libCategory = CMIRIS, EMA
# OS = WIN, LINUX, ANDROID

# CPU = arm, x86, x64

win32 {
    message(win32)

    WIN_LIB_DIR = ../lib
    WIN_INCLUDE_DIR = ../include

    INCLUDEPATH += $$WIN_INCLUDE_DIR
    CONFIG(debug, debug|release) {
        #DEFINES += QT_NO_DEBUG_OUTPUT
        #DEFINES += QT_NO_WARNING_OUTPUT
        LIBS += $${WIN_LIB_DIR}/cmirisLib2d.lib
        LIBS += $${WIN_LIB_DIR}/cmicolorLibd.lib
    } else {
        DEFINES += QT_NO_DEBUG_OUTPUT
        DEFINES += QT_NO_WARNING_OUTPUT
        LIBS += $${WIN_LIB_DIR}/cmirisLib2.lib
        LIBS += $${WIN_LIB_DIR}/cmicolorLib.lib
        irisclient.files = ./release/$${TARGET}.exe
        irisclient.path = ../bin
        INSTALLS += IrisClient
    }
}

linux-g++ {
    LINUX_LIB_DIR = /usr/local/lib
    LINUX_INCLUDE_DIR = /usr/local/include
    DEFINES += _EMALIB
    SOURCES += emaeventthread.cpp
    HEADERS += emaeventthread.h

    exists(/usr/lib/arm-linux-gnueabihf) {
        DEFINES += _EXYNOS_ARM
        CPU = arm
    } else {
        exists(/lib/x86_64-linux-gnu) {
            CPU = x64
        } else {
            CPU = x86
        }
    }

    message(linux-g++ $$CPU)

    CMIRIS_LINUX_LIB_NAME = cmirisLib2_$${CPU}_$$CMIRIS_VERSION
    CMICOLOR_LINUX_LIB_NAME = cmicolorLib_$${CPU}_$$CMICOLOR_VERSION
    EMA_LINUX_LIB_NAME = emaLib_$${CPU}_$$EMA_VERSION

    message(cmirisLib2: $${LINUX_LIB_DIR}/$$CMIRIS_LINUX_LIB_NAME)
    message(cmicolorLib: $${LINUX_LIB_DIR}/$$CMICOLOR_LINUX_LIB_NAME)
    message(emaLib: $${LINUX_LIB_DIR}/$$EMA_LINUX_LIB_NAME)

    DEFINES += _LIBUDEV _LIBUSB _OPENCV

    INCLUDEPATH += $$LINUX_INCLUDE_DIR
    LIBS  += -L$$LINUX_LIB_DIR -l$$CMIRIS_LINUX_LIB_NAME -fopenmp
    LIBS  += -l$$CMICOLOR_LINUX_LIB_NAME
    LIBS  += -l$$EMA_LINUX_LIB_NAME

    CONFIG(debug, debug|release) {
        #DEFINES += QT_NO_DEBUG_OUTPUT
        #DEFINES += QT_NO_WARNING_OUTPUT
        irisclient.files = $${TARGET}
        irisclient.path = /usr/local/bin
    } else {
        DEFINES += QT_NO_DEBUG_OUTPUT
        DEFINES += QT_NO_WARNING_OUTPUT
        irisclient.files = $${TARGET}
        irisclient.path = /usr/local/bin
        irisclient.extra = strip $${TARGET};
    }
    INSTALLS += IrisClient
}
