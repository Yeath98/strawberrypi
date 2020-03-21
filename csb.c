#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <wiringPi.h>
#define Trig	28
#define Echo	29
#define BUFSIZE 512
void ultraInit(void)
{
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}

float disMeasure(void)
{
  struct timeval tv1;
  struct timeval tv2;
  long start, stop;
  float dis;

  digitalWrite(Trig, LOW);
  delayMicroseconds(2);

  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);	  //发出超声波脉冲
  digitalWrite(Trig, LOW);
  
  while(!(digitalRead(Echo) == 1));
  gettimeofday(&tv1, NULL);		   //获取当前时间

  while(!(digitalRead(Echo) == 0));
  gettimeofday(&tv2, NULL);		   //获取当前时间

  start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间
  stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;

  dis = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离

  return dis;
}
void run()     // 前进
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,250); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,250); 

 
}

void brake(int time)         //刹车，停车
{
    softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //stop
	softPwmWrite(6,0); 
    delay(time * 100);//执行时间，可以调整  
}

void left(int time)         //左转(左轮不动，右轮前进)
{
    softPwmWrite(1,0); //左轮stop
	softPwmWrite(4,0); 
	softPwmWrite(5,0); //右轮前进
	softPwmWrite(6,250); 
	delay(time * 300);
}



void right(int time)        //右转(右轮不动，左轮前进)
{
    softPwmWrite(1,0); //左轮前进
	softPwmWrite(4,250); 
	softPwmWrite(5,0); //右轮stop
	softPwmWrite(6,0); 
    delay(time * 300);	//执行时间，可以调整  
}



void back(int time)          //后退
{
    softPwmWrite(4,250); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //右轮back
  	softPwmWrite(5,0); 
    delay(time *200);     //执行时间，可以调整  
}
int main(int argc, char *argv[])
{

    float dis;

   // char buf[BUFSIZE]={0xff,0x00,0x00,0x00,0xff};


    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
  while(1){
    dis = disMeasure();
    printf("distance = %0.2f cm\n",dis);//输出当前超声波测得的距离
     if(dis<30){   //测得前方障碍的距离小于30cm时做出如下响应


	   back(4);//后退500ms
	   left(4);//左转300ms
	   

      }
     else {

        run();  //无障碍时前进
         }
  }
  return 0;

}

