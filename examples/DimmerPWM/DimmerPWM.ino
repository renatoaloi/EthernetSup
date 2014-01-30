/*
  DimmerPWM.ino - Dimmer PWM sample for EthernetSup Library.
  Created by Renato Aloi, Jan 30, 2014.
  Released into the public domain.
*/

#include <SPI.h>
#include <Ethernet.h>
#include "EthernetSup.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };

// Todas as portas disponiveis do ATMega328p
// Exceto Serial e ISP
// Agora acionando as portas PWM
const char porta1 = 2;
const char portaPWM1 = 3;
const char porta2 = 4;
const char portaPWM2 = 5;
const char portaPWM3 = 6;
const char porta3 = 7;
const char porta4 = 8;
const char porta5 = 9;
const char porta6 = 14;
const char porta7 = 15;
const char porta8 = 16;
const char porta9 = 17;
const char porta10 = 18;
const char porta11 = 19;

EthernetSup sup;

void setup()
{
  // Iniciando Ethernet
  sup.begin(mac, ip);
  
  // Configurando portas dos botoes
  pinMode(porta1, OUTPUT);
  pinMode(porta2, OUTPUT);
  pinMode(porta3, OUTPUT);
  pinMode(porta4, OUTPUT);
  pinMode(porta5, OUTPUT);
  pinMode(porta6, OUTPUT);
  pinMode(porta7, OUTPUT);
  pinMode(porta8, OUTPUT);
  pinMode(porta9, OUTPUT);
  pinMode(porta10, OUTPUT);
  pinMode(porta11, OUTPUT);
  
  // Estado incial das portas
  digitalWrite(porta1, LOW);
  digitalWrite(porta2, LOW);
  digitalWrite(porta3, LOW);
  digitalWrite(porta4, LOW);
  digitalWrite(porta5, LOW);
  digitalWrite(porta6, LOW);
  digitalWrite(porta7, LOW);
  digitalWrite(porta8, LOW);
  digitalWrite(porta9, LOW);
  digitalWrite(porta10, LOW);
  digitalWrite(porta11, LOW);
  
  sup.addDimmer(portaPWM1, "PWM 1: ", 127, -1, 25);
  sup.addDimmer(portaPWM2, "PWM 2: ", 127, -1, 25);
  sup.addDimmer(portaPWM3, "PWM 3: ", 127, -1, 25);
  sup.addButton(porta1, "Botao 1", ONOFF_BUTTON);
  sup.addButton(porta2, "Botao 2", ONOFF_BUTTON);
  sup.addButton(porta3, "Botao 3", ONOFF_BUTTON);
  sup.addButton(porta4, "Botao 4", ONOFF_BUTTON);
  sup.addButton(porta5, "Botao 5", ONOFF_BUTTON);
  sup.addButton(porta6, "Botao 6", ONOFF_BUTTON);
  sup.addButton(porta7, "Botao 7", ONOFF_BUTTON);
  sup.addButton(porta8, "Botao 8", ONOFF_BUTTON);
  sup.addButton(porta9, "Botao 9", ONOFF_BUTTON);
  sup.addButton(porta10, "Botao 10", ONOFF_BUTTON);
  sup.addButton(porta11, "Botao 11", ONOFF_BUTTON);
}

void loop()
{
  // Load HTML
  if (sup.available())
  {
    // Verifica se algum botao foi pressionado
    int lastButton = sup.getLastClickedButton();
    
    // Executa o comando conforme o botao clicado
    switch(lastButton)
    {
      case portaPWM1:
        analogWrite(portaPWM1, sup.getDimmerValue());
        break;
      case portaPWM2:
        analogWrite(portaPWM2, sup.getDimmerValue());
        break;
      case portaPWM3:
        analogWrite(portaPWM3, sup.getDimmerValue());
        break;
      case porta1:
        digitalWrite(porta1, sup.getButtonState(lastButton));
        break;
      case porta2:
        digitalWrite(porta2, sup.getButtonState(lastButton));
        break;
      case porta3:
        digitalWrite(porta3, sup.getButtonState(lastButton));
        break;
      case porta4:
        digitalWrite(porta4, sup.getButtonState(lastButton));
        break;
      case porta5:
        digitalWrite(porta5, sup.getButtonState(lastButton));
        break;
      case porta6:
        digitalWrite(porta6, sup.getButtonState(lastButton));
        break;
      case porta7:
        digitalWrite(porta7, sup.getButtonState(lastButton));
        break;
      case porta8:
        digitalWrite(porta8, sup.getButtonState(lastButton));
        break;
      case porta9:
        digitalWrite(porta9, sup.getButtonState(lastButton));
        break;
      case porta10:
        digitalWrite(porta10, sup.getButtonState(lastButton));
        break;
      case porta11:
        digitalWrite(porta11, sup.getButtonState(lastButton));
        break;
      default:
        break;
    }
    
    // Novo metodo para limpar estado
    // do botao somente no final
    sup.clearButtonIdx();
  }
  
  // Delay
  delay(10);
}
