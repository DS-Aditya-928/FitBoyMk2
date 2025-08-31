#ifndef BUTTONS_H
#define BUTTONS_H

#define NO_BUTTON 19 ... 22
#define SELECT_B 12 ... 15
#define LEFT_B 7 ... 9
#define RIGHT_B 0
#define UP_B 1 ... 2
#define DOWN_B 3 ... 5

#define NumSamples 3

extern int lastButtons[NumSamples];
extern int cB();

#define currentButton cB()

#endif // BUTTONS_H