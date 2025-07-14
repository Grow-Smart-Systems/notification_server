######################################################################
# notification_server.pro file
######################################################################

TEMPLATE = app
TARGET = notification_server
INCLUDEPATH += .

CONFIG += c++17

QT -= gui
QT += core network sql concurrent

# Автоматическая поддержка QCA для Qt5 и Qt6
qtHaveModule(core): QT_MAJOR_VERSION = $$QT_MAJOR_VERSION
qcontains(QT_MAJOR_VERSION, 6) {
    message(Qt6 detected)
    CONFIG += link_pkgconfig
    PKGCONFIG += qca-qt6
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCrypto
} else:contains(QT_MAJOR_VERSION, 5) {
    message(Qt5 detected)
    # CONFIG += link_pkgconfig
    # PKGCONFIG += qca-qt5
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCrypto
    LIBS += -lqca-qt5
}

# Set build directory based on the build type
CONFIG(debug, debug|release) {
    message(debug build)
    BUILD_PATH = build/debug/
} else {
    message(release build)
    BUILD_PATH = build/release/
}

#DESTDIR = $$BUILD_PATH
OBJECTS_DIR = $$BUILD_PATH/objects
MOC_DIR = $$BUILD_PATH/moc
UI_DIR = $$BUILD_PATH/ui
RCC_DIR = $$BUILD_PATH/rcc

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



# === Ethernet ===
HEADERS += src/ethernet/ApplicationLayer.h \
           src/ethernet/HTTPPacket.h \
           src/ethernet/TransportLayer.h
SOURCES += src/ethernet/ApplicationLayer.cpp \
           src/ethernet/HTTPPacket.cpp \
           src/ethernet/TransportLayer.cpp
INCLUDEPATH += src/ethernet



# === Settings ===
HEADERS += src/settings/Settings.h
SOURCES += src/settings/Settings.cpp
INCLUDEPATH += src/settings



# === Logic ===
HEADERS += src/logic/MainController.h \
           src/logic/RequestRouter.h
SOURCES += src/logic/MainController.cpp \
           src/logic/RequestRouter.cpp
INCLUDEPATH += src/logic



# === Message Processor ===
HEADERS += src/logic/message_processor/BaseMessage.h \
           src/logic/message_processor/MessageFactory.h
SOURCES += src/logic/message_processor/MessageFactory.cpp
INCLUDEPATH += src/logic/message_processor

# === Message Processor: messages ===
HEADERS += src/logic/message_processor/messages/MessagePing.h
SOURCES += src/logic/message_processor/messages/MessagePing.cpp
INCLUDEPATH += src/logic/message_processor/messages



# === Common ===
HEADERS += src/common/Defines.h \
            src/common/Templates.h \
            src/common/GUID.h
INCLUDEPATH += src/common



# === Cryptor ===
HEADERS += src/crypto/Crypto.h \
           src/crypto/KeyWorker.h
SOURCES += src/crypto/Crypto.cpp \
           src/crypto/KeyWorker.cpp
INCLUDEPATH += src/crypto



# === Database ===
HEADERS += src/database/Connection.h \
           src/database/Database.h \
           src/database/Contexts.h
SOURCES += src/database/Connection.cpp \
           src/database/Database.cpp
INCLUDEPATH += src/database

# === Database Contexts ===
# -- Devices --
HEADERS += src/database/contexts/devices/BaseContextDevice.h \
           src/database/contexts/devices/ContextAddDevice.h
INCLUDEPATH += src/database/contexts/devices
# -- Connection Keys --
HEADERS += src/database/contexts/connection_keys/BaseContextConnectionKey.h \
           src/database/contexts/connection_keys/ContextCreateKey.h \
           src/database/contexts/connection_keys/ContextSetDeviceIDForKey.h \
           src/database/contexts/connection_keys/ContextGetPrivateKey.h \
           src/database/contexts/connection_keys/ContextGetPublicClientKeyByGUID.h
INCLUDEPATH += src/database/contexts/connection_keys
# -- Users --
HEADERS += src/database/contexts/users/BaseContextUser.h \
           src/database/contexts/users/ContextAddUser.h
INCLUDEPATH += src/database/contexts/users
# -- Guids --
HEADERS += src/database/contexts/guids/BaseСontextGuids.h
INCLUDEPATH += src/database/contexts/guids
# -- Model --
HEADERS += src/database/contexts/model/BaseContextModelVersion.h \
           src/database/contexts/model/ContextCheckModelVersion.h
INCLUDEPATH += src/database/contexts/model



# === Main ===
SOURCES += src/main.cpp


