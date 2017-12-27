/*
 2.4G 8-Channel Frsky Receiver decoding Arduino MEGA 2560
  
 Written by: Pason Tanpaiboon
 December 2017
 Version.1.2
 
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */
 

const int chA=22;  
const int chB=24;
const int chC=26;
const int chD=28;
const int chE=30;
const int chF=32;
const int chG=34;
const int chH=36;

int ch1;  
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;
int ch7;
int ch8;

void setup() {
  
  Serial.begin(115200);
  
  pinMode(chA,INPUT);
  pinMode(chB,INPUT);
  pinMode(chC,INPUT);
  pinMode(chD,INPUT);
  pinMode(chE,INPUT);
  pinMode(chF,INPUT);
  pinMode(chG,INPUT);
  pinMode(chH,INPUT);
}

void loop() {
 
  ch1 = pulseIn (chA,HIGH); 
  Serial.print ("Ch1:");  
  Serial.print (ch1);   
  Serial.print ("    ");
  delay(500);
 ////////////////////////////////////////////////////////////////////
  ch2 = pulseIn (chB,HIGH);
  Serial.print ("Ch2:");
  Serial.print (ch2);
  Serial.print ("    ");
  delay(500);
  ///////////////////////////////////////////////////////////////////////////
  ch3 = pulseIn (chC,HIGH);
  Serial.print ("Ch3:");
  Serial.print (ch3);
  Serial.print ("|");
  delay(300);
  /////////////////////////////////////////////////////////////////////////////
  ch4 = pulseIn (chD,HIGH);
  Serial.print ("Ch4:");
  Serial.print (ch4);
  Serial.print ("|");
 
  ch5 = pulseIn (chE,HIGH);
  Serial.print ("Ch5:");
  Serial.print (ch5);
  Serial.print ("|");
 
  ch6 = pulseIn (chF,HIGH);
  Serial.print ("Ch6:");
  Serial.print (ch6);
  Serial.print ("|");
 
  ch7 = pulseIn (chG,HIGH);
  Serial.print ("Ch7:");
  Serial.print (ch7);
  Serial.print ("|");
 
  ch8 = pulseIn (chH,HIGH);
  Serial.print ("Ch8:");
  Serial.println (ch8); 
}

