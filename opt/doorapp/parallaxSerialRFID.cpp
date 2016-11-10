/******************************************************************************
 * parallaxSerialRFID.cpp
 *
 * Implementation for Parallax Serial RFID reader on the Intel Edison platform
 *
 * Nile Mittow
 * 8/27/16
 * ***************************************************************************/

#include "parallaxSerialRFID.h"

/******************************************************************************
* CONSTRUCTORS & DESTRUCTOR
******************************************************************************/

/** default constructor, uses UART1 & pin 2 as enable **/
parallaxSerialRFID::parallaxSerialRFID() {
  try {
    readerUart = new mraa::Uart(0);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  if (readerUart->setTimeout(0,0,0) != mraa::SUCCESS) {
    cerr << "Error setting timout for UART" << endl;
  }
  ena = new mraa::Gpio(2);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  } 
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  return;
}

/** constructor taking enable gpio pin (int) as argument, uses UART1 **/
parallaxSerialRFID::parallaxSerialRFID(int _gpioNum) {
  try {
    readerUart = new mraa::Uart(0);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  if (readerUart->setTimeout(0,0,0) != mraa::SUCCESS) {
    cerr << "Error setting timout for UART" << endl;
  }
  ena = new mraa::Gpio(_gpioNum);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  } 
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  return;
}

/** constructor taking both custom UART port & enable pin as int **/
parallaxSerialRFID::parallaxSerialRFID(int _gpioNum, int _uartNum) {
  try {
    readerUart = new mraa::Uart(_uartNum);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  if (readerUart->setTimeout(0,0,0) != mraa::SUCCESS) {
    cerr << "Error setting timout for UART" << endl;
  }
  ena = new mraa::Gpio(_gpioNum);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  } 
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  return;
}

/** destructor, destroys all contexts **/
parallaxSerialRFID::~parallaxSerialRFID() {
  delete readerUart;
  delete ena;
  return;
}

/******************************************************************************
* MEMBER FUNCTIONS
******************************************************************************/

/** enable the RFID reader to read tags & clear UART buffer **/
void parallaxSerialRFID::enable() {
  readerUart->flush();
  ena->write(0);
  return;
}

/** disable the RFID reader from reading tags **/
void parallaxSerialRFID::disable() {
  ena->write(1);
  return;
}

/** returns true if a tag has been scanned **/
bool parallaxSerialRFID::tagScanned() {
  return readerUart->dataAvailable();
}

/** returns the read tag number as a string 
  * Has some ugly conversion business as parallax reader dumps a big
  * hex number and we want an integer.  revisit sometime to find a more
  * elegant solution                                                          
  ****                                                                        */
string parallaxSerialRFID::readTag() {
  string rawtagread = readerUart->readStr(TAGLENGTH);
  size_t n0 = std::count(rawtagread.begin(),rawtagread.end(),'0');
  size_t nc = std::count(rawtagread.begin(),rawtagread.end(),'C');
  size_t nf = std::count(rawtagread.begin(),rawtagread.end(),'F');
  if ((n0 + nc + nf) == 10) {
    std::cout << "Bad Read" << std::endl;
    return "-";
  }
  if (rawtagread.length() >= 3) {
    std::cout << "RawTag: " << rawtagread << std::endl;
    string hexStrOfTag = rawtagread.substr(3);
    unsigned int intOfTag;
    string intStrOfTag;
    stringstream ss;
    ss << hex << hexStrOfTag;
    ss >> intOfTag;
    stringstream si;
    si << static_cast<unsigned int>(intOfTag);
    readerUart->flush();
    return si.str();
  }
  else {
    readerUart->flush();
    return "-";
  }
}

void parallaxSerialRFID::flush() {
  while(readerUart->dataAvailable()) {
    readerUart->readStr(1);
    readerUart->flush();
  }
  return;
}
