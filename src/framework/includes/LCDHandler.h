#ifndef LCDHANDLER_H
#define LCDHANDLER_H

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

  LCDWriter();

  void clearDisplay();

  bool getDispState();
  void setDispState(bool val);

  void write(String s);
  void setFocusDeets(String fName);
};


namespace LCDManager
{
namespace
{
extern std::vector<LCDWriter*> dataTw[3];  //3 priority states
extern std::mutex arrayLock;
}

extern void focusMenu(void* arg);

extern void begin(uint8_t rs, uint8_t en, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
extern LCDWriter* registerWriter(int priority);
extern bool getDispState();
};

#endif // LCDHANDLER_H