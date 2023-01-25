cmake_minimum_required(VERSION 3.16)

project(GUIFetch VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

exec_program(uname OUTPUT_VARIABLE SYSTEM_NAME)
set(SYSTEM_NAME "${SYSTEM_NAME}" CACHE INTERNAL "")

find_package(Qt6 6.4 COMPONENTS Quick Gui REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

if(SYSTEM_NAME STREQUAL "FreeBSD")
  link_directories(/usr/local/lib)
endif(SYSTEM_NAME STREQUAL "FreeBSD")

file(GLOB SRC
      src/*.cpp
      include/*.hpp
      include/*.h
)

qt6_add_resources(UI_RESOURCES ui/qml.qrc)

qt_add_executable(guifetch
    ${SRC}
    ${UI_RESOURCES}
)

qt_add_qml_module(guifetch
    URI GUIFetch
    VERSION 1.0
    QML_FILES
    ui/main.qml
)


if(SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries(guifetch PRIVATE Qt6::Gui Qt6::Quick)
endif(SYSTEM_NAME STREQUAL "Linux")

if(SYSTEM_NAME STREQUAL "FreeBSD")
  target_link_libraries(guifetch PUBLIC sysinfo PRIVATE Qt6::Gui Qt6::Quick)
endif(SYSTEM_NAME STREQUAL "FreeBSD")
