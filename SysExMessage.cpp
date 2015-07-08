/*!
 *  @file       SysExMessage.cpp
 *  @brief      System Exclusive Message
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       08/07/2015
 */

#include "SysExMessage.h"

using namespace std;

namespace midi
{

	SysExMessage::SysExMessage() : MidiMessage::MidiMessage(SYSTEM_MESSAGE)
	{
	}
	SysExMessage::~SysExMessage()
	{
	}

	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool SysExMessage::addData(const uint8_t& newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
			if (newData == 0x7F)
			{
				this->myStatus = COMPLETE;
			}
			else
			{
				this->myData.push_back(newData);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	const int8_t& SysExMessage::payloadAt(const uint16_t& index) const
	{
		if(index < this->myData.size())
		{
			return this->myData.at(index);
		}
		else
		{
			return -1;
		}
	}
	
	const uint16_t& SysExMessage::payloadSize() const
	{
		return this->myData.size();
	}

}