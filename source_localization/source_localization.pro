#-------------------------------------------------
#
# Project created by QtCreator 2017-11-14T15:30:08
#
#-------------------------------------------------

QT       -= gui

TARGET = source_localization
TEMPLATE = lib
CONFIG += c++11

DEFINES += SOURCE_LOCALIZATION2_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/include/python3.5m/\
                            /usr/include/c++/5.4.0/\
                            /usr/include/\
                            /usr/lib64/python3.5/site-packages/numpy/core/include/\

LIBS += -L/usr/lib64 -lfftw3


SOURCES += \
    specframe.cpp \
    pelengator.cpp \
    lh_pel.cpp \
    dtypes.cpp \
    antenna.cpp \
    interpolator.cpp \
    lh.cpp \
    profiler.cpp \
    profile.cpp \
    profile_analizer.cpp \
    grid_pel.cpp

HEADERS += \
    antenna.h \
    lh_pel.h \
    pelengator.h \
    source_localization_global.h \
    specframe.h \
    interpolator.h \
    lh.h \
    profiler.h \
    profile.h \
    profile_analizer.h \
    grid_pel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
