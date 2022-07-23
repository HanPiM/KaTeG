# <small>kgeo::</small>rect_t

二维平面内由xy范围确定的一个矩形。

## 公有函数

### 构造

`rect_t()` 一个空的矩形

`rect_t(double x1, double y1, double x2, double y2)`/`rect_t(const `<small>kgeo::</small>[`point_t`](../point_t)`& s, const `<small>kgeo::</small>[`point_t`](../point_t)`& e)`

由两个点确定的矩形，两点连线所成线段即为对角线。

`rect_t(const std::initializer_list<double> vals)`

同上，但两点坐标由列表给出，形如 `{x0,y0,x1,y1}`。

### 运算重载

`rect_t operator &(const rect_t& rc)const`

`const rect_t& operator&=(const rect_t& rc)`

与矩形 rc 相交所得矩形

`friend `<small>kgeo::</small>[`range_t`](../range_t)` operator-(const `<small>kgeo::</small>[`range_t`](../range_t)`& p, const rect_t& ref_sys)`

 `friend const `<small>kgeo::</small>[`range_t`](../range_t)`& operator-=(`<small>kgeo::</small>[`range_t`](../range_t)`& p, const rect_t& ref_sys)`

p 相对于 ref_sys 的位置

`std::ostream& operator<<(std::ostream& os, const rect_t& rc)`

向流输出矩形，格式为 `{left,bottom,right,top}`

### getter

#### [检测器]

`bool empty()const` 是否为空

`bool contain(const `<small>kgeo::</small>[`point_t`](../point_t)`& p)const` 是否包含点 p

#### [边界]

`const `<small>kgeo::</small>[`range_t`](../range_t)`& xrange()const` x方向上的范围

`const `<small>kgeo::</small>[`range_t`](../range_t)`& yrange()const` y方向上的范围

---

<small>kgeo::</small>[`point_t`](../point_t) `spos()const` 左下角坐标

<small>kgeo::</small>[`point_t`](../point_t) `epos()const` 右下角坐标

---

`double left()const` 左边界

`double right()const` 右边界

`double bottom()` 底部

`double top()` 顶部

#### [大小]

`double width()const` 宽度

`double height()const` 高度

### setter

#### [边界]

`void xrange(const `<small>kgeo::</small>[`range_t`](../range_t)`& xr)` 设置 x 方向上的范围

`void yrange(const `<small>kgeo::</small>[`range_t`](../range_t)`& yr)` 设置 y 方向上的范围

---

`void spos(const `<small>kgeo::</small>[`point_t`](../point_t)`& pos)` 设置左下角坐标

`void epos(const `<small>kgeo::</small>[`point_t`](../point_t)`& pos)`设置右下角坐标

---

`void left(double v)` 设置左边界

`void right(double v)` 设置右边界

`void bottom(double v)` 设置底部

`void top(double v)` 设置顶部

#### [移动]

`void move_l(double v)` 将矩形左移 v

`void move_r(double v)` 将矩形右移 v

`void move_u(double v)` 将矩形上移 v

`void move_d(double v)` 将矩形下移 v

`void move_to0()` 将矩形移至原点（左下角为原点）
