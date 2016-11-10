/******************************************************************************
 * parallaxSerialRFID.h
 *
 * Interface for Parallax Serial RFID reader on the Intel Edison platform
 *
 * Nile Mittow
 * 8/27/16
 * ***************************************************************************/

#ifndef PARALLAXSERIALRFID_H
#define PARALLAXSERIALRFID_H

#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include "mraa.hpp"
#include <algorithm>

#define TAGLENGTH 12

using namespace std;

class parallaxSerialRFID {

  private:
    
    /** mraa UART context object for the RFID reader **/
    mraa::Uart* readerUart;
    
    /** mraa GPIO context object for the RFID reader's enable pin **/
    mraa::Gpio* ena;
    
  public:

    /**************************************************************************
     * CONSTRUCTORS & DESTRUCTOR
     *************************************************************************/

    /** default constructor, uses UART1 & pin 2 as enable **/
    parallaxSerialRFID();

    /** constructor taking enable gpio pin (int) as argument, uses UART1 **/
    parallaxSerialRFID(int _gpioNum);

    /** constructor taking both custom UART port & enable pin as int **/
    parallaxSerialRFID(int _gpioNum, int _uartNum);

    /** destructor, destroys all contexts **/
    ~parallaxSerialRFID();

    /**************************************************************************
     * MEMBER FUNCTIONS
     *************************************************************************/

    /** enable the RFID reader to read tags & clear UART buffer **/
    void enable();

    /** disable the RFID reader from reading tags **/
    void disable();

    /** returns true if a tag of length TAGLENGTH has been scanned **/
    bool tagScanned();

    /** returns the read tag number as a string **/
    string readTag();

    void flush();

}; /* End parallaxSerialRFID */
#endif
