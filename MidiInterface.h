/*!
 *  @file       MidiInterface.h
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */
 
#pragma once

#include "FootController.h"
#include "MidiConnection.h"

#include <Arduino.h>

#define CHANNEL 0

#define GRID_COLUMNS 12
#define GRID_ROWS 4

//Button Types
#define SWITCH_PRESET_IA 0
#define SWITCH_BANK 1
#define SWITCH_REVEAL_LOOPER 2
#define SWITCH_TEMPO_TUNER 3

#define LONG_PRESS 1000

static const uint8_t SWITCH_FUNCTIONS[] =
{
  SWITCH_PRESET_IA,
  SWITCH_PRESET_IA,
  SWITCH_PRESET_IA,
  SWITCH_PRESET_IA,
  SWITCH_PRESET_IA,
  SWITCH_BANK,
  SWITCH_REVEAL_LOOPER,
  SWITCH_TEMPO_TUNER
};

namespace midi
{
  class MidiInterface
  {
    private:
      MidiConnection* myConnection;
      FootController* myController;
      
      //Allow negative values to represent unknown starting state
      int8_t myProgram;
      int8_t myBank; //The midi bank (CC #0) currently used.
      
      int myTempo; //Tempo in bpm
      unsigned long myPreviousTempoPulse;
      
    public:
      MidiInterface();
      ~MidiInterface();
      
      void test(int testInt);
      void update();

      int getProgram();
      int getBank();
            
      static uint16_t processSeptets(uint8_t sept0, uint8_t sept1, uint8_t sept2);
      static uint16_t processSeptets(uint8_t sept0, uint8_t sept1);
      static vector<uint8_t>* getSeptetVector2(uint16_t num);
      static void addToSeptetVector2(uint16_t num, vector<uint8_t>* bvector);
      static vector<uint8_t>* getSeptetVector3(uint16_t num);
      static void addToSeptetVector3(uint16_t num, vector<uint8_t>* bvector);
      
      static String extractString(vector<uint8_t>* data, uint8_t start);
      
    private:         
      void updateTempo();
      void pulseTempo();
      
      void sendExtendedProgramChange(uint16_t extProgram);
      
      /****************************************/
      
      /* SysEx get and set message generation */
      
      vector<uint8_t> generateHeader(int vectorSize);
      void getFunction(uint8_t function);
  
      void getParameter(uint16_t effect, uint16_t parameter);
      void setParameter(uint16_t effect, uint16_t parameter, uint16_t value);
      
      void getModifier(uint16_t effect, uint16_t parameter, uint8_t selector);
      void setModifier(uint16_t effect, uint16_t parameter, uint8_t selector, uint16_t value);
      
      void getFirmwareVersion();
      
      void getPresetState();
      
      void getPresetName();
      void getPresetNumber();
      void setPresetNumber();
      
      void getGridRouting();
      
      void enableLoopStatus(bool enable);
      
      void setSceneNumber(uint8_t scene);
      
      /****************************************/
      
      /* Handle Messages */
      void handleMsg(MidiMessage* msg);
      void handleProgramChange(uint8_t program);
      bool handleControlChange(uint8_t controller, uint8_t value);
      bool handleSysEx(vector<uint8_t> data);
      
      bool handleParameter(vector<uint8_t> data);
      bool handleModifier(vector<uint8_t> data);
      bool handleFirmwareVersion(vector<uint8_t> data);
      bool handleTunerInfo(vector<uint8_t> data);
      bool handlePresetState(vector<uint8_t> data);
      bool handlePresetName(vector<uint8_t> data);
      bool handleTempoBeat();
      bool handlePresetNumber(vector<uint8_t> data);
      bool handleGridRouting(vector<uint8_t> data);
      bool handleLooperStatus(vector<uint8_t> data);
      bool handleSceneNumber(vector<uint8_t> data);
       
      /****************************************/
      
      bool validateChecksum(vector<uint8_t> data);
      uint8_t generateChecksum(vector<uint8_t> data);
      
      /****************************************/
      
      vector<uint8_t> handleFootswitches(vector<int16_t> presses);
      uint8_t handlePress(int16_t press);
  };
}
    


