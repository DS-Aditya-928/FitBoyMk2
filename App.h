#include <vector>
class App
{
public:
  bool enabled = true;
  bool active = false;
  void (*mainFunc)(void*);
  void (*bgFunc)(void*);
  void (*initFunc)(void*);
  std::vector<BTInstance*> btClasses;

  App()
  {
  }

  App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), BTInstance* btClass = new BTInstance(), bool enabled = true)  //GENERAL FORMAT. main func is responsible for displaying stuff and is only called when app is active
  {                                                                                                                                          //bgFunc always runs.
    this->initFunc = initFunc;
    this->mainFunc = mainFunc;
    this->bgFunc = bgFunc;
    this->btClasses.push_back(btClass);
    this->enabled = enabled;
  }

  App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), std::vector<BTInstance*> btClasses, bool enabled = true)  //GENERAL FORMAT. main func is responsible for displaying stuff and is only called when app is active
  {                                                                                                                 //bgFunc always runs.
    this->initFunc = initFunc;
    this->mainFunc = mainFunc;
    this->bgFunc = bgFunc;
    this->btClasses = btClasses;
    this->enabled = enabled;
  }
};

#include "AppList.h"

namespace AppManager
{
std::vector<App*> appList = {};
int8_t actIndex = 0;

namespace
{
void templateFunc(void* arg)
{
  App* appTr = (App*)arg;

  if (appTr->initFunc != NULL)
  {
    appTr->initFunc(NULL);
  }

  while (true)
  {
    long t1 = millis();
    if (appTr->enabled && appTr->active && appTr->mainFunc != NULL)
    {
      appTr->mainFunc(NULL);
    }

    else
    {
      delay(1);
    }

    if (appTr->bgFunc != NULL)
    {
      appTr->bgFunc(NULL);
    }

    long te = millis() - t1;

    //Serial.println(String(te));
  }
}
}

void begin()
{
  for (int i = 0; i < appList.size(); i++)
  {
    if (appList.at(i)->mainFunc != NULL || appList.at(i)->bgFunc != NULL || appList.at(i)->initFunc != NULL)
    {
      xTaskCreate(templateFunc, " ", 4096, appList.at(i), 1, NULL);
    }

    for (int j = 0; j < appList.at(i)->btClasses.size(); j++)
    {
      if (appList.at(i)->btClasses.at(j)->uuid.length() > 0)
      {
        BTManager::btiList.push_back(appList.at(i)->btClasses.at(j));
      }
    }

    if (appList.size() > 0)
    {
      appList.at(actIndex)->active = true;
    }
  }

  BTManager::begin();
}

#define buttonDelay 175
void modeUp()
{
  delay(buttonDelay);
  appList.at(actIndex)->active = false;

  while (true)
  {
    actIndex++;

    if (actIndex > appList.size() - 1)
    {
      actIndex = 0;
    }

    if (appList.at(actIndex)->enabled == true)
    {  //40ms windows of pure chaos
      appList.at(actIndex)->active = true;
      return;
    }
  }
}

void modeDown()
{
  delay(buttonDelay);
  appList.at(actIndex)->active = false;

  while (true)
  {
    actIndex--;

    if (actIndex < 0)
    {
      actIndex = appList.size() - 1;
    }

    if (appList.at(actIndex)->enabled == true)
    {  //40ms windows of pure chaos
      appList.at(actIndex)->active = true;
      return;
    }
  }
}
}