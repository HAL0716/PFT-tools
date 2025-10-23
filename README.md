# PFT-tools

## 概要

PFT-tools は、周期的禁止語によって定義されるシフト空間の生成、保存、解析を行うための C++17 CLI ツールです。
このツールは、情報理論や符号理論における研究者やエンジニアを対象としており、以下の機能を提供します。

- **グラフ生成**: De Bruijn アルゴリズムや Béal アルゴリズムを使用
- **データ保存**: CSV形式や画像形式 (PNG, PDF) での保存
- **解析**: 最大固有値の算出や許可系列の抽出

---

## 背景と目的

周期的禁止語 (Periodic Forbidden Words) に基づくシフト空間は、符号理論や情報理論において重要な役割を果たします。本ツールは、これらのシフト空間を効率的に扱うためのツールとして設計されました。

---

## アーキテクチャ概要

プロジェクトは以下のような構造を持っています。

```plaintext
├── src/
│   ├── algorithm/       # アルゴリズム (Beal, DeBruijn, Moore)
│   ├── core/            # 基本的なデータ構造 (Node, Edge, Graph)
│   ├── io/              # 入出力処理 (JSON, CSV)
│   ├── utils/           # ユーティリティ関数
│   └── main.cpp         # エントリーポイント
└── tests/               # Google Test によるユニットテスト
```

---

## インストール

### 必要なライブラリ

- C++17 対応のコンパイラ (例: g++)
- CMake 3.17 以上
- Python 3 (オプション: dot2tex パッケージ)

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

### JSON 設定ファイルを使用したグラフ生成

```sh
# JSON 設定ファイルからグラフを生成
./pft-tools --input config/sample.json --png
```

### CSV ファイルを使用した解析

```sh
# エッジリスト形式の CSV ファイルから最大固有値を計算
./pft-tools --input data/edges.csv --format edges --max-eig

# 隣接行列形式の CSV ファイルから最大固有値を計算
./pft-tools --input data/matrix.csv --format matrix --max-eig

# エッジリスト形式の CSV ファイルから指定長さの許可系列を取得
./pft-tools --input data/edges.csv --format edges --sequences 5

# グラフを PDF 形式で保存
./pft-tools --input data/edges.csv --format edges --pdf

# グラフを PNG 形式で保存
./pft-tools --input data/edges.csv --format edges --png
```

### ディレクトリ内の複数 CSV ファイルを一括処理

```sh
# ディレクトリ内のすべてのエッジリスト形式 CSV のファイルから最大固有値を計算
./pft-tools --input data/ --format edges --max-eig

# ディレクトリ内のすべての隣接行列形式 CSV のファイルを PNG 形式で保存
./pft-tools --input data/ --format matrix --png
```

---

## テスト

ユニットテストは Google Test を使用して記述されています。

```sh
# テストの実行
ctest
```

---

## 依存ライブラリ

PFT-toolsは以下の主要なライブラリを使用しています。これらはCMakeを通じて自動的に取得されます。

- **Google Test** (v1.17.0): ユニットテストフレームワーク
- **nlohmann-json** (v3.12.0): JSONパーサー
- **Eigen** (v5.0.0): 線形代数ライブラリ
- **Spectra** (v1.2.0): 固有値計算ライブラリ
- **CLI11** (v2.5.0): コマンドライン引数解析ライブラリ

---

## ライセンス

このプロジェクトは [Unlicense](LICENSE) の下で公開されています。詳細は LICENSE ファイルを参照してください。

---

## 参考文献

- Moision, B. & Siegel, P.H. _Periodic-finite-type shift spaces._ IEEE ISIT 2001
- de Bruijn, N.G. _A combinatorial problem._ Proc. KNAW 1946
- Béal, M.-P., Crochemore, M., Fici, G. _Presentations of constrained systems with unconstrained positions._ IEEE Trans. IT 2005
