#ifndef LOADCELL_H
#define LOADCELL_H

#include <Arduino.h>

class LoadCell
{
public:
    void init();
    void read();
    int values[2]; // L,Rの値
    void CaclurateMoment();
    int moment;

private:
    int LoadcellPin[2] = {PC1, PC0};  // L,Rのピン
    int LoadcellOffset[2] = {0, 0}; // L,Rのオフセット
};

#endif