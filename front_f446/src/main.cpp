#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <VL53L0X.h>
#include <Wire.h>

#define NUMPIXELS 12
#define NeoPixelPIN PA11

#define I2C_SDA PB7
#define I2C_SCL PB6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NeoPixelPIN, NEO_GRB + NEO_KHZ800);

HardwareSerial uart1(PA10, PA9); // uart1 is connected to the USB port
HardwareSerial uart2(PA3, PA2);  // uart2 is connected to the main board
HardwareSerial uart3(PC5, PC10); // uart3 is connected to the OpenMV

int tof_pins[] = {PA4, PA5, PA6, PA7, PA8};
int tof_distances[5];
VL53L0X tof_sensors[5];

void i2c_scan()
{
  byte error, address;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      uart1.print("I2C device found at address 0x");
      if (address < 16)
        uart1.print("0");
      uart1.print(address, HEX);
      uart1.println("  !");
    }
    else if (error == 4)
    {
      uart1.print("Unknown error at address 0x");
      if (address < 16)
        uart1.print("0");
      uart1.println(address, HEX);
    }
  }
}

// initialize I2C
void init_i2c()
{
  Wire.setSCL(I2C_SCL);
  Wire.setSDA(I2C_SDA);
  Wire.begin();
}

// initialize TOF sensors
void init_tof_sensors()
{
  for (int i = 0; i < 5; i++)
  {
    pinMode(tof_pins[i], OUTPUT);
    digitalWrite(tof_pins[i], LOW);
  }
  delay(100);
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(tof_pins[i], HIGH);
    delay(20);
    i2c_scan();
    tof_sensors[i].setTimeout(500);
    if (!tof_sensors[i].init())
    {
      uart1.print(i);
      uart1.println("Failed to detect and initialize sensor!");

      while (1)
        ;
    }
    tof_sensors[i].setAddress(0x30 + i);

    tof_sensors[i].startContinuous(0);

    uart1.println("TOF sensor initialized");
  }
}

// get TOF distances
void get_tof_distances()
{
  for (int i = 0; i < 5; i++)
  {
    tof_distances[i] = tof_sensors[i].readRangeContinuousMillimeters();
  }
}

// initialize NeoPixel
void init_neopixel()
{
  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels.setBrightness(10);
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    pixels.show();
    delay(20);
  }
  delay(500);

  pixels.clear();
  pixels.show();
}

void send_tof()
{
  for (int i = 0; i < 5; i++)
  {
    uart1.print(tof_distances[i]);
    uart1.print(" ");
  }
  uart1.println();
  for (int i = 0; i < 5; i++)
  {
    uart2.print(tof_distances[i]);
    uart2.print(" ");
  }
  uart2.println();
}

void setup()
{
  uart2.setRx(PA3);
  uart2.setTx(PA2);

  uart1.begin(115200);
  uart2.begin(115200);

  uart3.begin(115200);
  uart2.println("Hello from the main board");
  init_neopixel();

  init_i2c();

  init_tof_sensors();

  init_neopixel();
}

void loop()
{
  get_tof_distances();
  send_tof();

  pixels.setPixelColor(7, pixels.Color(0, 255, 0));
  pixels.setPixelColor(5, pixels.Color(0, 255, 0));
  pixels.setPixelColor(4, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(0, 255, 0));
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));

  if (tof_distances[4] < 100)
  {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  }
  if (tof_distances[3] < 100)
  {
    pixels.setPixelColor(2, pixels.Color(255, 0, 0));
  }
  if (tof_distances[2] < 100)
  {
    pixels.setPixelColor(4, pixels.Color(255, 0, 0));
  }
  if (tof_distances[1] < 100)
  {
    pixels.setPixelColor(5, pixels.Color(255, 0, 0));
  }
  if (tof_distances[0] < 100)
  {
    pixels.setPixelColor(7, pixels.Color(255, 0, 0));
  }
  pixels.show();
  // if(uart1.available())
  // {
  //   char c = uart1.read();
  //   uart2.write(c);
  //   uart1.write(c);
  // }
  // if(uart2.available())
  // {
  //   char c = uart2.read();
  //   uart1.write(c);
  //   uart2.write(c);
  // }
}
