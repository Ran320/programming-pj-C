# 1. 项目概述

---

### 1.1 项目背景

本项目是一个名为“文字王国历险记”的文字控制台游戏，使用 C 语言开发实现关卡冒险与寻宝体验。

### 1.2 项目目标

本项目的目标包括：

- **核心目标**
    - 关卡与模式的选择。
    - 初始化和渲染地图。
    - 实现小黄的正确移动及记录体力与路径。
    - 判断宝藏的获取及游戏结束条件，并更新地图状态。
    - 在结算页面展示完整的游戏统计数据。
- **扩展目标**
    - 提供彩色地图和计时功能。
    - 实时模式支持撤销操作。

# 2. 技术架构

---

- **操作系统**：Windows 11
- **集成开发环境（IDE）**：Visual Studio , MSVC 编译器
- **编程语言**：C语言
- **库**：标准C库

# 3. 功能描述

---

### 3.1. 关卡选择

- 玩家通过w、s键控制光标，使用`getch()`获取用户输入，更新光标位置
- 玩家按下enter时选择关卡

### 3.2. 模式选择

- 玩家通过w、s键控制光标，使用`getch()`获取用户输入，更新光标位置
- 玩家按下enter时选择模式

### 3.3. 初始化并渲染彩色地图

- **地图说明**：

| **符号** | **含义** |
| --- | --- |
| **Y** | 小黄（玩家） |
| **W** | 墙 |
| **D** | 陷阱 |
| **T** | 宝藏 |
- **功能细节**：
    1. 使用二维数组初始化地图，通过 `rendermap()` 函数渲染地图。
    2. 每次玩家操作后实时更新地图状态。

### 3.4. 小黄的移动

- **实时模式**：

| **按键** | **功能** |
| --- | --- |
| w/a/s/d | 控制方向移动 |
| i | 原地不动 |
| q | 结束冒险 |
| z | 撤销上一步操作 |
| 非法输入 | 显示“输入错误，请重新输入。” |
- **编程模式**：
    
    玩家依次输入完整的路径指令，按 Enter 键执行。非法输入会结束游戏。
    

### 3.5. 体力与路径记录

- 每次移动（包括撞墙）或原地不动会消耗 1 点体力
- 玩家所有操作路径记录在字符数组 `paths` 中，并在结算页面展示。

### 3.6. 寻宝成功的判断

- 玩家移动到宝藏上下左右任意方向时视为寻宝成功，宝藏会从地图中消失。
- 地图中所有宝藏被找到后，游戏自动结束。

### 3.7. 结算页面

玩家游戏结束后，结算页面上会显示游戏结束原因、行动路径、消耗总体力、找到宝藏数、时间，并可按任意键回到主页面

### 3.8. 附加功能

- **撤销操作**：实时模式下按 z键可撤销上一步操作，小黄的位置和体力恢复至上一步状态。
- **计时功能**：记录游戏时间，并展示在结算页面中。

# 4. 代码结构

---

## 4.1 项目目录结构

```bash
/PROJECT1
    └── main.c
```

- **main.c**该文件是程序的入口文件，包含了所有的逻辑实现。

## 4.2 文件结构详细说明

### >>头文件与宏定义

- 使用头文件 `stdio.h`、`stdlib.h`、`conio.h`、`windows.h` 和 `time.h`。
- 定义地图颜色及辅助宏

### >>定义全局变量

```c
//全局变量
int steps = 0;//记录步数
char paths[1000];//记录路径
int i = 0;
int treasure = 0, foundtreasure = 0;//宝藏数与找到的宝藏数
int judgeover = 0;
DWORD starttime, endtime; //计时功能
```

### >>定义函数

- **界面展示**

```c
int welcoming()  //欢迎界面（主页面）
int mode()       //过渡界面
void finish()    //结算界面
```

- **初始化与渲染地图**

```c
void map01()  //初始化地图1
void map02()  //初始化地图2
void rendermap(int* map, int length, int width, int x, int y) //用ASCII字符渲染地图
```

- **小黄的移动**

```c
void path()    //记录行动路径
void movexiaohuang(int* x, int* y, int length, int width, int* map)   //实时模式小黄的移动逻辑与体力计算
void movexiaohuang1(int* x, int* y, int length, int width, int* map)  //编程模式小黄的移动逻辑与体力计算
void into_z(int x, int y, int steps)      //存储撤销数据
void outof_z(int* x, int* y, int* steps)  //恢复撤销数据
```

- **判断是否找到宝藏**

```c
void iftreasure(int* x, int* y, int width, int* map) 
```

### >>主函数

主函数包含程序整体逻辑循环，实现关卡选择、模式选择以及地图渲染、游戏结算等功能。

# 5. 使用说明

---

### 5.1. 操作方法

| 模式 | 操作 | 说明 |
| --- | --- | --- |
| 实时模式 | w/a/s/d | 控制小黄移动方向 |
|  | i | 原地不动 |
|  | q | 结束冒险 |
|  | z | 撤销操作 |
| 编程模式 | 路径指令序列 | 一次性完成指令 |

### 5.2. 游戏规则

- 每步消耗 1 体力，撞墙或原地不动也会扣除 1 体力。
- 正常情况下找到所有宝藏时游戏结束。

### 5.3. 模式差异总结

- 实时模式：即时更新小黄位置，支持计时与撤销功能，有非法输入时会提示，但不影响游戏继续进行。
- 编程模式：玩家输入完整指令序列后，按下确认键一次性执行所有操作，支持计时功能，不支持撤销，且有非法输入时游戏停止。
