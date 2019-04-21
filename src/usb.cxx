#include "usb.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const string base = "/sys/bus/usb/drivers/usb";
const string onPath = base + "/bind";
const string offPath = base + "/unbind";
const string usbNum = "1-1";

void USBController::on() {
  execute(onPath);
}

void USBController::off() {
  execute(offPath);
}

void USBController::execute(string target) {
  ofstream fileStream(target.c_str());
  fileStream << usbNum;
  fileStream.close();
}
