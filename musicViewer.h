#define MUSIC_DEETS_UUID "5df4d2b0-a927-11ee-a506-0242ac120002"
#define MUSIC_CONTROL_UUID "6ddb28be-a927-11ee-a506-0242ac120002"

BTInstance mcService(MUSIC_CONTROL_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE, NULL);

void musicViewer(void* arg)
{
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);

  if (musicMeta.playing)
  {
    oled.print("Now Playing...");
  }

  else
  {
    oled.print("Paused...");
  }

  oled.setCursor(0, 9);
  oled.setTextSize(1);  //10 CHAR HORIZ for size 2.
  oled.printScrollText(musicMeta.trackName, 0, 128);
  //oled.getBuffer();
  //oled.print(musicMeta.trackName);

  oled.setCursor(0, 18);
  oled.printScrollText(musicMeta.artistName, 500, 128);

  oled.drawRect(4, 50, 120, 10, SH110X_WHITE);
  oled.fillRect(4, 50, ((float)((musicMeta.playing ? (millis() - musicMeta.startTime) : 0) + musicMeta.startLoc) / musicMeta.length) * 120, 10, SH110X_WHITE);

  oled.display();
  switch (currentButton)
  {
    /*
    "1" = pause/play
    "2" = rewind
    "3" = FF
    */
    case SELECT_B:
      {
        mcService.characteristic->setValue("1");
        mcService.characteristic->notify();
        break;
      }

    case UP_B:
      {
        mcService.characteristic->setValue("3");
        mcService.characteristic->notify();
        break;
      }

    case DOWN_B:
      {
        mcService.characteristic->setValue("2");
        mcService.characteristic->notify();
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

class delCB : public BLECharacteristicCallbacks
{
  void onNotify(BLECharacteristic* pCharacteristic)
  {
    Serial.println("On Notify");
  }

  void onStatus(BLECharacteristic* pCharacteristic, Status s, uint32_t code)
  {
    Serial.println(String(s) + " os");
  }
};

extern App mvApp;

class mDCB : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic* pCharacteristic)
  {
    String dat = String(pCharacteristic->getValue().c_str());
    Serial.println(dat);

    if (dat.compareTo("KILL") == 0)
    {
      mvApp.enabled = false;
      if (mvApp.active)
      {
        AppManager::modeUp();
      }

      return;
    }

    mvApp.enabled = true;

    musicMeta.trackName = dat.substring(4, dat.indexOf("<1>"));
    musicMeta.artistName = dat.substring(dat.indexOf("<1>") + 3, dat.indexOf("<2>"));
    musicMeta.albumName = dat.substring(dat.indexOf("<2>") + 3, dat.indexOf("<3>"));
    musicMeta.length = atol(dat.substring(dat.indexOf("<3>") + 3, dat.indexOf("<4>")).c_str());
    musicMeta.startLoc = atol(dat.substring(dat.indexOf("<4>") + 3, dat.indexOf("<5>")).c_str());
    musicMeta.playing = atoi(dat.substring(dat.indexOf("<5>") + 3).c_str());
    musicMeta.startTime = millis();
  }
};

App mvApp = App(NULL, musicViewer, NULL, { new BTInstance(MUSIC_DEETS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE, new mDCB()), &mcService }, false);