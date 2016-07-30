/**Ardoscopio 1MSPS
   Desenvolvida por Evanuel Ribeiro
   OLED 128X64
   DRIVER ssd1306
   Evanuel.r7@gmail.com
   whatasapp +55 98 999683780
   1MSPS 
*/

#include<U8glib.h>

U8GLIB_SSD1306_128X64_2X oled(U8G_I2C_OPT_NONE);

#define W 128
#define H 64
#define PRECISAO 1023.0
#define SCREEN_UPDATE 2

int probe = A0;
#define DELAY_POTENCIOMETRO
#ifdef DELAY_POTENCIOMETRO
int delayAI = A1;
#endif

float delayVariavel = 0;
float scala = 0;
float volts = 0.0;
int xCounter = 0;
int yPosition = 0;
int readings[W + 1];
int counter = 0;

unsigned long drawTime = 0;
unsigned long lastDraw = 0;
int frames = 0;

/*/Frequencimetro
#define inFreq 2
long freq, c;
int pulseCount;
boolean pulse;
*/
//--

// constante para configuração do prescaler
const unsigned char PS_16 = (1 << ADPS2);
//const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
//const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
//--
void delayP(void) {
#ifdef DELAY_POTENCIOMETRO
  delayVariavel = analogRead(delayAI);
  delayVariavel = (delayVariavel / 100);
#endif

  scala = (float)(H - 1) / PRECISAO;

  for (xCounter = 0; xCounter <= W; xCounter++) {
    yPosition = analogRead(probe);
    readings[xCounter] = (yPosition * scala);
    #ifdef DELAY_POTENCIOMETRO
        delay(delayVariavel);
    #endif
  }
}

void drawInf(void) {
  //Calculate FPS
  drawTime = micros();
  //oled.drawHLine(2, 35, 47);
  oled.drawVLine(105, 1, 64);
  oled.drawStr(106, 60, "Volts");
  //oled.drawStr(106, 10, "-3.3");
  oled.setPrintPos(106, 44);
  oled.print(1000000 / (drawTime - lastDraw));
  oled.print("fps");
  lastDraw = drawTime;
  oled.setPrintPos(106, 52);
  oled.print(analogRead(probe) / PRECISAO * 3.3);
}
void drawGrade(void){
  for(int ix=1; ix<=100;ix+=10){
    for(int iy=1; iy<=64; iy+=8){
      oled.drawPixel(ix, iy);
    }
  }
}

void drawWave(void) {
  for (xCounter = 0; xCounter <= W - 24; xCounter++)
  {
    oled.drawPixel(xCounter, (H - 1) - readings[xCounter]);
    if (xCounter > 1) {
      oled.drawLine(xCounter - 1, (H - 1) - readings[xCounter - 1], xCounter, (H - 1) - readings[xCounter]);
    }
  }
}

//void frequencimetro(){

void setup() {
  oled.setFont(u8g_font_4x6);
  ADCSRA &= ~PS_128;
  ADCSRA |= PS_16; // 16 prescaler
}

void loop() {
  delayP();
  if(digitalRead(SCREEN_UPDATE)){
    oled.firstPage();
    do {
      drawGrade();
      drawInf();
      drawWave();
//    frequencimetro();
    } while ( oled.nextPage() );
  }
}
