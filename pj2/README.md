# 1. 项目概述

---

### 1.1 项目背景

本项目是一个名为“文字王国历险记”的文字控制台游戏，使用 C 语言开发实现关卡冒险与寻宝体验。

### 1.2 项目目标

本项目在pj1的基础上进行完善

- **新增目标**
    - 从文件加载地图
    - 实现撤销与恢复
    - 可保存进度，并在开始界面显示存有进度的关卡
- **扩展目标**
    - 提供彩色地图
    - 拥有计时功能。

# 2. 技术架构

---

- **操作系统**：Windows 11
- **集成开发环境（IDE）**：Visual Studio , MSVC 编译器
- **编程语言**：C语言
- **库**：标准C库

# 3. 新增功能描述与实现

---

### 3.1. 撤销

- 玩家通过`z`键控制可实现撤销功能
- 按下`z`后，玩家回到上一步位置，同时宝藏、体力值等也回到上一步的数值
- 输入`z`后输入合法移动，则撤销操作清空
- **实现：**
    - 当玩家按下`z`后，调用`press_z()`函数，将链表中最后一节表元做标记（`tail->connected=1`），同时将记录玩家最后一步真实动向的指针`tail`向前移一位，更新数据与地图
    - 创建新表元时先调用`delete()`函数，切断`tail`后表元（即`tail->connected=1`的表元）的连接，并释放有标记的表元的空间，即实现在撤销后有效操作时清空撤销的操作

### 3.2. 恢复

- 玩家通过`y`键控制可实现恢复功能
- 即如果按`y`之前有撤销操作，则恢复至撤销前状态
- **实现**：
    - 玩家按下`y`后，调用`press_y()`函数，取消`tail`后一个表元的标记（`tail->connected=0`），并更新`tail`

### 3.3. 从文件中加载地图

- 读取exe所处文件夹下的maps文件夹中的地图数据并显示
- **实现**：调用`ReadMap(const char* filename, int map[50][50], int* length, int* width,int*x,int*y)`函数，更新地图长宽、小黄位置等数据

### 3.4. 进度保存

- 按下`q`键即保存进度
- 在欢迎界面出现<上次>提示
- **实现**：
    - 调用`Savedata(const char* filename)`函数，遍历链表，将每个表元的数据依次存入exe所处文件夹下的`save_data`文件中（若无文件则自动新建）
    - 利用变量`selection_level` 记录所选关卡，`selection_mode`记录所选模式，并在欢迎界面所对应关卡后显示<上次>
    - 玩家在过渡页面选择“是”时，读取进度，即调用`Readdata(const char* filename)` 函数，读取所存储的每个表元的数据并连成链表，并令`tail`指向最后一个无标记的表元（`tail->connected==0`）
    - 玩家在过渡页面选择“否”时，清空已存储的进度，即调用 `Clearfile("save_data")` 函数，后将`tail` 与`data`初始化，返回开始页面

# 4. 代码结构

---

## 4.1 项目目录结构

```bash
PROJECT2
├── main.c
├── main.exe
├── save_data.txt
└── maps
    ├── 平凡之路.txt
    ├── 康庄大道.txt
    └── 魔王之旅.txt
```

- **`main.c`**该文件是程序的入口文件，包含了所有的逻辑实现
- `save_data` 是存放进度的文件
- `maps` 文件夹用于存放地图

## 4.2 文件结构详细说明

### >>头文件与宏定义

- 使用头文件 `stdio.h`、`stdlib.h`、`conio.h`、`windows.h` 和 `time.h`。
- 定义地图颜色及辅助宏

### >>定义结构体 用于记录玩家信息与数据

```c
typedef struct {
    int steps;//玩家步数
    int x, y;//玩家坐标
    int foundtreasure_num; //找到的宝藏数量
}Player;
typedef struct Op{
    Player player;
    char move; //玩家路径记录
    int foundtreasure[4][2]; //已找到的宝藏的坐标
    int nextsteps; //下一步所需的步数
    int treasure; //总宝藏数
    int connected;//标记 表示是否连接在链表中
    struct Op* prev;
    struct Op* next;
}Op;
struct Op* head;
struct Op* tail;
typedef struct Data {
    int selection_level,selection_mode; //记录所选关卡与模式
    char currenttime[50]; //记录保存进度时的时间
    Op* head;
}Data;
Data *data;
```

### >>定义函数

- **完成特定功能类函数**

```c
/*初始化*/
void initialize(Op* tail)
/*渲染地图*/
void rendermap(int map[50][50], int length, int width)
/*读取文件地图*/
void ReadMap(const char* filename, int map[50][50], int* length, int* width,int*x,int*y)
/*判断是否找到宝藏 并存入数组*/
void iftreasure(int width, int map[50][50])
/*记录每一步的路径*/
void Move(char moving)
```

- **记录玩家状态相关**

```c
/*清空所有打标记的表元*/
void delete()
/*创建表元*/
Op* create_op(int newsteps, int xplus, int yplus, int nextsteps, int width, int map[50][50])
/*撤销——标记最后一个表元*/
void press_z()
/*按下y——取消标记一个表元*/
void press_y()
```

- **保存进度相关**

```c
/*获取当前时间*/
void gettime()
/*记录进度——存入文件*/
void Savedata(const char *filename)
/*读取进度——从文件里读出*/
void Readdata(const char* filename)
/*清空文档*/
void Clearfile(const char* filename)
```

- **控制移动相关**

```c
/*小黄的移动逻辑与体力计算 实时模式*/
void realtime_mode(int length, int width, int map[50][50],int*judgeover)
/*小黄的移动与体力计算 编程模式*/
void programming_mode( int length, int width, int map[50][50],int *judgeover)
```

- **显示相关**

```c
/*欢迎界面*/
int welcoming()
/*加载页面*/
int continue_playing()
/*过渡界面*/
int mode()
/*结算页面*/
void finish(int *judgeover, DWORD *starttime, DWORD *endtime)
```

### >>主函数

主函数包含程序整体逻辑循环，实现关卡选择、模式选择以及地图渲染、游戏结算等功能。

# 5.思考题

### 1.为什么使用Player而不是player指针？

Op存放并记录的是玩家的真实数据，而不是数据的地址

### 2.c语言函数的作用有哪些？

1. 模块化与结构化：函数将复杂的任务分解成更小、更易于管理的部分。每个函数执行一个特定的任务，这有助于代码的组织和维护。
2. 使代码更简洁：通过定义函数，可以在程序的不同部分多次调用同一个函数，而不需要重复编写相同的代码，提高可读性
3. 便于调试和维护：函数可以独立于程序的其他部分进行测试和调试，查找和修复错误变得更加容易，有利于代码的组织和维护。
4. 提高性能：函数可以帮助编译器更好地优化代码。

### 3.假设游戏里有一个会自己周期性移动的NPC，怎么刷新界面效果

在主函数中添加一部分，利用条件判断，如果时间到达规定周期则更新npc位置与地图

```c
    time_t lastUpdateTime = time(NULL);  // // 获取程序启动时间

    while (1) {
        time_t currentTime = time(NULL);   // 获取当前时间
        // 如果超过了周期，则更新 NPC
        if (currentTime - lastUpdateTime >= 1) {
            Move_npc();// 更新 NPC 的位置的函数
            rendermap();
            lastUpdateTime = currentTime;  // 更新最后一次更新时间
        }
        //其余用于响应用户输入的代码
    }
```

### 4.结构化UI一定好吗？有没有被迫做出非结构化设计的情况？

不一定，比如界面过于标准化，缺乏创新性和灵活性等。

当有个性化需求（例如自定义、艺术表达）时，结构化UI便不再适用

## 6.注

在调试过程中，我发现地图<魔王之旅>第19行所标出的部分若为1（如第一张图），则打印后的地图小黄没有可以到达宝藏的路径（如第二张图），便把图中所示位置的值改为0，以便游玩（第三张图）

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/394e37d9-3ee9-4780-a315-d4d2b39eb270/adac3fd8-75c4-4581-b5a3-b889d70e9b9a/image.png)

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/394e37d9-3ee9-4780-a315-d4d2b39eb270/b8f69451-7c92-4d73-b085-03587448f850/image.png)

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/394e37d9-3ee9-4780-a315-d4d2b39eb270/1863ed15-2a8e-4434-bcef-f5b948fc0a79/image.png)
