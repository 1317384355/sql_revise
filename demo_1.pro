QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#################### QxORM ############################
include(C:/libs/QxOrm-1.4.9-MinGW/QxOrm.pri)
INCLUDEPATH += C:/libs/QxOrm-1.4.9-MinGW/include
DEPENDPATH += C:/libs/QxOrm-1.4.9-MinGW/include

LIBS += -L"C:/libs/QxOrm-1.4.9-MinGW/lib"
CONFIG(debug, debug|release) {
TARGET = qxBlogd
LIBS += -l"QxOrmd"
} else {
TARGET = qxBlog
LIBS += -l"QxOrm"
} # CONFIG(debug, debug|release)

DEFINES += _BUILDING_QX_PRO
!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
PRECOMPILED_HEADER = ./include/db/command.h
}
#################### QxORM ############################

CONFIG(debug, debug|release) {
TARGET = qxBlogd
LIBS += -l"QxOrmd"
} else {
TARGET = qxBlog
LIBS += -l"QxOrm"
} # CONFIG(debug, debug|release)

SOURCES += \
    $$files($$PWD/src/*.cpp)\
    $$files($$PWD/src/widget/*.cpp)\
    $$files($$PWD/src/db/*.cpp)

HEADERS += \
    $$files($$PWD/include/widget/*.h)\
    $$files($$PWD/include/db/*.h)

FORMS += \
    $$files($$PWD/ui/*.ui)

#OpenCV environment
INCLUDEPATH += C:\libs\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\include
LIBS += C:\libs\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin/libopencv_*.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
