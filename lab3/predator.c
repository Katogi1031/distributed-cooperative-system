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

void MapUpdate(int field[16][16]){
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            if(field[i][j] != -10){
                map[i][j] = field[i][j];
            }
        }
    }
}


void Predator(int field1[16][16], int field2[16][16], int field3[16][16], int field4[16][16], int *point1, int *point2, int *point3, int *point4){

    Predator1(field1, point1);
    Predator2(field2, point2);
    Predator3(field3, point3);
    Predator4(field4, point4);

    // 各predatorの視界からマップを更新
    MapUpdate(field1);
    MapUpdate(field2);
    MapUpdate(field3);
    MapUpdate(field4);

    // mapの出力
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 16; j++){
            printf("%4d", map[i][j]);
        }
        printf("\n");
    }
}

void Predator1(int* field, int* point){
    *point = (int)('r');
    // printf("%d\n", *point);
}

void Predator2(int* field, int* point){
    *point = (int)("u");   
}

void Predator3(int* field, int* point){
    *point = (int)("u");
}

void Predator4(int* field, int* point){
    *point = (int)('d');    
}