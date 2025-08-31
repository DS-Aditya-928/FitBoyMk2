#include "../includes.h"

App::App()
{
}

App::App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), BTInstance* btClass, bool enabled)
{                                                                                                                                          //bgFunc always runs.
    this->initFunc = initFunc;
    this->mainFunc = mainFunc;
    this->bgFunc = bgFunc;
    this->btClasses.push_back(btClass);
    this->enabled = enabled;
}

App::App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), std::vector<BTInstance*> btClasses, bool enabled)
{                                                                                                                 //bgFunc always runs.
    this->initFunc = initFunc;
    this->mainFunc = mainFunc;
    this->bgFunc = bgFunc;
    this->btClasses = btClasses;
    this->enabled = enabled;
}