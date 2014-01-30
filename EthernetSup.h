/*
  EthernetSup.h - Library for Arduino Ethernet.
  Created by Renato Aloi, Dec 26, 2013.
  Released into the public domain.
*/
#ifndef EthernetSup_h
#define EthernetSup_h

#include "Arduino.h"
#include "html.h"

#define MAX_BUTTONS     20
#define MAX_TEXT_BUTTON 20

#define ONOFF_BUTTON    1
#define PULSE_BUTTON    0
#define DIMMER_BUTTON   2

#define DIMMER_MAX      255
#define DIMMER_MIN      0
#define DIMMER_UP       1
#define DIMMER_DOWN     0

class EthernetSup
{
  public:
    EthernetSup();
    void begin(unsigned char *_mac, unsigned char *_ip);
    unsigned char available();
    
    char getLastClickedButton();
    char getButtonState(char id);
    char getDimmerValue();
    void clearButtonIdx();
  //private:

    void addButton(char pin, char *texton, char type);
    void addButton(char pin, char *texton, char type, char state);
    void addDimmer(char id, char *textdim, unsigned char iniValue, char direction);
    void addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step);
    void addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step, char type);
};

#endif



