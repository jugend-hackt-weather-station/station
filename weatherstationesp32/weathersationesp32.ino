#include <dummy.h>
#include <WiFi.h>         //Librarys needed for HTTP requests
#include <HTTPClient.h>   //---

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
//#include <ArduinoJson.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;
const char* ssid = "GRANDGARAGE-PUBLIC";   //WiFi SSID
const char* password = "wifi4members";     //WiFi password

void setup() {
  Serial.begin(9600); //start the serial connection
  dht.begin();        //start the library for the humidity and temperature sensor
  Serial.write("wait 4 seconds before starting wifi");
  
  delay(4000);                          //1. connecting to the wifi
  WiFi.begin(ssid, password);           //---
  while (WiFi.status() != WL_CONNECTED) //---
  {                                     //---
    delay(1000);                        //---
    Serial.write("Connecting...");      //---
  }                                     //---
                                        //---
  Serial.write("connected.");           //---

  if (!bmp.begin()) {                                                       //Check the pressure sensor status
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");  //---
    while (1) {}                                                            //---
  }                                                                         //---

  pinMode(32, INPUT);
  pinMode(33, INPUT);
}

void loop() {
  float humid = dht.readHumidity();                       //get the sensor information
  float temp = dht.readTemperature();                     //---
  float light = 100 - analogRead(32) / 40.95;             //---
  float CO = analogRead(33);                              //---
  float pressure = bmp.readPressure();                    //---
  float longitude = 14.300827;                            //Hardcoded float
  float latitude = 48.319259;                             //Hardcoded float
  if (isnan(humid) || isnan(temp)) {                      //Check the Humidity and temperature sensor
    Serial.println(F("Failed to read from DHT sensor!")); //---
    return;                                               //---
  }                                                       //---

  Serial.print(F("Humidity: "));    //print all values
  Serial.print(humid);              //---
  Serial.println("%");              //---
  Serial.print(F("Temperature: ")); //---
  Serial.print(temp);               //---
  Serial.println(F("°C "));         //---
  Serial.print("Pressure = ");      //---
  Serial.print(pressure);           //---
  Serial.println(" Pa");            //---
  Serial.print("CO Lelvel: ");      //---
  Serial.println(CO);               //---
  Serial.print("Light Level: ");    //---
  Serial.print(light);              //---
  Serial.println("%");              //---
  Serial.println();                 //---

  if (WiFi.status() == WL_CONNECTED) {           //Check the WiFi status
    HTTPClient http;
    Serial.write("Contacting server...\n");     //Contact server
    http.begin("http://10.6.0.155:5000/submit");//---
    Serial.write("connected.\n");               //---
    /*String *data;
      StaticJsonDocument<1024> doc;
      doc["temperature"] = -10;
      serializeJson(doc, data);*/    
    http.addHeader("Content-Type", "application/json");           //send post request (json)
    
    String body = "{";                                            //Create a string to post to the server
    //body += "\"temperature\": 42"; demo hard coded temperature  //---
    body+= "\"temperature\": " + String(temp) + ",";              //---
    body+= "\"carbonmonoxide\": " + String(CO) + ",";             //---
    body+= "\"humidity\": " + String(humid) + ",";                //---
    body+= "\"pressure\": " + String(pressure) + ",";             //---
    body+= "\"longitude\": " + String(longitude) + ",";           //Hardcoded string
    body+= "\"latitude\": " + String(latitude) + ",";             //Hardcoded string
    body+= "\"brightness\": " + String(light);                    //---
    body+= '}';                                                   //---
    Serial.println(body);                                         //Print the final string
    
    int httpCode = http.POST(body);       //send the post request
    Serial.println(httpCode);             //---

    if (httpCode > 0)                     //Check if everything worked
    {                                     //---
      String payload = http.getString();  //---
      Serial.write(httpCode);             //print the failure code
      Serial.println(payload);            //---
    } else                                //---
    {                                     //---
      Serial.write("failure.");           //---
    }                                     //---
    http.end(); //end                     //---
    Serial.write("end the http request.");//---

  }

  delay(5000);  //Wait till next "scan"
}
