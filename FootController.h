/*!
 *  @file       FootController.h
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/01/2015
 */

#include <LiquidCrystal.h>
#include <Arduino.h>

#define LCD_COLUMNS 16
#define LCD_ROWS 2

namespace midi
{
  class FootController
  {
    private:
      LiquidCrystal* myLcd;
      
    public:
      FootController();
      ~FootController();

      void displayProgramNo(int ProgramNo);
      void displayProgramName(String ProgramName);

      int displayBpm(int tempo);
      void displayTempoPulse(boolean pulse);
  };
}
