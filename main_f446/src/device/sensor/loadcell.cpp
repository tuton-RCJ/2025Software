#include "loadcell.h"

void LoadCell::init()
{
    pinMode(LoadcellPin[0], INPUT);
    pinMode(LoadcellPin[1], INPUT);
    LoadcellOffset[0] = analogRead(LoadcellPin[0]);
    LoadcellOffset[1] = analogRead(LoadcellPin[1]);
}

void LoadCell::read()
{
    values[0] = analogRead(LoadcellPin[0]) - LoadcellOffset[0];
    values[1] = analogRead(LoadcellPin[1]) - LoadcellOffset[1];
}

void LoadCell::CaclurateMoment()
{
    moment = degrees(atan2(values[0], values[1]));
}