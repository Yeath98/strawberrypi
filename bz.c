#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>


#define Trig	28
#define Echo	29
#define LEFT	11
#define RIGHT	10
#define BUFSIZE 512

#define MOTOR_GO_FORWARD   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_BACK	   digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT	   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_LEFT	   digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_STOP	   digitalWrite(1, LOW);digitalWrite(4,LOW);digitalWrite(5, LOW);digitalWrite(6,LOW)


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

void left()         //左转()
{
    softPwmWrite(4,250); //左轮
	softPwmWrite(1,0); 
	softPwmWrite(6,0); //右轮前进
	softPwmWrite(5,250); 
	//delay(time * 300);
   // delay(time * 300);

}



void right()        //右转()
{
    softPwmWrite(4,0); //左轮前进
	softPwmWrite(1,250); 
	softPwmWrite(6,250); //右轮
	softPwmWrite(5,0); 
    //delay(time * 300);	//执行时间，可以调整
}



void back()          //后退
{
    softPwmWrite(4,250); //左轮back
	softPwmWrite(1,0); 
	softPwmWrite(6,250); //右轮back
  	softPwmWrite(5,0); 
    //delay(time *200);     //执行时间，可以调整
}
int main(int argc, char *argv[])
{

    float dis;

   // char buf[BUFSIZE]={0xff,0x00,0x00,0x00,0xff};

	int SR;
	int SL;
    /*RPI*/
    wiringPiSetup();
    /*WiringPi GPIO*/
    pinMode (1, OUTPUT);	//IN1
    pinMode (4, OUTPUT);	//IN2
    pinMode (5, OUTPUT);	//IN3
    pinMode (6, OUTPUT);	//IN4
    // pinMode (27, OUTPUT);	//
    softPwmCreate(1,1,500);   
    softPwmCreate(4,1,500);
    softPwmCreate(5,1,500);
    softPwmCreate(6,1,500);
    // softPwmCreate(27,1,50);	
	//softPwmWrite(27,1);

 while(1)
  {
  //有信号为LOW  没有信号为HIGH
  SR = digitalRead(RIGHT);//
  SL = digitalRead(LEFT);//
  if (SL == LOW&&SR==LOW){
   printf("BACK");  //前面有物体时小车后退？？？ms 再转弯
   back();
   delay(300);
   //后退500ms
   left();//左转400ms
   delay(601);
	   
   
  }
  else if (SL == HIGH&&SR == LOW){
	  printf("RIGHT");

	 left();
	  
  }
  else if (SR == HIGH&&SL == LOW) {//  
  printf("LEFT");
  
  right ();
  }
  else {// 前面没有物体 前进
 printf("GO");
 run();
 }
  }

 
  return 0;

}

