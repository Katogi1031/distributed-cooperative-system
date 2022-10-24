/*
l1 : オープンリストの長さ
l2 : クローズドリストの長さ
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// static int maze[8][8] = {{  1,  0,  0,  0,  0,  0,  0,  0},
//                          {  0,  0, -1, -1, -1, -1,  0,  0},
//                          {  0,  0,  0,  0,  0, -1,  0,  0},
//                          {  0,  0, -1, -1, -1, -1,  0,  0},
//                          {  0,  0, -1,  0,  0,  0,  0,  0},
//                          {  0,  0, -1,  0,  0,  0,  0,  0},
//                          {  0,  0, -1, -1, -1, -1,  0,  0},
//                          {  0,  0,  0,  0,  0, 10,  0,  0}
//                         };

struct PredatorNode
{
  struct PredatorPoint* pnt;
  struct PredatorNode* parentPredatorNode;
  float g;
  float h;       
};
struct PredatorPoint
{
  int x;
  int y;     
};
/* これって必要なのかな？ */
//------------------------------------------------------------------------------
//the function that operates during path finding process.
struct PredatorNode* predatorAStar(struct PredatorNode* current,struct PredatorNode* goal,struct PredatorNode **array,int l1,struct PredatorNode **closed,int l2);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//the function to find the neighboring PredatorNodes.to be called inside predatorAStar
//returns the number of elements inside the array...
//parameters:CURRENT PredatorNode,LIST OF PredatorNodeS TO BE INSPECTED,LENGTH OF THE LIST
int predatorExpandNode(struct PredatorNode* current,struct PredatorNode **array,int l1,struct PredatorNode **closed,int l2);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//finds the least total costed PredatorNode and returns a PredatorPointer for that PredatorNode...
//paramenters:LIST OF PredatorNodeS TO BE INSPECTED,LENGTH OF THE LIST
int predatorLeastCost(struct PredatorNode **array,int l1);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//calcualtes the total cost of each PredatorNode in the array...
//parameters: GOAL PredatorNode,LIST OF PredatorNodeS TO BE INSPECTED,LENGTH OF THE LIST
void predatorCalcTotalCost(struct PredatorNode* goalPredatorNode,struct PredatorNode **array,int l1);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//reconstructs the path from the beginning to the goal.prints out the path...
void predatorReconstructPath(struct PredatorNode* goalPredatorNode);
//------------------------------------------------------------------------------

/* predator関数からフィールド情報を受け取り、最短経路を求めて次の行動位置を送る */
void predatorAlgorithm(*ca){
  /* predatorとpreyの位置を格納する領域を確保*/
    int predator[2];
    int prey[2];
    int p, q;
    int size_1d = 64;
    int size_2d = 8;
    int field[8][8];

    /* caを１次元配列から二次元配列に変換 */
    for(int i = 0; i < size_1d; i++){
        p = i / size_1d;
        q = i % size_1d;

        field[p][q] = ca[i];
    }

    /* predatorとpreyの位置を特定*/
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            switch(field[i][j]){
                case 1:
                    
                    predator[0] = i;
                    predator[1] = j;
                case 10:
                    
                    prey[0] = i;
                    prey[1] = j;
            }
        }
    }

    //create START_PredatorNode=>sP and set the data...
    struct PredatorPoint* sP = (struct PredatorPoint*)malloc(sizeof(struct PredatorPoint));
    sP->x = predator[0],sP->y = predator[1];
    struct PredatorNode* startPredatorNode = (struct PredatorNode*)malloc(sizeof(struct PredatorNode));
    startPredatorNode->pnt = sP,startPredatorNode->parentPredatorNode = NULL,startPredatorNode->g=0;startPredatorNode->h=0;

    //create GOAL_PredatorNode=>gP and set the data...
    struct PredatorPoint* gP = (struct PredatorPoint*)malloc(sizeof(struct PredatorPoint));
    gP->x = prey[0],gP->y = prey[1];
    struct PredatorNode* goalPredatorNode = (struct PredatorNode*)malloc(sizeof(struct PredatorNode));
    goalPredatorNode->pnt = gP,goalPredatorNode->parentPredatorNode = NULL,goalPredatorNode->g=0,startPredatorNode->h=0;

    //create OPEN_LIST=>array as a PredatorPointer to a PredatorPointer that is PredatorPointing to an array...
    struct PredatorNode **array = (struct PredatorNode**)malloc(sizeof(struct PredatorNode*));
    *array = NULL;
    
    //create CLOSE_LIST=>closedArray as a PredatorPointer to a PredatorPointer that is PredatorPointing to an array...
    struct PredatorNode **closedArray = (struct PredatorNode**)malloc(sizeof(struct PredatorNode*));
    *closedArray = NULL;
    (*closedArray) = (struct PredatorNode*)realloc((*closedArray),sizeof(struct PredatorNode));
    memcpy(&((*closedArray)[0]),startPredatorNode,sizeof(struct PredatorNode));
    //create LAST_OBTAINED_PredatorNode=>finished and set it to the result returned from the recursive A_STAR_ALGORITHM_FUNC
    struct PredatorNode* finished = predatorAStar(startPredatorNode,goalPredatorNode,array,0,closedArray,1);

    //call RECONSTRUCT_FUNC=>predatorReconstructedPath with finished as parameter...
    predatorReconstructedPath(finished);
}


/* Predator : フィールド情報をA*アルゴリズムを実行する関数に送り、次の行動を受け取る */
void Predator(int *ca, int *action){
    /* caは実際１次元配列のため２次元に処理する(視覚的に見やすくするため)*/
    char act[] = {'u', 'd', 'l', 'r', 's'};

    predatorAlgorithm(ca);

    
    
    //wait for user input...
    getchar();

    //QUIT
    // return 0;   
}
struct PredatorNode* predatorAStar(struct PredatorNode* current,struct PredatorNode* goal,struct PredatorNode **array,int l1,struct PredatorNode **closed,int l2)
{
  int i,j,nextIndex;

  //call EXPAND_PredatorNode_FUNC predatorExpandNode and set LEN_OF_OPEN_LIST to the value returned from that function...
  l1 = predatorExpandNode(current,array,l1,closed,l2);

  //call CALCULATE_COST=>predatorCalcTotalCost function to set the G_FUNC=>g cost of each element in OPEN_LIST=>array...  
  predatorCalcTotalCost(goal,array,l1);

  //call FIND_LEAST_PredatorNode=>predatorLeastCost function to obtain the index of least costed PredatorNode in the OPEN_LIST=>array...
  nextIndex = predatorLeastCost(array,l1);

  struct PredatorNode *nextPredatorNode = &((*array)[nextIndex]);

  //create a new temporary list which is intended to be the next OPEN_LIST=>array
  //without the NEXT_PredatorNode=>nextPredatorNode inside.
  struct PredatorNode *tempArray = (struct PredatorNode*)calloc(l1-1,sizeof(struct PredatorNode));
  for(i = 0,j=0;i<l1;i++)
    if(i != nextIndex) memcpy(&tempArray[j],&((*array)[i]),sizeof(struct PredatorNode)),j++;
  l1--,l2++;
  *array = tempArray;

  //declare NEXT_PredatorNode=>nexPredatorNode to be visited by adding the PredatorNode to the CLOSE_LIST=>closed...
  *closed = (struct PredatorNode*)realloc((*closed),l2*(sizeof(struct PredatorNode)));
  memcpy(&((*closed)[l2-1]),nextPredatorNode,sizeof(struct PredatorNode));

  //check whether the NEXT_PredatorNode=>nextPredatorNode is the GOAL_PredatorNode=>goal by comparing its coordinates... 
  //if so => return NEXT_PredatorNode=>nextPredatorNode
  //else call A_STAR_ALGORITHM_FUNC0>predatorAStar with CURRENT_PredatorNode=>nextPredatorNode,
  //GOAL_PredatorNode=>goal, OPEN_LIST=>array, LEN_OPEN_LIST=>l1,CLOSE_LIST=>closed,LeN_CLOSE_LIST=>l2... 
  if(nextPredatorNode->pnt->x == goal->pnt->x && nextPredatorNode->pnt->y == goal->pnt->y)  return nextPredatorNode;
  return predatorAStar(nextPredatorNode,goal,array,l1,closed,l2);
}
int predatorExpandNode(struct PredatorNode* current,struct PredatorNode **array,int l1,struct PredatorNode **closed,int l2)
{   
    // printf("%d\n", l1);
   int i,j,count,found;
   count = 0;
   //--FIRST-PHASE--:EXPAND ALL THE NEIGHBORING PredatorNodeS...
   struct PredatorNode* tempArray;
   tempArray = (struct PredatorNode*)calloc(4,sizeof(struct PredatorNode));   
   for(i = 0;i<4;i++){
      tempArray[i].pnt = (struct PredatorPoint*)malloc(sizeof(struct PredatorPoint));
      if(i == 0 && field[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempArray[i].pnt->x = current->pnt->x-1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 3 && field[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempArray[i].pnt->x = current->pnt->x+1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 1 && field[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y+1,tempArray[i].g = current->g+1.0f;
      else if(i == 2 && field[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y-1,tempArray[i].g = current->g+1.0f;
    
      tempArray[i].parentPredatorNode = current;
   }

   //--SECOND-PHASE--:check whether current PredatorNode's children are inside the array.
   //if so, do not include those PredatorNodes to the expanded array. 
   for(j = 0;j<4;j++){
      found = 0;           
      if(tempArray[j].parentPredatorNode == NULL){
        return 0;
      }
      for(i = 0;i<l1;i++) if((*array)[i].pnt->x == tempArray[j].pnt->x && (*array)[i].pnt->y == tempArray[j].pnt->y) found++;
      for(i = 0;i<l2;i++) if(tempArray[j].pnt->x == (*closed)[i].pnt->x && (*closed)[i].pnt->y == tempArray[j].pnt->y) found++; 
      //--THIRD-PHASE--:expand the array with the additional neighbors.
      if(found == 0)
      {
        count++;
        int total = (l1+count);
        *array = (struct PredatorNode*)realloc((*array),total*(sizeof(struct PredatorNode)));
        memcpy(&((*array)[total-1]),&tempArray[j],sizeof(struct PredatorNode));              
      }      
   } 
   return count+l1;     
}
void predatorCalcTotalCost(struct PredatorNode* goalPredatorNode,struct PredatorNode **array,int l1)
{
    int i,difx,dify;
    for(i = 0;i<l1;i++)
    {
        difx = (*array)[i].pnt->x - goalPredatorNode->pnt->x;
        dify = (*array)[i].pnt->y - goalPredatorNode->pnt->y;
        (*array)[i].h = ((float)sqrt(pow(difx,2) + pow(dify,2)));
    }
}
int predatorLeastCost(struct PredatorNode **array,int l1)
{
    
    int i,min,minIndex;
    min = (*array)[0].g + (*array)[0].h;
    minIndex = 0;
    for(i= 1;i<l1;i++)
    {
        if((*array)[i].g + (*array)[i].h < min) min = (*array)[i].g + (*array)[i].h,minIndex = i;
    }
    return minIndex;
}
void predatorReconstructedPath(struct PredatorNode* goalPredatorNode)
{
    int field1[8][8]= {{  1,  0,  0,  0,  0,  0,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0,  0,  0,  0, -1,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0, -1,  0,  0,  0,  0,  0},
                         {  0,  0, -1,  0,  0,  0,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0,  0,  0,  0, 10,  0,  0}
                        };
     struct PredatorNode* current = goalPredatorNode;
     struct PredatorPoint* ptr = NULL;
     int steps = 0,i;
     while(current->parentPredatorNode != NULL)
     {
          steps++;
          ptr = (struct PredatorPoint*)realloc(ptr,steps*sizeof(struct PredatorPoint));
          memcpy(&ptr[steps-1],current->pnt,sizeof(struct PredatorPoint));
          current = current->parentPredatorNode;                    
     }
     for(i=steps;i>=1;i--)
     {
        field1[ptr[i-1].x][ptr[i-1].y] = 20;
        printf("(%d,%d)",ptr[i-1].x,ptr[i-1].y);
        if(i>1)  printf("=>");  
     }
     printf("\n");
     for(int k = 0; k < 8; k++){
      for(int l = 0; l < 8; l++){
        printf("%3d ", field1[k][l]);
      }
      printf("\n");
     }
}