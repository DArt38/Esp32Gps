#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
char   datoCmd = 0;


#define NMEA 0
#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to initialize SSD1306 OLED display"));
    while (1);
  }

  delay(1000);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
}

void loop() {
  if (NMEA)
 {
    while (neogps.available())
    {
     datoCmd  = (char)neogps.read(); 
     Serial.print (datoCmd );
    }
 } 

 else
 {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;         
      }
    }
  }  

  if(newData == true)
  {
    newData = false;
    Serial.print("Numero de Satelites: ");
    Serial.println(gps.satellites.value());    
    ActualizarPantallaOLED();
  }
  else
  {
    
  }  
  
 }
}


void ActualizarPantallaOLED() {
  oled.clearDisplay();

  if (gps.location.isValid())
  {
    oled.setCursor(24, 0);
    oled.println("dArt");
    oled.setCursor(0, 20);
    oled.println("Lat: " + String(gps.location.lat(), 6));
    oled.println("Lng: " + String(gps.location.lng(), 6));
    oled.println("Speed: " + String(gps.speed.kmph()) + " km/h");
    oled.println("Sat: " + String(gps.satellites.value()));
    oled.display();
  }
  else
  {
    oled.setCursor(4, 10);
    oled.println("No GPS Signal");
    oled.display();
  }
}
