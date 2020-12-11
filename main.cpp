#include <wiringPi.h>  
#include <pthread.h>
#include <stdio.h>  
#include<iostream>
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
pthread_mutex_t mute_Hand;

float dis_Front;
float dis_Back;
float dis_Right;
float dis_Left;

/*******************舵机定义*************************
*****************************************************/
int myservo1 = 15; //舵机0 手爪
int myservo2 = 14; //舵机1 上臂
int myservo3 = 13; //舵机2 下臂
int myservo4 = 12; //舵机3 底座

float currentAngle[4];
float s_MIN[4];
float s_MAX[4];
float INITANGLE[4];

void* armCtrl1(void*);
void* fakeOpencv(void*);

//传感器线程
void ultraInit(void);
void* disMeasureFront(void*);
void* disMeasureBack(void*);
void* disMeasureRight(void*);
void* disMeasureLeft(void*);

//行动
void forwardToWall(float dis);
void backwardToWall(float dis);

using namespace std;
int main(void) {
    pthread_mutex_init(&mute_Back, NULL);
    pthread_mutex_init(&mute_Left, NULL);
    pthread_mutex_init(&mute_Front, NULL);
    pthread_mutex_init(&mute_Right, NULL);
    pthread_mutex_init(&mute_Hand, NULL);
    //上传测试
    if (wiringPiSetup() == -1) { //when initialize wiring failed,print messageto screen  
        printf("setup wiringPi failed !");
        return 1;
    }

    //按钮启动
    while (0 == digitalRead(BTN));

    ultraInit();
    Motor_Init();


    //multitask
    pthread_t pidUltraFront;
    pthread_t pidUltraBack;
    pthread_t pidUltraRight;
    pthread_t pidUltraLeft;
    pthread_t pidHand;
    pthread_t pidOpencv;
    
    pthread_create(&pidUltraFront, NULL, disMeasureFront, NULL);
    pthread_create(&pidUltraBack, NULL, disMeasureBack, NULL);
    pthread_create(&pidUltraRight, NULL, disMeasureRight, NULL);
    pthread_create(&pidUltraLeft, NULL, disMeasureLeft, NULL);

    /*while (1) {
        printf("FRONT=:%.2f, BACK=:%.2f, RIGHT=:%.2f, LEFT=:%.2f\n", dis_Front, dis_Back, dis_Right, dis_Left);
        delay(500);
    }*/

    //set_servo_angle(myservo3, 25); //下臂舵机
    //set_servo_angle(myservo2, 70); //上臂舵机
    //set_servo_angle(myservo4, 0); //底座舵机
    //set_servo_angle(myservo1, 25); //手爪舵机
    //cout << "1 base 2 lhand 3hhand 4 hand\n";
    //while (1) {
    //    int temp;
    //    int n;
    //    int myservo;
    //    cin >> temp>>n;
    //    switch (temp) {
    //    case 1:myservo = myservo4;
    //        break;
    //    case 2:myservo = myservo3;
    //        break;
    //    case 3:myservo = myservo2;
    //        break;
    //    case 4:myservo = myservo1;
    //        break;
    //    default:
    //        break;
    //    }
    //    set_servo_angle(myservo, n);
    // 
    //}
    //pthread_mutex_init(&mute_Hand,NULL);
    
    delay(100);
    pthread_create(&pidOpencv, NULL, fakeOpencv, NULL);
    
    pthread_create(&pidHand, NULL, armCtrl1, NULL);

    //while (1) {
    //    set_servo_angle(myservo3, 25); //下臂舵机
    //    set_servo_angle(myservo2, 75); //上臂舵机
    //    delay(200);
    //    set_servo_angle(myservo4, 50); //底座舵机
    //    set_servo_angle(myservo1, 25); //手爪舵机
    //    delay(1000);

    //    set_servo_angle(myservo3, 55); //下臂舵机
    //    delay(500);

    //    set_servo_angle(myservo1, 16); //手爪舵机
    //    delay(500);

    //    set_servo_angle(myservo2, 90); //上臂舵机
    //    delay(500);

    //    set_servo_angle(myservo4, 110); //底座舵机
    //    delay(500);

    //    set_servo_angle(myservo3, 100); //下臂舵机
    //    delay(2000);

    //    set_servo_angle(myservo1, 25); //手爪舵机
    //    delay(1000);
    //}
    

    ////first cornor
    //forwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        turnRight(100, 500);
    //        break;
    //    }
    //}
   
    ////second cornor
    //forwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 1400);
    //        break;
    //    }
    //}
    //
    ////third corner
    //backwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 1400);
    //        break;
    //    }
    //}

    ////forth corner
    //forwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 1400);
    //        break;
    //    }
    //}

    ////fifth corner
    //backwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 1400);
    //        break;
    //    }
    //}

    ////sixth corner
    //forwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 1400);
    //        break;
    //    }
    //}

    ////seventh corner
    //backwardToWall(12);
    //delay(100);
    //while (1) {
    //    if (dis_Right > 20) {
    //        moveRight(100, 2000);
    //        break;
    //    }
    //}


while (1);
    return 0;
}


void ultraInit(void) {
    pinMode(Echo_Front, INPUT);
    pinMode(Trig_Front, OUTPUT);
    pinMode(Echo_Back, INPUT);
    pinMode(Trig_Back, OUTPUT);
    pinMode(Echo_Left, INPUT);
    pinMode(Trig_Left, OUTPUT);
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

        pthread_mutex_lock(&mute_Front);
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

        pthread_mutex_lock(&mute_Back);
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
        pthread_mutex_lock(&mute_Right);
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
        pthread_mutex_lock(&mute_Left);
        dis_Left = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
        pthread_mutex_unlock(&mute_Left);
        delay(100);
    }
}

//前进到墙
//param dis 到墙边距离
void forwardToWall(float dis) {

    while (1) {
        pthread_mutex_lock(&mute_Front);
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

//后退到墙
//parm dis 到墙边距离
void backwardToWall(float dis) {

    while (1) {
        pthread_mutex_lock(&mute_Back);
        if (dis_Back < 1190) {
            if (dis_Back > dis) {
                down(100);
            }
            else {
                stop();
                return;
            }

        }
        pthread_mutex_unlock(&mute_Back);
        delay(100);
    }
}

void* armCtrl1(void *){
    while (1) {
        set_servo_angle(myservo3, 25); //下臂舵机
        set_servo_angle(myservo2, 75); //上臂舵机
        delay(200);
        set_servo_angle(myservo4, 50); //底座舵机
        set_servo_angle(myservo1, 25); //手爪舵机
        delay(1000);

        set_servo_angle(myservo3, 55); //下臂舵机
        delay(500);

        set_servo_angle(myservo1, 16); //手爪舵机
        delay(500);

        set_servo_angle(myservo2, 90); //上臂舵机
        delay(500);

        set_servo_angle(myservo4, 110); //底座舵机
        delay(500);

        set_servo_angle(myservo3, 100); //下臂舵机
        delay(2000);
        pthread_mutex_lock(&mute_Hand);
        set_servo_angle(myservo1, 25); //手爪舵机
        delay(1000);
        pthread_mutex_unlock(&mute_Hand);
        delay(100);
    }
}

void* fakeOpencv(void*) {
    while (1) {
        pthread_mutex_lock(&mute_Hand);
        printf("wait\n");
        delay(5000);
        pthread_mutex_unlock(&mute_Hand);
        delay(200);
    }
}