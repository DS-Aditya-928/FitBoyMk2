#ifndef BTMANAGER_H
#define BTMANAGER_H

class BTInstance
{
public:
  String uuid = "";
  uint32_t bt_characteristics;
  BLECharacteristicCallbacks* btClass;
  BLECharacteristic* characteristic;

  BTInstance();
  BTInstance(String uuid, uint32_t bt_characteristics, BLECharacteristicCallbacks* btClass);
};


namespace BTManager
{
extern std::vector<BTInstance*> btiList;
extern void begin();
}

#endif // BTMANAGER_H