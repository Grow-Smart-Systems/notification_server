######################################################################
# notification_server.pro file
######################################################################

TEMPLATE = app
TARGET = notification_server
INCLUDEPATH += .

QT -= gui
QT += core network

# Set build directory based on the build type
CONFIG(debug, debug|release) {
    BUILD_PATH = build/debug/
} else {
    BUILD_PATH = build/release/
}

#DESTDIR = $$BUILD_PATH
OBJECTS_DIR = $$BUILD_PATH/objects
MOC_DIR = $$BUILD_PATH/moc
UI_DIR = $$BUILD_PATH/ui
RCC_DIR = $$BUILD_PATH/rcc

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/ethernet/ApplicationLayer.h \
           src/ethernet/HTTPPacket.h \
           src/ethernet/TransportLayer.h \
           src/settings/Settings.h

SOURCES += src/main.cpp \
           src/ethernet/ApplicationLayer.cpp \
           src/ethernet/HTTPPacket.cpp \
           src/ethernet/TransportLayer.cpp \
           src/settings/Settings.cpp
