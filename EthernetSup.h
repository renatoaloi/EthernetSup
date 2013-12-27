/*
  EthernetSup.h - Library for Arduino Ethernet.
  Created by Renato Aloi, Dec 26, 2013.
  Released into the public domain.
*/
#ifndef EthernetSup_h
#define EthernetSup_h

#include "Arduino.h"
#include "html.h"

#define PGM_CHUNK       10
#define MAX_BUTTONS     8
#define MAX_TEXT_BUTTON 19
#define MAX_TEXT_REQ    30

#define FLIP_BUTTON     1
#define SWITCH_BUTTON   0

class EthernetSup
{
  public:
    EthernetSup();
    void begin(unsigned char *_mac, unsigned char *_ip);
    unsigned char available();
    void addButton(char pin, char *texton, char *textoff, char type);
    char getLastClickedButton();
    char getButtonState(char id);
  //private:
    void addButton(char pin, char *texton, char *textoff, char type, char state);
};

#endif



