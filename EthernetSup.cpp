/*
  EthernetSup.cpp - Library for Arduino Ethernet.
  Created by Renato Aloi, Dec 26, 2013.
  Released into the public domain.
*/

#include <SPI.h>
#include <Ethernet.h>

#include "EthernetSup.h"

static int requestCount = 0; 
static char requestText[MAX_TEXT_REQ];
static char buttonIdx = -1;
static char lastButton = 0;

static char buttonId[MAX_BUTTONS];
static char buttonType[MAX_BUTTONS];
static char buttonState[MAX_BUTTONS];
static char buttonText[MAX_BUTTONS][MAX_TEXT_BUTTON];
//static char buttonTextOn[MAX_BUTTONS][MAX_TEXT_BUTTON];
//static char buttonTextOff[MAX_BUTTONS][MAX_TEXT_BUTTON];
static char dimmerStep[MAX_BUTTONS];
//static char buttonTextDim[MAX_BUTTONS][MAX_TEXT_BUTTON];
static unsigned char dimmerValue[MAX_BUTTONS];


static void printP(EthernetClient client, const prog_uchar *str);
static void setButtonId(char id);
static char getButton(int id);
static char getButtonId(char* requestText);
static void addButton(char pin, char *texton, char *textoff, char type, char state);
static char getButtonValue(char* requestText, char* compareTo, char size);


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
            //buttonTextOn[i][j] = 0;
            //buttonTextOff[i][j] = 0;
        }
    }
}

unsigned char EthernetSup::available() 
{
    unsigned char ret = 0;
    
  // Character counter
  requestCount = 0;
  
  // Clear request text
  for (int i = 0; i < MAX_TEXT_REQ; i++)
  {
    requestText[i] = '\0';
  }
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        //Serial.print(c);
        
        // Pegando texto do request
        if (requestCount < (MAX_TEXT_REQ - 1))
	{
	  requestText[requestCount] = c;
          requestCount++;
	}

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          //client.println("<H2>TESTE</H2>");
          client.println("<!DOCTYPE HTML>");
          
          // Verifica se algum botao foi clicado
          char t = getButtonId(requestText);
          
          //Serial.println(requestText);
          
          //Serial.println(t, DEC);
          
      	  setButtonId(t);

      	  if (buttonIdx != -1)
      	  {
            if (buttonType[buttonIdx] == ONOFF_BUTTON)
      	    {
      	      buttonState[buttonIdx] = (buttonState[buttonIdx] ? 0 : 1);
      	    }
            else if (buttonType[buttonIdx] == DIMMER_BUTTON)
            {
              if (getButtonValue(requestText, "button_value=up", 15) != -1)
              {
                  if (dimmerValue[buttonIdx] + dimmerStep[buttonIdx] < 255)
                    dimmerValue[buttonIdx] += dimmerStep[buttonIdx];
                  else
                    dimmerValue[buttonIdx] = 255;
              }
              else if (getButtonValue(requestText, "button_value=down", 17) != -1)
              {
                  if (dimmerValue[buttonIdx] - dimmerStep[buttonIdx] > 0)
                    dimmerValue[buttonIdx] -= dimmerStep[buttonIdx];
                  else
                    dimmerValue[buttonIdx] = 0;
              }

              
            }
      	  }
      
          //Serial.println(buttonIdx, DEC);
          
          printP(client, head_ini);
          printP(client, stylesheet);
          printP(client, head_fim);
          printP(client, div_ini);

          

          //printP(client, label_ini);
          //client.print("buttonDown: ");
          //client.print(getButtonValue(requestText, "button_value=down", 17), DEC);
          //printP(client, label_fim);
          
          for (int i = 0; i < MAX_BUTTONS; i++)
          {
            if (buttonType[i] != -1)
            {
              if (buttonType[i] == DIMMER_BUTTON)
              {
                printP(client, dimmer_ini1);
                //client.print("Velocidade ventilador: ");
                //client.print("10%");

                client.print(buttonText[i]);

                // converting to percent
                int val1 = map(dimmerValue[i], 0, 255, 0, 100);
                client.print(val1, DEC);
                client.print("%");

                printP(client, dimmer_ini2);

                // link do dimmer UP
                client.print("/?button_value=down");
                client.print("&button_id=");
                client.print(buttonId[i], DEC);
                

                printP(client, dimmer_mid11); 

                client.print("green");

                printP(client, dimmer_mid12);

                printP(client, dimmer_space);
                printP(client, dimmer_space);
                client.print("-");
                printP(client, dimmer_space);
                printP(client, dimmer_space);


                printP(client, dimmer_mid2); 

                // link do dimmer DOWN
                client.print("/?button_value=up");
                client.print("&button_id=");
                client.print(buttonId[i], DEC);


                printP(client, dimmer_mid21);

                client.print("green");

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
                client.print("/?button_id=");
                client.print(buttonId[i], DEC);
                
                //Serial.println(buttonId[i], DEC);
                printP(client, button_mid1);
                
                // cor do botao
                if (buttonState[i] == 1)
                {
                  client.print("red");
                }
                else
                {
                  client.print("blue");
                }
                printP(client, button_mid2);

                // texto do botao
                client.print(buttonText[i]);
                //if (buttonState[i] == 1)
                //{
                  //client.print(buttonTextOff[i]);
                //}
                //else
                //{
                  //client.print(buttonTextOn[i]);
                //}
                printP(client, button_fim);  
              }
            }
          }
          	
          printP(client, div_fim);
          
          ret = 1;
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
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
        //    buttonTextOn[idx][j]  = texton[j];
        //if (textoff[j] != '\0')
        //    buttonTextOff[idx][j] = textoff[j];
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
        //if (textup[j] != '\0')
        //    buttonTextOn[idx][j]  = textup[j];
        /*if (textdown[j] != '\0')
            buttonTextOff[idx][j] = textdown[j];*/
        if (textdim[j] != '\0')
          buttonText[idx][j] = textdim[j];
            //buttonTextDim[idx][j] = textdim[j];
    }
    dimmerStep[idx] = step;
    dimmerValue[idx] = iniValue;
    lastButton++;
  } 
}

char EthernetSup::getLastClickedButton()
{
  char id = buttonId[buttonIdx];
  //buttonIdx = -1;
  return id;
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
  char value = dimmerValue[buttonIdx];
  //buttonIdx = -1;
  return value;
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

static char getButtonValue(char* requestText, char* compareTo, char size)
{
  char ret =-1;
  for (int i = 0; i < (MAX_TEXT_REQ - size); i++)
  {
    if (strncmp(compareTo, (char*)&(requestText[i]), size) == 0)
    {
      ret = i;
      break;
    }
  }
  return ret;
}


static char getButtonId(char* requestText)
{
    int ret = 0;
    char c[] = { '\0', '\0', '\0' };
    int fatorDec = 0;	
    int found = -1;
    int idx;
    for (int i = 0; i < (MAX_TEXT_REQ - 11); i++)
    {
        if (strncmp("button_id=", (char*)&(requestText[i]), 10) == 0)
        {
            found = i;
            break;
        }
    }
    if (found != -1)
    {
        //Serial.println("found!");
        found += 10;
        for (int i = found; i < (MAX_TEXT_REQ - 11); i++)
        {
            if (strncmp(" HTTP/1.", (char*)&(requestText[i]), 8) == 0)
                break;
            else
                fatorDec++;
        }
            
        if (fatorDec > 0 && fatorDec <= 3)
        {
            idx = 0;
            for (int i = found; i < (found + fatorDec); i++)
            {
                c[idx++] = requestText[i];
            }
            
            for (int i = 0; i < fatorDec; i++)
            {
                if (i > 0) 
                    ret *= 10;
                ret += c[i] - '0';
            }
            
            return ret;
        }
    }
    return -1;
}

static char getButton(int id)
{
    char ret = -1;
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        if (buttonId[i] == id)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

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

static void printP(EthernetClient client, const prog_uchar *str) 
{
    uint8_t buffer[PGM_CHUNK];
    size_t bufferEnd = 0;
    while(buffer[bufferEnd++] = pgm_read_byte(str++))
    {
        if (bufferEnd == PGM_CHUNK)
        {
            client.write(buffer, PGM_CHUNK);
            bufferEnd = 0;
        }
    }
    
    if (bufferEnd > 1) 
        client.write(buffer, bufferEnd - 1);
}