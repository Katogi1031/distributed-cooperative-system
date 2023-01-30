#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

/* 見探索の地の数値表現 */
#ifndef VALUE_OF_UNREACH
#define VALUE_OF_UNREACH -10
#endif

static int map[WORLD_SIZE][WORLD_SIZE] = {{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
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

static int history[WORLD_SIZE][WORLD_SIZE] = {  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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


// Preyを見つけたかどうか
int findPrey = 0;
// Preyの4方向にPredatorがいるか
int top = 0;
int down = 0;
int left = 0;
int right = 0;
int preyX, preyY;

int predator_adj[NODE_NUM][NODE_NUM];

#define predator_E 0.000001 // ニュートン法で求める際の閾値

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

void MapUpdate(int field[16][16]){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                map[i][j] = field[i][j];
            }
            if(field[i][j] == 10){  // Preyが存在すれば
                findPrey = 1; 
                preyY = i;
                preyX = j;
            }
        }
    }
}

void Predator(int field1[16][16], int field2[16][16], int field3[16][16], int field4[16][16], int *point1, int *point2, int *point3, int *point4){
    // printf("success");


    // 各predatorの視界からマップを更新
    MapUpdate(field1);
    MapUpdate(field2);
    MapUpdate(field3);
    MapUpdate(field4);

    // mapの出力
    // for(int i = 0; i < 16; i++){
    //     for(int j = 0; j < 16; j++){
    //         printf("%4d", map[i][j]);
    //     }
    //     printf("\n");
    // }

    AdjacementMatrix();
    CheckAdjacent();

    // predatorの行動戦略を取得
    // Predator1(field1, point1);
    // Predator2(field2, point2);
    // Predator3(field3, point3);
    // Predator4(field4, point4);

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
  int cnt = 0;

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
      if(i == 0 && map[current->pnt->y-1][current->pnt->x] != -1 && history[current->pnt->y-1][current->pnt->x] == 0 && 0 < current->pnt->y-1)  // 上に障害物がないか
        tempList[i].pnt->y = current->pnt->y-1, tempList[i].pnt->x = current->pnt->x, tempList[i].g = current->g+1;
      else if(i == 3 && map[current->pnt->y+1][current->pnt->x] != -1 && history[current->pnt->y+1][current->pnt->x] == 0 && current->pnt->y-1 < 16) // 下に障害物がないか
        tempList[i].pnt->y = current->pnt->y+1,tempList[i].pnt->x = current->pnt->x, tempList[i].g = current->g+1;
      else if(i == 1 && map[current->pnt->y][current->pnt->x+1] != -1 && history[current->pnt->y][current->pnt->x+1] == 0 && current->pnt->x-1 < 16) // 右に障害物がないか
        tempList[i].pnt->y = current->pnt->y, tempList[i].pnt->x = current->pnt->x+1, tempList[i].g = current->g+1;
      else if(i == 2 && map[current->pnt->y][current->pnt->x-1] != -1 && history[current->pnt->y][current->pnt->x-1] == 0 && 0 < current->pnt->x-1) // 左に障害物がないか
        tempList[i].pnt->y = current->pnt->y, tempList[i].pnt->x = current->pnt->x-1, tempList[i].g = current->g+1;

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
  printf("%d %d\n", current->pnt->y, current->pnt->x);
  return &ptr[steps-1];
}

void Predator1(int* field, int* point){
    static int IsArrivedPosition = 0;
    
    /* スタートノードの作成 */
    struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    /* ゴールノードの作成 */
    struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(sP, 1);

    // 定位置についた、かつ、Preyが見つかっていない
    if(findPrey == 0 && IsArrivedPosition){
        *point = RightHandRule(sP->y, sP->x);
    }
    // Preyが見つかった、または、定位置についていない
    else if(findPrey == 1 || IsArrivedPosition == 0){
        // Preyを見つけていれば
        if(findPrey == 1){
            gP->x = preyX, gP->y = preyY;
        }
        // 定位置についていなければ
        else if(IsArrivedPosition == 0){
            gP->x = 3, gP->y = 3;
        }
        struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
        struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

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

        char act[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
        int actP[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for(int i = 0; i < 4; i++){
            /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
            if(nextPosition->x == sP->x + actP[i][1] && nextPosition->y == sP->y + actP[i][0]){
                *point = (int)act[i];
            }
        }
        if(nextPosition->x == goalNode->pnt->x && nextPosition->y == goalNode->pnt->y) IsArrivedPosition = 1;
        // free(startNode);
        // free(goalNode);
        // free(finished);
        // free(closedList);
        // free(openList);
    }    
    History();
    history[sP->y][sP->x] = 5;

    printf("%d\n", *point);
}

void Predator2(int* field, int* point){
    
    static int IsArrivedPosition = 0;
    
    /* スタートノードの作成 */
    struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    /* ゴールノードの作成 */
    struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(sP, 2);

    // 定位置についた、かつ、Preyが見つかっていない
    if(findPrey == 0 && IsArrivedPosition){
        *point = RightHandRule(sP->y, sP->x);
    }
    // Preyが見つかった、または、定位置についていない
    else if(findPrey == 1 || IsArrivedPosition == 0){
        // Preyを見つけていれば
        if(findPrey == 1){
            gP->x = preyX, gP->y = preyY;
        }
        // 定位置についていなければ
        else if(IsArrivedPosition == 0){
            gP->x = 11, gP->y = 3;
        }
        struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
        struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

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

        char act[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
        int actP[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for(int i = 0; i < 4; i++){
            /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
            if(nextPosition->x == sP->x + actP[i][1] && nextPosition->y == sP->y + actP[i][0]){
                *point = (int)act[i];
            }
        }
        if(nextPosition->x == goalNode->pnt->x && nextPosition->y == goalNode->pnt->y) IsArrivedPosition = 1;
        // free(startNode);
        // free(goalNode);
        // free(finished);
        // free(closedList);
        // free(openList);
    }    
    History();
    history[sP->y][sP->x] = 5;

    printf("%d\n", *point);
}

void Predator3(int* field, int* point){
    static int IsArrivedPosition = 0;
    
    /* スタートノードの作成 */
    struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    /* ゴールノードの作成 */
    struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(sP, 3);

    // 定位置についた、かつ、Preyが見つかっていない
    if(findPrey == 0 && IsArrivedPosition){
        *point = RightHandRule(sP->y, sP->x);
    }
    // Preyが見つかった、または、定位置についていない
    else if(findPrey == 1 || IsArrivedPosition == 0){
        // Preyを見つけていれば
        if(findPrey == 1){
            gP->x = preyX, gP->y = preyY;
        }
        // 定位置についていなければ
        else if(IsArrivedPosition == 0){
            gP->x = 3, gP->y = 11;
        }
        struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
        struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

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

        char act[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
        int actP[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for(int i = 0; i < 4; i++){
            /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
            if(nextPosition->x == sP->x + actP[i][1] && nextPosition->y == sP->y + actP[i][0]){
                *point = (int)act[i];
            }
        }
        if(nextPosition->x == goalNode->pnt->x && nextPosition->y == goalNode->pnt->y) IsArrivedPosition = 1;
        // free(startNode);
        // free(goalNode);
        // free(finished);
        // free(closedList);
        // free(openList);
    }    
    History();
    history[sP->y][sP->x] = 5;

    printf("%d\n", *point);
    
}

void Predator4(int* field, int* point){
    
    static int IsArrivedPosition = 0;
    
    /* スタートノードの作成 */
    struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    /* ゴールノードの作成 */
    struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(sP, 4);

    // 定位置についた、かつ、Preyが見つかっていない
    if(findPrey == 0 && IsArrivedPosition){
        *point = RightHandRule(sP->y, sP->x);
    }
    // Preyが見つかった、または、定位置についていない
    else if(findPrey == 1 || IsArrivedPosition == 0){
        // Preyを見つけていれば
        if(findPrey == 1){
            gP->x = preyX, gP->y = preyY;
        }
        // 定位置についていなければ
        else if(IsArrivedPosition == 0){
            gP->x = 11, gP->y = 11;
        }
        struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
        struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
        goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

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

        char act[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
        int actP[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for(int i = 0; i < 4; i++){
            /* 次の行動位置から現在地の差分をとり、移動方向を決定する */
            if(nextPosition->x == sP->x + actP[i][1] && nextPosition->y == sP->y + actP[i][0]){
                *point = (int)act[i];
            }
        }
        if(nextPosition->x == goalNode->pnt->x && nextPosition->y == goalNode->pnt->y) IsArrivedPosition = 1;
        // free(startNode);
        // free(goalNode);
        // free(finished);
        // free(closedList);
        // free(openList);
    }    
    History();
    history[sP->y][sP->x] = 5;

    printf("%d\n", *point);
}

    
int RightHandRule(int posY, int posX){
    char array[4] = {'t', 'd', 'l', 'r'};
    // 右方向に行けるなら
    // その場所に障害物がない、かつ、フィールド内であること
    printf("%d %d\n", posY, posX);
    if(map[posY][posX + 1] == 0 && posX+1 < 16) return (int)array[3]; // 右に行けるなら
    else if(map[posY + 1][posX] == 0 && posY+1 < 16) return (int)array[1]; // 下に行けるなら
    else if(map[posY][posX - 1] == 0 && 0 <= posX-1) return (int)array[2]; // 左に行けるなら
    else if(map[posY - 1][posX] == 0 && 0 <= posY-1) return (int)array[0]; // 上に行けるなら
}

void PredatorPosition(struct predatorPoint* pos, int n){
    int i, j;
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            if(map[i][j] == n){
                pos->y = i; // 行
                pos->x = j; // 列
            }
        }
    }
}

void History(){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(history[i][j] > 0){
                history[i][j] -= 1;
            }
        }
    }
}

struct predatorPoint* ManhattanDistance(int n){
  struct predatorPoint* pos = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  // return PredatorPosition(pos, n);
}

void AdjacementMatrix(){
  int from = -1;      // 走査するノード, 最初のループに入ったときに0になる
  int to = from + 1;  // fromの右隣のノード
  int i, j;

  /* 隣接行列を0で初期化*/
  for (i = 0; i < NODE_NUM; i++) {
    for (j = 0; j < NODE_NUM; j++) {
      predator_adj[i][j] = 0;
    }
  }

  /* x軸方向のエッジを検出*/
  for (i = 0; i < WORLD_SIZE; i++) {
    /* 走査するノードを１列右にシフト*/
    from += 1;
    to = from + 1;

    for (j = 0; j < WORLD_SIZE - 1; j++) {
      if (map[i][j] != VALUE_OF_OBSTACLE
           && map[i][j+1] != VALUE_OF_OBSTACLE) {
        predator_adj[from][to] = 1;
        predator_adj[to][from] = 1;
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
      if (map[j][i] != VALUE_OF_OBSTACLE
           && map[j+1][i] != VALUE_OF_OBSTACLE) {
        predator_adj[from][to] = 1;
        predator_adj[to][from] = 1;
      }

      from += WORLD_SIZE;
      to = from + WORLD_SIZE;
    }
  }

}

void CheckAdjacent(){
  int i, j;
  int cnt;

  for(i = 0; i < 256; i++){
    cnt = 0;
    for(j = 0; j < 256; j++){
      if(predator_adj[i][j] == 1){
        cnt+= 1;
      }
      // printf("%d ", predator_adj[i][j]);
    }
    printf("%3d", cnt);
    if((i+1) % WORLD_SIZE == 0 && i != 0){
      printf("\n");
    }
    // printf("\n");
  }
}