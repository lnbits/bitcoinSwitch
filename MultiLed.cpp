#include "MultiLed.h"

int MultiLed::COLOR_OFF = 0;
int MultiLed::COLOR_RED = 1;
int MultiLed::COLOR_GREEN = 2;
int MultiLed::COLOR_BLUE = 3;

MultiLed::MultiLed(int pinRed_, int pinGreen_, int pinBlue_) {
  pinRed = pinRed_;
  pinGreen = pinGreen_;
  pinBlue = pinBlue_;
}

void MultiLed::setup() {
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
}

void MultiLed::setColor(int color) {
  digitalWrite(pinRed, HIGH);
  digitalWrite(pinGreen, HIGH);
  digitalWrite(pinBlue, HIGH);  

  if (color == MultiLed::COLOR_RED) digitalWrite(pinRed, LOW);
  if (color == MultiLed::COLOR_GREEN) digitalWrite(pinGreen, LOW);
  if (color == MultiLed::COLOR_BLUE) digitalWrite(pinBlue, LOW);  
}
