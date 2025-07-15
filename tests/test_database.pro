TEMPLATE = app
CONFIG += console testcase c++17
QT += testlib sql core network

TARGET = test_database

BUILD_PATH = .build

include(../common.pri)

# === Test files ===
SOURCES += \
#    tests.cpp \
    database/TestDatabase.cpp



