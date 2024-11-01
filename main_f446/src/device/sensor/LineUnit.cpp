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

        int dataBox[18];
        StringToIntValues(str, dataBox);
        for (int i = 0; i < 15; i++)
        {
            _photoReflector[i] = dataBox[i];
        }
        _frontPhotoReflector = dataBox[15];
        colorL[0] = dataBox[16];
        colorR[0] = dataBox[17];
        return 1;
    }
    return 0;
}

void LineUnit::StringToIntValues(String str, int values[])
{
    int i = 0;
    int j = 0;
    while (i < str.length())
    {
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