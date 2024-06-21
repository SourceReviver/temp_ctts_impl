cmake_minimum_required(VERSION 3.28)
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

add_compile_definitions(
        QT_NO_CAST_TO_ASCII
        QT_STRICT_ITERATORS
)

add_executable(ctts
        ctts.cpp
        ctts_main_window.h
        ctts_main_window.cpp
        ctts_current_service.h
        ctts_config_window.cpp
        ctts_config_window.h
        tts_service.h
        azure.h
        azure.cpp
        global_network_access_manager.h
        dummy.h
)

target_link_libraries(ctts PRIVATE
        Qt6::Widgets
        Qt6::Network
        Qt6::Multimedia
        fmt::fmt
)

