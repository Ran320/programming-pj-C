#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define RESET_COLOR "\033[0m"
#define YELLOW "\033[33m"//��ɫ
#define LIGHT_RED "\033[91m" // ǳ��ɫ
#define LIGHT_BLUE "\033[94m"  // ǳ��ɫ
#define LIGHT_CYAN "\033[96m"    // ǳ��ɫ

#define getch() _getch()
#define scanf scanf_s

/*��ʼֵ*/
int steps = 0;
char paths[1000];
int i = 0;
int treasure = 0, foundtreasure = 0;
int judgeover = 0;
DWORD starttime, endtime;

/*��ӭ����*/
int welcoming() {
    int choice = 1;
    char c_choice;
    while (1) {
        system("cls");
        printf("С�Ƶ�����̽�գ�\n");
        printf("\n");
        printf("%s��ʼ��һ��\n", (choice == 1) ? "> " : "  ");
        printf("%s��ʼ�ڶ���\n", (choice == 2) ? "> " : "  ");
        printf("%s�˳�\n", (choice == 3) ? "> " : "  ");
        printf("\n���Ʒ�������W�����ƶ�����S�����ƶ�����<Enter>ѡ��");
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

/*���ɽ���*/
int mode() {
    int choice1 = 1;
    char c_choice1;
    while (1) {
        system("cls");
        printf("��ѡ�����ģʽ��\n");
        printf("%s0��ʵʱģʽ\n", (choice1 == 1) ? "> " : "  ");
        printf("%s1�����ģʽ\n", (choice1 == 2) ? "> " : "  ");
        printf("\n���Ʒ�������W�����ƶ�����S�����ƶ�����<Enter>ѡ��");
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

/*��ͼ1*/
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

/*��ͼ2*/
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

/*��ʼ����ͼ1*/
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

/*��ʼ����ͼ2*/
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

/*��Ⱦ��ͼ*/
void rendermap(int* map, int length, int width, int x, int y) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                printf(YELLOW "Y" RESET_COLOR);  // ��ʾС�Ƶ�λ��
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
    printf("�������ģ�%d\n", steps);
    printf("���Ʒ�������W�����ƶ�����S�����ƶ�����A�����ƶ�����D�����ƶ�����Iԭ�ز�������Q����ð�ա�\n��Z��������\n");
}

/*��¼�ж�·��*/
void path() {
    for (int i = 0; i < 1000; i++) {
        scanf("%c", &paths[i]);
        if (paths[i] == '\r' || paths[i] == '\n') {
            paths[i] = '\0';  // �����з��滻Ϊ�ַ���������
            break;
        }
    }
}

/*�ж��Ƿ��ҵ�����*/
void iftreasure(int* x, int* y, int width, int* map) {
    int dx[] = { 0, -1, 1, 0 };
    int dy[] = { -1, 0, 0, 1 };
    for (int dir = 0; dir < 4; dir++) {
        int nx = *x + dx[dir];
        int ny = *y + dy[dir];
        if (nx >= 0 && ny >= 0 && nx < width && ny < width) {
            if (*(map + ny * width + nx) == 3) {  // ����Ǳ���
                *(map + ny * width + nx) = 0;     // ����Ϊ 0����ʾ������ʧ
                foundtreasure++;
            }
        }
    }
}

/*���ӣ���z����*/
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

/*С�Ƶ��ƶ��߼����������� ʵʱģʽ*/
void movexiaohuang(int* x, int* y, int length, int width, int* map) {
    char moving;
    while (1) {
        moving = getch();
        int m = 1;
        int xplus = *x, yplus = *y;
        if (moving == 'W' || moving == 'w') {
            yplus--;
            paths[i++] = 'U';// �����ƶ�
        }
        else if (moving == 'A' || moving == 'a') {
            xplus--;
            paths[i++] = 'L';// �����ƶ�
        }
        else if (moving == 'S' || moving == 's') {
            yplus++;
            paths[i++] = 'D';// �����ƶ�
        }
        else if (moving == 'D' || moving == 'd') {
            xplus++;
            paths[i++] = 'R';// �����ƶ�
        }
        else if (moving == 'Z' || moving == 'z') {
                outof_z(x, y, &steps);
                system("cls");
                rendermap(map, length, width, *x, *y);
            continue;
        }
        else if (moving == 'Q' || moving == 'q') {
            printf("�˳���Ϸ��\n");
            judgeover = 1;
            break;  // �˳�ѭ��
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }

        if (m == 1) { //������Ч
            if (*(map + yplus * width + xplus) != 1) {    //δײ��ǽ ����λ��
                into_z(*x, *y, steps);
                *x = xplus;
                *y = yplus;
                system("cls");
                steps++;
                iftreasure(x, y, width, map);
                rendermap(map, length, width, *x, *y);
                Sleep(100);
                if (*(map + yplus * width + xplus) == 2) {   //�ȵ����� ��һ���ķ�����2
                    steps++;
                }
            }
            else {  //ײ��ǽ ֻ��������
                steps++;
                system("cls");
                rendermap(map, length, width, *x, *y);
            }
        }
        else { //��Ч����
            system("cls");
            rendermap(map, length, width, *x, *y);
            printf("����������������롣\n");
        }
        if (foundtreasure == treasure) {
            judgeover = 2;
            break;
        }
    }
}

/*С�Ƶ��ƶ����������� ���ģʽ*/
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
            paths[i++] = 'U';// �����ƶ�
        }
        else if (moving == 'A' || moving == 'a') {
            xplus--;
            paths[i++] = 'L';// �����ƶ�
        }
        else if (moving == 'S' || moving == 's') {
            yplus++;
            paths[i++] = 'D';// �����ƶ�
        }
        else if (moving == 'D' || moving == 'd') {
            xplus++;
            paths[i++] = 'R'; // �����ƶ�
        }
        else if (moving == 'Q' || moving == 'q') {
            judgeover = 1;
            break;  // �˳�ѭ��
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }
        if (m == 1) { //������Ч
            if (*(map + yplus * width + xplus) != 1) {    //δײ��ǽ ����λ��
                *x = xplus;
                *y = yplus;
                steps++;
                iftreasure(x, y, width, map);
                if (*(map + yplus * width + xplus) == 2) {   //�ȵ����� ��һ���ķ�����2
                    steps++;
                }
            }
            else {  //ײ��ǽ ֻ��������
                steps++;
            }
        }
        else { //��Ч����
            judgeover = 3;
            break;
        }
        if (foundtreasure == treasure) {
            judgeover = 2;
            break;
        }
    }
}

/*����ҳ��*/
void finish() {
    system("cls");
    double time = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    switch (judgeover) {
    case 1:
        printf("�㰴����Q������Ϸ����");
        break;
    case 2:
        printf("��ϲ�㣬С���ҵ������б��أ�\n");
        break;
    case 3:
        printf("�������ָ��������Ϸ����");
        break;
    }
    printf("\n�ж�·����");
    for (int j = 0; j < i; j++) {
        printf("%c", paths[j]);
    }
    printf("\n���ĵ�������%d\n", steps);
    printf("�ҵ��ı���������%d\n", foundtreasure);
    printf("\n��Ϸ����ʱ��%.2f ��\n", time);
    printf("\n<�����������>\n");
}

/*��Ϸ��ڡ�������*/
int main() {
    while (1) {
        /*���ò���*/
        steps = 0;
        i = 0;
        treasure = 0; foundtreasure = 0;
        judgeover = 0;
        for (int u = 0; u < 1000; u++) {
            paths[u] = '\0';
        }
        map01();
        map02();

        /*��ӭ����*/
        int selection = welcoming();
        /*���ɽ���*/
        int selection1 = mode();
        //ʵʱģʽ
        if (selection1 == 1) {
            starttime = GetTickCount();
            /*���ɲ���Ⱦ��ͼ*/
            int x = 1, y = 1;
            //��ͼ1
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
            //��ͼ2
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

        //���ģʽ
        else {
            /*���ɲ���Ⱦ��ͼ*/
            starttime = GetTickCount();
            int x = 1, y = 1;
            //��ͼ1
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
            //��ͼ2
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