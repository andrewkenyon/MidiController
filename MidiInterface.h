/*!
 *  @file       MidiInterface.h
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */

#include "MidiConnection.h"
#include "FootController.h"

#include <Arduino.h>

#define CHANNEL 0

namespace midi
{
  class MidiInterface
  {
    private:
      MidiConnection* myConnection;
      FootController* myController;
      
      int myProgram;
      int myBank; //The midi bank (CC #0) currently used.
      
      int myTempo; //Tempo in bpm
      unsigned long myPreviousTempoPulse;
      
    public:
      MidiInterface();
      ~MidiInterface();
      void test(int testInt);
      void update();

    private:
      void updateTempo();
      void pulseTempo();

      void handleMsg(MidiMessage* msg);
      void handleProgramChange(byte program);
      boolean handleControlChange(byte controller, byte value);
      boolean handleSysEx(vector<byte> data);
  };
}
    


