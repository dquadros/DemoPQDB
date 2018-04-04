/*
 * Real Time Clock da Pão de Queijo Development Board
 * 20/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

#include <arduino.h>
#include <Wire.h>
#include "pqdb.h"

static const byte DS1307_ADDR = 0x68;

// Iniciação
void rtcInit(void) {
  Wire.begin();
}

// Converte valor binário em BCD
static byte bin2bcd(byte val) {
  return ((val/10) << 4) | (val % 10);
}

// Converte valor BCD em binário
static byte bcd2bin(byte val) {
  return (val >> 4)*10 + (val & 0x0F);
}

// Acerta data e hora e liga o relógio
void rtcSet(RTC_DATAHORA *dh) {
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(0);  // endereço do primeiro registrador
  Wire.write(bin2bcd(dh->segundo));
  Wire.write(bin2bcd(dh->minuto));
  Wire.write(bin2bcd(dh->hora));
  Wire.write(dh->diasemana);
  Wire.write(bin2bcd(dh->dia));
  Wire.write(bin2bcd(dh->mes));
  Wire.write(bin2bcd(dh->ano));
  Wire.endTransmission();
}

// Lê a data e hora atuais
void rtcRead(RTC_DATAHORA *dh) {
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(0);  // endereço do primeiro registrador
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 7);
  dh->segundo = bcd2bin(Wire.read());
  dh->minuto = bcd2bin(Wire.read());
  dh->hora = bcd2bin(Wire.read());
  dh->diasemana = Wire.read();
  dh->dia = bcd2bin(Wire.read());
  dh->mes = bcd2bin(Wire.read());
  dh->ano = bcd2bin(Wire.read());
}

// Escreve um byte na memória do RTC
// Endereço deve estar entre 0 e 55
void rtcMemWrite(byte ender, byte val) {
  if (ender > 56) {
    ender = 56;
  }
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(ender+8);
  Wire.write(val);
  Wire.endTransmission();
}

// Lê um byte da memória do RTC
// Endereço deve estar entre 0 e 55
byte rtcMemRead(byte ender) {
  if (ender > 56) {
    ender = 56;
  }
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(ender+8);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 1);
  return Wire.read();
}

