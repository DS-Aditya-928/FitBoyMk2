#define NO_BUTTON 19 ... 22
#define SELECT_B 12 ... 15
#define LEFT_B 7 ... 9
#define RIGHT_B 0
#define UP_B 1 ... 2
#define DOWN_B 3 ... 5

#define NumSamples 3

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

#define currentButton cB()