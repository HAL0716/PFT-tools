cmake_minimum_required(VERSION 3.17)
project(PFT-tools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# Google Test を有効化
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# nlohmann-json を有効化
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

enable_testing()

# src ディレクトリ内のすべてのソースファイルを自動検出
file(GLOB_RECURSE SRC_SOURCES src/**/*.cpp)

# 実行可能ファイルの追加
add_executable(PFT-tools src/main.cpp ${SRC_SOURCES})

# インクルードディレクトリの設定
file(GLOB COMMON_INCLUDE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)
list(TRANSFORM COMMON_INCLUDE_DIRS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_include_directories(PFT-tools PRIVATE ${COMMON_INCLUDE_DIRS})

# nlohmann_json のリンク
target_link_libraries(PFT-tools PRIVATE nlohmann_json::nlohmann_json)

# テスト用ライブラリの設定
set(TEST_LIBRARIES gtest_main nlohmann_json::nlohmann_json)

# テストの自動登録
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE} ${SRC_SOURCES})
  target_include_directories(${TEST_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
  target_link_libraries(${TEST_NAME} PRIVATE ${TEST_LIBRARIES})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# 共通のインクルードディレクトリを追加
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# ビルド後に自動でテストを実行するターゲット
add_custom_target(run_tests ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS PFT-tools
)
