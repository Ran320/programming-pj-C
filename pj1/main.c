#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define RESET_COLOR "\033[0m"
#define YELLOW "\033[33m"//黄色
#define LIGHT_RED "\033[91m" // 浅红色
#define LIGHT_BLUE "\033[94m"  // 浅蓝色
#define LIGHT_CYAN "\033[96m"    // 浅青色

#define getch() _getch()
#define scanf scanf_s

/*初始值*/
int steps = 0;
char paths[1000];
int i = 0;
int treasure = 0, foundtreasure = 0;
int judgeover = 0;
DWORD starttime, endtime;

/*欢迎界面*/
int welcoming() {
    int choice = 1;
    char c_choice;
    while (1) {
        system("cls");
        printf("小黄的奇妙探险！\n");
        printf("\n");
        printf("%s开始第一关\n", (choice == 1) ? "> " : "  ");
        printf("%s开始第二关\n", (choice == 2) ? "> " : "  ");
        printf("%s退出\n", (choice == 3) ? "> " : "  ");
        printf("\n控制方法：按W向上移动，按S向下移动，按<Enter>选择。");
        c_choice = getch();
        if (c_choice == 'w' || c_choice == 'W') {
            if (choice > 1) choice--;
        }
        if (c_choice == 's' || c_choice == 'S') {
            if (choice < 3) choice++;
        }
        if (c_choice == '\r') {
            if (choice == 3) exit(0);
            else return choice;
        }
    }
}

/*过渡界面*/
int mode() {
    int choice1 = 1;
    char c_choice1;
    while (1) {
        system("cls");
        printf("请选择控制模式：\n");
        printf("%s0：实时模式\n", (choice1 == 1) ? "> " : "  ");
        printf("%s1：编程模式\n", (choice1 == 2) ? "> " : "  ");
        printf("\n控制方法：按W向上移动，按S向下移动，按<Enter>选择。");
        c_choice1 = getch();
        if (c_choice1 == 'w' || c_choice1 == 'W') {
            if (choice1 > 1) choice1--;
        }
        if (c_choice1 == 's' || c_choice1 == 'S') {
            if (choice1 < 2) choice1++;
        }
        if (c_choice1 == '\r')
            break;
    }
    return choice1;
}

/*地图1*/
int map1[15][15] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 3, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/*地图2*/
int map2[21][21] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 2, 2, 0, 1, 0, 0, 0, 0, 2, 0, 2, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 2, 0, 1, 0, 0, 2, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 2, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 3, 1, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 2, 1},
    {1, 0, 1, 0, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 2, 2, 1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/*初始化地图1*/
void map01() {
    int tempmap1[15][15] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 3, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            map1[i][j] = tempmap1[i][j];
        }
    }
}

/*初始化地图2*/
void map02() {
    int tempmap2[21][21] = {
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0, 0, 1, 2, 2, 0, 1, 0, 0, 0, 0, 2, 0, 2, 0, 0, 1},
     {1, 1, 1, 1, 1, 0, 1, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
     {1, 0, 2, 0, 1, 0, 0, 2, 1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 1, 0, 1},
     {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1},
     {1, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1},
     {1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
     {1, 0, 1, 0, 1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
     {1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1},
     {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 1, 0, 1, 0, 1},
     {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 1, 0, 1},
     {1, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
     {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
     {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 2, 0, 0, 1, 0, 1},
     {1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
     {1, 0, 1, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 2, 1, 0, 0, 0, 1},
     {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1},
     {1, 0, 1, 0, 1, 3, 1, 0, 0, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 2, 1},
     {1, 0, 1, 0, 1, 0, 1, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1, 0, 1, 0, 1},
     {1, 0, 1, 0, 0, 0, 2, 2, 1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 21; j++) {
            map2[i][j] = tempmap2[i][j];
        }
    }
}

/*渲染地图*/
void rendermap(int* map, int length, int width, int x, int y) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                printf(YELLOW "Y" RESET_COLOR);  // 显示小黄的位置
            }
            else {
                int value = *(map + i * width + j);
                if (value == 0) {
                    printf(" ");
                }
                else if (value == 1) {
                    printf(LIGHT_CYAN "W" RESET_COLOR);
                }
                else if (value == 2) {
                    printf(LIGHT_BLUE "D" RESET_COLOR);
                }
                else if (value == 3) {
                    printf(LIGHT_RED "T" RESET_COLOR);
                }
            }
        }printf("\n");
    }
    printf("体力消耗：%d\n", steps);
    printf("控制方法：按W向上移动，按S向下移动，按A向左移动，按D向右移动，按I原地不动，按Q结束冒险。\n按Z撤销操作\n");
}

/*记录行动路径*/
void path() {
    for (int i = 0; i < 1000; i++) {
        scanf("%c", &paths[i]);
        if (paths[i] == '\r' || paths[i] == '\n') {
            paths[i] = '\0';  // 将换行符替换为字符串结束符
            break;
        }
    }
}

/*判断是否找到宝藏*/
void iftreasure(int* x, int* y, int width, int* map) {
    int dx[] = { 0, -1, 1, 0 };
    int dy[] = { -1, 0, 0, 1 };
    for (int dir = 0; dir < 4; dir++) {
        int nx = *x + dx[dir];
        int ny = *y + dy[dir];
        if (nx >= 0 && ny >= 0 && nx < width && ny < width) {
            if (*(map + ny * width + nx) == 3) {  // 如果是宝藏
                *(map + ny * width + nx) = 0;     // 设置为 0，表示宝藏消失
                foundtreasure++;
            }
        }
    }
}

/*附加：按z撤销*/
typedef struct {
    int x, y, steps;
}movehistory;
movehistory history[1000];
int p = 0;
void into_z(int x, int y, int steps) {
    history[p].x = x;
    history[p].y = y;
    history[p].steps = steps;
    p++;
}
void outof_z(int* x, int* y, int* steps) {
    if (p > 0) {
        p--;
        *x = history[p].x;
        *y = history[p].y;
        *steps = history[p].steps;
    }
}

/*小黄的移动逻辑与体力计算 实时模式*/
void movexiaohuang(int* x, int* y, int length, int width, int* map) {
    char moving;
    while (1) {
        moving = getch();
        int m = 1;
        int xplus = *x, yplus = *y;
        if (moving == 'W' || moving == 'w') {
            yplus--;
            paths[i++] = 'U';// 向上移动
        }
        else if (moving == 'A' || moving == 'a') {
            xplus--;
            paths[i++] = 'L';// 向左移动
        }
        else if (moving == 'S' || moving == 's') {
            yplus++;
            paths[i++] = 'D';// 向下移动
        }
        else if (moving == 'D' || moving == 'd') {
            xplus++;
            paths[i++] = 'R';// 向右移动
        }
        else if (moving == 'Z' || moving == 'z') {
                outof_z(x, y, &steps);
                system("cls");
                rendermap(map, length, width, *x, *y);
            continue;
        }
        else if (moving == 'Q' || moving == 'q') {
            printf("退出游戏。\n");
            judgeover = 1;
            break;  // 退出循环
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }

        if (m == 1) { //输入有效
            if (*(map + yplus * width + xplus) != 1) {    //未撞到墙 更新位置
                into_z(*x, *y, steps);
                *x = xplus;
                *y = yplus;
                system("cls");
                steps++;
                iftreasure(x, y, width, map);
                rendermap(map, length, width, *x, *y);
                Sleep(100);
                if (*(map + yplus * width + xplus) == 2) {   //踩到陷阱 下一步耗费体力2
                    steps++;
                }
            }
            else {  //撞到墙 只增加体力
                steps++;
                system("cls");
                rendermap(map, length, width, *x, *y);
            }
        }
        else { //无效输入
            system("cls");
            rendermap(map, length, width, *x, *y);
            printf("输入错误，请重新输入。\n");
        }
        if (foundtreasure == treasure) {
            judgeover = 2;
            break;
        }
    }
}

/*小黄的移动与体力计算 编程模式*/
void movexiaohuang1(int* x, int* y, int length, int width, int* map) {
    char moving;
    int p = 0;
    while (1) {
        if (paths[p] == '\0' || paths[p] == '\r' || paths[p] == '\n') {
            break;
        }
        moving = paths[p++];
        int m = 1;
        int xplus = *x, yplus = *y;
        if (moving == 'W' || moving == 'w') {
            yplus--;
            paths[i++] = 'U';// 向上移动
        }
        else if (moving == 'A' || moving == 'a') {
            xplus--;
            paths[i++] = 'L';// 向左移动
        }
        else if (moving == 'S' || moving == 's') {
            yplus++;
            paths[i++] = 'D';// 向下移动
        }
        else if (moving == 'D' || moving == 'd') {
            xplus++;
            paths[i++] = 'R'; // 向右移动
        }
        else if (moving == 'Q' || moving == 'q') {
            judgeover = 1;
            break;  // 退出循环
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }
        if (m == 1) { //输入有效
            if (*(map + yplus * width + xplus) != 1) {    //未撞到墙 更新位置
                *x = xplus;
                *y = yplus;
                steps++;
                iftreasure(x, y, width, map);
                if (*(map + yplus * width + xplus) == 2) {   //踩到陷阱 下一步耗费体力2
                    steps++;
                }
            }
            else {  //撞到墙 只增加体力
                steps++;
            }
        }
        else { //无效输入
            judgeover = 3;
            break;
        }
        if (foundtreasure == treasure) {
            judgeover = 2;
            break;
        }
    }
}

/*结算页面*/
void finish() {
    system("cls");
    double time = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    switch (judgeover) {
    case 1:
        printf("你按下了Q键，游戏结束");
        break;
    case 2:
        printf("恭喜你，小黄找到了所有宝藏！\n");
        break;
    case 3:
        printf("你输入的指令有误，游戏结束");
        break;
    }
    printf("\n行动路径：");
    for (int j = 0; j < i; j++) {
        printf("%c", paths[j]);
    }
    printf("\n消耗的体力：%d\n", steps);
    printf("找到的宝箱数量：%d\n", foundtreasure);
    printf("\n游戏总用时：%.2f 秒\n", time);
    printf("\n<按任意键继续>\n");
}

/*游戏入口——主体*/
int main() {
    while (1) {
        /*重置参数*/
        steps = 0;
        i = 0;
        treasure = 0; foundtreasure = 0;
        judgeover = 0;
        for (int u = 0; u < 1000; u++) {
            paths[u] = '\0';
        }
        map01();
        map02();

        /*欢迎界面*/
        int selection = welcoming();
        /*过渡界面*/
        int selection1 = mode();
        //实时模式
        if (selection1 == 1) {
            starttime = GetTickCount();
            /*生成并渲染地图*/
            int x = 1, y = 1;
            //地图1
            system("cls");
            if (selection == 1) {
                for (int row = 0; row < 15; row++) {
                    for (int col = 0; col < 15; col++) {
                        if (map1[row][col] == 3) {
                            treasure++;
                        }
                    }
                }
                rendermap((int*)map1, 15, 15, 1, 1);
                movexiaohuang(&x, &y, 15, 15, (int*)map1);
            }
            //地图2
            if (selection == 2) {
                for (int row = 0; row < 21; row++) {
                    for (int col = 0; col < 21; col++) {
                        if (map2[row][col] == 3) {
                            treasure++;
                        }
                    }
                }
                rendermap((int*)map2, 21, 21, 1, 1);
                movexiaohuang(&x, &y, 21, 21, (int*)map2);
            }
            endtime = GetTickCount();

            finish();
            getch();
        }

        //编程模式
        else {
            /*生成并渲染地图*/
            starttime = GetTickCount();
            int x = 1, y = 1;
            //地图1
            system("cls");
            if (selection == 1) {
                for (int row = 0; row < 15; row++) {
                    for (int col = 0; col < 15; col++) {
                        if (map1[row][col] == 3) {
                            treasure++;
                        }
                    }
                }
                rendermap((int*)map1, 15, 15, 1, 1);
                path();
                movexiaohuang1(&x, &y, 15, 15, (int*)map1);
            }
            //地图2
            if (selection == 2) {
                for (int row = 0; row < 21; row++) {
                    for (int col = 0; col < 21; col++) {
                        if (map2[row][col] == 3) {
                            treasure++;
                        }
                    }
                }
                rendermap((int*)map2, 21, 21, 1, 1);
                path();
                movexiaohuang1(&x, &y, 21, 21, (int*)map2);
            }
            endtime = GetTickCount();

            finish();
            getch();
        }
    }
    return 0;
}