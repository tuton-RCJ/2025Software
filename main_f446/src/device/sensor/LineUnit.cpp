#include "LineUnit.h"

LineUnit::LineUnit(HardwareSerial *serial)
{
    _serial = serial;
    _serial->begin(115200);
}

int LineUnit::read()
{
    if (_serial->available())
    {
        String str = _serial->readStringUntil('\n');

        int dataBox[30];
        StringToIntValues(str, dataBox);
        for (int i = 0; i < 15; i++)
        {
            _photoReflector[i] = dataBox[i];
        }
        _frontPhotoReflector = dataBox[15];
        for (int i = 0; i < 3; i++)
        {
            colorL[i] = dataBox[16 + i];
        }
        for (int i = 0; i < 3; i++)
        {
            colorR[i] = dataBox[19 + i];
        }

        while (_serial->available() > 0)
        {
            _serial->read();
        }

        return 1;
    }
    return 0;
}

void LineUnit::setBrightness(int brightness)
{
    _serial->println(brightness);
}

void LineUnit::StringToIntValues(String str, int values[])
{
    int i = 0;
    int j = 0;
    while (i < str.length())
    {
        if (j > 29)
        {
            break;
        }
        if (str[i] == ' ')
        {
            i++;
            continue;
        }
        String value = "";
        while (str[i] != ' ' && i < str.length())
        {
            value += str[i];
            i++;
        }
        values[j] = value.toInt();
        j++;
    }
}