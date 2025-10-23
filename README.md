# PFT-tools

## 概要

PFT-tools は、PFT シフト空間のプレゼンテーションを生成、保存、解析するための C++17 CLI ツールです。

**PFT**: 有限個の周期的禁止語で定義されるシフト空間 [Moision & Siegel, 2001]

---

## CLI 使用例

以下はコマンドラインでの基本的な使い方です。

### JSON 設定ファイルを使用したグラフ生成

```sh
# JSON 設定ファイルからグラフを生成
pft-tools --input config/sample.json
```

### CSV ファイルを使用した解析

```sh
# エッジリスト形式の CSV ファイルから最大固有値を計算
pft-tools --input data/edges.csv --format edges --max-eig

# 隣接行列形式の CSV ファイルから最大固有値を計算
pft-tools --input data/matrix.csv --format matrix --max-eig

# エッジリスト形式の CSV ファイルから指定長さの許可系列を取得
pft-tools --input data/edges.csv --format edges --sequences 5

# グラフを PDF 形式で保存
pft-tools --input data/edges.csv --format edges --pdf

# グラフを PNG 形式で保存
pft-tools --input data/edges.csv --format edges --png
```

### ディレクトリ内の複数 CSV ファイルを一括処理

```sh
# ディレクトリ内のすべてのエッジリスト形式 CSV のファイルから最大固有値を計算
pft-tools --input data/ --format edges --max-eig

# ディレクトリ内のすべての隣接行列形式 CSV のファイルを PNG 形式で保存
pft-tools --input data/ --format matrix --png
```

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
  "generation": {
    "mode": "custom",
    "algorithm": "Beal",     // "DeBruijn", "Beal"
    "opt_mode": "none",      // "none"（通常）, "sink_less"（シンクレス）, "minimize"（最小化）
    "alphabet": 2,           // シンボル数
    "period": 3,             // 周期
    "forbidden": {
      "nodes": [             // 禁止語リスト
        {
          "word": "000",     // 禁止語
          "phase": 0         // 位置
        },
        {
          "word": "11",
          "phase": 1
        }
      ]
    }
  },
  "output": {
    "edge_list": true,       // エッジリスト形式で出力
    "png_file": false,       // PNG 形式で出力
    "output_dir": "results"  // 出力ディレクトリ
  }
}
```

### All-Patterns モード例

```json
{
  "generation": {
    "mode": "all-patterns",
    "algorithm": "DeBruijn",
    "opt_mode": "minimize",
    "alphabet": 3,
    "forbidden": {
      "length": 3,               // 禁止語長
      "position": [1, 0]           // 周期位置ごとの禁止語数
    }
  },
  "output": {
    "edge_list": true,       // エッジリスト形式で出力
    "png_file": false,       // PNG 形式で出力
    "output_dir": "results"  // 出力ディレクトリ
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
