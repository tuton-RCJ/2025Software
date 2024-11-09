#ifndef line_H
#define line_H

#include <Arduino.h>

class LineUnit
{
public:
    LineUnit(HardwareSerial *serial);
    int read();
    void flush() { _serial->flush(); }
    void setBrightness(int brightness);
    int _photoReflector[15];
    int _frontPhotoReflector;
    int colorL[3];
    int colorR[3];

private:
    HardwareSerial *_serial;

    void StringToIntValues(String str, int *values);
};

#endif
