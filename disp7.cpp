/*
 * Display de 7 segmentos da Pão de Queijo Development Board
 * 20/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

#include <TimerOne.h>
#include "pqdb.h"

// Controle do display de 7 segmentos
static volatile byte disp7Valor[4];
static volatile bool disp7Ativo;

// Converte valor de 0 a 9 nos segmentos
// valor 10 = apagado
static byte const disp7Decod[] = { 
  0xD7, 0x11, 0xCD, 0x5D, 0x1B, 0x5E, 0xDE, 0x15, 0xDF, 0x5F,  0x00
};

static void disp7Atl(void);

// Iniciação
void disp7Init(void) {
  pinMode(pinDISP1, OUTPUT);
  pinMode(pinDISP2, OUTPUT);
  pinMode(pinDISP3, OUTPUT);
  pinMode(pinDISP4, OUTPUT);
  digitalWrite(pinDISP1, LOW);
  digitalWrite(pinDISP2, LOW);
  digitalWrite(pinDISP3, LOW);
  digitalWrite(pinDISP4, LOW);
  disp7Ativo = false;
  disp7Valor[0] = disp7Valor[1] = disp7Valor[2] = disp7Valor[3] = 0;
  Timer1.initialize(5000L);
  Timer1.attachInterrupt(disp7Atl);
  interrupts();
}

// Atualização do display, toda na interrupção do timer1
static void disp7Atl(void) {
  static byte dig = 10;

  if (disp7Ativo) {
    srWrite(0);
    switch(dig) {
      case 0:
        digitalWrite(pinDISP4, LOW);
        digitalWrite(pinDISP1, HIGH);
        break;
      case 1:
        digitalWrite(pinDISP1, LOW);
        digitalWrite(pinDISP2, HIGH);
        break;
      case 2:
        digitalWrite(pinDISP2, LOW);
        digitalWrite(pinDISP3, HIGH);
        break;
      case 3:
        digitalWrite(pinDISP3, LOW);
        digitalWrite(pinDISP4, HIGH);
        break;
    }
    srWrite(disp7Decod[disp7Valor[dig]]);
    dig = (dig+1) & 3;
  }
}

// Ativa o display
void disp7Ativa(void) {
  disp7Ativo = true;
}

// Desativa o display
void disp7Desativa(void) {
  disp7Ativo = false;
  digitalWrite(pinDISP1, LOW);
  digitalWrite(pinDISP2, LOW);
  digitalWrite(pinDISP3, LOW);
  digitalWrite(pinDISP4, LOW);
}

// Altera um dígito do display
void disp7Write(byte dig, byte val) {
  if (dig > 3) {
    dig = 3;  // por segurança
  }
  if (val > 10) {
    val = 10; // por segurança 
  }
  disp7Valor[dig] = val;
}

// Mostra um valor no display
void disp7Show(unsigned val) {
  disp7Valor[3] = val % 10;
  disp7Valor[2] = (val/10) % 10;
  disp7Valor[1] = (val/100) % 10;
  disp7Valor[0] = (val/1000) % 10;
}


