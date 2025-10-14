# PFT-tools

## 概要

PFT-tools は、PFT シフト空間のプレゼンテーションを生成、保存、解析するための C++17 CLI ツールです。

**PFT**: 有限個の周期的禁止語で定義されるシフト空間 [Moision & Siegel, 2001]

---

## CLI 使用例

以下はコマンドラインでの基本的な使い方です。

```sh
# JSON 設定ファイルからグラフを生成
pft-tools --config <設定ファイル>.json

# CSV ファイルから最大固有値を計算
pft-tools --config <エッジリスト>.csv --format edges --max-eig
pft-tools --config <隣接行列>.csv --format matrix --max-eig

# ディレクトリ内すべての CSV ファイルから最大固有値を計算
pft-tools --config <ディレクトリ名> --format matrix --max-eig
```

- `<設定ファイル>.json`: 生成用の `JSON` ファイル
- `<エッジリスト>.csv`: エッジリスト形式の `CSV` ファイル
- `<隣接行列>.csv`: 隣接行列形式の `CSV` ファイル
- `<ディレクトリ名>`: 複数の `CSV` ファイルが入ったディレクトリ

---

## 機能

### 生成

- De Bruijn アルゴリズム [de Bruijn, 1946]
- Béal アルゴリズム [Béal et al., 2005]
- オプションで以下を適用可能:
  - 孤立頂点の削除
  - 最小化

### 保存

- CSV: エッジリスト、隣接行列

### 解析

- 最大固有値の算出（Spectra 使用）

---

## 入力 JSON（生成時の例）

### Custom モード例

```json
{
  "mode": "custom",
  "algorithm": "Beal", // "DeBruijn" も可
  "sink_less": true, // シンクレス化
  "minimize": true, // Moore 最小化 ( sink_less が true のときのみ有効)
  "alphabet_size": 2, // シンボル数
  "period": 3, // 周期
  "forbidden_words": [
    ["000", 0],
    ["11", 1]
  ], // 禁止語リスト
  "output": {
    "formats": ["edges", "matrix"], // 出力形式
    "directory": "results/" // 出力ディレクトリ
  }
}
```

### All-Patterns モード例

```json
{
  "mode": "all-patterns",
  "algorithm": "DeBruijn",
  "sink_less": true,
  "minimize": true,
  "alphabet_size": 3,
  "period": 2,
  "forbidden_word_length": 5, // 禁止語長
  "forbidden_per_position": [1, 0], // 周期位置ごとの禁止語数
  "output": {
    "formats": ["edges", "matrix"],
    "directory": "results/"
  }
}
```

---

## パラメータ範囲

| パラメータ     | 範囲    |
| -------------- | ------- |
| Q (シンボル数) | 2–36    |
| T (周期)       | 1–10    |
| L (禁止語長)   | 1–10    |
| N (禁止語数)   | 1–10    |
| 最大ノード数   | T × Q^L |

---

## 依存ライブラリ

- C++17
- CMake
- Eigen / Spectra
- Google Test

---

## ビルドと実行

```sh
mkdir build && cd build
cmake .. && cmake --build .
cd .. && ./build/pft-tools config/sample.json
```

---

## 参考文献

- Moision, B. & Siegel, P.H. _Periodic-finite-type shift spaces._ IEEE ISIT 2001
- de Bruijn, N.G. _A combinatorial problem._ Proc. KNAW 1946
- Béal, M.-P., Crochemore, M., Fici, G. _Presentations of constrained systems with unconstrained positions._ IEEE Trans. IT 2005
