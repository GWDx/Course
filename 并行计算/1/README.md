## <center>并行计算	实验一</center>

<center>甘文迪	PB19030801</center>

### 1. 问题描述

计算自然常数 $e$ 的前 100 位有效数字。

$e$ 用极限可以定义为 $e=\lim\limits_{n\rightarrow\infin}\left(1+\frac{1}{n}\right)^n$，而用无穷级数可以定义为 $e=\sum\limits_{n=0}^{\infin}\frac{1}{n!}$



### 2. 算法设计

#### 2.1 问题分析

使用无穷级数 $e=\sum\limits_{n=0}^{\infin}\frac{1}{n!}$

将循环结束值设为 N，则需要计算 $e=\sum\limits_{n=0}^{N}\frac{1}{n!}$

假设有 p 个处理器（不妨设 N 是 p 的倍数）

$$
\begin{align}
e&=\sum\limits_{n=0}^{N}\frac{1}{n!}
=\sum\limits_{i=0}^{p-1}\sum\limits_{n=\frac{iN}{p}+1}^{\frac{(i+1)N}{p}}\frac{1}{n!}
=\sum\limits_{i=0}^{p-1}\sum\limits_{j=1}^{\frac{N}{p}}\frac{1}{\left(j+\frac{iN}{p}\right)!}\\
&=\sum\limits_{i=0}^{p-1}\frac{1}{\left(\frac{iN}{p}\right)!}\sum\limits_{j=1}^{\frac{N}{p}}\frac{1}{\prod\limits_{k=1}^{j}\left(j+\frac{iN}{p}\right)!}\
\end{align}
$$

使用上式可实现并行



#### 2.2 算法描述

```
for all i : 0 <= i <= p-1 par-do
    partReciprocalFactorial[i] = 1
    partAns[i] = 0

    for j : N/p*i+1 <= j <= N/p*(i+1) do
        partReciprocalFactorial[i] /= j
        partAns[i] += partReciprocalFactorial[i]

ans = 1
prevReciprocalFactorial = 1

for all i : 0 <= i <= p-1 do
    ans += partAns[i] * prevReciprocalFactorial
    prevReciprocalFactorial * partReciprocalFactorial[i]
```

> 为便于阅读，省略了伪代码中的 `endfor`

符合 PCAM 设计方法学

+ 划分。将问题划分成多个子任务
+ 通讯。尽可能减少子任务间的通讯
+ 组合。减少任务数，增加重复计算
+ 映射。子任务数等于处理器数 p 时较优



### 3. 实验评测

#### 3.1 实验配置

##### 3.1.1 软硬件配置

+ CPU：Intel i5-9400
+ 内存：16GB
+ 操作系统：Ubuntu 20.04.4 LTS x86_64
+ OpenMP：OpenMP 4.5
+ 编译器：g++ 9.4.0

命令行参数

```bash
g++ 1.cpp -o 1 -L /usr/share/lib -lgmp -lgmpxx -fopenmp
```

##### 3.1.2 数据集配置

保证十进制下 100 位精度只需要 `N=100`, `precision=400`

而测试运行时间时使用 `N=10000`, `precision=40000`，因为规模太小不能观察到并行加速效果



#### 3.2 实验结果

##### 3.2.1 正确性验证

保留 101 位有效数字的结果是

```
e=2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274
```

结果正确

##### 3.2.2 加速比分析

时间复杂度 $T(n)=\Theta(\frac{N}{p}+p)$

设置 `N=100`, `precision=400` 时，并行反而变慢，这可能是因为操作系统分配线程需要一定时间

![0](/home/gwd/文档/大三下/并行计算/实验/1/image/0.png)



计算  `N=10000`, `precision=40000`，并行明显变快

![1](/home/gwd/文档/大三下/并行计算/实验/1/image/1.png)

| 处理器数 | 运行时间 | 加速比 |
| :------: | :------: | :----: |
|    1     | 0.02408  |        |
|    2     | 0.01277  |  1.89  |
|    3     | 0.00912  |  2.64  |
|    4     | 0.00724  |  3.33  |
|    5     | 0.00594  |  4.05  |
|    6     | 0.00567  |  4.24  |

处理器数增加时，加速比增大，但增加的幅度逐渐减小，这可能与任务分配不均有关

<!--3.2.3 可扩放性分析
3.2.4 工作量效率分析
3.2.5 消融实验
3.2.6 性能建模-->

### 4 结论

用无穷级数计算 e 可以使用并行进行加速

学习了 OpenMP 的使用，学习如何设计并行算法

建议改成计算 e 的更多位数，因为位数较小时无法体现并行效果

<!--5 致谢-->

### 6 参考文献

+ 程艺、陈卿、李平《数学分析讲义》中 e 的定义
+ 陈国良《并行计算 结构·算法·编程》中并行算法设计、并行算法分析的部分
