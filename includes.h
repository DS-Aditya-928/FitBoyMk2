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

template <typename T> int signum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

struct ArrayHash {
    template <typename T, std::size_t N>
    std::size_t operator()(const std::array<T, N>& arr) const noexcept {
        std::size_t seed = 0;
        for (const auto& elem : arr) {
            // Combine hashes (standard hash combining trick)
            seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

#include "SPIFFS.h"
#include "DFRobot_Heartrate.h"
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