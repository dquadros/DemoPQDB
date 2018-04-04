/*
 * Demonstração da Pão de Queijo Development Board
 * 21/03/18 - Daniel Quadros
 * https://dqsoft.blogspot.com
 * 
 * [x] Display LCD
 * [x] Buzzer
 * [x] LED RGB
 * [x] Display 7 segmentos
 * [x] Potenciômetro
 * [x] LM35
 * [x] LDR
 * [x] Teclado
 * [ ] Relógio
 */

#include "pqdb.h"

// Iniciação dos módulos
void setup() {
  buzzerInit();
  srInit();
  lcdInit();
  lcdWrite(0, 0, "Demo PQDB");
  lcdWrite(1, 0, "DQSoft");
  disp7Init();
  disp7Write(0, 10);
  disp7Write(1, 10);
  disp7Write(2, 10);
  disp7Write(3, 10);
  rtcInit();
  Serial.begin(9600);
  acertaHora();
  pausa();
}

// Demonstração
void loop() {

  // Buzzer
  lcdWrite(1, 0, "Buzzer          ");
  for (int i=0; i < 3; i++) {
    buzzerBeep();
    delay(500);
  }
  pausa();

  // LED RGB
  lcdWrite(1, 0, "LED RGB");
  while (tecLe() == 0) {
    ledRGB (HIGH, LOW, LOW);
    delay(500);
    ledRGB (LOW, HIGH, LOW);
    delay(500);
    ledRGB (LOW, LOW, HIGH);
    delay(500);
    ledRGB (LOW, LOW, LOW);
    delay(500);
  }
  while (tecLe() != 0) {
    delay (100);
  }

  // Entradas analógicas e display de 7 segmentos
  disp7Ativa();
  lcdWrite(1, 0, "Potenciometro");
  long tempo = millis() + 6000;
  while (millis() < tempo) {
    disp7Show (lePot());
    delay (300);
  }
  lcdWrite(1, 0, "LDR          ");
  tempo = millis() + 6000;
  while (millis() < tempo) {
    disp7Show (leLDR());
    delay (300);
  }
  lcdWrite(1, 0, "Temperatura");
  tempo = millis() + 6000;
  while (millis() < tempo) {
    disp7Show (leTemp());
    delay (300);
  }
  disp7Desativa();

  // Teclado
  lcdWrite(1, 0, "Teclado    ");
  while (tecLe() != (tecA | tecB)) {
    uint16_t tec = tecLe();
    if (tec == tecUP1) {
      lcdWrite(1, 8, "UP1");
    } else if (tec == tecUP2) {
      lcdWrite(1, 8, "UP2");
    } else if (tec == tecLF1) {
      lcdWrite(1, 8, "LF1");
    } else if (tec == tecLF2) {
      lcdWrite(1, 8, "LF2");
    } else if (tec == tecDN1) {
      lcdWrite(1, 8, "DN1");
    } else if (tec == tecDN2) {
      lcdWrite(1, 8, "DN2");
    } else if (tec == tecRG1) {
      lcdWrite(1, 8, "RG1");
    } else if (tec == tecRG2) {
      lcdWrite(1, 8, "RG2");
    } else if (tec == tecA) {
      lcdWrite(1, 8, "A  ");
    } else if (tec == tecB) {
      lcdWrite(1, 8, "B ");
    } else if (tec != 0) {
      lcdWrite(1, 8, "---");
    } else {
      lcdWrite(1, 8, "   ");
    }
  }  
  while (tecLe() != 0) {
    delay (100);
  }

  // Relogio
  char hora[15];
  for (int i = 0; i < 10; i++) {
    montaHora(hora);
    lcdWrite(1, 0, hora);
    delay(1000);
  }
  pausa();
}

/*
 * Acerta o relógio a partir da serial 
 * Por simplificação não tem consistências
 */
void acertaHora() {
  long timeout;
  
  // avisa que está pronto
  Serial.println();
  Serial.print('>');

  // espera 5 segundos pelo primeiro caracter
  timeout = millis()+5000;
  while ((timeout > millis()) && (Serial.available() < 1)) {
    delay(10);
  }
  if (Serial.available() < 1) {
    Serial.println('<');
    return;
  }

  // Le iniciação ddmmyyhhmmss
  RTC_DATAHORA dh;
  int c;
  dh.dia = dh.mes = dh.ano = 0;
  dh.hora = dh.minuto = dh.segundo = 0;
  dh.diasemana = 1;
  for (int i = 0; i < 12; ) {
    c = Serial.read();
    if (c < 0) {
      continue;
    }
    c = c - 0x30;
    switch (i) {
      case 0:
        dh.dia = c*10;
        break;
      case 1:
        dh.dia += c;
        break;
      case 2:
        dh.mes = c*10;
        break;
      case 3:
        dh.mes += c;
        break;
      case 4:
        dh.ano = c*10;
        break;
      case 5:
        dh.ano += c;
        break;
      case 6:
        dh.hora = c*10;
        break;
      case 7:
        dh.hora += c;
        break;
      case 8:
        dh.minuto = c*10;
        break;
      case 9:
        dh.minuto += c;
        break;
      case 10:
        dh.segundo = c*10;
        break;
      case 11:
        dh.segundo += c;
        break;
    }
    i++;
  }
  rtcSet(&dh);
  char hora[15];
  montaHora(hora);
  Serial.println();
  Serial.println(hora);
}

/*
 * Coloca data e hora atual em um buffer
 */
void montaHora(char *hora) {
  RTC_DATAHORA dh;
  strcpy(hora, "dd/mm hh:mm:ss");
  rtcRead(&dh);
  hora[0] = (dh.dia / 10) + 0x30;
  hora[1] = (dh.dia % 10) + 0x30;
  hora[3] = (dh.mes / 10) + 0x30;
  hora[4] = (dh.mes % 10) + 0x30;
  hora[6] = (dh.hora / 10) + 0x30;
  hora[7] = (dh.hora % 10) + 0x30;
  hora[9] = (dh.minuto / 10) + 0x30;
  hora[10] = (dh.minuto % 10) + 0x30;
  hora[12] = (dh.segundo / 10) + 0x30;
  hora[13] = (dh.segundo % 10) + 0x30;
}
 
/*
 * Espera uma tecla ser apertada
 */
void pausa() {
  // Espera soltar todas as teclas
  while (tecLe() != 0) {
    delay (100);
  }
  // Espera apertar qualquer tecla
  while (tecLe() == 0) {
    delay (100);
  }
  // Espera soltar todas as teclas
  while (tecLe() != 0) {
    delay (100);
  }
}

/*
 * Buzzer
 */
void buzzerInit() {
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);
}

void buzzerClick() {
  digitalWrite(pinBuzzer, HIGH);
  delay (20);
  digitalWrite(pinBuzzer, LOW);
}

void buzzerBeep() {
  digitalWrite(pinBuzzer, HIGH);
  delay (400);
  digitalWrite(pinBuzzer, LOW);
}

/*
 * LED RGB
 * pinos compartilhados com o display de 7 segmentos
 */
void ledRGB(byte red, byte green, byte blue) {
  srWrite(0); // desliga display 7 segmentos
  digitalWrite(pinLED_R, red);
  digitalWrite(pinLED_G, green);
  digitalWrite(pinLED_B, blue);
}

/*
 * Shift Register 74HC595
 * Compartilhado entre display LCD, display 7 segmentos e teclado
*/

// Iniciação
void srInit(void) {
  pinMode(pinSR_EN, OUTPUT);
  pinMode(pinSR_CLK, OUTPUT);
  pinMode(pinSR_DAT, OUTPUT);
  digitalWrite(pinSR_EN, LOW);
  digitalWrite(pinSR_CLK, LOW);
  digitalWrite(pinSR_DAT, LOW);
}

// Escrita
void srWrite(byte valor) {
  shiftOut(pinSR_DAT, pinSR_CLK, MSBFIRST, valor);
  digitalWrite(pinSR_EN, HIGH);
  digitalWrite(pinSR_EN, LOW);
}

/*
 * Entradas analógicas
 */

// Lê o potenciômetro, retorna uma posição de 0 a 100
// Potenciômetro está ligado entre 0 e 3.3V
int lePot() {
  int val = 0;
  analogReference(DEFAULT);
  analogRead(pinPOT); // descarta a primeira leitura
  for (int i = 0; i < 4; i++) {
    val += analogRead(pinPOT);
  }
  val = val >> 2;
  long perc = (val * 5000L) / (33L * 1024L);
  return (int) perc;
}

// Leitura do ADC ligado ao LDR
// tensao = 3.3*10000/(10000+LDR)  onde LDR é a resistência do LDR
// valor = (tensao * 1024)/5
int leLDR() {
  int val = 0;
  analogReference(DEFAULT);
  analogRead(pinLDR); // descarta a primeira leitura
  for (int i = 0; i < 4; i++) {
    val += analogRead(pinLDR);
  }
  return val >> 2;
  
}

// Retorna a temperatura em décimos de graus = tensão em miliVolts
int leTemp() {
  int val = 0;
  analogReference(INTERNAL);  // referência = 1,1V
  analogRead(pinLM35);        // descarta a primeira leitura
  delay(10);                  // referência foi selecionada no analogRead
                              // precisa de um tempo para descarregar o capacitor
                              // ligado a AREF
  for (int i = 0; i < 4; i++) {
    val += analogRead(pinLM35);
  }
  val = val >> 2;
  long temp = (val*1100L)/(1024L);
  analogReference(DEFAULT);
  return (int) temp;
}

