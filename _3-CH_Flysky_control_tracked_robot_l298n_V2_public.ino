/*
 2.4G 3-Channel Flysky FS-GT3C Receiver decoding Arduino
 Controlling tracked robot  
 Written by: Pason Tanpaiboon
 March 2017
 Version.1.2
 29/3/2017 This project purposed to decode Flyky receiver to control Tracked robot via L298N module.  
 30/3/2017 Modify code for HK-GT2R receiver
 Flysky read code based on Ryan Boland's sketch https://ryanboland.com/blog/reading-rc-receiver-values/
  
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */
 
#include <Servo.h> 
#include <SPI.h>
#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 115200 
#define RC_NUM_CHANNELS  3

#define RC_CH1  0
#define RC_CH2  1
#define RC_CH3  2

#define RC_CH1_INPUT  A0
#define RC_CH2_INPUT  A1
#define RC_CH3_INPUT  A2

//////////////////////////////////////////////////////////////////
int ch1;
int ch2;
int ch3;

int ch1Val;
int ch2Val;
int ch3Val;
//////////////////////////////////////////////////////////////////
//Motor Right
const int enableR = 3;
const int MotorR1 = 4;
const int MotorR2 = 5;

//Motor Left
const int enableL = 6;
const int MotorL1 = 7;
const int MotorL2 = 8;

int enableRval ;
int enableLval ;
int MotorR1val ;
int MotorR2val ;
int MotorL1val ;
int MotorL2val ;

int MotorRspeed ;
int MotorLspeed ;

//////////////////////////////////////////////////////////////////
uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }
void calc_ch2() { calc_input(RC_CH2, RC_CH2_INPUT); }
void calc_ch3() { calc_input(RC_CH3, RC_CH3_INPUT); }

void setup() {
  
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_INPUT, INPUT);
  pinMode(RC_CH2_INPUT, INPUT);
  pinMode(RC_CH3_INPUT, INPUT);
 
  enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);

//////////////////////////////////////////////////////////////////

  pinMode (enableR, OUTPUT);
  pinMode (MotorR1, OUTPUT);
  pinMode (MotorR2, OUTPUT);  

  pinMode (enableL, OUTPUT);
  pinMode (MotorL1, OUTPUT);
  pinMode (MotorL2, OUTPUT);  
  
  digitalWrite(enableR , HIGH);
  digitalWrite(enableL , HIGH);

}//void setup()

void loop() {
  rc_read_values();
delay(10);
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("  ");
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("  ");
  Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("  ");

ch1Val = rc_values[RC_CH1];//1380,1508 ,1650
ch2Val = rc_values[RC_CH2];//1804  1476  1208
ch3Val = rc_values[RC_CH3];//

////////////////////////////////////ch1////////////////////////////////////////
ch1Val = map(ch1Val, 1380, 1620, 0, 180);     // code between 1380,1508 ,1620

////////////////////////////////////ch1 neutral position////////////////////////////////////////
if (ch1Val >= 80 && ch1Val <= 100 ) ////neutral position
{
ch1Val = 90; 
  
}

////////////////////////////////////ch2 neutral position////////////////////////////////////////
if (ch2Val >= 1450 && ch2Val <= 1550 ) ////neutral position
{
ch2Val = 1500; 
  MotorRspeed = 0;
  MotorLspeed = 0;
  Engine();
}

/*************************************************************Stopping***************************************************************/      
      if (  ch2Val == 1500  )//neutral steering wheel + neutral trigger
     {
 
      MotorRspeed = 0;
      MotorLspeed = 0;
 
      MotorR1val = 0;
      MotorR2val = 0;
      MotorL1val = 0;
      MotorL2val = 0;
      Engine();
      Serial.print("Stopping");
     } 

  /*************************************************************Straight Forward***************************************************************/   
   
     if ( ch1Val == 90   &&  ch2Val > 1500 )//neutral steering wheel + positive trigger
     {
    
     MotorRspeed =  map(ch2Val,1810,1500,255,0);

     MotorLspeed =  MotorRspeed;
       
      MotorR1val = 1;
      MotorR2val = 0;
      MotorL1val = 1;
      MotorL2val = 0;
      Engine();
      Serial.print("Straight Forward");
     }      
 /*************************************************************Straight Backward***************************************************************/        
  
  if ( ch1Val == 90  && ch2Val < 1400   )//neutral steering wheel + negative trigger    
   {
      
     MotorRspeed = map(ch2Val,1200,1500,255,0);
         
     MotorLspeed = MotorRspeed;
        
      MotorR1val = 0;
      MotorR2val = 1;
      MotorL1val = 0;
      MotorL2val = 1;   
      Engine();
      Serial.print("Straight Backward");
   }

 /********************************************************************Forward Right Direction***************************************************************/   
 
  if (ch1Val > 100 && ch1Val < 170 && ch2Val > 1500 )//Right steering wheel + positive trigger     
     {
         
     MotorRspeed = 0;
           
     MotorLspeed = map(ch2Val,1810,1500,255,0);
      
      MotorR1val = 1;
      MotorR2val = 0;
      MotorL1val = 1;
      MotorL2val = 0;
      Engine();
      Serial.print("Forward Right");
     }

  /********************************************************************Forward Left Direction***************************************************************/      
     
  if ( ch1Val > 15  &&  ch1Val < 80 && ch2Val > 1500 )//Left steering wheel + positive trigger
     {
          
     MotorRspeed = map(ch2Val,1800,1500,255,0);
     
     MotorLspeed = 0;
       
      MotorR1val = 1;
      MotorR2val = 0;
      MotorL1val = 1;
      MotorL2val = 0;   
      Engine();
      Serial.print("Forward Left");
     }

/********************************************************************Backward Right Direction***************************************************************/      
   if (ch1Val > 100 && ch1Val < 170 && ch2Val < 1500 )//Right steering wheel + negative trigger    
     {
       
     MotorRspeed = 0;
          
     MotorLspeed = map(ch2Val,1200,1500,255,0);
          
      MotorR1val = 0;
      MotorR2val = 1;
      MotorL1val = 0;
      MotorL2val = 1;
      Engine();
      Serial.print("Backward Right");
     }
   /************************************************Backward Left Direction***************************************************************/   
   
  if (ch1Val < 80 && ch1Val > 10 && ch2Val < 1500)//Left steering wheel + negative trigger    
     {
         
     MotorRspeed = map(ch2Val,1200,1500,255,0);
         
     MotorLspeed = 0;
     
      MotorR1val = 0;
      MotorR2val = 1;
      MotorL1val = 0;
      MotorL2val = 1;   
      Engine();
      Serial.print("Backward Left");
     }   
  /*************************************************************Rotate Right***************************************************************/ 
   if ( ch1Val >= 170 && ch1Val <= 180  && ch2Val > 1500 )//Full right steering wheel + positive trigger
   {
         
     MotorRspeed = map(ch2Val,1800,1500,255,0);
      
     MotorLspeed = MotorRspeed;
        
      MotorR1val = 0;
      MotorR2val = 1;
      MotorL1val = 1;
      MotorL2val = 0;   
      Engine();
      Serial.print("Rotate Right");
   } 
 /*************************************************************Rotate Right Reverse***************************************************************/ 
   if ( ch1Val >= 170 && ch1Val <= 180  && ch2Val < 1500 )//Full right steering wheel + negative trigger
   {
         
     MotorRspeed = map(ch2Val,1200,1500,255,0);
      
     MotorLspeed = MotorRspeed;
        
      MotorR1val = 1;
      MotorR2val = 0;
      MotorL1val = 0;
      MotorL2val = 1;   
      Engine();
      Serial.print("Rotate Right Reverse");
   }
 /*************************************************************Rotate Left***************************************************************/    
 
   if ( ch1Val <= 10 && ch1Val >= 0 && ch2Val > 1500)//Full left steering wheel + trigger
   {
     
     MotorRspeed = map(ch2Val,1800,1500,255,0);
        
     MotorLspeed = MotorRspeed;
    
      MotorR1val = 1;
      MotorR2val = 0;
      MotorL1val = 0;
      MotorL2val = 1;  
      Engine();
      Serial.print("Rotate Left"); 
   }
   /*************************************************************Rotate Left Reverse***************************************************************/    
 
   if ( ch1Val <= 15 && ch1Val >= 0 && ch2Val < 1500)//Full left steering wheel + trigger
   {
     
     MotorRspeed = map(ch2Val,1200,1500,255,0);
        
     MotorLspeed = MotorRspeed;
    
      MotorR1val = 0;
      MotorR2val = 1;
      MotorL1val = 1;
      MotorL2val = 0;  
      Engine();
      Serial.print("Rotate Left Reverse"); 
   }



Serial.print(">>>");
Serial.print("ch1Val:"); Serial.print(ch1Val); Serial.print("  ");
Serial.print("ch2Val:"); Serial.print(ch2Val); Serial.print("  ");
Serial.print("MRspeed :"); Serial.print(MotorRspeed);Serial.print("  ");
Serial.print("MLspeed :"); Serial.println(MotorLspeed);
  
}//void loop


/*****************Driving motor *********************************************************************/

void Engine() {
//Right Motor 

  analogWrite(enableR , MotorRspeed);// 
 
  digitalWrite (MotorR1,MotorR1val);
  digitalWrite (MotorR2,MotorR2val);  
  delay (15);
 //Left Motor 

  analogWrite(enableL , MotorLspeed);// 

  digitalWrite (MotorL1,MotorL1val);
  digitalWrite (MotorL2,MotorL2val);  
  delay (15);
}


