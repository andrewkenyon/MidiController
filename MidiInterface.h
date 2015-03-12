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

namespace midi
{
	#define CHANNEL 0

	#define GRID_COLUMNS 12
	#define GRID_ROWS 4

  class MidiInterface
  {
    private:
      MidiConnection* myConnection;
      FootController* myController;
      
      uint16_t myProgram;
      uint8_t myBank; //The midi bank (CC #0) currently used. //Allow negative values to represent unknown starting state
      
      int myTempo; //Tempo in bpm
      unsigned long myPreviousTempoPulse;
      
    public:
      MidiInterface();
      ~MidiInterface();
      
      void test(uint16_t testInt);
      void update();

      int getProgram();
      int getBank();
            
      static uint16_t processSeptets(uint8_t sept0, uint8_t sept1, uint8_t sept2);
      static uint16_t processSeptets(uint8_t sept0, uint8_t sept1);
	  static std::vector<uint8_t>* getSeptetVector2(uint16_t num);
	  static void addToSeptetVector2(uint16_t num, std::vector<uint8_t>* bvector);
	  static std::vector<uint8_t>* getSeptetVector3(uint16_t num);
	  static void addToSeptetVector3(uint16_t num, std::vector<uint8_t>* bvector);
      
	  static String extractString(std::vector<uint8_t>* data, uint8_t start);
      
    private:         
      void updateTempo();
      void pulseTempo();
      
      void sendExtendedProgramChange(uint16_t extProgram);
      
      /****************************************/
      
      /* SysEx get and set message generation */
	  std::vector<uint8_t> generateHeader(int vectorSize);
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
      bool handleMsg(MidiMessage* msg);
      bool handleProgramChange(uint8_t program);
      bool handleControlChange(uint8_t controller, uint8_t value);
	  bool handleSysEx(std::vector<uint8_t> data);
      
	  bool handleParameter(std::vector<uint8_t> data);
	  bool handleModifier(std::vector<uint8_t> data);
	  bool handleFirmwareVersion(std::vector<uint8_t> data);
	  bool handleTunerInfo(std::vector<uint8_t> data);
	  bool handlePresetState(std::vector<uint8_t> data);
	  bool handlePresetName(std::vector<uint8_t> data);
      bool handleTempoBeat();
	  bool handlePresetNumber(std::vector<uint8_t> data);
	  bool handleGridRouting(std::vector<uint8_t> data);
	  bool handleLooperStatus(std::vector<uint8_t> data);
	  bool handleSceneNumber(std::vector<uint8_t> data);
       
      /****************************************/
      
	  bool validateChecksum(std::vector<uint8_t> data);
	  uint8_t generateChecksum(std::vector<uint8_t> data);
      
      /****************************************/
      
	  std::vector<uint8_t> handleFootswitches(std::vector<int16_t> presses);
      uint8_t handlePress(int16_t press);
  };
}
    


