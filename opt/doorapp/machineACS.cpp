#include <iostream>
#include "parallaxSerialRFID.h"
#include "/opt/memberdatabase/member.h"
#include "doorLCD.h"
#include "/opt/libs/doorappMacros.h"
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#define ENABLETIME 15
  
parallaxSerialRFID rfid(1);
doorLCD lcd;
mraa::Gpio* machineEnable;

void cleanExit(int signum) {
  rfid.disable();
  machineEnable->write(0);
  std::cout << "Exiting..." << std::endl; 
  exit(signum);
}

int main() {
  machineEnable = new mraa::Gpio(8);
  machineEnable->dir(mraa::DIR_OUT);
  machineEnable->write(0);
  signal(SIGINT, cleanExit);
  sleep(10);
  rfid.enable();
  std::cout << "Listening..." << std::endl;
  while(1) {
    if(rfid.tagScanned()) {
      rfid.disable();
      string tag = rfid.readTag();
      std::cout << tag << std::endl;
      if (tag != "-") {
        member mem;
        mem.initializeFromDBByAccid(DB_LOCATION,tag);
        if (mem.isAllowed()) {
          lcd.displayAllowed(mem.getSalutation());
          machineEnable->write(1);
          sleep(ENABLETIME);
          machineEnable->write(0);
        }
        else {
          lcd.displayDenied();
        }
        sleep(3);
        lcd.displayWelcome();
      }
      rfid.flush();
      rfid.enable();
    }
  }
  return 0;
}
