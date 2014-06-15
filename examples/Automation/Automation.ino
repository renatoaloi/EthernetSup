/*
  Automation.ino - Automation sample for EthernetSup Library.
  Created by Renato Aloi, Jan 30, 2014.
  Released into the public domain.
*/

#include <SPI.h>
#include <Ethernet.h>
#include "EthernetSup.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 177 };

const char portaLampada = 5;
const char portaVentilador = 6;
const char portaSentidoVentilador = 8;
const char portaFechadura = 7;

EthernetSup sup;

void setup()
{
  sup.begin(mac, ip);
  
  // Configurando portas dos botoes
  pinMode(portaLampada, OUTPUT);
  pinMode(portaVentilador, OUTPUT);
  pinMode(portaSentidoVentilador, OUTPUT);
  pinMode(portaFechadura, OUTPUT);
  
  // Estado incial das portas
  digitalWrite(portaLampada, LOW);
  digitalWrite(portaVentilador, LOW);
  digitalWrite(portaSentidoVentilador, LOW);
  digitalWrite(portaFechadura, LOW);
  
  // Registrando botoes
  //
  // addButton(char pin, char *text, char type)
  //    Onde:   pin: porta do Arduino a ser acionada
  //            text: texto do botao 
  //            type: tipo do botao, sendo:
  //                      ONOFF_BUTTON: botao que alterna o estado entre ligado e desligado
  //                      PULSE_BUTTON:botao do tipo pulsar, sem alterar estado
  //
  // Registrando dimmers
  //
  // addDimmer(char pin, char *text, char valorIni, char direction, char step)
  //    Onde:   pin: porta do Arduino a ser acionada
  //            text: texto do dimmer 
  //            valorIni: valor inicial do dimmer
  //            direction: direcao inicial do dimmer
  //            step: quantidade de passos por acionamento
  //
  sup.addButton(portaLampada, "Lampada", ONOFF_BUTTON);
  sup.addButton(portaVentilador, "Ventilador", ONOFF_BUTTON);
  sup.addButton(portaSentidoVentilador, "Inv. Vent.", ONOFF_BUTTON);
  sup.addButton(portaFechadura, "Porta", PULSE_BUTTON);
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
      case portaLampada:
        digitalWrite(portaLampada, sup.getButtonState(lastButton));
        break;
      case portaVentilador:
        digitalWrite(portaVentilador, sup.getButtonState(lastButton));
        break;
      case portaSentidoVentilador:
        digitalWrite(portaSentidoVentilador, sup.getButtonState(lastButton));
        break;
      case portaFechadura:
        digitalWrite(portaFechadura, HIGH);
        delay(100);
        digitalWrite(portaFechadura, LOW);
        break;
      default:
        break;
    }
  }
  
  // Delay
  delay(10);
}