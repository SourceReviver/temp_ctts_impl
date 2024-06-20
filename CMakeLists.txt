cmake_minimum_required(VERSION 3.29)
project(cloud_tts)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt)
find_package(tomlplusplus)
find_package(Qt6 COMPONENTS
        Widgets
        Network
        Multimedia
        REQUIRED)

qt_standard_project_setup()


add_executable(ctts
        ctts.cpp
        ctts_main_window.h
        ctts_main_window.cpp
        service.h
        ctts_config_window.cpp
        ctts_config_window.h
        azure.h
        azure.cpp
        azure_config_file.h
        azure_config_widget.h
        azure_service.h
)

target_link_libraries(ctts PRIVATE
        Qt6::Widgets
        Qt6::Network
        Qt6::Multimedia
        fmt::fmt
)

