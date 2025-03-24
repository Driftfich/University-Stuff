#include <stdio.h>
#include <stdlib.h>

#define ROWS 15
#define COLS ROWS

void disb_mat(char mat[ROWS][COLS]) {
    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            printf("%c", mat[i][j]);
        }
        printf("\n");
    }
    return;
}

int main() {
    char mat[ROWS][COLS];

    for (int i=0; i<ROWS; i++) {
        for (int j=0; j<COLS; j++) {
            mat[i][j] = '.';
        }
    }
    disb_mat(mat);
    printf("\n");
    for (int i=0; i<ROWS; i++) {
        mat[i][0] = 'O';
        mat[0][i] = 'O';
    }
    disb_mat(mat);
    return 0;
}
