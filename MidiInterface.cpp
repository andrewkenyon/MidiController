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
    
    this->myProgram = 0;
    this->myBank = -1; //Force bank change on first program change.
      
    this->myTempo = 60; 
    this->myPreviousTempoPulse = millis();
  }
  
  MidiInterface::~MidiInterface()
  {
    delete this->myConnection;
    delete this->myController;
  }
  
  //TODO
  void MidiInterface::test(int testInt)
  {
    this->sendExtendedProgramChange(testInt);
    delay(100);
    
    this->update();
  }
  
  void MidiInterface::update()
  {
    //MIDI input
    while(this->myConnection->checkBuffer())
    {
      this->handleMsg(this->myConnection->getMsg());
    }
    
    //Foot controller input
    this->myController->updateLeds(this->handleFootswitches(this->myController->updateFootswitches()));
    
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
  
  /* Takes an unsigned 16-bit number and returns it as septets (MIDI uint8_ts).
  Allows size to be set to 2 to allow conversion of numbers less than 14-bit to appropriately sized vector */
  vector<uint8_t>* MidiInterface::getSeptetVector2(uint16_t num)
  {
  	vector<uint8_t>* bvector = new vector<uint8_t>(2);
  	addToSeptetVector2(num, bvector);
        return bvector;
  }
  
  /* Takes an unsigned 16-bit number and returns it as 3 (7/7/2) septets (MIDI uint8_ts). */
  vector<uint8_t>* MidiInterface::getSeptetVector3(uint16_t num)
  {
  	vector<uint8_t>* bvector = new vector<uint8_t>(3);
  	addToSeptetVector3(num, bvector);
        return bvector;
  }
  
  /* Adds an unsigned 14-bit number as 2 (7/7) septets (MIDI uint8_ts) to existing vector. */
  void MidiInterface::addToSeptetVector2(uint16_t num, vector<uint8_t>* bvector)
  {
  	bvector->push_back(0x007F & num);
  	bvector->push_back((0x3F80 & num) >> 7);
  }  
  
  /* Adds an unsigned 16-bit number as 3 (7/7/2) septets (MIDI uint8_ts) to existing vector. */
  void MidiInterface::addToSeptetVector3(uint16_t num, vector<uint8_t>* bvector)
  {
  	bvector->push_back(0x007F & num);
  	bvector->push_back((0x3F80 & num) >> 7);
  	bvector->push_back((0xC000 & num) >> 14);
  }
  
  String MidiInterface::extractString(vector<uint8_t>* data, uint8_t start)
  {
    String text = "";
    uint8_t i = start;
    while(data->at(i) != 0x00) //Stop at null character
    {
      text += data->at(i); //Concatenate character to string
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
    if(this->myBank != (int8_t)(extProgram/128))
    {
      this->myBank = (int8_t)(extProgram/128);
      this->myConnection->sendControlChange(CC_BANKMSB, this->myBank);
    }
    this->myConnection->sendProgramChange(extProgram%128);
  }
  
  /*************************************************************************************************/
      
  /* SysEx get and set message generation */
  
  vector<uint8_t> MidiInterface::generateHeader(int vectorSize)
  {
    vector<uint8_t> header = vector<uint8_t>(vectorSize);
    
    header.push_back(MAN_ID1);
    header.push_back(MAN_ID2);
    header.push_back(MAN_ID3);
    header.push_back(MODEL_ID);
    
    return header;
  }    
  
  void MidiInterface::getFunction(uint8_t function)
  {
    vector<uint8_t> data = generateHeader(6);
    
    data.push_back(function); // Function ID
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  void MidiInterface::getParameter(uint16_t effect, uint16_t parameter)
  {
    vector<uint8_t> data = generateHeader(14);
    
    data.push_back(MIDI_PARAMETER); // Function ID
    
    addToSeptetVector2(effect, &data);
    addToSeptetVector2(parameter, &data);
    addToSeptetVector3(0, &data); //Get still requires arbitrary value as far as I can tell
    
    data.push_back(0x00); // 0 = query value
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  void MidiInterface::setParameter(uint16_t effect, uint16_t parameter, uint16_t value)
  {
    vector<uint8_t> data = generateHeader(14);
    
    data.push_back(MIDI_PARAMETER); // Function ID
    
    addToSeptetVector2(effect, &data);
    addToSeptetVector2(parameter, &data);
    addToSeptetVector3(value, &data);
    
    data.push_back(0x01); // 1 = set value
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  void MidiInterface::getModifier(uint16_t effect, uint16_t parameter, uint8_t selector)
  {
    vector<uint8_t> data = generateHeader(16);
    
    data.push_back(MIDI_MODIFIER); // Function ID
    
    addToSeptetVector2(effect, &data);
    addToSeptetVector2(parameter, &data);
    
    data.push_back(selector);
    data.push_back(0);
    
    addToSeptetVector3(0, &data); //Get still requires arbitrary value as far as I can tell
    
    data.push_back(0x00); // 0 = query value
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  void MidiInterface::setModifier(uint16_t effect, uint16_t parameter, uint8_t selector, uint16_t value)
  {
    vector<uint8_t> data = generateHeader(16);
    
    data.push_back(MIDI_MODIFIER); // Function ID
    
    addToSeptetVector2(effect, &data);
    addToSeptetVector2(parameter, &data);
    
    data.push_back(selector);
    data.push_back(0);
    
    addToSeptetVector3(value, &data);
    
    data.push_back(0x01); // 1 = set value
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
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
    vector<uint8_t> data = generateHeader(9);
    
    data.push_back(MIDI_PRESET_NUMBER);
    
    data.push_back(this->myBank);
    data.push_back(this->myProgram);
    
    data.push_back(0x13); // Seems to be arbritary byte!
    
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  void MidiInterface::getGridRouting()
  {
    getFunction(MIDI_GET_ROUTING_GRID_LAYOUT);
  }
  
  void MidiInterface::enableLoopStatus(bool enable)
  {
    vector<uint8_t> data = generateHeader(7);
    
    data.push_back(MIDI_LOOPER_STATUS); // Function ID

    if(enable)  
      data.push_back(0x01); // 1 = enable
    else
      data.push_back(0x00);
      
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
    void MidiInterface::setSceneNumber(uint8_t scene)
  {
    vector<uint8_t> data = generateHeader(7);
    
    data.push_back(MIDI_SET_SCENE_NUMBER); // Function ID
  
    data.push_back(scene);
      
    data.push_back(generateChecksum(data));
    
    this->myConnection->sendSysEx(data);
  }
  
  /*************************************************************************************************/
  
  /* Handle Messages */
  
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
      case SYSTEM_EXCLUSIVE:
      {
        this->handleSysEx(this->myConnection->getMsg()->getData());
      }
      default:
      {
        break;
      }
    }
  }

  void MidiInterface::handleProgramChange(uint8_t program)
  {
      this->myProgram = (int)((int)this->myBank*(int)128 + (int)program); //Calculate "extended" program number by taking into account the bank
      this->myController->displayProgramNumber(myProgram);
  }

  bool MidiInterface::handleControlChange(uint8_t controller, uint8_t value)
  {
    if(controller == 0) //Bank change
    {
      this->myBank = value;
      return true;
    }
    return false;
  }

  bool MidiInterface::handleSysEx(vector<uint8_t> data)
  {
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
    
    switch(data.at(4) /*Function ID */)
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
  
  bool MidiInterface::handleParameter(vector<uint8_t> data)
  {
    uint16_t effect = processSeptets(data.at(5), data.at(6));
    uint16_t parameter = processSeptets(data.at(7), data.at(8));
    uint16_t value = processSeptets(data.at(9), data.at(10), data.at(11));    
    
    //Not sure what bytes 12-16 do!!!
    
    String text = extractString(&data, 17);
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleModifier(vector<uint8_t> data)
  {
    uint16_t effect = processSeptets(data.at(5), data.at(6));
    uint16_t parameter = processSeptets(data.at(7), data.at(8));
    uint8_t selector = data.at(9);
    // byte 10 is apparently always 0
    uint16_t value = processSeptets(data.at(11), data.at(12), data.at(13));    
    
    String text = extractString(&data, 14);
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleFirmwareVersion(vector<uint8_t> data)
  {
    uint16_t version = processSeptets(data.at(5), data.at(6));

    // bytes 7-10 apparently are unimportant
    
    // TODO: Do something with this information...
    
    return validateChecksum(data);
  }
  
  bool MidiInterface::handleTunerInfo(vector<uint8_t> data)
  {
    if(data.size() >= 8)
    {
      this->myController->displayTuner(NOTE[data.at(5)], data.at(6), data.at(7));
      return true; //No checksum
    }
    return false;
  }
  
  bool MidiInterface::handlePresetState(vector<uint8_t> data)
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
  
  bool MidiInterface::handlePresetName(vector<uint8_t> data)
  {
    String presetName = extractString(&data,5);
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
  
  bool MidiInterface::handlePresetNumber(vector<uint8_t> data)
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
  
  bool MidiInterface::handleGridRouting(vector<uint8_t> data)
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
  
      
  bool MidiInterface::handleLooperStatus(vector<uint8_t> data)
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
  
  bool MidiInterface::handleSceneNumber(vector<uint8_t> data)
  {
    uint8_t scene = data.at(5);

    //TODO Do something with info
    
    return validateChecksum(data);
  }
  
  /*************************************************************************************************/
    
  /* Not all functions have checksums */
 bool MidiInterface::validateChecksum(vector<uint8_t> data)
  {
    //Assume checksum is the last uint8_t
    uint8_t checksum = 0xF0;
    for(uint8_t i=0; i < data.size()-1; i++) //Obviously don't include the actual checksum!
    {
      checksum ^= data.at(i);
    }
    return (checksum == data.back());
  }  
  
  uint8_t MidiInterface::generateChecksum(vector<uint8_t> data)
  {
    uint8_t checksum = 0xF0;
    for(uint8_t i=0; i < data.size(); i++)
    {
      checksum ^= data.at(i);
    }
    return checksum;    
  }
  
  /***************************************************************************************************/
  
  /* Footswitch Input handling */
  
  vector<uint8_t> MidiInterface::handleFootswitches(vector<int16_t> presses)
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
