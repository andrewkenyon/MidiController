/*!
 *  @file       MidiInterface.cpp
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */

#include "MidiInterface.h"

using namespace std;

namespace midi
{    
  MidiInterface::MidiInterface(int8_t channel)
  {
	this->myConnection = new MidiConnection();
	this->myController = new FootController(this);
	this->myChannel = channel;
    
    this->myProgram = 0;
    this->myBank = 0; //Force bank change on first program change.
      
    this->myTempo = 60; 
    this->myPreviousTempoPulse = millis();
  }
  
  MidiInterface::MidiInterface()
  {
	this->myConnection = new MidiConnection();
	this->myController = new FootController(this);
	  this->myChannel = -1; //Listen to all

	  this->myProgram = 0;
	  this->myBank = 0; //Force bank change on first program change.

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
		this->myConnection->sendProgramChange(pc);
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
		this->handleMsg(msg);
    }
    
    //Foot controller input
    //this->myController->updateLeds(this->handleFootswitches(this->myController->updateFootswitches()));
    
    //MIDI output
    
    //Foot controller output
    this->pulseTempo();
  }
  
  int MidiInterface::getProgram()
  {
    return this->myProgram;
  }
  
  int MidiInterface::getBank()
  {
    return this->myBank;
  }
  
  /* Process 16-bit unsigned number transferred as 3 septets.
  sept0 is the bottom 7, sept1 the next 7, and sept2 is the top 2. */
  uint16_t MidiInterface::processSeptets(uint8_t sept0, uint8_t sept1, uint8_t sept2)
  {
  	return sept0 + ((uint16_t)sept1 << 7) + ((uint16_t)sept2 << 14);
  }
  
    /* Process 16-bit unsigned number transferred as 3 septets.
  sept0 is the bottom 7, sept1 the next 7, and sept2 is the top 2. */
  uint16_t MidiInterface::processSeptets(uint8_t sept0, uint8_t sept1)
  {
  	return sept0 + ((uint16_t)sept1 << 7);
  }

  void MidiInterface::encodeForSysEx(uint16_t num, uint8_t bytes, SysExMessage* msg)
  {
	  for(int i=0; i < bytes; i++)
	  {
		  msg->addData((num >> (i*7)) & 0x7F);
	  }
  }
  
  String MidiInterface::extractString(const vector<uint8_t>& data, uint8_t start)
  {
    String text = "";
    uint8_t i = start;
    while(data.at(i) != 0x00) //Stop at null character
    {
      text += data.at(i); //Concatenate character to string
      i++;
    }
    return text;
  }
  
  /***********************************************************/
  
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
      this->myConnection->sendControlChange(cc);
    }
	ProgramChangeMessage pc(this->myChannel);
	pc.addData((uint8_t)(extProgram % 128));
    this->myConnection->sendProgramChange(pc);
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

	this->myConnection->sendSysEx(sysEx);
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

		this->myConnection->sendSysEx(sysEx);
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
		
		this->myConnection->sendSysEx(sysEx);
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
		
		this->myConnection->sendSysEx(sysEx);
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
		
		this->myConnection->sendSysEx(sysEx);
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

	this->myConnection->sendSysEx(sysEx);
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

		this->myConnection->sendSysEx(sysEx);
	}
  
    void MidiInterface::setSceneNumber(uint8_t scene)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_SET_SCENE_NUMBER); // Function ID
	  
		sysEx.addData(scene);
		  
		generateChecksum(&sysEx);

		this->myConnection->sendSysEx(sysEx);
	}
  
  /*************************************************************************************************/
  
  /* Handle Messages */
  
  bool MidiInterface::handleMsg(const MidiMessage& msg)
  {
	  switch (msg.getType())
	  {
		case PROGRAM_CHANGE:
		{
			if ( (( ((ProgramChangeMessage&)msg).getChannel() ) == this->myChannel) || (this->myChannel == -1) )
			{
				return this->handleProgramChange((ProgramChangeMessage&)msg);
			}
			else
			{
				return false;
			}
		}
		case CONTROL_CHANGE:
		{
			if ( (( ((ControlChangeMessage&)msg).getChannel()) == this->myChannel) || (this->myChannel == -1) )
			{
				return this->handleControlChange((ControlChangeMessage&)msg);
			}
			else
			{
				return false;
			}
		}
		case SYSTEM_MESSAGE:
		{
			return this->handleSysEx((SysExMessage&)msg);
		}
		default:
		{
		return false;
		}
	}
  }

  bool MidiInterface::handleProgramChange(const ProgramChangeMessage& msg)
  {
      this->myProgram = (uint16_t)((uint16_t)this->myBank*(uint16_t)128 + (uint16_t)msg.getProgram()); //Calculate "extended" program number by taking into account the bank
      this->myController->displayProgramNumber(this->myProgram);
      return true;
  }

  bool MidiInterface::handleControlChange(const ControlChangeMessage& msg)
  {
    if(msg.getControllerNumber() == (uint8_t)CC_BANKMSB) //Bank change
    {
      this->myBank = msg.getControllerValue();
      return true;
    }
	else
	{
		return false;
	}
  }

  bool MidiInterface::handleSysEx(const SysExMessage& msg)
  {
	  const vector<uint8_t>& data = msg.getData();
    //Check it has at least the bare minimum to be a meaningful SysEx message
    if(data.size() < 5)
    {
      return false;
    }
    
    //Check header matches
    if( (data.at(0) != MAN_ID1)
    || (data.at(1) != MAN_ID2) 
    || (data.at(2) != MAN_ID3)
    || (data.at(3) != MODEL_ID) )
    {
      return false;
    }
    
    switch(data.at(4) /*Function ID*/ )
    {
      case MIDI_PARAMETER:                                         return this->handleParameter(data);
      case MIDI_MODIFIER:                                          return this->handleModifier(data); //Wiki says response to modifier is actually 0x02 (MIDI parameter), but I am assuming this is a mistake.
      case MIDI_GET_FIRMWARE_VERSION:                              return this->handleFirmwareVersion(data);
      case MIDI_TUNER_INFO:                                        return this->handleTunerInfo(data);
      case MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE:  return this->handlePresetState(data);
      case MIDI_GET_PRESET_NAME:                                   return this->handlePresetName(data);
      case MIDI_TEMPO_BEAT:                                        return this->handleTempoBeat();
      case MIDI_GET_ROUTING_GRID_LAYOUT:                           return this->handleGridRouting(data);
      case MIDI_LOOPER_STATUS:                                     return this->handleLooperStatus(data);
      case MIDI_SET_SCENE_NUMBER:                                  return this->handleSceneNumber(data);
      default:                                                     return false;
    }
  }
  
  bool MidiInterface::handleParameter(const vector<uint8_t>& data)
  {
    uint16_t effect = processSeptets(data.at(5), data.at(6));
    uint16_t parameter = processSeptets(data.at(7), data.at(8));
    uint16_t value = processSeptets(data.at(9), data.at(10), data.at(11));    
    
    //Not sure what bytes 12-16 do!!!
    
    String text = extractString(data, 17);
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleModifier(const vector<uint8_t>& data)
  {
    uint16_t effect = processSeptets(data.at(5), data.at(6));
    uint16_t parameter = processSeptets(data.at(7), data.at(8));
    uint8_t selector = data.at(9);
    // byte 10 is apparently always 0
    uint16_t value = processSeptets(data.at(11), data.at(12), data.at(13));    
    
    String text = extractString(data, 14);
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleFirmwareVersion(const vector<uint8_t>& data)
  {
    uint16_t version = processSeptets(data.at(5), data.at(6));

    // bytes 7-10 apparently are unimportant
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleTunerInfo(const vector<uint8_t>& data)
  {
    if(data.size() >= 8)
    {
      this->myController->displayTuner(NOTE[data.at(5)], data.at(6), data.at(7));
      return true; //No checksum
    }
    return false;
  }
  
  bool MidiInterface::handlePresetState(const vector<uint8_t>& data)
  {
    uint16_t i=5;
    while(i < (data.size()-1)) //For the whole data packet, except the checksum
    {
      //For each block
      
      //Status bits
      bool enabled = data.at(i) & 0x01;
      bool yState = data.at(i) & 0x02;

      uint8_t ccNumber = (data.at(i+1) >> 1) + (data.at(i+2) << 6);
      
      uint8_t effectId = (data.at(i+3) >> 3) + (data.at(i+4) << 4);
      
      i+=5;
    }
    // TODO Do something with this info
    return validateChecksum(data);
  }
  
  bool MidiInterface::handlePresetName(const vector<uint8_t>& data)
  {
    String presetName = extractString(data,5);
    if(validateChecksum(data))
    {
      this->myController->displayProgramName(presetName);
      return true;
    }
    else
    {
      return false;
    }    
  }
  
  bool MidiInterface::handleTempoBeat()
  {    
    this->myController->displayTempoPulse(true);   
    unsigned long currentTime = millis();
    this->myTempo = (myPreviousTempoPulse - currentTime)/60;
    this->myPreviousTempoPulse = currentTime;
    this->myController->displayBpm(myTempo);
    //No checksum
    return true;
  }
  
  bool MidiInterface::handlePresetNumber(const vector<uint8_t>& data)
  {
    if(validateChecksum(data))
    {
      this->myController->displayProgramNumber(processSeptets(data.at(5), data.at(6)));
      return true;
    }
    else
    {
      return false;
    }
  }
  
  bool MidiInterface::handleGridRouting(const vector<uint8_t>& data)
  {
    uint8_t effects[GRID_COLUMNS][GRID_ROWS];
    bool routing[GRID_COLUMNS][GRID_ROWS][GRID_ROWS];
 
    for(uint8_t column=0; column < GRID_COLUMNS; column++)
    {
      for(uint8_t row=0; row < GRID_ROWS; row++)
      {
        effects[column][row] = processSeptets(data.at(5+(column*GRID_ROWS)+row),data.at(6+(column*GRID_ROWS)+row));
        
        for(uint8_t input=0; input < GRID_ROWS; input++)
        {
          routing[column][row][input] = (0x01 << input) & data.at(7+(column*GRID_ROWS)+row);
        }
      }
    }
    
    //TODO: Do something with this information
    return validateChecksum(data);
  }
  
      
  bool MidiInterface::handleLooperStatus(const vector<uint8_t>& data)
  {
    bool record = 0x01 & data.at(5); //Bit 0: Record
    bool play = 0x02 & data.at(5); //Bit 1: Play
    bool once = 0x04 & data.at(5); //Bit 2: Once
    bool overdub = 0x08 & data.at(5); //Bit 3: Overdub
    bool reverse = 0x10 & data.at(5); //Bit 4: Reverse
    bool half = 0x20 & data.at(5); //Bit 5: Half
    bool undo = 0x40 & data.at(5); //Bit 6: Undo
    
    uint8_t range = data.at(6);

    //TODO Do something with info
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleSceneNumber(const vector<uint8_t>& data)
  {
    uint8_t scene = data.at(5);

    //TODO Do something with info
    
    return validateChecksum(data);
  }
  
  /*************************************************************************************************/
    
  /* Not all functions have checksums */
	bool MidiInterface::validateChecksum(const vector<uint8_t>& data)
	{
		uint8_t checksum = 0xF0;
		for(uint8_t i=0; i < data.size()-1; i++)
		{
			checksum ^= data.at(i);
		}
		return (checksum == data.back());
	}  
  
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
