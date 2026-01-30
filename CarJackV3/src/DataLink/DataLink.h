#ifndef DATALINK_H
#define DATALINK_H

#include <Arduino.h>
#include "../setup.h"

class DataLink
{
    private:
    //SoftwareSerial softwareSerial;
    

    public:
    DataLink();
    void transmit_MenuInteraction(MenuInteraction menuInteraction);
    void transmit_Data(CarJackDataFrame dataFrame);
};



#endif