#include "../includes.h"

BTInstance::BTInstance()
{
    this->uuid = "";
    this->bt_characteristics = 0;
    this->btClass = NULL;
}

BTInstance::BTInstance(String uuid, uint32_t bt_characteristics, BLECharacteristicCallbacks* btClass)
{
    this->uuid = uuid;
    this->bt_characteristics = bt_characteristics;
    this->btClass = btClass;
}