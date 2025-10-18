#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include "core/constants.hpp"
#include "nlohmann/json.hpp"

namespace io::formats::json {

using json = nlohmann::json;

/**
 * @brief 出力設定を表す構造体
 *
 * 出力フォーマットやディレクトリ情報を管理します。
 */
struct OutputConfig {
    std::vector<std::string> formats;  ///< 出力フォーマットのリスト
    std::string directory;             ///< 出力先ディレクトリ
};

/**
 * @brief 設定情報を表す構造体
 *
 * アルゴリズムやモード、禁止語リストなどの設定を管理します。
 */
struct Config {
    std::string mode;            ///< 実行モード (例: "custom", "all-patterns")
    std::string algorithm;       ///< 使用するアルゴリズム (例: "DeBruijn", "Beal")
    bool sink_less;              ///< Sink-lessモードの有効/無効
    bool minimize;               ///< 最小化オプションの有効/無効
    unsigned int alphabet_size;  ///< 使用するアルファベットのサイズ
    unsigned int period;         ///< 周期の長さ
    std::optional<unsigned int> forbidden_word_length;  ///< 禁止語の長さ (オプション)
    std::optional<std::vector<std::pair<std::string, unsigned int>>>
        forbidden_words;  ///< 禁止語リスト (オプション)
    std::optional<std::vector<unsigned int>>
        forbidden_per_position;  ///< 位置ごとの禁止語数 (オプション)
    OutputConfig output;         ///< 出力設定

    /**
     * @brief 設定内容を検証する
     *
     * @throws std::invalid_argument モードに応じた必須フィールドが不足している場合
     */
    void validate() const;

    /**
     * @brief DeBruijnアルゴリズム用に禁止語リストの単語長を揃える
     *
     * @throws std::invalid_argument forbidden_wordsが未設定の場合
     */
    void formatForDeBruijn();

   private:
    /**
     * @brief 単語を指定長さまで拡張する
     *
     * @param words 拡張対象の単語リスト
     * @param targetLen 目標の単語長
     * @param alphabet 使用するアルファベット
     */
    void extendWords(std::vector<std::pair<std::string, unsigned int>>& words,
                     unsigned int targetLen, const std::string& alphabet) const;
};

/**
 * @brief JSONオブジェクトからOutputConfig構造体を生成する
 *
 * @param j 入力JSONオブジェクト
 * @param o 出力用のOutputConfig構造体
 */
void from_json(const json& j, OutputConfig& o);

/**
 * @brief JSONオブジェクトからConfig構造体を生成する
 *
 * @param j 入力JSONオブジェクト
 * @param c 出力用のConfig構造体
 */
void from_json(const json& j, Config& c);

}  // namespace io::formats::json
