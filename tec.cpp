/*
 * Teclado da Pão de Queijo Development Board
 * 20/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 * 
 * A matriz de teclado é acionada por D3 a D7 do shift register
 * e lida pelos pinos TEC1 e TEC2
 * 
 */

#include <arduino.h>
#include "pqdb.h"

// Rotinas locais
static uint16_t tecVarre(void);

// Le o teclado com debounce
// Retorna um mapa de bits com o estado das teclas
uint16_t tecLe(void) {
  int rep = 0;
  uint16_t tecAnt = tecVarre();
  while (rep < 3) {
    delay (10);
    uint16_t tec = tecVarre();
    if (tec == tecAnt) {
      rep++;
    } else {
      tecAnt = tec;
      rep = 0;
    }
  }
  return tecAnt;
}

// Varedura do teclado
static uint16_t tecVarre(void) {
  uint16_t leitura = 0;
  byte mask = 0x08;
  for (int i = 0; i < 5; i++) {
    // aciona uma coluna
    srWrite (mask);
    if (digitalRead(pinTEC1)) {
      leitura |= 0x01 << i;
    }
    if (digitalRead(pinTEC2)) {
      leitura |= 0x20 << i;
    }
    mask = mask << 1;
  }
  return leitura;
}

