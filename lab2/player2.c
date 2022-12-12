#include<stdio.h>

// 2つの式の最大値を求める
void maximize(int exp1, int exp2, int *x, int *y) {
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

// int solve(int player1, int player2) {
// 	// プレイヤー1が実際に選択した戦略
// 	int strategy1 = player1;
	
// 	// プレイヤー2が実際に選択した戦略
// 	int strategy2 = player2;
	
// 	// 囚人のジレンマをフェルマーの未定乗数法で解く
// 	int x, y;
// 	maximize(3 * x + y, 3 * y + x, &x, &y);
	
// 	// プレイヤー1が得る利得
// 	int gain1 = 0;
// 	if (strategy1 == 1 && strategy2 == 1) {
// 		// 両者が協調した場合のプレイヤー1が得る利得
// 		gain1 = x;
// 	} else if (strategy1 == 1 && strategy2 == 0) {
// 		// プレイヤー1が協調し、プレイヤー2が裏切った場合のプレイヤー1が得る利得
// 		gain1 = 0;
// 	} else if (strategy1 == 0 && strategy2 == 1) {
// 		// プレイヤー1が裏切り、プレイヤー2が協調した場合のプレイヤー1が得る利得
// 		gain1 = 5;
// 	} else if (strategy1 == 0 && strategy2 == 0) {
// 		// 両者が裏切った場合のプレイヤー1が得る利得
// 		gain1 = 1;
// 	}
// }

// 囚人のジレンマゲームのプレイヤーのアクションを求める
int get_action(int player1, int player2) {
    // プレイヤー1が実際に選択した戦略
    int strategy1 = player1;

    // プレイヤー2が実際に選択した戦略
    int strategy2 = player2;

    // 囚人のジレンマをフェルマーの未定乗数法で解く
    int x, y;
    maximize(3 * x + y, 3 * y + x, &x, &y);

    // プレイヤー1が実際に取るべきアクション
    int action1 = 0;
    if (strategy1 == 1 && strategy2 == 1) {
        // 両者が協調した場合のプレイヤー1が取るべきアクション
        action1 = x;
    } else if (strategy1 == 1 && strategy2 == 0) {
        // プレイヤー1が協調し、プレイヤー2が裏切った場合のプレイヤー1が取るべきアクション
        action1 = y;
    } else if (strategy1 == 0 && strategy2 == 1) {
        // プレイヤー1が裏切り、プレイヤー2が協調した場合のプレイヤー1が取るべきアクション
        action1 = y;
    } else if (strategy1 == 0 && strategy2 == 0) {
        // 両者が裏切った場合のプレイヤー1が取るべきアクション
        action1 = x;
    }

    return action1;
}

Player2(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer2Strategy){

    // int cooperate = 1; // 協調
    // int deception = 0; // 裏切り
    // static int previousPlayer1StrategyArray[10];
    // static int previousPlayer2StrategyArray[10];
    // static int player2chosenStrategy; // 今回player2が選択した戦略
    static int i = 0;
    // // 最初は協調する
    // if (i = 0 && previousPlayer1Strategy == -1 && previousPlayer2Strategy == -1){
    //     *currentPlayer2Strategy = 1;
    // }else{
    //     // 前回の両プレイヤーの戦略を保存する
    //     previousPlayer1StrategyArray[i] = previousPlayer1Strategy;
    //     previousPlayer2StrategyArray[i] = previousPlayer2Strategy;
    //     i++;


    // }
    // // エラー率による戦略反転が起きたかどうかを判定するために保存
    // player2chosenStrategy = *currentPlayer2Strategy;
    if(previousPlayer2Strategy == -1){
        *currentPlayer2Strategy = 1;
        printf("player2 action 1\n");
    }else{
        int num = get_action(previousPlayer1Strategy, previousPlayer2Strategy);
        printf("player2 action %d\n", num);
        *currentPlayer2Strategy = num;
    }
    
    // *currentPlayer2Strategy

}

