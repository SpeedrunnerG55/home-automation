

//speaker pins
#define SPK 4 //(gpio)

//it beeps and it sleeps
void beepSleep(int freq, int time){
  tone(SPK, freq, 50);
  delay(time);
}

void ramp(unsigned char type){
  if(type == 0){
    for(int hz = 1200; hz >= 400; hz -= 3){
      beepSleep(hz,1);
    }
  }
  else if(type == 1){
    for(int hz = 400; hz < 1200; hz += 3){
      beepSleep(hz,1);
    }
  } else if(type == 2){
    for(int hz = 1500; hz < 2400; hz += 4){
      beepSleep(hz,3);
    }
  }
}

void sound(unsigned char type){
  if(type ==  0) {
    for(int i = 0; i < 2; i++){
      delay(50);
      ramp(0);
      delay(50);
    }
  } else if(type == 1){
    for(int i = 0; i < 2; i++){
      delay(50);
      ramp(1);
      delay(50);
    }
  } else if(type == 2){
    delay(50);
    ramp(0);
    delay(200);
    ramp(1);
    delay(50);
  } else if(type == 3){
    ramp(2);
  }
}
//NEO PIXEL SETTUP
#include <Adafruit_NeoPixel.h>

// How many LEDS are attached to the Arduino?
#define numLED      6

// Which pin on the Arduino is connected to the LEDS? (GPIO)
#define LED_PIN     5

// When we setup the NeoPixel library, we tell it how many LED, and which pin to use to send signals.
// Note that for older NeoPixel rings you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel LED = Adafruit_NeoPixel(numLED, LED_PIN, NEO_GRB + NEO_KHZ800);

//define struct for color
struct color{
  //these values range from 0 - 255 (no brightness to full brightness)
  char red;
  char green;
  char blue;
};

//set color of all leds to a color
void LEDSet(struct color arg){
  //write analog pins for on time
  for (int i = 0; i < LED.numPixels(); i++) {
    LED.setPixelColor(i, LED.Color(arg.red,arg.green,arg.blue)); //set color.
  }
  delay(10);
  LED.show(); // This sends the updated pixel color to the hardware.
  delay(10);
}

void LEDoff(){
  //write analog pins for on time
  for (int i = 0; i < LED.numPixels(); i++) {
    LED.setPixelColor(i, LED.Color(0,0,0)); //set color.
  }
  delay(10);
  LED.show(); // This sends the updated pixel color to the hardware.
}

//blinks led a color for some time then turns it of for some time
void LEDBlink(struct color arg, int onTime, int offTime){
  LEDSet(arg); //write analog pins for on time
  delay(onTime); // Delay for a period of time (in milliseconds).
  LEDoff(); //turn off for off time
  delay(offTime);
}

// Fill the dots one after the other with a color
void colorWipe(struct color arg, int wait) {
  for(uint16_t i=0;i < LED.numPixels(); i++) {
    LED.setPixelColor(i,LED.Color(arg.red,arg.green,arg.blue));
    delay(wait);
    LED.show();
    delay(wait);
  }
}

void initialiseLED(){
  LED.begin(); // This initializes the NeoPixel library.

  struct color start;
  start.red   = 150;
  start.green = 10;
  start.blue  = 30;

  LEDSet(start);
  beepSleep(300,50);
  beepSleep(500,50);
  LEDBlink(start,300,0);
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
struct color Wheel(byte WheelPos) {
  struct color ret;
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    ret.red = 255 - WheelPos * 3;
    ret.green = 0;
    ret.blue =  WheelPos * 3;
    return ret;
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    ret.red = 0;
    ret.green = WheelPos * 3;
    ret.blue =  255 - WheelPos * 3;
    return ret;
  }
  WheelPos -= 170;
  ret.red = WheelPos * 3;
  ret.green = 255 - WheelPos * 3;
  ret.blue =  0;
  return ret;
}

void LEDCycle(struct color arg, int onTime, int offTime){

  //write analog pins for on time
  for (int i = 0; i < LED.numPixels(); i++) {
    LED.setPixelColor(i, LED.Color(arg.red,arg.green,arg.blue)); //set color.
    LED.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  }

  delay(onTime); // Delay for a period of time (in milliseconds).

  //turn off for off time
  for (int i = 0; i < LED.numPixels(); i++) {
    LED.setPixelColor(i, LED.Color(0,0,0)); //set color.
    LED.show(); // This sends the updated pixel color to the hardware.
    delay(50);
  }

  delay(offTime);
}

void LEDFade(struct color arg, int onTime, int offTime){
  //write analog pins for on time
  for (float j = 0; j < 255; j++) {
    for (int i = 0; i < LED.numPixels(); i++) {
      LED.setPixelColor(i, LED.Color(j / 255 * arg.red,j / 255 * arg.green,j / 255 * arg.blue)); //set color.
    }
    delay(1);
    delay(onTime); // Delay for a period of time (in milliseconds).
    LED.show(); // This sends the updated pixel color to the hardware.
  }

  //turn off for off time
  for (float j = 255; j >= 0; j--) {
    for (int i = 0; i < LED.numPixels(); i++) {
      LED.setPixelColor(i, LED.Color(j / 255 * arg.red,j / 255 * arg.green,j / 255 * arg.blue)); //set color.
    }
    delay(1);
    delay(offTime);
    LED.show(); // This sends the updated pixel color to the hardware.
  }
}

void error(int type){

  //define color to indicate connecting to wifi
  struct color wifi;
  wifi.red   = 0;
  wifi.green = 128;
  wifi.blue  = 128;

  //define color to indicate connecting to MQTT
  struct color mqtt;
  mqtt.red   = 128;
  mqtt.green = 0;
  mqtt.blue  = 128;

  //define color to indicate connecting to HTTP
  struct color HTTP;
  HTTP.red   = 128;
  HTTP.green = 128;
  HTTP.blue  = 0;

  //define color to indicate connecting to MQTT
  struct color JSON;
  JSON.red   = 0;
  JSON.green = 128;
  JSON.blue  = 0;

  switch (type) {
    case 0:
    LEDSet(wifi);
    beepSleep(500,100);
    delay(50);
    beepSleep(500,100);
    LEDBlink(wifi,200,500); //blink this color while connecting to wifi
    break;
    case 1:
    LEDSet(mqtt);
    beepSleep(700,100);
    delay(50);
    beepSleep(700,100);
    LEDBlink(mqtt,200,500); //blink this color while connecting to wifi
    break;
    case 2:
    LEDSet(HTTP);
    beepSleep(900,100);
    delay(50);
    beepSleep(900,100);
    LEDBlink(HTTP,200,500); //blink this color while connecting to wifi
    case 3:
    LEDSet(JSON);
    beepSleep(1300,100);
    delay(50);
    beepSleep(1300,100);
    LEDBlink(JSON,200,500); //blink this color while connecting to wifi
    break;
  }
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <auth.h>

#include <ArduinoJson.h>

// #include <SPI.h>
//the city you want the weather for
char*  location= "07410,US";
char*  server = "api.openweathermap.org";
//open weather map api key
char*  apikey = "a44dae32b78c321c82e0ae0664d9357f";
int cnt = 1;

// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

WiFiClient client;

void connectWiFi(_Bool beep){

  //define color to indicate connecting to wifi
  struct color wifi;
  wifi.red   = 0;
  wifi.green = 128;
  wifi.blue  = 128;

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
  would try to act as both a client and an access-point and could cause
  network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password); //begin wifi connection

  while (WiFi.status() != WL_CONNECTED) {

    if(beep){
      error(0);
    }

    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  beepSleep(500,50);
  beepSleep(800,50);

  LEDFade(wifi,1,1); //blink this color while connecting to wifi
  LEDoff();
}

struct weatherInfo{
  float temp;
  int humidity;
  _Bool success;
};

struct weatherInfo getWeatherInside(){

  struct weatherInfo ret;

  _Bool readTemp, readHumididty;

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    readTemp = 0;
  }
  else {
    ret.temp = event.temperature;
    readTemp = 1;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    readHumididty = 0;
  }
  else {
    ret.humidity = event.relative_humidity;
    readHumididty = 1;
  }

  ret.success = readTemp && readHumididty;

  return ret;
}

struct weatherInfo getWeatherOutside() {

  struct weatherInfo ret;

  //just make sure its connected
  if (WiFi.status() != WL_CONNECTED) { //Check WiFi connection status
    Serial.println("not connected to wifi");
    connectWiFi(0);
  }

  HTTPClient http;  //Declare an object of class HTTPClient
  char request[150];
  snprintf(request,150,"http://%s/data/2.5/forecast?q=%s&cnt=%i&appid=%s&units=metric",server,location,cnt,apikey);
  http.begin(request);  //Specify request destination
  int httpCode = http.GET();//Send the request
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();   //Get the request response payload

    //create a json buffer where to store the json data
    StaticJsonDocument<5000> doc;
    DeserializationError Jsonerror = deserializeJson(doc,payload);

    if (Jsonerror) {
      ret.success = 0;
      Serial.print("deserializeJson() failed with code ");
      Serial.println(Jsonerror.c_str());
      error(3);
    } else {
      ret.success = 1;
      //get the data from the json tree
      String nextWeatherTime = doc["list"][0]["dt_txt"];
      float temp = doc["list"][0]["main"]["temp"];
      int humidity = doc["list"][0]["main"]["humidity"];

      ret.temp = temp;
      ret.humidity = humidity;

      // Print values.
      Serial.println(nextWeatherTime);
    }

  } else {
    Serial.println("httpCode ");
    error(2);
  }
  http.end();   //Close connection

  return ret;

}

//this code runs once at startup
void setup() {
  Serial.begin(115200);//used for diagnostics
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));

  pinMode(D6,OUTPUT); //relay

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  initialiseLED();
  connectWiFi(1);
}

int onMargin = 10;
int offMargin = 2;

void loop() {

  struct weatherInfo in, out;
  out = getWeatherOutside();
  in = getWeatherInside();

  if(in.success && out.success){
    if(in.temp > out.temp + onMargin){
      digitalWrite(D6,HIGH);
    } else if(in.temp <= out.temp + offMargin){
      digitalWrite(D6,LOW);
    }
  }

  delay(60000);    //Send a request every 60 seconds
}
