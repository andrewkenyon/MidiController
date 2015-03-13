// 
// 
// 

#include "SysExMessage.h"

namespace midi
{

	SystemMessage::SystemMessage(uint8_t subType) : MidiMessage::MidiMessage(SYSTEM_MESSAGE)
	{
		this->mySubType = subType;
	}

	SystemMessage::~SystemMessage()
	{
	}

	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool SystemMessage::addData(byte newData)
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


}