#include "includes.h"

void setup()
{
  Serial.begin(9600);
  pinMode(12, INPUT);

  oled.begin(i2c_Address, true);
  oled.clearDisplay();
  oled.setRotation(2);
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.setTextColor(SH110X_WHITE, 0);
  oled.print("Initializing...");
  oled.display();

  LCDManager::begin(D8, D9, D4, D5, D6, D7);

  AppManager::appList = {&mmApp, &nvApp, &fmApp, &hvApp, &mvApp};

  AppManager::begin();
  
  disableCore0WDT();
  disableCore1WDT();
}

void loop()
{

}