/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created October 25, 2022
Language : C


Function predator(int *ca, int action)
Description
*/

/*
ExpandNode()に問題あり
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


int gfield[8][8];

struct node{
  struct point* pnt;
  struct node* parentNode;
  float g;
  float h;
};

struct point{
  int x;
  int y;
};

struct point* predatorCreatePosition(){
  struct point* p = (struct point*)malloc(sizeof(struct point));
  return p;
}

/* 関数の呼び出し順に宣言したいための処置 */
struct node* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2);
int ExpandNode(struct node* current, struct node **openList, int l1, struct node **closedList, int l2);
void CalculateTheTotalCost(struct node* goalNode, struct node **openList, int l1);
int FindTheLeastCosted(struct node **openList, int l1);
void ReconstructThePath(struct node* goalNode);

/* AStarAlgorithm */
struct node* AStarAlgorithm(struct node* current, struct node* goal, struct node **openList, int l1, struct node** closedList, int l2){
  int i, j, nextIndex;

  static int count = 0;
  printf("count = %d\n", count);
  count++;
  
  /* オープンリストの長さを求める */
  l1 = ExpandNode(current, openList, l1, closedList, l2);
  

  /* 経路コストを計算する */
  CalculateTheTotalCost(goal, openList, l1); //問題あり
  
  struct node *nextNode = &((*openList)[nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct node *tempList = (struct node*)calloc(l1-1, sizeof(struct node));
  
  for(i = 0, j = 0; i < l1; i++){
    if(i != nextIndex){
      memcpy(&tempList[j], &((*openList)[i]), sizeof(struct node));
      j++;
    }
  }
  l1--;
  l2++;
  *openList = tempList;

  

  /* クローズドノードに追加していく */
  *closedList = (struct node*)realloc((*closedList), l2*(sizeof(struct node)));
  memcpy(&(*closedList)[l2-1], nextNode, sizeof(struct node));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->x){
    return nextNode;
  }
  return AStarAlgorithm(nextNode, goal, openList, l1, closedList, l2);

}

/* ExpandNode */
int ExpandNode(struct node* current,struct node **array,int l1,struct node **closed,int l2)
{   
    // printf("%d\n", l1);
   int i,j,count,found;
   count = 0;
   //--FIRST-PHASE--:EXPAND ALL THE NEIGHBORING NODES...
   struct node* tempArray;
   tempArray = (struct node*)calloc(4,sizeof(struct node));   
   for(i = 0;i<4;i++){
      tempArray[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && gfield[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempArray[i].pnt->x = current->pnt->x-1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 3 && gfield[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempArray[i].pnt->x = current->pnt->x+1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 1 && gfield[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y+1,tempArray[i].g = current->g+1.0f;
      else if(i == 2 && gfield[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y-1,tempArray[i].g = current->g+1.0f;
    
      tempArray[i].parentNode = current;
   }

   //--SECOND-PHASE--:check whether current node's children are inside the array.
   //if so, do not include those nodes to the expanded array. 
   for(j = 0;j<4;j++){
      found = 0;           
      if(tempArray[j].parentNode == NULL){
        return 0;
      }
      for(i = 0;i<l1;i++) if((*array)[i].pnt->x == tempArray[j].pnt->x && (*array)[i].pnt->y == tempArray[j].pnt->y) found++;
      for(i = 0;i<l2;i++) if(tempArray[j].pnt->x == (*closed)[i].pnt->x && (*closed)[i].pnt->y == tempArray[j].pnt->y) found++; 
      //--THIRD-PHASE--:expand the array with the additional neighbors.
      if(found == 0)
      {
        count++;
        int total = (l1+count);
        *array = (struct node*)realloc((*array),total*(sizeof(struct node)));
        memcpy(&((*array)[total-1]),&tempArray[j],sizeof(struct node));              
      }      
   } 
   return count+l1;     
}


/* CalculateTheTotalCost */
void CalculateTheTotalCost(struct node* goalNode,struct node **array,int l1)
{
    int i,difx,dify;
    for(i = 0;i<l1;i++)
    {
        difx = (*array)[i].pnt->x - goalNode->pnt->x;
        dify = (*array)[i].pnt->y - goalNode->pnt->y;
        (*array)[i].h = ((float)sqrt(pow(difx,2) + pow(dify,2)));
    }
}

/* FindTheLeastCosted */
int FindTheLeastCosted(struct node **openList, int l1){
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

/* ReconstructThePath */
void ReconstructThePath(struct node* goalNode){
  struct node* current = goalNode;
  struct point* ptr = NULL;
  int steps = 0, i;
  
  while(current->parentNode != NULL){
    steps++;
    ptr = (struct point*)realloc(ptr, steps*sizeof(struct point));
    memcpy(&ptr[steps-1], current->pnt, sizeof(struct point));
    current = current->parentNode;
  }
  for(i = steps; i >= 1; i--){
    gfield[ptr[i-1].x][ptr[i-1].y] = 20;
    printf("(%d, %d)", ptr[i-1].x, ptr[i-1].y);
    if(i > 1)
      printf("=>");
  }
  printf("\n");
}




void predator(int *ca){
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;

  struct point *predator, *prey;
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
  struct point* sp = predatorCreatePosition();
  sp->x = predator->x, sp->y = predator->y;
  struct node* sn = (struct node*)malloc(sizeof(struct node));
  sn->pnt = sp, sn->parentNode = NULL, sn->g = 0, sn->h = 0;
  
  /* ゴールノードの作成 */
  struct point* gp = predatorCreatePosition();
  gp->x = prey->x, gp->y = prey->y;
  struct node* gn = (struct node*)malloc(sizeof(struct node));
  gn->pnt = gp, gn->parentNode = NULL, gn->g = 0, gn->h = 0;

  /* オープンリストの作成 */
  struct node **openList = (struct node**)malloc(sizeof(struct node*));
  *openList = NULL;

  /* クローズリストの作成 */
  struct node **closedList = (struct node**)malloc(sizeof(struct node*));
  
  *closedList = NULL;

  *closedList = (struct node*)realloc((*closedList), sizeof(struct node));
  memcpy(&((*closedList)[0]), sn, sizeof(struct node));
  closedLen = 1;

 

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct node* finished = AStarAlgorithm(sn, gn, openList, openLen, closedList, closedLen);
  
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

