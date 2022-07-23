# <small>kgeo::</small>range_t

一个左右均闭的范围区间

## 公有函数

### 构造

`range_t()` 一个空区间

`range_t(double a, double b)` 区间 [a,b]

### 运算重载

`friend std::ostream& operator<<(std::ostream& o, const range_t& r)`

向流中输出 r，格式形如 [L,R]

### getter

`double left()const` 区间左端点

`double right()const` 区间右端点

`double width()const` 区间大小

`bool contain(double v)const` 是否包含值 v

`bool empty()const` 是否为空

### setter

`void left(double v)` 设置左端点为 v

`void right(double v)` 设置右端点为 v

`void move_l(double v)` 将区间整体向左移 v

`void move_r(double v)` 将区间整体向右移 v

`void assign(double L, double R)` 赋值为 [L,R] 