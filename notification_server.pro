######################################################################
# notification_server.pro file
######################################################################

TEMPLATE = app
TARGET = notification_server
INCLUDEPATH += .

CONFIG += c++17

QT -= gui
QT += core network sql concurrent


# Set build directory based on the build type
CONFIG(debug, debug|release) {
    message(debug build)
    BUILD_PATH = .build/debug/
} else {
    message(release build)
    BUILD_PATH = .build/release/
}

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# === Headers and source files ===
include(common.pri)

# === Main ===
SOURCES += src/main.cpp


