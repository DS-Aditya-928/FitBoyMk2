//cut string down.
#define speedCon 3000

int maxChar = ceil((float)maxPixels / (float)(l / text.length()));
oneLet = ((float)l / text.length());

int numCycleChars = text.length() - maxChar + 1;

int bVal = (int)(((millis() - startTime) / speedCon) % ((numCycleChars * 2) + 1)) - numCycleChars;
float lCounter = abs(bVal);
pCounter = (((millis() - startTime) / (speedCon / oneLet)) % oneLet) * signum(bVal % (numCycleChars));

text = text.substring((int)(lCounter - ((signum(pCounter) == -1) ? 1 : 0)), min((int)(lCounter) + maxChar, (int)text.length()));

Serial.println(String(lCounter) + " " + String(pCounter) + " " + String(numCycleChars) + " " + String(bVal) + " Demo");

if (pCounter)  //behind text data.
{
  bArray = (int8_t**)malloc(h * sizeof(int8_t*));

  for (int j = 0; j < h; j++)
  {
    bArray[j] = (int8_t*)malloc(ceil((float)(abs(pCounter) + 1) / 8.f));
    memset(bArray[j], 0, ceil((float)(abs(pCounter) + 1) / 8.f));

    for (int i = 0; i <= abs(pCounter); i++)
    {
      Serial.print(this->getPixel(this->getCursorX() - i, this->getCursorY() + j) ? '1' : ' ');
      if (this->getPixel(this->getCursorX() - i, this->getCursorY() + j))
      {
        bArray[j][i / 8] = bArray[j][i / 8] | (1 << i % 8);
      }
    }

    Serial.println();
  }
}

this->setCursor(this->getCursorX() - (pCounter) - ((signum(pCounter) == -1) ? oneLet : 0), this->getCursorY());
}

this->print(text);

if (bArray != nullptr)
{
  for (int j = 0; j < h; j++)
  {
    for (int i = 0; i <= abs(pCounter); i++)
    {
      int intI = abs(pCounter) - i;
      this->writePixel(this->getCursorX() + i - ((text.length()) * oneLet), this->getCursorY() + j, (bArray[j][intI / 8] >> (int)(intI % 8)) & 1);
    }

    free(bArray[j]);
  }
  free(bArray);
}
