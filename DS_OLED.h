class DS_OLED : public Adafruit_SH1106G
{
public:
  DS_OLED(int a, int b, TwoWire* c, int d)
    : Adafruit_SH1106G(a, b, c, d)
  {
  }

  void printScrollText(String text, long startTime, int maxPixels = 128)  //only supports pixel scroll on strings covering the entire length of the display.
  {
    //Adafruit_GFX_Button testButton;
    //testButton.initButton(this, this->getCursorX() + 15, this->getCursorY() + 4, 30, 8, SH110X_WHITE, SH110X_BLACK, SH110X_WHITE, "Testygdfghjuid", 1, 1);
    //testButton.drawButton();
    this->setTextWrap(false);

    uint16_t l = 0;
    uint16_t h = 0;
    int16_t def = 0;

    this->getTextBounds(text, 0, 0, &def, &def, &l, &h);
    //h = this->getCursorY() - h;
    //Serial.println(String(l));
    //maxPixels--;
    int pixNeeded = l - maxPixels;

    int8_t** bArray = nullptr;
    int8_t** fArray = nullptr;
    int pCounter;
    int oneLet;
    int bVal;
    int cursorComp;
    int pxAfterLim;

    if (pixNeeded > 0)
    {
#define speedCon 300
      int maxChar = floor((float)(maxPixels)/ (float)(l / text.length()));
      oneLet = ((float)l / text.length());

      int numCycleChars = text.length() - maxChar;

      bVal = (int)(((millis() - startTime) / speedCon) % max((numCycleChars * 2), 1)) - numCycleChars;
      float lCounter = abs(bVal);
      pCounter = (((millis() - startTime) / (speedCon / oneLet)) % oneLet) * ((bVal >= 0) ? 1 : -1);

      text = text.substring(lCounter - (int)(signum(bVal) == -1), min((int)(lCounter) + maxChar + (int)(signum(bVal) >= 0) + 1, (int)text.length()));

      cursorComp = ((int)(signum(bVal) == -1) * oneLet) + pCounter;
      pxAfterLim = (text.length() * oneLet) - cursorComp - maxPixels + 1;

      Serial.println(String(lCounter) + " " + String(pCounter) + " " + String(pxAfterLim) + " " + String(bVal) + " Demo");

      if (pxAfterLim > 0)
      {
        fArray = (int8_t**)malloc(h * sizeof(int8_t*));

        for (int j = 0; j < h; j++)
        {
          fArray[j] = (int8_t*)malloc(ceil((float)(pxAfterLim / 8.f)));
          memset(fArray[j], 0, ceil((float)(pxAfterLim / 8.f)));

          for (int i = 0; i <= pxAfterLim; i++)
          {
            //Serial.print(this->getPixel(this->getCursorX() + pxAfterLim + maxPixels - i, this->getCursorY() + j) ? '1' : ' ');
            if (this->getPixel(this->getCursorX() + pxAfterLim + maxPixels - i - 1, this->getCursorY() + j))
            {
              fArray[j][i / 8] = fArray[j][i / 8] | (1 << i % 8);
            }
          }

          //Serial.println();
        }
      }

      if (abs(cursorComp))  //behind text data.
      {
        bArray = (int8_t**)malloc(h * sizeof(int8_t*));

        for (int j = 0; j < h; j++)
        {
          bArray[j] = (int8_t*)malloc(ceil((float)(abs(pCounter) + 1) / 8.f));
          memset(bArray[j], 0, ceil((float)(abs(pCounter) + 1) / 8.f));

          for (int i = 0; i <= abs(cursorComp); i++)
          {
            if (this->getPixel(this->getCursorX() - i, this->getCursorY() + j))
            {
              bArray[j][i / 8] = bArray[j][i / 8] | (1 << i % 8);
            }
          }
        }
      }

      this->setCursor(this->getCursorX() - cursorComp, this->getCursorY());
    }

    this->print(text);

    if (bArray != nullptr)
    {
      for (int j = 0; j < h; j++)
      {
        for (int i = 0; i <= abs(cursorComp); i++)
        {
          int intI = abs(cursorComp) - i;
          this->writePixel(this->getCursorX() - ((text.length()) * oneLet) + i, this->getCursorY() + j, (bArray[j][intI / 8] >> (int)(intI % 8)) & 1);
        }

        free(bArray[j]);
      }
      free(bArray);
    }

    if (fArray != nullptr)
    {
      for (int j = 0; j < h; j++)
      {
        for (int i = 0; i <= abs(pxAfterLim); i++)
        {
          this->writePixel(this->getCursorX() - i, this->getCursorY() + j, (fArray[j][i / 8] >> (int)(i % 8)) & 1);
        }

        free(fArray[j]);
      }
      free(fArray);
    }

    this->setTextWrap(true);
    }
};