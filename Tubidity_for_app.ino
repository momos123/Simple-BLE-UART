#include <Wire.h>
#include <SPI.h>
//#include <KX022.h>
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include  <avr/dtostrf.h>
#include <Si114.h>
PulsePlug<> pulse(Wire);

#include <BLEPeripheral.h>
#include "BLESerial.h"
BLESerial bleSerial(10, 2, 9);
unsigned long long lastSent = 0;
#define OLED_WIDTH 64
#define OLED_HEIGHT 32
MicroOLED oled(OLED_RST, OLED_DC, OLED_CS); // (pin_rst, pin_dc, pin_cs)

float xyz[3];
uint32_t tPage;
bool B1_isPressed = false;
uint8_t page_num = 0;
const uint8_t page_count = 2;
void draw_page(uint8_t idx = 0);


void setup()
{
  Wire.begin();

 bleSerial.setLocalName("UART");
 bleSerial.begin();
  Serial.begin(9600);
  Serial.println(__FILE__);
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  oled.setScreenSize(OLED_WIDTH, OLED_HEIGHT);
  oled.begin();
  draw_page(page_num++);
  delay(1000); // show splash for 1s
  tPage = millis();
  
   while (pulse.isPresent() == false)
   {
      Serial.println(F("No SI114x foundxxx"));
      delay(1000);
   }
   
   Serial.println(F("SI114x Pulse Sensor found"));
   pulse.init();
}

void loop()
{
if (!B1_isPressed & !digitalRead(PIN_BUTTON1)) // timer used for button debounce
  {
    page_num = (page_num + 1 < page_count)?page_num+1:0;
  }
  B1_isPressed = !digitalRead(PIN_BUTTON1);
  
  if (millis() - tPage > 20) // 20ms = 50Hz
  {
    tPage = millis();
    draw_page(page_num);
  }





  yield();
  delay(1000); // show splash for 3s
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
}




void float2chars(float &in, char (&out)[5])
{
  bool sign_bit = (in < 0);
  uint16_t tmp = sign_bit ? (-in * 10) : (in * 10);
  out[0] = (sign_bit) ? '-' : ' ';
  out[1] = char('0' + (tmp / 10));
  out[2] = '.';
  out[3] = char('0' + (tmp % 10));
  out[4] = '\0';
}

void draw_page(uint8_t idx)
{
  switch(idx)
  {
    case 1:
      page_accelerometer(); break;
    default:
      page_startup();
    break;
  }
}

void page_startup()
{
  oled.clear();
  //oled.drawString(0,0,"github.com/");
  //oled.drawString(0,10,"micooke");
//  oled.drawString(0,20,__TIME__);
  oled.display();
}



void page_accelerometer()
{
    pulse.readSensor(3); 
   Serial.print(F("Active LEDs [red, ir1, ir2] = ["));
   Serial.print(pulse.led_red); 
   Serial.print(F(", "));
   Serial.print(pulse.led_ir1);
   Serial.print(F(", "));
   Serial.print(pulse.led_ir2);
   Serial.println(F("]"));
   Serial.print(F("Ambiant LEDs [visible, ir] = ["));
   Serial.print(pulse.als_vis);
   Serial.print(F(", "));
   Serial.print(pulse.als_ir); 
   Serial.println(F("]"));
   float total=pulse.led_red+pulse.led_ir1+pulse.led_ir2;
    
    char fltBuf[5];
    char charVal[10]; 
    
    oled.clear();
    oled.setCursor(5, 4); // points cursor to x=27 y=0
    oled.print("Vis");
    oled.setCursor(25, 4); // points cursor to x=27 y=0
    dtostrf(pulse.als_vis,5,0,charVal);
    oled.print(charVal);
    
    oled.setCursor(5, 20); // points cursor to x=27 y=0
    oled.print("IR");
    oled.setCursor(25, 20); // points cursor to x=27 y=0
   // float2chars(pulse.als_ir,fltBuf);
    dtostrf(pulse.als_ir,5,0,charVal);
    oled.print(charVal);
    oled.display();  


    bleSerial.print("V");
    bleSerial.print(pulse.led_red);
    bleSerial.print(",");
    bleSerial.print("I");
    bleSerial.print(pulse.led_ir1);
}


