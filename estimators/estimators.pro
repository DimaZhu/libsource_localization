#-------------------------------------------------
#
# Project created by QtCreator 2017-10-12T10:30:55
#
#-------------------------------------------------

QT       -= gui

TARGET = estimators
TEMPLATE = lib

DEFINES += ESTIMATORS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        estimators.cpp \
    pelengator.cpp

HEADERS += \
        estimators.h \
        estimators_global.h \   
    pelengator.h

INCLUDEPATH += $$PWD/../dlib-19.7/\
                              $$PWD/../likelihoods/\
                              $$PWD/../datatypes/\
                              $$PWD/../pydatatypes/\
                              $$PWD/../grid_synth/\
                              $$PWD/../\
                             /usr/include/c++/5.4.0/\
                             /usr/include/\
                             /usr/include/python2.7/



LIBS += -L$$PWD/../build_likelihoods/-llikelihoods\
              -L$$PWD/../-lpydatatypes\
              -L$$PWD/../-lgrid_synth\
              -L$$PWD/../build_datatypes/-ldatatypes\

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CXXFLAGS += -Wmissing-field-initializers

