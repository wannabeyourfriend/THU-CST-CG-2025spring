# 图形学 CG

> Notes Taking: 王子轩
>
> `wang-zx23@mails.tsinghua.edu.cn`
>
> Lecture Instructor: 胡事明

[TOC]

### 清华大学学生纪律处分管理规定实施细则

#### 第六章　学术不端、违反学习纪律的行为与处分

第二十一条 有下列违反课程学习纪律情形之一的，给予警告以上、留校察看以下处分：

（一）课程作业抄袭严重的；

（二）实验报告抄袭严重或者篡改实验数据的；

（三）期中、期末课程论文抄袭严重的；

（四）在课程学习过程中严重弄虚作假的其他情形。

## 1 图形学基本概念

### 1.1 颜色视觉

人眼感知到的颜色由三个决定：照明条件（光源的谱分布），物体材质（物体的反射光谱），观察条件

几种常见的颜色空间：

**RGB:**$(R,G,B), rR+gG+bB$ 有负系数情况，通常归一化为$[0,1]$之间的float number,或$[0,255]$之间的8=bits无符号整数

![image-20250225082604505](assets/image-20250225082604505.png)

**CMY:**分别用RGB的互补颜色，减色系统

**HSV：**常用于图像处理、艺术领域，（Hue色调，Saturation饱和度，Value of brightness亮度)

**CIE XYZ:**基于RGB的线性变换

### 1.2 图像与像素

- 二维场景：**pixel**，$f(x,y)$, eg: 四维（RGBA），$f:\mathbb{R}^2\rightarrow \mathbb{R}^4$ 

- 三维图形的表征：

​	顶点集合：$V = (v_1, \cdots, v_n)$

​	面片集合：$F = (f_1, \cdots f_m), \quad f_i = (v_{ai},v_{bi}, v_{ci})$

> 如何确定三角面片的法向：按照周围的面片的法向有不同的加权平均方式

![image-20250225084431227](assets/image-20250225084431227.png)

### 1.3 光照与着色

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

![image-20250225091709625](assets/image-20250225091709625.png)

Phong模型常采用对法向进行插值的方式做平滑。

## 2 真实感渲染

### 2.1 BRDF(Bidirectional Reflectance Distribution Function)

- 对于给定的场景点 \( p \)，BRDF 是关于入射光方向和反射光方向的四维实值函数，它等于反射方向的光亮度和沿入射方向的入射光的辉度之比：
  $$
  f_r(\omega_i \rightarrow \omega_r) = \frac{\mathrm{d}L_r(\omega_r)}{\mathrm{d}E_i(\omega_i)}
  $$
  
- 可以写成关于入射光的光亮度的形式：
  $$
   f_r(\omega_i \rightarrow \omega_r) = \frac{\mathrm{d}L_r(\omega_r)}{\mathrm{d}E_i(\omega_i)} = \frac{\mathrm{d}L_r(\omega_r)}{L_i(\omega_i) \cos \theta_i \mathrm{d}\omega_i}
  $$
  

### 2.2 The Rendering Equation

描述物体表面的光线传播，相比于反射方程多一项自发项
$$
L_o(p, \omega_o) = L_e(p, \omega_o) + \int_{\Omega} L_i(p, \omega_i) f_r(p, \omega_i, \omega_r) (n \cdot \omega_i) \mathrm{d}\omega_i
$$

- 递归，$L_i$对应的是另外一个点的渲染方程.

## Ray Tracing

主要计算量：确定光线的可见点，对每一条光线，要检查它是否与场景中的物体相交（**Ray Intersection**）

![image-20250317185420111](assets/image-20250317185420111.png)

```python 
IntersectColor(vBeginPoint, vDirection)
{
	Determine IntersectPoint # 这一步需要对场景中的物体光线求交
	Color = ambient color
	for each light
		Color += local shading term # 使用局部光照模型
		if surface is reflective:
			color += ReflectC * IntersectColor(IntersectPoint, Reflect Ray)
		if surface is refrative:
			color += RefractC * IntersectColor(IntersectPoint, Refract Ray)
	return color
}
```

- 光线用

$$
P(t) = R_o + tR_d
$$

- `Determine IntersectPoint`处理光线与平面、三角形、球形、正方体的交点

- 递归的深度：1递归最大深度 2. 当光线的贡献小于阈值 3. 光线射到场景外

蒙特卡洛光线追踪：

估计渲染方程





## 包围盒计算

采用三角面片模型和光线直接求交会导致$\mathcal{o}(n)$的时间复杂，用包围box对于光线进行求交前置判断

- 包围盒：





- ​	隐式表面的包围盒计算
  - 由于隐式表示是$F(x,y,z) =  0$,则对$ax+by+cz = 0$做拉格朗日乘子求min和max值，

- 包围球（boungding ball）
- ![img](https://thu-private-qn.yuketang.cn/slide/15752853/cover512_20250318075030.jpg?e=1742278473&token=IAM-gs8ue1pDIGwtR1CR0Zjdagg7Q2tn5G_1BqTmhmqa:j7Cpb1gV8JaR1r2TXn6Yb2kGDKU=)

![img](https://thu-private-qn.yuketang.cn/slide/15752853/cover620_20250318075030.jpg?e=1742278473&token=IAM-gs8ue1pDIGwtR1CR0Zjdagg7Q2tn5G_1BqTmhmqa:bIfPBmzRkH-KHRneR7FUi_YvPx0=)

