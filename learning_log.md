# 学习文档



## level 1.1 1.2
### 1.合并复用设计思路：
- 将 print 和 checkout 中的 print 合并，核心逻辑是“打印商品清单”的动作是重复的，区别是，只有 checkout 时才需要打印表头和打印总价。
- 用 ifReceipt bool判断开关：
         print命令 -> printCart（cart， false）
         checkout命令 -> printCart (cart,true)
- 缺点：逻辑没有那么清晰

### 2.分离数据设计模式：
- cart 的 key 为 string id，而 value 为int qty。为什么不用 map<string, Product> 来当购物车？
- catalog 存放商品静态信息（id， name， price）
  cart 存放商品动态信息 （id，qty）
- string id 来连接 cart 和 catalog， 节省内存，保证数据准确性，简洁高效

### 3.数据流向和转换
- 硬盘上的CSV文件(文本) -> 读入内存(需要变成数字计算) -> 重新写入硬盘(变回文本) 使用stoi stod


## level 1.3
### 1.拆解需求：
- 时间处理
- 数据结构
- 持久化文件
  
### 2.怎么设计内存中的数据结构？
- 每一笔交易需要五种属性（Day No. Time Items Amount），需要用结构体来代表“一笔交易”
- 一旦checkout后，交易就固定了，所以直接把它拼成一个字符串存下来，商品明细用 string itemsStr, 而不是 map<string, int> cart
- 用动态数组 vector 把所有的vector都装进去

### 3.设计持久化文件
- ofstream 打开文件，模式选择ios::app追加 
- 在main 函数一开始读，把文件内容全部读进salesHistory 这个vector里
  
### 4.怎么管理currentDay？
- 需要全局变量 int currentDay = 1 来记录现在是第几天
- 如果程序重启了，读文件时发现里面最大的Day 是2，那么currentDay 也要自动更新为2

### 5.流水号怎么算？
- 第二天的流水号应从1开始算；遍历salesHistory，统计 day == currentDay的记录数量，然后 +1

### 6.数据结构思考总结：
看输出 -> 定结构 -> 选容器(单条记录用struct，多条记录用vector<struct>) -> 做持久化 -> 想细节

## level 2.1
- common.h（公共头文件）：存放结构体（Product、SaleRecord）和所有全局变量（catalog、isAdminMode 等）的声明。
- admin.h 和 admin.cpp（管理员模块）：存放管理员模式的所有逻辑。
- main.cpp（主程序）：只负责读取数据、开启主循环、并在主循环里根据模式调用不同的函数。