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

int gfield[8][8]; // グローバル変数としてフィールド情報を保持する

struct node{
  struct point* pnt;
  struct node* parent;
  int g;
  int h;
  int f;
};

struct point{
  int x;
  int y;
};


/* 関数の呼び出し順に宣言したいための処置 */
struct point* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2);
int ExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2);
int CalcCost(struct node** openList, struct node* goalNode, int l1);
struct point* ReconstructThePath(struct node* goalNode);

/* AStarAlgorithm */
struct point* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2){
  
  int i, j, nextIndex;

  /* オープンリストの長さを求める */
  l1 = ExpandNode(current, openList, l1, closedList, l2);
  printf("Astar\n");
  /* 経路コストを計算する */
  nextIndex = CalcCost(openList, goal, l1);
  
  struct node *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct node *tempList = (struct node*)calloc(l1-1, sizeof(struct node));
  for(i = 0, j = 0; i < l1; i++){
    if(i != nextIndex){
      tempList[j] = (*openList)[i];
      j++;
    } 
  }
  l1--,l2++;
  *openList = tempList;

  

  /* クローズドノードに追加していく */
  *closedList = (struct node*)realloc((*closedList), l2*(sizeof(struct node)));
  (*closedList)[l2-1] = *nextNode;

  

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y){
    struct point *p = ReconstructThePath(nextNode);
    return p;
  }  
  return AStarAlgorithm(nextNode, goal, openList, l1, closedList, l2);

}

/* ExpandNode */
int ExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2){
  int i, j, count, found;
  count = 0;

  struct node* tempList = (struct node*)calloc(4, sizeof(struct node));   
  int dx = current->pnt->x;
  int dy = current->pnt->y;
   for(i = 0; i < 4; i++){
      
      tempList[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && gfield[dx-1][dy] != -1)  // 左に障害物がないか
        tempList[i].pnt->x = dx-1,tempList[i].pnt->y = dy,tempList[i].g = current->g + 1;
      else if(i == 3 && gfield[dx+1][dy] != -1) // 右に障害物がないか
        tempList[i].pnt->x = dx+1,tempList[i].pnt->y = dy,tempList[i].g = current->g + 1;
      else if(i == 1 && gfield[dx][dy+1] != -1) // 下に障害物がないか
        tempList[i].pnt->x = dx,tempList[i].pnt->y = dy+1,tempList[i].g = current->g + 1;
      else if(i == 2 && gfield[dx][dy-1] != -1) // 上に障害物がないか
        tempList[i].pnt->x = dx, tempList[i].pnt->y = dy-1,tempList[i].g = current->g + 1;
    
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
        // memcpy(&((*openList)[total-1]),&tempList[j],sizeof(struct node)); 
        (*openList)[total-1] = tempList[j];
      }      
   } 
  return count + l1;
}

int CalcCost(struct node** openList, struct node* goalNode, int l1){
  int i, difx, dify;
  for(i = 0; i < l1; i++){
    difx = (*openList)[i].pnt->x - goalNode->pnt->x;
    dify = (*openList)[i].pnt->y - goalNode->pnt->y;
    (*openList)[i].h = (int)(difx*difx + dify*dify); //((float)sqrt(pow(difx,2) + pow(dify,2)));
  }

  int min, minIndex = 0;
  (*openList)[0].f = (*openList)[0].g + (*openList)[0].h;
  min = (*openList)[0].f;
  
  for(i = 0; i < l1; i++){
    (*openList)[i].f = (*openList)[i].g + (*openList)[i].h;
    if((*openList)[i].f < min){
      min = (*openList)[i].f;
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
      ptr[steps-1] = *current->pnt;
      current = current->parent;                    
  }
  // printf("%d %d\n", ptr[steps-1].y, ptr[steps-1].x);
  return &ptr[steps-1];
}




void Predator(int *ca, int *action){
  
  int p, q;
  int openLen = 0, closedLen = 0; // オープンリスト、クローズリストの長さ
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
      default:
        break;
    }
  }

  printf("predator (%d %d)\n", predator->x, predator->y);
  printf("prey     (%d %d)\n", prey->x, prey->y);

  /* スタートノードの作成 */
  struct point* sP = (struct point*)malloc(sizeof(struct point));
  sP->x = predator->x,sP->y = predator->y;
  struct node* startNode = (struct node*)malloc(sizeof(struct node));
  startNode->pnt = sP,startNode->parent = NULL,startNode->g=0;startNode->h=0;
  
  /* ゴールノードの作成 */
  struct point* gP = (struct point*)malloc(sizeof(struct point));
    gP->x = prey->x,gP->y = prey->y;
    struct node* goalNode = (struct node*)malloc(sizeof(struct node));
    goalNode->pnt = gP,goalNode->parent = NULL,goalNode->g=0,startNode->h=0;

  /* オープンリストの作成 */
  struct node **openList = (struct node**)malloc(sizeof(struct node*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct node **closedList = (struct node**)malloc(sizeof(struct node*));
  *closedList = NULL;

  (*closedList) = (struct node*)realloc((*closedList),sizeof(struct node));
  // memcpy(&((*closedList)[0]),startNode,sizeof(struct node));
  (*closedList)[0] = *startNode;
  
  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */

  struct point *nextPoint = AStarAlgorithm(startNode, goalNode, openList, 0, closedList, 1);
  
  printf("%d %d\n", nextPoint->y, nextPoint->x);

  int act[4][2] = {{-1, 0}, {1 ,0}, {0, -1}, {0, 1}}; // up, down, left, right
  char actStr[] = {'u', 'd', 'l', 'r'};
  for(int i = 0; i < 4; i++){
    if(act[i][0] == nextPoint->y && act[i][1] == nextPoint->x){
      *action = (int)actStr[i];
    }
  }


}

