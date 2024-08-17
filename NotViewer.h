#define NOTBUF_UUID "05590c96-12bb-11ee-be56-0242ac120002"
#define NOTDELBUF_UUID "19e04166-12bb-11ee-be56-0242ac120002"
#define DEL_UUID "c533a7ba-272e-11ee-be56-0242ac120002"

BTInstance delService(DEL_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE, NULL);
LCDWriter* lR;
#define notDispTime 7000


void notViewerINIT(void* arg)
{
  lR = LCDManager::registerWriter(0);
}

void nvHomePage(void* arg)
{
  static int dispIndex = 0;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.print("Notifications");

  long st = millis();
  if (notBuf.size())
  {
    String numMsgD = String(dispIndex + 1) + "/" + String(notBuf.size());
    oled.setCursor(128 - (numMsgD.length()*6), 0);
    oled.print(numMsgD);

    int numChars = (128 - ((notBuf[dispIndex].appName.length() + 1) * 6)) / 6;
    int numPx = (128 - ((notBuf[dispIndex].title.substring(0, numChars).length()) * 6));
    oled.setCursor(0, 8);
    oled.print(notBuf[dispIndex].appName);
    oled.setCursor(numPx, 8);
    oled.print(notBuf[dispIndex].title.substring(0, 16));
    oled.setCursor(0, 16);

    if(notBuf[dispIndex].text.compareTo("null"))
    {
      oled.print(notBuf[dispIndex].text);    
    }
  }

  else
  {
    oled.setCursor(0, 16);
    oled.print("No new notifications");
  }
  oled.display();

  switch (currentButton)
  {
    case UP_B:
      {
        if(dispIndex > 0)
        {
          dispIndex--;
          delay(buttonDelay);          
        }
        break;
      }

    case DOWN_B:
      {
        dispIndex++;
        delay(buttonDelay);
        break;
      }

    case SELECT_B:
      {
        if (dispIndex < notBuf.size())
        {
          std::string tS = notBuf[dispIndex].id.c_str();
          delService.characteristic->setValue(tS);
          delay(100);
          delService.characteristic->notify();
          notBuf.erase(notBuf.begin() + dispIndex);
          Serial.println(String(tS.c_str()));
          delay(buttonDelay);
        }

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


  if (dispIndex >= notBuf.size())
  {
    dispIndex = notBuf.size() - 1;
  }

  if (dispIndex < 0)
  {
    dispIndex = 0;
  }
}

class nbCB : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic* pCharacteristic)
  {
    String toAdd = pCharacteristic->getValue().c_str();
    Serial.println(toAdd);

    Notification tA;

    tA.appName = toAdd.substring(3, toAdd.indexOf("<1>"));
    tA.title = toAdd.substring(toAdd.indexOf("<1>") + 3, toAdd.indexOf("<2>"));
    tA.subTitle = toAdd.substring(toAdd.indexOf("<2>") + 3, toAdd.indexOf("<3>"));
    String key = toAdd.substring(toAdd.indexOf("<3>") + 3, toAdd.indexOf("<4>"));  //T means text, D means regular

    tA.isMsg = (key.indexOf("T") != -1);

    tA.text = toAdd.substring(toAdd.indexOf("<4>") + 3, toAdd.indexOf("<5>"));
    tA.text.trim();
    tA.id = toAdd.substring(toAdd.indexOf("<5>") + 3);
    tA.notTime = millis();

    bool nAdd = true;

    for (int i = 0; i < nSenders.size(); i++)
    {
      if (!nSenders[i].name.compareTo(tA.appName))
      {
        nSenders[i].lastAddTime = millis();
        nAdd = false;
        break;
      }
    }

    if (nAdd)
    {
      NotApp appTA;
      appTA.name = tA.appName;
      appTA.lastAddTime = millis();
      nSenders.push_back(appTA);
    }

    sort(nSenders.begin(), nSenders.end(), [](NotApp& a, NotApp& b)
         {
           return (a.lastAddTime < b.lastAddTime);
         });

    for (int i = 0; i < notBuf.size(); i++)
    {
      if (!notBuf[i].id.compareTo(tA.id))
      {
        //mathcing notification. ignore. UNLESS its a text message in which case go and update the text message we've already got.
        if (tA.isMsg)
        {
          notBuf[i] = tA;
        }
        return;
      }
    }

    notBuf.push_back(tA);
    Serial.println(tA.toString());

    sort(notBuf.begin(), notBuf.end(), [](Notification& a, Notification& b)
         {
           if (!a.appName.compareTo(b.appName))
           {
             return a.notTime > b.notTime;
           }

           else
           {
             long aT;
             long bT;
             for (int i = 0; i < nSenders.size(); i++)
             {
               if (!nSenders[i].name.compareTo(a.appName))
               {
                 aT = nSenders[i].lastAddTime;
               }

               if (!nSenders[i].name.compareTo(b.appName))
               {
                 bT = nSenders[i].lastAddTime;
               }
             }
             return (aT < bT);
           }
         });

    lR->write(tA.appName + "\n" + tA.title);

    delay(notDispTime);
    lR->clearDisplay();
    delay(100);
    //THE SENT CALLBACK ON THE PHONE IS ONLY CALLED AFTER THIS CALLBACK EXECUTES
  }

  void onRead(BLECharacteristic* pCharacteristic)
  {
    Serial.println("R1");
  }
};

class ndbCB : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic* pCharacteristic)
  {
    String val = pCharacteristic->getValue().c_str();
    Serial.println(val);

    for (int i = 0; i < notBuf.size(); i++)
    {
      if (!notBuf[i].id.compareTo(val))
      {
        notBuf.erase(notBuf.begin() + i);
      }
    }
  }

  void onRead(BLECharacteristic* pCharacteristic)
  {
    Serial.println("R2");
  }
};

App nvApp(notViewerINIT, nvHomePage, NULL, { new BTInstance(NOTBUF_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE, new nbCB()), new BTInstance(NOTDELBUF_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE, new ndbCB()), &delService });