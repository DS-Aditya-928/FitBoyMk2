#ifndef DS_OLED_H
#define DS_OLED_H

class DS_OLED : public Adafruit_SH1106G {
public:
  DS_OLED(int a, int b, TwoWire* c, int d)
    : Adafruit_SH1106G(a, b, c, d) {
  }

  std::unordered_map<std::array<uint16_t, 2>, uint8_t*, ArrayHash> backPixelBuf, frontPixelBuf;
  void printScrollText(String text, long startTime, int maxPixels);
};

#endif // DS_OLED_H