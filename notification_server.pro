######################################################################
# notification_server.pro file
######################################################################

TEMPLATE = app
TARGET = notification_server
INCLUDEPATH += .

QT -= gui
QT += core network


CONFIG += debug
CONFIG(debug, debug|release) {
    DESTDIR = build/debug/
} else {
    DESTDIR = build/release/
}

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/ethernet/ApplicationLayer.h \
           src/ethernet/HTTPPacket.h \
           src/ethernet/TransportLayer.h
SOURCES += src/main.cpp \
           src/ethernet/ApplicationLayer.cpp \
           src/ethernet/HTTPPacket.cpp \
           src/ethernet/TransportLayer.cpp
