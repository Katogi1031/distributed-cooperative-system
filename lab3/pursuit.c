/*
 Author:       Ei-Ichi Osawa
 Created:      February 5, 2002
 Updated;      October 10, 2005
 Language:     C
 */


#define VERSION_1_2 1

/*
 * 必要なライブラリのロード
 * (stdio.hおよびstdlib.h)
 */
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifdef VERSION_1_2
#ifndef PTHREAD_H
#include <pthread.h>
#endif /* PTHREAD_H */
#endif /* VERSION_1_2 */

/* 最大ゲームステップ数 */

#ifndef MAX_GAME_STEP
#define MAX_GAME_STEP 100
#endif

/* predatorの数値表現 */
#ifndef VALUE_OF_PREDATOR
#define VALUE_OF_PREDATOR 1
#endif

/* preyの数値表現 */
#ifndef VALUE_OF_PREY
#define VALUE_OF_PREY 10
#endif

/* obstacleの数値表現 */
#ifndef VALUE_OF_OBSTACLE
#define VALUE_OF_OBSTACLE -1
#endif

/* free gridの数値表現 */
#ifndef VALUE_OF_FREE
#define VALUE_OF_FREE 0
#endif

/* 格子世界の大きさ */
#ifndef WORLD_SIZE
#define WORLD_SIZE 16
#endif
 
/* 視界のサイズ(デフォルト値) */
#ifndef VIEW_SIZE
#define VIEW_SIZE 2
#endif

/* VALID(妥当である) */
#ifndef VALID
#define VALID 1
#endif

/* INVALID(妥当でない) */
#ifndef INVALID
#define INVALID 0
#endif

/* 格子世界ファイル名の最大文字数 */
#ifndef WORLD_FILE_NAME_LENGTH
#define WORLD_FILE_NAME_LENGTH 256
#endif

/* 表示のウェイト(デフォルト値) */
#ifdef VERSION_1_2
#ifndef DISPLAY_WAIT
#define DISPLAY_WAIT 0
#endif
#else /* VERSION_1_2 */
#ifndef DISPLAY_WAIT
#define DISPLAY_WAIT 0
#endif
#endif /* VERSION_1_2 */
int viewSize;                               /* 視界のサイズ */
/*
 *PredatorおよびPreyの位置を表す構造体
 */
typedef struct{
  int x;
  int y;
} pursuit_position;


#ifdef VERSION_1_2

/* 
 * watch dog timer用の構体
 */
typedef struct{
  pthread_mutex_t mutex; /* 排他制御用 */
  int count;             /* watch dog timer */
  int state;             /* watch dog state */
}watchdog_t;

static watchdog_t wdtimer =
{
  PTHREAD_MUTEX_INITIALIZER,
  0,
  0
};

#define WATCHDOG_COUNT_INIT  1
#define WATCHDOG_INTERVAL 1000000 /* usec */

#define WATCHDOG_RUN           1 /* WD status */
#define WATCHDOG_STOP          0 /* WD status */

pthread_t wd_tid;     /* Watch dog スレッドのthread ID */
pthread_t player_tid; /* Prey/Predator スレッドのthread ID */

#endif /* VERSION_1_2 */


/*
 * 初期状態データファイルの読み込み
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * char *fileName: 格子世界ファイル名が格納された文字列
 * 返り値:正常終了なら0,異常終了なら1
 */
int ReadInitConf(int battleFieldArray[WORLD_SIZE][WORLD_SIZE], char *fileName){
  FILE *fp;
  int i, j;

  fp = fopen(fileName, "r");
  if (fp == NULL){
    /* ファイルオープンエラー */
    return(1);
  }
  else{
    for (i = 0; i < WORLD_SIZE; i++){
      for (j = 0; j < WORLD_SIZE; j++){

        if (EOF == fscanf(fp,"%d", &battleFieldArray[i][j])){
          /* ファイルの長さが想定していたよりも短いエラー */
          return(1);
        }

      }
    }

    fclose(fp);
    return(0);
  }
}

/*
 * 格子世界を表示する関数
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * int clockNum: 現在のクロック数
 * 返り値:正常終了なら0,異常終了なら1
 */
int PrintWorld(int battleFieldArray[WORLD_SIZE][WORLD_SIZE], int clockNum){
  int i, j;

  printf("clock %d\n", clockNum);
  for (i = 0; i < WORLD_SIZE; i++){
    for (j = 0; j < WORLD_SIZE; j++){

      switch(battleFieldArray[i][j]){
      case -10:
        printf ("?");
        break;
      case VALUE_OF_OBSTACLE: /* 障害物 */
        printf ("#");
        break;
      case VALUE_OF_FREE: /* 自由格子 */
        printf(".");
        break;
      case VALUE_OF_PREDATOR: /* Predator1 */
        printf("@");
        break;
      case VALUE_OF_PREDATOR+1: /* Predator2 */
        printf("@");
        break;
      case VALUE_OF_PREDATOR+2: /* Predator3 */
        printf("@");
        break;
      case VALUE_OF_PREDATOR+3: /* Predator4 */
        printf("@");
        break;
      case VALUE_OF_PREY: /* Prey */
        printf("*");
        break;
      default: /* ここには本来到達しないはず */
        fprintf(stderr, "%d\n",battleFieldArray[i][j]);
        fprintf(stderr, "Execution Error!! \n");
        return(1);
        break;
      }
    }
    printf("\n");
  }
  printf("\n");
  return(0);
}

int PrintWorld2(int battleFieldArray[WORLD_SIZE][WORLD_SIZE], int clockNum){
  int i, j;

  printf("clock %d\n", clockNum);
  for (i = 0; i < WORLD_SIZE; i++){
    for (j = 0; j < WORLD_SIZE; j++){
      printf("%2d", battleFieldArray[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  return(0);
}

/*
 * Predatorの位置座標を取得する
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * 返り値:Predatorの位置座標を表すpursuit_position構造体
 */
pursuit_position getPredatorPosition
(int battleFieldArray[WORLD_SIZE][WORLD_SIZE], int num){
  
  int i,j;
  pursuit_position returnPosition;

  /*
   * battleFieldArrayのなかで、値がVALUE_OF_PREDATORになっているところを
   * 探して、その座標をreturnPositionに代入してreturnする
   */
  for (i=0; i<WORLD_SIZE; i++){
    for (j=0; j<WORLD_SIZE; j++){
      if (battleFieldArray[i][j] == VALUE_OF_PREDATOR+num){
        returnPosition.x=j;
        returnPosition.y=i;
        return returnPosition;
      }
    }
  }

  /*
   * ここまで処理が来るということは異常事態なので、positionとして
   * (-1,-1)を返す
   */
  returnPosition.x=-1;
  returnPosition.y=-1;
  return returnPosition;
}

/*
 * Preyの位置座標を取得する
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * 返り値:Preyの位置座標を表すpursuit_position構造体
 */
pursuit_position getPreyPosition
(int battleFieldArray[WORLD_SIZE][WORLD_SIZE]){
  
  int i,j;
  pursuit_position returnPosition;
  /*
   * battleFieldArrayのなかで、値がVALUE_OF_PREYになっているところを
   * 探して、その座標をreturnPositionに代入してreturnする
   */
  for (i=0; i<WORLD_SIZE; i++){
    for (j=0; j<WORLD_SIZE; j++){
      if (battleFieldArray[i][j] == VALUE_OF_PREY){
        returnPosition.x=j;
        returnPosition.y=i;
        return returnPosition;
      }
    }
  }

  /*
   * ここまで処理が来るということは異常事態なので、positionとして
   * (-1,-1)を返す
   */
  returnPosition.x=-1;
  returnPosition.y=-1;
  return returnPosition;

}

/*
 * 指定されたactionに従って、座標を更新する
 * pursuit_position currentPosition: 
 * PredatorまたはPreyの位置座標を表すpursuit_position構造体
 * int action: 次の行動を表す文字(をint型で表したもの)
 * 'u','d','l','r','s'のどれか
 * 返り値:更新された位置座標を表すposition構造体
 */

pursuit_position movePP(pursuit_position currentPosition, int action){
  switch(action){
  case 'u': /* 上に移動 */
    currentPosition.y=currentPosition.y-1;
    break;
  case 'd': /* 下に移動 */
    currentPosition.y=currentPosition.y+1;
    break;
  case 'l': /* 左に移動 */
    currentPosition.x=currentPosition.x-1;
    break;
  case 'r': /* 右に移動 */
    currentPosition.x=currentPosition.x+1;
    break;
  case 's': /* その場に留まる */
    break;
  default:  /* 指定外の文字を指定した場合は's'と等価とみなす */
    break;
  }

  return (currentPosition);
}

/*
 * 移動した座標の妥当性を検討する
 * pursuit_position currentPosition: 
 * PredatorまたはPreyの位置座標を表すpursuit_position構造体
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * 返り値: 妥当ならばVALID(=1),そうでなければINVALID(=0)
 */
int evalPosition(pursuit_position currentPosition, 
                 int battleFieldArray[WORLD_SIZE][WORLD_SIZE],
                 int number){
  /* 
   * ここで調べているのは,
   *   1. 指定された位置が、WORLD_SIZE*WORLD_SIZEの格子世界からはみ出して
   *      いないかどうか
   *   2. 指定された位置に障害物が存在していないかどうか
   */

  if (currentPosition.x >=0 && currentPosition.x < WORLD_SIZE
      && currentPosition.y >= 0 && currentPosition.y < WORLD_SIZE
      && (battleFieldArray[currentPosition.y][currentPosition.x] == 0 ||
          battleFieldArray[currentPosition.y][currentPosition.x] == number)){
    return VALID;
  }
  else{
    return INVALID;
  }

}

/*
 * 格子世界を更新する
 * int battleFieldArray[WORLD_SIZE][WORLD_SIZE]
 *      : 格子世界を表すWORLD_SIZE*WORLD_SIZEの整数型2次元配列
 * int predatorAction: Predatorの行動を表す文字(をintで表したもの)
 * ('u','d','l','r','s'のどれか)
 * int preyAction: preyの行動を表す文字(をintで表したもの)
 * ('u','d','l','r','s'のどれか)
 * 返り値: 格子世界更新の結果Preyが捕獲されれば1、そうでなければ0
 */
int RenewWorld(int battleFieldArray[WORLD_SIZE][WORLD_SIZE],
               int predatorAction[4],
               int preyAction){
  int i,j;
  int OK = 0;
  
/* PredatorとPreyの位置座標を表すposition構造体 */
  pursuit_position predatorPosition[4], preyPosition;
  pursuit_position newPredatorPosition[4], newPreyPosition;

  int copy[WORLD_SIZE][WORLD_SIZE];
  for (i = 0; i < WORLD_SIZE; i++) {
    for (j = 0; j < WORLD_SIZE; j++) {
      copy[i][j] = battleFieldArray[i][j];
    }  
  }
  
  while (!OK) {
    OK = 1;
    for (i = 0; i < WORLD_SIZE; i++) {
      for (j = 0; j < WORLD_SIZE; j++) {
         battleFieldArray[i][j] = copy[i][j];
      }  
    }

    /* PredatorとPreyの現在位置を得る */
    for (i = 0; i < 4; i++) {
      predatorPosition[i] = getPredatorPosition(battleFieldArray, i);
    }
    preyPosition = getPreyPosition(battleFieldArray);

    /* 移動後のPredatorとPreyの座標を得る */
    for (i = 0; i < 4; i++) {
      newPredatorPosition[i] = movePP(predatorPosition[i], predatorAction[i]);
    }
    newPreyPosition = movePP(preyPosition, preyAction);

    /* 確認用の出力
     *fprintf(stderr, "prey (%d,%d) -> (%d,%d)\n", preyPosition.x, preyPosition.y,
     *    newPreyPosition.x, newPreyPosition.y);
     *
     *    for (i = 0; i < 4; i++) {
     *      fprintf(stderr, "predator[%d] (%d,%d) -> (%d,%d)\n", i,
     *      predatorPosition[i].x, predatorPosition[i].y,
     *      newPredatorPosition[i].x,newPredatorPosition[i].y);
     *}
     */

    /* 2004/01/13 
     * すりぬけ防止用に改変 ここから
     */

    for (i = 0; i < 4; i++) {
      if (predatorAction[i] != 's') {

        /* すりぬけようとしている場合 */
        if (preyAction != 's' && 
            preyPosition.x == newPredatorPosition[i].x &&
            preyPosition.y == newPredatorPosition[i].y &&
            predatorPosition[i].x == newPreyPosition.x &&
            predatorPosition[i].y == newPreyPosition.y) {

          /* 両者その場にStay */
          
          //printf("********Stay *prey @%d\n",i); 

          predatorAction[i] = 's';
          newPredatorPosition[i].x = predatorPosition[i].x;
          newPredatorPosition[i].y = predatorPosition[i].y;
          preyAction = 's';
          newPreyPosition.x = preyPosition.x;
          newPreyPosition.y = preyPosition.y;
        }

        for (j = i+1; j < 4; j++) {
          /* すりぬけようとしている場合 */
          if (predatorAction[j] != 's' &&
              predatorPosition[i].x == newPredatorPosition[j].x &&
              predatorPosition[i].y == newPredatorPosition[j].y &&
              predatorPosition[j].x == newPredatorPosition[i].x &&
              predatorPosition[j].y == newPredatorPosition[i].y) {
            
            /* 両者その場にStay */
            ///*
            //  fprintf(stderr, "#####Stay @%d @%d\n", j,i); //*/
            
            predatorAction[i] = 's';
            newPredatorPosition[i].x = predatorPosition[i].x;
            newPredatorPosition[i].y = predatorPosition[i].y;
            predatorAction[j] = 's';
            newPredatorPosition[j].x = predatorPosition[j].x;
            newPredatorPosition[j].y = predatorPosition[j].y; 
          }
        }
      }
    }
    /* 2004/01/13 
     * すりぬけ防止用に改変 ここまで
     */
    
    /* battleFieldArrayから一旦PredatorとPreyを削除 */
    if (preyAction != 's') {
      battleFieldArray[preyPosition.y][preyPosition.x] = 0;
    }
    for (i = 0; i < 4; i++) {
      if (predatorAction[i] != 's') {
        battleFieldArray[predatorPosition[i].y][predatorPosition[i].x] = 0;
      }
    }
    
    for (i = 0; i < 4; i++) {
      if (predatorAction[i] != 's') {
        /* もし移動後の座標値が妥当ならば */
        if (evalPosition(newPredatorPosition[i], battleFieldArray,
                         VALUE_OF_PREDATOR+i) == VALID){
          /* 新しい座標をpredatorPositionにコピー */
          predatorPosition[i].x = newPredatorPosition[i].x;
          predatorPosition[i].y = newPredatorPosition[i].y;
          battleFieldArray[predatorPosition[i].y][predatorPosition[i].x] 
            = VALUE_OF_PREDATOR + i;
        }
        else {
          //      fprintf(stderr,"#####INVALID %d\n", i);
          predatorAction[i] = 's';
          OK = 0;
          continue;
        }
      }
    }

    /* もし移動後の座標値が妥当ならば */
    if (evalPosition(newPreyPosition,battleFieldArray,VALUE_OF_PREY) == VALID){
      /* 新しい座標をpreyPositionにコピー */    
      preyPosition.x = newPreyPosition.x;
      preyPosition.y = newPreyPosition.y;
    }
    else {
      //      fprintf(stderr,"####INVALID prey\n");
      preyAction = 's';
      OK = 0;
      continue;
    }
    battleFieldArray[preyPosition.y][preyPosition.x] = VALUE_OF_PREY;
  }

  /* 終了判定 */

  if (0 < preyPosition.x && 
      battleFieldArray[preyPosition.y][preyPosition.x-1] == 0) {
    return(0);
  }
  else if (preyPosition.x < WORLD_SIZE-1 && 
           battleFieldArray[preyPosition.y][preyPosition.x+1] == 0) {
    return (0);
  }
  else if (0 < preyPosition.y && 
           battleFieldArray[preyPosition.y-1][preyPosition.x] == 0) {
    return (0);
  }
  else if (preyPosition.y < WORLD_SIZE-1 && 
           battleFieldArray[preyPosition.y+1][preyPosition.x] == 0) {
    return (0);
  }
  else{
    return(1);
  }

}

/*
 * 格子世界のコピーを行う
 * int copy[WORLD_SIZE][WORLD_SIZE]: コピー先
 * int original[WORLD_SIZE][WORLD_SIZE]: コピー元
 * 返り値: 0
 */
int copyWorld(int copy[WORLD_SIZE][WORLD_SIZE], 
              int original[WORLD_SIZE][WORLD_SIZE]){
  int i,j;

  for (i=0; i<WORLD_SIZE; i++){
    for (j=0; j<WORLD_SIZE; j++){
      copy[i][j] = original[i][j];
    }
  }
  return(1);
}

#ifdef VERSION_1_2 
static int copy_array[WORLD_SIZE][WORLD_SIZE];     /* 格子世界(コピー) */

/*
 * Watch dog timer構造体の初期設定(スタート)
 */
static void setWatchdogTimer(){
  int status;

  /* 排他制御始まり */
  status=pthread_mutex_lock(&(wdtimer.mutex));
  if(status != 0){
    fprintf(stderr,"mutex lock fail!!\n");
    abort();
  }
  
  /* 本体 */
  wdtimer.count = WATCHDOG_COUNT_INIT;
  wdtimer.state = WATCHDOG_RUN; /* 時間の監視開始 */

  /* 排他制御終わり */
  status=pthread_mutex_unlock(&(wdtimer.mutex));
  if(status != 0){
    fprintf(stderr,"mutex unlock fail!!\n");
    abort();
  }
}

/*
 * Watch dog timerの解除
 * 1秒のWait用にも使用
 */
static void resetWatchdogTimer(){
  int status;

  /* 排他制御始まり */
  status=pthread_mutex_lock(&(wdtimer.mutex));
  if(status != 0){
    fprintf(stderr,"mutex lock fail!!\n");
    abort();
  }
  
  /* 本体 */
  wdtimer.state = WATCHDOG_STOP; /* 時間の監視終了 */
  
  /* 排他制御終わり */
  status=pthread_mutex_unlock(&(wdtimer.mutex));
  if(status != 0){
    fprintf(stderr,"mutex unlock fail!!\n");
    abort();
  }
}

/*
 * Watchdog timerスレッドの本体（関数)
 * void *arg:Prey/Predatorスレッドのthread IDへのポインタ
 */
static void *threadWatchdogTimer(void *arg){
  int status;
  pthread_t *tid = (pthread_t *) arg;
  int wdexit=0;

  setWatchdogTimer(); /* watchdog_t構造体の初期化 */

  /* 排他制御始まり */
  while(1){
    status=pthread_mutex_lock(&(wdtimer.mutex));
    if(status != 0){
      fprintf(stderr,"mutex lock fail!!\n");
      abort();
    }
  
    /* 本体 */
    if(wdtimer.count == 0){
      if(wdtimer.state == WATCHDOG_RUN){
        /* 時間内に終わらないPrey/Predatorスレッドをキャンセルする */
        status = pthread_cancel(*tid); 
      }
      /* ここは一時コメントアウト
      if(status != 0){
        fprintf(stderr,"pthread_cancel fail!!\n");
        abort();
      }
      */
      wdexit = 1; /* watchdog threadを終了するためのフラグ */
    }
    else {
      /* watch dog timerのカウンタを1引く */
      if(wdtimer.count>0){
        wdtimer.count--;
      }
      else{
        wdexit = 1; /* watchdog threadを終了するためのフラグ */
      }
    }

    /* 排他制御終わり */
    status=pthread_mutex_unlock(&(wdtimer.mutex));
    if(status != 0){
      fprintf(stderr,"mutex unlock fail!!\n");
      abort();
    }
    if(wdexit)
      { 
        /* watchdog threadを終了する */
        pthread_exit(0);
      }
    usleep(WATCHDOG_INTERVAL);
  }
}

/*
 * Prey スレッドの本体（関数)
 * void *arg:格子空間へのポインタ（int *)
 */
static void *threadPrey(int arg[WORLD_SIZE][WORLD_SIZE])
{
  static int Action;
  int cancel_type;
  int status;
  int i,x,y;
  int tWorld[WORLD_SIZE][WORLD_SIZE];
  pursuit_position P;

  /* 非同期キャンセルを可能にする */
  status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,
                                 &cancel_type);
  if(status != 0){
    fprintf(stderr,"pthread_setcanceltype fail!!\n");
    abort();
  }

  P = getPreyPosition(arg);
  for (y = 0; y < WORLD_SIZE; y++) {
    for (x = 0; x < WORLD_SIZE; x++) {
      if (y < P.y - viewSize || P.y + viewSize < y ||
          x < P.x - viewSize || P.x + viewSize < x) {
          tWorld[y][x] = -10;
      }
      else {
        tWorld[y][x] = arg[y][x];
      }
    }
  }
  
  Prey(tWorld, &Action);
  resetWatchdogTimer();  /* watchdog timerを終了する */
  pthread_exit(&Action); /* thread終了時の戻り値として次の動作を返す */
}

/*
 * Predator スレッドの本体（関数)
 * void *arg:格子空間へのポインタ（int *)
 */

static void *threadPredator(void* arg)
{
  int cancel_type;
  int status;
  static int Action[4];
  int i,x,y;
  int tWorld[5][WORLD_SIZE][WORLD_SIZE];
  pursuit_position P[4];

  for (i = 0; i < 4; i++)
    Action[i] = 0;

  copyWorld(tWorld[4], arg);

  /* 非同期キャンセルを可能にする */
  status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,
                                 &cancel_type);
  if(status != 0){
    fprintf(stderr,"pthread_setcanceltype fail!!\n");
    abort();
  }

  for (i = 0; i < 4; i++) {
    P[i] = getPredatorPosition(tWorld[4], i);
    for (y = 0; y < WORLD_SIZE; y++) {
      for (x = 0; x < WORLD_SIZE; x++) {
        if (y < P[i].y - viewSize || P[i].y + viewSize < y ||
            x < P[i].x - viewSize || P[i].x + viewSize < x) {
          tWorld[i][y][x] = -10;
        }
        else {
          tWorld[i][y][x] = tWorld[4][y][x];
        }
      }
    }
  }

//  PrintWorld(tWorld[3], 0);

  Predator(tWorld[0], tWorld[1], tWorld[2], tWorld[3], &Action[0], &Action[1], &Action[2], &Action[3]);
  resetWatchdogTimer();  /* watchdog timerを終了する */
  pthread_exit(Action); /* thread終了時の戻り値として次の動作を返す */
}

PrintDistance(int battleFieldArray[WORLD_SIZE][WORLD_SIZE]) {
  int i,j;
  /* PredatorとPreyの位置座標を表すposition構造体 */
  pursuit_position predatorPosition[4], preyPosition;
  int copy[WORLD_SIZE+2][WORLD_SIZE+2];
  int flag,d,sum;

    for (j = 0; j < WORLD_SIZE+2; j++) {
      for (i = 0; i < WORLD_SIZE+2; i++) {
      if (i == 0 || j == 0 || i == WORLD_SIZE+1 || j == WORLD_SIZE+1) {
        copy[j][i] = -1;
      }
      else {
        copy[j][i] = battleFieldArray[j-1][i-1];
      }
    }
  }

  for (i = 0; i < 4; i++) {
    predatorPosition[i] = getPredatorPosition(battleFieldArray, i);
    copy[predatorPosition[i].y+1][predatorPosition[i].x+1] = 0;
  }
  preyPosition = getPreyPosition(battleFieldArray);
  copy[preyPosition.y+1][preyPosition.x+1] = 1;

  flag = 0;
  for (d = 1; flag == 0; d++) {
    flag = 1;
    for (j = 1; j < WORLD_SIZE+1; j++) {
      for (i = 1; i < WORLD_SIZE+1; i++) {
        if (copy[j][i] == d) {
          if (copy[j-1][i] == 0) {
            copy[j-1][i] = d+1;
            flag = 0;
          }
          if (copy[j+1][i] == 0) {
            copy[j+1][i] = d+1;
            flag = 0;
          }
          if (copy[j][i-1] == 0) {
            copy[j][i-1] = d+1;
            flag = 0;
          }
          if (copy[j][i+1] == 0) {
            copy[j][i+1] = d+1;
            flag = 0;
          }
        }
      }
    }
  }

  sum = 0;
  for (i = 0; i < 4; i++) {
    sum += copy[predatorPosition[i].y+1][predatorPosition[i].x+1]-1;
    printf("predator%d:%3d\n", i+1, copy[predatorPosition[i].y+1][predatorPosition[i].x+1]-1);
  }
  printf("Total Distance:%3d\n", sum);
}


#endif /* VERSION_1_2 */
/*
 * Pursuit Gameのmain関数
 * 返り値: 正常終了なら0,異常終了なら1
 */

int main(int argc, char *argv[]){
  int i;
  int array[WORLD_SIZE][WORLD_SIZE];          /* 格子世界(オリジナル) */

#ifndef VERSION_1_2
  int copy_array[WORLD_SIZE][WORLD_SIZE];     /* 格子世界(コピー) */
#endif /* VERSION_1_2 */
  int gameStepNum;                            /* ゲームのステップ数 */
  int predatorAction[4], preyAction;             /* PredatorやPreyの動きを表す */

#ifdef VERSION_1_2
  int status;                                 /* pthread_*関数の戻り値用 */
  void *tret;                                 /* Prey/Predatorスレッド終了時の戻り値用 */
#endif /* VERSION_1_2 */

  /* 以下はオプション指定により変更される可能性のある変数 */
  int displayWait;                            /* 表示更新間隔(マイクロ秒) */
  char worldFileName[WORLD_FILE_NAME_LENGTH]; /* 格子世界ファイル名 */

  /* 変数のデフォルト値を設定する */
  viewSize = VIEW_SIZE;
  displayWait = DISPLAY_WAIT;
  strcpy(worldFileName, "battlefield.dat");

  /* 標準入力からのパラメータ処理 */
  for (i=1; i<argc; i++){
    /* "-v"(視界のサイズ指定)オプションの処理 */
    if (strcmp(argv[i], "-v")==0){              /* "-v"があったら */
      if (i+1 < argc){                          /* その次に入力があるか? */
        if (atoi(argv[i+1]) > 0){               /* 入力された値は妥当? */
          viewSize = atoi(argv[i+1]);    /* 全てOKのときの処理 */
          i++;
        }
        else{
          printf("Invalid input value!! Specify one positive number.\n");
          /* 値が妥当でない場合 */
          return(1);
        }
      }
      else{
        /* オプションの次に入力がない場合 */
        printf("Option error!! You must specify number of view size.\n");
        return(1);
      }
    }
    /* "-t"(表示間隔指定)オプションの処理 */
    else if(strcmp(argv[i], "-t")==0){         /* "-t"があったら */
      if (i+1 < argc){                         /* その次に入力があるか? */
        if (atoi(argv[i+1]) >= 0){              /* 入力された値は妥当か? */
          displayWait = atoi(argv[i+1]);       /* 全てOKのときの処理 */
          i++;
        }
        else{
          printf("Invalid input value!! Specify one positive number.\n");
          /* 値が妥当でない場合 */
          return(1);
        }
      }
      else{
        /* オプションの次に入力がない場合 */
        printf("Option error!! you must specify interval time of redraw.\n");
        return(1);
      }
    }
    /* "-w"(格子世界ファイル名指定)オプションの処理 */
    else if(strcmp(argv[i], "-w")==0){          /* "-w"があったら */
      if (i+1 < argc){                          /* その次に入力があるか? */
        /* 全てOKのときの処理 */
        strncpy(worldFileName, argv[i+1], WORLD_FILE_NAME_LENGTH);
        i++;
      }
      else{
        /* オプションの次に入力がない場合 */
        printf("Option error!! You must specify world file name.\n");
        return(1);
      }
    }
    else {
      printf("Invalid option: available options are:\n");
      printf("-s: Specify number of game step. ex: \"-s 10\"\n");
      printf("-t: Specify interval  time of redraw. ex: \"-t 100000\"\n");
      printf("-w: Specify world file name. ex: \"-w battlefield2.dat\"\n");
      return(1);
    }
  }

  /* BattleFieldの読み込み */
  if (ReadInitConf(array,worldFileName) == 1){
    fprintf(stderr,"File read failed!!\n");
    return(1);
  }

  /* 表示のクリア */
  system("clear");

  /* 初期状態の表示 */
  PrintWorld(array,0);

#ifdef VERSION_1_2
  /* 並列実行するスレッド数をセットする */
  pthread_setconcurrency(3);
#endif /* VERSION_1_2 */
  /* ゲームスタート */
  for (gameStepNum=1; gameStepNum<= MAX_GAME_STEP; gameStepNum++){
#ifdef VERSION_1_2 /* 時間監視バージョン */
    /* Predatorの行動 */
    copyWorld(copy_array,array);

    /* Predator threadを作る */
    status = pthread_create(&player_tid, NULL, threadPredator, (void *)copy_array);
    if(status != 0){
      fprintf(stderr,"pthread_create fail!!: %s\n", strerror(status));
      abort();
    }

    /* watch dog timer threadを作る */
    status = pthread_create(&wd_tid, NULL, 
                            threadWatchdogTimer, (void *)&player_tid);
    if(status != 0){
      fprintf(stderr,"pthread_create fail!!: %s\n", strerror(status));
      abort();
    }

    /* Predator threadの終了を待つ．終了戻り値はtret */
    status = pthread_join(player_tid, &tret);
    if(status != 0){
      fprintf(stderr,"pthread_join fail!!: %s\n", strerror(status));
      abort();
    }

    /* Predator threadがキャンセルされたか確認 */
    if(tret == (void *)PTHREAD_CANCELED){
      for (i = 0; i < 4; i++)
        predatorAction[i] = 's';
    }
    else{
      for (i = 0; i < 4; i++)
        predatorAction[i] = *((int *)tret+i);
    }

    /* watch dog threadの終了を待つ．終了戻り値はtret */
    status = pthread_join(wd_tid, &tret);
    if(status != 0){
      fprintf(stderr,"pthread_join fail!!: %s\n", strerror(status));
      abort();
    }


      copyWorld(copy_array,array);

      /* Prey threadを作る */
      status = pthread_create(&player_tid, NULL, threadPrey, (void *)copy_array);
      if(status != 0){
        fprintf(stderr,"pthread_create fail!!: %s\n", strerror(status));
        abort();
      }

      /* watch dog timer threadを作る */
      status = pthread_create(&wd_tid, NULL, threadWatchdogTimer, (void *)&player_tid);
      if(status != 0){
        fprintf(stderr,"pthread_create fail!!: %s\n", strerror(status));
        abort();
      }

      /* Prey threadの終了を待つ．終了戻り値はtret */
      status = pthread_join(player_tid, &tret);
      if(status != 0){
        fprintf(stderr,"pthread_join fail!!: %s\n", strerror(status));
        abort();
      }

      /* Prey threadがキャンセルされたか確認 */
      if(tret == (void *)PTHREAD_CANCELED){
        preyAction = 's';
      }
      else{
        preyAction = *((int *)tret);
      }

      /* watch dog threadの終了を待つ．終了戻り値はtret */
      status = pthread_join(wd_tid, &tret);
      if(status != 0){
        fprintf(stderr,"pthread_join fail!!: %s\n", strerror(status));
        abort();
        
      }

#else /* VERSION_1_2 */
    /* Predatorの行動 */
    copyWorld(copy_array,array);
    Predator(copy_array, &predatorAction);

    /* Preyの行動(2回動いて1回休む) */
    if (gameStepNum % 3 != 0){
      copyWorld(copy_array,array);
      Prey(copy_array,&preyAction);      
    }
    else{
      preyAction = 's';
    }
#endif /* VERSION_1_2 */

    /* 表示のウェイト */
    usleep(displayWait);

    /*
     * それぞれの行動に合わせて格子世界を更新
     * RenewWorldの帰り値が0ならば継続
     */
    if (RenewWorld(array,predatorAction, preyAction) == 0){
      system("clear");
      PrintWorld(array, gameStepNum);
    }
    /* そうでなければ(1ならば)終了 */
    else{
      system("clear");
      PrintWorld(array, gameStepNum);      
      printf("Game Over!! Winner Predator (%d clock)!!\n",gameStepNum);
      return(0);
    }
  }

  /* 2004/01/13 
   * 引き分けのときにそれぞれのpredatorの距離を出力する
   */
  PrintDistance(array); 
}
