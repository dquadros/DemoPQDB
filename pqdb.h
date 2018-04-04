/*
 * Conexões e Rotinas da Pão de Queijo Development Board
 * 20/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 */

// Shift Register 74HC595
// Compartilhado entre LCD, display 7 segmentos e teclado
const int pinSR_EN = 10;
const int pinSR_CLK = 11;
const int pinSR_DAT = 8;
void srInit(void);
void srWrite(byte valor);

// Buzzer
const int pinBuzzer = 9;

// LED RGB
// Pinos compartilhados com display 7 segmentos
const int pinLED_R = 2;
const int pinLED_G = 3;
const int pinLED_B = 4;

// Display de sete segmentos
const int pinDISP1 = 2;
const int pinDISP2 = 3;
const int pinDISP3 = 4;
const int pinDISP4 = 5;
void disp7Init(void);
void disp7Ativa(void);
void disp7Desativa(void);
void disp7Write(byte dig, byte val);
void disp7Show(unsigned val);

// Entradas analógicas
const int pinPOT = A2;
const int pinLDR = A1;
const int pinLM35 = A0;

// Teclado
const int pinTEC1 = 13;
const int pinTEC2 = 12;
const int tecUP1 = 0x001;
const int tecLF1 = 0x002;
const int tecDN1 = 0x004;
const int tecRG1 = 0x008;
const int tecA   = 0x010;
const int tecUP2 = 0x020;
const int tecRG2 = 0x040;
const int tecDN2 = 0x080;
const int tecLF2 = 0x100;
const int tecB   = 0x200;
uint16_t tecLe(void);

// Display LCD
const int pinLCD_RS = 7;
const int pinLCD_EN = 6;
void lcdInit(void);
void lcdWrite(byte lin, byte col, char *texto);
void lcdClear(void);

// RTC (DS1307)
typedef struct {
  byte dia;   // 01 a 31
  byte mes;   // 01 a 12
  byte ano;   // 00 a 99
  byte diasemana; // 1 a 7 (incrementado quando muda de dia)
  byte hora;    // 00 a 23
  byte minuto;  // 00 a59
  byte segundo; // 00 a 59
} RTC_DATAHORA;
void rtcInit();
void rtcSet(RTC_DATAHORA *dh);
void rtcRead(RTC_DATAHORA *dh);
void rtcMemWrite(byte ender, byte val);
byte rtcMemRead(byte ender);


