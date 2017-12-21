//Nokia5110_I2C_Backpack_V1.ino

#define programname "Nokia5110_I2C_Backpack_Adafruit"
#define programversion "V1.0"
#define dateproduced "211217"
#define aurthorname "Stuart Robinson"
#include <Arduino.h>



/*
******************************************************************************************************

  LoRaTracker Programs for Arduino

  Copyright of the author Stuart Robinson - 21/12/2017

  http://www.LoRaTracker.uk

  These programs may be used free of charge for personal, recreational and educational purposes only.

  This program, or parts of it, may not be used for or in connection with any commercial purpose without
  the explicit permission of the author Stuart Robinson.

  The programs are supplied as is, it is up to individual to decide if the programs are suitable for the
  intended purpose and free from errors.


  To Do:
  Check for overflow on I2C 32 character input buffer
  Change contrast setting to match Adafruit library


  Changes:



  ******************************************************************************************************
*/


#include <SPI.h>

#include "BackpackV4 _Board_Definitions.h"
#include "I2CDisplay_Definitions.h"

#include <Adafruit_GFX.h>                   //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_PCD8544.h>               //https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
#define default_contrast 55
Adafruit_PCD8544 disp = Adafruit_PCD8544(DISP_DC, DISP_CS, DISP_RESET);
byte contrast = default_contrast;


#include <Wire.h>


#define Backpack_Address 0x08
//#define DEBUG                              //define this to see debug on serial terminal

#define Serial_Monitor_Baud 115200         //this is baud rate used for the Arduino IDE Serial Monitor

byte receive_buffer[32];
byte receive_buffer_ptr;
boolean buffer_ready = false;
byte textsize = 1;                          //used to keep track of current font scale 1 or 2

unsigned long startmS, timemS;


void loop()
{
  if (buffer_ready)
  {
    process_Buffer();
  }
}


void process_Buffer()
{
  byte Count;
  byte command = receive_buffer[0];

#ifdef DEBUG
  Count = receive_buffer_ptr + 1;
  Serial.print(F("RX,"));
  Serial.println(Count);
  startmS = millis();
  Serial.write(command);
#endif

  switch (command)
  {
    case StartText:
      process_StartText();
      break;

    case CursorPosition:
      process_CursorPosition();
      break;

    case ClearDisplay:
      process_ClearDisplay();
      break;

    case Contrast:
      process_Contrast();
      break;

    case FontScale:
      process_FontScale();
      break;

    case InitialiseDisplay:
      process_InitialiseDisplay();
      break;

    case UpdateDisplay:
      process_UpdateDisplay();
      break;

    case Test:
      process_Test();
      break;  

    default:
      Serial.print(F("Command Error"));
      Serial.println(command, HEX);
  }
  buffer_ready = false;

#ifdef DEBUG
  Serial.println();
  timemS = millis() - startmS;
  Serial.print(timemS);
  Serial.println(F("mS"));
#endif
}


void process_StartText()                             //process text in the buffer
{
  byte writebyte, i;

  for (i = 1; i <= receive_buffer_ptr; i++)
  {
    writebyte = receive_buffer[i];
    disp.write(writebyte);
#ifdef DEBUG
    Serial.write(writebyte);
#endif
  }

}


void process_CursorPosition()
{
  byte lcol, lrow;

  lcol = receive_buffer[1];
  lrow = receive_buffer[2];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lcol);
  Serial.print(F(","));
  Serial.print(lrow);
#endif

  if (textsize == 1)                 // test for the font scale currently in use
  {
    lcol = lcol * 6;              // Calculate the column position
    lrow = (lrow * 8);            // Calculate the row position
  }
  if (textsize == 2)
  {
    lcol = lcol * 12;             // Calculate the column position
    lrow = (lrow * 16);           // Calculate the row position
  }

  disp.setCursor(lcol, lrow);  // done the conversion so position the cursor
}


void process_ClearDisplay()
{
  disp.clearDisplay();
}


void process_Contrast()
{
  byte lcontrast = receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lcontrast);
#endif

  disp.setContrast(lcontrast);
}


void process_FontScale()
{
  byte textsize = receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(textsize);
#endif
  disp.setTextSize(textsize);
}


void process_UpdateDisplay()
{
  disp.display();
}


void receiveEvent (int howMany)
{
  buffer_ready = false;
  receive_buffer_ptr = 0;

  while (Wire.available())
  {
    char c = Wire.read();
    receive_buffer[receive_buffer_ptr] = c;
    receive_buffer_ptr++;
  }
  receive_buffer[receive_buffer_ptr] = (byte) howMany;
  receive_buffer_ptr--;
  buffer_ready = true;
}


void return_Busy_Status()
{
  //this interrupt routine processes the request for returning the busy flag to master

  if (buffer_ready)
  {
    Wire.write(1);
  }
  else
  {
    Wire.write(0);
  }

}


void process_InitialiseDisplay()
{
  //this sets the display back to defaults

  disp.setContrast(default_contrast);
  disp.clearDisplay();
  textsize = 1;
  disp.setCursor(0, 0);
  disp.println("Ready");
  disp.display();
}


void process_Test()
{

  //a standard display routine test. 
  unsigned long startmS = 0, texttimemS = 0, cleartimemS = 0;
  Serial.println();
  startmS = millis();
  disp.clearDisplay();
  disp.display();
  cleartimemS = millis() - startmS;
  
  startmS = millis();
  disp.setTextSize(1);
  disp.setCursor(0, 0);

  disp.println(F("Nokia5100   "));    //padded to 12 characters
  disp.setCursor(0, 8);
  disp.print("Ready");
  disp.display();
  texttimemS = millis() - startmS;
  disp.setCursor(0, 16);
  
  disp.print(F("Text  "));
  disp.print(texttimemS);
  disp.print(F("mS"));
 
  Serial.print(F("<Text  "));
  Serial.print(texttimemS);
  Serial.println(F("mS>"));
  
  disp.setCursor(0, 24);
  
  disp.print(F("Clear "));
  disp.print(cleartimemS);
  disp.print(F("mS"));
  disp.display();
  
  Serial.print(F("<Clear "));
  Serial.print(cleartimemS);
  Serial.println(F("mS>"));
  
}


void startText()
{
  disp.clearDisplay();
  disp.setTextSize(1);
  disp.setCursor(0, 0);
  disp.println(F("Nokia 5110"));    
  disp.setCursor(0, 8);    
  disp.println(F("I2C Backpack"));    
  disp.setCursor(0, 16);    
  disp.println(F("Ready"));    
  disp.display();
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);       //setup Serial console ouput
  Serial.println();
  Serial.println(F(programname));
  Serial.println(F(programversion));
  Serial.println(F(dateproduced));
  Serial.println(F(aurthorname));

  SPI.begin();                              //initialize SPI:
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
 
  disp.begin();                             //Start the display
  process_InitialiseDisplay();
  startText();
  
  Wire.begin(Backpack_Address);             //connect to i2c bus, address of Backpack
  Wire.onReceive(receiveEvent);             //this interrupt routine processes incoming characters
  Wire.onRequest(return_Busy_Status);       //this interrupt routine processes the request for return of busy flag from master
  Serial.println(F("Backpack Ready"));
}



