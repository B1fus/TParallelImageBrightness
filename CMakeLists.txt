cmake_minimum_required(VERSION 3.26)
project(programm)

set (CMAKE_CXX_STANDARD 23)
set (CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
configure_file (paths.h.in ${CMAKE_CURRENT_SOURCE_DIR}/paths.h @ONLY)

find_package(OpenMP)

include_directories(lib/zlib)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/zlib)
add_subdirectory(lib/zlib ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib)
set(ZLIB_LIBRARY lib/zlib/zlib)

set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/zlib ${CMAKE_CURRENT_BINARY_DIR}/lib/zlib) # zlib

#libpng start
set(PNG_SHARED ON CACHE BOOL "" FORCE)
set(PNG_STATIC OFF CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL OFF CACHE BOOL "" FORCE)

include_directories(lib/libpng)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/libpng)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/libpng)
add_subdirectory(lib/libpng ${CMAKE_CURRENT_BINARY_DIR}/lib/libpng)
#libpng end

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

if(OpenMP_CXX_FOUND)
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp")
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()

target_link_libraries(${PROJECT_NAME} zlib)
target_link_libraries(${PROJECT_NAME} png)

#copy png16d.dll file to .exe directory
if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib/libpng/png16d.dll ${CMAKE_CURRENT_BINARY_DIR}/
        )
    else()
        add_custom_command(TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/lib/libpng/png16.dll ${CMAKE_CURRENT_BINARY_DIR}/
        )
    endif()
endif()