cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

SET(TESTING TRUE CACHE BOOL "Some user-specified option")

if(WEB)
SET(GCC_COVERAGE_COMPILE_FLAGS "-g -s USE_SDL=2")
else()
SET(GCC_COVERAGE_COMPILE_FLAGS "-g")
endif()

SET(GCC_COVERAGE_LINK_FLAGS    "")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

project(CHIP8)
if(WEB)
set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
endif()



file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(SYSTEM include)
set(PROG "CHIP8")
include(GoogleTest)

if(TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    file(GLOB Tracer_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    list(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable( ${PROG}  ${Tracer_tests} ${SOURCE})
    target_link_libraries(${PROG} GTest::gtest_main)
    target_link_libraries(${PROG} ${SDL2_LIBRARIES})
    gtest_discover_tests(${PROG})
else()
    add_executable(${PROG} ${SOURCE})
    target_link_libraries(${PROG} ${SDL2_LIBRARIES})
endif()
