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
#include "grove_air_quality_sensor.h"
SYSTEM_MODE(AUTOMATIC);
// initialize pins
int AQpin1 = A1;    
int AQpin2 = A5;
int relayPin = D2;
int moisturePin = A2;




// initialize I2C BUS variables


int BMEADDRESS = 0x76;
int OLEDADDRESS = 0x3c;
int OLEDRESET = -1;
int tempC, tempF, pasc, inHg, humidity, lastTime, BME;




// initialize variables
int airQuality1, airQuality2;
int lastWtr, sinceLastWtr;
int wtrInterval;
int moisture;
int pTimer, pTimer1, duration2;
int pumpTimer;
int pumpDuration = 30000;


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
Adafruit_BME280 bme;




// setup() runs once, when the device is first turned on
void setup() {
 Serial.begin(9600);
 waitFor(Serial.isConnected, 10000);
 pinMode(relayPin, OUTPUT);
 pinMode(moisturePin, INPUT);


 bme.begin(BMEADDRESS);
 display.begin(SSD1306_SWITCHCAPVCC, OLEDADDRESS);
 display.display();
 delay(2000);
 display.clearDisplay();




}


// loop() runs over and over again, as quickly as it can execute.
void loop() {
if((millis() - pumpTimer)>= pumpDuration){
  pump(5000);
  pumpTimer = millis();


}


fetchBME();
fetchAQ();
dispOLED();
}






// Turns on the pump for specified duration
void pump(int duration){
 duration2 = duration+1000;
 if((millis() - pTimer)>=duration){


 digitalWrite(relayPin, HIGH);
pTimer = millis();
 }
 if((millis() - pTimer1)>=duration2){
  digitalWrite(relayPin, LOW);
pTimer1 = millis();
 }
 


 
 
 sinceLastWtr =millis();


}






// Read BME information
void fetchBME(){
tempC = bme.readTemperature();
pasc = bme.readPressure();
humidity = bme.readHumidity();
moisture = analogRead(moisturePin);
inHg = pasc/3386;
tempF = (tempC*9/5)+32;
}








// Read air quality sensor information
void fetchAQ(){
airQuality1 = analogRead(AQpin1);
airQuality2 = analogRead(AQpin2);
}






// Displays the information
void dispOLED(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  lastWtr = (millis() - sinceLastWtr)/60000; //Converts time since last water to minutes
  display.printf("Temp:%i\nPressure:%i\nHumidity:%i\nDirt moisture%i\nLarge Particles:%i\nSmall Particles:%i\n Last Watered:%i\n", tempF, inHg, humidity, moisture, airQuality1, airQuality2, lastWtr);
  display.display();
}


void printAll(int delay){


  Serial.printf("Temp:%i\nPressure:%i\nHumidity:%i\nAir quality:%i\nDirt moisture%i\n", tempC, pasc, humidity, airQuality1, moisture);




}



