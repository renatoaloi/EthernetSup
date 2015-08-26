/*
  EthernetSup.cpp - Library for Arduino Ethernet.
  Created by Renato Aloi, Dec 26, 2013.
  Released into the public domain.
*/

#include <SPI.h>
#include <Ethernet.h>
#include "EthernetSup.h"

static char buttonIdx = -1;
static char lastButton = 0;
static char isReferer = 0;

static char buttonId[MAX_BUTTONS];
static char buttonType[MAX_BUTTONS];
static char buttonState[MAX_BUTTONS];
static char buttonText[MAX_BUTTONS][MAX_TEXT_BUTTON];
static char dimmerStep[MAX_BUTTONS];
static unsigned char dimmerValue[MAX_BUTTONS];
static char dimmerDirection[MAX_BUTTONS];

static void printP(EthernetClient client, const unsigned char *str);
static void setButtonId(char id);
static char getButton(int id);
static char getButtonId(char* requestText);
static void addButton(char pin, char *texton, char *textoff, char type, char state);
static char getButtonValue(char* requestText, char* compareTo, char size);

static void findButtonId(EthernetClient client, char c);
static void findDimmerValue(EthernetClient client, char c);
static char checkReferer(EthernetClient client, char c);

static void setDimmerDirection(char dir);

EthernetServer server(80);

EthernetSup::EthernetSup()
{
    
}

void EthernetSup::begin(unsigned char *_mac, unsigned char *_ip)
{
    IPAddress ip(_ip[0], _ip[1], _ip[2], _ip[3]);
    Ethernet.begin(_mac, ip);
    server.begin();
    
    for (char i = 0; i < MAX_BUTTONS; i++)
    {
      buttonId[i] = -1;
      buttonType[i] = -1;
      buttonState[i] = 0;
      for (char j = 0; j < MAX_TEXT_BUTTON; j++)
      {
        buttonText[i][j] = 0;
      }
      dimmerStep[i] = -1;
      dimmerValue[i] = 0;
      dimmerDirection[i] = -1;
    }
}

unsigned char EthernetSup::available() 
{
  unsigned char ret = 0;
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    boolean currentLineIsBlank = true;
    boolean isReferer = false;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();

        if (!isReferer)
        {
          findButtonId(client, c);
          findDimmerValue(client, c);
          isReferer = checkReferer(client, c);
        }

        if (c == '\n' && currentLineIsBlank) 
        {
          // send a standard http response header
          printP(client, http200);
          printP(client, contenttype);
          printP(client, connkeep);  
          printP(client, doctype);

          // Verificando o tipo de botao
          if (buttonIdx != -1)
          {
            if (buttonType[buttonIdx] == ONOFF_BUTTON)
            {
              buttonState[buttonIdx] = (buttonState[buttonIdx] ? 0 : 1);
            }
            else if (buttonType[buttonIdx] == DIMMER_BUTTON)
            {
              if (dimmerDirection[buttonIdx] == 1)
              {
                if (dimmerValue[buttonIdx] + dimmerStep[buttonIdx] < 255)
                  dimmerValue[buttonIdx] += dimmerStep[buttonIdx];
                else
                  dimmerValue[buttonIdx] = 255;
              }
              else if (dimmerDirection[buttonIdx] == 2)
              {
                if (dimmerValue[buttonIdx] - dimmerStep[buttonIdx] > 0)
                  dimmerValue[buttonIdx] -= dimmerStep[buttonIdx];
                else
                  dimmerValue[buttonIdx] = 0;
              }
            }
          }
          
          printP(client, head_ini);
          printP(client, stylesheet);
          printP(client, head_fim);
          printP(client, div_ini);
          
          for (int i = 0; i < MAX_BUTTONS; i++)
          {
            if (buttonType[i] != -1)
            {
              if (buttonType[i] == DIMMER_BUTTON)
              {
                printP(client, dimmer_ini1);
                client.print(buttonText[i]);

                // converting to percent
                int val1 = map(dimmerValue[i], 0, 255, 0, 100);
                client.print(val1, DEC);
                client.print("%");

                printP(client, dimmer_ini2);

                // link do dimmer UP
                printP(client, btnid);
                client.print(buttonId[i], DEC);
                printP(client, dimmerdown);
                printP(client, dimmer_mid11); 
                printP(client, colorgreen);
                printP(client, dimmer_mid12);
                printP(client, dimmer_space);
                printP(client, dimmer_space);
                client.print("-");
                printP(client, dimmer_space);
                printP(client, dimmer_space);
                printP(client, dimmer_mid2); 

                // link do dimmer DOWN
                printP(client, btnid);
                client.print(buttonId[i], DEC);
                printP(client, dimmerup);
                printP(client, dimmer_mid21);
                printP(client, colorgreen);
                printP(client, dimmer_mid22);
                printP(client, dimmer_space);
                printP(client, dimmer_space);
                client.print("+");
                printP(client, dimmer_space);
                printP(client, dimmer_space);
                printP(client, dimmer_fim); 
              }
              else
              {
                printP(client, button_ini);

                // link do botao
                printP(client, btnid);
                client.print(buttonId[i], DEC);
                printP(client, button_mid1);
                
                // cor do botao
                if (buttonState[i] == 1)
                {
                  printP(client, colorred);
                }
                else
                {
                  printP(client, colorblue);
                }
                printP(client, button_mid2);

                // texto do botao
                client.print(buttonText[i]);
                printP(client, button_fim);  
              }
            }
          }
          printP(client, div_fim);
          
          ret = 1;
          break;
        }

      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  
  return ret;
}

void EthernetSup::addButton(char pin,  char *texton,  char type)
{
  if (type == ONOFF_BUTTON)
  {
    addButton(pin, texton, type, (char)0);
  }
  else if (type == PULSE_BUTTON)
  {
    addButton(pin, texton, type, (char)-1);
  }
  else
  {
    // invalid button type
  }
}

void EthernetSup::addButton(char pin,   char *texton, char type, char state)
{
  int idx = lastButton;
  if (idx < MAX_BUTTONS)
  {
    buttonId[idx]    = pin;
    buttonType[idx]  = type;
    buttonState[idx] = state;
    for (int j = 0; j < MAX_TEXT_BUTTON; j++)
    {
        if (texton[j] != '\0')
          buttonText[idx][j]  = texton[j];
    }
    lastButton++;
  }
}

void EthernetSup::addDimmer(char id, char *textdim, unsigned char iniValue, char direction)
{
  addDimmer(id, textdim, iniValue, direction, 1, DIMMER_BUTTON);
}

void EthernetSup::addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step)
{
  addDimmer(id, textdim, iniValue, direction, step, DIMMER_BUTTON);
}

void EthernetSup::addDimmer(char id, char *textdim, unsigned char iniValue, char direction, char step, char type)
{
  int idx = lastButton;
  if (idx < MAX_BUTTONS)
  {
    buttonId[idx]    = id;
    buttonType[idx]  = type;
    buttonState[idx] = direction;
    for (int j = 0; j < MAX_TEXT_BUTTON; j++)
    {
        if (textdim[j] != '\0')
          buttonText[idx][j] = textdim[j];
    }
    dimmerStep[idx] = step;
    dimmerValue[idx] = iniValue;
    lastButton++;
  } 
}

char EthernetSup::getLastClickedButton()
{
  return buttonId[buttonIdx];
}

char EthernetSup::getButtonState(char id)
{
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        if (buttonId[i] == id)
        {
            return buttonState[i];
        }
    }
}

char EthernetSup::getDimmerValue()
{
  return dimmerValue[buttonIdx];
}

void EthernetSup::clearButtonIdx()
{
  buttonIdx = -1;
}











/**********************************************************************************
 *
 * STATIC FUNCS
 *
 **********************************************************************************/

static void setButtonId(char id)
{
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        if (buttonId[i] == id)
        {
            buttonIdx = i;
            break;
        }
    }
}

static void setDimmerDirection(char dir)
{
  dimmerDirection[buttonIdx] = dir;
}

// Funcoes de leitura realtime, sem buffer
static void findButtonId(EthernetClient client, char c)
{
  char myc = c;
  if (myc == 'b')
  {
    myc = client.read();
    if (myc == 'u')
    {
      myc = client.read();
      if (myc == 't')
      {
        myc = client.read();
        if (myc == 't')
        {
          myc = client.read();
          if (myc == 'o')
          {
            myc = client.read();
            if (myc == 'n')
            {
              myc = client.read();
              if (myc == '_')
              {
                myc = client.read();
                if (myc == 'i')
                {
                  myc = client.read();
                  if (myc == 'd')
                  {
                    myc = client.read();
                    if (myc == '=')
                    {
                      int imyc = 0;
                      myc = client.read();
                      if (myc >= '0' && myc <= '9')
                      {
                        imyc = (myc - '0');
                        myc = client.read();
                        if (myc >= '0' && myc <= '9')
                        {
                          imyc = (imyc * 10) + (myc - '0');
                          myc = client.read();
                          if (myc >= '0' && myc <= '9')
                          {
                            imyc = (imyc * 10) + (myc - '0');
                          }
                        }
                        setButtonId(imyc);
                      }
                    } 
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  c = myc;
}

// Funcoes de leitura realtime, sem buffer
static void findDimmerValue(EthernetClient client, char c)
{
  // Clear button value
  char myc = c;

  if (myc == 'b')
  {
    myc = client.read();
    if (myc == 'b')
    {
      myc = client.read();
      if (myc == 'u')
      {
        myc = client.read();
        if (myc == 't')
        {
          myc = client.read();
          if (myc == 't')
          {
            myc = client.read();
            if (myc == 'o')
            {
              myc = client.read();
              if (myc == 'n')
              {
                myc = client.read();
                if (myc == '_')
                {
                  myc = client.read();
                  if (myc == 'v')
                  {
                    myc = client.read();
                    if (myc == 'a')
                    {
                      myc = client.read();
                      if (myc == 'l')
                      {
                        myc = client.read();
                        if (myc == 'u')
                        {
                          myc = client.read();
                          if (myc == 'e')
                          {
                            myc = client.read();
                            if (myc == '=')
                            {
                              myc = client.read();
                              if (myc == 'u')
                              {
                                myc = client.read();
                                if (myc == 'p')
                                {
                                  setDimmerDirection(1);
                                }
                              }
                              else if (myc == 'd')
                              {
                                myc = client.read();
                                if (myc == 'o')
                                {
                                  myc = client.read();
                                  if (myc == 'w')
                                  {
                                    myc = client.read();
                                    if (myc == 'n')
                                    {
                                      setDimmerDirection(2);
                                    }
                                  }
                                }
                              } 
                            } 
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  c = myc;
}

static char checkReferer(EthernetClient client, char c)
{
  char myc = c;
  if (myc == 'R')
  {
    myc = client.read();
    if (myc == 'e')
    {
      myc = client.read();
      if (myc == 'f')
      {
        myc = client.read();
        if (myc == 'e')
        {
          myc = client.read();
          if (myc == 'r')
          {
            myc = client.read();
            if (myc == 'e')
            {
              myc = client.read();
              if (myc == 'r')
              {
                myc = client.read();
                if (myc == ':')
                {
                  return 1;
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

static void printP(EthernetClient client, const unsigned char *str) 
{
    uint8_t pgmChar;
    while(pgmChar = pgm_read_byte(str++))
    {
      client.write(pgmChar);
    }
}
