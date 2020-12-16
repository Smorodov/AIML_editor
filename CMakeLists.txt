cmake_minimum_required(VERSION 3.1.0)
# define your project name. This will be available through "${PROJECT_NAME}" now
project(AIML_Editor)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc, uic and rcc automatically when needed.
# This will intelegently
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
# Executables fail to build with Qt 5 in the default configuration, becasue the -fPIE flag is needed. This adds it.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} )
# Set up compilers (Clang, MSVC, GCC) so that C++11 can be used. The optimization settings are optional. Make sure not to "strip" the executable (-s in linker options for Clang and GCC) if you want to debug your application! Else debug information won't be available.
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    #set up compiler flags for Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2") #support C++11 for std::, optimize
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2") #optimize
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    #check for G++ 5.4+
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE G++_VERSION)
    if (G++_VERSION VERSION_LESS 5.4)
            message(SEND_ERROR "You need at least G++ 5.4 to compile ${PROJECT_NAME}!")
    endif()
    #set up compiler flags for GCC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2") #support C++11 for std::, optimize
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2") #optimize
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") #multi-processor compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP") #multi-processor compilation
endif()

# Now define your header files, .ui files here:
set(MYPROJECT_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/codeeditor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/highlighter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/aimlparser.h
)

# Your source and .qrc files go here:
set(MYPROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/highlighter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/codeeditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/aimlparser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/qdarkstyle/style.qrc
)

# Optional: You can also add OS-specific header or source files:
if(WIN32)
    #LIST(APPEND MYPROJECT_SOURCES
    #    ${CMAKE_CURRENT_SOURCE_DIR}/src/stuff_win.cpp
    #)
elseif(UNIX AND NOT APPLE)
    #LIST(APPEND MYPROJECT_SOURCES
    #    ${CMAKE_CURRENT_SOURCE_DIR}/src/stuff_linux.cpp
    #)
endif()

# Optional: Or OS-specific libraries:
if(WIN32)
    #set(MYPROJECT_LIBRARIES
    #    winmm
    #)
elseif(UNIX AND NOT APPLE)
    #set(MYPROJECT_LIBRARIES
    #    pthread
    #)
endif()

# Set up your build directory
set(dir ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${dir} CACHE PATH "Build directory" FORCE)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
add_definitions(-DKGL_API -DKGL_STATIC)
# Set up your project build using include and source dirs and link libraries
include_directories(${Qt5Widgets_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} ${MYPROJECT_SOURCES} ${MYPROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Xml_LIBRARIES}
    ${Qt5Svg_LIBRARIES}
    ${Qt5PrintSupport_LIBRARIES}
                      ${MYPROJECT_LIBRARIES})

set_target_properties( ${PROJECT_NAME} PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
