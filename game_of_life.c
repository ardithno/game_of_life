#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

void input(int** Array);
void reuse(int** Array);
void field(int** Array);
void coppy(int** Array);
void neighbor(int** Array);
int speed_func(int speed, char s);
int kbhit();

int hieght = 27;
int wieght = 82;

int main(void) {
    int speed = 100000;
    char c;
    int** Array = calloc(hieght, sizeof(int*));
    for (int i = 0; i < hieght; i++) {
        Array[i] = calloc(wieght, sizeof(int));
    }
    input(Array);
    freopen("/dev/tty", "r", stdin);
    system("/bin/stty cbreak");
    while (1) {
        if (kbhit()) {
        c = getchar();
        speed = speed_func(speed, c);
        if (c == 'q') break;
        }
        field(Array);
        reuse(Array);
        usleep(speed);
    }
    system("/bin/stty cooked");
    for (int i = 0; i < hieght; i++) {
        free(Array[i]);
    }
    free(Array);
    return 0;
}
// Отрисовка поля
void field(int** Array) {
    for (int y=0; y < hieght; y++) {
        for (int x=0; x < wieght; x++) {
            if ((y == 0) || (y == hieght - 1)) printf("-");
            else if ((x == 0) || (x == wieght - 1)) printf("|");
            else if (Array[y][x] >= 10) printf("0");
            else
                printf(" ");
        }
        printf("\n");
    }
    printf("\e[h\e[2J\e[3J");
}
// Объявление массива + заполнение начального пресета
void input(int** Array) {
    int x, y;
    for (int i = 1; i < hieght - 1; i++) {
        for (int j = 0; j < wieght -1; j++) {
           scanf("%d %d", &x, &y);
            Array[x][y] = 10;
        }
    }
}
// изменение скорости
int speed_func(int speed, char c) {
    if (c == '-') speed += 10000;
    else if (c == '+') speed -= 10000;
    return speed;
}
// Расчет новых значений на новый ход
void reuse(int** Array) {
    coppy(Array);
    neighbor(Array);
// Блок для обновления значений
    for (int i = 1; i < hieght - 1; i++) {
        for (int j = 1; j < wieght - 1; j++) {
            if (Array[i][j] == 12 || Array[i][j] == 13 || Array[i][j] == 3) {
                Array[i][j] = 10;
            } else {
                Array[i][j] = 0;
            }
        }
    }
}
void coppy(int** Array) {
// копирование граничных линий в противоположные рамки
    for (int i = 0; i < wieght; i++) {
        Array[hieght-1][i] = Array[1][i];
        Array[0][i] = Array[hieght-2][i];
    }
    for (int i = 1; i < hieght-1; i++) {
        Array[i][0] = Array[i][wieght-2];
        Array[i][wieght-1] = Array[i][1];
    }
}
void neighbor(int** Array) {
// Добавляем соседям очки соседства // Добавляем соседство из созданных рамок
    for (int i = 1; i < hieght - 1; i++) {
        for (int j = 1; j < wieght - 1; j++) {
            if (Array[i][j] >= 10) {
                Array[i-1][j-1] += 1;
                Array[i-1][j] += 1;
                Array[i-1][j+1] += 1;
                Array[i][j+1] += 1;
                Array[i+1][j+1] += 1;
                Array[i+1][j] += 1;
                Array[i+1][j-1] += 1;
                Array[i][j-1] += 1;
            }
        }
    }
    for (int i = 1; i < wieght-1; i++) {
        if (Array[0][i] >= 10) {
            Array[1][i-1] += 1;
            Array[1][i] += 1;
            Array[1][i+1] += 1;
        }
        if (Array[hieght-1][i] >= 10) {
            Array[hieght-2][i-1] += 1;
            Array[hieght-2][i] += 1;
            Array[hieght-2][i+1] += 1;
        }
    }
    for (int i = 1; i < hieght-1; i++) {
        if (Array[i][0] >= 10) {
            Array[i-1][1] += 1;
            Array[i][1] += 1;
            Array[i+1][1] += 1;
        }
        if (Array[i][wieght-1] >= 10) {
            Array[i-1][wieght-2] += 1;
            Array[i][wieght-2] += 1;
            Array[i+1][wieght-2] += 1;
        }
    }
}
// Magic для работы с stdin
int kbhit() {
    int c;
    ioctl(1, FIONREAD, &c);
    return c;
}
