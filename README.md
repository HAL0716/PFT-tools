# PFT-tools

## 概要

PFT-toolsは，周期的禁止語によって定義されるシフト空間の生成，保存，解析を行うためのC++17 CLIツール．
このツールは，情報理論や符号理論における研究者やエンジニアを対象としており，以下の機能を提供．

- **グラフ生成**: De BruijnアルゴリズムやBéalアルゴリズムを使用
- **データ保存**: CSV形式や画像形式（PNG，PDF）での保存
- **解析**: 最大固有値の算出や許可系列の抽出

---

## アーキテクチャ概要

```plaintext
├── src/
│   ├── algorithm/       # アルゴリズム（Beal，DeBruijn，Moore）
│   ├── core/            # 基本的なデータ構造（Node，Edge，Graph）
│   ├── io/              # 入出力処理（JSON，CSV）
│   ├── utils/           # ユーティリティ関数
│   └── main.cpp         # エントリーポイント
├── scripts/             # dot2texなどの補助スクリプト
└── tests/               # Google Testによるユニットテスト
```

---

## インストール

### 必要なライブラリ

- C++17対応のコンパイラ（例: g++）
- CMake 3.17以上
- Python 3（オプション: dot2texパッケージ）

### セットアップ手順

```sh
# 必要なライブラリをインストール
sudo apt-get install cmake g++ python3

# リポジトリをクローン
git clone https://github.com/haruki/PFT-tools.git
cd PFT-tools

# ビルド
mkdir build && cd build
cmake .. && cmake --build .
```

---

## 使用例

### JSON設定ファイルを使用したグラフ生成

```sh
# JSON設定ファイルからグラフを生成
./pft-tools --input config/sample.json --png
```

### CSVファイルを使用した解析

```sh
# エッジリスト形式のCSVファイルから最大固有値を計算
./pft-tools --input data/edges.csv --format edges --max-eig

# 隣接行列形式のCSVファイルから最大固有値を計算
./pft-tools --input data/matrix.csv --format matrix --max-eig

# エッジリスト形式のCSVファイルから指定長さの許可系列を取得
./pft-tools --input data/edges.csv --format edges --sequences 5

# グラフをPDF形式で保存
./pft-tools --input data/edges.csv --format edges --pdf

# グラフをPNG形式で保存
./pft-tools --input data/edges.csv --format edges --png
```

### ディレクトリ内の複数CSVファイルを一括処理

```sh
# ディレクトリ内のすべてのエッジリスト形式CSVのファイルから最大固有値を計算
./pft-tools --input data/ --format edges --max-eig

# ディレクトリ内のすべての隣接行列形式CSVのファイルをPNG形式で保存
./pft-tools --input data/ --format matrix --png
```

---

## JSON設定ファイルのパラメータ

### `generation` セクション

- **`mode`**: グラフ生成モードを指定．
  - `custom`: ユーザー定義の禁止語を使用
  - `all-patterns`: すべてのパターンを生成
- **`algorithm`**: 使用するアルゴリズムを指定．
  - `Beal`: Béalアルゴリズム
  - `DeBruijn`: De Bruijnアルゴリズム
- **`opt_mode`**: 最適化モードを指定．
  - `none`: 通常モード
  - `sink_less`: シンクレスモード
  - `minimize`: 最小化モード
- **`alphabet`**: 使用するアルファベットのサイズ（例: 2なら{0, 1}）．
- **`period`**: 周期の長さ．
- **`forbidden`**: 禁止語のリストまたは長さを指定．
  - `nodes`: 禁止語のリスト（`custom`モードで使用）．
  - `length`: 禁止語の長さ（`all-patterns`モードで使用）．

### `output` セクション

- **`edge_list`**: エッジリスト形式で出力するかどうか（`true` または `false`）．
- **`png_file`**: PNG形式で出力するかどうか（`true` または `false`）．
- **`output_dir`**: 出力ファイルを保存するディレクトリ．

---

### JSON設定ファイル例

#### Customモード

```json
{
  "generation": {
    "mode": "custom",
    "algorithm": "Beal",
    "opt_mode": "none",
    "alphabet": 2,
    "period": 3,
    "forbidden": {
      "nodes": [
        {
          "word": "000",
          "phase": 0
        },
        {
          "word": "11",
          "phase": 1
        }
      ]
    }
  },
  "output": {
    "edge_list": true,
    "png_file": false,
    "output_dir": "results"
  }
}
```

#### All-Patternsモード

```json
{
  "generation": {
    "mode": "all-patterns",
    "algorithm": "DeBruijn",
    "opt_mode": "minimize",
    "alphabet": 3,
    "forbidden": {
      "length": 3,
      "position": [1, 0]
    }
  },
  "output": {
    "edge_list": true,
    "png_file": false,
    "output_dir": "results"
  }
}
```

---

## テスト

```sh
# テストの実行
ctest --output-on-failure
```

---

## 依存ライブラリ（CMake経由で取得）

- **Google Test** (v1.17.0): ユニットテストフレームワーク
- **nlohmann-json** (v3.12.0): JSONパーサー
- **Eigen** (v5.0.0): 線形代数ライブラリ
- **Spectra** (v1.2.0): 固有値計算ライブラリ
- **CLI11** (v2.5.0): コマンドライン引数解析ライブラリ

---

## ライセンス

このプロジェクトは[Unlicense](LICENSE)の下で公開．詳細はLICENSEファイルを参照．

---

## 参考文献

- Moision, B. & Siegel, P.H. _Periodic-finite-type shift spaces._ IEEE ISIT 2001
- de Bruijn, N.G. _A combinatorial problem._ Proc. KNAW 1946
- Béal, M.-P., Crochemore, M., Fici, G. _Presentations of constrained systems with unconstrained positions._ IEEE Trans. IT 2005
