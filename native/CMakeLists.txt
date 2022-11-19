project(ClashNative)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_definitions(-DFMT_HEADER_ONLY -DCPPHTTPLIB_OPENSSL_SUPPORT)

find_package(OpenSSL REQUIRED)

find_package(Qt5 COMPONENTS Widgets Network)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})

include_directories(libs ../src)

file(GLOB CORE_SRCS ../src/*.h ../src/*.cpp)
file(GLOB NATIVE_SRCS *.h *.cpp *.ui *.qrc *.rc)

list(REMOVE_ITEM CORE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp)

add_executable(${PROJECT_NAME} ${CORE_SRCS} ${NATIVE_SRCS})
target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
target_link_libraries(${PROJECT_NAME} Qt::Widgets Qt::Network OpenSSL::SSL OpenSSL::Crypto)

