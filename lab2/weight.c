/*  
 Author:       Atsuya Katogi
 Student ID:   b1020003
 Class:        K
 Created:      December 20, 2022
 Language:     C

 Function:     Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy)
 Description:  Variable that returns the past strategies of both players and the strategy calculated from the original algorithm.

*/
#include <stdio.h>
#include <stdlib.h>
Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){

    // 前回自分が実際に選択した行動
    static int player1_choice;

    // エラーにより、前回裏切る選択を取った場合、協調する
    if(previousPlayer1Strategy == 1 && player1_choice == 0){
        *currentPlayer1Strategy = 1;
    }else if(previousPlayer2Strategy == 1){     // 相手が協調した場合、その確率を上げる
        *currentPlayer1Strategy = 1;
    }else{
        if (rand() / (double)RAND_MAX < 0.7) {
            *currentPlayer1Strategy = 1;
        } else {
            *currentPlayer1Strategy = 0;
        }
    }
    player1_choice = *currentPlayer1Strategy;
}