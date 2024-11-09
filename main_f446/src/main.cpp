#include <Arduino.h>
#include "./device/device.h"
#include <Wire.h>

HardwareSerial uart1(PA10, PA9);  // USB
HardwareSerial uart2(PA3, PA2);   // LINE BOARD
HardwareSerial uart3(PC11, PC10); // STS3032
HardwareSerial uart4(PA1, PA0);   // FRONT->>not used
HardwareSerial uart5(PD2, PC12);  // Top
HardwareSerial uart6(PC7, PC6);   // ESP32->>FRONT

STS3032 sts3032(&uart3);

Buzzer buzzer(PB1);

LineUnit line(&uart2);

void LineTrace();
void CheckRed();
void CheckGreen();
// tof tof(&uart6);

#define I2C_SDA PB7
#define I2C_SCL PB6

int threshold = 700;

void init_i2c()
{
  Wire.setSCL(I2C_SCL);
  Wire.setSDA(I2C_SDA);
  Wire.begin();
}

void setup()
{

  uart1.begin(115200);

  uart1.println("Hello, World!");
  // uart2.println("Hello, World!");

  init_i2c();
  line.setBrightness(76);
  buzzer.boot();

  // uart2.begin(115200);
  //  sts3032.drive(20, 100);
  //  buzzer.kouka();
}

void loop()
{
  line.read();
  for (int i = 0; i < 3; i++)
  {
    uart1.print(line.colorL[i]);
    uart1.print(" ");
  }
  uart1.println();
  LineTrace();
  CheckRed();
  CheckGreen();
}

int Kps[15] = {-2, -2, -2, -2, -2, -2, -2, 0, 2, 2, 2, 2, 2, 2, 2};
int Kp = 10;
int Kd = 0;
int Ki = 0;
int error = 0;
int lastError = 0;
int sumError = 0;
void LineTrace()
{
  error = 0;
  int pid = 0;
  int speed = 70;
  int turnRate = 0;
  for (int i = 0; i < 15; i++)
  {
    if (line._photoReflector[i] > threshold)
    {
      error += Kps[i];
    }
  }
  // if (abs(error) > 10 && line._frontPhotoReflector > threshold)
  // {
  //   error = 0;
  // }
  sumError += error;
  pid = Kp * error + Ki * sumError + Kd * (error - lastError);
  lastError = error;

  turnRate = pid;
  sts3032.drive(speed, turnRate);
}

void CheckRed()
{
  if (line.colorL[0] > 16 && line.colorL[1] < 20)
  {
    sts3032.stop();
    buzzer.kouka();
  }
}

void CheckGreen()
{
  if (line.colorL[0] < 10 && line.colorL[1] > 16)
  {
    for (int i = 0; i < 5; i++)
    {
      sts3032.stop();
    }
    buzzer.GreenMarker();
  }
}