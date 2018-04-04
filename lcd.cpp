/*
 * Display LCD da Pão de Queijo Development Board
 * 20/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

#include <arduino.h>
#include "pqdb.h"

const byte CMD = LOW;
const byte DADO = HIGH;

const byte CMD_CLS = 0x01;
const byte CMD_DISPON = 0x0C;
const byte CMD_POSCUR = 0x80;
const byte CMD_FUNCTIONSET = 0x20;
const byte LCD_4BITMODE = 0x00;
const byte LCD_2LINE = 0x08;
const byte LCD_5x8DOTS = 0x00;

static void setRS(byte valor) {
  digitalWrite(pinLCD_RS, valor);
}

static void setE(byte valor) {
  digitalWrite(pinLCD_EN, valor);
}

static void setDado(byte valor) {
  srWrite(valor);
}

static void writeByte(byte rs, byte dado) {
  setRS(rs);
  setE(HIGH);
  setDado(dado >> 4);
  setE(LOW);
  setE(HIGH);
  setDado(dado & 0x0F);
  setE(LOW);
}

static void writeCmd(byte cmd) {
  writeByte (CMD, cmd);
}

static void writeChar(byte chr) {
  writeByte (DADO, chr);
}

// Iniciação do Display
void lcdInit(void) {
  pinMode(pinLCD_RS, OUTPUT);
  pinMode(pinLCD_EN, OUTPUT);
  digitalWrite(pinLCD_RS, LOW);
  digitalWrite(pinLCD_EN, LOW);
  srWrite(0);
  delay (100);
  writeCmd (0x03);
  delay(5);    
  writeCmd (0x03);
  delay(1);    
  writeCmd (0x03);
  delay(1);    
  writeCmd (0x02);
  delay(1);    
  writeCmd(CMD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
  delay(1);    
  writeCmd (CMD_CLS);
  delay(2);    
  writeCmd (CMD_DISPON);
  delay(1);    
}

void lcdWrite(byte lin, byte col, char *texto) {
  byte ender = col;
  if (lin == 1) {
    ender += 0x40;
  }
  writeCmd(CMD_POSCUR | ender);
  while (*texto) {
    writeChar(*texto++);
  }
}

void lcdClear(void) {
  writeCmd (CMD_CLS);
  delay(2);    
}


