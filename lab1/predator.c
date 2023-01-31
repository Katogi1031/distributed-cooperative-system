/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created November 2, 2022
Language : C

Function : predator(int *ca, int action)
Description : Using the A* algorithm, find the shortest distance between Predator and Prey and choose an action.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define predator_E 0.000001 // ニュートン法で求める際の閾値
int predator_field[8][8]; // フィールド情報を持つグローバルな配列

// 親ノードや座標、移動コストを持つ構造体
struct predatorNode{
  struct predatorPoint* pnt;
  struct predatorNode* parent;
  int g;
  int h;
};

// 座標の構造体
struct predatorPoint{
  int x;
  int y;
};

// ニュートン法を用いてルートの近似値を求める
double predatorSqrt(double a)
{
    a = a < 0 ? -a : a;
    double x = a / 2;
    while (1) {
      double e = x * x - a;
      e = e < 0 ? -e : e;
      if (e < predator_E) return x;
      x -= e / (x * 2);
    }
}

/* 関数の呼び出し順に宣言したいための処置 */
struct predatorNode* predatorAStar(struct predatorNode* current, struct predatorNode* goal, struct predatorNode **openList, int l1, struct predatorNode** losedList, int l2);
int predatorSearch(struct predatorNode* current, struct predatorNode **openList, int l1, struct predatorNode **closedList, int l2);
int predatorCalcCost(struct predatorNode **openList, struct predatorNode* goalNode, int l1);
struct predatorPoint* predatorRetrace(struct predatorNode* goalNode);

/* AStarAlgorithm */
/* A*アルゴリズムを用いてスタートノードからゴールノードまでの最短距離を求める */
struct predatorNode* predatorAStar(struct predatorNode* current, struct predatorNode* goal, struct predatorNode **openList, int l1, struct predatorNode** closedList, int l2){
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = predatorSearch(current, openList, l1, closedList, l2);
  /* 経路コストを計算する */
  nextIndex = predatorCalcCost(openList, goal, l1);
  struct predatorNode *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct predatorNode *tempList = (struct predatorNode*)calloc(l1-1, sizeof(struct predatorNode));
  for(i = 0, j=0; i < l1; i++){
    if(i != nextIndex){
      memcpy(&tempList[j], &((*openList)[i]), sizeof(struct predatorNode));
      j++;
    } 
  }
  l1--, l2++;
  *openList = tempList;

  /* クローズドノードに追加していく */
  *closedList = (struct predatorNode*)realloc((*closedList), l2*(sizeof(struct predatorNode)));
  memcpy(&((*closedList)[l2-1]), nextNode, sizeof(struct predatorNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  return nextNode;
  return predatorAStar(nextNode, goal, openList, l1, closedList, l2);
}

/* predatorSearch */
/* 現在地から移動可能なノードを拡張する */
int predatorSearch(struct predatorNode* current, struct predatorNode **openList, int l1, struct predatorNode **closedList, int l2){
  int i, j, count, found;
  count = 0;

  // ４方向のうち、移動可能なノードを拡張する
  struct predatorNode* tempList = (struct predatorNode*)calloc(4, sizeof(struct predatorNode));   
   for(i = 0; i < 4; i++){
      tempList[i].pnt = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
      if(i == 0 && predator_field[current->pnt->x-1][current->pnt->y] != -1)  // 上に障害物がないか
        tempList[i].pnt->x = current->pnt->x-1, tempList[i].pnt->y = current->pnt->y, tempList[i].g = current->g+1;
      else if(i == 3 && predator_field[current->pnt->x+1][current->pnt->y] != -1) // 下に障害物がないか
        tempList[i].pnt->x = current->pnt->x+1,tempList[i].pnt->y = current->pnt->y, tempList[i].g = current->g+1;
      else if(i == 1 && predator_field[current->pnt->x][current->pnt->y+1] != -1) // 右に障害物がないか
        tempList[i].pnt->x = current->pnt->x, tempList[i].pnt->y = current->pnt->y+1, tempList[i].g = current->g+1;
      else if(i == 2 && predator_field[current->pnt->x][current->pnt->y-1] != -1) // 左に障害物がないか
        tempList[i].pnt->x = current->pnt->x, tempList[i].pnt->y = current->pnt->y-1, tempList[i].g = current->g+1;

      tempList[i].parent = current;
   }

  /* 拡張したノードががオープンリスト、クローズドリストになければ追加する*/
  for(j = 0; j < 4; j++){
      found = 0;           
      if(tempList[j].parent == NULL) return 0;
      for(i = 0; i < l1; i++) if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y) found++;
      for(i = 0; i < l2; i++) if(tempList[j].pnt->x == (*closedList)[i].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y) found++; 
      if(found == 0){
        count++;
        int total = l1 + count;
        *openList = (struct predatorNode*)realloc((*openList), total*(sizeof(struct predatorNode)));
        memcpy(&((*openList)[total-1]), &tempList[j],sizeof(struct predatorNode));              
      }      
   } 
  return count + l1;
}

/* CalcCost */
/* 現在地からゴールノードまでの仮に見積もった距離を算出する
   オープンリスト内にあるノード全てと現在地までの総移動コストを求め、そのノードのインデックスを求める
 */
int predatorCalcCost(struct predatorNode **openList, struct predatorNode* goalNode, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    // 三平方の定理(現在地からゴールノードまでの仮に見積もった距離)
    (*openList)[i].h = ((int)predatorSqrt(difx * difx + dify * dify)); 
  }
  
  //  
  int min, minIndex;
  min = (*openList)[0].g + (*openList)[0].h;
  minIndex = 0;
  for(i = 1; i < l1; i++){
    if((*openList)[i].g + (*openList)[i].h < min){
      min = (*openList)[i].g + (*openList)[i].h;
      minIndex = i;
    } 
  }
  return minIndex;
}

/* Retrace */
/* ゴールノードから現在地に至るまでのノードに遡り、現在地から次に移動するノードを求める */
struct predatorPoint* predatorRetrace(struct predatorNode* goalNode){
  struct predatorNode* current = goalNode;
  struct predatorPoint* ptr = NULL;
  int steps = 0, i;
  while(current->parent != NULL){
      steps++;
      ptr = (struct predatorPoint*)realloc(ptr, steps*sizeof(struct predatorPoint));
      memcpy(&ptr[steps-1], current->pnt, sizeof(struct predatorPoint));
      current = current->parent;                    
  }
  return &ptr[steps-1];
}
/* Predator */
void Predator(int *ca, int *action){
  long cpu_time;
  int p, q;
  struct predatorPoint *predator = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  struct predatorPoint *prey = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));

  for(int i = 0; i < 64; i++){
    p = i / 8;
    q = i % 8;

    predator_field[p][q] = ca[i];
    
    switch(predator_field[p][q]){
      case 1:   // predatorの位置を求める
        predator->x = p, predator->y = q;
        break;
      case 10:  // preyの位置を求める
        prey->x = p, prey->y = q;
        break;
        
    }
  }

  /* スタートノードの作成 */
  struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  sP->x = predator->x, sP->y = predator->y;
  struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
  
  /* ゴールノードの作成 */
  struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  gP->x = prey->x, gP->y = prey->y;
  struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, startNode->h=0;

  /* オープンリストの作成 */
  struct predatorNode **openList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct predatorNode **closedList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *closedList = NULL;

  (*closedList) = (struct predatorNode*)realloc((*closedList),sizeof(struct predatorNode));
  memcpy(&((*closedList)[0]), startNode, sizeof(struct predatorNode));
  
  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct predatorNode* finished = predatorAStar(startNode, goalNode, openList, 0, closedList, 1);
  /* 現在地から次の移動位置を受け取る */
  struct predatorPoint* nextPosition = predatorRetrace(finished);

  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int point[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  for(int i = 0; i < 4; i++){

    /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
    if(nextPosition->x == predator->x + point[i][0] && nextPosition->y == predator->y + point[i][1]){
      *action = (int)act[i];
    }
  }

  printf("cpu : %d\n", cpu_time);
}

/*
 * Predatorからの距離を正規化し、評価マップを生成
 * float evalMap[WORLD_SIZE][WORLD_SIZE]
 *        :各座標における評価値を保存するWORLD_SIZE*WORLD_SIZEのfloat型二次元配列
 * int predatorNodeNum:Predatorのノード番号
 */
void preyCreateEvalMapDistance(float evalMap[WORLD_SIZE][WORLD_SIZE], int predatorNodeNum)
{
  int fromNodeNum = 0;  // ノード番号は0から走査
  float evaluation;     // 評価値
  for (int i = 0; i < WORLD_SIZE; i++) {
    for (int j = 0; j < WORLD_SIZE; j++) {
       /*
       * Predatorまでの距離を正規化
       * prey_dp[fromNodeNum][predatorNodeNum]:Predatorまでの距離
       * 評価値は距離0で0.0
       */
      if (prey_dp[fromNodeNum][predatorNodeNum] == prey_INF) {
        evaluation = 0.0;
      } else {
        evaluation = (float)(prey_dp[fromNodeNum][predatorNodeNum]) / prey_INF;
       }
      evalMap[i][j] = evaluation;
      fromNodeNum++;
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
void preyCreateEvalMapCorner(int battleFieldArray[WORLD_SIZE][WORLD_SIZE],
                             float evalMap[WORLD_SIZE][WORLD_SIZE])
{
  int i,j;

  /* すべて0で初期化*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      evalMap[i][j] = 0.0;
    }
  }

  /* 各座標への評価づけ*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (battleFieldArray[i][j] == VALUE_OF_OBSTACLE) {
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

  /* 値を反転して正規化*/
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
void preyCreateEvalMapSurround(int battleFieldArray[WORLD_SIZE][WORLD_SIZE],
                               float evalMap[WORLD_SIZE][WORLD_SIZE])
{
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
     if (battleFieldArray[i][j] == VALUE_OF_OBSTACLE) continue;

      /*上と右の確認*/
      if (battleFieldArray[i - 1][j] == VALUE_OF_OBSTACLE
           && battleFieldArray[i][j + 1] == VALUE_OF_OBSTACLE) {
            evalMap[i][j] += 2.0;
            evalMap[i + 1][j] += 1.0;
            evalMap[i][j - 1] += 1.0;
       }
      /*下と右の確認*/
      if (battleFieldArray[i + 1][j] == VALUE_OF_OBSTACLE
           && battleFieldArray[i][j + 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j - 1] += 1.0;
       }
      /*上と左の確認*/
      if (battleFieldArray[i - 1][j] == VALUE_OF_OBSTACLE
           && battleFieldArray[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i + 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
      /*下と左の確認*/
      if (battleFieldArray[i + 1][j] == VALUE_OF_OBSTACLE
           && battleFieldArray[i][j - 1] == VALUE_OF_OBSTACLE) {
      evalMap[i][j] += 2.0;
      evalMap[i - 1][j] += 1.0;
      evalMap[i][j + 1] += 1.0;
       }
     }
  }

  /* 障害物の評価を最低値にする*/
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      if (battleFieldArray[i][j] == VALUE_OF_OBSTACLE)
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
