cmake_minimum_required(VERSION 3.17)
project(PFT-tools)

# -------------------------
# CMP0169: FetchContent_Populate 非推奨警告の回避
# -------------------------
# Eigen や Spectra をヘッダオンリーで展開するために
# FetchContent_Populate を使用しているが、将来的には
# FetchContent_MakeAvailable に置き換える必要がある
cmake_policy(SET CMP0169 OLD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# -------------------------
# Google Test
# -------------------------
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# -------------------------
# nlohmann-json
# -------------------------
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# -------------------------
# Eigen
# -------------------------
FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 5.0.0
)
FetchContent_Populate(Eigen)

# -------------------------
# Spectra
# -------------------------
FetchContent_Declare(
  Spectra
  GIT_REPOSITORY https://github.com/yixuan/spectra.git
  GIT_TAG v1.2.0
)
FetchContent_Populate(Spectra)

# -------------------------
# CLI11
# -------------------------
FetchContent_Declare(
  cli11_proj
  QUIET
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(cli11_proj)

# -------------------------
# プロジェクトソース
# -------------------------
enable_testing()
file(GLOB_RECURSE SRC_SOURCES src/**/*.cpp)

add_executable(PFT-tools src/main.cpp ${SRC_SOURCES})

# 共通インクルードディレクトリ
file(GLOB COMMON_INCLUDE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*)
list(TRANSFORM COMMON_INCLUDE_DIRS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/")

target_include_directories(PFT-tools PRIVATE
  ${COMMON_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${eigen_SOURCE_DIR}
  ${spectra_SOURCE_DIR}/include
  ${cli11_proj_SOURCE_DIR}/include
  ${dotenv_cpp_SOURCE_DIR}/include
)

# ライブラリリンク
target_link_libraries(PFT-tools PRIVATE nlohmann_json::nlohmann_json CLI11::CLI11)

# -------------------------
# テスト設定
# -------------------------
set(TEST_LIBRARIES gtest_main nlohmann_json::nlohmann_json)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE} ${SRC_SOURCES})
  target_include_directories(${TEST_NAME} PRIVATE
    ${COMMON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${eigen_SOURCE_DIR}
    ${spectra_SOURCE_DIR}/include
  )
  target_link_libraries(${TEST_NAME} PRIVATE ${TEST_LIBRARIES})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# -------------------------
# ビルド後に自動テスト
# -------------------------
add_custom_target(run_tests ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS PFT-tools
)

# Python仮想環境のセットアップ
set(PYTHON_VENV_PATH "${CMAKE_BINARY_DIR}/venv")
set(PYTHON_EXECUTABLE "${PYTHON_VENV_PATH}/bin/python3")

add_custom_target(setup_venv ALL
  COMMAND python3 -m venv ${PYTHON_VENV_PATH}
  COMMAND ${PYTHON_EXECUTABLE} -m pip install --upgrade pip --quiet
  COMMAND ${PYTHON_EXECUTABLE} -m pip install dot2tex --quiet
)

# 仮想環境のパスをコンパイル時に渡す
target_compile_definitions(PFT-tools PRIVATE PYTHON_VENV_PATH="${PYTHON_VENV_PATH}")
