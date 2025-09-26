uint8_t heartRate;
uint8_t dispHR;

uint8_t writeCounter = 0;
#define WRITE_FREQ 10

#define MIN_DR 60
#define MAX_DR 195

#define MAX_SPIKE 300     //if theres a spiek between readings greater than this, the new readings are ignored.
#define BEGIN_RATE 175    //if readings are picked up after prolonged absence of them, they must be below this value.
#define TIMEOUT_MS 20000  //if no new readings are registered after this, heartrate is set to 0.

#define TARGET_HEARTRATE 150  //150

time_t lastUpdate = 0;

LCDWriter* hR;
LCDWriter* calBurn;

void healthViewer_Init(void* arg)
{
  hR = LCDManager::registerWriter(2);
  hR->setFocusDeets("Heart Rate");
  calBurn = LCDManager::registerWriter(2);
  calBurn->setFocusDeets("Calories Burned");
}

void healthViewer_Main(void* arg)
{
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(0, 10);
  oled.print(String(dispHR));
  oled.setTextSize(1);
  oled.setCursor(String(dispHR).length() * 12, 16);
  oled.print("BPM");

  oled.setCursor(0, 30);
  oled.print("kcal: " + String(caloriesBurned));

  oled.display();

  switch (currentButton)
  {
    case UP_B:
      {
        break;
      }

    case DOWN_B:
      {
        break;
      }

    case SELECT_B:
      {
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

void healthViewer_BG(void* arg)
{
  uint8_t rateValue;
  heartrate.getValue(HRPIN);
  rateValue = heartrate.getRate();

  if (rateValue && ((heartRate > 0) ? ((rateValue - heartRate) < MAX_SPIKE) : (rateValue < BEGIN_RATE)))
  {
    heartRate = rateValue;

    if (heartRate >= TARGET_HEARTRATE)
    {
      caloriesBurned += (((millis() - lastUpdate) / 60000.f) * ((0.6309f * heartRate + 0.1988f * 72 + 0.2017f * 19) - 55.0969f) / 4.184f);
      writeCounter++;

      if (writeCounter >= WRITE_FREQ)
      {
        writeCounter = 0;
        String accessName = "/" + String(rtc.getDayofYear()) + String(rtc.getYear()) + ".txt";
        File f = SPIFFS.open(accessName, FILE_WRITE);
        f.print(String(caloriesBurned));
        f.close();
      }
    }

    lastUpdate = millis();
  }

  if (millis() - lastUpdate > TIMEOUT_MS)
  {
    heartRate = 0;
    lastUpdate = millis();
  }

  calBurn->write("kcal: " + String(caloriesBurned));

  if (heartRate >= MIN_DR && heartRate <= MAX_DR)
  {
    dispHR = heartRate;
  }

  hR->write("HR: " + String(dispHR));
}

App hvApp(healthViewer_Init, healthViewer_Main, healthViewer_BG);