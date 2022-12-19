#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Player1(int previousPlayer1Strategy, int previousPlayer2Strategy, int *currentPlayer1Strategy){
  int player2_choice;   // 相手の選択
  static int cooperations = 0; // 相手の協力の回数
  static int deception = 0;    // 相手の裏切りの回数

}

int main(){
    int my_score = 0; // 自分のスコア
    int opponent_score = 0; // 相手のスコア
    int choice; // 自分の選択
    
    int previous_opponent_choice = -1; // 相手の直近の行動

    // 乱数の種を設定
    srand((unsigned)time(NULL));

    // 反復囚人のジレンマゲームを200回実施
    for (int i = 0; i < 200; i++){
        // 相手の戦略を予測する
        if (previous_opponent_choice == 0){
						cooperations += 2;
        }
        else if (previous_opponent_choice == 1){
            defections += 2;
        }

        if(cooperations > defections){
            opponent_choice = 0;
        }
        else{
            opponent_choice = 1;
        }

        // 自分が相手の予想を満たすように、選択を行う
        choice = opponent_choice;

        // 選択に応じて、スコアを更新
        if (choice == 0){
            my_score += 3;
            opponent_score += 3;
        }
        else if (choice == 1){
            my_score += 5;
            opponent_score += 0;
        }

        // 相手の直近の行動を記録
        previous_opponent_choice = opponent_choice;
    }

    printf("自分のスコア: %d\n", my_score);
    printf("相手のスコア: %d\n", opponent_score);

    return 0;
}