cmake_minimum_required(VERSION 3.5)

project(scary-stuff LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Declare dependencies
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort Network Widgets)

# Qt Flags
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Instruct CMake to get the resources
set(CMAKE_AUTORCC ON)


add_executable(${PROJECT_NAME}
    main.cpp
    src/ui/widget.cpp
    src/ui/widget.h
    src/ui/widget.ui
    )

# Set includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    )


# Link libs
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core PRIVATE Threads::Threads)
else(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core)
endif(UNIX)

# Set compile options, enable warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
    $<$<CXX_COMPILER_ID:MSVC>: /W3>
    )
