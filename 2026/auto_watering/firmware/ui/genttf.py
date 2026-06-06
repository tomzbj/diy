import argparse
import os
import sys
from fontTools import subset
import matplotlib.font_manager as fm

def get_font_path(font_name):
    if os.path.exists(font_name):
        return font_name
    try:
        # 尝试匹配系统字体名
        font_props = fm.FontProperties(family=font_name)
        path = fm.findfont(font_props, fallback_to_default=False)
        return path
    except:
        return None

def main():
    # --- 核心修复：手动分离选项和位置参数 ---
    # 这样无论你把 -a 放在哪里，解析器都能正确把最后两个识别为 font_name 和 output
    argv = sys.argv[1:]
    options_part = []
    positional_part = []
    
    i = 0
    while i < len(argv):
        if argv[i].startswith('-'):
            options_part.append(argv[i])
            # 如果是带值的选项 (-t, -f)，把下一个值也带走
            if argv[i] in ['-t', '--text', '-f', '--file'] and i + 1 < len(argv):
                options_part.append(argv[i+1])
                i += 1
        else:
            positional_part.append(argv[i])
        i += 1
    
    # 重新组合：选项在前，位置在后
    reordered_args = options_part + positional_part

    parser = argparse.ArgumentParser(description="字体子集提取工具")
    parser.add_argument("-a", "--ascii", action="store_true")
    parser.add_argument("-ax", "--ascii_ext", action="store_true")
    parser.add_argument("-n", "--numbers", action="store_true")
    parser.add_argument("-t", "--text", type=str)
    parser.add_argument("-f", "--file", type=str)
    parser.add_argument("font_name", help="字体名")
    parser.add_argument("output", help="输出文件名")

    # 使用重排后的参数
    args = parser.parse_args(reordered_args)

    # --- 后续逻辑不变 ---
    font_path = get_font_path(args.font_name)
    if not font_path or not os.path.exists(font_path):
        print(f"错误: 找不到字体 '{args.font_name}'。")
        print("提示: 请尝试输入完整的字体路径，或检查字体名是否正确。")
        sys.exit(1)

    chars = set()
    if args.ascii: chars.update(chr(i) for i in range(128))
    if args.ascii_ext: chars.update(chr(i) for i in range(256))
    if args.numbers: chars.update("0123456789")
    if args.text: chars.update(args.text)
    if args.file:
        with open(args.file, 'r', encoding='utf-8') as f:
            chars.update(f.read())

    if not chars:
        print("错误: 必须指定提取范围 (-a, -ax, -n, -t, -f 至少选其一)")
        sys.exit(1)

    try:
        opt = subset.Options()
        # 保持一些基础的 OpenType 特性
        font = subset.load_font(font_path, opt)
        subsetter = subset.Subsetter(options=opt)
        subsetter.populate(text="".join(chars))
        subsetter.subset(font)
        font.save(args.output)
        font.close()
        print(f"成功生成: {args.output} (来自: {font_path})")
    except Exception as e:
        print(f"处理失败: {e}")

if __name__ == "__main__":
    main()

