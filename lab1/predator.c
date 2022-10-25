/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created October 25, 2022
Language : C


Function predator(int *ca, int action)
Description
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int gfield[8][8];

struct predatorNode{
  struct predatorPosition* pnt;
  struct predatorNode* parent;
  float g;
  float h;
  // float f;
};

struct predatorPosition{
  int x;
  int y;
};

struct predatorPosition* predatorCreatePosition(){
  struct predatorPosition* p = (struct predatorPosition*)malloc(sizeof(struct predatorPosition));
  return p;
}
int ExpandNode(struct predatorNode* current, struct predatorNode **openList, int l1, struct predatorNode **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct predatorNode* tempList = (struct predatorNode*)calloc(4, sizeof(struct predatorNode));

  for(i = 0; i < 4; i++){
    tempList[i].pnt = predatorCreatePosition();
    
    if(i == 0 && gfield[current->pnt->x-1][current->pnt->y] != -1){
      tempList[i].pnt->x = current->pnt->x-1;
      tempList[i].pnt->y = current->pnt->y;
    }else if(i == 1 && gfield[current->pnt->x+1][current->pnt->y] != -1){
      tempList[i].pnt->x = current->pnt->x+1;
      tempList[i].pnt->y = current->pnt->y;
    }else if(i == 2 && gfield[current->pnt->x][current->pnt->y-1] != -1){
      tempList[i].pnt->x = current->pnt->x;
      tempList[i].pnt->y = current->pnt->y-1;
    }else if(i == 3 && gfield[current->pnt->x][current->pnt->y+1] != -1){
      tempList[i].pnt->x = current->pnt->x;
      tempList[i].pnt->y = current->pnt->y+1;
    }

    tempList[i].parent = current;
      
  }

  /* 現在のノードが小ノードを持つかどうか*/
  for(j = 0; j < 4; j++){
    found = 0;
    if(tempList[j].parent == NULL){
      return 0;
    }
    for(i = 0; i < l1; i++){
      if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y)
        found++;
    }
    for(i = 0; i < l2; i++){
      if((*closedList)[i].pnt->x == tempList[j].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y)
        found++;
    }
    if(found == 0){
      count++;
      int total = 1l + count;
      *openList = (struct predatorNode*)realloc((*openList), total*(sizeof(struct predatorNode)));
      memcpy(&((*openList)[total-1]), &tempList[j], sizeof(struct predatorNode));
    }
  }
  return count + l1;


}

void CalculateTheTotalCost(struct predatorNode* goalNode, struct predatorNode **openList, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    (*openList)[i].h = ((float)sqrt(pow(difx, 2) + pow(dify, 2)));
  }
}

int FindTheLeastCosted(struct predatorNode **openList, int l1){
  int i, min, minIndex;
  min = (*openList)[0].g + (*openList)[0].h;
  minIndex = 0;
  for(i = 1; i < l1; i++){
    if((*openList)[i].g * (*openList)[i].h < min){
      min = (*openList)[i].g + (*openList)[i].h;
      minIndex = i;
    }
  }
  return minIndex;
}

void ReconstructThePath(struct predatorNode* goalNode){
  struct predatorNode* current = goalNode;
  struct predatorPosition* ptr = NULL;
  int steps = 0, i;
  
  while(current->parent != NULL){
    steps++;
    ptr = (struct predatorPosition*)realloc(ptr, steps*sizeof(struct predatorPosition));
    memcpy(&ptr[steps-1], current->pnt, sizeof(struct predatorPosition));
    current = current->parent;
  }
  for(i = steps; i >= 1; i--){
    gfield[ptr[i-1].x][ptr[i-1].y] = 20;
    printf("(%d, %d)", ptr[i-1].x, ptr[i-1].y);
    if(i > 1)
      printf("=>");
  }
  printf("\n");
}


struct predatorNode* AStarAlgorithm(struct predatorNode* current, struct predatorNode* goal, struct predatorNode **openList, int l1, struct predatorNode** closedList, int l2){
  int i, j, nextIndex;
  
  /* オープンリストの長さを求める */
  l1 = ExpandNode(current, openList, l1, closedList, l2);

  /* 経路コストを計算する */
  CalculateTheTotalCost(goal, openList, l1);
  struct predatorNode *nextNode = &((*openList)[nextIndex]);

  /* 次のオープンリストを作成する */
  struct predatorNode *tempList = (struct predatorNode*)calloc(l1-1, sizeof(struct predatorNode));
  for(i = 0, j = 0; i < l1; i++){
    if(i != nextIndex){
      memcpy(&tempList[j], &((*openList)[i]), sizeof(struct predatorNode));
      j++;
    }
  }
  l1--;
  l2++;
  *openList = tempList;

  /* クローズドノードに追加していく */
  *closedList = (struct predatorNode*)realloc((*closedList), l2*(sizeof(struct predatorNode)));
  memcpy(&(*closedList)[l2-1], nextNode, sizeof(struct predatorNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->x){
    return nextNode;
  }
  return AStarAlgorithm(nextNode, goal, openList, l1, closedList, l2);

}







void predator(int *ca){
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;

  struct predatorPosition *predator, *prey;
  int openLen = 0, closedLen = 0; // オープンリスト、クローズリストの長さ

  

  predator = predatorCreatePosition();
  prey = predatorCreatePosition();

  for(int i = 0; i < size_1d; i++){
    p = i / size_2d;
    q = i % size_2d;

    gfield[p][q] = ca[i];
    
    switch(gfield[p][q]){
      case 1:   // predator
        predator->x = p, predator->y = q;
        break;
      case 10:  // prey
        prey->x = p, prey->y = q;
        break;
    }

  }

  printf("predator (%d %d)\n", predator->x, predator->y);
  printf("prey     (%d %d)\n", prey->x, prey->y);

  /* スタートノードの作成 */
  struct predatorPosition* sp = predatorCreatePosition();
  sp->x = predator->x, sp->y = predator->y;
  struct predatorNode* sn = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  sn->pnt = sp, sn->parent = NULL, sn->g = 0, sn->h = 0;
  
  /* ゴールノードの作成 */
  struct predatorPosition* gp = predatorCreatePosition();
  gp->x = prey->x, gp->y = prey->y;
  struct predatorNode* gn = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  gn->pnt = gp, gn->parent = NULL, gn->g = 0, gn->h = 0;

  /* オープンリストの作成 */
  struct predatorNode **openList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct predatorNode **closedList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  
  *closedList = NULL;

  *closedList = (struct predatorNode*)realloc((*closedList), sizeof(struct predatorNode));
  memcpy(&((*closedList)[0]), sn, sizeof(struct predatorNode));
  closedLen = 1;

  
  // AStarAlgorithm(sn, gn, openList, openLen, closedList, closedLen, ca);
  // AStarAlgorithm();
  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct predatorNode* finished = AStarAlgorithm(sn, gn, openList, openLen, closedList, closedLen);

  ReconstructThePath(finished);

  getchar();
  
}


int main(void) {
  FILE *fp;	          /* file pointer */
  int rstat, i;		  /* fscanf return status and loop parameter */
  int array[64];	       /* data array */

  fp = fopen("battlefield.dat", "r"); /* open file to read */

  if (fp == NULL) {                    /* if fp is NULL, it means open file failed */
    printf("Failed file open.\n"); 
  } else {
    for(i = 0; i < 64; i++){
      rstat = fscanf(fp, "%d", &array[i]);
    }

  }
  fclose(fp);
  predator(array);
  return 0;

}

