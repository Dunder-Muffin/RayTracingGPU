cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME ${PROJECT}-${BUILD_TYPE}.exe)
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(PARENT_DIR ${CMAKE_SOURCE_DIR}/../)
set(COMMON_ROOT ${PARENT_DIR}/Common/Sources)
set(PROJECT_ROOT ${PARENT_DIR}/Projects/${PROJECT}/Sources)

include_directories(${COMMON_ROOT})
include_directories(${PROJECT_ROOT})
message("common root is ${COMMON_ROOT}")
message("project root is ${PROJECT_ROOT}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -O0 -DDEBUG -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")
    message("DebugBuild")
elseif(BUILD_TYPE STREQUAL "dev")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    message("DevelopBuild")
elseif(BUILD_TYPE STREQUAL "rel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3")
    message("ReleaseBuild")
endif()

file(GLOB_RECURSE COMMON_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${COMMON_ROOT}/*.cpp")
file(GLOB_RECURSE COMMON_C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${COMMON_ROOT}/*.c")
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${PROJECT_ROOT}/*.cpp")
file(GLOB_RECURSE PROJECT_C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${PROJECT_ROOT}/*.c")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${COMMON_SOURCES} ${COMMON_C_SOURCES} ${PROJECT_C_SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${CMAKE_DL_LIBS})



