#include "gtest/gtest.h"
#include "path/utils.hpp"

// PathUtils クラスのテスト
TEST(PathUtilsTest, extractPath) {
    std::string filePath = "/home/user/data/file.csv";

    // ディレクトリのみ取得
    EXPECT_EQ(path::utils::extractPath(filePath, 0, true, false, false), "/home/user/data");
    EXPECT_EQ(path::utils::extractPath(filePath, 1, true, false, false), "/home/user");
    EXPECT_EQ(path::utils::extractPath(filePath, 2, true, false, false), "/home");
    EXPECT_EQ(path::utils::extractPath(filePath, 3, true, false, false), "/");
    EXPECT_THROW(path::utils::extractPath(filePath, 4, true, false, false), std::out_of_range);

    // ファイル名のみ取得(拡張子あり・なし)
    EXPECT_EQ(path::utils::extractPath(filePath, 0, false, true, true), "file.csv");
    EXPECT_EQ(path::utils::extractPath(filePath, 0, false, true, false), "file");

    // フルパス取得(拡張子あり・なし)
    EXPECT_EQ(path::utils::extractPath(filePath, 0, true, true, true), "/home/user/data/file.csv");
    EXPECT_EQ(path::utils::extractPath(filePath, 0, true, true, false), "/home/user/data/file");

    // 拡張子だけを取得
    EXPECT_EQ(path::utils::extractPath(filePath, 0, false, false, true), ".csv");

    // 何も取得しない場合
    EXPECT_EQ(path::utils::extractPath(filePath, 0, false, false, false), "");
}
