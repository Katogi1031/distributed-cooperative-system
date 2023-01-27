#include <stdio.h>
#include <stdlib.h>

// 迷路の大きさ
#define N 5

// 探索するステップの最大値
#define MAX_STEP 15

// 迷路のマップ
int map[N][N] = {
    { 0, 1, 0, 0, 0 },
    { 0, 1, 0, 1, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 0 },
    { 0, 0, 0, 1, 0 }
};

// スタート地点
int startX = 0, startY = 0;

// ゴール地点
int goalX = 4, goalY = 4;

// 移動方向
int dx[4] = { 1, 0, -1, 0 }, dy[4] = { 0, 1, 0, -1 };

// 深さ制限
int depthLimit;

// 探索する
int idaStar(int x, int y, int depth, int cost) {
    // ゴールに到達したら終了
    if (x == goalX && y == goalY) {
        return cost;
    }

    // 深さ制限を超えたら終了
    if (depth == depthLimit) {
        return -1;
    }

    int minCost = MAX_STEP;
    for (int i = 0; i < 4; i++) {
        int nextX = x + dx[i], nextY = y + dy[i];
        // 移動先が壁ならスキップ
        if (map[nextX][nextY] == 1) {
            continue;
        }
        // 移動先に探索
        int cost = idaStar(nextX, nextY, depth + 1, cost + 1);
        if (cost == -1) {
            continue;
        }
        // 最小コストを更新
        minCost = cost < minCost ? cost : minCost;
    }

    return minCost;
}

int main() {
    for (depthLimit = 0; depthLimit < MAX_STEP; depthLimit++) {
        int cost = idaStar(startX, startY, 0, 0);
        if (cost != -1) {
            printf("Found a path with cost %d\n", cost);
        }
    }
    return 0;
}
