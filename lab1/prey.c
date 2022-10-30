/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created October 25, 2022
Language : C


Function : predator(int *ca, int action)
Description : Using the A* algorithm, find the shortest distance between Predator and Prey and choose an action.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int gfield[8][8];

struct node{
  struct point* pnt;
  struct node* parent;
  float g;
  float h;
  float f;
};

struct point{
  int x;
  int y;
};


/* 関数の呼び出し順に宣言したいための処置 */
int preyAStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2);
int preyExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2);
void preyCalculateTheTotalCost(struct node* goalNode, struct node **openList, int l1);
int preyFindTheLeastCosted(struct node **openList, int l1);
// void ReconstructThePath(struct node* goalNode);
struct point* preyReconstructThePath(struct node* goalNode);

/* AStarAlgorithm */
int preyAStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2){
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = preyExpandNode(current, openList, l1, closedList, l2);
  // printf("l1 = %d\n", l1);

  /* 経路コストを計算する */
  preyCalculateTheTotalCost(goal, openList, l1); //問題あり

  nextIndex = preyFindTheLeastCosted(openList,l1);
  
  struct node *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct node *tempList = (struct node*)calloc(l1-1, sizeof(struct node));
  for(i = 0,j=0;i<l1;i++){
    if(i != nextIndex){
      memcpy(&tempList[j],&((*openList)[i]),sizeof(struct node));
      j++;
    } 
  }
  l1--,l2++;
  *openList = tempList;

  // printf("f = %d\n", (int)(*openList)[nextIndex].f);

  /* クローズドノードに追加していく */
  *closedList = (struct node*)realloc((*closedList), l2*(sizeof(struct node)));
  memcpy(&((*closedList)[l2-1]),nextNode,sizeof(struct node));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  // if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y){
  //   return nextNode;
  // }
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  
    return (int)(*openList)[nextIndex].f;
  return preyAStarAlgorithm(nextNode, goal, openList, l1, closedList, l2);

}

/* ExpandNode */
int preyExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct node* tempList;
   tempList = (struct node*)calloc(4,sizeof(struct node));   
   for(i = 0;i<4;i++){
      tempList[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && gfield[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempList[i].pnt->x = current->pnt->x-1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1.0f;
      else if(i == 3 && gfield[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempList[i].pnt->x = current->pnt->x+1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1.0f;
      else if(i == 1 && gfield[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempList[i].pnt->x = current->pnt->x,tempList[i].pnt->y = current->pnt->y+1,tempList[i].g = current->g+1.0f;
      else if(i == 2 && gfield[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempList[i].pnt->x = current->pnt->x,tempList[i].pnt->y = current->pnt->y-1,tempList[i].g = current->g+1.0f;
    
      tempList[i].parent = current;
   }

  /* 現在のノードが小ノードを持つかどうか*/
  for(j = 0; j < 4; j++){
      found = 0;           
      if(tempList[j].parent == NULL){
        return 0;
      }
      for(i = 0;i<l1;i++) if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y) found++;
      for(i = 0;i<l2;i++) if(tempList[j].pnt->x == (*closedList)[i].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y) found++; 
      //--THIRD-PHASE--:expand the array with the additional neighbors.
      if(found == 0)
      {
        count++;
        int total = (l1+count);
        *openList = (struct node*)realloc((*openList),total*(sizeof(struct node)));
        memcpy(&((*openList)[total-1]),&tempList[j],sizeof(struct node));              
      }      
   } 
  return count + l1;
}



/* CalculateTheTotalCost */
void preyCalculateTheTotalCost(struct node* goalNode, struct node **openList, int l1){
  int i,difx,dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    (*openList)[i].h = ((float)sqrt(pow(difx,2) + pow(dify,2)));
  }
}

/* FindTheLeastCosted */
int preyFindTheLeastCosted(struct node **openList, int l1){
  int i, min, minIndex;
    min = (*openList)[0].g + (*openList)[0].h;
    (*openList)[0].f = (*openList)[0].g + (*openList)[0].h;
    minIndex = 0;
    for(i = 1; i < l1; i++){
      (*openList)[i].f = (*openList)[i].g + (*openList)[i].h;
      if((*openList)[i].g + (*openList)[i].h < min){
        min = (*openList)[i].g + (*openList)[i].h;
        minIndex = i;
      } 
    }
    return minIndex;
}

/* ReconstructThePath */
// void ReconstructThePath(struct node* goalNode){
struct point* preyReconstructThePath(struct node* goalNode){
  struct node* current = goalNode;
  struct point* ptr = NULL;
  int steps = 0,i;
  while(current->parent != NULL){
      steps++;
      ptr = (struct point*)realloc(ptr,steps*sizeof(struct point));
      memcpy(&ptr[steps-1],current->pnt,sizeof(struct point));
      current = current->parent;                    
  }
  printf("%d", steps);
  // for(i = steps; i >= 1; i--){
  //   printf("(%d,%d)",ptr[i-1].x,ptr[i-1].y);
  //   if(i>1)  printf("=>");  
  // }
  printf("\n");
  return &ptr[steps-1];
}




void Prey(int *ca, int *action){
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;

  struct point *predator, *prey;
  int openLen = 0, closedLen = 0; // オープンリスト、クローズリストの長さ

  predator = (struct point*)malloc(sizeof(struct point));
  prey = (struct point*)malloc(sizeof(struct point));

  for(int i = 0; i < size_1d; i++){
    p = i / size_2d;
    q = i % size_2d;

    gfield[p][q] = ca[i];
    
    switch(gfield[p][q]){
      case 1:   // predator
        predator->y = p, predator->x = q;
        break;
      case 10:  // prey
        prey->y = p, prey->x = q;
        break;
    }
  }

  // printf("predator (%d %d)\n", predator->x, predator->y);
  // printf("prey     (%d %d)\n", prey->x, prey->y);
  
  int arr[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
  int max = -1, maxIndex = -1;

  /* スタートノードの作成 */
  struct point* sP = (struct point*)malloc(sizeof(struct point));
  sP->x = predator->x,sP->y = predator->y;
  struct node* startNode = (struct node*)malloc(sizeof(struct node));
  startNode->pnt = sP,startNode->parent = NULL,startNode->g=0;startNode->h=0;

  for(int i = 0; i < 4; i++){
    /* ゴールノードの作成 */
    struct point* gP = (struct point*)malloc(sizeof(struct point));
    gP->y = prey->y + arr[i][0], gP->x = prey->x + arr[i][1];

    
    if(gfield[gP->y][gP->x] == -1 || gP->x >= 8 || gP->x < 0 || gP->y >= 8 || gP->y < 0) 
      continue;
    

    struct node* goalNode = (struct node*)malloc(sizeof(struct node));
    goalNode->pnt = gP,goalNode->parent = NULL,goalNode->g=0,startNode->h=0;

    /* オープンリストの作成 */
    struct node **openList = (struct node**)malloc(sizeof(struct node*));
    *openList = NULL;

    /* クローズリストの作成 */
    struct node **closedList = (struct node**)malloc(sizeof(struct node*));
    *closedList = NULL;

    (*closedList) = (struct node*)realloc((*closedList),sizeof(struct node));
    memcpy(&((*closedList)[0]),startNode,sizeof(struct node));
    printf("y = %d, x = %d\n", goalNode->pnt->y, goalNode->pnt->x);
    /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
    // struct node* finished = AStarAlgorithm(startNode, goalNode, openList, openLen, closedList, closedLen);
    int value = preyAStarAlgorithm(startNode, goalNode, openList, 0, closedList, 1);
    printf("value = %d\n", value);

    if(max < value){
      max = value;
      maxIndex = i;
    }

    // free(startNode);
    // free(sP);
    free(goalNode);
    free(gP);
    free(openList);
    free(closedList);
  }

  printf("max = %d, index = %d\n", max, maxIndex);

  
 

  int r = rand() % 4;

  if(maxIndex == 0){
    *action = (int)act[0];
    printf("up\n");
  }else if(maxIndex == 1){
    *action = (int)act[1];
    printf("down\n");
  }else if(maxIndex == 2){
    *action = (int)act[2];
    printf("left\n");
  }else if(maxIndex == 3){
    *action = (int)act[3];
    printf("right\n");
  }
  
}
