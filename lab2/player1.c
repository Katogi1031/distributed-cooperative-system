#include<stdio.h>

Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
    static int array1[10];
    static int i = 0;
    array1[i] = i;
    i++;
    int num = sizeof(array1);
    // printf("num = %d\n", i);
    for(int j = 0; j < i; j++){
        printf("array = %d\n", array1[j]);
    }
    *currentPlayer1Strategy = 1;
}