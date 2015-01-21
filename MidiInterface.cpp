/*!
 *  @file       MidiInterface.cpp
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */


#include "MidiInterface.h"

namespace midi
{    
  MidiInterface::MidiInterface()
  {
    this->myConnection = new MidiConnection(CHANNEL);
    this->myController = new FootController();
    
    myProgram = 0;
    myBank = 0; 
      
    myTempo = 60; 
    myPreviousTempoPulse = millis();
  }
  
  MidiInterface::~MidiInterface()
  {
    delete this->myConnection;
    delete this->myController;
  }
  
  //TODO
  boolean MidiInterface::test()
  {
    this->myConnection->sendProgramChange(myProgram);
    delay(1000);
  
    this->myProgram = (++myProgram)%128;
    
    while(this->myConnection->checkBuffer())
    {
      if(this->myConnection->getMsg()->getType() == PROGRAM_CHANGE)
      {
        this->myController->displayProgramNo(this->myConnection->getMsg()->getData().front());
      }
    }
  }

  /* Clears tempo "!" mark if half way through pulses, 
  in order to show as 50/50 on/off pulse */
  void MidiInterface::pulseTempo()
  {
    unsigned long currentMillis = millis();
    if (currentMillis > (myPreviousTempoPulse+myTempo/2))
    {
      this->myController->displayTempoPulse(false);
    }
  }

  void MidiInterface::handleProgramChange(byte program)
  {
      myProgram = (this->myBank*128) + program; //Calculate "extended" program number by taking into account the bank
  }

  boolean MidiInterface::handleControlChange(byte controller, byte value)
  {
    if(controller = 0) //Bank change
    {
      this->myBank = value;
      return true;
    }
    return false;
  }

  boolean MidiInterface::handleSysEx(vector<byte> data)
  {
    //TODO
    return false;
  }
}
