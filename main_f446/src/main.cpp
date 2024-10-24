#include <Arduino.h>
#include <device/actuator.h>




HardwareSerial uart3(PC11, PC10);

STS3032 sts3032(&urat3);

Buzzer buzzer(PB1);


void setup()
{
  buzzer.boot();
}

void loop()
{


  int SPEED = 3000;

  sms_sts.WriteSpe(1, SPEED);
  sms_sts.WriteSpe(2, SPEED);
  sms_sts.WriteSpe(3, -SPEED);
  sms_sts.WriteSpe(4, -SPEED);

  delay(2000);

  sms_sts.WriteSpe(1, -SPEED);
  sms_sts.WriteSpe(2, -SPEED);
  sms_sts.WriteSpe(3, SPEED);
  sms_sts.WriteSpe(4, SPEED);

  delay(2000);
}