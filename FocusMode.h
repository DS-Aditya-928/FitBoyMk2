void focusMenu(void* arg)
{
  static int baseIndex = 0;
  static int selectedIndex = 0;

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.print("Focus Mode");
  oled.setTextSize(1);

  for (int i = baseIndex; i < LCDManager::dataTw[2].size() && (i - baseIndex < 7); i++)
  {
    oled.setCursor(0, 8 + ((i - baseIndex) * 8));

    if (LCDManager::dataTw[2][i]->focusName[0])
    {
      if (LCDManager::dataTw[2][i]->getDispState())
      {
        oled.setTextColor(0, 1);
      }
      oled.print(LCDManager::dataTw[2][i]->focusName);
      oled.setTextColor(1, 0);
    }

    if (i == selectedIndex)
    {
      oled.setCursor(120, 8 + ((i - baseIndex) * 8));
      oled.print("<");
    }
  }

  oled.display();

  switch (currentButton)
  {
    case SELECT_B:
      {
        LCDManager::dataTw[2][selectedIndex]->setDispState(!(LCDManager::dataTw[2][selectedIndex]->getDispState()));
        delay(buttonDelay);
        break;
      }

    case RIGHT_B:
      {
        AppManager::modeUp();
        break;
      }

    case LEFT_B:
      {
        AppManager::modeDown();
        break;
      }

    case UP_B:
      {
        if (selectedIndex >= 1)
        {
          selectedIndex--;
          if (selectedIndex < baseIndex)
          {
            baseIndex--;
          }
          delay(buttonDelay);
        }

        break;
      }

    case DOWN_B:
      {
        if (selectedIndex < LCDManager::dataTw[2].size() - 1)
        {
          selectedIndex++;
          if (selectedIndex >= baseIndex + 7)
          {
            baseIndex++;
          }
          delay(buttonDelay);
        }

        break;
      }
  }
}

App fmApp(NULL, focusMenu, NULL);