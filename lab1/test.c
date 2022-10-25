#include <stdio.h>

int main(void) {
  FILE *fp;	          /* file pointer */
  int rstat, i;		  /* fscanf return status and loop parameter */
  int array[8][8];	       /* data array */
  int p, q;

  fp = fopen("battlefield.dat", "r"); /* open file to read */

  if (fp == NULL) {                    /* if fp is NULL, it means open file failed */
    printf("Failed file open.\n"); 
  } else {
    for(i = 0; i < 64; i++){
        p = i / 8;
        q = i % 8;
      rstat = fscanf(fp, "%d", &array[p][q]);
    }

    for(i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
      
    
  }   
  
  fclose(fp);

    

}