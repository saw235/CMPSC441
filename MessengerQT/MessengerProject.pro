TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    guihandler.cpp

RESOURCES += qml.qrc

LIBS += -pthread -lssl -lcrypto

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    guihandler.h
