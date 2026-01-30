#include "DataLink.h"

DataLink::DataLink()
{

    Serial.print(F("Creating DataLink at adress "));
    Serial.println((int)this, DEC);

}



void DataLink::transmit_MenuInteraction(MenuInteraction menuInteraction)
{
  /*software*/Serial.print(DataPointID::MENUINTERACTION);
  /*software*/Serial.print(menuInteraction);
}
void DataLink::transmit_Data(CarJackDataFrame dataFrame)
{
  /*software*/Serial.print(DataPointID::VEHICLESPEED);
  /*software*/Serial.print(dataFrame.speed);
  /*software*/Serial.print(DataPointID::THROTTLEPOSITION);
  /*software*/Serial.print(dataFrame.throttlePosition);
  /*software*/Serial.print(DataPointID::CCSTATE);
  /*software*/Serial.print(dataFrame.rpm);
  Serial.println();
}