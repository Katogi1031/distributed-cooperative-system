#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
    static int player1_score = 0;
    static int player2_score = 0;

    // 前回までの行動によるスコア
    if(previousPlayer1Strategy == 1 && previousPlayer2Strategy == 1){
        player1_score += 3;
        player2_score += 3;
    }else if(previousPlayer1Strategy == 1 && previousPlayer2Strategy == 0){
        // player1_score += 3;
        player2_score += 5;
    }if(previousPlayer1Strategy == 0 && previousPlayer2Strategy == 1){
        player1_score += 5;
        // player2_score += 3;
    }else{
        player1_score += 1;
        player2_score += 1;
    }
    // 自分のスコアが低ければ
    if(player1_score > player2_score){
        *currentPlayer1Strategy = 1;
    }else{  // 自分のスコアが低ければ
        *currentPlayer1Strategy = 0;
    }
}