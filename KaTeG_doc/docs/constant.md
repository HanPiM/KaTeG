---
hide:
  - navigation
---

# 常量

## ktype.hpp

### kitem_size <small>`enum class`</small>

环境中受文本大小控制的所有物件的大小

- normal
- tiny
- script
- footnote
- small
- large
- Large
- LARGE
- huge
- Huge

### kalign_style <small>`enum class`</small>

对齐方式

- left
- middle
- right

### kcolor <small>`struct_enum`</small>

部分 KaTeX 的特殊颜色

- transparent

## serializer.hpp

### serializer_mode <small>`struct_enum|bit_flag`</small>

序列化器的输出设置

- no_def 不使用自定义宏
- for_discuss 指明为讨论区版本，修改部分函数的行为
- inherit_item_size 在讨论区模式下强制子环境继承父环境的 item_size 参数
- discuss_mode `for_discuss | no_def | inherit_item_size` 同时指定上述 3 个标志
- for_inline 指明为内联模式，修改部分函数的行为

## kdraw.hpp

### border_style <small>`enum class`</small>

边框宽度体现方式

- expend_inward 向内绘制
- expend_middle 向内外同时绘制（居中）
- expend_outward 向外绘制