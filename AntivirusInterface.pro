QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    handler.cpp \
    main.cpp \
    core/fixer.cpp \
    core/scanner.cpp \
    global/helper.cpp \
    ui/infowidget.cpp \
    ui/mainwindow.cpp \
    ui/scanneddatamodel.cpp \
    ui/scanresultform.cpp

HEADERS += \
    handler.h \
    core/fixer.h \
    core/scanner.h \
    global/helper.h \
    ui/infowidget.h \
    ui/mainwindow.h \
    ui/scanneddatamodel.h \
    ui/scanresultform.h

FORMS += \
    ui/mainwindow.ui \
    ui/scanresultform.ui

TRANSLATIONS += \
    AntivirusInterface_en_US.ts \
    AntivirusInterface_ru_RU.ts

TEMPLATE = app
TARGET = CoffeAntivirus
## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    ui/resources.qrc

RC_ICONS = icons/Ico.ico

RC_FILE = icon.rc
