cmake_minimum_required(VERSION 3.15)

project(tetris_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 폴더 설정
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# include 디렉토리 추가 (모든 .hpp 인식)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

# Raylib FetchContent 설정
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)

FetchContent_MakeAvailable(raylib)

# 실행 파일 정의
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Raylib 링크
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
