#include "usb.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>

using namespace std;
const int cutoffTemp = 35000;
USBController controller;
bool status = false;

int getTemp() {
  string temp;
  ifstream tempFile;
  tempFile.open("/sys/class/thermal/thermal_zone0/temp");
  tempFile >> temp;
  return stoi(temp);
}

void check() {
  const int temp = getTemp();
  const string log = "<Windowsill> temp read:" + to_string(temp);
  syslog(LOG_NOTICE, log.c_str());
  if (getTemp() > cutoffTemp) {
    if (!status) syslog(LOG_NOTICE, "<Windowsill> turning fan on");
    controller.on();
  } else {
    if (status) syslog(LOG_NOTICE, "<Windowsill> turning fan off");
    controller.off();
  }
}

int main(void) {
  pid_t pid, sessionid;

  pid = fork();
  if(pid > 0) {
    exit(EXIT_SUCCESS);
  } else if(pid < 0) {
    exit(EXIT_FAILURE);
  }

  umask(0);
  openlog("Windowsill", LOG_NDELAY | LOG_PID, LOG_USER);
  syslog(LOG_NOTICE, "<Windowsill> Succesfully started");

  sessionid = setsid();
  if (sessionid < 0) {
    syslog(LOG_ERR, "<Windowsill> Could not generate session ID for child process");
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(true) {
    check();
    sleep(20);
  }

  syslog(LOG_NOTICE, "<Windowsill> Succesfully exiting");
  closelog();

  exit(EXIT_SUCCESS);
}
