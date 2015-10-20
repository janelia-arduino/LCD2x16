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

#ifndef LCD2x16_H_
#define LCD2x16_H_
// #include <util/atomic.h>
#include "BCSIII.h"

#define LCD2x16_ID 4


class LCD2x16 {
 public:
  LCD2x16();


  uint16_t begin();
  void wrbyte(char c);
  void write(char * cp );
  void cls(void);
  void setPosition(uint8_t row, uint8_t col);
  void setBacklight(uint8_t level);
  uint8_t getID(void);

 private:
  void sendCmd(void);
  // Private Constants
  const static uint8_t CMD_PREFIX = 0xFE;
  const static uint8_t CLS_CMD = 0x51;
//  const static uint8_t HOME_CMD = 0x46;
  const static uint8_t POSITION_CMD = 0x45;
  const static uint8_t BACKLIGHT_CMD = 0x53;

  int _csPin;
  uint8_t cmdbuf[10];

};

#endif // LCD2x16_H_



