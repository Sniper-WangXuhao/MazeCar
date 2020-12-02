/*************************************************
* @par Copyright (C): 2010-2019, hunan CLB Tech
* @file         Basic_movement.c
* @version      V2.0
* @details
* @par History

@author: zhulin
***************************************************/
#include "LOBOROBOT.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <arpa/inet.h>

#include <wiringPi.h>
#include <softPwm.h>


#define BUFSIZE 512


int main_1(int argc, char *argv[])
{
    while(1){
    // 电机初始化
    Motor_Init();
    int move,speed,time;
    printf("forward 0 backward 1 turnleft 2 turnright 3 forward_Left 4 forward_Right 5\n");
    printf("backward_Left 6 backward_Right 7 moveLeft 8 moveRight 9\n");
    scanf("%d %d %d",&move,&speed,&time);
    switch(move){
        case 0:t_up(speed,time);
            break;
        case 1:t_down(speed,time);
            break;
        case 2:turnLeft(speed,time);
            break;
        case 3:turnRight(speed,time);
            break;
        case 4:forward_Left(speed,time);
            break;
        case 5:forward_Right(speed,time);
            break;
        case 6:backward_Left(speed,time);
            break;
        case 7:backward_Right(speed,time);
            break;
        case 8:moveLeft(speed,time);
            break;
        case 9:moveRight(speed,time);
            break;
    }
     t_stop(2);
  }
}
