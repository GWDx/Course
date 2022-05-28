## <center>并行计算	实验三</center>

<center>甘文迪	PB19030801</center>

### 1. 问题描述

输入一个 $m\times n$ 稀疏矩阵 M（非零元比例 <1%）、一个 $n\times1$ 向量 V，求两者的乘积 MV 。
参考现有的并行稀疏矩阵向量乘法（搜索关键词：parallel SpMV）实现，或者自行设计一种并行稀疏矩阵向量乘法实现。使用 MPI 实现。

> 稀疏矩阵用三元组表示，三元组中三个数分别表示行号、列号和非零元值，三元组之间以空格分隔。行号和列号从 1 开始，保证行号和列号均不重复。假设输入数据有序。



### 2. 算法设计

#### 2.1 问题分析

查阅资料，计算稀疏矩阵与向量的乘积的算法有 CSR、CSB 等。

CSR 简明易行

CSR 的思路如下：
对于矩阵中的非零值 $A_{i,j}$，找到对应的向量中的元素 $X_j$，计算乘积，为结果中的 $Y_i$ 加上这个乘积。



#### 2.2 算法描述

```python
CSR_SPMV(A, x, y)
    n = A.rows
    for i = 0 to n-1 parallel
        y[i] = 0;
        for k = A.rowPtr[i] to A.rowPtr[i + 1] - 1
            y[i] = y[i] + A.val[k] * x[A.colIndex[k]];
```

> `rowPtr[i]` 表示首个行号为 i 的三元组的下标。
> `val[k]` 表示第 k 个三元组的非零元值。
> `colIndex[k]` 表示第 k 个三元组的列号。





### 3. 实验评测

#### 3.1 实验配置

##### 3.1.1 软硬件配置

+ CPU：Intel i5-9400
+ 内存：16GB
+ 操作系统：Ubuntu 20.04.4 LTS x86_64
+ OpenMP：OpenMP 4.5
+ 编译器：g++ 9.4.0

命令

```bash
mpic++ -O2
```

##### 3.1.2 数据集配置

稀疏矩阵数据格式参考 https://sparse.tamu.edu/

测试数据可以由 `generate.cpp` 生成
行号、列号分别是小于等于 m, n 的整数，非零元值是浮点数。非零值是 [1, 100] 间的整数。



#### 3.2 实验结果

##### 3.2.1 正确性验证

<img src="/home/gwd/文档/大三下/并行计算/实验/3/image/1.png" alt="1" style="zoom:50%;" />

产生 1000 个顶点和 100 条有向边

并行计算与使用定义计算结果相同



##### 3.2.2 加速比分析

###### 通信方式

<img src="/home/gwd/文档/大三下/并行计算/实验/3/image/2.png" alt="2" style="zoom:50%;" />

>
> 设三元组的数量为 `notZeroCount`
>
> `CSR_SPMV` 算法串行的时间复杂度与 `notZeroCount` 成正比
>
> 而进程间通信传输的时间也与 `notZeroCount` 成正比
>
> 因此，使用通信的方式加速不理想
>

若忽略将稀疏矩阵传递给子进程的时间，则加速明显



###### 共享内存方式

> MPI 支持共享内存

另外也可以使用共享内存的方式，并行加速效果明显（考虑将结果传回）

<img src="/home/gwd/文档/大三下/并行计算/实验/3/image/3.png" alt="3" style="zoom:50%;" />

| 处理器数 | 运行时间（秒） | 加速比 |
| :------: | :------------: | :----: |
|    1     |   0.0130609    |        |
|    2     |   0.00797161   |  1.64  |
|    3     |   0.00560819   |  2.33  |
|    4     |   0.00476244   |  2.74  |



<!--3.2.3 可扩放性分析
3.2.4 工作量效率分析
3.2.5 消融实验
3.2.6 性能建模-->



### 4 结论

CSR 是一种简单的稀疏矩阵向量乘法的解法。

使用通信方式并行此问题效果不理想，原因是通信代价高昂。条件允许时使用共享内存或许可以获得较好的并行效果。



<!--5 致谢-->

### 6 参考文献

+ Aydin Buluç, Jeremy T. Fineman, Matteo Frigo, John R. Gilbert, Charles E. Leiserson
  Parallel Sparse Matrix-Vector and Matrix-Transpose-Vector Multiplication Using Compressed Sparse Blocks
  DOI: 10.1145/1583991.1584053
