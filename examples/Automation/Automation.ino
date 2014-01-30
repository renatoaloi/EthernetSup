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
  // addButton(char pin, char *texton, char *textoff, char type)
  //    Onde:   pin: porta do Arduino a ser acionada
  //            texton: texto do botao para ativar o rele
  //            textoff: texto do botao para desativar o rele
  //            type: tipo do botao, sendo:
  //                      FLIP_BUTTON: botao que alterna o estado entre ligado e desligado
  //                      SWITCH_BUTTON:botao do tipo pulsar, sem alterar estado
  //
  // Obs: SWITCH_BUTTON nao precisa de textoff, apenas texton
  //
  sup.addButton(portaLampada, "Liga Lampada", "Desliga Lampada", FLIP_BUTTON);
  sup.addButton(portaVentilador, "Liga Ventilador", "Desliga Ventilador", FLIP_BUTTON);
  sup.addButton(portaSentidoVentilador, "Inverter Sentido", "Inverter Sentido", FLIP_BUTTON);
  sup.addButton(portaFechadura, "Abre Porta", "", SWITCH_BUTTON);
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