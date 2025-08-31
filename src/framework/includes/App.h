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

  App();
  //GENERAL FORMAT. main func is responsible for displaying stuff and is only called when app is active
  App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), BTInstance* btClass = new BTInstance(), bool enabled = true);  
  App(void (*initFunc)(void*), void (*mainFunc)(void*), void (*bgFunc)(void*), std::vector<BTInstance*> btClasses, bool enabled = true);
};

namespace AppManager
{
extern std::vector<App*> appList;
extern int8_t actIndex;

#define buttonDelay 175
extern void begin();
extern void modeUp();
extern void modeDown();
}