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

int predatorMap(){
  FILE *fp;	          /* file pointer */
  int rstat, i;		  /* fscanf return status and loop parameter */
  int array[64];	       /* data array */
  int p, q;

  fp = fopen("battlefield.dat", "r"); /* open file to read */

  if (fp == NULL) {                    /* if fp is NULL, it means open file failed */
    printf("Failed file open.\n"); 
  } else {
    for(i = 0; i < 64; i++){
      // p = i / 8;
      // q = i % 8;
      rstat = fscanf(fp, "%d", &array[i]);
    }
  }

  fclose(fp);
  for(int i = 0; i < 64; i++){
    printf("%d ", array[i]);
  }
  printf("\n");
  return &array;

}

// int field[64] = predatorMap();

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




struct predatorPosition* predatorCreatepredatorPosition(){
  struct predatorPosition* p = (struct predatorPosition*)malloc(sizeof(struct predatorPosition));
  return p;
}





void predator(int *ca){
  printf("1");
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;

  struct predatorPosition *predator, *prey;
  int openLen = 0, closedLen = 0; // オープンリスト、クローズリストの長さ

  

  predator = predatorCreatepredatorPosition();
  prey = predatorCreatepredatorPosition();

  for(int i = 0; i < size_1d; i++){
    p = i / size_2d;
    q = i % size_2d;

    field[p][q] = ca[i];
    
    switch(field[p][q]){
      case 1:   // predator
        predator->x = p, predator->y = q;
        break;
      case 10:  // prey
        prey->x = p, prey->y = q;
        break;
    }

  }

  // printf("predator (%d %d)\n", predator->x, predator->y);
  // printf("prey     (%d %d)\n", prey->x, prey->y);

  /* スタートノードの作成 */
  struct predatorPosition* sp = predatorCreatepredatorPosition();
  sp->x = predator->x, sp->y = predator->y;
  struct predatorNode* sn = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  sn->pnt = sp, sn->parent = NULL, sn->g = 0, sn->h = 0;
  
  /* ゴールノードの作成 */
  struct predatorPosition* gp = predatorCreatepredatorPosition();
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

  

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  // struct predatorNode* finished = AStarAlgorithm(sn, gn, openList, openLen, closedList, closedLen);

  // ReconstructThePath(finished);

  getchar();
  
}

// struct node* AstarALgorithm(struct node* current, struct node* goal, node **openList, int l1, struct node ** closedList, int l2){
//   int i, j, nextIndex;
  
//   /* オープンリストの長さを求める */
//   l1 = ExpandNode(current, openList, l1, closd, l2);

//   /* 経路コストを計算する */
//   CalculateTheTotalCost(goal, openList, l1);
//   struct node *nextNode = &((*openList)[nextIndex]);

//   /* 次のオープンリストを作成する */
//   struct node *tempList = (struct node*)calloc(l1-1, sizeof(struct node));
//   for(i = 0, j = 0; i < l1; i++){
//     if(i != nextIndex){
//       memcpy(&tempList[j], &((*openList)[i]), sizeof(struct node));
//       j++;
//     }
//   }
//   l1--;
//   l2++;
//   *openList = tempList;

//   /* クローズドノードに追加していく */
//   *closedList = (struct node*)realloc((*closedList), l2*(sizeof(struct node)));
//   memcpy(&(*closed)[l2-1], nextNode, sizeof(struct node));

//   /* ゴールに到達していなければAStarAlgorithmを呼び出す */
//   if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt-y == goal->pnt->x){
//     return nextNode;
//   }
//   return AstarALgorithm(nextNode, goal, openList, l1, closedList, l2);

// }

// int ExpandNode(struct node* current, struc node **openList, int l1, struct node **closedList, int l2){
//   int i, j, count, found;
//   count = 0;

//   struct node* tempList = (struct node*)calloc(4, sizeof(struct node));

//   for(i = 0; i < 4; i++){
//     tempList[i].pnt = predatorCreatepredatorPosition();
    
//     if(i == 0 && )
//   }


// }

int main(void) {
  // FILE *fp;	          /* file pointer */
  // int rstat, i;		  /* fscanf return status and loop parameter */
  // int array[64];	       /* data array */

  // fp = fopen("battlefield.dat", "r"); /* open file to read */

  // if (fp == NULL) {                    /* if fp is NULL, it means open file failed */
  //   printf("Failed file open.\n"); 
  // } else {
  //   for(i = 0; i < 64; i++){
  //     rstat = fscanf(fp, "%d", &array[i]);
  //   }

  // }
  // fclose(fp);

  int array[64];

  array = predatorMap();

  predator(array);
  return 0;

}

