#define TIME_UUID "93c37a10-1f37-11ee-be56-0242ac120002"

void mainMenu(void* arg)
{
  //Serial.println("MM");
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor((128 - (11 * 5)) / 2, 20);
  oled.print(rtc.getTime(String("%H") + ((rtc.getSecond() % 2) ? ":" : " ") + String("%M")));
  oled.setTextSize(1);

  String dateText = rtc.getTime("%B %d");
  oled.setCursor((128 - (6 * dateText.length())) / 2, 0);
  oled.print(dateText);
  oled.setCursor((128 - (6 * 3)) / 2, 42);
  oled.print(rtc.getTime("%A").substring(0, 3));

  
  if (notBuf.size())
  {
    oled.setCursor(6, 0);
    oled.drawBitmap(0, 0, myBitmapBell, 5, 7, SH110X_WHITE);
    oled.print(notBuf.size());
  }

  /*
  if (modeGet(MUSIC_M))
  {
    oled.setCursor(0, 10);
    oled.print("M");
  }
  */

  oled.display();

  switch (currentButton)
  {
    case SELECT_B:
      {
        Serial.println("SelectB");
        //ackService->setValue("AAAA");
        //ackService->notify();
        delay(500);
        break;
      }

    case UP_B:
      {
        //Serial.println(String(clientCC->getNotifications()) + " " + String(clientCC->getIndications()));
        break;
      }

    case RIGHT_B:
      {
        AppManager::modeUp();
        break;
      }

    case LEFT_B:
      {
        AppManager::modeDown();
        break;
      }
  }
}

class tCB : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic* pCharacteristic)
  {
    //display notification warning
    //save it in an internal buffer
    String val = pCharacteristic->getValue().c_str();
    Serial.println(val);
    rtc.setTime(val.toInt());
    String accessName = "/" + String(rtc.getDayofYear()) + String(rtc.getYear()) + ".txt";
    File f = SPIFFS.open(accessName);
    caloriesBurned = f.readString().toFloat();
    Serial.println(f.readString());
    f.close();

    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
  }
};


App mmApp(NULL, mainMenu, NULL, new BTInstance(TIME_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE, new tCB()));
