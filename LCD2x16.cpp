// ----------------------------------------------------------------------------
// LCD2x16.h
//
// Provides an SPI based interface to the TLE7232 eight channel
// low-side power switch for enhanced relay control
//
// Author: Steve Sawtelle
// ----------------------------------------------------------------------------

// ---- VERSIONS  ----
// 20150429 sws
// - remove atomic stuff to be compatible with MAX32, use Interrupts

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include "CS.h"
#include "LCD2x16.h"
#include "BCSIII.h"


//---------- constructor ----------------------------------------------------

LCD2x16::LCD2x16(void)
{
}

//------------------ private -----------------------------------------------


void LCD2x16::sendCmd(void)
{
    uint8_t * cp;

    cp = cmdbuf;

    noInterrupts(); //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
       SPI.setDataMode(SPI_MODE3);  // set proper mode, clk idle low, falling edge = 0 => mode = 0
       SPI.setClockDivider(400); // 100khz max on this chip
       CS.SPIselect(_csPin);

       SPI.transfer(CMD_PREFIX);
       while( *cp != '\0')
       {
          SPI.transfer(*cp++);
          delayMicroseconds(100);
       }
       CS.SPIselect(NO_CS);
      SPI.setClockDivider(4); // back to high speed

     interrupts();
}


//---------- public ----------------------------------------------------


// ----------------------------------------------------------------------------
// LCD2x16::init
//
// ----------------------------------------------------------------------------
uint16_t LCD2x16::begin(void)
{


    uint16_t ain = 0;
    uint16_t sense_lo = LCD2x16_ID * 32 - 8;
    uint16_t sense_hi = LCD2x16_ID * 32 + 8;


      for(  _csPin = 1;  _csPin < 7;  _csPin++)
      {
          CS.SPIselect( _csPin);
          delay(10);  // let settle a bit

          ain =  analogRead(ID_SENSE_PIN);
          if( (ain > sense_lo) && (ain < sense_hi) )  break;
          //uint16_t ID_val = (uint16_t) ( ( (uint32_t)(2500) * ain ) / 1023 );

          //if( (ID_val > (LCD2x16_ID - ID_SENSE_RANGE)) && (ID_val < (LCD2x16_ID + ID_SENSE_RANGE)) ) break;
      }
      CS.SPIselect(NO_CS);
      if( _csPin > 6 ) _csPin = NO_CS;


      return ain;
}

void LCD2x16::wrbyte(char c)
{

 noInterrupts(); //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
 {
    SPI.setDataMode(SPI_MODE3);  // set proper mode
    SPI.setClockDivider(400); // 100khz max on this chip
    CS.SPIselect(_csPin);
    SPI.transfer(c);
  }
  CS.SPIselect(NO_CS);
  SPI.setClockDivider(4); // back to high speed
  interrupts();
}

void LCD2x16::write(char * cp) {

 noInterrupts(); //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
 {
    SPI.setDataMode(SPI_MODE3);  // set proper mode
    SPI.setClockDivider(400); // 100khz max on this chip
    CS.SPIselect(_csPin);

    while( *cp != '\0')
    {
        SPI.transfer(*cp++);
        delayMicroseconds(100);
    }

  }
  CS.SPIselect(NO_CS);
  SPI.setClockDivider(4); // back to high speed
  interrupts();
}




void LCD2x16::cls(void)
{
    cmdbuf[0] = CLS_CMD;
    cmdbuf[1] = '\0';
    sendCmd();

}

void LCD2x16::setPosition(uint8_t row, uint8_t col)
{
    cmdbuf[0] = POSITION_CMD;
    cmdbuf[1] = row * 0x40 + col;
    cmdbuf[2] = '\0';
    sendCmd();
}

void LCD2x16::setBacklight(uint8_t level)
{
    if( level <= 8)
    {
        cmdbuf[0] = BACKLIGHT_CMD;
        cmdbuf[1] = level;
        cmdbuf[2] = '\0';
        sendCmd();
     }
}

uint8_t LCD2x16::getID(void)
{
    return(_csPin);
}


