#include<stdio.h>

// 2つの式の最大値を求める
void player2_maximize(int exp1, int exp2, int *x, int *y) {
	// フェルマーの未定乗数法を用いて最大値を求める
	*x = 0;
	*y = 0;
	while (exp1 > exp2) {
		*x += 1;
		*y += 1;
		exp1 -= exp2;
		exp2 = 3 * *y + *x;
	}
}

// 囚人のジレンマゲームのプレイヤーのアクションを求める
int player2_get_action(int player1Strategy, int player2Strategy) {
    // 囚人のジレンマをフェルマーの未定乗数法で解く
    int x, y;
    player2_maximize(3 * x + y, 3 * y + x, &x, &y);
     printf("x = %d, y = %d\n", exp1, exp2);
    
    if(x >= 1) x = 1;
    if(y >= 1) y = 1;
    // プレイヤー1が実際に取るべきアクション
    int action2 = 0;
    if (player1Strategy == 1 && player2Strategy == 1) {         // 両者が協調した場合
        action2 = x;
    } else if (player1Strategy == 1 && player2Strategy == 0) {  // プレイヤー1が協調し、プレイヤー2が裏切った場合
        action2 = y;
    } else if (player1Strategy == 0 && player2Strategy == 1) {  // プレイヤー1が裏切り、プレイヤー2が協調した場合
        action2 = y;
    } else if (player1Strategy == 0 && player2Strategy == 0) {  // 両者が裏切った場合
        action2 = x;
    }
    return action2;
}

Player2(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer2Strategy){
    // 最初は協調する
    if(previousPlayer2Strategy == -1){
        *currentPlayer2Strategy = 1;
    }else{
        *currentPlayer2Strategy = player2_get_action(previousPlayer1Strategy, previousPlayer2Strategy);
    }
}

