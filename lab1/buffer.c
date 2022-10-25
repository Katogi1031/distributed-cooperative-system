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

struct predatorNode{
  struct predatorPoint* predator_pnt;
  struct predatorNode* predator_parent;
  float predator_predatorg;
  float predator_h;
  // float f;
};

struct predatorPoint{
  int predator_x;
  int predator_y;
};

struct predatorPoint* predatorCreatePosition(){
  struct predatorPoint* predator_p = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  return predator_p;
}

/* 関数の呼び出し順に宣言したいための処置 */
struct predatorNode* AStarAlgorithm(struct predatorNode* predator_current, struct predatorNode* predator_goal, struct predatorNode **predator_openList, int predator_l1, struct predatorNode** predator_closedList, int predator_l2);
int ExpandNode(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_l1, struct predatorNode **predator_closedList, int predator_l2);
void CalculateTheTotalCost(struct predatorNode* predator_goalNode, struct predatorNode **predator_openList, int predator_l1);
int FindTheLeastCosted(struct predatorNode **predator_openList, int predator_l1);
// void ReconstructThePath(struct node* goalNode);
struct predatorPoint* ReconstructThePath(struct predatorNode* predator_goalNode);

/* AStarAlgorithm */
struct predatorNode* AStarAlgorithm(struct predatorNode* predator_current, struct predatorNode* predator_goal, struct predatorNode **predator_openList, int predator_l1, struct predatorNode** predator_closedList, int predator_l2){
  int predator_i, predator_j, predator_nextIndex;

  /* オープンリストの長さを求める */
  predator_l1 = ExpandNode(predator_current, predator_openList, predator_l1, predator_closedList, predator_l2);

  /* 経路コストを計算する */
  CalculateTheTotalCost(predator_goal, predator_openList, predator_l1); //問題あり

  predator_nextIndex = FindTheLeastCosted(predator_openList, predator_l1);
  
  struct predatorNode *predator_nextNode = &((*predator_openList)[predator_nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct predatorNode *predator_tempList = (struct predatorNode*)calloc(predator_l1 - 1, sizeof(struct predatorNode));
  for(predator_i = 0, predator_j = 0; predator_i < predator_l1; predator_i++){
    if(predator_i != predator_nextIndex){
      memcpy(&predator_tempList[predator_j], &((*predator_openList)[predator_i]),sizeof(struct predatorNode));
      predator_j++;
    } 
  }
  predator_l1--, predator_l2++;
  *predator_openList = predator_tempList;

  

  /* クローズドノードに追加していく */
  *predator_closedList = (struct predatorNode*)realloc((*predator_closedList), predator_l2*(sizeof(struct predatorNode)));
  memcpy(&((*predator_closedList)[predator_l2-1]), predator_nextNode, sizeof(struct predatorNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(predator_nextNode->predator_pnt->predator_x == predator_goal->predator_pnt->predator_x && predator_nextNode->predator_pnt->predator_y == predator_goal->predator_pnt->predator_y){
    return predator_nextNode;
  }
  if(predator_nextNode->predator_pnt->predator_x == predator_goal->predator_pnt->predator_x && predator_nextNode->predator_pnt->predator_y == predator_goal->predator_pnt->predator_y)  return predator_nextNode;
  return AStarAlgorithm(predator_nextNode, predator_goal, predator_openList, predator_l1, predator_closedList, predator_l2);

}

/* ExpandNode */
int ExpandNode(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_l1, struct predatorNode **predator_closedList, int predator_l2){
  int predator_i, predator_j, predator_count, predator_found;
  predator_count = 0;

  struct predatorNode* predator_tempList;
   predator_tempList = (struct predatorNode*)calloc(4, sizeof(struct predatorNode));   
   for(predator_i = 0; predator_i < 4; predator_i++){
      tempList[predator_i].pnt = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
      if(predator_i == 0 && predator_gfield[predator_current->predator_pnt->predator_x-1][predator_current->predator_pnt->predator_y] != -1)  // 左に障害物がないか
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
        *openList = (struct predatorNode*)realloc((*openList),total*(sizeof(struct predatorNode)));
        memcpy(&((*openList)[total-1]),&tempList[j],sizeof(struct predatorNode));              
      }      
   } 
  return count + l1;
}



/* CalculateTheTotalCost */
void CalculateTheTotalCost(struct predatorNode* predator_goalNode, struct predatorNode **predator_openList, int predator_l1){
  int predator_i, predator_difx, predator_dify;
  for(predator_i = 0; predator_i < predator_l1; predator_i++){
    predator_difx = (*predator_openList)[predator_i].predator_pnt->predator_x - predator_goalNode->predator_pnt->predator_x;
    predator_dify = (*predator_openList)[predator_i].predator_pnt->predator_y - predator_goalNode->predator_pnt->predator_y;
    (*predator_openList)[predator_i].predator_h = ((float)sqrt(pow(predator_difx, 2) + pow(predator_dify, 2)));
  }
}

/* FindTheLeastCosted */
int FindTheLeastCosted(struct predatorNode **predator_openList, int predator_l1){
  int predator_i, predator_min, predator_minIndex;
    predator_min = (*predator_openList)[0].predator_g + (*predator_openList)[0].predator_h;
    predator_minIndex = 0;
    for(predator_i = 1; predator_i < predator_l1; predator_i++){
      if((*predator_openList)[predator_i].predator_g + (*predator_openList)[predator_i].predator_h < predator_min){
        predator_min = (*predator_openList)[predator_i].predator_g + (*predator_openList)[predator_i].predator_h;
        predator_minIndex = predator_i;
      } 
    }
    return predator_minIndex;
}

/* ReconstructThePath */
// void ReconstructThePath(struct node* goalNode){
struct predatorPoint* ReconstructThePath(struct predatorNode* predator_goalNode){
  struct predatorNode* predator_current = predator_goalNode;
  struct predatorPoint* predator_ptr = NULL;
  int predator_steps = 0, predator_i;
  while(predator_current->predator_parent != NULL){
      predator_steps++;
      predator_ptr = (struct predatorPoint*)realloc(predator_ptr, predator_steps*sizeof(struct predatorPoint));
      memcpy(&predator_ptr[predator_steps-1], predator_current->predator_pnt, sizeof(struct predatorPoint));
      predator_current = predator_current->predator_parent;                    
  }
  printf("%d", predator_steps);
  // for(i = steps; i >= 1; i--){
  //   printf("(%d,%d)",ptr[i-1].x,ptr[i-1].y);
  //   if(i>1)  printf("=>");  
  // }
  printf("\n");
  return &predator_ptr[predator_steps-1];
}




void Predator(int *ca, int *action){
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;

  struct predatorPoint *predator, *prey;
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
  struct predatorPoint* sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  sP->x = predator->x,sP->y = predator->y;
  struct predatorNode* startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  startNode->pnt = sP,startNode->parent = NULL,startNode->g=0;startNode->h=0;
  
  /* ゴールノードの作成 */
  struct predatorPoint* gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    gP->x = prey->x,gP->y = prey->y;
    struct predatorNode* goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
    goalNode->pnt = gP,goalNode->parent = NULL,goalNode->g=0,startNode->h=0;

  /* オープンリストの作成 */
  struct predatorNode **openList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct predatorNode **closedList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *closedList = NULL;

  (*closedList) = (struct predatorNode*)realloc((*closedList),sizeof(struct predatorNode));
  memcpy(&((*closedList)[0]),startNode,sizeof(struct predatorNode));
  

 

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  // struct node* finished = AStarAlgorithm(startNode, goalNode, openList, openLen, closedList, closedLen);
  struct predatorNode* finished = AStarAlgorithm(startNode, goalNode, openList, 0, closedList, 1);
  
  struct predatorPoint* a = ReconstructThePath(finished);

  printf("next Action (%d %d)\n", a->x, a->y);

  if(a->x - predator->x == 1){
    *action = (int)act[1];
    printf("Down\n");
  }else if(a->x - predator->x == -1){
    *action = (int)act[0];
  }else if(a->y - predator->y == 1){
    *action = (int)act[3];
  }else{
    *action = (int)act[2];
  }
  printf("%d\n", *action);
  
}