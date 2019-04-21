#include "usb.h"
#include <iostream>
#include <fstream>

using namespace std;
int getTemp() {
  string temp;
  ifstream tempFile;
  tempFile.open("/sys/class/thermal/thermal_zone0/temp");
  tempFile >> temp;
  return stoi(temp);
}

const int cutoffTemp = 35000;
USBController controller;

int main() {
  cout << getTemp();
  if (getTemp() > cutoffTemp) controller.on();
  else controller.off();
}
