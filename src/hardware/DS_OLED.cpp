#include "../includes.h"

#define speedCon 300

void DS_OLED::printScrollText(String text, long startTime, int maxPixels = 128) {
    this->setTextWrap(false);

    uint16_t l = 0;
    uint16_t h = 0;
    int16_t def = 0;

    this->getTextBounds(text, 0, 0, &def, &def, &l, &h);

    uint16_t cursorX = this->getCursorX();
    uint16_t cursorY = this->getCursorY();
    int pixNeeded = l - maxPixels;

    int pCounter;
    int oneLet = ((float)l / text.length());
    int bVal;
    int cursorComp;
    int pxAfterLim;

    if (!backPixelBuf.count({ oneLet, h })) {
      backPixelBuf[{ oneLet, h }] = (uint8_t*)malloc(((oneLet + 7) / 8) * h);
      //Serial.println("new allocb");
    }

    if (!frontPixelBuf.count({ oneLet, h })) {
      frontPixelBuf[{ oneLet, h }] = (uint8_t*)malloc(((oneLet + 7) / 8) * h);
      //Serial.println("new allocf");
    }

    uint8_t* bBuf = backPixelBuf[{ oneLet, h }];
    uint8_t* fBuf = frontPixelBuf[{ oneLet, h }];
    
    if (pixNeeded > 0) {
      int maxChar = maxPixels / oneLet;
      int numCycleChars = text.length() - maxChar;
      bVal = (int)(((millis() - startTime) / speedCon) % max((numCycleChars * 2), 1)) - numCycleChars;
      int lCounter = abs(bVal);
      pCounter = (((millis() - startTime) / (speedCon / oneLet)) % oneLet) * ((bVal >= 0) ? 1 : -1);

      text = text.substring(lCounter - (int)(signum(bVal) == -1), min((int)(lCounter) + maxChar + (int)(signum(bVal) >= 0), (int)text.length()));

      cursorComp = ((int)(signum(bVal) == -1) * oneLet) + pCounter;
      pxAfterLim = (int)((text.length() * oneLet)) - cursorComp - maxPixels + 1;

      if (pxAfterLim > 0) {
        for (int j = 0; j < h; j++) {
          for (int i = 0; i <= pxAfterLim; i++) {
            //Serial.print(this->getPixel(this->getCursorX() + pxAfterLim + maxPixels - i, this->getCursorY() + j) ? '1' : ' ');
            if (this->getPixel(this->getCursorX() + pxAfterLim + maxPixels - i - 1, this->getCursorY() + j)) {
              fBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] = fBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] | (1 << i % 8);
            }

            else {
              fBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] = fBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] & ~(1 << i % 8);
            }
          }
        }
      }

      if (abs(cursorComp))  //behind text data.
      {
        for (int j = 0; j < h; j++) {
          for (int i = 0; i <= abs(cursorComp); i++) {
            if (this->getPixel(this->getCursorX() - i, this->getCursorY() + j)) {
              bBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] = bBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] | (1 << i % 8);
            }

            else {
              bBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] = bBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] & ~(1 << i % 8);
            }
          }
        }
      }

      this->setCursor(this->getCursorX() - cursorComp, this->getCursorY());
    }

    this->print(text);

    if (pixNeeded > 0) {
      for (int j = 0; j < h; j++) {
        for (int i = 0; i <= abs(cursorComp); i++) {
          int intI = abs(cursorComp) - i;
          this->writePixel(this->getCursorX() - ((text.length()) * oneLet) + i, this->getCursorY() + j, (bBuf[(j * ((oneLet + 7) / 8)) + (intI / 8)] >> (int)(intI % 8)) & 1);
        }

        for (int i = 0; i <= abs(pxAfterLim); i++) {
          this->writePixel(this->getCursorX() - i, this->getCursorY() + j, (fBuf[(j * ((oneLet + 7) / 8)) + (i / 8)] >> (int)(i % 8)) & 1);
        }
      }
    }

    this->setTextWrap(true);
  }