#pragma once

#include <cstdlib>  // for exit
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

namespace io::utils {

/**
 * @brief ファイルストリームが開いているかを確認する
 *
 * @tparam Stream ファイルストリームの型 (例: std::ifstream, std::ofstream)
 * @param file 確認するファイルストリーム
 * @param path ファイルのパス
 * @return true ファイルストリームが開いている場合
 * @return false ファイルストリームが開いていない場合
 */
template <typename Stream>
bool checkFileOpen(Stream& file, const std::string& path) {
    const std::string mode = [] {
        if constexpr (std::is_same_v<Stream, std::ifstream>) {
            return "read";
        } else if constexpr (std::is_same_v<Stream, std::ofstream>) {
            return "write";
        } else {
            return "unknown";
        }
    }();

    if (!file) {
        std::cerr << "Failed to open " << mode << ": " << path << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief エラーメッセージを出力し、プログラムを終了する
 *
 * @param message 出力するエラーメッセージ
 * @note この関数はプログラムを終了します。
 */
inline void printErrorAndExit(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    std::exit(1);
}

/**
 * @brief ログメッセージを出力する
 *
 * @param message 出力するログメッセージ
 */
inline void logMessage(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
}

}  // namespace io::utils
