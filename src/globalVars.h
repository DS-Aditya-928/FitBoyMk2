ESP32Time rtc(0);
BLE2902* clientCC;

#define HRPIN 34
DFRobot_Heartrate heartrate(DIGITAL_MODE);

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
DS_OLED oled = DS_OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//ScrollText.oled = &oled;

#define D4 15
#define D5 2
#define D6 4
#define D7 16
#define D8 5
#define D9 18

std::vector<Notification> notBuf;
std::vector<NotApp> nSenders;
float caloriesBurned;

MusicMeta musicMeta;