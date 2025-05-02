# ChronoRail - 时空列车模拟系统

这是一个基于双向链表实现的时空列车编组与调度模拟系统。系统支持列车的编组、调度、存储以及撤销/重做等功能。

## 功能特性

- 基本编组操作（添加、插入、删除车厢）
- 列车遍历（正向/反向）
- 高级列车操作（分裂、合并、区间反转、排序）
- 循环列车支持
- 时空节点操作
- 文件持久化（JSON格式）

## 构建要求

- C++17 或更高版本
- CMake 3.10 或更高版本
- nlohmann/json 库（用于JSON序列化）

## 构建步骤

```bash
mkdir build
cd build
cmake ..
make
```

## 使用说明

程序支持以下命令：

- ADD_FRONT/ADD_BACK：在列车前端/后端添加车厢
- INSERT_AFTER：在指定车厢后插入新车厢
- REMOVE：移除指定车厢
- TRAVERSE：正向或反向遍历列车
- SPLIT：从指定位置拆分列车
- MERGE：合并两列列车
- REVERSE：反转指定区间的车厢
- SORT：按重量或长度排序
- MAKE_LOOP/BREAK_LOOP：创建/断开循环列车
- SAVE/LOAD：保存/加载列车配置

## 测试示例

以下是一个完整的测试流程示例：

1. 创建并添加车厢到列车1：
```bash
ADD_FRONT 1 101 passenger 50.5 20.0  # 添加一个乘客车厢到列车1前端
ADD_BACK 1 102 cargo 80.0 25.0       # 添加一个货运车厢到列车1后端
ADD_BACK 1 103 dining 45.0 18.0      # 添加一个餐车到列车1后端
```

2. 查看列车1的组成：
```bash
TRAVERSE 1 FORWARD                    # 正向遍历列车1
```

3. 在指定位置插入新车厢：
```bash
INSERT_AFTER 1 102 104 sleeper 60.0 22.0  # 在车厢102后插入一个卧铺车厢
```

4. 按重量对车厢排序：
```bash
SORT 1 WEIGHT ASC                    # 按重量升序排序
TRAVERSE 1 FORWARD                    # 查看排序结果
```

5. 创建循环列车：
```bash
MAKE_LOOP 1                          # 将列车1设置为循环列车
```

6. 分裂列车：
```bash
SPLIT 1 103                          # 从车厢103处分裂列车
TRAVERSE 1 FORWARD                    # 查看原列车
TRAVERSE 2 FORWARD                    # 查看新列车
```

7. 反转部分车厢：
```bash
REVERSE 1 101 102                    # 反转列车1中101到102之间的车厢
```

8. 保存当前配置：
```bash
SAVE trains.json                     # 保存当前配置到文件
```

9. 加载配置：
```bash
LOAD trains.json                     # 从文件加载配置
```


### 错误处理示例

系统会优雅地处理各种错误情况：

```bash
ADD_FRONT 1 -1 passenger 50.5 20.0   # 错误：车厢ID不能为负数
REMOVE 1 999                         # 错误：找不到指定ID的车厢
MAKE_LOOP 999                        # 错误：列车不存在
SORT 1 INVALID ASC                   # 错误：无效的排序类型
``` 
