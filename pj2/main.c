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
#define fscanf fscanf_s

//记录信息
typedef struct {
    int steps;
    int x, y;
    int foundtreasure_num;
}Player;
typedef struct Op{
    Player player;
    char move;
    int foundtreasure[4][2];
    int nextsteps;
    int treasure;
    int connected;//标记 是否连接在链表中
    struct Op* prev;
    struct Op* next;
}Op;
struct Op* head;
struct Op* tail;
typedef struct Data {
    int selection_level,selection_mode;
    char currenttime[50];
    Op* head;
}Data;
Data *data;

//功能类函数
/*初始化*/
void initialize(Op* tail) {
    tail->player.steps = 0;
    tail->player.foundtreasure_num = 0;
    tail->move='0';
    tail->nextsteps = 1;
    tail->connected = 1;
    for (int m = 0; m < 4; m++) {
        for (int n = 0; n < 2; n++) {
            tail->foundtreasure[m][n] = -1;
        }
    }
    tail->treasure = 0;
    return;
}
/*渲染地图*/
void rendermap(int map[50][50], int length, int width) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i == tail->player.y && j == tail->player.x) {
                printf(YELLOW "Y" RESET_COLOR);  // 显示小黄的位置
            }
            else {
                int value = *(*(map + i) + j);
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
                    int flag = 0;//宝藏是否被找到
                    for (int k = 0; k < 4; k++) {
                        //如果宝藏已被找到
                        if (tail->foundtreasure[k][0] == i && tail->foundtreasure[k][1] == j) {
                            printf(" ");
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 0) {
                        printf(LIGHT_RED "T" RESET_COLOR);
                    }
                }
            }
        }printf("\n");
    }
    printf("体力消耗：%d\n", tail->player.steps);
    printf("控制方法：按W向上移动，按S向下移动，按A向左移动，按D向右移动，按I原地不动，按Q结束冒险。\n按Z撤销操作\n");
}
/*读取文件地图*/
void ReadMap(const char* filename, int map[50][50], int* length, int* width,int*x,int*y) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("失败");
        return;
    }
    tail->treasure = 0;
    fscanf(file, "%d %d", length, width);
    fscanf(file, "%d %d", x,y);
    for (int i = 0; i < *length; i++) {
        for (int j = 0; j < *width; j++) {
            fscanf(file, "%d", &map[i][j]);
            if (map[i][j] == 3) {
                tail->treasure++;
            }
        }
    }
    
    fclose(file);
}
/*判断是否找到宝藏 并存入数组*/
void iftreasure(int width, int map[50][50]) {
    int dx[] = { 0, -1, 1, 0 };
    int dy[] = { -1, 0, 0, 1 };
    for (int dir = 0; dir < 4; dir++) {//上下左右四个方位
        int nx = tail->player.x + dx[dir];
        int ny = tail->player.y + dy[dir];
        if (nx >= 0 && ny >= 0 && nx < width && ny < width) {
            if (*(*(map+ny)+nx) == 3) {  // 如果是宝藏且未存入数组, 存入数组
                int flag = 0;
                for (int k = 0; k < 4; k++) {
                    if (tail->foundtreasure[k][0] == ny && tail->foundtreasure[k][1] == nx) {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    tail->foundtreasure[tail->player.foundtreasure_num][0] = ny;
                    tail->foundtreasure[tail->player.foundtreasure_num][1] = nx;
                    tail->player.foundtreasure_num++;
                }

            }
        }
    }
}
/*记录每一步的路径*/
void Move(char moving) {
    if (moving == 'W' || moving == 'w') {
        tail->move = 'U';// 向上移动
    }
    else if (moving == 'A' || moving == 'a') {
        tail->move = 'L';// 向左移动
    }
    else if (moving == 'S' || moving == 's') {
        tail->move = 'D';// 向下移动
    }
    else if (moving == 'D' || moving == 'd') {
        tail->move = 'R';// 向右移动
    }
    else {
        tail->move = '0';
    }
}

/*清空所有打标记的表元*/
void delete() {
    Op* p = tail, * w;
    if (head == NULL) {
        return;
    }
    while (p != NULL) {
        w = p;
        p = p->next;
        if (w->connected == 0) {
            if (w->prev != NULL) {
                w->prev->next = w->next;
            }
            else {
                head = w->next;
            }
            if (w->next != NULL)
                w->next->prev = w->prev;
            else {
                tail = w->prev;
            }
            free(w);
        }
    }
}
/*创建表元*/
Op* create_op(int newsteps, int xplus, int yplus, int nextsteps, int width, int map[50][50]) {
    delete();
    Op* p = (Op*)malloc(sizeof(Op));
    if (p == NULL) {
        printf("失败");
        return NULL;
    }
    p->connected = 1;//此表元连在链表里
    p->player.steps = newsteps;
    p->player.x = xplus;
    p->player.y = yplus;
    p->nextsteps = nextsteps;
    p->player.foundtreasure_num = tail->player.foundtreasure_num;
    p->treasure = tail->treasure;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            p->foundtreasure[i][j] = tail->foundtreasure[i][j];
        }
    }
    iftreasure(width, map);
    p->next = NULL;
    p->prev = tail;
    if (tail == NULL) {
        head = p;
    }
    else {
        tail->next = p;
    }
    tail = p;
    return p;
}
/*撤销——标记最后一个表元*/
void press_z() {
    if (tail->prev != NULL) {
        tail->connected = 0;
        tail = tail->prev;
    }
}
/*按下y——取消标记一个表元*/
void press_y() {
    if (tail != NULL && tail->next != NULL) {
        tail = tail->next;
        tail->connected = 1;
    }
}

/*获取当前时间*/
void gettime() {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(data->currenttime, sizeof(data->currenttime), "%Y-%m-%d %H:%M:%S", tm_info);
}
/*记录进度——存入文件*/
void Savedata(const char *filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("出错了！");
        return;
    }
    fwrite(&data->selection_level, sizeof(int), 1, file);
    fwrite(&data->selection_mode, sizeof(int), 1, file);
    gettime();
    fwrite(data->currenttime, sizeof(char), sizeof(data->currenttime) / sizeof(char), file);
    Op* p = head;
    while (p != NULL) {
        fwrite(&p->player.steps, sizeof(int), 1, file);
        fwrite(&p->player.x, sizeof(int), 1, file);
        fwrite(&p->player.y, sizeof(int), 1, file);
        fwrite(&p->player.foundtreasure_num, sizeof(int), 1, file);
        fwrite(&p->foundtreasure, sizeof(int), sizeof(p->foundtreasure)/sizeof(int), file);
        fwrite(&p->nextsteps, sizeof(int), 1, file);
        fwrite(&p->move, sizeof(char), 1, file);
        fwrite(&p->treasure, sizeof(int), 1, file);
        fwrite(&p->connected, sizeof(int), 1, file);
        p = p->next;
    }
    fclose(file);
}
/*读取进度——从文件里读出*/
void Readdata(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return;
    }
    fread(&data->selection_level, sizeof(int), 1, file);
    fread(&data->selection_mode, sizeof(int), 1, file);
    fread(data->currenttime, sizeof(char), sizeof(data->currenttime) / sizeof(char), file);
    Op* p, * w = NULL;
    while (1) {
        p = (Op*)malloc(sizeof(Op));
        if (fread(&p->player.steps, sizeof(int), 1, file) != 1) {
            free(p);
            w->next = NULL;
            break;
        }
        fread(&p->player.x, sizeof(int), 1, file);
        fread(&p->player.y, sizeof(int), 1, file);
        fread(&p->player.foundtreasure_num, sizeof(int), 1, file);
        fread(&p->foundtreasure, sizeof(int), sizeof(p->foundtreasure) / sizeof(int), file);
        fread(&p->nextsteps, sizeof(int), 1, file);
        fread(&p->move, sizeof(char), 1, file);
        fread(&p->treasure, sizeof(int), 1, file);
        fread(&p->connected, sizeof(int), 1, file);
        if (w == NULL) {
            head = p;
            head->prev = NULL;
        }
        else {
            w->next = p;
            p->prev = w;
        }
        if (p->connected == 1) {
            tail = p; // connected = 1 的节点是尾节点
        }
        w = p;
    }
    fclose(file);
}
/*清空文档*/
void Clearfile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("打开文件失败\n");
        return;
    }
    fclose(file);
}

//用于控制移动的函数
/*小黄的移动逻辑与体力计算 实时模式*/
void realtime_mode(int length, int width, int map[50][50],int*judgeover) {
    char moving;
    int m = 1, newsteps;
    while (1) {
        moving = getch();
        int xplus = tail->player.x, yplus = tail->player.y;
        if (moving == 'W' || moving == 'w') {
            yplus--;
        }
        else if (moving == 'A' || moving == 'a') {
            xplus--;
        }
        else if (moving == 'S' || moving == 's') {
            yplus++;
        }
        else if (moving == 'D' || moving == 'd') {
            xplus++;
        }
        else if (moving == 'Z' || moving == 'z') {
            press_z();
            system("cls");
            rendermap(map, length, width);
            continue;
        }
        else if (moving == 'Y' || moving == 'y') {
            press_y();
            system("cls");
            rendermap(map, length, width);
            continue;
        }
        else if (moving == 'Q' || moving == 'q') {
            printf("退出游戏。\n");
            Savedata("save_data");
            *judgeover = 1;
            break;  // 退出循环
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }

        if (m == 1) { //输入有效
            newsteps = tail->player.steps + tail->nextsteps;
            if (*(*(map + yplus) + xplus) == 0|| *(*(map + yplus) + xplus) == 3) {    //可正常移动
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                system("cls");
                iftreasure(width, map);
                rendermap(map, length, width);
            }
            else  if (*(*(map + yplus) + xplus) == 2) {   //踩到陷阱 下一步耗费体力2
                tail = create_op(newsteps, xplus, yplus, 2, width, map);
                Move(moving);
                system("cls");
                iftreasure(width, map);
                rendermap(map, length, width);
            }
            else {  //撞到墙
                xplus = tail->player.x;
                yplus = tail->player.y;
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                system("cls");
                rendermap(map, length, width);
            }
        }
        else { //无效输入
            system("cls");
            rendermap(map, length, width);
            printf("输入错误，请重新输入。\n");
        }
        if (tail->player.foundtreasure_num == tail->treasure) {
            break;
        }
    }
}
/*小黄的移动与体力计算 编程模式*/
void programming_mode( int length, int width, int map[50][50],int *judgeover) {
    char input[1000];
    for (int m = 0; m < 1000; m++) {
            scanf("%c", &input[m]);    
        if (input[m] == '\n') {
            break;
        }
    }
    for (int k = 0; input[k] != '\n'; k++) {
        char moving = input[k];
        int m = 1, newsteps;
        int xplus = tail->player.x, yplus = tail->player.y;
            if (moving == 'W' || moving == 'w') {
                yplus--;
            }
            else if (moving == 'A' || moving == 'a') {
                xplus--;
            }
            else if (moving == 'S' || moving == 's') {
                yplus++;
            }
            else if (moving == 'D' || moving == 'd') {
                xplus++;
            }
            else if (moving == 'Q' || moving == 'q') {
                Savedata("save_data");
                *judgeover = 1;
                break;  // 退出循环
            }
            else {
                *judgeover = 3;
                break;
            }
            newsteps = tail->player.steps + tail->nextsteps;
            if (*(*(map + yplus) + xplus) == 0 || *(*(map + yplus) + xplus) == 3) {    //可正常移动
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                iftreasure(width, map);
            }
            else  if (*(*(map + yplus) + xplus) == 2) {   //踩到陷阱 下一步耗费体力2
                tail = create_op(newsteps, xplus, yplus, 2, width, map);
                Move(moving);
                iftreasure(width, map);
            }
            else {  //撞到墙
                xplus = tail->player.x;
                yplus = tail->player.y;
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
            }
            if (tail->player.foundtreasure_num == tail->treasure) {
                break;
            }
        
    }
}

//显示类函数
/*欢迎界面*/
int welcoming() {
    int choice = 1;
    char c_choice;
    while (1) {
        system("cls");
        printf("小黄的奇妙探险！\n");
        printf("\n");
        printf("%s开始<平凡之路>  %s\n", (choice == 1) ? "> " : "  ", (data->selection_level == 1) ? "<上次>" : " ");
        printf("%s开始<康庄大道>  %s\n", (choice == 2) ? "> " : "  ", (data->selection_level == 2) ? "<上次>" : " ");
        printf("%s开始<魔王之旅>  %s\n", (choice == 3) ? "> " : "  ", (data->selection_level == 3) ? "<上次>" : " ");
        printf("%s退出\n", (choice == 4) ? "> " : "  ");
        printf("\n控制方法：按W向上移动，按S向下移动，按<Enter>选择。");
        c_choice = getch();
        if (c_choice == 'w' || c_choice == 'W') {
            if (choice > 1) choice--;
        }
        if (c_choice == 's' || c_choice == 'S') {
            if (choice < 4) choice++;
        }
        if (c_choice == '\r') {
            if (choice == 4) exit(0);
            else return choice;
        }
    }
}
/*加载页面*/
int continue_playing() {
    int choice = 0;
    char c_choice;
    while (1) {
        system("cls");
        printf("是否加载上次的进度？\n");
        printf("上次游玩的时间：%s\n", data->currenttime);
        printf("寻得的宝箱数：%d/%d\n\n", tail->player.foundtreasure_num, tail->treasure);
        printf("%s  是\n", (choice == 0) ? ">" : " ");
        printf("%s  否\n\n", (choice == 1) ? ">" : " ");
        printf("按<Enter>选择");
        c_choice = getch();
        if (c_choice == 'w' || c_choice == 'W') {
            if (choice > 0) choice--;
        }
        if (c_choice == 's' || c_choice == 'S') {
            if (choice < 1) choice++;
        }
        if (c_choice == '\r') {
            return choice;
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
/*结算页面*/
void finish(int *judgeover, DWORD *starttime, DWORD *endtime) {
    system("cls");
    double time = ((double)(*endtime - *starttime)) / CLOCKS_PER_SEC;
    if (tail->player.foundtreasure_num == tail->treasure) {
        printf("恭喜你，小黄找到了所有宝藏！\n");
    }
    switch (*judgeover) {
    case 1:
        printf("检测到你按下了Q键，进度已保存");
        break;
    case 3:
        printf("检测到你输入的指令存在错误，游戏结束");
        break;
    }
    printf("\n行动路径：");
    for (Op* p = head; p!=tail->next; p = p->next) {
        if (p->move != '0') {
            printf("%c", p->move);
        }
        else continue;
    }
    printf("\n消耗的体力：%d\n", tail->player.steps);
    printf("找到的宝箱数量：%d\n", tail->player.foundtreasure_num);
    printf("\n游戏总用时：%.2f 秒\n", time);
    printf("\n<按任意键继续>\n");
}

/*游戏入口——主体*/
int main() {
    while (1) {
        /*重置参数*/
        int map[50][50]; 
        int length, width;  
        int judgeover = 0;
        DWORD starttime, endtime;
        tail = (Op*)malloc(sizeof(Op));
        head = (Op*)malloc(sizeof(Op));
        initialize(tail);
        tail->next = NULL;
        tail->prev = NULL;
        head = tail;
        data = (Data*)malloc(sizeof(Data));
        Readdata("save_data");

        //欢迎界面
        int choose = welcoming();

        /*判断部分*/
        //如果未选择上次存档
        if (data->selection_level != choose) {
            data->selection_level=choose;
            data->selection_mode = mode();
            initialize(tail);
            head = tail;
        }
        //如果选择上次存档
        else {
            system("cls");
            //如果选择清空存档 则初始化并回到开始页面
            if (continue_playing() == 1) {
                Clearfile("save_data");
                tail->move = '0';
                initialize(tail);
                tail->next = NULL;
                tail->prev = NULL;
                head = tail;
                data->selection_level = 0;
                data->selection_mode = 0;
                data->selection_level = welcoming();
                data->selection_mode= mode();
            };
        }

        /*游玩部分*/
        //实时模式
        if (data->selection_mode == 1) {
            starttime = GetTickCount();
            /*生成并渲染地图*/
            system("cls");
            //地图1
            if (data->selection_level == 1) {
                ReadMap("maps/平凡之路.txt", map, &length, &width,&head->player.x,&head->player.y);
                rendermap(map, length, width);
                realtime_mode(length, width, map,& judgeover);
            }
            //地图2
            if (data->selection_level == 2) {
                ReadMap("maps/康庄大道.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    realtime_mode(length, width, map,& judgeover);
            }
            //地图3
            if (data->selection_level == 3) {
                ReadMap("maps/魔王之旅.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    realtime_mode(length, width, map, &judgeover);
            }
            endtime = GetTickCount();

            finish(&judgeover,&starttime,&endtime);
            getch();
        }

        //编程模式
        else {
            /*生成并渲染地图*/
            starttime = GetTickCount();
            //地图1
            system("cls");
            if (data->selection_level == 1) {
                ReadMap("maps/平凡之路.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    programming_mode(length, width, map,& judgeover);
            }
            //地图2
            if (data->selection_level == 2) {
                ReadMap("maps/康庄大道.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    programming_mode(length, width, map, &judgeover);
            }
            //地图3
            if (data->selection_level == 3) {
                ReadMap("maps/魔王之旅.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    programming_mode(length, width, map, &judgeover);
            }
            endtime = GetTickCount();

            finish(&judgeover,&starttime,&endtime);
            getch();
        }
    }
    return 0;
}