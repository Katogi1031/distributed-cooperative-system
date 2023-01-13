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
int oneEighthMap[4];

// Preyを見つけたかどうか
int findPrey = 0;


void MapUpdate(int field[16][16]){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field[i][j] != -10){
                map[i][j] = field[i][j];
            }
            if(field[i][j] == 10) findPrey = 1; 
        }
    }
}

// 未踏の地が4x4のグリッドにいくつ存在するか
void UnreachableMap(){
    int i, j, p, q, cnt;
    for(i = 0; i < 16; i++){
        cnt = 0;
        for(j = 0; j < 16; j++){
            p = i / 8;
            q = j / 8;
            if(p == 0){
                if(q == 0){
                    if(map[i][j] == -10) oneEighthMap[0]+=1;
                }else if(q == 1){
                    if(map[i][j] == -10) oneEighthMap[1]+=1;
                }
            }else if(p == 1){
                if(q == 0){
                    if(map[i][j] == -10) oneEighthMap[2]+=1;
                }else  if(q == 1){
                    if(map[i][j] == -10) oneEighthMap[3]+=1;
                }
            }
        }
        
        
    }
}

void Predator(int field1[16][16], int field2[16][16], int field3[16][16], int field4[16][16], int *point1, int *point2, int *point3, int *point4){

    // oneEightthMapの初期化
    for(int i = 0; i < 4; i++)
       oneEighthMap[i] = 0;

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

    for(int i = 0; i < 4; i++)
        printf("%d ", oneEighthMap[i]);
    printf("\n");

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
    
    // 座標の取得
    for(i = 0; i < 16; i++){
        for(j = 0; j < 16; j++){
            if(map[i][j] == 1){
                posX = j;
                posY = i;
            }
        }
    }

    int max = 64, maxIndex;
    int goalX, goalY;

    // 最も未開の地が占めるところが多いところを求める
    for(i = 0; i < 4; i++){
        if(oneEighthMap[i] >= max){
            max = oneEighthMap[i];
            maxIndex = i;
        }
    }
    // printf("maxIndex: %d\n", maxIndex);

    if(maxIndex == 0){
        goalX = 0;
        goalY = 0;
    }else if(maxIndex == 1){
        goalX = 15;
        goalY = 0;
    }else if(maxIndex == 2){
        goalX = 0;
        goalY = 15;
    }else if(maxIndex == 3){
        goalX = 15;
        goalY = 15;
    }
    

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
    

