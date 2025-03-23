QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    settingswindow.cpp

HEADERS += \
    settingswindow.h

FORMS += \
    settingswindow.ui

TRANSLATIONS += \
    tablet-switch_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Deploy for Archlinux. This allows the qmake6 command to be as the following line
# DESTDIR="$pkgdir" PREFIX="/usr" qmake6 ../src/tablet-switch.pro;
# This will create a Makefile that is capable of installing (e.g. # make install) to the correct location via
# standard DESTDIR and PREFIX environment variables.
unix:!android: target.path = $$(DESTDIR)$$(PREFIX)/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    tray_icons.qrc

unix|win32: LIBS += -levdev
