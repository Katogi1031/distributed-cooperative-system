/* 
 Author:       Atsuya Katogi
 Student ID:   b1020003
 Class:        K
 Created:      Februry 7, 2023
 Language:     C

 Function:     Prey(int field[16][16], int *point)
 Description:  Implimentation of an algorithm that escapes the predator from the given arguments

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

/* 格子世界の大きさ */
#ifndef WORLD_SIZE
#define WORLD_SIZE 16
#endif

/* 隣接行列の大きさ */
#ifndef NODE_NUM
#define NODE_NUM 256
#endif
/* Predatorの数値表現(2~4は加算する) */
#ifndef VALUE_OF_PREDATOR
#define VALUE_OF_PREDATOR 1
#endif

/* preyの数値表現 */
#ifndef VALUE_OF_PREY
#define VALUE_OF_PREY 10
#endif

/* obstacleの数値表現 */
#ifndef VALUE_OF_OBSTACLE
#define VALUE_OF_OBSTACLE -1
#endif

/* free gridの数値表現 */
#ifndef VALUE_OF_FREE
#define VALUE_OF_FREE 0
#endif

/* 未探索の地の数値表現 */
#ifndef VALUE_OF_UNREACH
#define VALUE_OF_UNREACH -10
#endif

/* Predatorの数 */
#ifndef NUM_OF_PREDATOR
#define NUM_OF_PREDATOR 4
#endif

/* 対象の位置を表す構造体 */
typedef struct{
  int x;
  int y;
} prey_position;

const int prey_INF = 30;          // エッジが存在しない場合の表現(無限大の距離)
int prey_adj[NODE_NUM][NODE_NUM]; // 隣接行列本体
int prey_dp[NODE_NUM][NODE_NUM];  // 最短経路

/* 初期マップを作成 マップを探索し、マップを更新する*/
static int preyMap[WORLD_SIZE][WORLD_SIZE] = {
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
  {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10}
};

/* 配列を初期化する */
void PreyInitializeArray(int matrix[WORLD_SIZE][WORLD_SIZE]){
    int i, j;
    for(i = 0; i < WORLD_SIZE; i++){
        for(j = 0; j < WORLD_SIZE; j++){
            matrix[i][j] = 0;
        }
    }
}

/* 
視界に入ったマップを逐一更新する
その際、Predatorを見つけたら、どのPredatorに見つかったかを判定する 
*/
void PreyMapUpdate(int field[16][16], int flag[4]){
    for(int i = 0; i < WORLD_SIZE; i++){
        for(int j = 0; j < WORLD_SIZE; j++){
            if(field[i][j] != VALUE_OF_UNREACH && preyMap[i][j] != -5){
                preyMap[i][j] = field[i][j];
            }

            // どのPredatorを見つけたか
            if(field[i][j] == VALUE_OF_PREDATOR) flag[0] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 1) flag[1] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 2) flag[2] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 3) flag[3] = 1;
        }
    }
}


/* 指定した数値(PreyかPredator)で、その対象がいる座標を返す */
prey_position PreyGetPosition(int n){
    int i, j;
    prey_position position;
    position.y = 0;
    position.x = 0;
    for(i = 0; i < WORLD_SIZE; i++){
        for(j = 0; j < WORLD_SIZE; j++){
            if(preyMap[i][j] == n){
                position.y = i; // 行
                position.x = j; // 列
            }
        }
    }
    return position;
}

/* 16×16に対応した座標を256×256に対応した配列に当てはめる */
int PreyNodePos(prey_position pos){
  return pos.y * WORLD_SIZE + pos.x;
}



// 隣接行列を用いてワ―シャルフロイド法により求めた最短距離を使って、16×16のマップにPredatorからの距離に当てはめる
void PreyEvalDistance(float map[WORLD_SIZE][WORLD_SIZE], int predatorNodePos){
    int fromNodeNum = 0;  // ノード番号は0から走査
    float evaluation;     // 評価値
    
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
       
          if (prey_dp[fromNodeNum][predatorNodePos] == prey_INF) {
              evaluation = 0.0;
          } else {
              evaluation = (float)(prey_dp[fromNodeNum][predatorNodePos]) / prey_INF;
          }
          map[i][j] += evaluation;
          fromNodeNum++;
        }
    }
}


// 隣接行列の作成
void PreyAdjacementMatrix(){
  int from = -1;      // 走査するノード, 最初のループに入ったときに0になる
  int to = from + 1;  // fromの右隣のノード
  int i, j;

  /* 隣接行列を0で初期化*/
  for (i = 0; i < NODE_NUM; i++) {
    for (j = 0; j < NODE_NUM; j++) {
      prey_adj[i][j] = 0;
    }
  }

  /* x軸方向のエッジを検出*/
  for (i = 0; i < WORLD_SIZE; i++) {
    /* 走査するノードを１列右にシフト*/
    from += 1;
    to = from + 1;

    for (j = 0; j < WORLD_SIZE - 1; j++) {
      if (preyMap[i][j] != VALUE_OF_OBSTACLE && preyMap[i][j+1] != VALUE_OF_OBSTACLE) {
        prey_adj[from][to] = 1;
        prey_adj[to][from] = 1;
      }

      from++;
      to++;
    }
  }
  from = (WORLD_SIZE * (WORLD_SIZE - 1) - 1);  // 最初のループに入ったときに0になる
  to = from + WORLD_SIZE;                      // fromの下のノード

  /* y軸方向のエッジを検出*/
  for (i = 0; i < WORLD_SIZE; i++) {
    /* 走査するノードを１列右にシフト*/
    from -= (WORLD_SIZE * (WORLD_SIZE - 1) - 1);
    to = from + WORLD_SIZE;

    for (j = 0; j < WORLD_SIZE - 1; j++) {
      if (preyMap[j][i] != VALUE_OF_OBSTACLE && preyMap[j+1][i] != VALUE_OF_OBSTACLE) {
        prey_adj[from][to] = 1;
        prey_adj[to][from] = 1;
      }
      from += WORLD_SIZE;
      to = from + WORLD_SIZE;
    }
  }

}

/* 隣接行列をコピー */
void PreyCopyArray(){
    int i, j;
    for(i = 0; i < NODE_NUM; i++){
        for(j = 0; j < NODE_NUM; j++){
            prey_dp[i][j] = prey_adj[i][j];
        }
    }
}

void PreyFloydWarshall(){
    int i, j, k;

    for(i = 0; i < NODE_NUM; i++){
        for(j = 0; j < NODE_NUM; j++){
            if (prey_dp[i][j] != 1) {        // 隣接していなければ
                if (i == j) continue;        // 自身への距離は0
                prey_dp[i][j] = prey_INF;    // 障害物との距離は大きく
            }
        }
    }

    /* ワーシャルフロイド法*/
    for (int k = 0; k < NODE_NUM; k++) {
        for(int i = 0; i < NODE_NUM; i++) {
            for (int j = 0; j < NODE_NUM; j++) {
                if (prey_dp[i][j] > prey_dp[i][k] + prey_dp[k][j])
                    prey_dp[i][j] = prey_dp[i][k] + prey_dp[k][j];
            }
        }
    }
}



/* 角にあるかどうかを評価 */
void PreyEvalCorner(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;
  /* 各座標への評価づけ*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (preyMap[i][j] == VALUE_OF_OBSTACLE) {
          /* 障害物は最低値*/
         evalMap[i][j] = 10.0;
      } else {
          /* 右上*/
         if (j + 1 >= WORLD_SIZE && i - 1 < 0) {
           evalMap[i][j] = 3.0;
           evalMap[i + 1][j] = 2.0;
           evalMap[i][j - 1] = 2.0;
          }
          /* 右下*/
         if (j + 1 >= WORLD_SIZE && i + 1 >= WORLD_SIZE) {
           evalMap[i][j] = 3.0;
           evalMap[i - 1][j] = 2.0;
           evalMap[i][j - 1] = 2.0;
          }
          /* 左下*/
         if (j - 1 < 0 && i + 1 >= WORLD_SIZE) {
           evalMap[i][j] = 3.0;
           evalMap[i - 1][j] = 2.0;
           evalMap[i][j + 1] = 2.0;
          }
          /* 左上*/
         if (j - 1 < 0 && i - 1 < 0) {
           evalMap[i][j] = 3.0;
           evalMap[i + 1][j] = 2.0;
           evalMap[i][j + 1] = 2.0;
          }
       }
    }
  }

  /* 正規化*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      evalMap[i][j] = 1.0 - evalMap[i][j] / 10.0;
    }
  }
}

/* 障害物により、移動価値をを評価 */
void PreyEvalAbstacle(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;

  /* 障害物が周囲にあるかを評価 */
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
     if (preyMap[i][j] == VALUE_OF_OBSTACLE) continue;

      /*上と右の確認*/
      if (preyMap[i - 1][j] == VALUE_OF_OBSTACLE && preyMap[i][j + 1] == VALUE_OF_OBSTACLE) {
            evalMap[i][j] += 2.0;
            evalMap[i + 1][j] += 1.0;
            evalMap[i][j - 1] += 1.0;
       }
      /*下と右の確認*/
      if (preyMap[i + 1][j] == VALUE_OF_OBSTACLE && preyMap[i][j + 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j - 1] += 1.0;
       }
      /*上と左の確認*/
      if (preyMap[i - 1][j] == VALUE_OF_OBSTACLE && preyMap[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i + 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
      /*下と左の確認*/
      if (preyMap[i + 1][j] == VALUE_OF_OBSTACLE && preyMap[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
     }
  }

  /* 障害物の評価を最低値にする*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (preyMap[i][j] == VALUE_OF_OBSTACLE)
       evalMap[i][j] = 10.0;
    }
  }

  /* 値を反転して正規化*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      evalMap[i][j] = 1.0 - evalMap[i][j] / 10.0;
    }
  }
}

/* heatMapを正規化 */
void PreyEvalUnreach(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;
  PreyMakeHeatMap(evalMap);

  // 正規化
  for(i = 0; i < WORLD_SIZE; i++){
    for(j = 0; j < WORLD_SIZE; j++){
      evalMap[i][j] /= 50;
    }
  }
}

/* 壁際かどうかを評価 */
void PreyEvalSide(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;
  float penalty = 0.4;

  for(i = 0; i < WORLD_SIZE; i++){
    for(j = 0; j < WORLD_SIZE; j++){
      if((i != 0 && i != 15) && (j != 0 && j != 15)){
        evalMap[i][j] = 1;
      }
      if(preyMap[i + 1][j] == VALUE_OF_OBSTACLE){
        evalMap[i][j] -= penalty;
      }
      if(preyMap[i - 1][j] == VALUE_OF_OBSTACLE){
        evalMap[i][j] -= penalty;
      }
      if(preyMap[i][j + 1] == VALUE_OF_OBSTACLE){
        evalMap[i][j] -= penalty;
      }
      if(preyMap[i][j - 1] == VALUE_OF_OBSTACLE){
        evalMap[i][j] -= penalty;
      }
    }
  }
}

/* すべての評価マップを加算 */
void PreySumEval(float resultMap[16][16], float map_a[16][16], float map_b[16][16], float map_c[16][16], float map_d[16][16], float map_e[16][16]){
  
  int i, j;
  // 各評価の重み
  const float a_ratio = 0.6;  
  const float b_ratio = 0.2;  
  const float c_ratio = 0.4;
  const float d_ratio = 0.8;  
  const float e_ratio = 0.4;

  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      resultMap[i][j] = map_a[i][j] * a_ratio + map_b[i][j] * b_ratio + map_c[i][j] * c_ratio + map_d[i][j] * d_ratio + map_e[i][j] * e_ratio;
    }
  }
}

/* 周囲などが未探索であるほど加算、見探索な場所を優先して探索するようにする*/
void PreyMakeHeatMap(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;

  for(i = 0; i < WORLD_SIZE; i++){
    for(j = 0; j < WORLD_SIZE; j++){
      if(preyMap[i][j] == VALUE_OF_UNREACH){
        evalMap[i][j] += 10;
      }
      if(preyMap[i+1][j] == VALUE_OF_UNREACH && i+1 < 16){
        evalMap[i][j] += 10;
      }
      if(preyMap[i-1][j] == VALUE_OF_UNREACH && 0 <= i+1){
        evalMap[i][j] += 10;
      }
      if(preyMap[i][j+1] == VALUE_OF_UNREACH && j+1 < 16){
        evalMap[i][j] += 10;
      }
      if(preyMap[i][j-1] == VALUE_OF_UNREACH && 0 <= j-1){
        evalMap[i][j] += 10;
      }
      if(preyMap[i][j] == -5){  // すでに通過した場所は優先度を下げる
        evalMap[i][j] -= 5;
      }
    }
  }
}


/* いくつかのマップ構成をもとに行動を決定する */
void Prey(int field[16][16], int *point){
    // char act = 'r';
    int findFlag[] = {0, 0, 0, 0};
    float action[4];
    float max = 0;
    int index, i, j;
    char act[] = {'u', 'd', 'l', 'r'};

    float evalDistance[WORLD_SIZE][WORLD_SIZE]; // Predatorとの距離
    float evalCorner[WORLD_SIZE][WORLD_SIZE];   // 角
    float evalObstacle[WORLD_SIZE][WORLD_SIZE]; // 障害物
    float evalUnreach[WORLD_SIZE][WORLD_SIZE];  // 未探索
    float evalSide[WORLD_SIZE][WORLD_SIZE];     // 壁際
    float evalTotal[WORLD_SIZE][WORLD_SIZE];    // すべての評価を合計

    // 各配列を初期化(すべて0にする)
    PreyInitializeArray(evalDistance);
    PreyInitializeArray(evalCorner);
    PreyInitializeArray(evalObstacle);
    PreyInitializeArray(evalUnreach);
    PreyInitializeArray(evalSide);

    PreyMapUpdate(field, findFlag); // マップの更新, Predatorの発見

    PreyAdjacementMatrix();             // 隣接行列を求める
    PreyCopyArray();                    // 隣接行列をコピー
    
    PreyFloydWarshall();                // ワ―シャルフロイド法を使って、各ノードから最短距離を求める

    

    // 発見したPredatorからの距離を評価
    for(i = 0; i < NUM_OF_PREDATOR; i++){
        if(findFlag[i] == 1){
            prey_position predatorPosition = PreyGetPosition(i);
            PreyEvalDistance(evalDistance, PreyNodePos(predatorPosition));
        }
    }
    // Preyの位置を取得
    prey_position preyPosition = PreyGetPosition(VALUE_OF_PREY);
    preyMap[preyPosition.y][preyPosition.x] = -5; // Preyが今いる場所の評価を下げる
    
    PreyEvalAbstacle(evalObstacle);  // 障害物の評価
    PreyEvalCorner(evalCorner);      // 角の評価
    PreyEvalUnreach(evalUnreach);    // 未探索であれば、優先して移動するようにする
    PreyEvalSide(evalSide);          // 各ノードの位置が壁際かどうかを評価

    // すべての評価を加算
    PreySumEval(evalTotal, evalDistance, evalObstacle, evalCorner, evalUnreach, evalSide);
    

    // 評価が最も高い方に移動
    for(i = 0; i < 4; i++){
        action[i] = 0.0;
    }

    if(preyPosition.y - 1 >= 0) // 上方向
        action[0] = evalTotal[preyPosition.y - 1][preyPosition.x];
    if(preyPosition.y + 1 < WORLD_SIZE) // 下方向
        action[1] = evalTotal[preyPosition.y + 1][preyPosition.x];
    if(preyPosition.x - 1 >= 0) // 左方向
        action[2] = evalTotal[preyPosition.y][preyPosition.x - 1];
    if(preyPosition.x + 1 < WORLD_SIZE) // 右方向
        action[3] = evalTotal[preyPosition.y][preyPosition.x + 1];
    

    // どの方向に移動するのが最も評価が高いか
    for(i = 0; i < 4; i++){
      if(max < action[i]){
          max = action[i];
          index = i;
      }
    }

    *point = (int)act[index];
}