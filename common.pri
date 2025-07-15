# ==============================================
# =============  Crypto includes  ==============
# ==============================================

# Автоматическая поддержка QCA для Qt5 и Qt6
qtHaveModule(core): QT_MAJOR_VERSION = $$QT_MAJOR_VERSION
equals(QT_MAJOR_VERSION, 6) {
    message(using Qt6)
    CONFIG += link_pkgconfig
    PKGCONFIG += qca-qt6
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCrypto
} else:equals(QT_MAJOR_VERSION, 5) {
    message(using Qt5)
    # CONFIG += link_pkgconfig
    # PKGCONFIG += qca-qt5
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCrypto
    LIBS += -lqca-qt5
}

# ==============================================
# ================  buildpaths  ================
# ==============================================

OBJECTS_DIR = $$BUILD_PATH/objects
MOC_DIR = $$BUILD_PATH/moc
UI_DIR = $$BUILD_PATH/ui
RCC_DIR = $$BUILD_PATH/rcc

# ==============================================
# ===============  src includes  ===============
# ==============================================

PROJECT_ROOT = $$PWD

# === Ethernet ===
HEADERS += $${PROJECT_ROOT}/src/ethernet/ApplicationLayer.h \
           $${PROJECT_ROOT}/src/ethernet/HTTPPacket.h \
           $${PROJECT_ROOT}/src/ethernet/TransportLayer.h
SOURCES += $${PROJECT_ROOT}/src/ethernet/ApplicationLayer.cpp \
           $${PROJECT_ROOT}/src/ethernet/HTTPPacket.cpp \
           $${PROJECT_ROOT}/src/ethernet/TransportLayer.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/ethernet

# === Settings ===
HEADERS += $${PROJECT_ROOT}/src/settings/Settings.h
SOURCES += $${PROJECT_ROOT}/src/settings/Settings.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/settings

# === Logic ===
HEADERS += $${PROJECT_ROOT}/src/logic/MainController.h \
           $${PROJECT_ROOT}/src/logic/RequestRouter.h
SOURCES += $${PROJECT_ROOT}/src/logic/MainController.cpp \
           $${PROJECT_ROOT}/src/logic/RequestRouter.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/logic

# === Message Processor ===
HEADERS += $${PROJECT_ROOT}/src/logic/message_processor/BaseMessage.h \
           $${PROJECT_ROOT}/src/logic/message_processor/MessageFactory.h
SOURCES += $${PROJECT_ROOT}/src/logic/message_processor/MessageFactory.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/logic/message_processor

# === Message Processor: messages ===
HEADERS += $${PROJECT_ROOT}/src/logic/message_processor/messages/MessagePing.h
SOURCES += $${PROJECT_ROOT}/src/logic/message_processor/messages/MessagePing.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/logic/message_processor/messages



# === Common ===
HEADERS += $${PROJECT_ROOT}/src/common/Defines.h \
            $${PROJECT_ROOT}/src/common/Templates.h \
            $${PROJECT_ROOT}/src/common/GUID.h
INCLUDEPATH += $${PROJECT_ROOT}/src/common



# === Cryptor ===
HEADERS += $${PROJECT_ROOT}/src/crypto/Crypto.h \
           $${PROJECT_ROOT}/src/crypto/KeyWorker.h
SOURCES += $${PROJECT_ROOT}/src/crypto/Crypto.cpp \
           $${PROJECT_ROOT}/src/crypto/KeyWorker.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/crypto



# === Database ===
HEADERS += $${PROJECT_ROOT}/src/database/Connection.h \
           $${PROJECT_ROOT}/src/database/Database.h \
           $${PROJECT_ROOT}/src/database/Contexts.h
SOURCES += $${PROJECT_ROOT}/src/database/Connection.cpp \
           $${PROJECT_ROOT}/src/database/Database.cpp
INCLUDEPATH += $${PROJECT_ROOT}/src/database

# === Database Contexts ===
# -- Devices --
HEADERS += $${PROJECT_ROOT}/src/database/contexts/devices/BaseContextDevice.h \
           $${PROJECT_ROOT}/src/database/contexts/devices/ContextAddDevice.h
INCLUDEPATH += $${PROJECT_ROOT}/src/database/contexts/devices
# -- Connection Keys --
HEADERS += $${PROJECT_ROOT}/src/database/contexts/connection_keys/BaseContextConnectionKey.h \
           $${PROJECT_ROOT}/src/database/contexts/connection_keys/ContextCreateKey.h \
           $${PROJECT_ROOT}/src/database/contexts/connection_keys/ContextGetPrivateKey.h \
           $${PROJECT_ROOT}/src/database/contexts/connection_keys/ContextGetPublicClientKeyByGUID.h \
           $${PROJECT_ROOT}/src/database/contexts/connection_keys/ContextSetPublicKeyForDevice.h
INCLUDEPATH += $${PROJECT_ROOT}/src/database/contexts/connection_keys
# -- Users --
HEADERS += $${PROJECT_ROOT}/src/database/contexts/users/BaseContextUser.h \
           $${PROJECT_ROOT}/src/database/contexts/users/ContextAddUser.h
INCLUDEPATH += $${PROJECT_ROOT}/src/database/contexts/users
# -- Guids --
HEADERS += $${PROJECT_ROOT}/src/database/contexts/guids/BaseContextGuids.h
INCLUDEPATH += $${PROJECT_ROOT}/src/database/contexts/guids
# -- Model --
HEADERS += $${PROJECT_ROOT}/src/database/contexts/model/BaseContextModelVersion.h \
           $${PROJECT_ROOT}/src/database/contexts/model/ContextCheckModelVersion.h
INCLUDEPATH += $${PROJECT_ROOT}/src/database/contexts/model