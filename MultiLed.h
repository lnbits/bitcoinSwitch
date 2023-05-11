#ifndef MultiLed_h
#define MultiLed_h

#include <Arduino.h>

class MultiLed {
  private:
    int pinRed;
    int pinGreen;
    int pinBlue;
  
  public:
    MultiLed(int pinRed_, int pinGreen_, int pinBlue_);
    void setup();
    void setColor(int color);

    static int COLOR_OFF;
    static int COLOR_RED;
    static int COLOR_GREEN;
    static int COLOR_BLUE;
};

#endif