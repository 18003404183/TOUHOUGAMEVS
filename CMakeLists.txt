cmake_minimum_required(VERSION 3.15)
project(TOUHOU_GAME)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    # 强制源码和执行文件使用 UTF-8
    add_compile_options("/utf-8")
    set(ENV{VSLANG} "1033")
endif()



set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)

set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(${LIB_PATH}/SDL)
add_subdirectory(${LIB_PATH}/SDL_image)

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

target_link_libraries(main PRIVATE 
    EasyXa.lib
    SDL2_image::SDL2_image-static
    )
target_include_directories(main PRIVATE ${LIB_PATH}/SDL/include)
target_include_directories(main PRIVATE ${LIB_PATH}/SDL_image/include)

if(WIN32)
    target_link_libraries(main PRIVATE SDL2::SDL2-static SDL2::SDL2main)
else()
    target_link_libraries(main PRIVATE SDL2::SDL2-static)
endif()

add_custom_command(TARGET main POST_BUILD
    # 2. 使用 CMake 内置命令复制整个文件夹
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:main>/resources"
)
