cmake_minimum_required(VERSION 3.15)
project(TOUHOU_GAME)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    # 强制源码和执行文件使用 UTF-8
    add_compile_options("/utf-8")
    set(ENV{VSLANG} "1033")
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/SDL)

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

file(GLOB_RECURSE SOURCE_FILES 
    "${SRC_PATH}/*.cpp"
    "${SRC_PATH}/*.c"
)


file(GLOB_RECURSE HEADER_FILES
    "${SRC_PATH}/*.h"
)

include_directories(
    ${SRC_PATH}
    ${SRC_PATH}/Core
    ${SRC_PATH}/Entities
    ${SRC_PATH}/Entities/Type
    ${SRC_PATH}/Factories
    ${SRC_PATH}/Input
    ${SRC_PATH}/Interface
    ${SRC_PATH}/Managers
    ${SRC_PATH}/Render
    ${SRC_PATH}/Resources
    ${SRC_PATH}/Scenes
    ${SRC_PATH}/Scripting
    ${SRC_PATH}/Utils
    ${LIB_PATH}/EasyX/include
    ${LIB_PATH}/glm
)

link_directories(
    ${LIB_PATH}/EasyX/lib/VC2015/x64
)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(main PRIVATE EasyXa.lib)
target_include_directories(main PRIVATE ${LIB_PATH}/SDL/include)

if(WIN32)
    target_link_libraries(main PRIVATE SDL2::SDL2-static SDL2::SDL2main)
else()
    target_link_libraries(main PRIVATE SDL2::SDL2-static)
endif()
