# 图形学 CG

## 图形学基本概念

### 颜色视觉

人眼感知到的颜色由三个决定：照明条件（光源的谱分布），物体材质（物体的反射光谱），观察条件

几种常见的颜色空间：

**RGB:**$(R,G,B), rR+gG+bB$ 有负系数情况，通常归一化为$[0,1]$之间的float number,或$[0,255]$之间的8=bits无符号整数

![image-20250225082604505](C:\Users\35551\AppData\Roaming\Typora\typora-user-images\image-20250225082604505.png)

**CMY:**分别用RGB的互补颜色，减色系统

**HSV：**常用于图像处理、艺术领域，（Hue色调，Saturation饱和度，Value of brightness亮度)

**CIE XYZ:**基于RGB的线性变换

### 图像与像素

- 二维场景：**pixel**，$f(x,y)$, eg: 四维（RGBA），$f:\mathbb{R}^2\rightarrow \mathbb{R}^4$ 

- 三维图形的表征：

​	顶点集合：$V = (v_1, \cdots, v_n)$

​	面片集合：$F = (f_1, \cdots f_m), \quad f_i = (v_{ai},v_{bi}, v_{ci})$

> 如何确定三角面片的法向：按照周围的面片的法向有不同的加权平均方式

![image-20250225084431227](C:\Users\35551\AppData\Roaming\Typora\typora-user-images\image-20250225084431227.png)

### 光照与着色

- 光照模型

  - local lighting
  - global lighting
  - 光照模型的历史：bouknight ->Gouraund->Phong->Jimkajiya

- **Phong**光照模型

  Phong模型认为光照可以分解为环境光，漫反射光和镜面反射光

$$
I = I_a + I_d + I_s\\
I_a = k_a \cdot I_l\\
I_d = k_d \cdot \max\{(\mathbf{L} \cdot \mathbf{N})\} \cdot I_l\\
I_s = k_s \cdot \max\{0,(\mathbf{R} \cdot \mathbf{V})^n)\} \cdot I_l\\
\mathbf{R} = 2 \cdot (\mathbf{L} \cdot \mathbf{N}) \cdot \mathbf{N} - \mathbf{L}\\
I = k_a \cdot I_l + k_d \cdot (\mathbf{L} \cdot \mathbf{N}) \cdot I_l + k_s \cdot (\mathbf{R} \cdot \mathbf{V})^n \cdot I_l\\
$$

![image-20250225091709625](C:\Users\35551\AppData\Roaming\Typora\typora-user-images\image-20250225091709625.png)

Phong模型常采用对法向进行插值的方式做平滑。