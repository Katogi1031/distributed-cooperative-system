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
  // struct point* sP = (struct point*)malloc(sizeof(struct point));
  return p;
}

void predator(int *ca){
  char act[] = {'u', 'd', 'l', 'r', 's'}; // up, down, left, right, stay
  int size_1d = 64;
  int size_2d = 8;
  int field[8][8];
  int p, q;
  struct predatorPosition *predator, *prey;

  // predatorCreatepredatorPosition(predator);
  // predatorCreatepredatorPosition(prey);
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

  printf("predator (%d %d)\n", predator->x, predator->y);
  printf("prey     (%d %d)\n", prey->x, prey->y);
}

int main(void) {
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

    // for(i = 0; i < 8; i++){
    //     for(int j = 0; j < 8; j++){
            
    //         printf("%3d ", array[i][j]);
    //     }
    //     printf("\n");
    // }
      
    
  }   
  predator(array);
  fclose(fp);

  

}