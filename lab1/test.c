#include <stdio.h>
#include <string.h>

int *predatorMap(){
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
  return array;

}

int main(void) {
  int array[64];

  memcpy(array, predatorMap(), sizeof(*predatorMap()));
  // array = predatorMap();
  

}