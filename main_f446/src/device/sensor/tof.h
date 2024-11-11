#ifndef tof_H
#define tof_H

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

class ToF
{
public:
    ToF(HardwareSerial *frontSerial);
    void init(HardwareSerial *debugSerial);
    int tof_main[4];
    int tof_front[5];
    void getTofValues();
    void Flash();

private:
    int tof_pins[4] = {PA12, PA13, PA14, PA15};
    VL53L0X tof_sensors[4];
    HardwareSerial *_frontSerial;
    void get_tof_main();
    void get_tof_front();
    int init_tof_sensors(HardwareSerial *debugSerial);
    void StringToIntValues(String str, int *values);
};

#endif