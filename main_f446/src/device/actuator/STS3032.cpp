#include "STS3032.h"

// positive speed is forward, Positive turnRate is right

STS3032::STS3032(HardwareSerial *serial)
{
    _serial = serial;
    _serial->begin(baudrate);
    sms_sts.pSerial = _serial;
    init();
}

void STS3032::init()
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

void STS3032::LeftDrive(int SpeedPercent, int acceleration)
{
    int _SpeedPercent = constrain(SpeedPercent, -100, 100);
    int speed = _SpeedPercent * _maxSpeed / 100;

    sms_sts.WriteSpe(1, speed, acceleration);
    sms_sts.WriteSpe(2, speed, acceleration);
}

void STS3032::RightDrive(int SpeedPercent, int acceleration)
{
    int _SpeedPercent = constrain(SpeedPercent, -100, 100);
    int speed = _SpeedPercent * _maxSpeed / 100;

    speed = -speed;

    sms_sts.WriteSpe(3, speed, acceleration);
    sms_sts.WriteSpe(4, speed, acceleration);
}

void STS3032::stop()
{
    LeftDrive(0, 0);
    RightDrive(0, 0);
}

void STS3032::drive(int driveSpeedPercent, int turnRate)
{
    if (turnRate >= 100)
    {
        LeftDrive(driveSpeedPercent, 0);
        RightDrive(-driveSpeedPercent, 0);
    }
    else if (turnRate <= -100)
    {
        LeftDrive(-driveSpeedPercent, 0);
        RightDrive(driveSpeedPercent, 0);
    }
    else if (turnRate >= 0)
    {
        LeftDrive(driveSpeedPercent, 0);
        RightDrive(driveSpeedPercent - turnRate, 0);
    }
    else if (turnRate <= 0)
    {
        LeftDrive(driveSpeedPercent - turnRate, 0);
        RightDrive(driveSpeedPercent, 0);
    }
}