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

// 16x16のマップを4x4のマップで表現
// 要素の値は未踏(mapで-10)の個数を表現
int quarterMap[4][4];


void MapUpdate(int field[16][16]){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field[i][j] != -10){
                map[i][j] = field[i][j];
            }
        }
    }
}

// 未踏の地が4x4のグリッドにいくつ存在するか
void UnreachableMap(){
    int i, j, p, q, cnt;
    for(i = 0; i < 16; i++){
        cnt = 0;
        for(j = 0; j < 16; j++){
            p = i / 4;
            q = j / 4;
            if(map[i][j] == -10) quarterMap[p][q]+=1;
            // quarterMap[p][q] = cnt;
        }
        
        
    }
}


void Predator(int field1[16][16], int field2[16][16], int field3[16][16], int field4[16][16], int *point1, int *point2, int *point3, int *point4){

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            quarterMap[i][j] = 0;
        }
    }

    // 各predatorの視界からマップを更新
    MapUpdate(field1);
    MapUpdate(field2);
    MapUpdate(field3);
    MapUpdate(field4);

    // 未踏の地を計算
    UnreachableMap();

    // mapの出力
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            printf("%4d", map[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%4d", quarterMap[i][j]);
        }
        printf("\n");
    }

    // predatorの行動戦略を取得
    Predator1(field1, point1);
    // Predator2(field2, point2);
    // Predator3(field3, point3);
    // Predator4(field4, point4);

}

void Predator1(int* field, int* point){
    
    // quarterMap内の自分がどのエリアに属するか求める
    int posX, posY;
    int i, j;

    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            if(map[i][j] == 1){
                posX = j;
                posY = i;
            }
        }
    }

    int quarterX =posX / 4;
    int quarterY = posY / 4;

    printf("%d, %d\n", quarterX, quarterY);
    

    // 最も近い未踏の地を計算

    *point = (int)('r');
    // printf("%d\n", *point);
}

// void Predator2(int* field, int* point){
//     *point = (int)("u");   
// }

// void Predator3(int* field, int* point){
//     *point = (int)("u");
// }

// void Predator4(int* field, int* point){
//     *point = (int)('d');    
// }
    

