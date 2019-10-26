#include <dummy.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#include <WiFi.h>         //Librarys needed for HTTP requests
#include <HTTPClient.h>   //---

#include <ArduinoJson.h>
const char* ssid = "GRANDGARAGE-PUBLIC";
const char* password = "wifi4members";

Adafruit_BMP085 bmp;
HTTPClient http;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.write("wait 4 seconds before starting wifi");
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) //connecting to the wifi
  {                                     //---
    delay(1000);                        //---
    Serial.write("Connecting...");      //---
  }                                     //---
  Serial.write("connected. 41");        //---
                                     
  if (!bmp.begin()) {               //checking the air presure sensor                
  Serial.write("A error occured");  //Error message   
  //while (1) {}                      //---
  }
}
  
  //int light = 32;
  //int lightvalue = 0;
  //int pa = 0;
  //int ce = 0;


void loop() {
  // put your main code here, to run repeatedly:
  //Demo Code for Testing
  Serial.write("reached loop");
  int ce = bmp.readTemperature();
  int pa = bmp.readPressure();
  int lightvalue = analogRead(32);
  Serial.write(lightvalue);
  Serial.write("light");
  Serial.write(pa);
  Serial.write("pressure");
  Serial.write(ce);
  Serial.write("Temparature");
  delay(2000);

  Serial.write("Contacting server...");
  http.begin(""); //Contact server
  Serial.write("connected. 42");

  http.addHeader("Content-Type", "text/plain");           //send post request
  int httpCode = http.POST("POSTING from ESP32"); //---

  if (httpCode > 0)                     //Check if everything worked
  {                                     //---
    String payload = http.getString();  //---
    Serial.write(httpCode);             //print the failure code
    Serial.print(payload);              //---
  }else                                 //---
  {                                     //---
    Serial.write("failure.");           //---
  }                                     //---

  http.end(); //end
  Serial.write("end the http request.");

}
  
