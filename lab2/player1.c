#include<stdio.h>

// 平衡点推定法
int estimate(int player1, int player2) {
	// プレイヤー1が実際に選択した戦略
	int strategy1 = player1;
	
	// プレイヤー2が実際に選択した戦略
	int strategy2 = player2;
	
	// 平衡点を推定する
	int equilibrium = 0;
	if (strategy1 == 1 && strategy2 == 1) {
		// 両者が協調した場合の平衡点
		equilibrium = 3;
	} else if (strategy1 == 1 && strategy2 == 0) {
		// プレイヤー1が協調し、プレイヤー2が裏切った場合の平衡点
		equilibrium = 0;
	} else if (strategy1 == 0 && strategy2 == 1) {
		// プレイヤー1が裏切り、プレイヤー2が協調した場合の平衡点
		equilibrium = 5;
	} else if (strategy1 == 0 && strategy2 == 0) {
		// 両者が裏切った場合の平衡点
		equilibrium = 1;
	}
	
	return equilibrium;
}

// プレイヤーが選択する戦略を決定する
int decision(int player1, int player2) {
	// 平衡点を推定する
	int equilibrium = estimate(player1, player2);
	
	// プレイヤーが選択する戦略
	int strategy = 0;
	if (equilibrium > 3) {
		// 平衡点が3より大きい場合は協調する
		strategy = 1;
	} else {
		// 平衡点が3以下の場合は裏切る
		strategy = 0;
	}
	
	return strategy;
}

Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
    // static int array1[10];
    static int i = 0;
	static int cnt = 0;
    // array1[i] = i;
    // i++;
    // int num = sizeof(array1);
    // // printf("num = %d\n", i);
    // for(int j = 0; j < i; j++){
    //     printf("array = %d\n", array1[j]);
    // }
    // *currentPlayer1Strategy = 1;
	
	// プレイヤーが選択する戦略を決定する
    if(previousPlayer1Strategy == -1){
        *currentPlayer1Strategy = 1;
        printf("player2 action 1\n");
    }else{
        int num = decision(previousPlayer1Strategy, previousPlayer2Strategy);
        printf("player1 action %d, cnt=%d\n", num, cnt);
        *currentPlayer1Strategy = num;
    }

	if(*currentPlayer1Strategy == 1){
		cnt++;
	}

}
