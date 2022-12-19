#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COOPERATE 0
#define BETRAY 1

int main(int argc, char** argv) {
  // シミュレーションの回数
  int num_iterations = 100;
  // 自分と相手のスコア
  int my_score = 0, opponent_score = 0;
  // 乱数の種
  srand(time(NULL));

  for (int i = 0; i < num_iterations; i++) {
    // 今回の行動を決定する
    int my_action;
    if (my_score - opponent_score > 0) {
      // 自分のスコアが高ければ、自分は裏切りをする
      my_action = BETRAY;
    } else {
      // 自分のスコアが低ければ、自分は協調をする
      my_action = COOPERATE;
    }
    // 相手の行動を決定する
    int opponent_action = rand() % 2;
    // スコアを計算する
    if (my_action == COOPERATE && opponent_action == COOPERATE) {
      // 両者とも協調した場合
      my_score += 2;
      opponent_score += 2;
    } else if (my_action == COOPERATE && opponent_action == BETRAY) {
      // 自分は協調したが、相手は裏切りした場合
      my_score -= 1;
      opponent_score += 3;
    } else if (my_action == BETRAY && opponent_action == COOPERATE) {
      // 自分は裏切りしたが、相手は協調した場合
      my_score += 3;
      opponent_score -= 1;
    } else {
      // 両者とも裏切りした場合
      my_score += 1;
      opponent_score += 1;
    }
  }

  // 最終的なスコアを表示する
  printf("My score: %d\n", my_score);
  printf("Opponent score: %d\n", opponent_score);

  return 0;
}