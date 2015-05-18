// 
// 
// 

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
	bool SysExMessage::addData(uint8_t newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
			if(myManufacturer.size() < 1)
			{
				if ((newData == 0x7E) || (newData == 0x7F)) //General messages that we do not support
				{
					this->myStatus = INVALID;
					return false;
				}
			}
			else if(myManufacturer.size() < 3)
			{
				this->myManufacturer.push_back(newData);
			}
			else if (newData == 0x7F)
			{
				this->myStatus = COMPLETE;
			}
			else
			{
				this->myBody.push_back(newData);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool SysExMessage::checkManufacturer(uint8_t man0, uint8_t man1, uint8_t man2) const
	{
		if(this->myManufacturer.size() < 3)
		{
			return false;
		}
		else
		{
			return ( (man0 == this->myManufacturer.at(0))&&(man1 == this->myManufacturer.at(1))&&(man2 == this->myManufacturer.at(2)) );
		}
	}
	
	const vector<uint8_t>& SysExMessage::getManufacturer() const
	{
		return this->myManufacturer;
	}

	const vector<uint8_t>& SysExMessage::getBody() const
	{
		return this->myBody;
	}
	
	  
	/* Send System Exclusive Message.
	Should pass in with checksum if required, but not start or terminating byte. */
	void SysExMessage::sendMessage(MidiConnection* conn)
	{
		Serial.write(0xF0);
		for(uint8_t i=0; i < myManufacturer.size(); i++)
		{
		  conn->sendData(this->myManufacturer.at(i));
		}
		for(uint8_t i=0; i < myBody.size(); i++)
		{
		  conn->sendData(this->myBody.at(i));
		}
		Serial.write(0xF7);
	}
  
}