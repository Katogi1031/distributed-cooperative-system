#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ROW 5
#define COL 5

// Function to find the minimum number of steps needed to reach the destination
int minSteps(int grid[ROW][COL], int x, int y, int *pathX, int *pathY, int *length) {
    // If the destination is out of grid or it is not reachable
    if (x < 0 || y < 0 || x >= ROW || y >= COL || grid[x][y] == -1)
        return INT_MAX;

    // If the destination is already reached
    if (x == 0 && y == 0) {
        pathX[0] = x;
        pathY[0] = y;
        *length = 1;
        return grid[x][y];
    }

    // If the subproblem is already solved
    if (grid[x][y] != -2) {
        *length = grid[x][y+ROW];
        for (int i = 0; i < *length; i++) {
            pathX[i] = grid[x][i+COL];
            pathY[i] = grid[x][i+2*COL];
        }
        return grid[x][y];
    }

    int minStepsLeft = minSteps(grid, x - 1, y, pathX, pathY, length);
    int minStepsUp = minSteps(grid, x, y - 1, pathX, pathY, length);

    if (minStepsLeft < minStepsUp) {
        grid[x][y] = minStepsLeft + 1;
        grid[x][y+ROW] = *length;
        for (int i = 0; i < *length; i++) {
            grid[x][i+COL] = pathX[i];
            grid[x][i+2*COL] = pathY[i];
        }
        pathX[*length] = x;
        pathY[*length] = y;
        (*length)++;
    } else {
        grid[x][y] = minStepsUp + 1;
        grid[x][y+ROW] = *length;
        for (int i = 0; i < *length; i++) {
            grid[x][i+COL] = pathX[i];
            grid[x][i+2*COL] = pathY[i];
        }
        pathX[*length] = x;
        pathY[*length] = y;
        (*length)++;
    }

    return grid[x][y];
}

int main() {
    int grid[ROW][COL] = {
        { 1, -1, 2, -1, 3 },
        { 2, -1, 3, -1, 4 },
        { 3, -1, 4, -1, 5 },
        { 4, -1, 5, -1, 6 },
        { 5, -1, 6, -1, 7 }
        };
    int x, y;
    printf("Enter the destination x and y coordinates: ");
    scanf("%d %d", &x, &y);
    
    int pathX[ROW*COL], pathY[ROW*COL];
    int length;
    
    // Initializing all cells as not visited
    for (int i = 0; i < ROW; i++)
        for (int j = 0; j < COL; j++)
            if (grid[i][j] == -1)
                grid[i][j] = -2;
    
    int min = minSteps(grid, x, y, pathX, pathY, &length);
    printf("Minimum number of steps needed to reach the destination: %d\n", min);
    printf("Path to reach the destination: ");
    for (int i = 0; i < length; i++)
        printf("(%d, %d) ", pathX[i], pathY[i]);
    
    return 0;
}