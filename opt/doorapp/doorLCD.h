/******************************************************************************
 * doorLCD.h
 *
 * Interface for front door LCD screen as part of RFIDACS
 *
 * Nile Mittow
 * 8/27/16
 * ***************************************************************************/

#ifndef DOORLCD_H
#define DOORLCD_H

#include "grove.hpp"
#include "jhd1313m1.hpp"
#include "mraa.hpp"
#include <string>
#include "/opt/libs/doorappMacros.h"

using namespace std;

class doorLCD {

  private:
    string line1;
    string line2;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    upm::Jhd1313m1 *lcd;

  public:

    doorLCD();
    ~doorLCD();
   
    void displayWelcome();
    void displayAllowed(string salutation);
    void displayDenied();
    void writeToDisplay();

}; /* end doorLCD */
#endif
