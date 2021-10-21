#-------------------------------------------------
#
# Project created by QtCreator 2021-10-13T00:04:37
#
#-------------------------------------------------

QT += core gui widgets bluetooth

TARGET = BluePassServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        bluetooth_device_list_model.cpp \
        bluetooth_service.cpp \
        choose_adapter_dialog.cpp \
        dashboard.cpp \
        main.cpp \
        main_app.cpp \
        settings.cpp \
        settings_type.cpp

HEADERS += \
        app_constants.h \
        bluetooth_device_list_model.h \
        bluetooth_service.h \
        choose_adapter_dialog.h \
        dashboard.h \
        main_app.h \
        settings.h \
        settings_type.h

FORMS += \
        choose_adapter_dialog.ui \
        dashboard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    bpserver.qrc

RC_ICONS = BluePassServer.ico

# Deployment:
# Copy following libraries:
# - C:\Qt\5.15.2\mingw81_32\bin\libgcc_s_dw2-1.dll
# - C:\Qt\5.15.2\mingw81_32\bin\libstdc++-6.dll
# - C:\Qt\5.15.2\mingw81_32\bin\libwinpthread-1.dll
#
# Run  C:\Qt\5.15.2\mingw81_32\windeployqt.exe <path-to-build-folder>\BluePassServer.exe

DISTFILES +=
