#ifndef USB_ON_OFF_H
#define USB_ON_OFF_H

#include <string>

class USBController {
public:
  USBController() {};
  ~USBController() {};

  void on();
  void off();

private:
  void execute(std::string target);
};

#endif
