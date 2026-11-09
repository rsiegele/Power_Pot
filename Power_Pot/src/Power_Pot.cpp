/*
 * Project myProject
 * Author: Your Name
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */


// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "adafruit_gfx.h"
#include "adafruit_bme280.h"
SYSTEM_MODE(AUTOMATIC);
// initialize pins
int AQpin = A2;    
int relayPin = D9;




// initialize I2C BUS variables


int BMEADDRESS = 0x76;
int OLEDADDRESS = 0x3c;
int OLEDRESET = -1;
int tempC, tempF, pasc, inHg, humidity, lastTime, BME;




// initialize variables
int airQ;
int lastWtr, sinceLastWtr;
int wtrInterval;
int
// initialize functions
void pump(int duration);
void fetchBME();
void fetchAQ();
void dispOLED();
void printAll(int delay);




int readingAQ, readingBME;
SYSTEM_THREAD(ENABLED);




SerialLogHandler logHandler(LOG_LEVEL_INFO);
Adafruit_SSD1306 display(OLEDRESET);
Adafruit_BME280 bme(BME);




// setup() runs once, when the device is first turned on
void setup() {
 Serial.begin(9600);
 waitFor(Serial.isConnected, 10000);
 pinMode(relayPin, OUTPUT);
 
 bme.begin(BMEADDRESS);
 display.begin(SSD1306_SWITCHCAPVCC, OLEDADDRESS);
 display.display();
 delay(200);
 display.clearDisplay();




}


// loop() runs over and over again, as quickly as it can execute.
void loop() {
 printAll(50);
 delay(1000);




}






// Turns on the pump for specified duration
void pump(int duration){
 digitalWrite(relayPin, HIGH);
  delay(duration/2);
  digitalWrite(relayPin, LOW);
  delay(duration/2);
 sinceLastWtr = millis();


}






// Read BME information
void fetchBME(){
tempC = bme.readTemperature();
pasc = bme.readPressure();
humidity = bme.readHumidity();
}








// Read air quality sensor information
void fetchAQ(){
readingAQ = analogRead(AQpin);
}






// Displays the information
void dispOLED(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  lastWtr = (millis() - sinceLastWtr)/60000; //Converts time since last water to minutes
  display.printf("Last watered:%i", lastWtr);
  display.display();
}


void printAll(int delay){


  Serial.printf("Temp:%i\nPressure:%i\nHumidity:%i\nAir quality:%i\nDirt moisture%i\n", tempC, pasc, humidity, airQuality, moisture);




}

