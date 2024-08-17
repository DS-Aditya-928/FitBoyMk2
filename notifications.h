class Notification
{
public:
  String appName;
  String title;
  String subTitle;
  String text;
  bool isMsg;
  String id;
  time_t notTime;

  String toString()
  {
    return ("App name: " + appName + "\nTitle: " + title + "\nSub Titles: " + subTitle + "\nIs Message: " + String(isMsg) + "\nText: " + text + "\nID: " + id);
  }
};

class NotApp
{
public:
  String name;
  long lastAddTime;
};