QT -= core
QT -= gui
TARGET = vrepZMQ
TEMPLATE = app
DEFINES -= UNICODE
CONFIG   += console
CONFIG   -= app_bundle



*-msvc* {
    QMAKE_CXXFLAGS += -O2
    QMAKE_CXXFLAGS += -W3
}
*-g++* {
    QMAKE_CXXFLAGS += -O3
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -Wno-empty-body
    QMAKE_CXXFLAGS += -Wno-write-strings

    QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
    QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
    QMAKE_CXXFLAGS += -Wno-narrowing

    QMAKE_CFLAGS += -O3
    QMAKE_CFLAGS += -Wall
    QMAKE_CFLAGS += -Wno-strict-aliasing
    QMAKE_CFLAGS += -Wno-unused-parameter
    QMAKE_CFLAGS += -Wno-unused-but-set-variable
    QMAKE_CFLAGS += -Wno-unused-local-typedefs
}


win32 {
    #boost库头文件和库文件目录
    BOOST_INCLUDE_DIRS="D:/RawDownLoads/boost_1_62"
    BOOST_LIB_DIRS = "$$BOOST_INCLUDE_DIRS/lib64-msvc-14.0"

    #B0的头文件目录和库
    B0_INCLUDE_DIRS="$$PWD/include"
    B0_LIBS = "$$PWD/lib/b0.lib"

    INCLUDEPATH += $$BOOST_INCLUDE_DIRS
    INCLUDEPATH += $$B0_INCLUDE_DIRS

    LIBS +=  $$B0_LIBS
    LIBS += -L$$BOOST_LIB_DIRS

}


unix:!macx {
    LIBS += -lboost_system
}


SOURCES += \
    main.cpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
