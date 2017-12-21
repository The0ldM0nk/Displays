//ILI9341_I2C_Backpack_V1B.ino

#define programname "ILI9341_I2C_Backpack_211217"
#define programversion "V1.0"
#define dateproduced "21/12/17"
#define aurthorname "Stuart Robinson"


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
  Done - 121217 disp.SetBackgroundColour(Black) clears screen
  Changes:


  ******************************************************************************************************
*/


#include <SPI.h>

#include "BackpackV4 _Board_Definitions.h"
#include "I2CDisplay_Definitions.h"

#include <ILI9341_due.h>                 //https://github.com/marekburiak/ILI9341_due
#include <ILI9341_due_config.h>

ILI9341_due disp = ILI9341_due(DISP_CS, DISP_DC, DISP_RESET);
#include "fonts\SystemFont5x7.h"

#define Serial_Monitor_Baud 115200                   //this is baud rate used for the Arduino IDE Serial Monitor

#define default_displayrotation iliRotation0         //default rotation iliRotation0,iliRotation90,iliRotation180,iliRotation270   
#define default_fontstyle 1                          //default font style - not yet implemented
#define default_backgroundcolour ILI9341_BLACK       //default background colour
#define default_textcolour ILI9341_WHITE             //default textcolour 
#define default_textscale 3                          //default text size


byte font;
unsigned int textcolour;
unsigned int backgroundcolour;
byte rotation;


#include <Wire.h>

#define Backpack_Address 0x08
#define DEBUG                              //define this to see debug on serial terminal

byte receive_buffer[128];                  //plenty of spare memory
byte receive_buffer_ptr;
boolean buffer_ready = false;

byte textscale = 1;                      //used to keep track of current text scale 1,2,3 or 4
unsigned long startmS, timemS;

void loop()
{
  if (buffer_ready)
  {
    process_Buffer();
  }
}


void cursorToXY(byte lcol, byte lrow)
{
  disp.cursorToXY((lcol * 6 * textscale), (lrow * 10 * textscale));
}


void setTextScale(byte lscale)
{
  disp.setTextScale(lscale);
  textscale = lscale;
  disp.setTextLineSpacing(lscale);
  disp.setTextLetterSpacing(1);
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
      process_SetContrast();
      break;

    case FontScale:
      process_SetFontScale();
      break;

    case InitialiseDisplay:
      process_InitialiseDisplay();
      break;

    case TextColour:
      process_SetTextColour();
      break;

    case BackgroundColour:
      process_SetBackgroundColour();
      break;

    case Test:
      process_Test();
      break;

    case DisplayRotation:
      process_DisplayRotation();
      break;


    default:
      Serial.print(F(",Command Error "));
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


void process_StartText()                             // process text in the buffer
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
  unsigned int lcol, lrow;
  lcol = receive_buffer[1];
  lrow = receive_buffer[2];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lcol);
  Serial.print(F(","));
  Serial.print(lrow);
#endif

  disp.cursorToXY((lcol * 6 * textscale), (lrow * 10 * textscale));
}


void process_ClearDisplay()
{
  disp.fillScreen(backgroundcolour);
}


void process_SetContrast()
{
  //not required
}


void process_SetFontScale()
{
  byte lfontscale = receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lfontscale);
#endif

  disp.setTextScale(lfontscale);
  textscale = lfontscale;
  disp.setTextLineSpacing(lfontscale);
  disp.setTextLetterSpacing(1);
}

void process_DisplayRotation()
{
  byte lrotation = (byte) receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lrotation);
#endif

  //disp.setRotation(lrotation);
  rotation = lrotation;

  switch (lrotation)
  {
    case 0:
      disp.setRotation(iliRotation0);
      break;

    case 1:
      disp.setRotation(iliRotation90);
      break;

    case 2:
      disp.setRotation(iliRotation180);
      break;

    case 3:
      disp.setRotation(iliRotation270);
      break;


    default:
      Serial.print(F(",Rotation "));
      Serial.println(lrotation, HEX);
  }
}




void process_SetTextColour()
{
  byte ltextcolour = receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(ltextcolour);
#endif


  switch (ltextcolour)
  {

    case 0:
      disp.setTextColor(ILI9341_BLACK, backgroundcolour);
      break;

    case 1:
      disp.setTextColor(ILI9341_BLUE, backgroundcolour);
      break;

    case 2:
      disp.setTextColor(ILI9341_RED, backgroundcolour);
      break;

    case 3:
      disp.setTextColor(ILI9341_MAGENTA, backgroundcolour);
      break;

    case 4:
      disp.setTextColor(ILI9341_GREEN, backgroundcolour);
      break;

    case 5:
      disp.setTextColor(ILI9341_CYAN, backgroundcolour);
      break;

    case 6:
      disp.setTextColor(ILI9341_YELLOW, backgroundcolour);
      break;

    case 7:
      disp.setTextColor(ILI9341_WHITE, backgroundcolour);
      break;


    default:
      Serial.print(F(",Colour Error "));
      Serial.print(ltextcolour);
      break;

  }
  textcolour = ltextcolour;
}


void process_SetBackgroundColour()
{
  unsigned int lbackgroundcolour = receive_buffer[1];

#ifdef DEBUG
  Serial.print(F(","));
  Serial.print(lbackgroundcolour);
#endif


  switch (lbackgroundcolour)
  {

    case 0:
      backgroundcolour = ILI9341_BLACK;
      break;

    case 1:
      backgroundcolour = ILI9341_BLUE;
      break;

    case 2:
      backgroundcolour = ILI9341_RED;
      break;

    case 3:
      backgroundcolour = ILI9341_MAGENTA;
      break;

    case 4:
      backgroundcolour = ILI9341_GREEN;
      break;

    case 5:
      backgroundcolour = ILI9341_CYAN;
      break;

    case 6:
      backgroundcolour = ILI9341_YELLOW;
      break;

    case 7:
      backgroundcolour = ILI9341_WHITE;
      break;


    default:
      Serial.print(F(",Colour Error "));
      Serial.print(lbackgroundcolour);
      break;
  }

}


void receiveEvent(int howmany)
{
  buffer_ready = false;
  receive_buffer_ptr = 0;

  while (Wire.available())
  {
    char c = Wire.read();
    receive_buffer[receive_buffer_ptr] = c;
    receive_buffer_ptr++;
  }

  receive_buffer[receive_buffer_ptr] = (byte) howmany;
  receive_buffer_ptr--;

  buffer_ready = true;
}


void return_Busy_Status()
{
  // this interrupt routine processes the request for returning the busy flag to master

  if (buffer_ready)
  {
    Wire.write(1);
  }
  else
  {
    Wire.write(0);
  }

}


void startText()
{
  disp.fillScreen(default_backgroundcolour);
  disp.cursorToXY(0, 0);
  disp.setTextColor(default_textcolour);
  disp.setTextScale(default_textscale);
  disp.setRotation(default_displayrotation);
  disp.println("I2C Backpack");
  disp.setTextScale(2);
  disp.println("www.LoraTracker.uk");
  disp.println();
  disp.println("V1.0 December 2017");
  disp.println();
}


void process_InitialiseDisplay()
{
  //this sets the display back to defaults
#ifdef DEBUG
  Serial.println(F("InitialiseDisplay"));
#endif
  disp.setFont(SystemFont5x7);
  disp.fillScreen(default_backgroundcolour);
  disp.setRotation(default_displayrotation);
  disp.setTextColor(default_textcolour);
  disp.setTextLetterSpacing(1);
  disp.cursorToXY(0, 0);
  disp.setTextScale(default_textscale);
  disp.setTextLetterSpacing(1);
  backgroundcolour = default_backgroundcolour;
  textscale = default_textscale;
  textcolour = default_textcolour;
  rotation = default_displayrotation;

  disp.println("Ready");
}



void process_Test()
{
  //a standard display routine test.
  unsigned long startmS = 0, texttimemS = 0, cleartimemS = 0;
  Serial.println(F("display_Test()"));

  startmS = millis();
  disp.fillScreen(backgroundcolour);
  cleartimemS = millis() - startmS;

  startmS = millis();
  setTextScale(default_textscale);
  cursorToXY(0, 0);

  disp.println(F("ILI9341     "));    //padded to 12 characters
  cursorToXY(0, 1);
  disp.print("Ready");
  texttimemS = millis() - startmS;
  cursorToXY(0, 2);

  disp.print(F("Text  "));
  disp.print(texttimemS);
  disp.print(F("mS"));

  Serial.print(F("Text "));
  Serial.print(texttimemS);
  Serial.println(F("mS"));

  cursorToXY(0, 3);

  disp.print(F("Clear "));
  disp.print(cleartimemS);
  disp.print(F("mS"));

  Serial.print(F("Clear "));
  Serial.print(cleartimemS);
  Serial.println(F("mS"));
}


void setup()
{
  Serial.begin(Serial_Monitor_Baud);
  Serial.println(F("Starting ILI9341 Display"));
  Serial.println(F("Stuart Robinson - December 2017"));

  pinMode(DISP_RESET, OUTPUT);             //setup pin for display reset
  digitalWrite(DISP_RESET, HIGH);

  disp.begin();
  process_InitialiseDisplay();
  startText();
  delay(1000);
  process_InitialiseDisplay();


  Wire.begin(Backpack_Address);            //connect to i2c bus, address of Backpack
  Wire.onReceive(receiveEvent);            //this interrupt routine processes incoming characters
  Wire.onRequest(return_Busy_Status);      //this interrupt routine processes the request for return of busy flag from master
  Serial.println(F("Ready"));
}



