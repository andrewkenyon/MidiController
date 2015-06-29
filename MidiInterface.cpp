/*!
 *  @file       MidiInterface.cpp
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       29/06/2015
 */

#include "MidiInterface.h"
#include "AxeController.h"
#include "MessageHandler.h"

using namespace std;

namespace midi
{    
	MidiInterface::MidiInterface(int8_t channel)
	{
		this->myConnection = new MidiConnection();
		this->myController = new AxeController(this);
		this->myChannel = channel;
		
		this->myProgram = 0;
		this->myBank = 0;
		  
		this->myTempo = 60; 
		this->myPreviousTempoPulse = millis();
	}
  
	MidiInterface::MidiInterface()
	{
		this->myConnection = new MidiConnection();
		this->myController = new AxeController(this);
		this->myChannel = -1; //Listen to all

		this->myProgram = 0;
		this->myBank = 0;

		this->myTempo = 60;
		this->myPreviousTempoPulse = millis();
	}

	MidiInterface::~MidiInterface()
	{
		delete this->myConnection;
		delete this->myController;
	}
  
	//TODO
	void MidiInterface::test(uint16_t testInt)
	{
		this->sendExtendedProgramChange(testInt);
		/*ProgramChangeMessage pc((uint8_t)0);
		if(pc.addData((uint8_t) testInt%128))
		{
			this->myConnection->sendMessage(pc);
		}*/
		delay(100);
		
		this->update();
	}
  
	void MidiInterface::update()
	{
		//MIDI input
		while(this->myConnection->checkBuffer())
		{
			const MidiMessage& msg = this->myConnection->getMsg();
			MessageHandler handler(this, this->myController);
			handler.handleMsg(msg);
		}
    
		//Foot controller input
		//this->myController->updateLeds(this->handleFootswitches(this->myController->updateFootswitches()));
		
		//MIDI output
		
		//Foot controller output
		this->pulseTempo();
	}
  
	uint16_t MidiInterface::getPreset()
	{
		return (uint16_t)((uint16_t)this->myBank*(uint16_t)128 + (uint16_t)this->myProgram);
	}

	int8_t MidiInterface::getChannel()
	{
		return this->myChannel;
	}
	
	/* Sets preset from program change message. This will be a number from 0-127, but along with the bank is a 14-bit number */
	bool MidiInterface::setProgram(uint8_t programChange)
	{
		if(programChange < 128)
		{
			this->myProgram = programChange;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/* Sets bank from control change message.*/
	bool MidiInterface::setBank(uint8_t bankChange)
	{
		if(bankChange < 128)
		{
			this->myBank = bankChange;
			return true;
		}
		else
		{
			return false;
		}
	}

	void MidiInterface::encodeForSysEx(uint16_t num, uint8_t bytes, SysExMessage* msg)
	{
		for(int i=0; i < bytes; i++)
		{
			msg->addData((num >> (i*7)) & 0x7F);
		}
	}

	/***********************************************************/
	
	void MidiInterface::updateTempo()
	{
		this->myController->displayTempoPulse(true);   
		unsigned long currentTime = millis();
		this->myTempo = (myPreviousTempoPulse - currentTime)/60;
		this->myPreviousTempoPulse = currentTime;
		this->myController->displayBpm(myTempo);

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
		else
		{
			this->myController->displayTempoPulse(true);
		}
	}
  
      
  /* Work out the bank and PC commands necessary for a Program larger than 127.
  Try to always use this for user I/O, so as to shield user from bank number complications. */
  void MidiInterface::sendExtendedProgramChange(uint16_t extProgram)
  {
    if(this->myBank != (uint8_t)(extProgram/(uint16_t)128))
    {
      this->myBank = (uint8_t)(extProgram/128);
	  ControlChangeMessage cc(this->myChannel);
	  cc.addData((uint8_t)CC_BANKMSB);
	  cc.addData(this->myBank);
      this->myConnection->sendMessage(cc);
    }
	ProgramChangeMessage pc(this->myChannel);
	pc.addData((uint8_t)(extProgram % 128));
    this->myConnection->sendMessage(pc);
  }
  
  /*************************************************************************************************/
      
  /* SysEx get and set message generation */
  
  void MidiInterface::generateHeader(SysExMessage* msg)
  {  
    msg->addData(MAN_ID1);
    msg->addData(MAN_ID2);
    msg->addData(MAN_ID3);
    msg->addData(MODEL_ID);
  }    
  
  void MidiInterface::getFunction(uint8_t function)
  {
	SysExMessage sysEx;
	generateHeader(&sysEx);
    
    sysEx.addData(function); // Function ID
    
    generateChecksum(&sysEx);

	this->myConnection->sendMessage(sysEx);
  }
  
	void MidiInterface::getParameter(const uint16_t& effect, const uint16_t& parameter)
	{	
	  
		SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_PARAMETER); // Function ID

		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell

		sysEx.addData(0x00); // 0 = query value
    
		generateChecksum(&sysEx);

		this->myConnection->sendMessage(sysEx);
	}
  
	void MidiInterface::setParameter(const uint16_t& effect, const uint16_t& parameter, const uint16_t& value)
	{	
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_PARAMETER); // Function ID
	
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		encodeForSysEx(value, 3, &sysEx);				
    
		sysEx.addData(0x01); // 1 = set value
    
		generateChecksum(&sysEx);
		
		this->myConnection->sendMessage(sysEx);
	}
  
	void MidiInterface::getModifier(uint16_t effect, uint16_t parameter, uint8_t selector)
	{
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_MODIFIER); // Function ID
    
		vector<uint8_t> data;
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		
		sysEx.addData(selector);
		sysEx.addData(0);
		
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell
		
		sysEx.addData(0x00); // 0 = query value
		
		generateChecksum(&sysEx);
		
		this->myConnection->sendMessage(sysEx);
	}
  
	void MidiInterface::setModifier(uint16_t effect, uint16_t parameter, uint8_t selector, uint16_t value)
	{
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_MODIFIER); // Function ID
    
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		sysEx.addData(selector);
		sysEx.addData(value);
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell
		
		sysEx.addData(0x00); // 0 = query value
		
		generateChecksum(&sysEx);
		
		this->myConnection->sendMessage(sysEx);
	}
  
  void MidiInterface::getFirmwareVersion()
  {
    getFunction(MIDI_GET_FIRMWARE_VERSION);
  }
  
  void MidiInterface::getPresetState()
  {
    getFunction(MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE);
  }
  
  void MidiInterface::getPresetName()
  {
    getFunction(MIDI_GET_PRESET_NAME);
  }
  
  void MidiInterface::getPresetNumber()
  {
    getFunction(MIDI_PRESET_NUMBER);
  }
  
  /* Set preset number using SysEx.
  Wiki is not very clear on this message, so may be wrong. */
  void MidiInterface::setPresetNumber()
  {    
	SysExMessage sysEx;
    generateHeader(&sysEx);
    
    sysEx.addData(MIDI_PRESET_NUMBER);
    
    sysEx.addData(this->myBank);
    sysEx.addData(this->myProgram);
    
    sysEx.addData(0x13); // Seems to be arbritary byte!
    
    generateChecksum(&sysEx);

	this->myConnection->sendMessage(sysEx);
  }
  
  void MidiInterface::getGridRouting()
  {
    getFunction(MIDI_GET_ROUTING_GRID_LAYOUT);
  }
  
	void MidiInterface::enableLoopStatus(bool enable)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_LOOPER_STATUS); // Function ID

		if(enable)  
			sysEx.addData(0x01); // 1 = enable
		else
			sysEx.addData(0x00);
		  
		generateChecksum(&sysEx);

		this->myConnection->sendMessage(sysEx);
	}
  
    void MidiInterface::setSceneNumber(uint8_t scene)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_SET_SCENE_NUMBER); // Function ID
	  
		sysEx.addData(scene);
		  
		generateChecksum(&sysEx);

		this->myConnection->sendMessage(sysEx);
	}
  
  /*************************************************************************************************/

	void MidiInterface::generateChecksum(SysExMessage* msg)
	{
		uint8_t checksum = 0xF0;
		const vector<uint8_t>& data = msg->getData();
		for(uint8_t i=0; i < data.size(); i++)
		{
			checksum ^= data.at(i);
		}
		msg->addData(checksum);    
	}
  
  /***************************************************************************************************/
  
  /* Footswitch Input handling */
  
  vector<uint8_t> MidiInterface::handleFootswitches(const vector<int16_t>& presses)
  {
    vector<uint8_t> ledStates = vector<uint8_t>(SWITCHES);
    for(uint8_t i=0; i < presses.size(); i++)
    {
      ledStates.at(i) = this->handlePress(presses.at(i));
    }
    return ledStates;
  }
  
  uint8_t MidiInterface::handlePress(int16_t press)
  {
    //TODO do something other than turn on LED!
    if(press >= LONG_PRESS)
    {
      return 1;
    }
    else if(press >= 0) //Short press
    {
      return 1;
    }
    else //Not pressed
    {
      return 0;
    }
  }
}
