#define ARRAY_SIZE 36
#define BLPIN 19

class LCDWriter
{
private:
  bool display = false;
public:
  String focusName = "";
  std::mutex dispValLock;
  std::mutex dataLock;
  char* data;

  LCDWriter()
  {
    data = (char*)malloc(ARRAY_SIZE);
    memset((void*)data, 0, ARRAY_SIZE);
  }

  void clearDisplay()
  {
    memset((void*)data, 0, ARRAY_SIZE);
  }

  bool getDispState()
  {
    bool tR = false;
    dispValLock.lock();
    tR = display;
    dispValLock.unlock();
    return (tR);
  }

  void setDispState(bool val)
  {
    dispValLock.lock();
    display = val;
    dispValLock.unlock();
    return;
  }

  void write(String s)
  {
    dataLock.lock();
    memset((void*)data, 0, ARRAY_SIZE);
    s.toCharArray(data, ARRAY_SIZE);
    dataLock.unlock();
  }

  void setFocusDeets(String fName)
  {
    focusName = fName;
  }
};


namespace LCDManager
{
//private:
namespace
{
std::vector<LCDWriter*> dataTw[3];  //3 priority states
std::mutex arrayLock;

bool dispOn = true;

String splashText[3] = { "You're stronger than you think", "Illegitimi non   carborundum", "Think before youdo, jackass." };

LiquidCrystal* lcdInt;

void blOff()
{
  if (dispOn)
  {
    pinMode(BLPIN, OUTPUT);
    digitalWrite(BLPIN, LOW);
    dispOn = false;
  }
}

void blOn()
{
  if (!dispOn)
  {
    pinMode(BLPIN, INPUT);
    dispOn = true;
  }
}

void smartOff()
{
  if (dispOn)
  {
    lcdInt->clear();
    lcdInt->setCursor(0, 0);
    int index = (rand() % 3);
    //int index = 2;
    lcdInt->print(splashText[index].substring(0, 16));
    if (splashText[index].length() >= 16)
    {
      lcdInt->setCursor(0, 1);
      String buf = splashText[index].substring(16);
      buf.trim();
      lcdInt->print(buf);
    }
    blOff();
  }
}

void LCDHandler(void* arg)
{
  //LCDManager* lcm = (LCDManager*)arg;
  char* toDraw[2];
  char* lastDraw[2];
  toDraw[0] = (char*)malloc(ARRAY_SIZE);
  toDraw[1] = (char*)malloc(ARRAY_SIZE);
  lastDraw[0] = (char*)malloc(ARRAY_SIZE);
  lastDraw[1] = (char*)malloc(ARRAY_SIZE);

  while (true)
  {
    int linesToDraw = 1;
    memset(&toDraw[0][0], 0, ARRAY_SIZE);
    memset(&toDraw[1][0], 0, ARRAY_SIZE);

    for (int i = 2; i >= 0; i--)
    {
      for (int j = 0; j < dataTw[i].size(); j++)
      {
        delay(1);
        dataTw[i][j]->dataLock.lock();
        if ((dataTw[i][j]->getDispState() || (i < 2)) && dataTw[i][j]->data[0])  //0 and 1 get automatic write priveledges.
        {
          //Serial.println(i);
          //set toDraw[0] to the value. if occupied, copy value to toDraw[1] and then draw.
          if (!strchr(dataTw[i][j]->data, '\n'))
          {
            if (toDraw[0][0])
            {
              memset(&toDraw[1][0], 0, ARRAY_SIZE);
              memcpy(&toDraw[1][0], &toDraw[0][0], ARRAY_SIZE);
            }

            memset(&toDraw[0][0], 0, ARRAY_SIZE);
            memcpy(&toDraw[0][0], dataTw[i][j]->data, ARRAY_SIZE);
          }

          else
          {
            memcpy(&toDraw[0][0], dataTw[i][j]->data, (size_t)(strchr(dataTw[i][j]->data, '\n') - dataTw[i][j]->data));
            memcpy(&toDraw[1][0], strchr(dataTw[i][j]->data, '\n') + 1, ARRAY_SIZE - ((size_t)(strchr(dataTw[i][j]->data, '\n') - dataTw[i][j]->data)));
          }
        }
        dataTw[i][j]->dataLock.unlock();
      }
    }

    if (!toDraw[0][0] && !toDraw[1][0])
    {
      memset(&lastDraw[0][0], 0, ARRAY_SIZE);
      memset(&lastDraw[1][0], 0, ARRAY_SIZE);
      smartOff();
      continue;
    }

    blOn();

    if (strcmp(toDraw[0], lastDraw[0]) || strcmp(toDraw[1], lastDraw[1]))
    {
      lcdInt->clear();
      lcdInt->setCursor(0, 0);
      lcdInt->print(toDraw[0]);
      lcdInt->setCursor(0, 1);
      lcdInt->print(toDraw[1]);

      memcpy(&lastDraw[0][0], &toDraw[0][0], ARRAY_SIZE);
      memcpy(&lastDraw[1][0], &toDraw[1][0], ARRAY_SIZE);
    }

    delay(5);
  }
}
}

//public:


//LCDManager(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
//{
//LiquidCrystal* lcd = new LiquidCrystal(rs, en, d0, d1, d2, d3);
//lcdInt = lcd;
//}

void begin(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  LiquidCrystal* lcd = new LiquidCrystal(rs, en, d0, d1, d2, d3);
  lcdInt = lcd;
  lcdInt->begin(16, 2);

  delay(100);

  xTaskCreate(LCDHandler, "LCDH", 2048, NULL, 1, NULL);
}

LCDWriter* registerWriter(int priority)
{

  if (priority < 0 || priority >= 3)
  {
    return (NULL);
  }

  LCDWriter* tR = new LCDWriter;
  arrayLock.lock();
  dataTw[priority].push_back(tR);
  arrayLock.unlock();

  return (tR);
}

bool getDispState()
{
  return (dispOn);
}
};