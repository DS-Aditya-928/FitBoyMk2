#define NO_BUTTON 20 ... 22
#define SELECT_B 14
#define LEFT_B 7 ... 9
#define RIGHT_B 0
#define UP_B 1
#define DOWN_B 5

#define NumSamples 3

int lastButtons[NumSamples];

int cB()
{
  int val = (((int)((analogRead(12) * 1.1f) + 30))/100);
  //Serial.println(analogRead(12));

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
  
  if(val == 0)
  {
    //Serial.println("ZERO");
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

#define currentButton cB()