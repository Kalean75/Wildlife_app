QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    entities.cpp \
    main.cpp \
    physics.cpp \
    quiz.cpp \
    renderer.cpp \
    terrain.cpp \
    view.cpp

HEADERS += \
    entities.h \
    physics.h \
    quiz.h \
    renderer.h \
    terrain.h \
    view.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/lib/box2d/build/bin/ -lbox2d

INCLUDEPATH += $$PWD/lib/box2d/include
DEPENDPATH += $$PWD/lib/box2d/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/box2d/build/bin/box2d.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/box2d/build/bin/libbox2d.a

RESOURCES += \
    res.qrc
