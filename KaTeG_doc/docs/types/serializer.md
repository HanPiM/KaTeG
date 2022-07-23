# serializer

序列化器

储存用于输出的各种参数设置，提供基础操作函数

## 子类型

`using draw_func_t = std::function<kunit(serializer&)>;`

储存一个绘制函数。该函数应接受一个序列化器，对其操作完后返回占用的宽度。

使用 `run()`，流插入运算或直接将序列化器作为参数传入以调用。

## 公有函数

### 构造

`serializer(uint32_t mode=0,kitem_size siz=kitem_size::normal)`

指定初始的 [模式](../../constant/#serializer_mode-struct_enumbit_flag)，[大小](../../constant/#kitem_size-enum-class)

### 运算重载

`serializer& operator<<(const draw_func_t& func)`

用自身调用 `func`，调用后不进行归位。

### caller

`kunit run(const draw_func_t& func)` 

执行 `func(*this)` 后归位。

即 `kern(-func(*this))`

### getter

`uint32_t mode()const` 返回序列化器当前参数：模式

`kitem_size item_size()const` 返回序列化器当前参数：大小

`void output_to(std::ostream& os)` 将结果输出至 `os`

`unsigned int color()const noexcept` 返回当前参数：颜色

`bool support_math_context()const` 返回是否支持在文本模式中启用数学模式

`bool support_fake_math_context()const` 返回是否支持使用 `begin_fake_math_context` 详见下文

`void output_to(std::string& s)const` 将结果输出至 `s`

### setter

`uint32_t mode(uint32_t m)` 将当前模式设为 `m`

`kitem_size item_size(kitem_size siz)`  将当前 item 大小设为 `siz`

`void color(unsigned int hex_rgb)` 设置当前颜色为 16 进制 RGB 颜色 `hex_rgb`

`void color(unsigned char r, unsigned char g, unsigned char b)` 设置当前颜色为 RGB 颜色

### 原始 KaTeX 宏

`void add_str(const std::string& s)` 添加字符串

`void add_char(char ch)` 添加字符

---

`void kern(kunit x)` 添加大小为 x 的横向偏移

---

`void rule(kunit w, kunit h, kunit offset = 0_pt)` 在上方 `offset` 处绘制一长宽（均不可为负）分别为 w 和 h 的实心矩形

**最小精度**：

- w 大于 0.1pt
- h 大于 0.1pt

### 环境

`void end_context()` 结束当前环境

`void end_all_context()` 结束所有环境

---

- `void begin_empty_context(char beg = '{', char end = '}')`

    写入 `beg` 并在环境结束时写入 `end`

- `void begin_raisebox_context(kunit x)`

	使用 `\raisebox` 使输出纵向偏移 `x`（如果 `x` 为 0 该函数等价于 begin_empty_context）

- `void begin_raisemath_context(kunit x)`

	在 `begin_raisebox_context` 功能的基础上尝试将内部转换为数学模式。

- `void begin_x_overlapping_context(kalign_style align_style = kalign_style::left)`

	横向重叠环境（消除横向碰撞箱）讨论区环境下超高部分会下移

	`align_style` 向哪个方向对齐，参见 [kalign_style](../../constant/#kalign_style-enum-class)

- `void begin_y_overlapping_context()` 纵向重叠环境（消除纵向碰撞箱）

- `void begin_fake_math_context()`

	用 boxed 和 overlapping 制造出数学模式，讨论区环境下不可用

- `void begin_inline_math_context()`

	使用 `$...$`（for_inline 模式下不可用）

- `void try_begin_math_context(bool no_color_op = false)`

	尝试启用数学模式，如失败将抛出 `std::logic_error` 异常：`cur_mode not support math_mode on text_mode`