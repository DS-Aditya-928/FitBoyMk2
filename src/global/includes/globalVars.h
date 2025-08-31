#ifndef GLOBALVARS_H
#define GLOBALVARS_H

extern ESP32Time rtc;
extern BLE2902* clientCC;

#define HRPIN 34
extern DFRobot_Heartrate heartrate;

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
extern DS_OLED oled;// = DS_OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//ScrollText.oled = &oled;

#define D4 15
#define D5 2
#define D6 4
#define D7 16
#define D8 5
#define D9 18

extern std::vector<Notification> notBuf;
extern std::vector<NotApp> nSenders;
extern float caloriesBurned;

extern MusicMeta musicMeta;

#endif // GLOBALVARS_H