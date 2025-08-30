#include "../includes.h"

LCDWriter::LCDWriter()
{
    data = (char*)malloc(ARRAY_SIZE);
    memset((void*)data, 0, ARRAY_SIZE);
}

void LCDWriter::clearDisplay()
{
    memset((void*)data, 0, ARRAY_SIZE);
}

bool LCDWriter::getDispState()
{
    bool tR = false;
    dispValLock.lock();
    tR = display;
    dispValLock.unlock();
    return (tR);
}

void LCDWriter::setDispState(bool val)
{
    dispValLock.lock();
    display = val;
    dispValLock.unlock();
    return;
}

void LCDWriter::write(String s)
{
    dataLock.lock();
    memset((void*)data, 0, ARRAY_SIZE);
    s.toCharArray(data, ARRAY_SIZE);
    dataLock.unlock();
}

void LCDWriter::setFocusDeets(String fName)
{
    focusName = fName;
}