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

/*
 *PredatorおよびPreyの位置を表す構造体
 */
typedef struct{
  int x;
  int y;
} Position;

const int prey_INF = 30;          // エッジが存在しない場合の表現(無限大の距離)
int prey_adj[NODE_NUM][NODE_NUM]; // 隣接行列本体
int prey_dp[NODE_NUM][NODE_NUM];  // 最短経路

// int predator_adj[NODE_NUM][NODE_NUM];

static int PreyMap[WORLD_SIZE][WORLD_SIZE] = {{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
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

void PreyMapUpdate(int field[16][16], int flag[4]){
    for(int i = 0; i < WORLD_SIZE; i++){
        for(int j = 0; j < WORLD_SIZE; j++){
            if(field[i][j] != VALUE_OF_UNREACH){
                PreyMap[i][j] = field[i][j];
            }
            if(field[i][j] == VALUE_OF_PREDATOR) flag[0] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 1) flag[1] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 2) flag[2] = 1;
            if(field[i][j] == VALUE_OF_PREDATOR + 3) flag[3] = 1;
        }
    }
}


Position PreyGetPosition(int n){
    int i, j;
    Position position;
    for(i = 0; i < WORLD_SIZE; i++){
        for(j = 0; j < WORLD_SIZE; j++){
            if(PreyMap[i][j] == n){
                position.y = i; // 行
                position.x = j; // 列
            }
        }
    }
    return position;
}

// ノード位置を返す(配列サイズ : 256)
int PreyNodePos(Position pos){
    return pos.y * WORLD_SIZE + pos.x;
}

void InitializeArray(int matrix[WORLD_SIZE][WORLD_SIZE]){
    int i, j;
    for(i = 0; i < WORLD_SIZE; i++){
        for(j = 0; j < WORLD_SIZE; j++){
            matrix[i][j] = 0;
        }
    }
}

void Prey(int field[16][16], int *point){
    // char act = 'r';
    int findFlag[] = {0, 0, 0, 0};
    float action[4];
    int max = 0;
    int index, i, j;
    char act[] = {'u', 'd', 'l', 'r'};

    float evalDistance[WORLD_SIZE][WORLD_SIZE];
    float evalCorner[WORLD_SIZE][WORLD_SIZE];
    float evalObstacle[WORLD_SIZE][WORLD_SIZE];
    float evalTotal[WORLD_SIZE][WORLD_SIZE];

    InitializeArray(evalDistance);
    InitializeArray(evalCorner);
    InitializeArray(evalObstacle);

    // マップの更新, Predatorの発見
    PreyMapUpdate(field, findFlag);
    
    

    // 隣接行列を求める
    AdjacementMatrix();

    // 隣接行列をコピー
    CopyArray();
    
    // 最短距離を求める
    FloydWarshall();

    // Predatorがいれば、逃走配列の更新
    
    for(i = 0; i < NUM_OF_PREDATOR; i++){
        if(findFlag[i] == 1){
            // 探索処理 ワ―シャルフロイド法
            Position predatorPosition = PreyGetPosition(i);
            PreyDistance(evalDistance, PreyNodePos(predatorPosition));
            printf("Predator%dに見つかった\n", i);
        }
    }
    
    // 障害物、角の評価
    preyCreateEvalMapSurround(evalObstacle);
    preyCreateEvalMapCorner(evalCorner);

    // すべての評価を加算
    preySumOfProducts(evalTotal, evalDistance, evalObstacle, evalCorner);

    // for(i = 0; i < WORLD_SIZE; i++){
    //     for(j = 0; j < WORLD_SIZE; j++){
    //         printf("%.3f ", evalTotal[i][j]);
    //     }
    //     printf("\n");
    // }

    // 評価が最も高い方に移動
    for(i = 0; i < 4; i++){
        action[i] = 0.0;
    }

    // Preyの位置を取得
    Position preyPosition = PreyGetPosition(VALUE_OF_PREY);

    if(preyPosition.y - 1 >= 0){
        action[0] = evalTotal[preyPosition.y - 1][preyPosition.x];
    }
    if(preyPosition.y + 1 < WORLD_SIZE){
        action[1] = evalTotal[preyPosition.y + 1][preyPosition.x];
    }
    if(preyPosition.x - 1 >= 0){
        action[2] = evalTotal[preyPosition.y][preyPosition.x - 1];
    }
    if(preyPosition.x + 1 < WORLD_SIZE){
        action[3] = evalTotal[preyPosition.y][preyPosition.x + 1];
    }
    max = action[0];
    for(i = 1; i < 4; i++){
        if(max < action[i]){
            max = action[i];
            printf("%f\n", max);
            index = i;
        }
    }

    *point = (int)act[index];
    printf("%d\n", *point);
    
}

// shortestMap[256][256] ⇒ Distancemap[16][16]
// 隣接行列を用いてワ―シャルフロイド法により求めた最短距離を使って、16×16のマップにPredatorからの距離に当てはめる
void PreyDistance(float map[WORLD_SIZE][WORLD_SIZE], int predatorNodePos){
    int from = 0;  // ノード番号は0から走査
    float evaluation;     // 評価値
    for (int i = 0; i < WORLD_SIZE; i++) {
        for (int j = 0; j < WORLD_SIZE; j++) {
        /*
        * Predatorまでの距離を正規化
        * prey_dp[from][predatorNodeNum]:Predatorまでの距離
        * 評価値は距離0で0.0
        */
        if (prey_dp[from][predatorNodePos] == prey_INF) {
            evaluation = 0.0;
        } else {
            evaluation = (float)(prey_dp[from][predatorNodePos]) / prey_INF;
        }
        map[i][j] += evaluation;
        from++;
        }
    }
}


// 隣接行列の作成
void AdjacementMatrix(){
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
      if (PreyMap[i][j] != VALUE_OF_OBSTACLE && PreyMap[i][j+1] != VALUE_OF_OBSTACLE) {
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
      if (PreyMap[j][i] != VALUE_OF_OBSTACLE && PreyMap[j+1][i] != VALUE_OF_OBSTACLE) {
        prey_adj[from][to] = 1;
        prey_adj[to][from] = 1;
      }

      from += WORLD_SIZE;
      to = from + WORLD_SIZE;
    }
  }

}


void CopyArray(){
    int i, j;
    for(i = 0; i < NODE_NUM; i++){
        for(j = 0; j < NODE_NUM; j++){
            prey_dp[i][j] = prey_adj[i][j];
        }
    }
}

void FloydWarshall(){
    int i, j, k;

    for(i = 0; i < NODE_NUM; i++){
        for(j = 0; j < NODE_NUM; j++){
            if (prey_dp[i][j] != 1) {        // 隣接していなければ
                if (i == j) continue;      // 自身への距離は0
                prey_dp[i][j] = prey_INF;     // 障害物との距離は無限
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



/*
 * 格子世界の角隅となる場所に負の評価づけをし、評価マップを生成
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      :格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * float evalMap[WORLD_SIZE][WORLD_SIZE]
 *        :各座標における評価値を保存するWORLD_SIZE*WORLD_SIZEのfloat型二次元配列
 */
void preyCreateEvalMapCorner(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;

  /* すべて0で初期化*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      evalMap[i][j] = 0.0;
    }
  }

  /* 各座標への評価づけ*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (PreyMap[i][j] == VALUE_OF_OBSTACLE) {
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

/*
 * 障害物に囲まれる場所に負の評価づけをし、評価マップを生成
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      :格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * float evalMap[WORLD_SIZE][WORLD_SIZE]
 *        :各座標における評価値を保存するWORLD_SIZE*WORLD_SIZEのfloat型二次元配列
 */
void preyCreateEvalMapSurround(float evalMap[WORLD_SIZE][WORLD_SIZE]){
  int i, j;

  /* すべて0で初期化*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      evalMap[i][j] = 0.0;
    }
  }

  /* 障害物に囲まれた場所とその周辺に重み付け*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
     if (PreyMap[i][j] == VALUE_OF_OBSTACLE) continue;

      /*上と右の確認*/
      if (PreyMap[i - 1][j] == VALUE_OF_OBSTACLE && PreyMap[i][j + 1] == VALUE_OF_OBSTACLE) {
            evalMap[i][j] += 2.0;
            evalMap[i + 1][j] += 1.0;
            evalMap[i][j - 1] += 1.0;
       }
      /*下と右の確認*/
      if (PreyMap[i + 1][j] == VALUE_OF_OBSTACLE && PreyMap[i][j + 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j - 1] += 1.0;
       }
      /*上と左の確認*/
      if (PreyMap[i - 1][j] == VALUE_OF_OBSTACLE && PreyMap[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i + 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
      /*下と左の確認*/
      if (PreyMap[i + 1][j] == VALUE_OF_OBSTACLE && PreyMap[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
     }
  }

  /* 障害物の評価を最低値にする*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (PreyMap[i][j] == VALUE_OF_OBSTACLE)
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


/*
 * 3つのマップを積和合成する
 * float resultMap[WORLD_SIZE][WORLD_SIZE]
 *        :合成したマップを格納するWORLD_SIZE*WORLD_SIZEのfloat型二次元配列
 * float map_a[WORLD_SIZE][WORLD_SIZE]
 * float map_b[WORLD_SIZE][WORLD_SIZE]
 * float map_c[WORLD_SIZE][WORLD_SIZE]
 *        :合成するマップ
 */
void preySumOfProducts(float resultMap[WORLD_SIZE][WORLD_SIZE], float map_a[WORLD_SIZE][WORLD_SIZE], float map_b[WORLD_SIZE][WORLD_SIZE], float map_c[WORLD_SIZE][WORLD_SIZE]){
  
  int i, j;
  const float a_ratio = 1;  // マップAの影響比率
  const float b_ratio = 0.1;  // マップBの影響比率
  const float c_ratio = 0.1;  // マップBの影響比率

  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      // resultMap[i][j] = map_a[i][j]+ map_b[i][j] * b_ratio + map_c[i][j] * c_ratio;
      resultMap[i][j] = map_a[i][j] * a_ratio + map_b[i][j] * b_ratio + map_c[i][j] * c_ratio;
    }
  }
}