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


int gfield[8][8];
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

double my_sqrt(double a)
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


/* 関数の呼び出し順に宣言したいための処置 */
struct node* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2);
int ExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2);
int CalcCost(struct node **openList, struct node* goalNode, int l1);
struct point* ReconstructThePath(struct node* goalNode);

/* AStarAlgorithm */
struct node* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2){
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = ExpandNode(current, openList, l1, closedList, l2);

  /* 経路コストを計算する */
  nextIndex = CalcCost(openList, goal, l1);
  
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
  return AStarAlgorithm(nextNode, goal, openList, l1, closedList, l2);

}

/* ExpandNode */
int ExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct node* tempList;
   tempList = (struct node*)calloc(4,sizeof(struct node));   
   for(i = 0;i<4;i++){
      tempList[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && gfield[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempList[i].pnt->x = current->pnt->x-1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1;
      else if(i == 3 && gfield[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempList[i].pnt->x = current->pnt->x+1,tempList[i].pnt->y = current->pnt->y,tempList[i].g = current->g+1;
      else if(i == 1 && gfield[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempList[i].pnt->x = current->pnt->x,tempList[i].pnt->y = current->pnt->y+1,tempList[i].g = current->g+1;
      else if(i == 2 && gfield[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
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



int CalcCost(struct node **openList, struct node* goalNode, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    
    (*openList)[i].h = ((int)my_sqrt(difx*difx+dify*dify));
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
struct point* ReconstructThePath(struct node* goalNode){
  struct node* current = goalNode;
  struct point* ptr = NULL;
  int steps = 0,i;
  while(current->parent != NULL){
      steps++;
      ptr = (struct point*)realloc(ptr,steps*sizeof(struct point));
      memcpy(&ptr[steps-1],current->pnt,sizeof(struct point));
      current = current->parent;                    
  }
  return &ptr[1];
}

int flag(int x, int y){
  if(gfield[x][y] != -1 && x < 8 && x >= 0 && y < 8 && y >= 0){
      return 1;
    }
  return -1;
}

void Prey(int *ca, int *action){
  int p, q;

  struct point *predator = (struct point*)malloc(sizeof(struct point));
  struct point *prey = (struct point*)malloc(sizeof(struct point));

  for(int i = 0; i < 64; i++){
    p = i / 8;
    q = i % 8;

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

  /* スタートノードの作成 */
  struct point* sP = (struct point*)malloc(sizeof(struct point));
  sP->x = predator->x,sP->y = predator->y;
  struct node* startNode = (struct node*)malloc(sizeof(struct node));
  startNode->pnt = sP,startNode->parent = NULL,startNode->g=0;startNode->h=0;
  
  /* ゴールノードの作成 */
  struct point* gP = (struct point*)malloc(sizeof(struct point));
    gP->x = prey->x, gP->y = prey->y;
    struct node* goalNode = (struct node*)malloc(sizeof(struct node));
    goalNode->pnt = gP,goalNode->parent = NULL,goalNode->g=0,goalNode->h=0;

  /* オープンリストの作成 */
  struct node **openList = (struct node**)malloc(sizeof(struct node*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct node **closedList = (struct node**)malloc(sizeof(struct node*));
  *closedList = NULL;

  (*closedList) = (struct node*)realloc((*closedList),sizeof(struct node));
  memcpy(&((*closedList)[0]),startNode,sizeof(struct node));
  

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct node* finished = AStarAlgorithm(startNode, goalNode, openList, 0, closedList, 1);
  struct point* a = ReconstructThePath(finished);

  int nextX = prey->x + prey->x - a->x;
  int nextY = prey->y + prey->y - a->y;
  int nextActX = prey->x - a->x;
  int nextActY = prey->y - a->y;
  char nextOpe[] = {'u', 'd', 'l', 'r'}; // up, down, left, right, stay
  int actPoint[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
  int r, f, i;
  
  for(i = 0; i < 4; i++){
    if(nextActX == actPoint[i][0] && nextActY == actPoint[i][1]){
      f = flag(nextX, nextY);
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
