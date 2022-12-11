#include<stdio.h>

Player2(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer2Strategy){
    static int array1[10];
    static int i = 0;

    array1[i] = i;
    i++;
    int num = sizeof(array1);
    for(int j = 0; j < i; j ++){
        printf("array2 = %d\n", array1[j]);
    }
    *currentPlayer2Strategy = 1;
}