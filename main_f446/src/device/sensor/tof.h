#ifndef tof_H
#define tof_H

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

class tof
{
public:
    tof(HardwareSerial *frontSerial);
    void init();
    int tof_main[4];
    int tof_front[5];

private:
    int tof_pins[4] = {PA12, PA13, PA14, PA15};
    VL53L0X tof_sensors[4];
    HardwareSerial *_frontSerial;
    void get_tof_main();
    //void get_tof_front();
    int init_tof_sensors();
};

#endif