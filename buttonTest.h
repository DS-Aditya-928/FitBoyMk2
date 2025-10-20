void buttonTest(void* arg) 
{
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);

  oled.print(String(analogRead(12)));
  oled.setCursor(0, 18);
  oled.print(String(currentButton));
  oled.display();
}

App buttonTestApp = App(NULL, buttonTest, NULL);