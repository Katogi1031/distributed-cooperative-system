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


int predator_field[8][8];

struct predatorNode{
  struct predatorPoint* predator_pnt;
  struct predatorNode* predator_parent;
  float predator_g;
  float predator_h;
};

struct predatorPoint{
  int predator_x;
  int predator_y;
};

/* 関数の呼び出し順に宣言したいための処置 */
struct predatorNode* predator_astar(struct predatorNode* predator_current, struct predatorNode* predator_goalNode, struct predatorNode **predator_openList, int predator_openIndex, struct predatorNode** predator_closedList, int predator_closedIndex);
int predator_expand(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_openIndex, struct predatorNode **predator_closedList, int predator_closedIndex);
void predator_calcCust(struct predatorNode* predator_goalNode, struct predatorNode **predator_openList, int predator_openIndex);
int predator_calcpredator_totalCost(struct predatorNode **predator_openList, int predator_openIndex);
// void predator_nextAct(struct predatorNode* predator_goalNode);
struct predatorPoint* predator_nextAct(struct predatorNode* predator_goalNode);

/* predator_astar */
struct predatorNode* predator_astar(struct predatorNode* predator_current, struct predatorNode* goal, struct predatorNode **predator_openList, int predator_openIndex, struct predatorNode** predator_closedList, int predator_closedIndex){
  int predator_i, predator_j, predator_nextIndex;

  /* オープンリストの長さを求める */
  predator_openIndex = predator_expand(predator_current, predator_openList, predator_openIndex, predator_closedList, predator_closedIndex);

  /* 経路コストを計算する */
  predator_calcCust(predator_goalNode, predator_openList, predator_openIndex);

  predator_nextIndex = predator_calcpredator_totalCost(predator_openList, predator_openIndex);
  
  struct predatorNode *predator_nextNode = &((*predator_openList)[predator_nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct predatorNode *predator_tempList = (struct predatorNode*)calloc(predator_openIndex-1, sizeof(struct predatorNode));
  for(predator_i = 0,predator_j=0; predator_i < predator_openIndex; predator_i++){
    if(predator_i != predator_nextIndex){
      memcpy(&predator_tempList[j],&((*predator_openList)[i]),sizeof(struct predatorNode));
      predator_j++;
    } 
  }
  predator_openIndex--, predator_closedIndex++;
  *predator_openList = predator_tempList;

  /* クローズドノードに追加していく */
  *predator_closedList = (struct predatorNode*)realloc((*predator_closedList), predator_closedIndex*(sizeof(struct predatorNode)));
  memcpy(&((*predator_closedList)[predator_closedIndex-1]),predator_nextNode,sizeof(struct predatorNode));

  /* ゴールに到達していなければpredator_astarを呼び出す */
  if(predator_nextNode->predator_pnt->predator_x == predator_goalNode->predator_pnt->predator_x && predator_nextNode->predator_pnt->predator_y == predator_goalNode->predator_pnt->predator_y){
    return predator_nextNode;
  }
  if(predator_nextNode->predator_pnt->predator_x == predator_goalNode->predator_pnt->predator_x && predator_nextNode->predator_pnt->predator_y == predator_goalNode->predator_pnt->predator_y)  return predator_nextNode;
  return predator_astar(predator_nextNode, predator_goalNode, predator_openList, predator_openIndex, predator_closedList, predator_closedIndex);

}

/* predator_expand */
int predator_expand(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_openIndex, struct predatorNode **predator_closedList, int predator_closedIndex){
  int predator_i, predator_j, predator_count, predator_found;
  predator_count = 0;

  struct predatorNode* predator_tempList;
   predator_tempList = (struct predatorNode*)calloc(4,sizeof(struct predatorNode));   
   for(predator_i = 0; predator_i < 4; predator_i++){
      predator_tempList[predator_i].predator_pnt = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
      if(predator_i == 0 && predator_field[predator_current->predator_pnt->predator_x-1][predator_current->predator_pnt->predator_y] != -1)  // 左に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_current->predator_pnt->predator_x-1,predator_tempList[predator_i].predator_pnt->predator_y = predator_current->predator_pnt->predator_y,predator_tempList[predator_i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 3 && predator_field[predator_current->predator_pnt->predator_x+1][predator_current->predator_pnt->predator_y] != -1) // 右に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_current->predator_pnt->predator_x+1,predator_tempList[predator_i].predator_pnt->predator_y = predator_current->predator_pnt->predator_y,predator_tempList[i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 1 && predator_field[predator_current->predator_pnt->predator_x][predator_current->predator_pnt->predator_y+1] != -1) // 下に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_current->predator_pnt->predator_x,predator_tempList[predator_i].predator_pnt->predator_y = predator_current->predator_pnt->predator_y+1,predator_tempList[i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 2 && predator_field[predator_current->predator_pnt->predator_x][predator_current->predator_pnt->predator_y-1] != -1) // 上に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_current->predator_pnt->predator_x,predator_tempList[predator_i].predator_pnt->predator_y = predator_current->predator_pnt->predator_y-1,predator_tempList[i].predator_g = predator_current->predator_g+1.0f;
    
      predator_tempList[predator_i].predator_parent = predator_current;
   }

  /* 現在のノードが小ノードを持つかどうか*/
  for(predator_j = 0; predator_j < 4; predator_j++){
      predpredator_found = 0;           
      if(predator_tempList[j].predator_parent == NULL){
        return 0;
      }
      for(predator_i = 0; predator_i < predator_openIndex; predator_i++) if((*predator_openList)[predator_i].predator_pnt->predator_x == predator_tempList[predator_j].predator_pnt->predator_x && (*predator_openList)[predator_i].predator_pnt->predator_y == predator_tempList[predator_j].predator_pnt->predator_y) predator_found++;
      for(predator_i = 0; predator_i < predator_closedIndex; predator_i++) if(predator_tempList[predator_j].predator_pnt->predator_x == (*predator_closedList)[predator_i].predator_pnt->predator_x && (*predator_closedList)[predator_i].predator_pnt->predator_y == predator_tempList[predator_j].predator_pnt->predator_y) predator_found++; 
      //--THIRD-PHASE--:expand the array with the additional neighbors.
      if(predator_found == 0)
      {
        predator_count++;
        int predator_total = (predator_openIndex+predator_count);
        *predator_openList = (struct predatorNode*)realloc((*predator_openList),predator_total*(sizeof(struct predatorNode)));
        memcpy(&((*predator_openList)[predator_total-1]),&predator_tempList[predator_j],sizeof(struct predatorNode));              
      }      
   } 
  return predator_count + predator_openIndex;
}



/* predator_calcCust */
void predator_calcCust(struct predatorNode* predator_goalNode, struct predatorNode **predator_openList, int predator_openIndex){
  int predator_i, predator_difx, predator_dify;
  for(predator_i = 0; predator_i < predator_openIndex; predator_i++){
    predator_difx = (*predator_openList)[predator_i].predator_pnt->predator_x - predator_goalNode->predator_pnt->predator_x;
    predator_dify = (*predator_openList)[predator_i].predator_pnt->predator_y - predator_goalNode->predator_pnt->predator_y;
    (*predator_openList)[predator_i].predator_h = ((float)sqrt(pow(predator_difx,2) + pow(predator_dify,2)));
  }
}

/* predator_calcpredator_totalCost */
int predator_calcpredator_totalCost(struct predatorNode **predator_openList, int predator_openIndex){
  int predator_i, predator_min, predator_minIndex;
    predator_min = (*predator_openList)[0].predator_g + (*predator_openList)[0].predator_h;
    predator_minIndex = 0;
    for(predator_i = 1; predator_i < predator_openIndex; predator_i++){
      if((*predator_openList)[predator_i].predator_g + (*predator_openList)[predator_i].h < predator_min){
        predator_min = (*predator_openList)[predator_i].predator_g + (*predator_openList)[predator_i].predator_h;
        predator_minIndex = predator_i;
      } 
    }
    return predator_minIndex;
}

/* predator_nextAct */
// void predator_nextAct(struct predatorNode* predator_goalNode){
struct predatorPoint* predator_nextAct(struct predatorNode* predator_goalNode){
  struct predatorNode* predator_current = predator_goalNode;
  struct predatorPoint* predator_ptr = NULL;
  int predator_steps = 0, predator_i;
  while(predator_current->predator_parent != NULL){
      predator_steps++;
      predator_ptr = (struct predatorPoint*)realloc(predator_ptr, predator_steps*sizeof(struct predatorPoint));
      memcpy(&predator_ptr[predator_steps-1],predator_current->predator_pnt,sizeof(struct predatorPoint));
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




void Predator(int *predator_ca, int *predator_action){
  char predator_act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int predator_size_1d = 64;
  int predator_size_2d = 8;
  int predator_field[8][8];
  int predator_p, predator_q;

  struct predatorPoint *predator_predator = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  struct predatorPoint *predator_prey = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));

  for(int predator_i = 0; predator_i < predator_size_1d; predator_i++){
    predator_p = predator_i / predator_size_2d;
    predator_q = predator_i % predator_size_2d;

    predator_field[predator_p][predator_q] = predator_ca[predator_i];
    
    switch(predator_field[predator_p][predator_q]){
      case 1:   // predator
        predator->predator_x = predator_p, predator->predator_y = predator_q;
        break;
      case 10:  // prey
        prey->predator_x = predator_p, prey->predator_y = predator_q;
        break;
    }
  }

  printf("predator (%d %d)\n", predator_predator->predator_x, predator_predator->predator_y);
  printf("prey     (%d %d)\n", predator_prey->predator_x, predator_prey->predator_y);

  /* スタートノードの作成 */
  struct predatorPoint* predator_sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  predator_sP->predator_predator_x = predator_predator->predator_x,predator_sP->predator_y = predator_predator->predator_y;
  struct predatorNode* predator_startNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
  predator_startNode->predator_pnt = predator_sP, predator_startNode->predator_parent = NULL, predator_startNode->predator_g=0, predator_startNode->predator_h=0;
  
  /* ゴールノードの作成 */
  struct predatorPoint* predator_gP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    predator_gP->predator_x = predator_prey->predator_x, predator_gP->predator_y = predator_prey->predator_y;
    struct predatorNode* predator_goalNode = (struct predatorNode*)malloc(sizeof(struct predatorNode));
    predator_goalNode->predator_pnt = predator_gP, predator_goalNode->predator_parent = NULL, predator_goalNode->predator_g=0, predator_startNode->predator_h=0;

  /* オープンリストの作成 */
  struct predatorNode **predator_openList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *predator_openList = NULL;

  /* クローズリストの作成 */
  struct predatorNode **predator_closedList = (struct predatorNode**)malloc(sizeof(struct predatorNode*));
  *predator_closedList = NULL;

  (*predator_closedList) = (struct predatorNode*)realloc((*predator_closedList),sizeof(struct predatorNode));
  memcpy(&((*predator_closedList)[0]),predator_startNode,sizeof(struct predatorNode));

  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct predatorNode* predator_finished = predator_astar(predator_startNode, predator_goalNode, predator_openList, 0, predator_closedList, 1);
  
  struct predatorPoint* predator_a = predator_nextAct(predator_finished);

  printf("next Action (%d %d)\n", predator_a->predator_x, predator_a->predator_y);

  if(predator_a->predator_x - predator_predator->predator_x == 1){
    *predator_action = (int)predator_act[1];
    printf("Down\n");
  }else if(predator_a->predator_x - predator_predator->predator_x == -1){
    *predator_action = (int)act[0];
  }else if(predator_a->predator_y - predator_predator->predator_y == 1){
    *predator_action = (int)act[3];
  }else{
    *predator_action = (int)act[2];
  }
  printf("%d\n", *predator_action);
  
}
