#include<stdio.h>
// 前回の相手の行動をまねる
Player2(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
    int opponent = previousPlayer1Strategy;
    if (opponent == 0){
        *currentPlayer1Strategy = 0;
    }else{
        *currentPlayer1Strategy = 1;
    }
}