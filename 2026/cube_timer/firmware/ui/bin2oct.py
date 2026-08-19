import sys
import os
import argparse
import re

def is_valid_c_identifier(name):
    """检查是否为合法的 C 语言标识符"""
    # C 标识符：字母或下划线开头，后跟字母、数字或下划线
    return re.fullmatch(r'[a-zA-Z_][a-zA-Z0-9_]*', name) is not None

def generate_escaped_data(data):
    """核心转换逻辑：八进制转义 + Trigraph 防护"""
    res = []
    line = "\""
    escaped = False 
    last_was_question = False

    for c in data:
        char_str = ""
        current_is_octal = False
        current_is_question = False

        if c == ord('\\'):
            char_str = "\\\\"
        elif c == ord('\"'):
            char_str = "\\\""
        elif c == ord('?'):
            if last_was_question:
                char_str = "\\77"
                current_is_octal = True
            else:
                char_str = "?"
                current_is_question = True
        elif 33 <= c <= 126:
            if escaped and ord('0') <= c <= ord('7'):
                char_str = "\\%o" % c
                current_is_octal = True
            else:
                char_str = chr(c)
        else:
            char_str = "\\%o" % c
            current_is_octal = True

        if len(line) + len(char_str) > 75:
            res.append(line + "\"\n")
            line = "\""
            escaped = False
            last_was_question = False
        
        line += char_str
        escaped = current_is_octal
        last_was_question = current_is_question

    res.append(line + "\"")
    return "".join(res)

def main():
    parser = argparse.ArgumentParser(description="二进制文件转 C 字符串工具")
    parser.add_argument("input", help="输入文件路径")
    parser.add_argument("output", help="输出文件名（作为变量名，不含后缀）")
    parser.add_argument("-c", action="store_true", help="生成 .c 和 .h 文件")
    args = parser.parse_args()

    # 验证变量名合法性
    var_name = args.output
    if not is_valid_c_identifier(var_name):
        print(f"错误: '{var_name}' 不是合法的 C 语言标识符（不能以数字开头，只能包含字母数字下划线）。")
        sys.exit(1)

    try:
        with open(args.input, "rb") as f:
            data = f.read()
    except Exception as e:
        print(f"读取输入文件失败: {e}")
        sys.exit(1)

    escaped_text = generate_escaped_data(data)

    if args.c:
        c_file = f"{var_name}.c"
        h_file = f"{var_name}.h"
        macro_name = f"_{var_name.upper()}_H_"

        # 写入 .c 文件
        with open(c_file, "w") as f:
            f.write("#include <stdint.h>\n\n")
            f.write(f"const uint8_t {var_name}[] = \n{escaped_text};\n\n")
            f.write(f"const uint32_t {var_name}_size = {len(data)};\n")

        # 写入 .h 文件
        with open(h_file, "w") as f:
            f.write(f"#ifndef {macro_name}\n#define {macro_name}\n\n")
            f.write("#include <stdint.h>\n\n")
            f.write(f"extern const uint8_t {var_name}[];\n")
            f.write(f"extern const uint32_t {var_name}_size;\n\n")
            f.write(f"#endif // {macro_name}\n")
        
        print(f"转换成功: 已生成 {c_file} 和 {h_file}")
    else:
        with open(var_name, "w") as f:
            f.write(escaped_text + "\n")
        print(f"转换成功: 已生成文本文件 {var_name}")

if __name__ == "__main__":
    main()

