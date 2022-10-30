#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <string.h>

struct node{
  struct point* pnt;
  struct node* parent;
  int f;
  int g;
  int h;
};

struct point{
  int x;
  int y;
};

void astar(int* array){
  
  int p, q;

  struct point* predator = (struct point*)malloc(sizeof(struct point));
  struct point* prey = (struct point*)malloc(sizeof(struct point));

  for(int i = 0; i < 64; i++){
    switch(array[i]){
      case 1:  // predator
        p = i % 8;
        q = i % 8;
        predator->y = p, predator->x = q;
        break;
      case 10: // prey
        p = i % 8;
        q = i % 8;
        prey->y = p, prey->x = q;
        break;

      default:
        break;
    }
  }

  struct node* startNode = (struct node*)malloc(sizeof(struct node));
  startNode->pnt = predator, startNode->parent = NULL;
  startNode->f = 0, startNode->g = 0, startNode->h = 0;
  
  printf("%d %d\n", startNode->pnt->y, startNode->pnt->x);

  struct node* goalNode = (struct node*)malloc(sizeof(struct node));
  goalNode->pnt = prey, goalNode->parent = NULL;
  goalNode->f = 0, goalNode->g = 0, goalNode->h = 0;

  struct node** openList = (struct node**)malloc(sizeof(struct node*));
  *openList = NULL;

  struct node** closedList = (struct node**)malloc(sizeof(struct node*));
  *closedList = NULL;
  *openList = (struct node*)realloc((*openList), sizeof(struct node));
  memcpy(&((*openList)[0]), startNode, sizeof(struct node));
  int openLen = 1;

  
  while(openLen > 0){
    struct node* currentNode = (struct node*)malloc(sizeof(struct node));
    memcpy(&(*currentNode), &((*openList)[0]), sizeof(struct node));
    printf("%d %d\n", currentNode->pnt->y, currentNode->pnt->x);
    // openLen--;
    int currentIndex = 0;

    for(int i = 0; i < openLen; i++){
      struct node* tempNode = (struct node*)malloc(sizeof(struct node));
      memcpy(&(*tempNode), &((*openList)[0]), sizeof(struct node));
      if(tempNode->f < currentNode->f){
        memcpy(&(*currentNode), &(*tempNode), sizeof(struc node));
        int currentIndex = i;
      }
    }

    


  }


}

int main(void){
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
  astar(array);
}
