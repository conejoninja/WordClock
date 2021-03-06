/*
Copyright 2014 Daniel Esteban  -  conejo@conejo.me

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/ 

#include <Wire.h>

// Arduino pins to connect to MAX7219 
int dataIn = 2;
int load = 3;
int clock = 5;

// Buttons to change the time without need of a computer
int hourBtn = 12;
int minuteBtn = 11;
boolean hourState = 0;
boolean minState = 0;

// Default time
int hours = 0;
int minutes = 0;
int seconds = 0;


int maxInUse = 1;
int e = 0; // just a varialble

// define max7219 registers - DO NOT TOUCH unless you know what you're doing
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x07;
byte max7219_reg_digit7      = 0x08;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

//*******************************
// AUX. FUNCTION TO HANDLE MAX7219 EASILY
// I left original comments there
//*******************************
void putByte(byte data) {
  byte i = 8;
  byte mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// write 1
    }else{
      digitalWrite(dataIn, LOW); // write 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

void maxSingle( byte reg, byte col) {    
//maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);       // begin     
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data   
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load,HIGH); 
}

void maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin     
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  digitalWrite(load, LOW);
  digitalWrite(load,HIGH);
}

void maxOne(byte maxNr, byte reg, byte col) {    
//maxOne is for adressing different MAX7219's, 
//whilele having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin     

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data 

  for ( c =maxNr-1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load,HIGH); 
}
//*******************************
// END OF AUX MAX7219 FUNCTIONS
//*******************************

void setup () {

  // MAX 7219
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);
  
  // Buttons to change the time
  pinMode(hourBtn, INPUT);
  pinMode(minuteBtn, INPUT);

  // connection to DS3231
  Wire.begin();
 
  // clear /EOSC bit
  // Sometimes necessary to ensure that the clock
  // keeps running on just battery power. Once set,
  // it shouldn't need to be reset but it's a good
  // idea to make sure.
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();


  //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
}  

// Aux. function to set the time
void set_time()
{
   Wire.beginTransmission(104);
   Wire.write(0);
   Wire.write(decToBcd(seconds));
   Wire.write(decToBcd(minutes));
   Wire.write(decToBcd(hours));
   Wire.endTransmission();
}

byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}


// Main loop
void loop () {


    // write request to read data starting at register 0
  Wire.beginTransmission(0x68); // 0x68 is DS3231 device address
  Wire.write(0); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3); // request three bytes (seconds, minutes, hours)
 
  // Get time from RTC
  while(Wire.available())
  { 
    seconds = Wire.read(); // get seconds
    minutes = Wire.read(); // get minutes
    hours = Wire.read();   // get hours
 
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); // convert BCD to decimal
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); // convert BCD to decimal
    hours = (((hours & 0b00110000)>>4)*10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)
  }

  // Button is pressed, change hour (+1)
  if(digitalRead(hourBtn)==1 && hourState==0) {
    hours = hours+1;
    if(hours>23) {
      hours = 0;
    }
    set_time();
  }
  hourState = digitalRead(hourBtn);
 
  // Button is pressed, change minutes (+1) 
  if(digitalRead(minuteBtn)==1 && minState==0) {
    minutes = minutes+1;
    if(minutes>59) {
      minutes = 0;
    }
    set_time();
  }
  minState = digitalRead(minuteBtn);
 
  // Reset variables
  int a1 = 0;
  int a2 = 0;
  int a3 = 0;
  int a4 = 0;
  int a5 = 0;
  int a6 = 0;

  /*************
   ** MINUTES **
  *************/
  if(minutes>=0 && minutes<=4) {
    // O'clock
  } else if(minutes>=5 && minutes<=9) {
    // (y)CINCO
    a6 = a6+64;
  } else if(minutes>=10 && minutes<=14) {
    // (y)DIEZ
    a5 = a5+14;
  } else if(minutes>=15 && minutes<=19) {
    // (y)CUARTO
    a5 = a5+224;
  } else if(minutes>=20 && minutes<=24) {
    // (y)VEINTE
    a5 = a5+16;
  } else if(minutes>=25 && minutes<=29) {
    // (y)VEINTICINCO
    a3 = a3+136;
    a4 = a4+68;
    a5 = a5+68;
    a6 = a6+64;
    // VEINTICINCO shares some chars with a few hours, adjust the variables
    if(hours==9 || hours==21) { a3 = a3-128; }
    if(hours==7 || hours==19) { a3 = a3-8; }
    if(hours==6 || hours==18) { a4 = a4-64; }
  } else if(minutes>=30 && minutes<=34) {
    // (y)MEDIA
    a6 = a6+128;
  } else if(minutes>=35 && minutes<=39) {
    // (menos)VEINTICINCO
    a3 = a3+136;
    a4 = a4+64;
    a5 = a5+68;
    a6 = a6+64;
    // VEINTICINCO shares some chars with a few hours, adjust the variables
    if(hours==8 || hours==20) { a3 = a3-128; }
    if(hours==6 || hours==18) { a3 = a3-8; }
    if(hours==5 || hours==17) { a4 = a4-64; }
  } else if(minutes>=40 && minutes<=44) {
    // (menos)VEINTE
    a5 = a5+16;
  } else if(minutes>=45 && minutes<=49) {
    // (menos)CUARTO
    a5 = a5+224;
  } else if(minutes>=50 && minutes<=54) {
    // (menos)DIEZ
    a5 = a5+14;
  } else if(minutes>=55 && minutes<=59) {
    // (menos)CINCO    
    a6 = a6+64;
  }

  if(minutes>=5 && minutes<=34) {
    // Y
    a4 = a4+16;
  } else if(minutes>=35) {
    // MENOS
    a4 = a4+14;
  }

  // Move the hour if it's past 35 minutes as 7:35 is "OCHO(8) MENOS ..."
  if(minutes>=35) {
    hours++;
  }

  // SHOW none, 1,2,3 or 4 corner leds to exactly tell the time 7:17 -> SIETE Y CUARTO + 2 leds on
  if((minutes%5)==1) {
    a6 = a6+8;
  } else if((minutes%5)==2) {
    a6 = a6+8+4;
  } else if((minutes%5)==3) {
    a6 = a6+16+8+4;
  } else if((minutes%5)==4) {
    a6 = a6+32+16+8+4;
  }

   /*************************
   ** HOURS (follow table) **
   *************************/
   if(hours==0 || hours==12 || hours==24) {
    a1 = a1+96;
    a1 = a1+24;
    a3 = a3+2;
  } else if(hours==1 || hours==13) {
    a1 = a1+192;
    a1 = a1+16;
    a1 = a1+4;
  } else if(hours==2 || hours==14) {
    a1 = a1+96;
    a1 = a1+24;
    a1 = a1+2;
  } else if(hours==3 || hours==15) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+16;
  } else if(hours==4 || hours==16) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+8;
  } else if(hours==5 || hours==17) {
    a1 = a1+96;
    a1 = a1+24;
    a3 = a3+32;
  } else if(hours==6 || hours==18) {
    a1 = a1+96;
    a1 = a1+24;
    a4 = a4+224;
  } else if(hours==7 || hours==19) {
    a1 = a1+96;
    a1 = a1+24;
    a3 = a3+28;
  } else if(hours==8 || hours==20) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+192;
  } else if(hours==9 || hours==21) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+2;
    a3 = a3+192;
  } else if(hours==10 || hours==22) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+4;
  } else if(hours==11 || hours==23) {
    a1 = a1+96;
    a1 = a1+24;
    a2 = a2+96;
  }

  // Tell MAX7219 to print it pretty
  maxSingle(1, a1);
  maxSingle(2, a2);
  maxSingle(3, a3);
  maxSingle(4, a4);
  maxSingle(5, a5);
  maxSingle(6, a6);

  // loop every second, as it doesn't need to be very precise
  delay(1000);
  // This 1second delay makes it a bit harder to update the time with the buttons, as it doesn't feel very friendly
  // but you will not be changing the time very frequently, so not a big problem (for me)
}
