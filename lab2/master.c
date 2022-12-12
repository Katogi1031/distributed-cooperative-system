/*
 * Master Program for Iterative Prisoner's Dilemma Game (for Solaris)
 * version 1.0
 *
 * Authour: Ei-Ichi Osawa
 * Date: November 1, 2002
 * Copyright (c) 2002, Ei-Ichi Osawa, FUN
 *
 */

#define VERSION_1_0 1

/*
 * Load libralies (stdio.h, stdlib.h, time.h)
 *
 */
#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef COOPERATION
#define COOPERATION 1
#endif

#ifndef DECEPTION
#define DECEPTION 0
#endif

/* ゲーム回数(ディフォルト値) */

#ifndef NUM_OF_GAME
#define NUM_OF_GAME 1000
#endif

/* エラーを決定する閾値の設定 */

#ifndef ERROR
#define ERROR 10 
#endif

/*
 * 各プレイヤの利得計算
 * 返り値: なし
 */

void payoff(int strtgy1, int strtgy2, int *payoff1, int *payoff2) {

  if (strtgy1 == COOPERATION)  {
    if (strtgy2 == COOPERATION) {
      *payoff1 = 3;
      *payoff2 = 3;
    } else if (strtgy2 == DECEPTION) {
      *payoff1 = 0;
      *payoff2 = 5;
    } else {
      *payoff1 = 5;
      *payoff2 = 0;
    }
  } else if (strtgy1 == DECEPTION) {
    if (strtgy2 == COOPERATION) {
      *payoff1 = 5;
      *payoff2 = 0;
    } else if (strtgy2 == DECEPTION) {
      *payoff1 = 1;
      *payoff2 = 1;
    } else {
      *payoff1 = 5;
      *payoff2 = 0;
    }
  } else {
    if ((strtgy2 == COOPERATION) ||
	(strtgy2 == DECEPTION)) {
      *payoff1 = 0;
      *payoff2 = 5;
    } else {
      *payoff1 = 0;
      *payoff2 = 0;
    }
  }
  return;
}

/*
 * Game Master の main 関数
 * 返り値: 正常なら 0, 以上なら -1
 */

int main(int argc, char *argv[]) {
  int i, random, error_count1 = 0, error_count2 = 0;
  time_t t;
  int score1 = 0, score2 = 0;  /*各プレイヤの総得点 */
  int payoff1, payoff2;   /*各プレイヤの各回のゲームの得点 */
  int gameStepNum;  /* ゲームの現在の回数 */
  int prevStrtgy1 = -1,  prevStrtgy2 = -1; /*各プレイヤの前回の戦略 */
  int currentStrtgy1, currentStrtgy2; /*各プレイヤの今回の戦略 */

  /* 以下はオプション指定により変更される可能性のある変数 */
  int gameStepNum_Max = NUM_OF_GAME; /* ゲームの最大ステップ数 */

  /*標準入力からのパラメータ処理 */
  for (i=1; i < argc; i++) {
    /* "-s" 対戦回数オプションの処理 */
    if(strcmp(argv[i], "-s")==0) {
      if (i+1 < argc) {
	gameStepNum_Max = atoi(argv[i+1]);
	if ((gameStepNum_Max > 0) &&  (gameStepNum_Max <= 10000)) {
	  i++;
	} else {
	  printf("Invalid input value!! Specify one positive number no more than 10000.\n");
	  /* 値が妥当でない場合*/
	  return(1);
	}
      } else {
	/* オプションの次に入力パラメータがない場合 */
	printf("Option error!! You must specify number of games.\n");
	return (1);
      }
    } else {
      printf("Invalid option:  available option is:\n");
      printf("-s: Specify number of games. ex: \"-s 200\"\n");
      return (1);
    }
  }

  /* 時間に依存した乱数のシードの設定 */
  srand((unsigned) time(&t));

   /* ゲームスタート */

   for (gameStepNum=1; gameStepNum <= gameStepNum_Max; gameStepNum++) {

     currentStrtgy1 = -1;
     currentStrtgy2 = -1;     

     /* 各プレイヤの戦略決定*/
     Player1(prevStrtgy1, prevStrtgy2,  &currentStrtgy1);
     Player2(prevStrtgy1, prevStrtgy2,  &currentStrtgy2);

     /*乱数の発生*/
     random = rand()%100;

     /* プレイヤ1にエラーが起きた場合は戦略を逆にする */

     if (random < ERROR) {
       error_count1++;

#ifdef DEBUG
       printf("ERROR on p1. step = %d, random = %d\n", gameStepNum, random);
#endif

       if (currentStrtgy1 == 0) {
	 currentStrtgy1 = 1;
       } else if (currentStrtgy1 == 1) {
	 currentStrtgy1 = 0;
       }
     }

     /*乱数の発生*/
     random = rand()%100;

     /* プレイヤ2にエラーが起きた場合は戦略を逆にする */

     if (random < ERROR) {

       error_count2++;

#ifdef DEBUG
       printf("ERROR on p2. step = %d, random = %d\n", gameStepNum, random);
#endif

       if (currentStrtgy2 == 0) {
	 currentStrtgy2 = 1;
       } else if (currentStrtgy2 == 1) {
	 currentStrtgy2 = 0;
       }
     }

#ifdef DEBUG
     printf("p1: %d, p2: %d\n", currentStrtgy1, currentStrtgy2);
#endif

     prevStrtgy1 = currentStrtgy1;
     prevStrtgy2 = currentStrtgy2;

     /*両プレイヤの今回の戦略の利得と総得点の計算 */
     payoff(currentStrtgy1, currentStrtgy2,  &payoff1, &payoff2);

     score1 = score1 + payoff1; 
     score2 = score2 + payoff2; 

   }

   /* ゲームの終了時の総得点の表示 */
   printf("Game Over!! Player1 score: %d, Player2 score: %d\n", score1, score2);

//#ifdef DEBUG

   printf("p1 error: %d, ratio: %f\n", error_count1, ((float) error_count1)/((float) gameStepNum_Max));
   printf("p2 error: %d, ratio: %f\n", error_count2, ((float) error_count2)/((float) gameStepNum_Max));

//#endif
   return(0);
}
