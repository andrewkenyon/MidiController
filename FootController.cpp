/*!
 *  @file       FootController.cpp
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */

#include "FootController.h"

using namespace std;

namespace midi
{
	FootController::FootController()
	{
		this->myBank = 0;
	}
  
  FootController::~FootController()
  {
	  delete this->myLcd;
	  
	  for(int i=0; i < this->mySwitches.size(); i++)
	  {
		  delete this->mySwitches.at(i);
	  }
  }
}
