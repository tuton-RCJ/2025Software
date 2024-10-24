#include "STS3032.h"

STS3032::STS3032(HardwareSerial *serial)
{
    _serial = serial;
    _serial->begin(baudrate);
    sms_sts.pSerial = _serial;
    init();
}

STS3032::init()
{
    for (int i = 1; i < 5; i++)
    {
        sms_sts.unLockEprom(i);
        sms_sts.EnableTorque(i, 1);
        sms_sts.WheelMode(i);
        sms_sts.writeByte(i, SMS_STS_MODE, 1);
        sms_sts.LockEprom(i);
    }
}

STS3032::directDrive(int id, int speed)
{
    sms_sts.WriteSpe(id, speed);
}


STS3032::drive(speed,)