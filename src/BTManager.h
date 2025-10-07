#include "BLEDevice.h"
#include "BLECharacteristic.h"
class BTInstance
{
public:
  String uuid = "";
  uint32_t bt_characteristics;
  BLECharacteristicCallbacks* btClass;
  BLECharacteristic* characteristic;

  BTInstance(String uuid, uint32_t bt_characteristics, BLECharacteristicCallbacks* btClass)
  {
    this->uuid = uuid;
    this->bt_characteristics = bt_characteristics;
    this->btClass = btClass;
  }

  BTInstance()
  {
    this->uuid = "";
    this->bt_characteristics = 0;
    this->btClass = NULL;
  }
};



namespace BTManager
{
std::vector<BTInstance*> btiList = {};

void begin()
{
#define SERVICE_UUID "1f55d926-12bb-11ee-be56-0242ac120002"
  BLEDevice::init("FitBoy");
  BLEDevice::setMTU(512);
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(SERVICE_UUID);

  for (int i = 0; i < btiList.size(); i++)
  {
    btiList.at(i)->characteristic = pService->createCharacteristic(
      btiList.at(i)->uuid.c_str(),
      btiList.at(i)->bt_characteristics);

    btiList.at(i)->characteristic->setCallbacks(btiList.at(i)->btClass);

    if((btiList.at(i)->bt_characteristics & BLECharacteristic::PROPERTY_NOTIFY) == BLECharacteristic::PROPERTY_NOTIFY)
    {
      btiList.at(i)->characteristic->addDescriptor(new BLE2902());
    }

    btiList.at(i)->characteristic->setValue("RDY");
  }

  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  //pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("BT Init ");
  //Serial.println(BLEDevice::getAddress().toString())
}
}