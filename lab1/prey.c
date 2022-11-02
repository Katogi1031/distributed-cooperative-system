/*
Author : Atsuya Katogi
Student ID: b1020003
Class : K
Created November 2, 2022
Language : C


Function : prey(int *ca, int action)
Description : Find the shortest path from the A* algorithm and go in the opposite direction to that path.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int prey_field[8][8];
#define prey_E 0.000001 


struct node{
  struct point* prey_pnt;
  struct node* prey_parent;
  int prey_g;
  int prey_h;
};

struct point{
  int prey_x;
  int prey_y;
};

double preySqrt(double prey_a)
{
    prey_a = prey_a < 0 ? -prey_a : prey_a;
    double prey_x = prey_a / 2;
    while (1) {
		double prey_e = prey_x * prey_x - prey_a;
    prey_e = prey_e < 0 ? -prey_e : prey_e;
        if (prey_e < prey_E) return prey_x;
        prey_x -= prey_e / (prey_x * 2);
    }
}

// prey_

/* 関数の呼び出し順に宣言したいための処置 */
struct node* preyAStar(struct node* prey_current, struct node* prey_goal, struct node **prey_openList, int prey_l1, struct node** prey_closedList, int prey_l2);
int preySearch(struct node* prey_current, struct node **prey_openList, int prey_l1, struct node **prey_closedList, int prey_l2);
int preyCalcCost(struct node **prey_openList, struct node* prey_goalNode, int prey_l1);
struct point* preyRetrace(struct node* prey_goalNode);

/* AStarAlgorithm */
struct node* preyAStar(struct node* prey_current, struct node* prey_goal, struct node **prey_openList, int prey_l1, struct node** prey_closedList, int prey_l2){
  int prey_i, prey_j, prey_nextIndex;

  /* オープンリストの長さを求める */
  prey_l1 = preySearch(prey_current, prey_openList, prey_l1, prey_closedList, prey_l2);

  /* 経路コストを計算する */
  prey_nextIndex = preyCalcCost(prey_openList, prey_goal, prey_l1);
  
  struct node *prey_nextNode = &((*prey_openList)[prey_nextIndex]);
  
  /* 次のオープンリストを作成する */
  struct node *prey_tempList = (struct node*)calloc(prey_l1-1, sizeof(struct node));
  for(prey_i = 0, prey_j = 0; prey_i < prey_l1; prey_i++){
    if(prey_i != prey_nextIndex){
      memcpy(&prey_tempList[prey_j], &((*prey_openList)[prey_i]), sizeof(struct node));
      prey_j++;
    } 
  }
  prey_l1--, prey_l2++;
  *prey_openList = prey_tempList;

  /* クローズドノードに追加していく */
  *prey_closedList = (struct node*)realloc((*prey_closedList), prey_l2*(sizeof(struct node)));
  memcpy(&((*prey_closedList)[prey_l2-1]), prey_nextNode, sizeof(struct node));

  /* ゴールに到達していなければAStarAlgorithmを呼び出す */
  if(prey_nextNode->prey_pnt->prey_x == prey_goal->prey_pnt->prey_x && prey_nextNode->prey_pnt->prey_y == prey_goal->prey_pnt->prey_y)  return prey_nextNode;
  return preyAStar(prey_nextNode, prey_goal, prey_openList, prey_l1, prey_closedList, prey_l2);

}

/* preySearch */
int preySearch(struct node* prey_current, struct node **prey_openList, int prey_l1, struct node **prey_closedList, int prey_l2){
  int prey_i, prey_j, prey_count, prey_found;
  prey_count = 0;

  struct node* prey_tempList;
   prey_tempList = (struct node*)calloc(4, sizeof(struct node));   
   for(prey_i = 0; prey_i < 4; prey_i++){
      prey_tempList[prey_i].prey_pnt = (struct point*)malloc(sizeof(struct point));
      if(prey_i == 0 && prey_field[prey_current->prey_pnt->prey_x-1][prey_current->prey_pnt->prey_y] != -1)  // 左に障害物がないか
        prey_tempList[prey_i].prey_pnt->prey_x = prey_current->prey_pnt->prey_x-1, prey_tempList[prey_i].prey_pnt->prey_y = prey_current->prey_pnt->prey_y, prey_tempList[prey_i].prey_g = prey_current->prey_g+1;
      else if(prey_i == 3 && prey_field[prey_current->prey_pnt->prey_x+1][prey_current->prey_pnt->prey_y] != -1) // 右に障害物がないか
        prey_tempList[prey_i].prey_pnt->prey_x = prey_current->prey_pnt->prey_x+1, prey_tempList[prey_i].prey_pnt->prey_y = prey_current->prey_pnt->prey_y, prey_tempList[prey_i].prey_g = prey_current->prey_g+1;
      else if(prey_i == 1 && prey_field[prey_current->prey_pnt->prey_x][prey_current->prey_pnt->prey_y+1] != -1) // 下に障害物がないか
        prey_tempList[prey_i].prey_pnt->prey_x = prey_current->prey_pnt->prey_x, prey_tempList[prey_i].prey_pnt->prey_y = prey_current->prey_pnt->prey_y+1, prey_tempList[prey_i].prey_g = prey_current->prey_g+1;
      else if(prey_i == 2 && prey_field[prey_current->prey_pnt->prey_x][prey_current->prey_pnt->prey_y-1] != -1) // 上に障害物がないか
        prey_tempList[prey_i].prey_pnt->prey_x = prey_current->prey_pnt->prey_x, prey_tempList[prey_i].prey_pnt->prey_y = prey_current->prey_pnt->prey_y-1, prey_tempList[prey_i].prey_g = prey_current->prey_g+1;
    
      prey_tempList[prey_i].prey_parent = prey_current;
   }

  /* 現在のノードが子ノードを持つかどうか*/
  for(prey_j = 0; prey_j < 4; prey_j++){
      prey_found = 0;           
      if(prey_tempList[prey_j].prey_parent == NULL){
        return 0;
      }
      for(prey_i = 0; prey_i < prey_l1; prey_i++) if((*prey_openList)[prey_i].prey_pnt->prey_x == prey_tempList[prey_j].prey_pnt->prey_x && (*prey_openList)[prey_i].prey_pnt->prey_y == prey_tempList[prey_j].prey_pnt->prey_y) prey_found++;
      for(prey_i = 0; prey_i < prey_l2; prey_i++) if(prey_tempList[prey_j].prey_pnt->prey_x == (*prey_closedList)[prey_i].prey_pnt->prey_x && (*prey_closedList)[prey_i].prey_pnt->prey_y == prey_tempList[prey_j].prey_pnt->prey_y) prey_found++; 

      if(prey_found == 0){
        prey_count++;
        int prey_total = prey_l1 + prey_count;
        *prey_openList = (struct node*)realloc((*prey_openList), prey_total*(sizeof(struct node)));
        memcpy(&((*prey_openList)[prey_total-1]), &prey_tempList[prey_j],sizeof(struct node));              
      }      
  } 
  return prey_count + prey_l1;
}



int preyCalcCost(struct node **prey_openList, struct node* prey_goalNode, int prey_l1){
  int prey_i, prey_difx, prey_dify;
  for(prey_i = 0; prey_i < prey_l1; prey_i++){
    prey_difx = (*prey_openList)[prey_i].prey_pnt->prey_x - prey_goalNode->prey_pnt->prey_x;
    prey_dify = (*prey_openList)[prey_i].prey_pnt->prey_y - prey_goalNode->prey_pnt->prey_y;
    
    (*prey_openList)[prey_i].prey_h = ((int)preySqrt(prey_difx * prey_difx + prey_dify * prey_dify));
  }

  int prey_min, prey_minIndex = 0;
  prey_min = (*prey_openList)[0].prey_g + (*prey_openList)[0].prey_h;
  for(prey_i = 1; prey_i < prey_l1; prey_i++){
    if((*prey_openList)[prey_i].prey_g + (*prey_openList)[prey_i].prey_h < prey_min){
      prey_min = (*prey_openList)[prey_i].prey_g + (*prey_openList)[prey_i].prey_h;
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
  while(prey_current->prey_parent != NULL){
      prey_steps++;
      prey_ptr = (struct point*)realloc(prey_ptr, prey_steps*sizeof(struct point));
      memcpy(&prey_ptr[prey_steps-1], prey_current->prey_pnt, sizeof(struct point));
      prey_current = prey_current->prey_parent;                    
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
        prey_predator->prey_x = prey_p, prey_predator->prey_y = prey_q;
        break;
      case 10:  // prey
        prey_prey->prey_x = prey_p, prey_prey->prey_y = prey_q;
        break;
    }
  }

  /* スタートノードの作成 */
  struct point* prey_sP = (struct point*)malloc(sizeof(struct point));
  prey_sP->prey_x = prey_predator->prey_x, prey_sP->prey_y = prey_predator->prey_y;
  struct node* prey_startNode = (struct node*)malloc(sizeof(struct node));
  prey_startNode->prey_pnt = prey_sP, prey_startNode->prey_parent = NULL, prey_startNode->prey_g=0, prey_startNode->prey_h=0;
  
  /* ゴールノードの作成 */
  struct point* prey_gP = (struct point*)malloc(sizeof(struct point));
    prey_gP->prey_x = prey_prey->prey_x, prey_gP->prey_y = prey_prey->prey_y;
    struct node* prey_goalNode = (struct node*)malloc(sizeof(struct node));
    prey_goalNode->prey_pnt = prey_gP, prey_goalNode->prey_parent = NULL, prey_goalNode->prey_g=0, prey_goalNode->prey_h=0;

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

  int prey_nextX = prey_prey->prey_x + prey_prey->prey_x - prey_a->prey_x;
  int prey_nextY = prey_prey->prey_y + prey_prey->prey_y - prey_a->prey_y;
  int prey_nextActX = prey_prey->prey_x - prey_a->prey_x;
  int prey_nextActY = prey_prey->prey_y - prey_a->prey_y;
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
