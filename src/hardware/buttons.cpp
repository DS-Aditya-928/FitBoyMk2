#include "../includes.h"

int lastButtons[NumSamples];

int cB()
{
  int inp = analogRead(12);
  int  val = (inp/100) + (int)((inp%100) >= 50);

  //Serial.println(analogRead(12));
  //Serial.println(val);

  for (int i = NumSamples - 1; i > 0; i--)
  {
    lastButtons[i - 1] = lastButtons[i];
  }

  lastButtons[NumSamples - 1] = val;

  bool aS = true;

  for(int i = 0; i < NumSamples - 1; i++)
  {
    if(lastButtons[i] != lastButtons[i + 1])
    {
      aS = false;
    }
  }

  if(aS)
  { 
    return(val);
  }

  else
  {
    return(21);
  }
}