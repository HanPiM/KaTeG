# <small>kgeo::</small>draw_range_t

绘制范围的信息

!!! warning "缩放比限制"
	由于 KaTeX 对于较小对象的绘制精度极低（部分甚至无法绘制），请确保缩放后能满足 [绘制函数](../../../draw_func/draw) 的需求。默认最小缩放比为 1pt。

## 基类

<small>kgeo::</small>[`rect_t`](../../../types/kgeo/rect_t)

## 公有函数

### 构造

`draw_range_t()` 空范围

`draw_range_t(const rect_t& rc, kunit zoom = 4_pt, kunit zoom_y = 0)`

- `rc` 坐标系内绘制范围
- `zoom` 横向缩放
- `zoom_y` 纵向缩放（如小于零将设为 `zoom`）

`draw_range_t(kunit w, kunit h, const rect_t& rc)`

通过期望输出大小计算缩放比。

- `w` 期望输出宽度
- `h` 期望输出高度
- `rc` 坐标系内绘制范围

### getter

`kunit view_width() const` 期望输出宽度

`kunit view_height() const` 期望输出高度

`kunit zoomx() const` 横向缩放

`kunit zoomy()const` 纵向缩放