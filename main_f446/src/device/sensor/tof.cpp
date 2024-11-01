#include "tof.h"

tof::tof(HardwareSerial *frontSerial)
{
    _frontSerial = frontSerial;
    _frontSerial->begin(115200);
    init();
}

void tof::init()
{
    int e = init_tof_sensors();
    return;
}

int tof::init_tof_sensors()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(tof_pins[i], OUTPUT);
        digitalWrite(tof_pins[i], LOW);
    }
    delay(100);
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(tof_pins[i], HIGH);
        delay(20);
        tof_sensors[i].setTimeout(500);
        if (!tof_sensors[i].init())
        {
            return 1;
        }
        tof_sensors[i].setAddress(0x30 + i);

        tof_sensors[i].startContinuous(0);
    }
    return 0;
}

void tof::get_tof_main()
{
    for (int i = 0; i < 4; i++)
    {
        tof_main[i] = tof_sensors[i].readRangeContinuousMillimeters();
    }
}

// void  tof::get_tof_front()
// {
//     if(_frontSerial.available()){
//         //read the data from the front sensor
//         char data = _frontSerial.read();
//     }
// }