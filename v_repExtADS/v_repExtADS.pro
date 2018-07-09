QT -= core
QT -= gui

TARGET = v_repExtADS
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += QT_COMPIL
CONFIG += shared
INCLUDEPATH += "$$PWD/include"

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
    DEFINES += WIN_VREP
    LIBS+= "ws2_32.lib"
}

macx {
    DEFINES += MAC_VREP
}

unix:!macx {
    DEFINES += LIN_VREP
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

HEADERS += \
    include/scriptFunctionData.h \
    include/scriptFunctionDataItem.h \
    include/v_repLib.h \
    v_repExtADS.h \
    AdsLib/AdsDef.h \
    AdsLib/AdsLib.h \
    AdsLib/AdsNotification.h \
    AdsLib/AmsConnection.h \
    AdsLib/AmsHeader.h \
    AdsLib/AmsPort.h \
    AdsLib/AmsRouter.h \
    AdsLib/Frame.h \
    AdsLib/Log.h \
    AdsLib/NotificationDispatcher.h \
    AdsLib/RingBuffer.h \
    AdsLib/Router.h \
    AdsLib/Sockets.h \
    AdsLib/wrap_endian.h \
    AdsLib/wrap_socket.h

SOURCES += \
     common/scriptFunctionData.cpp \
     common/scriptFunctionDataItem.cpp \
     common/v_repLib.cpp \
    v_repExtADS.cpp \
    AdsLib/AdsDef.cpp \
    AdsLib/AdsLib.cpp \
    AdsLib/AmsConnection.cpp \
    AdsLib/AmsPort.cpp \
    AdsLib/AmsRouter.cpp \
    AdsLib/Frame.cpp \
    AdsLib/Log.cpp \
    AdsLib/NotificationDispatcher.cpp \
    AdsLib/Sockets.cpp


