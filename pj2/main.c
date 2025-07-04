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
#define fscanf fscanf_s

//��¼��Ϣ
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
    int connected;//��� �Ƿ�������������
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

//�����ຯ��
/*��ʼ��*/
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
/*��Ⱦ��ͼ*/
void rendermap(int map[50][50], int length, int width) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i == tail->player.y && j == tail->player.x) {
                printf(YELLOW "Y" RESET_COLOR);  // ��ʾС�Ƶ�λ��
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
                    int flag = 0;//�����Ƿ��ҵ�
                    for (int k = 0; k < 4; k++) {
                        //��������ѱ��ҵ�
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
    printf("�������ģ�%d\n", tail->player.steps);
    printf("���Ʒ�������W�����ƶ�����S�����ƶ�����A�����ƶ�����D�����ƶ�����Iԭ�ز�������Q����ð�ա�\n��Z��������\n");
}
/*��ȡ�ļ���ͼ*/
void ReadMap(const char* filename, int map[50][50], int* length, int* width,int*x,int*y) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("ʧ��");
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
/*�ж��Ƿ��ҵ����� ����������*/
void iftreasure(int width, int map[50][50]) {
    int dx[] = { 0, -1, 1, 0 };
    int dy[] = { -1, 0, 0, 1 };
    for (int dir = 0; dir < 4; dir++) {//���������ĸ���λ
        int nx = tail->player.x + dx[dir];
        int ny = tail->player.y + dy[dir];
        if (nx >= 0 && ny >= 0 && nx < width && ny < width) {
            if (*(*(map+ny)+nx) == 3) {  // ����Ǳ�����δ��������, ��������
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
/*��¼ÿһ����·��*/
void Move(char moving) {
    if (moving == 'W' || moving == 'w') {
        tail->move = 'U';// �����ƶ�
    }
    else if (moving == 'A' || moving == 'a') {
        tail->move = 'L';// �����ƶ�
    }
    else if (moving == 'S' || moving == 's') {
        tail->move = 'D';// �����ƶ�
    }
    else if (moving == 'D' || moving == 'd') {
        tail->move = 'R';// �����ƶ�
    }
    else {
        tail->move = '0';
    }
}

/*������д��ǵı�Ԫ*/
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
/*������Ԫ*/
Op* create_op(int newsteps, int xplus, int yplus, int nextsteps, int width, int map[50][50]) {
    delete();
    Op* p = (Op*)malloc(sizeof(Op));
    if (p == NULL) {
        printf("ʧ��");
        return NULL;
    }
    p->connected = 1;//�˱�Ԫ����������
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
/*��������������һ����Ԫ*/
void press_z() {
    if (tail->prev != NULL) {
        tail->connected = 0;
        tail = tail->prev;
    }
}
/*����y����ȡ�����һ����Ԫ*/
void press_y() {
    if (tail != NULL && tail->next != NULL) {
        tail = tail->next;
        tail->connected = 1;
    }
}

/*��ȡ��ǰʱ��*/
void gettime() {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(data->currenttime, sizeof(data->currenttime), "%Y-%m-%d %H:%M:%S", tm_info);
}
/*��¼���ȡ��������ļ�*/
void Savedata(const char *filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("�����ˣ�");
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
/*��ȡ���ȡ������ļ������*/
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
            tail = p; // connected = 1 �Ľڵ���β�ڵ�
        }
        w = p;
    }
    fclose(file);
}
/*����ĵ�*/
void Clearfile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("���ļ�ʧ��\n");
        return;
    }
    fclose(file);
}

//���ڿ����ƶ��ĺ���
/*С�Ƶ��ƶ��߼����������� ʵʱģʽ*/
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
            printf("�˳���Ϸ��\n");
            Savedata("save_data");
            *judgeover = 1;
            break;  // �˳�ѭ��
        }
        else if (moving != 'I' && moving != 'i') {
            m = 0;
        }

        if (m == 1) { //������Ч
            newsteps = tail->player.steps + tail->nextsteps;
            if (*(*(map + yplus) + xplus) == 0|| *(*(map + yplus) + xplus) == 3) {    //�������ƶ�
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                system("cls");
                iftreasure(width, map);
                rendermap(map, length, width);
            }
            else  if (*(*(map + yplus) + xplus) == 2) {   //�ȵ����� ��һ���ķ�����2
                tail = create_op(newsteps, xplus, yplus, 2, width, map);
                Move(moving);
                system("cls");
                iftreasure(width, map);
                rendermap(map, length, width);
            }
            else {  //ײ��ǽ
                xplus = tail->player.x;
                yplus = tail->player.y;
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                system("cls");
                rendermap(map, length, width);
            }
        }
        else { //��Ч����
            system("cls");
            rendermap(map, length, width);
            printf("����������������롣\n");
        }
        if (tail->player.foundtreasure_num == tail->treasure) {
            break;
        }
    }
}
/*С�Ƶ��ƶ����������� ���ģʽ*/
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
                break;  // �˳�ѭ��
            }
            else {
                *judgeover = 3;
                break;
            }
            newsteps = tail->player.steps + tail->nextsteps;
            if (*(*(map + yplus) + xplus) == 0 || *(*(map + yplus) + xplus) == 3) {    //�������ƶ�
                tail = create_op(newsteps, xplus, yplus, 1, width, map);
                Move(moving);
                iftreasure(width, map);
            }
            else  if (*(*(map + yplus) + xplus) == 2) {   //�ȵ����� ��һ���ķ�����2
                tail = create_op(newsteps, xplus, yplus, 2, width, map);
                Move(moving);
                iftreasure(width, map);
            }
            else {  //ײ��ǽ
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

//��ʾ�ຯ��
/*��ӭ����*/
int welcoming() {
    int choice = 1;
    char c_choice;
    while (1) {
        system("cls");
        printf("С�Ƶ�����̽�գ�\n");
        printf("\n");
        printf("%s��ʼ<ƽ��֮·>  %s\n", (choice == 1) ? "> " : "  ", (data->selection_level == 1) ? "<�ϴ�>" : " ");
        printf("%s��ʼ<��ׯ���>  %s\n", (choice == 2) ? "> " : "  ", (data->selection_level == 2) ? "<�ϴ�>" : " ");
        printf("%s��ʼ<ħ��֮��>  %s\n", (choice == 3) ? "> " : "  ", (data->selection_level == 3) ? "<�ϴ�>" : " ");
        printf("%s�˳�\n", (choice == 4) ? "> " : "  ");
        printf("\n���Ʒ�������W�����ƶ�����S�����ƶ�����<Enter>ѡ��");
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
/*����ҳ��*/
int continue_playing() {
    int choice = 0;
    char c_choice;
    while (1) {
        system("cls");
        printf("�Ƿ�����ϴεĽ��ȣ�\n");
        printf("�ϴ������ʱ�䣺%s\n", data->currenttime);
        printf("Ѱ�õı�������%d/%d\n\n", tail->player.foundtreasure_num, tail->treasure);
        printf("%s  ��\n", (choice == 0) ? ">" : " ");
        printf("%s  ��\n\n", (choice == 1) ? ">" : " ");
        printf("��<Enter>ѡ��");
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
/*����ҳ��*/
void finish(int *judgeover, DWORD *starttime, DWORD *endtime) {
    system("cls");
    double time = ((double)(*endtime - *starttime)) / CLOCKS_PER_SEC;
    if (tail->player.foundtreasure_num == tail->treasure) {
        printf("��ϲ�㣬С���ҵ������б��أ�\n");
    }
    switch (*judgeover) {
    case 1:
        printf("��⵽�㰴����Q���������ѱ���");
        break;
    case 3:
        printf("��⵽�������ָ����ڴ�����Ϸ����");
        break;
    }
    printf("\n�ж�·����");
    for (Op* p = head; p!=tail->next; p = p->next) {
        if (p->move != '0') {
            printf("%c", p->move);
        }
        else continue;
    }
    printf("\n���ĵ�������%d\n", tail->player.steps);
    printf("�ҵ��ı���������%d\n", tail->player.foundtreasure_num);
    printf("\n��Ϸ����ʱ��%.2f ��\n", time);
    printf("\n<�����������>\n");
}

/*��Ϸ��ڡ�������*/
int main() {
    while (1) {
        /*���ò���*/
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

        //��ӭ����
        int choose = welcoming();

        /*�жϲ���*/
        //���δѡ���ϴδ浵
        if (data->selection_level != choose) {
            data->selection_level=choose;
            data->selection_mode = mode();
            initialize(tail);
            head = tail;
        }
        //���ѡ���ϴδ浵
        else {
            system("cls");
            //���ѡ����մ浵 ���ʼ�����ص���ʼҳ��
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

        /*���沿��*/
        //ʵʱģʽ
        if (data->selection_mode == 1) {
            starttime = GetTickCount();
            /*���ɲ���Ⱦ��ͼ*/
            system("cls");
            //��ͼ1
            if (data->selection_level == 1) {
                ReadMap("maps/ƽ��֮·.txt", map, &length, &width,&head->player.x,&head->player.y);
                rendermap(map, length, width);
                realtime_mode(length, width, map,& judgeover);
            }
            //��ͼ2
            if (data->selection_level == 2) {
                ReadMap("maps/��ׯ���.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    realtime_mode(length, width, map,& judgeover);
            }
            //��ͼ3
            if (data->selection_level == 3) {
                ReadMap("maps/ħ��֮��.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    realtime_mode(length, width, map, &judgeover);
            }
            endtime = GetTickCount();

            finish(&judgeover,&starttime,&endtime);
            getch();
        }

        //���ģʽ
        else {
            /*���ɲ���Ⱦ��ͼ*/
            starttime = GetTickCount();
            //��ͼ1
            system("cls");
            if (data->selection_level == 1) {
                ReadMap("maps/ƽ��֮·.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    programming_mode(length, width, map,& judgeover);
            }
            //��ͼ2
            if (data->selection_level == 2) {
                ReadMap("maps/��ׯ���.txt", map, &length, &width, &head->player.x, &head->player.y);
                    rendermap(map, length, width);
                    programming_mode(length, width, map, &judgeover);
            }
            //��ͼ3
            if (data->selection_level == 3) {
                ReadMap("maps/ħ��֮��.txt", map, &length, &width, &head->player.x, &head->player.y);
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