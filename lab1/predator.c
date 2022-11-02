/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created November 2, 2022
Language : C

Function : predator(int *ca, int action)
Description : Using the A* algorithm, find the shortest distance between Predator and Prey and choose an action.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define predator_E 0.000001 // ニュートン法
int predator_field[8][8];

struct predatorNode{
  struct predatorPoint* predator_pnt;
  struct predatorNode* predator_parent;
  int predator_g;
  int predator_h;
};

struct predatorPoint{
  int predator_x;
  int predator_y;
};


double predator_sqrt(double predator_a)
{
    predator_a = predator_a < 0 ? -predator_a : predator_a;
    double predator_x = predator_a / 2;
    while (1) {
      double predator_e = predator_x * predator_x - predator_a;
      predator_e = predator_e < 0 ? -predator_e : predator_e;
      if (predator_e < predator_E) return predator_x;
      predator_x -= predator_e / (predator_x * 2);
    }
}

/* 関数の呼び出し順に宣言したいための処置 */
struct predatorNode* predatorAStar(struct predatorNode* predator_current, struct predatorNode* predator_goal, struct predatorNode **predator_openList, int predator_l1, struct predatorNode** cpredator_losedList, int predator_l2);
int predatorSearch(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_l1, struct predatorNode **predator_closedList, int predator_l2);
int predatorCalcCost(struct predatorNode **predator_openList, struct predatorNode* predator_goalNode, int predator_l1);
struct predatorPoint* predatorRetrace(struct predatorNode* predator_goalNode);

/* AStarAlgorithm */
struct predatorNode* predatorAStar(struct predatorNode* predator_current, struct predatorNode* predator_goal, struct predatorNode **predator_openList, int predator_l1, struct predatorNode** predator_closedList, int predator_l2){
  int predator_i, predator_j, predator_nextIndex;

  /* オープンリストの長さを求める */
  predator_l1 = predatorSearch(predator_current, predator_openList, predator_l1, predator_closedList, predator_l2);
  /* 経路コストを計算する */
  predator_nextIndex = predatorCalcCost(predator_openList, predator_goal, predator_l1);
  struct predatorNode *predator_nextNode = &((*predator_openList)[predator_nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct predatorNode *predator_tempList = (struct predatorNode*)calloc(predator_l1-1, sizeof(struct predatorNode));
  for(predator_i = 0, predator_j=0; predator_i < predator_l1; predator_i++){
    if(predator_i != predator_nextIndex){
      memcpy(&predator_tempList[predator_j], &((*predator_openList)[predator_i]), sizeof(struct predatorNode));
      predator_j++;
    } 
  }
  predator_l1--, predator_l2++;
  *predator_openList = predator_tempList;

  /* クローズドノードに追加していく */
  *predator_closedList = (struct predatorNode*)realloc((*predator_closedList), predator_l2*(sizeof(struct predatorNode)));
  memcpy(&((*predator_closedList)[predator_l2-1]), predator_nextNode, sizeof(struct predatorNode));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(predator_nextNode->predator_pnt->predator_x == predator_goal->predator_pnt->predator_x && predator_nextNode->predator_pnt->predator_y == predator_goal->predator_pnt->predator_y)  return predator_nextNode;
  return predatorAStar(predator_nextNode, predator_goal, predator_openList, predator_l1, predator_closedList, predator_l2);
}

/* ExpandNode */
int predatorSearch(struct predatorNode* predator_current, struct predatorNode **predator_openList, int predator_l1, struct predatorNode **predator_closedList, int predator_l2){
  int predator_i, predator_j, predator_count, predator_found;
  predator_count = 0;
  int predator_cx = predator_current->predator_pnt->predator_x; // 探索中の今現在のpredatorの座標
  int cPredator_y = predator_current->predator_pnt->predator_y;

  struct predatorNode* predator_tempList = (struct predatorNode*)calloc(4, sizeof(struct predatorNode));   
   for(predator_i = 0; predator_i < 4; predator_i++){
      predator_tempList[predator_i].predator_pnt = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
      if(predator_i == 0 && predator_field[predator_cx-1][cPredator_y] != -1)  // 上に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_cx-1, predator_tempList[predator_i].predator_pnt->predator_y = cPredator_y, predator_tempList[predator_i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 3 && predator_field[predator_cx+1][cPredator_y] != -1) // 下に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_cx+1,predator_tempList[predator_i].predator_pnt->predator_y = cPredator_y, predator_tempList[predator_i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 1 && predator_field[predator_cx][cPredator_y+1] != -1) // 右に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_cx, predator_tempList[predator_i].predator_pnt->predator_y = cPredator_y+1, predator_tempList[predator_i].predator_g = predator_current->predator_g+1.0f;
      else if(predator_i == 2 && predator_field[predator_cx][cPredator_y-1] != -1) // 左に障害物がないか
        predator_tempList[predator_i].predator_pnt->predator_x = predator_cx, predator_tempList[predator_i].predator_pnt->predator_y = cPredator_y-1, predator_tempList[predator_i].predator_g = predator_current->predator_g+1.0f;
    
      predator_tempList[predator_i].predator_parent = predator_current;
   }

  /* 現在のノードが小ノードを持つかどうか*/
  for(predator_j = 0; predator_j < 4; predator_j++){
      predator_found = 0;           
      if(predator_tempList[predator_j].predator_parent == NULL) return 0;
      for(predator_i = 0; predator_i < predator_l1; predator_i++) if((*predator_openList)[predator_i].predator_pnt->predator_x == predator_tempList[predator_j].predator_pnt->predator_x && (*predator_openList)[predator_i].predator_pnt->predator_y == predator_tempList[predator_j].predator_pnt->predator_y) predator_found++;
      for(predator_i = 0; predator_i < predator_l2; predator_i++) if(predator_tempList[predator_j].predator_pnt->predator_x == (*predator_closedList)[predator_i].predator_pnt->predator_x && (*predator_closedList)[predator_i].predator_pnt->predator_y == predator_tempList[predator_j].predator_pnt->predator_y) predator_found++; 
      if(predator_found == 0){
        predator_count++;
        int predator_total = predator_l1 + predator_count;
        *predator_openList = (struct predatorNode*)realloc((*predator_openList), predator_total*(sizeof(struct predatorNode)));
        memcpy(&((*predator_openList)[predator_total-1]), &predator_tempList[predator_j],sizeof(struct predatorNode));              
      }      
   } 
  return predator_count + predator_l1;
}

/* CalcCost */
int predatorCalcCost(struct predatorNode **predator_openList, struct predatorNode* predator_goalNode, int predator_l1){
  int predator_i, predator_difx, predator_dify;
  for(predator_i = 0; predator_i < predator_l1; predator_i++){
    predator_difx = (*predator_openList)[predator_i].predator_pnt->predator_x - predator_goalNode->predator_pnt->predator_x;
    predator_dify = (*predator_openList)[predator_i].predator_pnt->predator_y - predator_goalNode->predator_pnt->predator_y;
    (*predator_openList)[predator_i].predator_h = ((int)predator_sqrt(predator_difx*predator_difx+predator_dify*predator_dify));
  }

  int predator_min, predator_minIndex;
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

/* Retrace */
struct predatorPoint* predatorRetrace(struct predatorNode* predator_goalNode){
  struct predatorNode* predator_current = predator_goalNode;
  struct predatorPoint* predator_ptr = NULL;
  int predator_steps = 0, predator_i;
  while(predator_current->predator_parent != NULL){
      predator_steps++;
      predator_ptr = (struct predatorPoint*)realloc(predator_ptr, predator_steps*sizeof(struct predatorPoint));
      memcpy(&predator_ptr[predator_steps-1], predator_current->predator_pnt, sizeof(struct predatorPoint));
      predator_current = predator_current->predator_parent;                    
  }
  return &predator_ptr[predator_steps-1];
}

void Predator(int *predator_ca, int *predator_action){
  int predator_p, predator_q;
  struct predatorPoint *predator_predator = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  struct predatorPoint *predator_prey = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));

  for(int predator_i = 0; predator_i < 64; predator_i++){
    predator_p = predator_i / 8;
    predator_q = predator_i % 8;

    predator_field[predator_p][predator_q] = predator_ca[predator_i];
    
    switch(predator_field[predator_p][predator_q]){
      case 1:   // predator
        predator_predator->predator_x = predator_p, predator_predator->predator_y = predator_q;
        break;
      case 10:  // prey
        predator_prey->predator_x = predator_p, predator_prey->predator_y = predator_q;
        break;
    }
  }

  /* スタートノードの作成 */
  struct predatorPoint* predator_sP = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
  predator_sP->predator_x = predator_predator->predator_x, predator_sP->predator_y = predator_predator->predator_y;
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
  memcpy(&((*predator_closedList)[0]), predator_startNode, sizeof(struct predatorNode));
  
  /* 現在位置からゴールまで全てのノードを保持するノードを作成 */
  struct predatorNode* predator_finished = predatorAStar(predator_startNode, predator_goalNode, predator_openList, 0, predator_closedList, 1);
  struct predatorPoint* predator_a = predatorRetrace(predator_finished);

  char predator_act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int predator_point[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  for(int predator_i = 0; predator_i < 4; predator_i++){
    if(predator_a->predator_x == predator_predator->predator_x + predator_point[predator_i][0] && predator_a->predator_y == predator_predator->predator_y + predator_point[predator_i][1]){
      *predator_action = (int)predator_act[predator_i];
    }
  }
}