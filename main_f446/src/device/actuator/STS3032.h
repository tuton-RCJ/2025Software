#ifndef STS3032_H
#define STS3032_H

#include <Arduino.h>
#include <SCServo.h>

class STS3032
{
public:
    STS3032(HardwareSerial *serial);
    void init();
    const bool isDisabled = false;
    const int baudrate = 1000000;
private:
    SMS_STS sms_sts;
    HardwareSerial *_serial;
    int _maxSpeed = 3000;
    int _minSpeed = -3000;
};

#endif