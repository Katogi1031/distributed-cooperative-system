#include<stdio.h>

int player1_scores(int player1Strategy, int player2Strategy) {
	int score = 0;
	if (player1Strategy == 1 && player2Strategy == 1) {			// 両者が協調した場合
		score = 3;
	} else if (player1Strategy == 1 && player2Strategy == 0) {	// プレイヤー1が協調し、プレイヤー2が裏切った場合
		score = 0;
	} else if (player1Strategy == 0 && player2Strategy == 1) {	// プレイヤー1が裏切り、プレイヤー2が協調した場合
		score = 5;
	} else if (player1Strategy == 0 && player2Strategy == 0) {	// 両者が裏切った場合の平衡点
		score = 1;
	}
	return score;
}

// プレイヤーが選択する戦略を決定する
int player1_get_action(int player1Strategy, int player2Strategy) {
	int score = player1_scores(player1Strategy, player2Strategy);
	printf("score %d\n", score);
	int strategy = 0;
	if (score > 3) {	// 3より大きい場合は協調する
		strategy = 1;
	} else {			// 3以下の場合は裏切る
		strategy = 0;
	}
	return strategy;
}

Player2(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
    if(previousPlayer1Strategy == -1){
        *currentPlayer1Strategy = 1;
        printf("player2 action 1\n");
    }else{
        *currentPlayer1Strategy = player1_get_action(previousPlayer2Strategy, previousPlayer1Strategy);
    }
}
