#include "tof.h"

ToF::ToF(HardwareSerial *frontSerial)
{
    _frontSerial = frontSerial;
    _frontSerial->begin(115200);
}

void ToF::init(HardwareSerial *debugSerial)
{
    int e = init_tof_sensors(debugSerial);
    Flash();
    return;
}

void ToF::getTofValues()
{
    get_tof_main();
    get_tof_front();
}

int ToF::init_tof_sensors(HardwareSerial *debugSerial)
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(tof_pins[i], OUTPUT);
        digitalWrite(tof_pins[i], LOW);
    }
    delay(100);
    for (int i = 0; i < 4; i++)
    {

        digitalWrite(tof_pins[i], HIGH);
        delay(20);
        tof_sensors[i].setTimeout(500);
        if (!tof_sensors[i].init())
        {
            debugSerial->println("Failed to detect and initialize sensor!");
            return 1;
        }
        tof_sensors[i].setAddress(0x30 + i);

        tof_sensors[i].startContinuous(0);
    }
    return 0;
}

void ToF::get_tof_main()
{
    for (int i = 0; i < 4; i++)
    {
        tof_main[i] = tof_sensors[i].readRangeContinuousMillimeters();
    }
}

void ToF::get_tof_front()
{
    if (_frontSerial->available())
    {
        // read the data from the front sensor
        String str = _frontSerial->readStringUntil('\n');
        int dataBox[5];
        StringToIntValues(str, dataBox);
        for (int i = 0; i < 5; i++)
        {
            tof_front[i] = dataBox[i];
        }
        Flash();
    }
}
void ToF::Flash()
{
    while (_frontSerial->available())
    {
        _frontSerial->readStringUntil('\n');
    }
}
void ToF::StringToIntValues(String str, int values[])
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
