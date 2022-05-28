## <center>并行计算	实验二</center>

<center>甘文迪	PB19030801</center>

### 1. 问题描述

输入一个带非负边权的图 G、一个源节点，求该节点到图中所有节点的最短路径长度。

> 图 G 用顶点数 V、边数 E、各条边的顶点及权重进行表示



### 2. 算法设计

#### 2.1 问题分析

查阅资料，并行计算单源最短路径常见的算法有 ∆-stepping 算法、Radius stepping 算法等。

∆-stepping 算法每次迭代对边缘上的点进行松弛操作，从而计算得到单源最短路径



#### 2.2 算法描述

> 以下伪代码来自 ∆-stepping: a parallelizable shortest path algorithm

```python
foreach v ∈ V do tent(v) := ∞
relax(s, 0);
while ¬isEmpty(B) do
    i := min{j >= 0: B[j] != ∅}
    R := ∅
    while B[i] != ∅ do
        Req := findRequests(B[i], light)
        R := R ∪ B[i]
        B[i] := ∅
        relaxRequests(Req)
    Req := findRequests(R, heavy)
    relaxRequests(Req)

Function findRequests(V', kind : {light, heavy}) : set of Request
    return {(w, tent(v) + c(v, w)): v ∈ V' ∧ (v, w) ∈ Ekind)}

Procedure relaxRequests(Req)
    foreach (w, x) ∈ Req do relax(w, x)

Procedure relax(w, x)
    if x < tent(w) then
        B[tent(w)/∆] := B[tent(w)/∆] \ {w}
        B[x/∆] := B[x/∆] ∪{w}
        tent(w) := x
```

代码中第一个 `foreach` 无需并行，这部分串行执行的时间是 $O(V)$，可以接受

而第二个 `foreach` 需要避免线程同时从 B 中增加或删除元素

```c++
void parallelRelax(int w, int x) {
    if (x < tent[w]) {
        int index = tent[w] / delta;
        if (index < lenB)
            B[index][w] = false;
        index = x / delta;

#pragma omp critical
        B[index][w] = true;
        tent[w] = x;
    }
}
```

> 将 `B[index][w]` 标记为 `false` 而非将其删除是为了避免 `erase` 引起复杂的操作
>
> 插入使用临界区 `#pragma omp critical`

并且，当 Req 较大时这一段才有并行的必要，代码中设置超过 50 才尝试并行



### 3. 实验评测

#### 3.1 实验配置

##### 3.1.1 软硬件配置

+ CPU：Intel i5-9400
+ 内存：16GB
+ 操作系统：Ubuntu 20.04.4 LTS x86_64
+ OpenMP：OpenMP 4.5
+ 编译器：g++ 9.4.0

命令行参数



##### 3.1.2 数据集配置

使用 PaRMAT 产生各条边
边权重在 [10,100) 内随机产生



#### 3.2 实验结果

##### 3.2.1 正确性验证

<img src="/home/gwd/文档/大三下/并行计算/实验/2/image/1.png" alt="1" style="zoom:50%;" />

产生 1000 个顶点和 10000 条有向边

并行计算的结果与 dijkstra 的结果一致

> dijkstra 的复杂度有些奇怪



##### 3.2.2 加速比分析

产生 2000000 个顶点和 100000000 条有向边

<img src="/home/gwd/文档/大三下/并行计算/实验/2/image/2.png" alt="2" style="zoom:50%;" />

| 处理器数 | 运行时间 | 加速比 |
| :------: | :------: | :----: |
|    1     |   7.08   |        |
|    2     |   6.33   |  1.12  |
|    3     |   6.21   |  1.14  |

处理器数增加时，加速比有所增大尽量避免临界区操作

这可能和临界区操作有一定关系

<!--3.2.3 可扩放性分析
3.2.4 工作量效率分析
3.2.5 消融实验
3.2.6 性能建模-->

### 4 结论

∆-stepping 算法可以进行一定程度的并行，不过实现不理想

了解了 ∆-stepping 算法
学习了并行算法的设计





<!--5 致谢-->

### 6 参考文献

+ https://en.wikipedia.org/wiki/Parallel_single-source_shortest_path_algorithm
+ U. Meyer and P. Sanders
  ∆-stepping: a parallelizable shortest path algorithm
  doi:10.1016/S0196-6774(03)00076-2
