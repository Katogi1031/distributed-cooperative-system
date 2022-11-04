/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created November 2, 2022
Language : C


Function : prey(int *ca, int action)
Description : Find the shortest path from the A* algorithm and go in the opposite direction to that path.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int prey_field[8][8];
#define E 0.000001 


struct preyNode{
  struct preyPoint* pnt;
  struct preyNode* parent;
  int g;
  int h;
};

struct preyPoint{
  int x;
  int y;
};

double preySqrt(double a)
{
    a = a < 0 ? -a : a;
    double x = a / 2;
    while (1) {
		double e = x * x - a;
    e = e < 0 ? -e : e;
        if (e < E) return x;
        x -= e / (x * 2);
    }
}

// 

/* 関数の呼び出し順に宣言したいための処置 */
struct preyNode* preyAStar(struct preyNode* current, struct preyNode* goal, struct preyNode **openList, int l1, struct preyNode** closedList, int l2);
int preySearch(struct preyNode* current, struct preyNode **openList, int l1, struct preyNode **closedList, int l2);
int preyCalcCost(struct preyNode **openList, struct preyNode* goalNode, int l1);
struct preyPoint* preyRetrace(struct preyNode* goalNode);

/* AStarAlgorithm */
struct preyNode* preyAStar(struct preyNode* current, struct preyNode* goal, struct preyNode **openList, int l1, struct preyNode** closedList, int l2){
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = preySearch(current, openList, l1, closedList, l2);

  /* 経路コストを計算する */
  nextIndex = preyCalcCost(openList, goal, l1);
  
  struct preyNode *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct preyNode *tempList = (struct preyNode*)calloc(l1-1, sizeof(struct preyNode));
  for(i = 0, j = 0; i < l1; i++){
    if(i != nextIndex){
      memcpy(&tempList[j], &((*openList)[i]), sizeof(struct preyNode));
      j++;
    } 
  }
  l1--, l2++;
  *openList = tempList;

  /* クローズドノードに追加していく */
  *closedList = (struct preyNode*)realloc((*closedList), l2*(sizeof(struct preyNode)));
  memcpy(&((*closedList)[l2-1]), nextNode, sizeof(struct preyNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  return nextNode;
  return preyAStar(nextNode, goal, openList, l1, closedList, l2);

}

/* preySearch */
int preySearch(struct preyNode* current, struct preyNode **openList, int l1, struct preyNode **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct preyNode* tempList;
   tempList = (struct preyNode*)calloc(4, sizeof(struct preyNode));   
   for(i = 0; i < 4; i++){
      tempList[i].pnt = (struct preyPoint*)malloc(sizeof(struct preyPoint));
      if(i == 0 && prey_field[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempList[i].pnt->x = current->pnt->x-1, tempList[i].pnt->y = current->pnt->y, tempList[i].g = current->g+1;
      else if(i == 3 && prey_field[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempList[i].pnt->x = current->pnt->x+1, tempList[i].pnt->y = current->pnt->y, tempList[i].g = current->g+1;
      else if(i == 1 && prey_field[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempList[i].pnt->x = current->pnt->x, tempList[i].pnt->y = current->pnt->y+1, tempList[i].g = current->g+1;
      else if(i == 2 && prey_field[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempList[i].pnt->x = current->pnt->x, tempList[i].pnt->y = current->pnt->y-1, tempList[i].g = current->g+1;
    
      tempList[i].parent = current;
   }

  /* 現在のノードが子ノードを持つかどうか*/
  for(j = 0; j < 4; j++){
      found = 0;           
      if(tempList[j].parent == NULL){
        return 0;
      }
      for(i = 0; i < l1; i++) if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y) found++;
      for(i = 0; i < l2; i++) if(tempList[j].pnt->x == (*closedList)[i].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y) found++; 

      if(found == 0){
        count++;
        int total = l1 + count;
        *openList = (struct preyNode*)realloc((*openList), total*(sizeof(struct preyNode)));
        memcpy(&((*openList)[total-1]), &tempList[j],sizeof(struct preyNode));              
      }      
  } 
  return count + l1;
}



int preyCalcCost(struct preyNode **openList, struct preyNode* goalNode, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    
    (*openList)[i].h = ((int)preySqrt(difx * difx + dify * dify));
  }

  int min, minIndex = 0;
  min = (*openList)[0].g + (*openList)[0].h;
  for(i = 1; i < l1; i++){
    if((*openList)[i].g + (*openList)[i].h < min){
      min = (*openList)[i].g + (*openList)[i].h;
      minIndex = i;
    } 
  }
  return minIndex;
}

/* ReconstructThePath */
struct preyPoint* preyRetrace(struct preyNode* goalNode){
  struct preyNode* current = goalNode;
  struct preyPoint* ptr = NULL;
  int steps = 0, i;
  while(current->parent != NULL){
      steps++;
      ptr = (struct preyPoint*)realloc(ptr, steps*sizeof(struct preyPoint));
      memcpy(&ptr[steps-1], current->pnt, sizeof(struct preyPoint));
      current = current->parent;                    
  }
  return &ptr[1];
}

int preyFlag(int x, int y){
  if(prey_field[x][y] != -1 && x < 8 && x >= 0 && y < 8 && y >= 0){
      return 1;
    }
  return -1;
}

void Prey(int *ca, int *action){
  int p, q, i;

  struct preyPoint *predator = (struct preyPoint*)malloc(sizeof(struct preyPoint));
  struct preyPoint *prey = (struct preyPoint*)malloc(sizeof(struct preyPoint));

  for(i = 0; i < 64; i++){
    p = i / 8;
    q = i % 8;

    prey_field[p][q] = ca[i];
    
    switch(prey_field[p][q]){
      case 1:   // predator
        predator->x = p, predator->y = q;
        break;
      case 10:  // prey
        prey->x = p, prey->y = q;
        break;
    }
  }

  /* スタートノードの作成 */
  struct preyPoint* sP = (struct preyPoint*)malloc(sizeof(struct preyPoint));
  sP->x = predator->x, sP->y = predator->y;
  struct preyNode* startNode = (struct preyNode*)malloc(sizeof(struct preyNode));
  startNode->pnt = sP, startNode->parent = NULL, startNode->g=0, startNode->h=0;
  
  /* ゴールノードの作成 */
  struct preyPoint* gP = (struct preyPoint*)malloc(sizeof(struct preyPoint));
    gP->x = prey->x, gP->y = prey->y;
    struct preyNode* goalNode = (struct preyNode*)malloc(sizeof(struct preyNode));
    goalNode->pnt = gP, goalNode->parent = NULL, goalNode->g=0, goalNode->h=0;

  /* オープンリストの作成 */
  struct preyNode **openList = (struct preyNode**)malloc(sizeof(struct preyNode*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct preyNode **closedList = (struct preyNode**)malloc(sizeof(struct preyNode*));
  *closedList = NULL;

  (*closedList) = (struct preyNode*)realloc((*closedList), sizeof(struct preyNode));
  memcpy(&((*closedList)[0]), startNode, sizeof(struct preyNode));
  

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct preyNode* finished = preyAStar(startNode, goalNode, openList, 0, closedList, 1);
  struct preyPoint* a = preyRetrace(finished);

  int nextX = prey->x + prey->x - a->x;
  int nextY = prey->y + prey->y - a->y;
  int nextActX = prey->x - a->x;
  int nextActY = prey->y - a->y;
  char nextOpe[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
  int actPoint[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
  int r, f;
  
  for(i = 0; i < 4; i++){
    if(nextActX == actPoint[i][0] && nextActY == actPoint[i][1]){
      f = preyFlag(nextX, nextY);
      break;
    }
  }
  if(f == 1){
    *action = (int)(nextOpe[i]);
  }else{
    srand(100);
    if(nextActY != 0){ 
      r = rand() % 2;
      *action = (int)(nextOpe[r]);
    }else{
      r = 2 + rand() % 2;
      *action = (int)(nextOpe[r]);
    }
  }
}
