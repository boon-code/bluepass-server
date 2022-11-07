cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(BluePassServer LANGUAGES CXX)

option(HAS_VERSION "The project includes versioning information" ON)
if(HAS_VERSION)
    add_subdirectory(modules/c_git_version)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5
    COMPONENTS Core Gui Widgets Bluetooth
    REQUIRED
)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(BluePassServer
    app_constants.h
    bluetooth_device_list_model.cpp
    bluetooth_device_list_model.h
    bluetooth_service.cpp
    bluetooth_service.h
    choose_adapter_dialog.cpp
    choose_adapter_dialog.h
    dashboard.cpp
    dashboard.h
    main.cpp
    main_app.cpp
    main_app.h
    settings.cpp
    settings.h
    settings_type.cpp
    settings_type.h
    bpserver.qrc
    choose_adapter_dialog.ui
    dashboard.ui
)

target_include_directories(BluePassServer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(BluePassServer PRIVATE
    -DQT_DEPRECATED_WARNINGS=1
    -DQT_DISABLE_DEPRECATED_BEFORE=0xFFFFFF
)

if(HAS_VERSION)
    target_compile_definitions(BluePassServer PRIVATE -DHAS_VERSION=1)
    target_link_libraries(BluePassServer c_git_version)
endif()

target_link_libraries(BluePassServer
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Bluetooth
)

set_property(TARGET BluePassServer PROPERTY CXX_STANDARD 11)
