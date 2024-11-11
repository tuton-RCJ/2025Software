#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define I2C_SDA PB7
#define I2C_SCL PB6

#define I2C2_SDA PC12
#define I2C2_SCL PB10
TwoWire Wire2(I2C2_SDA, I2C2_SCL);

#define S11059_ADDR 0x2A

#define NP_R PC14
#define NP_L PC15
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NP_R, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(3, NP_L, NEO_GRB + NEO_KHZ800);

HardwareSerial Serial1(PA10, PA9);
HardwareSerial Serial3(PC11, PC10);
int sensor[] = {PC0, PC1, PC2, PC3, PA0, PA1, PA2, PA4, PA5, PA6, PA7, PC4, PC5, PB0, PB1, PA3}; // LLLLLLLCRRRRRRRF

int ReadColorSensorL();
int ReadColorSensorR();

void setup()
{

  Serial1.begin(115200);
  Serial3.begin(115200);

  Serial1.println("start");

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  // Init I2C
  Wire.begin();

  // 0x00 = Control
  // 0x8B = ADC-Reset + High Gain + 179.2ms
  Wire.beginTransmission(S11059_ADDR);
  Wire.write(0x00);
  Wire.write(0x8A);
  Wire.endTransmission();

  // 0x00 = Control
  // 0x0B = ADC-Run + High Gain + 179.2ms
  Wire.beginTransmission(S11059_ADDR);
  Wire.write(0x00);
  Wire.write(0x0A);
  Wire.endTransmission();

  Wire2.setSDA(I2C2_SDA);
  Wire2.setSCL(I2C2_SCL);
  Wire2.begin();

  Wire2.beginTransmission(S11059_ADDR);
  Wire2.write(0x00);
  Wire2.write(0x8A);
  Wire2.endTransmission();

  Wire2.beginTransmission(S11059_ADDR);
  Wire2.write(0x00);
  Wire2.write(0x0A);
  Wire2.endTransmission();

  strip.begin();
  strip.show();
  for (int i = 0; i < 3; i++)
  {
    strip.setPixelColor(i, 255, 255, 0);
  }
  strip.setBrightness(48);
  strip.show();

  strip2.begin();
  strip2.show();
  for (int i = 0; i < 3; i++)
  {
    strip2.setPixelColor(i, 255, 255, 0);
  }
  strip2.setBrightness(48);
  strip2.show();
}

void loop()
{
  delay(20);
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 16; i++)
  {
    int val = analogRead(sensor[i]);
    Serial1.print(val);
    Serial1.print(" ");
    Serial3.print(val);
    Serial3.print(" ");
  }

  ReadColorSensorL();
  Serial1.print(" ");
  Serial3.print(" ");
  ReadColorSensorR();

  Serial1.println();
  Serial3.println();

  if (Serial3.available())
  {
    int brightness = Serial3.read();
    for (int i = 0; i < 3; i++)
    {
      strip.setPixelColor(i, 255, 255, 0);
    }
    strip.setBrightness(brightness);
    for (int i = 0; i < 3; i++)
    {
      strip2.setPixelColor(i, 255, 255, 0);
    }
    strip2.setBrightness(brightness);
    strip.show();
    strip2.show();
  }
  if (Serial1.available())
  {
    int brightness = Serial1.read();
    for (int i = 0; i < 3; i++)
    {
      strip.setPixelColor(i, 255, 255, 0);
    }
    strip.setBrightness(brightness);
    for (int i = 0; i < 3; i++)
    {
      strip2.setPixelColor(i, 255, 255, 0);
    }
    strip2.setBrightness(brightness);
    strip.show();
    strip2.show();
  }
}

int ReadColorSensorL()
{
  int h, l, r, g, b, a;
  // 0x03 Read Data
  Wire.beginTransmission(S11059_ADDR);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(S11059_ADDR, 8);

  if (Wire.available())
  {

    // Red
    h = Wire.read();
    l = Wire.read();
    r = h << 8 | l;

    // Green
    h = Wire.read();
    l = Wire.read();
    g = h << 8 | l;

    // Blue
    h = Wire.read();
    l = Wire.read();
    b = h << 8 | l;

    // It converted to RGB?
    int rr, gg, bb;

    rr = map(r, 30, 56000, 0, 255);
    gg = map(g, 30, 41000, 0, 255);
    bb = map(b, 30, 56000, 0, 255);
    Serial1.print(rr);
    Serial1.print(" ");
    Serial1.print(gg);
    Serial1.print(" ");
    Serial1.print(bb);

    Serial3.print(rr);
    Serial3.print(" ");
    Serial3.print(gg);
    Serial3.print(" ");
    Serial3.print(bb);

    if (rr > 100 && gg > 100)
    {
      a = 1;
    }
    else if (rr > 55 && gg > 55)
    {
      a = 2;
    }
    else if (rr > 55)
    {
      a = 3;
    }
    else if (gg > 75)
    {
      a = 4;
    }
    else
    {
      a = 0;
    }
  }
  Wire.endTransmission();
  return a;
}

int ReadColorSensorR()
{
  int h, l, r, g, b, a;
  // 0x03 Read Data
  Wire2.beginTransmission(S11059_ADDR);
  Wire2.write(0x03);
  Wire2.endTransmission();

  Wire2.requestFrom(S11059_ADDR, 8);

  if (Wire2.available())
  {

    // Red
    h = Wire2.read();
    l = Wire2.read();
    r = h << 8 | l;

    // Green
    h = Wire2.read();
    l = Wire2.read();
    g = h << 8 | l;

    // Blue
    h = Wire2.read();
    l = Wire2.read();
    b = h << 8 | l;

    // It converted to RGB?
    int rr, gg, bb;

    rr = map(r, 30, 56000, 0, 255);
    gg = map(g, 30, 41000, 0, 255);
    bb = map(b, 30, 56000, 0, 255);
    Serial1.print(rr);
    Serial1.print(" ");
    Serial1.print(gg);
    Serial1.print(" ");
    Serial1.print(bb);

    Serial3.print(rr);
    Serial3.print(" ");
    Serial3.print(gg);
    Serial3.print(" ");
    Serial3.print(bb);

    // if (rr > 100 && gg > 100)
    // {
    //   a = 1;
    // }
    // else if (rr > 55 && gg > 55)
    // {
    //   a = 2;
    // }
    // else if (rr > 55)
    // {
    //   a = 3;
    // }
    // else if (gg > 75)
    // {
    //   a = 4;
    // }
    // else
    // {
    //   a = 0;
    // }
  }
  Wire2.endTransmission();
  return a;
}
