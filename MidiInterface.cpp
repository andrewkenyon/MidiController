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
  void MidiInterface::test(int testInt)
  {
    this->myConnection->sendExtendedProgramChange(testInt);
    delay(100);
    
    this->update();
  }
  
  void MidiInterface::update()
  {
    while(this->myConnection->checkBuffer())
    {
      this->handleMsg(this->myConnection->getMsg());
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
  
  void MidiInterface::handleMsg(MidiMessage* msg)
  {
    switch(this->myConnection->getMsg()->getType())
    {
      case PROGRAM_CHANGE:
      {
        this->handleProgramChange(this->myConnection->getMsg()->getData().front());
        break;
      }
      case CONTROL_CHANGE:
      {
        this->handleControlChange(this->myConnection->getMsg()->getData().front(),this->myConnection->getMsg()->getData().back());
        break;
      }
      default:
      {
        break;
      }
    }
  }

  void MidiInterface::handleProgramChange(byte program)
  {
      this->myProgram = (int)((int)this->myBank*(int)128 + (int)program); //Calculate "extended" program number by taking into account the bank
      this->myController->displayProgramNo(myProgram);
  }

  boolean MidiInterface::handleControlChange(byte controller, byte value)
  {
    if(controller == 0) //Bank change
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
