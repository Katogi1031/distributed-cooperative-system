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

struct node
{
  struct point* pnt;
  struct node* parentNode;
  float g;
  float h;       
};
struct point
{
  int x;
  int y;     
};

//------------------------------------------------------------------------------
//the function that operates during path finding process.
struct node* AStarAlgorithm(struct node* current,struct node* goal,struct node **array,int l1,struct node **closed,
int l2);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//the function to find the neighboring nodes.to be called inside AStarAlgorithm
//returns the number of elements inside the array...
//parameters:CURRENT NODE,LIST OF NODES TO BE INSPECTED,LENGTH OF THE LIST
int ExpandNode(struct node* current,struct node **array,int l1,struct node **closed,int l2);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//finds the least total costed node and returns a pointer for that node...
//paramenters:LIST OF NODES TO BE INSPECTED,LENGTH OF THE LIST
int FindTheLeastCosted(struct node **array,int l1);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//calcualtes the total cost of each node in the array...
//parameters: GOAL NODE,LIST OF NODES TO BE INSPECTED,LENGTH OF THE LIST
void CalculateTheTotalCost(struct node* goalNode,struct node **array,int l1);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//reconstructs the path from the beginning to the goal.prints out the path...
void ReconstructThePath(struct node* goalNode);
//------------------------------------------------------------------------------
void predator(int *ca, int action){
    /* predatorとpreyの位置を格納する領域を確保*/
    int predator[2];
    int prey[2];

    /* predatorとpreyの位置を特定*/
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            switch(maze[i][j]){
                case 1:

                    predator[0] = i;
                    predator[1] = j;
                case 10:

                    prey[0] = i;
                    prey[1] = j;
            }
        }
    }

    //create START_NODE=>sP and set the data...
    struct point* sP = (struct point*)malloc(sizeof(struct point));
    sP->x = predator[0],sP->y = predator[1];
    struct node* startNode = (struct node*)malloc(sizeof(struct node));
    startNode->pnt = sP,startNode->parentNode = NULL,startNode->g=0;startNode->h=0;

    //create GOAL_NODE=>gP and set the data...
    struct point* gP = (struct point*)malloc(sizeof(struct point));
    gP->x = prey[0],gP->y = prey[1];
    struct node* goalNode = (struct node*)malloc(sizeof(struct node));
    goalNode->pnt = gP,goalNode->parentNode = NULL,goalNode->g=0,startNode->h=0;

    //create OPEN_LIST=>array as a pointer to a pointer that is pointing to an array...
    struct node **array = (struct node**)malloc(sizeof(struct node*));
    *array = NULL;

    //create CLOSE_LIST=>closedArray as a pointer to a pointer that is pointing to an array...
    struct node **closedArray = (struct node**)malloc(sizeof(struct node*));
    *closedArray = NULL;
    (*closedArray) = (struct node*)realloc((*closedArray),sizeof(struct node));
    memcpy(&((*closedArray)[0]),startNode,sizeof(struct node));
    //create LAST_OBTAINED_NODE=>finished and set it to the result returned from the recursive A_STAR_ALGORITHM_FUNC
    struct node* finished = AStarAlgorithm(startNode,goalNode,array,0,closedArray,1);

    //call RECONSTRUCT_FUNC=>ReconstructThePath with finished as parameter...
    ReconstructThePath(finished);

    //wait for user input...
    getchar();

    //QUIT
    return 0;   
}
struct node* AStarAlgorithm(struct node* current,struct node* goal,struct node **array,int l1,struct node **closed,int l2)
{
  int i,j,nextIndex;

  //call EXPAND_NODE_FUNC ExpandNode and set LEN_OF_OPEN_LIST to the value returned from that function...
  l1 = ExpandNode(current,array,l1,closed,l2);

  //call CALCULATE_COST=>CalculateTheTotalCost function to set the G_FUNC=>g cost of each element in OPEN_LIST=>array...  
  CalculateTheTotalCost(goal,array,l1);

  //call FIND_LEAST_NODE=>FindTheLeastCosted function to obtain the index of least costed node in the OPEN_LIST=>array...
  nextIndex = FindTheLeastCosted(array,l1);

  struct node *nextNode = &((*array)[nextIndex]);

  //create a new temporary list which is intended to be the next OPEN_LIST=>array
  //without the NEXT_NODE=>nextNode inside.
  struct node *tempArray = (struct node*)calloc(l1-1,sizeof(struct node));
  for(i = 0,j=0;i<l1;i++)
    if(i != nextIndex) memcpy(&tempArray[j],&((*array)[i]),sizeof(struct node)),j++;
  l1--,l2++;
  *array = tempArray;

  //declare NEXT_NODE=>nexNode to be visited by adding the node to the CLOSE_LIST=>closed...
  *closed = (struct node*)realloc((*closed),l2*(sizeof(struct node)));
  memcpy(&((*closed)[l2-1]),nextNode,sizeof(struct node));

  //check whether the NEXT_NODE=>nextNode is the GOAL_NODE=>goal by comparing its coordinates... 
  //if so => return NEXT_NODE=>nextNode
  //else call A_STAR_ALGORITHM_FUNC0>AStarAlgorithm with CURRENT_NODE=>nextNode,
  //GOAL_NODE=>goal, OPEN_LIST=>array, LEN_OPEN_LIST=>l1,CLOSE_LIST=>closed,LeN_CLOSE_LIST=>l2... 
  if(nextNode->pnt->x == goal->pnt->x && nextNode->pnt->y == goal->pnt->y)  return nextNode;
  return AStarAlgorithm(nextNode,goal,array,l1,closed,l2);
}
int ExpandNode(struct node* current,struct node **array,int l1,struct node **closed,int l2)
{   
    // printf("%d\n", l1);
   int i,j,count,found;
   count = 0;
   //--FIRST-PHASE--:EXPAND ALL THE NEIGHBORING NODES...
   struct node* tempArray;
   tempArray = (struct node*)calloc(4,sizeof(struct node));   
   for(i = 0;i<4;i++){
      tempArray[i].pnt = (struct point*)malloc(sizeof(struct point));
      if(i == 0 && maze[current->pnt->x-1][current->pnt->y] != -1)  // 左に障害物がないか
        tempArray[i].pnt->x = current->pnt->x-1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 3 && maze[current->pnt->x+1][current->pnt->y] != -1) // 右に障害物がないか
        tempArray[i].pnt->x = current->pnt->x+1,tempArray[i].pnt->y = current->pnt->y,tempArray[i].g = current->g+1.0f;
      else if(i == 1 && maze[current->pnt->x][current->pnt->y+1] != -1) // 下に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y+1,tempArray[i].g = current->g+1.0f;
      else if(i == 2 && maze[current->pnt->x][current->pnt->y-1] != -1) // 上に障害物がないか
        tempArray[i].pnt->x = current->pnt->x,tempArray[i].pnt->y = current->pnt->y-1,tempArray[i].g = current->g+1.0f;

      tempArray[i].parentNode = current;
   }

   //--SECOND-PHASE--:check whether current node's children are inside the array.
   //if so, do not include those nodes to the expanded array. 
   for(j = 0;j<4;j++){
      found = 0;           
      if(tempArray[j].parentNode == NULL){
        return 0;
      }
      for(i = 0;i<l1;i++) if((*array)[i].pnt->x == tempArray[j].pnt->x && (*array)[i].pnt->y == tempArray[j].pnt->y) found++;
      for(i = 0;i<l2;i++) if(tempArray[j].pnt->x == (*closed)[i].pnt->x && (*closed)[i].pnt->y == tempArray[j].pnt->y) found++; 
      //--THIRD-PHASE--:expand the array with the additional neighbors.
      if(found == 0)
      {
        count++;
        int total = (l1+count);
        *array = (struct node*)realloc((*array),total*(sizeof(struct node)));
        memcpy(&((*array)[total-1]),&tempArray[j],sizeof(struct node));              
      }      
   } 
   return count+l1;     
}
void CalculateTheTotalCost(struct node* goalNode,struct node **array,int l1)
{
    int i,difx,dify;
    for(i = 0;i<l1;i++)
    {
        difx = (*array)[i].pnt->x - goalNode->pnt->x;
        dify = (*array)[i].pnt->y - goalNode->pnt->y;
        (*array)[i].h = ((float)sqrt(pow(difx,2) + pow(dify,2)));
    }
}
int FindTheLeastCosted(struct node **array,int l1)
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
void ReconstructThePath(struct node* goalNode)
{
    int field[8][8]= {{  1,  0,  0,  0,  0,  0,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0,  0,  0,  0, -1,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0, -1,  0,  0,  0,  0,  0},
                         {  0,  0, -1,  0,  0,  0,  0,  0},
                         {  0,  0, -1, -1, -1, -1,  0,  0},
                         {  0,  0,  0,  0,  0, 10,  0,  0}
                        };
     struct node* current = goalNode;
     struct point* ptr = NULL;
     int steps = 0,i;
     while(current->parentNode != NULL)
     {
          steps++;
          ptr = (struct point*)realloc(ptr,steps*sizeof(struct point));
          memcpy(&ptr[steps-1],current->pnt,sizeof(struct point));
          current = current->parentNode;                    
     }
     for(i=steps;i>=1;i--)
     {
        field[ptr[i-1].x][ptr[i-1].y] = 20;
        printf("(%d,%d)",ptr[i-1].x,ptr[i-1].y);
        if(i>1)  printf("=>");  
     }
     printf("\n");
     for(int k = 0; k < 8; k++){
      for(int l = 0; l < 8; l++){
        printf("%3d ", field[k][l]);
      }
      printf("\n");
     }
}