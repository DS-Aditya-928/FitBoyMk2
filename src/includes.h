#include <LiquidCrystal.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <ESP32Time.h>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <array>

#include "SPIFFS.h"
#include "DFRobot_Heartrate.h"

#include "util/ArrayHash.h"
#include "util/signum.h"

#include "buttons.h"

#include "DS_OLED.h"

//#include "modeNums.h"
#include "bitmaps.h"
#include "LCDHandler.h"
#include "BTManager.h"
#include "App.h"
#include "notifications.h"
#include "musicMetaData.h"

#include "globalVars.h"

#include "musicViewer.h"
#include "MainMenu.h"
#include "NotViewer.h"
#include "FocusMode.h"
#include "HealthViewer.h"
#include "buttonTest.h"