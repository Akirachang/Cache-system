# <center>cache系统实验</center>

<center>姓名：常晟 学号：2017080064</center>

## 1.实验目的：

​	本实验要求同学们使用 C/C++编写一个 Cache 替换策略模拟器。输入为存储器访问 trace，
输出为在不同的条件下(如 Cache 块大小，组织方式替换策略，不同的写策略等)，在给定
trace 上的缺失率，以及访问 log(记录命中和缺失的结果)。希望同学们能够深入理解不同
的 Cache 替换策略，动手实现不同的 Cache 替换策略，同时观测不同的替换策略对于程序运
行性能的影响。

## 2.Cache替换算法的算法

### 1.LRU替换算法

也称least recently used，是将cpu里面最少用到的块儿替换出去。要求一直都记录并更新cache块的状态，来确定替换出cache当中的哪一块。这就需要用到一个index来计数，然后命中的话清0，没有则加一。这是我想到最有效的办法，代码当中的insert_cache里面有用到。

```c++
void CacheSimulator::insert_cache(double_long setBase, double_long index, int swapStyle){
    double rand;
    switch(swapStyle){
        case LRU:
                    for (double_long j = 0; j < mapWays; ++j) {
                        if ((caches[setBase + j].LRU < caches[setBase + index].LRU) && (caches[setBase + j].flag & FLAG_VALID)) {
                            caches[setBase + j].LRU ++;
                        }
                    }
                    caches[setBase + index].LRU = 0;
                    break;
    }
}
```

### 2.随机替换

随机替换是最快的一种替换，他就是完全不管cache的块状态，直接生成一个随机数，然后把数字对应的块替换掉，速度绝对比lru要快，但是效率和性能上绝对低很多。

## 3.测试方法

直接到文件路径下时，输入make run就可以让main.cpp文件跑起来。然后结果会在terminal里面输出出来，也会保存到output文件夹下面的两个txt文档里面。随后我还写了一份python脚本，用来生成cache命中和缺失的图像。代码如下：

```python
import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import array as arr

a = arr.array('d', [])
b = arr.array('d', [])
c = arr.array('d', [])

f=open("output/RANDoutput.txt","r")

line=f.readlines()
for x in line:
    txt = x
    temp = txt.split(",")
    a.append(float(temp[0]))
    b.append(float(temp[1]))
    c.append(float(temp[2]))
    print(x)

# x = np.linspace(0, 20, 100)

plt.plot(a,b)
plt.plot(a,c)
plt.show() 
```

实验要求的log数据在Log文件夹里面。

## 4.实验数据分析

| ![LRU8-1-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU8-1-ASTAR.png) | ![LRU8-4-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU8-4-ASTAR.png) | ![LRU8-8-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU8-8-ASTAR.png) |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![LRU32-1-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU32-1-ASTAR.png) | ![LRU32-4-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU32-4-ASTAR.png) | ![LRU32-8-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU32-8-ASTAR.png) |
| ![LRU64-1-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU64-1-ASTAR.png) | ![LRU64-4-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU64-4-ASTAR.png) | ![LRU64-8-ASTAR](/Users/akirachang/Desktop/CacheSimulator/graphs/LRU64-8-ASTAR.png) |

以上是使用python脚本得到LRU替换算法的结果图。蓝色线为命中率，绿色线为缺失率。最左边为直接关联，后两个分别为4，还有8路关联，且第一排的块大小为8B，下面是32B，最下面是64B 。而且由于数据过多，还有为了节约时间，我只用astar进行测试，且用了lru替换算法。结果得出，使用直接关联时，命中率特别低，而且很难上升，缺失率特别高，而且线路相对平稳。跟我预期的一样，可以看出，4路还有8路的命中率会比起直接关联会高很多，直接关联的命中率在百分之20一下左右，但是4路还有8路的有50到60之间。 4路和8路的结果差不多，可以总结的是它们两个对于cache的命中率没有大幅度的变化。

### 不同trace文件下替换算法的结果

| astar.trace                                                  | Bzip2.trace                                                  | Mcf.trace                                                    | Perlbench.trace                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![astar](/Users/akirachang/Desktop/CacheSimulator/trace results/astar.png) | ![bzip2](/Users/akirachang/Desktop/CacheSimulator/trace results/bzip2.png) | ![mcf](/Users/akirachang/Desktop/CacheSimulator/trace results/mcf.png) | ![perlbench](/Users/akirachang/Desktop/CacheSimulator/trace results/perlbench.png) |

我是用8B块大小，8路组相连来测试不同trace的结果，发现除了astar.trace之外的图都相对平稳。astar的命中率一开始很不稳，且比较低，比缺失率低。然后之后就逐渐慢慢下降。其他三个的命中率都要比缺失率高，而且相对平稳尤其是mcf。在前几千行命令慢慢上升，然后到了差不多80以上就保持在那个值上面。bzip2和perlbench也类似，但是它们一开始很不稳，但之后就慢慢稳下来了。

## 5.实验结果分析  

### 1.LRU替换策略：

lru会一直记录cache的状态，比较耗容量，空间还有时间。它主要就是记录最近使用的块。本人使用了计数器的方式来记录使用状态，这样可以比较快的得到lru所需要的块结果。但是比较耗时间因为命中时需要改变cache每一个块当中的计数器的值。结果如上面所示，一开始会有一个下降，不是很稳定，但是到了后面的命令之后，整张图就会慢慢平稳下来，而且命中率慢慢上升。

### 2.随机替换算法：

随机替换算法就是会随机选择cache里面的块，然后进行一个替换的操作。这个算法虽然比较简单，但是实现的速度和时间都要比lru要快。但是结果发现对于命中率没有任何改进，其实跟lru差不多，没有什么变化。

### 3.二叉树替换算法：

最后结果发现，二叉树替换算法的效率最高，而且速度也最快。但是使用了同样的trace，同样的块大小还有组相连的时候，发现结果差不多，没有显著的变化。所以可以得到的结论就是无论是随机，lru还是二叉树替换策略，效率和性能速度方面是肯定有很大的差距的，但是最终得到的结果差不多意味着替换策略不会大大得提高cache的命中率，只会提高性能和速度。

## 6.实验结论

通过本次实验，深入理解了cache不同策略的算法效率，时间等等重要的因素。遇到的困难就是不知从哪里下手，而且对于文档以及c++最基础的位操作基本上都是我的弱点。但是通过朋友还有助教的帮助，完成本次实验，而且收获还算挺多的。