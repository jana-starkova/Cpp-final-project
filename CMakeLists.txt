cmake_minimum_required(VERSION 3.0)

project(scary-stuff LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_USE_QTMULTIMEDIA TRUE)
set(QT_USE_QTMULTIMEDIAWIDGETS TRUE)

# Declare dependencies
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort Network Widgets Multimedia MultimediaWidgets PrintSupport)
find_package(OpenCV REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)


# QT5_USE_MODULES (${EXECUTABLE_NAME} Multimedia MultimediaWidgets)

# Qt Flags
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Instruct CMake to get the resources
set(CMAKE_AUTORCC ON)
set(FACE_DETECTION_CONFIGURATION
 "${PROJECT_SOURCE_DIR}/assets/deploy.prototxt")
set(FACE_DETECTION_WEIGHTS
 "${PROJECT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")


add_executable(${PROJECT_NAME}
    main.cpp
    src/widget.cpp
    src/widget.h
    src/widget.ui
    src/watershedsegmenter.h
    )

qt5_use_modules(${PROJECT_NAME} multimedia multimediawidgets)


# Set includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )


# Link libs
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::MultimediaWidgets Qt5::PrintSupport ${OpenCV_LIBS} Qt5::Core Qt5::Multimedia PRIVATE Threads::Threads)
else(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::MultimediaWidgets Qt5::PrintSupport ${OpenCV_LIBS} Qt5::Core Qt5::Multimedia)
endif(UNIX)

# Set compile options, enable warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
    $<$<CXX_COMPILER_ID:MSVC>: /W3>
    )
target_compile_definitions(${PROJECT_NAME} PRIVATE  FACE_DETECTION_CONFIGURATION="${FACE_DETECTION_CONFIGURATION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE  FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")

# Copy necessary files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/butterflies.gif
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/ghost.gif
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/spider.gif
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
