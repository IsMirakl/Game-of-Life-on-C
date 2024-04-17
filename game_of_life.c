#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define STR 25
#define COLS 80

void initializeMatrix(int matr[STR][COLS]);
void updateMatrix(int matrix1[STR][COLS], int matrix2[STR][COLS]);
int countNeighbors(int matrix1[STR][COLS], int i, int j);
int makeDecision(int neighbors, int condition);
void replace_matr(int matrix2[STR][COLS], int matrix1[STR][COLS]);
int compare_matr(int matrix1[STR][COLS], int matrix2[STR][COLS]);
int speed(char control_button, int *flag, int time);
int countCells(int matr[STR][COLS]);

int main() {
    int matrix1[STR][COLS];
    int matrix2[STR][COLS];
    int time = 500;
    int stop = 0;

    initializeMatrix(matrix1);
    if (freopen("/dev/tty", "r", stdin)) initscr();
    nodelay(stdscr, true);

    while (stop != 1) {
        char control_button = getch();

        if (countCells(matrix1) == 0) {
            stop = 1;
        }

        time = speed(control_button, &stop, time);

        usleep(time * 1000);
        clear();
        updateMatrix(matrix1, matrix2);

        if (compare_matr(matrix1, matrix2) == STR * COLS) {
            stop = 1;
        }
        replace_matr(matrix2, matrix1);
    }

    endwin();
    return 0;
}

void initializeMatrix(int matr[STR][COLS]) {
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COLS; j++) {
            scanf("%d", &matr[i][j]);
        }
    }
}

void updateMatrix(int matrix1[STR][COLS], int matrix2[STR][COLS]) {
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix2[i][j] = makeDecision(countNeighbors(matrix1, i, j), matrix1[i][j]);
            if (matrix2[i][j] == 1)
                printw("0");
            else
                printw("*");
        }
        printw("\n");
    }
}

int countNeighbors(int matrix1[STR][COLS], int i, int j) {
    int sum = 0;

    for (int ii = i - 1; ii <= i + 1; ii++) {
        for (int jj = j - 1; jj <= j + 1; jj++) {
            if (ii == i && jj == j) continue;
            sum += matrix1[(ii + STR) % STR][(jj + COLS) % COLS];
        }
    }

    return sum;
}

int makeDecision(int neighbors, int condition) {
    int live_dead = -1;
    if ((neighbors == 2 || neighbors == 3) && condition == 1) {
        live_dead = 1;
    } else if (neighbors == 3 && condition == 0) {
        live_dead = 1;
    } else {
        live_dead = 0;
    }
    return live_dead;
}

void replace_matr(int matrix2[STR][COLS], int matrix1[STR][COLS]) {
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix1[i][j] = matrix2[i][j];
        }
    }
}

int compare_matr(int matrix1[STR][COLS], int matrix2[STR][COLS]) {
    int count = 0;
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix1[i][j] == matrix2[i][j]) count++;
        }
    }
    return count;
}

int speed(char control_button, int *flag, int time) {
    if (control_button == '1')
        time = 900;
    else if (control_button == '2')
        time = 400;
    else if (control_button == '3')
        time = 70;
    else if (control_button == 'q')
        *flag = 1;

    return time;
}

int countCells(int matr[STR][COLS]) {
    int sum = 0;
    for (int i = 0; i < STR; i++) {
        for (int j = 0; j < COLS; j++) {
            sum += matr[i][j];
        }
    }
    return sum;
}
