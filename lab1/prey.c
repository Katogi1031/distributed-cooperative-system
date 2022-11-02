/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created October 25, 2022
Language : C


Function : prey(int *ca, int action)
Description : Using the A* algorithm, find the shortest distance between Predator and Prey and choose an action.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int prey_field[8][8];
#define E 0.000001 


struct node{
  struct point* pnt;
  struct node* parent;
  int g;
  int h;
};

struct point{
  int x;
  int y;
};

double preySqrt(double a)
{
    a = a < 0 ? -a : a;
    double x = a / 2;
    while (1) {
		double e = x * x - a;
		double t = e < 0 ? -e : e;
        if (t < E) return x;
        x -= e / (x * 2);
    }
}

// prey_

/* 関数の呼び出し順に宣言したいための処置 */
struct node* preyAStar(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2);
int preySearch(struct node* current, struct node **openList, int l1, struct node **closedList, int l2);
int preyCalcCost(struct node **openList, struct node* goalNode, int l1);
struct point* preyRetrace(struct node* goalNode);

/* AStarAlgorithm */
struct node* preyAStar(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2){
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = preySearch(current, openList, l1, closedList, l2);

  /* 経路コストを計算する */
  nextIndex = preyCalcCost(openList, goal, l1);
  
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

  /* クローズドノードに追加していく */
  *closedList = (struct node*)realloc((*closedList), l2*(sizeof(struct node)));
  memcpy(&((*closedList)[l2-1]),nextNode,sizeof(struct node));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  return nextNode;
  return preyAStar(nextNode, goal, openList, l1, closedList, l2);

}

/* preySearch */
int preySearch(struct node* current, struct node **openList, int l1, struct node **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct node* tempList;
   tempList = (struct node*)calloc(4,sizeof(struct node));   
   for(i = 0;i<4;i++){
      tempList[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && prey_field[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempList[i].pnt->x = current->pnt->x-1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1;
      else if(i == 3 && prey_field[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempList[i].pnt->x = current->pnt->x+1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1;
      else if(i == 1 && prey_field[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempList[i].pnt->x = current->pnt->x,tempList[i].pnt->y = current->pnt->y+1,tempList[i].g = current->g+1;
      else if(i == 2 && prey_field[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempList[i].pnt->x = current->pnt->x,tempList[i].pnt->y = current->pnt->y-1,tempList[i].g = current->g+1;
    
      tempList[i].parent = current;
   }

  /* 現在のノードが子ノードを持つかどうか*/
  for(j = 0; j < 4; j++){
      found = 0;           
      if(tempList[j].parent == NULL){
        return 0;
      }
      for(i = 0;i<l1;i++) if((*openList)[i].pnt->x == tempList[j].pnt->x && (*openList)[i].pnt->y == tempList[j].pnt->y) found++;
      for(i = 0;i<l2;i++) if(tempList[j].pnt->x == (*closedList)[i].pnt->x && (*closedList)[i].pnt->y == tempList[j].pnt->y) found++; 

      if(found == 0){
        count++;
        int total = l1 + count;
        *openList = (struct node*)realloc((*openList),total*(sizeof(struct node)));
        memcpy(&((*openList)[total-1]),&tempList[j],sizeof(struct node));              
      }      
  } 
  return count + l1;
}



int preyCalcCost(struct node **prey_openList, struct node* prey_goalNode, int prey_l1){
  int prey_i, prey_difx, prey_dify;
  for(prey_i = 0; prey_i < prey_l1; prey_i++){
    prey_difx = (*prey_openList)[prey_i].pnt->x - prey_goalNode->pnt->x;
    prey_dify = (*prey_openList)[prey_i].pnt->y - prey_goalNode->pnt->y;
    
    (*prey_openList)[prey_i].h = ((int)preySqrt(prey_difx * prey_difx + prey_dify * prey_dify));
  }

  int prey_min, prey_minIndex = 0;
  prey_min = (*prey_openList)[0].g + (*prey_openList)[0].h;
  for(prey_i = 1; prey_i < prey_l1; prey_i++){
    if((*prey_openList)[prey_i].g + (*prey_openList)[prey_i].h < prey_min){
      prey_min = (*prey_openList)[prey_i].g + (*prey_openList)[prey_i].h;
      prey_minIndex = prey_i;
    } 
  }
  return prey_minIndex;
}

/* ReconstructThePath */
struct point* preyRetrace(struct node* prey_goalNode){
  struct node* prey_current = prey_goalNode;
  struct point* prey_ptr = NULL;
  int prey_steps = 0, prey_i;
  while(prey_current->parent != NULL){
      prey_steps++;
      prey_ptr = (struct point*)realloc(prey_ptr, prey_steps*sizeof(struct point));
      memcpy(&prey_ptr[prey_steps-1], prey_current->pnt, sizeof(struct point));
      prey_current = prey_current->parent;                    
  }
  return &prey_ptr[1];
}

int preyFlag(int prey_x, int prey_y){
  if(prey_field[prey_x][prey_y] != -1 && prey_x < 8 && prey_x >= 0 && prey_y < 8 && prey_y >= 0){
      return 1;
    }
  return -1;
}

void Prey(int *prey_ca, int *prey_action){
  int prey_p, prey_q, prey_i;

  struct point *prey_predator = (struct point*)malloc(sizeof(struct point));
  struct point *prey_prey = (struct point*)malloc(sizeof(struct point));

  for(prey_i = 0; prey_i < 64; prey_i++){
    prey_p = prey_i / 8;
    prey_q = prey_i % 8;

    prey_field[prey_p][prey_q] = prey_ca[prey_i];
    
    switch(prey_field[prey_p][prey_q]){
      case 1:   // predator
        prey_predator->x = prey_p, prey_predator->y = prey_q;
        break;
      case 10:  // prey
        prey_prey->x = prey_p, prey_prey->y = prey_q;
        break;
    }
  }

  /* スタートノードの作成 */
  struct point* prey_sP = (struct point*)malloc(sizeof(struct point));
  prey_sP->x = prey_predator->x, prey_sP->y = prey_predator->y;
  struct node* prey_startNode = (struct node*)malloc(sizeof(struct node));
  prey_startNode->pnt = prey_sP, prey_startNode->parent = NULL, prey_startNode->g=0, prey_startNode->h=0;
  
  /* ゴールノードの作成 */
  struct point* prey_gP = (struct point*)malloc(sizeof(struct point));
    prey_gP->x = prey_prey->x, prey_gP->y = prey_prey->y;
    struct node* prey_goalNode = (struct node*)malloc(sizeof(struct node));
    prey_goalNode->pnt = prey_gP, prey_goalNode->parent = NULL, prey_goalNode->g=0, prey_goalNode->h=0;

  /* オープンリストの作成 */
  struct node **prey_openList = (struct node**)malloc(sizeof(struct node*));
  *prey_openList = NULL;

  /* クローズリストの作成 */
  struct node **prey_closedList = (struct node**)malloc(sizeof(struct node*));
  *prey_closedList = NULL;

  (*prey_closedList) = (struct node*)realloc((*prey_closedList), sizeof(struct node));
  memcpy(&((*prey_closedList)[0]), prey_startNode, sizeof(struct node));
  

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct node* prey_finished = preyAStar(prey_startNode, prey_goalNode, prey_openList, 0, prey_closedList, 1);
  struct point* prey_a = preyRetrace(prey_finished);

  int prey_nextX = prey_prey->x + prey_prey->x - prey_a->x;
  int prey_nextY = prey_prey->y + prey_prey->y - prey_a->y;
  int prey_nextActX = prey_prey->x - prey_a->x;
  int prey_nextActY = prey_prey->y - prey_a->y;
  char prey_nextOpe[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
  int prey_actPoint[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
  int prey_r, prey_f;
  
  for(prey_i = 0; prey_i < 4; prey_i++){
    if(prey_nextActX == prey_actPoint[prey_i][0] && prey_nextActY == prey_actPoint[prey_i][1]){
      prey_f = preyFlag(prey_nextX, prey_nextY);
      break;
    }
  }
  if(prey_f == 1){
    *prey_action = (int)(prey_nextOpe[prey_i]);
  }else{
    srand(100);
    if(prey_nextActY != 0){ 
      prey_r = rand() % 2;
      *prey_action = (int)(prey_nextOpe[prey_r]);
    }else{
      prey_r = 2 + rand() % 2;
      *prey_action = (int)(prey_nextOpe[prey_r]);
    }
  }
}
