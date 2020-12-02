#include <wiringPi.h>  
#include <pthread.h>
#include <stdio.h>  
#include <sys/time.h>  
#include "LOBOROBOT.h"

#define BTN 24
#define Trig_Front    0 
#define Echo_Front    27 
#define Trig_Back    28
#define Echo_Back    29
#define Trig_Left    21
#define Echo_Left    22
#define Trig_Right    23
#define Echo_Right    26

pthread_mutex_t mute_Front;
pthread_mutex_t mute_Back;
pthread_mutex_t mute_Right;
pthread_mutex_t mute_Left;

float dis_Front;
float dis_Back;
float dis_Right;
float dis_Left;

void ultraInit(void);
void* disMeasureFront(void*);
void* disMeasureBack(void*);
void* disMeasureRight(void*);
void* disMeasureLeft(void*);

void forwardToWall(float dis);


int main(void) {
   
    //上传测试
    if (wiringPiSetup() == -1) { //when initialize wiring failed,print messageto screen  
        printf("setup wiringPi failed !");
        return 1;
    }

    while (0 == digitalRead(BTN));

    ultraInit();
    Motor_Init();

    pthread_t pidUltraFront;
    pthread_t pidUltraBack;
    pthread_t pidUltraRight;
    pthread_t pidUltraLeft;
    pthread_create(&pidUltraFront, NULL, disMeasureFront, NULL);
    pthread_create(&pidUltraBack, NULL, disMeasureBack, NULL);
    pthread_create(&pidUltraRight, NULL, disMeasureRight, NULL);
    pthread_create(&pidUltraLeft, NULL, disMeasureLeft, NULL);

    /*while (1) {
        printf("FRONT=:%.2f, BACK=:%.2f, RIGHT=:%.2f, LEFT=:%.2f\n", dis_Front, dis_Back, dis_Right, dis_Left);
        delay(500);
    }*/



    delay(100);

    //first cornor
    forwardToWall(12);
    delay(100);
    while (1) {
        if (dis_Right > 20) {
            turnRight(100, 500);
            break;
        }
    }
    
    //second cornor
    forwardToWall(12);
    delay(100);
    while (1) {
        if (dis_Right > 20) {
            moveRight(100, 1000);
            break;
        }
    }



    return 0;
}


void ultraInit(void) {
    pinMode(Echo_Front, INPUT);
    pinMode(Trig_Front, OUTPUT);
    pinMode(Echo_Back, INPUT);
    pinMode(Trig_Back, OUTPUT);
    //pinMode(Echo_Left, INPUT);
    //pinMode(Trig_Left, OUTPUT);
    pinMode(Echo_Right, INPUT);
    pinMode(Trig_Right, OUTPUT);
}

void* disMeasureFront(void*) {


    while (1) {

        struct timeval tv1;
        struct timeval tv2;
        long start, stop;


        digitalWrite(Trig_Front, LOW);
        delayMicroseconds(2);

        digitalWrite(Trig_Front, HIGH);
        delayMicroseconds(10);      //发出超声波脉冲  
        digitalWrite(Trig_Front, LOW);

        while (!(digitalRead(Echo_Front) == 1));
        gettimeofday(&tv1, NULL);           //获取当前时间  

        while (!(digitalRead(Echo_Front) == 0));
        gettimeofday(&tv2, NULL);           //获取当前时间  

        start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间  
        stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

        pthread_mutex_trylock(&mute_Front);
        dis_Front = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
        pthread_mutex_unlock(&mute_Front);

        delay(100);
    }
}
void* disMeasureBack(void*) {


    while (1) {

        struct timeval tv1;
        struct timeval tv2;
        long start, stop;


        digitalWrite(Trig_Back, LOW);
        delayMicroseconds(2);

        digitalWrite(Trig_Back, HIGH);
        delayMicroseconds(10);      //发出超声波脉冲  
        digitalWrite(Trig_Back, LOW);

        while (!(digitalRead(Echo_Back) == 1));
        gettimeofday(&tv1, NULL);           //获取当前时间  

        while (!(digitalRead(Echo_Back) == 0));
        gettimeofday(&tv2, NULL);           //获取当前时间  

        start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间  
        stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

        pthread_mutex_trylock(&mute_Back);
        dis_Back = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
        pthread_mutex_unlock(&mute_Back);


        delay(100);
    }
}
void* disMeasureRight(void*) {


    while (1) {

        struct timeval tv1;
        struct timeval tv2;
        long start, stop;


        digitalWrite(Trig_Right, LOW);
        delayMicroseconds(2);

        digitalWrite(Trig_Right, HIGH);
        delayMicroseconds(10);      //发出超声波脉冲  
        digitalWrite(Trig_Right, LOW);

        while (!(digitalRead(Echo_Right) == 1));
        gettimeofday(&tv1, NULL);           //获取当前时间  

        while (!(digitalRead(Echo_Right) == 0));
        gettimeofday(&tv2, NULL);           //获取当前时间  

        start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间  
        stop = tv2.tv_sec * 1000000 + tv2.tv_usec;
        pthread_mutex_trylock(&mute_Right);
        dis_Right = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
        pthread_mutex_unlock(&mute_Right);
        delay(100);
    }
}
void* disMeasureLeft(void*) {


    while (1) {

        struct timeval tv1;
        struct timeval tv2;
        long start, stop;


        digitalWrite(Trig_Left, LOW);
        delayMicroseconds(2);

        digitalWrite(Trig_Left, HIGH);
        delayMicroseconds(10);      //发出超声波脉冲  
        digitalWrite(Trig_Left, LOW);

        while (!(digitalRead(Echo_Left) == 1));
        gettimeofday(&tv1, NULL);           //获取当前时间  

        while (!(digitalRead(Echo_Left) == 0));
        gettimeofday(&tv2, NULL);           //获取当前时间  

        start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间  
        stop = tv2.tv_sec * 1000000 + tv2.tv_usec;
        pthread_mutex_trylock(&mute_Left);
        dis_Left = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
        pthread_mutex_unlock(&mute_Left);
        delay(100);
    }
}
//前进到墙
//param dis 到墙边距离
void forwardToWall(float dis) {

    while (1) {
        pthread_mutex_trylock(&mute_Front);
        if (dis_Front < 1190) {
            if (dis_Front > dis) {
                up(100);
            }
            else {
                stop();
                return;
            }
                
        }
        pthread_mutex_unlock(&mute_Front);
    }
}