/*!
 *  @file       FootSwitchHandler.cpp
 *  @brief      Handles presses of the footswitches
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */

#include "FootSwitchHandler.h"

using namespace std;

namespace midi
{    
	FootSwitchHandler::FootSwitchHandler(MidiInterface* interface, AxeController* controller)
	{
		this->myInterface = interface;
		this->myController = controller;
	}

	/********************************** Footswitch Input handling *************************************/
  
	vector<uint8_t> FootSwitchHandler::handleFootswitches(const vector<int16_t>& presses)
	{
		vector<uint8_t> ledStates = vector<uint8_t>(SWITCHES);
		for(uint8_t i=0; i < presses.size(); i++)
		{
		  ledStates.at(i) = this->handlePress(presses.at(i));
		}
		return ledStates;
	}
  
	uint8_t FootSwitchHandler::handlePress(int16_t press)
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
