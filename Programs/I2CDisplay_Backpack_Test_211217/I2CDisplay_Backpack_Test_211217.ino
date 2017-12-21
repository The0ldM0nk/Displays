//I2CDisplay_Backpack_Test_211217.ino
#define programname "I2CDisplay_Backpack_Speed_Test_211217"
#define programversion "V1.0"
#define dateproduced "21/12/2017"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>

/*
*****************************************************************************************************************************

  LoRaTracker Test Programs

  Copyright of the author Stuart Robinson - 21/12/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without the explicit permission
  of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the intended purpose and
  free from errors.

*****************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Program operation

  This test program has been written to that the LoRaTracker I2C backpack display is working. The program displays two screens
  a breief welcom screen and then a simulation of the Locator2 tracker screen.

  The test program should work on both ILI9341 and Nokia5100 I2CDisplay backpacks, see the definition of the default_textscale
  below.

********************************************************************************************************************************
*/

/*
********************************************************************************************************************************
  Connections

  The program uses the standard I2C pins on an Arduino
  
********************************************************************************************************************************
*/

/*
********************************************************************************************************************************

  To Do:

  Changes:
  
********************************************************************************************************************************
*/


#include "I2CDisplay.h"                          

#define Display_Address 0x08
I2CDisplay disp(Display_Address);              //create the display class

unsigned long start_timems;
unsigned long end_timems;
unsigned int count = 0;

#define Serial_Monitor_Baud 38400              //this is baud rate used for the Arduino IDE Serial Monitor
#define default_textscale 1                    //set to tesxtscvale for display in use, 1 for Nokia 5100, 3 for ILI9341  
#define default_contrast 50                    //contrast for Nokia 5110 if in use, some 5110 displays have fixed contrast  

void loop()
{
  Serial.println();
  Serial.println(F("Display Test Starting"));
  
  disp.SetwaitReady(dowaitready);
  disp.SetTextColour(White);                   //ignored on non-colour screens
  disp.SetBackgroundColour(Black);             //ignored on non-colour screens 

  screen1();
  delay(1000);
  
  screen2();
  delay(5000);
}


void screen1()
{
Serial.println(F("Screen 1"));
disp.Clear();
disp.SetContrast(default_contrast);
disp.SetTextScale(default_textscale);
disp.SetDisplayRotation(1);
disp.SetCursorPosition(0,0);               
disp.print("I2C Backpack");     
disp.SetCursorPosition(0,1);
disp.print("LoRaTracker");
disp.SetCursorPosition(0,2);
disp.print("Ready");
disp.Update();
}


void screen2()
{
Serial.println(F("Screen 2"));
disp.Clear();
disp.SetContrast(default_contrast);
disp.SetTextScale(default_textscale);
disp.SetDisplayRotation(0);
disp.SetCursorPosition(0,0);                
disp.print("LA 51.48230");
disp.SetCursorPosition(0,1);
disp.print("LO -3.18136");         
disp.SetCursorPosition(0,2);
disp.print("AL 48M");
disp.SetCursorPosition(0,3);
disp.print("9.2K 6D");             
disp.SetCursorPosition(0,4);
disp.print("8dB  1    SEG");
disp.Update();
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);                    //setup Serial console ouput
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));
  Serial.println("If this is all you see, display may not be responding");
  Serial.println();
}






