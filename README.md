## Project 1.1 曲线的绘制 - 代码实现


### Bezier曲线绘制
1. VTNB初始化:
$R[0].V=P[0] \\
R[0].T=P[1]-P[0] \\
R[0].N=(0,0,1)\times R[0].T \\
R[0].B=R[0].T\times R[0].N$

1. 递推公式:
$\begin{aligned}
R[t].V&=[P_1,P_2,P_3,P_4]\cdot M_{BEZ}\cdot T \\
&=[P_1,P_2,P_3,P_4]\cdot\begin{bmatrix}
    1 & -3 & 3 & -1 \\
    0 & 3 & -6 & 3 \\
    0 & 0 & 3 & -3 \\
    0 & 0 & 0 & 1
\end{bmatrix}\cdot\begin{bmatrix}
    1 \\
    t \\
    t^2 \\
    t^3
\end{bmatrix}
\end{aligned}$
$\begin{aligned}
R[t].T&=(R[t].V)'=[P_1,P_2,P_3,P_4]\cdot M_{BEZ}\cdot T' \\
&=[P_1,P_2,P_3,P_4]\cdot\begin{bmatrix}
    1 & -3 & 3 & -1 \\
    0 & 3 & -6 & 3 \\
    0 & 0 & 3 & -3 \\
    0 & 0 & 0 & 1
\end{bmatrix}\cdot\begin{bmatrix}
    0 \\
    1 \\
    2t \\
    3t^2
\end{bmatrix}
\end{aligned}$
$R[t].N=R[t-1].B\times R[t].T \\
R[t].B=R[t].T\times R[t].N$

```cpp
Curve evalBezier(const vector<Vector3f> &P, unsigned steps)
{
	int parts = (P.size() - 1) / 3;
	Curve R((parts * steps) + 1);
	R[0].V = P[0];
	R[0].T = (P[1] - P[0]).normalized();
	R[0].N = Vector3f::cross(Vector3f(0.f, 0.f, 1.f), R[0].T).normalized();
	R[0].B = Vector3f::cross(R[0].T, R[0].N).normalized();
	for (int p = 0; p < parts; p++)
	{
		Matrix4f G_BEZ(P[p * 3 + 0][0], P[p * 3 + 1][0], P[p * 3 + 2][0], P[p * 3 + 3][0],
					   P[p * 3 + 0][1], P[p * 3 + 1][1], P[p * 3 + 2][1], P[p * 3 + 3][1],
					   P[p * 3 + 0][2], P[p * 3 + 1][2], P[p * 3 + 2][2], P[p * 3 + 3][2],
					   0, 0, 0, 0);
		Matrix4f M_BEZ(1, -3, 3, -1,
					   0, 3, -6, 3,
					   0, 0, 3, -3,
					   0, 0, 0, 1);
		for (int i = 1; i <= steps; i++)
		{
			int index = p * steps + i;
			float t = float(i) / steps;
			Vector4f T(1, t, t * t, t * t * t);
			Vector4f T_delta(0, 1, 2 * t, 3 * t * t);
			auto temp1 = G_BEZ * M_BEZ * T;
			auto temp2 = G_BEZ * M_BEZ * T_delta;
			R[index].V = Vector3f(temp1[0], temp1[1], temp1[2]);
			R[index].T = -Vector3f(temp2[0], temp2[1], temp2[2]).normalized();
			R[index].N = Vector3f::cross(R[index - 1].B, R[index].T).normalized();
			R[index].B = Vector3f::cross(R[index].T, R[index].N).normalized();
		}
	}
	return R;
}

```

### Bspline曲线绘制
1. VTNB初始化:
$R[0].V=(P_0+4P_1+P_2)/6 \\
R[0].T=(P_2-P_0)/\|P_2-P_0\| \\
R[0].N=(0,0,1)\times R[0].T \\
R[0].B=R[0].T\times R[0].N$

2. 递推公式:
$\begin{aligned}
R[t].V&=[P_{t-1},P_t,P_{t+1},P_{t+2}]\cdot M_{BSP}\cdot T \\
&=[P_{t-1},P_t,P_{t+1},P_{t+2}]\cdot\begin{bmatrix}
    1 & -3 & 3 & -1 \\
    4 & 0 & -6 & 3 \\
    1 & 3 & 3 & -3 \\
    0 & 0 & 0 & 0
\end{bmatrix}\cdot\begin{bmatrix}
    1 \\
    t \\
    t^2 \\
    t^3
\end{bmatrix}
\end{aligned}$
$\begin{aligned}
R[t].T&=(R[t].V)'=[P_{t-1},P_t,P_{t+1},P_{t+2}]\cdot M_{BSP}\cdot T' \\
&=[P_{t-1},P_t,P_{t+1},P_{t+2}]\cdot\begin{bmatrix}
    1 & -3 & 3 & -1 \\
    4 & 0 & -6 & 3 \\
    1 & 3 & 3 & -3 \\
    0 & 0 & 0 & 1
\end{bmatrix}\cdot\begin{bmatrix}
    0 \\
    1 \\
    2t \\
    3t^2
\end{bmatrix}
\end{aligned}$
$R[t].N=R[t-1].B\times R[t].T \\
R[t].B=R[t].T\times R[t].N$

```cpp
Curve evalBspline(const vector<Vector3f> &P, unsigned steps)
{
	int parts = P.size() - 3;
	Curve R((parts * steps) + 1);
	R[0].V = (P[0] + P[1] * 4 + P[2]) / 6;
	R[0].T = (P[2] - P[0]).normalized();
	R[0].N = Vector3f::cross(Vector3f(0.f, 0.f, 1.f), R[0].T).normalized();
	R[0].B = Vector3f::cross(R[0].T, R[0].N).normalized();
	for (int p = 0; p < parts; p++)
	{
		Matrix4f G_BSP(P[p + 0][0], P[p + 1][0], P[p + 2][0], P[p + 3][0],
					   P[p + 0][1], P[p + 1][1], P[p + 2][1], P[p + 3][1],
					   P[p + 0][2], P[p + 1][2], P[p + 2][2], P[p + 3][2],
					   0, 0, 0, 0);
		Matrix4f M_BSP(1, -3, 3, -1,
					   4, 0, -6, 3,
					   1, 3, 3, -3,
					   0, 0, 0, 1);
		M_BSP /= 6;
		for (int i = 1; i <= steps; i++)
		{
			int index = p * steps + i;
			float t = float(i) / steps;
			Vector4f T(1, t, t * t, t * t * t);
			Vector4f T_delta(0, 1, 2 * t, 3 * t * t);
			auto temp1 = G_BSP * M_BSP * T;
			auto temp2 = G_BSP * M_BSP * T_delta;
			R[index].V = Vector3f(temp1[0], temp1[1], temp1[2]);
			R[index].T = Vector3f(temp2[0], temp2[1], temp2[2]).normalized();
			R[index].N = Vector3f::cross(R[index - 1].B, R[index].T).normalized();
			R[index].B = Vector3f::cross(R[index].T, R[index].N).normalized();
		}
	}
	return R;
}
```

## Project 1.1 曲线的绘制 - 效果展示

![core.png](https://s2.loli.net/2024/04/14/5n2BHzMILUKtG1e.png =800x)



## Project 1.2 曲⾯的绘制 - 代码实现

### 旋转曲面绘制
$P'=M\times P
=\begin{bmatrix}
\cos\theta & 0 & \sin\theta & 0 \\
0 & 1 & 0 & 0 \\
-\sin\theta & 0 & \cos\theta & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}\times P$
$N'=(M^{-1})^T\times N=M\times N$

```cpp
Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    for (unsigned p = 0; p < profile.size(); p++)
    {
        for (unsigned i = 0; i <= steps; i++)
        {
            float rad = float(i) / steps * 2 * M_PI;
            Matrix3f rMat = Matrix4f::rotateY(rad).getSubmatrix3x3(0, 0);
            Vector3f vVec = rMat * Vector3f(profile[p].V[0], profile[p].V[1], profile[p].V[2]);
            Vector3f nVec = rMat * profile[p].N;
            surface.VV.push_back(vVec);
            surface.VN.push_back(-1 * nVec);

            if (p != profile.size() - 1)
            {
                // 顺时针构造三角形
                int index = p * (steps + 1) + i;
                int next_index = index + (steps + 1);
                surface.VF.push_back(Tup3u(index, next_index, index + 1));
                surface.VF.push_back(Tup3u(index + 1, next_index, next_index + 1));
            }
        }
    }
    return surface;
}
```

### ⼴义圆柱体绘制
$P'=M\times P
=\begin{bmatrix}
N & B & T & V \\
0 & 0 & 0 & 1
\end{bmatrix}\times P$
$N'=(M^{-1})^T\times N$


> 曲⾯的闭合问题
> 插值扫描曲线的开始和结束之间的旋转差值，
> 并将其添加到沿着曲线的坐标系中
> 起始法向量与结束法向量的夹⻆$\alpha=-\arccos(N_1\cdot N_2)$
> Rodrigue旋转公式: $v_{rot}=v\cos\theta+(1-\cos\theta)(k\cdot v)k+(k\times v)\sin\theta$
> 令$v=N, k=T$
> $\implies N_{rot}=N\cos\alpha+(T\cdot N)(1-\cos\alpha)T+(T\times N)\sin\alpha$
> $\implies N_{rot}=N\cos\alpha+0+(B)\sin\alpha=N\cos\alpha+B\sin\alpha$
> 同理可得: $B_{rot}=-N\sin\alpha+B\cos\alpha$
> 在遍历sweep的过程中将法向量NB逐步旋转, 使得曲面首尾相接

<br>

```cpp
Surface makeGenCyl(const Curve &profile, const Curve &sweep)
{
    // 计算初始法向量与终止法向量的夹角
    float alpha = -acos(Vector3f::dot(sweep.front().N, sweep.back().N));

    Surface surface;
    for (unsigned p = 0; p < profile.size(); p++)
    {
        for (unsigned i = 0; i < sweep.size(); i++)
        {
            // 插值旋转法向量
            auto rad_delta = alpha * i / (sweep.size() - 1);
            auto rotate_N = (cos(rad_delta) * sweep[i].N + sin(rad_delta) * sweep[i].B).normalized();
            auto rotate_B = (-sin(rad_delta) * sweep[i].N + cos(rad_delta) * sweep[i].B).normalized();
            Matrix4f rMat(rotate_N[0], rotate_B[0], sweep[i].T[0], sweep[i].V[0],
                          rotate_N[1], rotate_B[1], sweep[i].T[1], sweep[i].V[1],
                          rotate_N[2], rotate_B[2], sweep[i].T[2], sweep[i].V[2],
                          0, 0, 0, 1);
            Vector4f temp = rMat * Vector4f(profile[p].V[0], profile[p].V[1], profile[p].V[2], 1);
            Vector3f vVec = Vector3f(temp[0], temp[1], temp[2]);
            Matrix3f rMat_N = rMat.getSubmatrix3x3(0, 0).transposed().inverse();
            Vector3f nVec = rMat_N * profile[p].N;
            surface.VV.push_back(vVec);
            surface.VN.push_back(-1 * nVec);

            if (p != profile.size() - 1)
            {
                // 顺时针构造三角形
                int index = p * sweep.size() + i;
                int next_index = index + sweep.size();
                surface.VF.push_back(Tup3u(index, next_index, index + 1));
                surface.VF.push_back(Tup3u(index + 1, next_index, next_index + 1));
            }
        }
    }
    return surface;
}
```

## Project 1.2 曲⾯的绘制 - 效果展示

![tor.png](https://s2.loli.net/2024/04/14/DJGeodmpjhQKxFf.png =800x)

![florus.png](https://s2.loli.net/2024/04/14/CtydGcnM436lJP1.png =800x)

![flircle.png](https://s2.loli.net/2024/04/14/Zdn2wY3HEui1ITy.png =800x)

![wineglass.png](https://s2.loli.net/2024/04/14/ndaC7BSFlp4XzmP.png =800x)

![norm.png](https://s2.loli.net/2024/04/14/d1OcDhWZCVJoRrT.png =800x)


## Project 1 拓展 曲⾯的闭合问题 - 效果展示

![weird.png](https://s2.loli.net/2024/04/14/1bcHf5CZVnQm6SU.png =800x)

![weirder.png](https://s2.loli.net/2024/04/14/KFgPUSq5WvwTGIz.png =800x)
