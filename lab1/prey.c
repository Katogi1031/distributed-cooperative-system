#include<stdio.h>
#include<stdlib.h>
void Prey(int *ca, int *action){
        int size_1d  = 64;
        int size_2d = 8;
        char s[] = {'u', 'd', 'l', 'r', 's'};
        char field[8][8];
        int p, q;
        int predator_x, predator_y; // predator position
        int prey_x, prey_y; // prey position
        int i, j;

        int r = rand() % 5;
        *action = (int)s[r];
        printf("PredatorAction=%ls\n", action);

        /* convert from 1D array to 2D array */
        for(int i = 0; i < size_1d; i++){
                p = i / size_2d;
                q = i % size_2d;

                field[p][q] = ca[i];
        }

        for(i = 0; i < size_2d; i++){
                for(j = 0; j < size_2d; j++){
                        switch(field[i][j]){
                                case -1:
                                        printf("#");
                                        break;
                                case 0:
                                        printf(".");
                                        break;
                                case 1:
                                        printf("@");
                                        predator_x = i;
                                        predator_y = j;
                                        break;
                                case 10:
                                        printf("*");
                                        prey_x = i;
                                        prey_y = j;
                                        break;
                                default:
                                        break;
                        }
                }
                printf("\n");
        }
}