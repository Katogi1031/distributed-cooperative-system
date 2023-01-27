#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static int map[16][16] = {{-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10},
                          {-10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10}
};


// Preyを見つけたかどうか
int findPrey = 0;
// Preyの4方向にPredatorがいるか
int top = 0;
int down = 0;
int left = 0;
int right = 0;

struct predatorNode{
  struct predatorPoint* pnt;
  struct predatorNode* parent;
  int g;
  int h;
};

// 座標の構造体
struct predatorPoint{
  int x;
  int y;
};

void MapUpdate(int field[16][16]){
    struct predatorPoint *prey = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field[i][j] != -10){ // Predatorの視界範囲を共有マップに反映
                map[i][j] = field[i][j];
            }
            if(field[i][j] == 10){  // Preyが存在すれば
                findPrey = 1; 
                prey->y = i;
                prey->x = j;
            }
        }
    }
}

void Predator(int field1[16][16], int field2[16][16], int field3[16][16], int field4[16][16], int *point1, int *point2, int *point3, int *point4){
    // printf("success");


    // 各predatorの視界からマップを更新
    MapUpdate(field1);
    MapUpdate(field2);
    MapUpdate(field3);
    MapUpdate(field4);

    // predatorの行動戦略を取得
    Predator1(field1, point1);
    Predator2(field2, point2);
    Predator3(field3, point3);
    Predator4(field4, point4);
}


void Predator1(int* field, int* point){
    
    struct predatorPoint* pos = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(pos, 1);
    printf("%d, %d\n", pos->y, pos->x);
    *point = RightHandRule(pos->y, pos->x);
    free(pos); //使用後にfreeをする
}

void Predator2(int* field, int* point){
    
    struct predatorPoint* pos = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(pos, 2);
    printf("%d, %d\n", pos->y, pos->x);
    *point = RightHandRule(pos->y, pos->x);
    free(pos); //使用後にfreeをする
}

void Predator3(int* field, int* point){
    
    struct predatorPoint* pos = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(pos, 3);
    printf("%d, %d\n", pos->y, pos->x);
    *point = RightHandRule(pos->y, pos->x);
    free(pos); //使用後にfreeをする
}

void Predator4(int* field, int* point){
    
    struct predatorPoint* pos = (struct predatorPoint*)malloc(sizeof(struct predatorPoint));
    PredatorPosition(pos, 4);
    printf("%d, %d\n", pos->y, pos->x);
    *point = RightHandRule(pos->y, pos->x);
    free(pos); //使用後にfreeをする
    printf("%d\n", *point);
}

    
int RightHandRule(int posY, int posX){
    char array[4] = {'t', 'd', 'l', 'r'};
    // 右方向に行けるなら
    // その場所に障害物がない、かつ、フィールド内であること
    printf("%d %d\n", posY, posX);
    if(map[posY][posX + 1] == 0 && posX+1 < 16) return (int)array[3]; // 右に行けるなら
    else if(map[posY + 1][posX] == 0 && posY+1 < 16) return (int)array[1]; // 下に行けるなら
    else if(map[posY][posX - 1] == 0 && 0 <= posX-1) return (int)array[2]; // 左に行けるなら
    else if(map[posY - 1][posX] == 0 && 0 <= posY-1) return (int)array[0]; // 上に行けるなら
}

void PredatorPosition(struct predatorPoint* pos, int n){
    int i, j;
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            if(map[i][j] == n){
                pos->y = i; // 行
                pos->x = j; // 列
            }
        }
    }
}