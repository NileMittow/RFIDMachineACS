/******************************************************************************
 * doorLCD.cpp
 *
 * Implementation for front door LCD screen as part of RFIDACS
 *
 * Nile Mittow
 * 8/27/16
 * ***************************************************************************/

#include "doorLCD.h"

using namespace std;

doorLCD::doorLCD() {
  lcd = new upm::Jhd1313m1(0,0x3E,0x62);
  this->displayWelcome();
  return;
}

doorLCD::~doorLCD() {
  delete lcd;
  return;
}
   
void doorLCD::displayWelcome() {
  line1 = WMSG_L1;
  line2 = WMSG_L2;
  r = WR;
  g = WG;
  b = WB;
  this->writeToDisplay();
  return;
}

void doorLCD::displayAllowed(string salutation) {
  line1 = AMSG_L1;
  line2 = salutation.substr(0,16);
  r = AR;
  g = AG;
  b = AB;
  this->writeToDisplay();
  return;
}

void doorLCD::displayDenied() {
  line1 = DMSG_L1;
  line2 = DMSG_L2;
  r = DR;
  g = DG;
  b = DB;
  this->writeToDisplay();
  return;
}

void doorLCD::writeToDisplay() {
  lcd->clear();
  lcd->setColor(r,g,b);
  lcd->setCursor(0,0);
  lcd->write(line1);
  lcd->setCursor(1,0);
  lcd->write(line2);
}

