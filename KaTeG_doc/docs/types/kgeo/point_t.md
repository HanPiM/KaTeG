# <small>kgeo::</small>point_t

描述平面直角坐标系中的一个点，储存 x 和 y

## 公有变量

`double x` 横坐标

`double y` 纵坐标

## 公有函数

### 构造

`point_t()` 原点

`point_t(double x, double y)` 点 (x,y)

`point_t(const std::initializer_list<double> vals)` 取列表中的前两项作为 x 和 y（如 {1,2} 得到点 (1,2)）

### 运算重载

- 与 kunit 的加减（按照向量的加减法处理）
- 与 double 相乘/除（按照向量乘以标量来处理）
- 流插入符（输出点）

### setter

`void assign(double xx, double yy)` 赋值为点 (xx,yy)

`void assign(const point_t& p)` 赋值为点 p