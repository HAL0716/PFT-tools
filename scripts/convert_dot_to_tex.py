from dot2tex import dot2tex
import os
import sys

def format_tex_code(tex_code: str) -> str:
    """TeXコードを整形"""
    tex_code = tex_code.replace(r'\documentclass{article}', r'\documentclass[border=5pt]{standalone}')
    return '\n'.join(line for line in tex_code.splitlines() if not line.strip().startswith(r'\enlargethispage'))

def read_dot_file(dot_file_path: str) -> str:
    """DOTファイルを読み込む"""
    with open(dot_file_path, 'r') as dot_file:
        return dot_file.read()

def write_tex_file(tex_file_path: str, tex_content: str) -> None:
    """TeXファイルに書き込む"""
    with open(tex_file_path, 'w') as tex_file:
        tex_file.write(tex_content)

def convert_dot_to_tex(dot_file_path: str, tex_file_path: str = "") -> None:
    """DOTファイルをTeXファイルに変換"""
    if not os.path.exists(dot_file_path):
        print(f"Error: {dot_file_path} が存在しません。")
        sys.exit(1)

    # 出力ファイルのパスを生成
    tex_file_path = tex_file_path or os.path.splitext(dot_file_path)[0] + ".tex"
    dot_content = read_dot_file(dot_file_path)
    tex_content = dot2tex(dot_content, format='tikz')
    formatted_tex_content = format_tex_code(tex_content)
    write_tex_file(tex_file_path, formatted_tex_content)

def main() -> None:
    """メイン処理"""
    if len(sys.argv) < 2 or 3 < len(sys.argv):
        print("使い方: python convert_dot_to_tex.py <dot_file_path> <tex_file_path (optional)>")
        sys.exit(1)

    dot_file_path = sys.argv[1]
    tex_file_path = sys.argv[2] if len(sys.argv) == 3 else ""
    convert_dot_to_tex(dot_file_path, tex_file_path)

if __name__ == "__main__":
    main()
