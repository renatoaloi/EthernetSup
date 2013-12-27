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
static char buttonTextOn[MAX_BUTTONS][MAX_TEXT_BUTTON];
static char buttonTextOff[MAX_BUTTONS][MAX_TEXT_BUTTON];


static void printP(EthernetClient client, const prog_uchar *str);
static void setButtonId(char id);
static char getButton(int id);
static char getButtonId(char* requestText);
static void addButton(char pin, char *texton, char *textoff, char type, char state);


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
            buttonTextOn[i][j] = 0;
            buttonTextOff[i][j] = 0;
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
            if (buttonType[buttonIdx] == FLIP_BUTTON)
      	    {
      	      buttonState[buttonIdx] = (buttonState[buttonIdx] ? 0 : 1);
      	    }
      	  }
      
            //Serial.println(buttonIdx, DEC);
        
          printP(client, head_ini);
          printP(client, stylesheet);
          printP(client, head_fim);
          printP(client, div_ini);
          
          for (int i = 0; i < MAX_BUTTONS; i++)
          {
            if (buttonType[i] != -1)
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
                
                
                if (buttonState[i] == 1)
                {
                  client.print(buttonTextOff[i]);
                }
                else
                {
                  client.print(buttonTextOn[i]);
                }
                
              
              
              printP(client, button_fim);
              
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

void EthernetSup::addButton(char pin,  char *texton,  char *textoff, char type)
{
  if (type == FLIP_BUTTON)
  {
    addButton(pin, texton, textoff, type, (char)0);
  }
  else
  {
    addButton(pin, texton, textoff, type, (char)-1);
  }
}

void EthernetSup::addButton(char pin,   char *texton,   char *textoff, char type, char state)
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
            buttonTextOn[idx][j]  = texton[j];
        if (textoff[j] != '\0')
            buttonTextOff[idx][j] = textoff[j];
    }
    lastButton++;
  }
}

char EthernetSup::getLastClickedButton()
{
  char id = buttonId[buttonIdx];
  buttonIdx = -1;
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












/**********************************************************************************
 *
 * STATIC FUNCS
 *
 **********************************************************************************/



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