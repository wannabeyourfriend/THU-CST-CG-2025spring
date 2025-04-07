# 将LaTeX文档转换为Markdown格式

以下是将您的LaTeX报告转换为Markdown格式的内容：

# Computer Graphic PA0

**作者：** 王子轩  
**学号：** 2023011307  
**邮箱：** wang-zx23@mails.tsinghua.edu.cn  
**日期：** 2025/3/15

## Algorithm

### 线段绘制

首先，利用最原始的Bresenham画线算法，可以画出一条从起点(x₀,y₀)到终点(x₁, y₁)的线段，其中起点需要在终点的左下方。但该过程需要利用除法计算k，实际上不是必须的。

**Basic Bresenham Line Drawing Algorithm:**

输入：起点(x₀, y₀)，终点(x₁, y₁)，图像对象和颜色  
输出：在指定颜色下从(x₀, y₀)到(x₁, y₁)的线段

1. 计算差值：dx = x₁ - x₀, dy = y₁ - y₀
2. 计算斜率：k = dy/dx
3. 初始化误差项：e = -0.5
4. 设置起始位置：x = x₀, y = y₀
5. 对于每个i从0到dx：
   - 在图像中将像素(x, y)设置为给定颜色
   - x增加1
   - 更新误差项：e = e + k
   - 如果e ≥ 0：
     - y增加1
     - 减少误差项：e = e - 1

我们将该算法进行改进：通过计算起点和终点的坐标差值(dx, dy)，确定绘制过程中每一步在x和y方向上的变化量。根据起点和终点的相对位置，代码决定了每次x和y坐标的增减步长(sx和sy)。然后，设置一个初始误差值(err)，并进入循环，根据误差值调整x和y的坐标。每次迭代中，当前像素点(x0, y0)会被绘制，如果当前点已经到达终点，则退出循环。如果没有到达终点，算法根据误差值判断是更新x坐标还是y坐标。

**实现 void Line::draw():**

```cpp
void draw(Image &img) override {
    int x0 = xA, y0 = yA, x1 = xB, y1 = yB;
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    while (true){
        img.SetPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy){ 
            if (x0 == x1) break;
            err += dy; x0 += sx; 
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            err += dx; y0 += sy;  
        }
    }
}
```

### 圆弧绘制

使用中点画圆算法(Midpoint Circle Algorithm)来绘制圆。该算法基于圆的八分对称性，只需要计算圆的八分之一的点，然后通过对称变换得到整个圆的所有点。

**Midpoint Circle Drawing Algorithm:**

输入：圆心(cx, cy)，半径r，图像对象和颜色  
输出：在指定颜色下绘制中心为(cx, cy)，半径为r的圆

1. 初始化：x = 0, y = r
2. 计算初始决策参数：d = 1.25 - r
3. 在(cx ± x, cy ± y)和(cx ± y, cy ± x)处绘制点
4. 当x ≤ y时：
   - 如果d < 0：
     - 更新决策参数：d = d + 2x + 3
   - 否则：
     - 更新决策参数：d = d + 2(x - y) + 5
     - y减少1
   - x增加1
   - 在(cx ± x, cy ± y)和(cx ± y, cy ± x)处绘制点

算法使用一个决策参数d来决定下一个像素点的位置，避免了浮点数运算。当d < 0时，选择水平方向的像素点；当d ≥ 0时，选择对角线方向的像素点。决策参数d的推导过程：考虑圆的方程：F(x,y) = x² + y² - r² = 0，在点(xₖ, yₖ)处，我们需要决定下一个点是选择(xₖ+1, yₖ)还是(xₖ+1, yₖ-1)。中点判别法考虑这两个候选点的中点(xₖ+1, yₖ-½)。定义决策参数：dₖ = F(xₖ+1, yₖ-½)，= (xₖ+1)² + (yₖ-½)² - r²。当dₖ < 0时，中点在圆内，选择(xₖ+1, yₖ)；当dₖ ≥ 0时，中点在圆外或圆上，选择(xₖ+1, yₖ-1)。

下一个决策参数dₖ₊₁的递推关系：
1. 当dₖ < 0时(选择E点)：
   - dₖ₊₁ = F(xₖ+2, yₖ-½)
   - = dₖ + 2(xₖ+1) + 1
   - = dₖ + 2xₖ + 3
2. 当dₖ ≥ 0时(选择SE点)：
   - dₖ₊₁ = F(xₖ+2, (yₖ-1)-½)
   - = dₖ + 2(xₖ+1) - 2(yₖ-1) + 1
   - = dₖ + 2(xₖ - yₖ) + 5

这就是代码中更新决策参数d的依据。初始值d₀ = 1.25 - r是将k=0代入决策参数公式得到的。

**实现 void Circle::draw():**

```cpp
void draw(Image &img) override {
    int x = 0, y = radius; float d = 1.25 - radius;
    auto drawCirclePoints = [&](int x, int y) {
        img.SetPixel(cx + x, cy + y, color); img.SetPixel(cx - x, cy + y, color);
        img.SetPixel(cx + x, cy - y, color); img.SetPixel(cx - x, cy - y, color);
        img.SetPixel(cx + y, cy + x, color); img.SetPixel(cx - y, cy + x, color);
        img.SetPixel(cx + y, cy - x, color); img.SetPixel(cx - y, cy - x, color);
    };
    drawCirclePoints(x, y);
    while (x <= y) {
        if (d < 0)  d += 2 * x + 3
        else { 
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        drawCirclePoints(x, y);
    }
}
```

### 颜色填充

洪水填充算法(Flood Fill Algorithm)是一种用于填充连通区域的算法。该算法从一个起始点开始，通过广度优先搜索的方式，将所有与起始点颜色相同且相连的像素点都替换为目标颜色。

**Flood Fill Algorithm:**

输入：起始点(cx, cy)，目标颜色color，图像对象img  
输出：从给定点开始填充连通区域

1. 获取起始点的颜色：targetColor = img(cx, cy)
2. 如果targetColor = color则返回
3. 创建队列Q并将起始点(cx, cy)添加到Q中
4. 当Q非空时：
   - 从Q中获取前端像素(x, y)
   - 如果(x, y)超出边界或者(x, y)处的颜色≠targetColor：
     - 继续下一次迭代
   - 将(x, y)处的颜色设置为color
   - 将(x+1, y)，(x-1, y)，(x, y+1)，(x, y-1)添加到Q中

采用广度优先搜索策略，使用队列存储待处理的像素点。对于每个待处理的像素点，首先检查其是否越界或是否需要填充(颜色是否与目标颜色相同)。如果需要填充，则将其颜色更改为目标颜色，并将其四个相邻像素点(上，下，左，右)加入队列。这个过程会一直持续到队列为空，即所有需要填充的像素都已处理完毕。

**实现 void Fill::draw():**

```cpp
void draw(Image &img) override {
    Vector3f targetColor = img.GetPixel(cx, cy);
    if (targetColor == color) {
        return;
    }
    std::queue<std::pair<int, int>> pixels;
    pixels.push({cx, cy});
    int width = img.Width();
    int height = img.Height();
    while (!pixels.empty()) {
        auto [x, y] = pixels.front();
        pixels.pop();
        if (x < 0 || x >= width || y < 0 || y >= height ||
            img.GetPixel(x, y) != targetColor) {
            continue;
        }
        img.SetPixel(x, y, color);
        pixels.push({x + 1, y}); pixels.push({x - 1, y}); 
        pixels.push({x, y + 1}); pixels.push({x, y - 1}); 
    }
}
```

## Honor Code

PA0算法思路部分借鉴了<计算机图形学基础>(第二版)P22-P31内容，就代码实现时的bug与Claud-Sonnet 3.5模型进行了交互，并借鉴了其部分做法

## Problem and Solution

### k = dy/dx当dx=0 以及起止点相对位置讨论

在1.1部分的代码实现中，我本来是利用的原版的Bresenham画线算法，需要计算直线的斜率，然而没有考虑到除法分母为零的竖直直线情形，从而产生了错误的代码逻辑，产生了如图所示的测试例子；同时对起止点的位置进行讨论因此绘图没有正确处理。解决办法：采用改进后的算法，改用整数避免除法，做e₂ = 2 * err的替换，并引入sx和sy控制起止点相对位置与绘制点自增方向的关系。

## Suggestion

一个是建议可以给更多的测试例子；第二个是好像这三个算法都比较简单(?)，也许可以增加一些反走样的实验。感觉上小作业难度和大作业难度跨越较大(?)上课内容感觉相对前沿，但上课内容和作业似乎线性无关。
