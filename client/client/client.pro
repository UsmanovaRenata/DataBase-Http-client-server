QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    loginwindow.cpp \
    main.cpp \
    newclientwindow.cpp \
    photodelegate.cpp \
    tabledelegate.cpp \
    tablefiltermodel.cpp \
    tablemodel.cpp \
    tablewindow.cpp

HEADERS += \
    client.h \
    loginwindow.h \
    newclientwindow.h \
    photodelegate.h \
    tabledelegate.h \
    tablefiltermodel.h \
    tablemodel.h \
    tablewindow.h

FORMS += \
    loginwindow.ui \
    newclientwindow.ui \
    tablewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
