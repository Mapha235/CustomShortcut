# include_directories(${PROJECT_SOURCE_DIR}/GamepadInput/include)
set(TARGET CustomShortcut)

# needed for some MSVC installations
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
# set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

file(GLOB HEADER
    "*.hpp"
    )
file(GLOB SRC
    "*.cpp"
    )
    
# set(HEADER_LIST "${GamepadInput_SOURCE_DIR}/include/Gamepad.hpp")

# source_group(
#   TREE "${PROJECT_SOURCE_DIR}/include"
#   PREFIX "Header Files"
#   FILES ${HEADER_LIST})

# add_library(
#     classes
#     Gamepad.cpp
#     main.cpp
# )



# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# run moc automatically
set(CMAKE_AUTOMOC ON)
# if you're using Widgets and editing .ui forms in Designer
set(CMAKE_AUTOUIC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# find_library(XInput xinput)
# if(${XInput_FOUND})
# message("Found Xinput " ${Qt5_VERSION})
# else()
#     message("Couldn't find xinput")
# endif()

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html) you're using in your application
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

# your source files
# set(SOURCES
#     mainwindow.cpp
#     gamepad.cpp
#     main.cpp
# )

# specify resource file names here, required for QML
qt5_add_resources(qrc qml.qrc)
# name of the .exe file, window flag and the list of things to compile
# add_executable(${CMAKE_PROJECT_NAME} WIN32 ${sources})

# also tell CMake to link with the libraries from the line 14
# otherwise it'll think that you wanted to find those just for fun and won't link them
add_executable(
    # GamepadInput
    ${TARGET} 
    WIN32
    ${HEADER}
    ${SRC}
    )

# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_library(xinput 
    xinput.h
    ${SRC}
    )

target_link_libraries(
    ${TARGET}
    xinput.lib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

# target_link_libraries(GamepadInput)
# target_include_directories(include PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
