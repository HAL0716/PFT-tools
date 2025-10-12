# PFT-tools

## 概要

* PFT シフト空間のプレゼンテーション生成・保存・解析ツール（C++17 CLI）
* 主目的

  * PFT プレゼンテーション構築
  * 最小化（Moore アルゴリズム）
  * 同型判定
  * 最大固有値算出
* PFT: 有限個の周期的禁止語で定義されるシフト空間 [Moision & Siegel, 2001]

---

## 機能

* **生成**

  * De Bruijn アルゴリズム [de Bruijn, 1946]
  * Béal アルゴリズム [Béal et al., 2005]
  * オプションで生成時に孤立頂点削除・最小化を適用可能
* **保存**

  * CSV: エッジリスト、隣接行列（密行列）
  * Graphviz: `.dot` / `.png`
* **解析**

  * Moore 最小化
  * ディレクトリ内グラフ同型判定（ラベル無視）
  * 最大固有値（Spectra 使用）

---

## 動作モード

| モード          | 入力設定           | basename 生成規則 |
| ------------ | -------------- | ------------- |
| custom       | 禁止語リスト必須       | `<word>-<t>`  |
| all-patterns | 禁止語数を周期位置ごとに指定 | `<word>-<t>`  |

---

## 入力 JSON（生成時の例）

```json
{
  "mode": "custom",
  "algorithm": "debruijn",
  "remove_isolated_nodes": true,
  "minimize": true,
  "alphabet_size": 3,
  "period": 2,
  "forbidden_word_length": 5,
  "forbidden_list": [{ "word": "01210", "position": 0 }],
  "forbidden_per_position": [1, 0],
  "output": {
    "formats": ["edges", "matrix", "dot"],
    "directory": "results/"
  }
}
```

* `remove_isolated_nodes`: `true` → 生成後に孤立頂点を削除
* `minimize`: `true` → 生成後に Moore 最小化を適用
* `output.directory` 未指定 → `<alphabet_size>-<period>-<forbidden_word_length>-<forbidden_word_num>` 自動生成
* 出力サブディレクトリ: `edges/`, `matrix/`, `dot/`

---

## 出力ファイル例（Custom / All-Patterns 共通）

```
<output.directory>/edges/<word>-<t>.csv
<output.directory>/matrix/<word>-<t>.csv
<output.directory>/dot/<word>-<t>.dot
```

* `basename = <word>-<t>`
* サブディレクトリ: `edges/`, `matrix/`, `dot/`

---

## CLI例

```
# 生成
pft-tools generate --config config/sample.json

# 解析
pft-tools analyze --input graphs/ --isomorphism --max-eig
```

---

## パラメータ範囲

| パラメータ     | 範囲      |
| --------- | ------- |
| Q (シンボル数) | 2–36    |
| T (周期)    | 1–10    |
| L (禁止語長)  | 1–10    |
| N (禁止語数)  | 1–10    |
| 最大ノード数    | T × Q^L |

---

## 依存ライブラリ

* C++20 (GCC / Clang)
* CMake
* Eigen / Spectra
* Graphviz (`.png` 出力)
* Google Test

---

## ビルド手順

```
mkdir build
cd build
cmake ..
cmake --build .
ctest
sudo cmake --install .
```

---

## 参考文献

* Moision, B. & Siegel, P.H. *Periodic-finite-type shift spaces.* IEEE ISIT 2001
* de Bruijn, N.G. *A combinatorial problem.* Proc. KNAW 1946
* Béal, M.-P., Crochemore, M., Fici, G. *Presentations of constrained systems with unconstrained positions.* IEEE Trans. IT 2005

---

## 今後の実装ステップ

* コアデータ構造: Graph, Node, Edge, SparseMatrix
* 入出力モジュール: JSON 入力 / CSV・DOT 出力
* 生成アルゴリズム: De Bruijn / Beal
* 解析モジュール: Moore 最小化 / 同型判定 / 固有値計算
* CLI 実装・ログ出力
* テスト自動化（Google Test）
