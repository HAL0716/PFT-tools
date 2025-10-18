#pragma once

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

}  // namespace io::utils
