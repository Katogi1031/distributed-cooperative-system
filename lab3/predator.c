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

static int predator_map[WORLD_SIZE][WORLD_SIZE] = {{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
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

static int predator_history[WORLD_SIZE][WORLD_SIZE] = {  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


static int predator_heatMap[WORLD_SIZE][WORLD_SIZE];

// Preyを見つけたかどうか
int predator_findPrey = 0;

int predator_preyX, predator_preyY; // Preyの位置を保存

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

/* 配列を初期化する */
void PredatorInitializeArray(int matrix[WORLD_SIZE][WORLD_SIZE]){
    int i, j;
    for(i = 0; i < WORLD_SIZE; i++){
        for(j = 0; j < WORLD_SIZE; j++){
            matrix[i][j] = 0;
        }
    }
}

void PredatorUpdateMap(int field1[WORLD_SIZE][WORLD_SIZE], int field2[WORLD_SIZE][WORLD_SIZE], int field3[WORLD_SIZE][WORLD_SIZE], int field4[WORLD_SIZE][WORLD_SIZE]){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field1[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                predator_map[i][j] = field1[i][j];
            }
            if(field2[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                predator_map[i][j] = field2[i][j];
            }
            if(field3[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                predator_map[i][j] = field3[i][j];
            }
            if(field4[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                predator_map[i][j] = field4[i][j];
            }
            if(predator_map[i][j] == 10){  // Preyが存在すれば
                predator_findPrey = 1; 
                predator_preyY = i;
                predator_preyX = j;
            }
        }
    }
}

void Predator(int field1[WORLD_SIZE][WORLD_SIZE], int field2[WORLD_SIZE][WORLD_SIZE], int field3[WORLD_SIZE][WORLD_SIZE], int field4[WORLD_SIZE][WORLD_SIZE], int *point1, int *point2, int *point3, int *point4){
  
  // 各predatorの視界からマップを更新
  PredatorUpdateMap(field1, field2, field3, field4);

  // predatorの行動戦略を取得
  PredatorMakeHeatMap();
  Predator1(field1, point1);
  Predator2(field2, point2);
  Predator3(field3, point3);
  Predator4(field4, point4);
}

/* 関数の呼び出し順に宣言したいための処置 */
// struct predatorNode* PredatorAStar(struct predatorNode* current, struct predatorNode* goal, struct predatorNode **openList, int l1, struct predatorNode** losedList, int l2);
// int PredatorSearch(struct predatorNode* current, struct predatorNode **openList, int l1, struct predatorNode **closedList, int l2);
// int PredatorCalcCost(struct predatorNode **openList, struct predatorNode* goalNode, int l1);
// struct predatorPoint* PredatorRetrace(struct predatorNode* goalNode);

/* AStarAlgorithm */
/* A*アルゴリズムを用いてスタートノードからゴールノードまでの最短距離を求める */
struct predatorNode* PredatorAStar(struct predatorNode* current, struct predatorNode* goal, struct predatorNode **openList, int l1, struct predatorNode** closedList, int l2){
  int i, j, nextIndex;
  int cnt = 0;

  /* オープンリストの長さを求める */
  l1 = PredatorSearch(current, openList, l1, closedList, l2);
  /* 経路コストを計算する */
  nextIndex = PredatorCalcCost(openList, goal, l1);
  struct predatorNode *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct predatorNode *tempList = calloc(l1-1, sizeof(struct predatorNode));

  for(i = 0, j=0; i < l1; i++){
    if(i != nextIndex){
      memcpy(&tempList[j], &((*openList)[i]), sizeof(struct predatorNode));
      j++;
    } 
  }
  l1--, l2++;
  *openList = tempList;

  /* クローズドノードに追加していく */
  *closedList = realloc((*closedList), l2*(sizeof(struct predatorNode)));
  memcpy(&((*closedList)[l2-1]), nextNode, sizeof(struct predatorNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  return nextNode;
  return PredatorAStar(nextNode, goal, openList, l1, closedList, l2);
}

/* predatorSearch */
/* 現在地から移動可能なノードを拡張する */
int PredatorSearch(struct predatorNode* current, struct predatorNode **openList, int l1, struct predatorNode **closedList, int l2){
  int i, j, count, found;
  count = 0;

  // ４方向のうち、移動可能なノードを拡張する
  struct predatorNode* tempList = calloc(4, sizeof(struct predatorNode));   
  for(i = 0; i < 4; i++){
    tempList[i].pnt = malloc(sizeof(struct predatorPoint));
    if(i == 0 && predator_map[current->pnt->y-1][current->pnt->x] != -1 && predator_history[current->pnt->y-1][current->pnt->x] < 3 && 0 <= current->pnt->y-1)      // 上に障害物がないか
      tempList[i].pnt->y = current->pnt->y-1, tempList[i].pnt->x = current->pnt->x, tempList[i].g = current->g+1;
    else if(i == 3 && predator_map[current->pnt->y+1][current->pnt->x] != -1 && predator_history[current->pnt->y+1][current->pnt->x] < 3 && current->pnt->y+1 < 16) // 下に障害物がないか
      tempList[i].pnt->y = current->pnt->y+1,tempList[i].pnt->x = current->pnt->x, tempList[i].g = current->g+1;
    else if(i == 1 && predator_map[current->pnt->y][current->pnt->x+1] != -1 && predator_history[current->pnt->y][current->pnt->x+1] < 3 && current->pnt->x+1 < 16) // 右に障害物がないか
      tempList[i].pnt->y = current->pnt->y, tempList[i].pnt->x = current->pnt->x+1, tempList[i].g = current->g+1;
    else if(i == 2 && predator_map[current->pnt->y][current->pnt->x-1] != -1 && predator_history[current->pnt->y][current->pnt->x-1] < 3 && 0 <= current->pnt->x-1) // 左に障害物がないか
      tempList[i].pnt->y = current->pnt->y, tempList[i].pnt->x = current->pnt->x-1, tempList[i].g = current->g+1;

    tempList[i].parent = current;
  }

  /* 拡張したノードがオープンリスト、クローズドリストになければ追加する*/
  for(j = 0; j < 4; j++){
      found = 0;           
      if(tempList[j].parent == NULL) return 0;
      for(i = 0; i < l1; i++) if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y) found++;
      for(i = 0; i < l2; i++) if(tempList[j].pnt->x == (*closedList)[i].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y) found++; 
      if(found == 0){
        count++;
        int total = l1 + count;
        *openList = realloc((*openList), total*(sizeof(struct predatorNode)));
        memcpy(&((*openList)[total-1]), &tempList[j],sizeof(struct predatorNode));              
      }      
   } 
  return count + l1;
}

/* CalcCost */
/* 現在地からゴールノードまでの仮に見積もった距離を算出する
   オープンリスト内にあるノード全てと現在地までの総移動コストを求め、そのノードのインデックスを求める
 */
int PredatorCalcCost(struct predatorNode **openList, struct predatorNode* goalNode, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = abs((*openList)[i].pnt->x - goalNode->pnt->x);
    dify = abs((*openList)[i].pnt->y - goalNode->pnt->y);
    // マンハッタン距離(現在地からゴールノードまでの仮に見積もった距離)
    (*openList)[i].h = difx+dify; 
  }
  
    
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
struct predatorPoint* PredatorRetrace(struct predatorNode* goalNode){
  struct predatorNode* current = goalNode;
  struct predatorPoint* ptr = NULL;
  int steps = 0, i;
  while(current->parent != NULL){
      steps++;
      ptr = realloc(ptr, steps*sizeof(struct predatorPoint));
      memcpy(&ptr[steps-1], current->pnt, sizeof(struct predatorPoint));
      current = current->parent;                    
  }
  return &ptr[steps-1];
}

void Predator1(int* field, int* point){
    *point = PredatorAct(1);
}

void Predator2(int* field, int* point){
   
    *point = PredatorAct(2);    
}

void Predator3(int* field, int* point){
    *point = PredatorAct(3);
    
}

void Predator4(int* field, int* point){
    *point = PredatorAct(4);
}

int PredatorAct(int n){
  int actNum[] = {117, 100, 108, 114};
  int actP[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  int i;
    
  /* スタートノードの作成 */
  struct predatorPoint* sP = malloc(sizeof(struct predatorPoint));
  /* ゴールノードの作成 */
  struct predatorPoint* gP = malloc(sizeof(struct predatorPoint));
  PredatorPosition(sP, n);

  if(predator_findPrey == 1){
      gP->x = predator_preyX, gP->y = predator_preyY;
  }else if(predator_findPrey == 0){
    PredatorShortestManhattanDistance(sP, gP);
  }

  struct predatorNode* startNode = malloc(sizeof(struct predatorNode));
  startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
  struct predatorNode* goalNode = malloc(sizeof(struct predatorNode));
  goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

  /* オープンリストの作成 */
  struct predatorNode **openList = malloc(sizeof(struct predatorNode*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct predatorNode **closedList = malloc(sizeof(struct predatorNode*));
  *closedList = NULL;
  *closedList = realloc((*closedList),sizeof(struct predatorNode));
  memcpy(&((*closedList)[0]), startNode, sizeof(struct predatorNode));

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct predatorNode* finished = PredatorAStar(startNode, goalNode, openList, 0, closedList, 1);
  /* 現在地から次の移動位置を受け取る */
  struct predatorPoint* nextPosition = PredatorRetrace(finished);
  for(i = 0; i < 4; i++){
      /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
      if(nextPosition->x == sP->x + actP[i][1] && nextPosition->y == sP->y + actP[i][0]){
          break;
      }
  }
  predator_history[sP->y][sP->x] += 1;
  return actNum[i];
}


    

void PredatorPosition(struct predatorPoint* pos, int n){
    int i, j;
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            if(predator_map[i][j] == n){
                pos->y = i; // 行
                pos->x = j; // 列
            }
        }
    }
}

void PredatorShortestManhattanDistance(struct predatorPoint* sP, struct predatorPoint* gP){
  int i, j;
  int max = 0;
  int min = 100;
  int distance;

  for(i = 0; i < WORLD_SIZE; i++){
    for(j = 0; j < WORLD_SIZE; j++){
      if(predator_heatMap[i][j] >= max){
        distance = abs(i - sP->y) + abs(j - sP->x);
        if(distance <= min){
          min = distance;
          max = predator_heatMap[i][j];
          gP->y = i;
          gP->x = j;
        } 
      }
    }
  }
}

void PredatorMakeHeatMap(){
  int i, j;
  PredatorInitializeArray(predator_heatMap);

  for(i = 0; i < WORLD_SIZE; i++){
    for(j = 0; j < WORLD_SIZE; j++){
      if(predator_map[i][j] == VALUE_OF_UNREACH){
        predator_heatMap[i][j] += 10;
      }
      if(predator_map[i+1][j] == VALUE_OF_UNREACH&& i+1 < 16){
        predator_heatMap[i][j] += 10;
      }
      if(predator_map[i-1][j] == VALUE_OF_UNREACH&& 0 <= i+1){
        predator_heatMap[i][j] += 10;
      }
      if(predator_map[i][j+1] == VALUE_OF_UNREACH&& j+1 < 16){
        predator_heatMap[i][j] += 10;
      }
      if(predator_map[i][j-1] == VALUE_OF_UNREACH&& 0 <= j-1){
        predator_heatMap[i][j] += 10;
      }
    }
  }
}
