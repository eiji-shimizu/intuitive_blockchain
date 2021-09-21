cmake_minimum_required(VERSION 3.19.6)
project(intuitive_blockchain CXX)

set(INTUITIVE_BLOCKCHAIN_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# Releaseビルド時 cmake -DCMAKE_BUILD_TYPE=Release ..
# テストビルド時 cmake -DCMAKE_BUILD_TYPE=Debug ..
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message("---------------------- Release Build")
    set(COMPILE_OPTIONS /W4 /EHsc /MD /utf-8 /std:c++17)
    set(COMPILE_DEFINITIONS NDEBUG)
else()
    message("---------------------- Not Release Build")
    set(COMPILE_OPTIONS /W4 /EHa /MDd /utf-8 /std:c++17)
endif()

# サブディレクトリを登録
add_subdirectory(src)
