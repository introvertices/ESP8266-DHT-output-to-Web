#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

//WIFI and FTP creds
//###### EDIT THIS!!!!!
const char *ssid="SSID";
const char *password="PW";


void read_settings(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // This will print the current readout to the serial monitor for testing
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


  // Pass on to webserver

  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) 
    {
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

      // Ignore SSL certificate validation
      client->setInsecure();
    
      //create an HTTPClient instance
      HTTPClient https;
    
      //Initializing an HTTPS communication using the secure client
      Serial.print("[HTTPS] begin...\n");

      //Send values to PHP
      String curr_read;
      curr_read = "?temp=" + String(t);
      curr_read += "&humid=" + String(h);
      
      //###### EDIT THIS!!!!!
      String dataline = "WEB PATH TO subtemp.php";
      dataline += curr_read;

      // Check HTTP validation and results
      bool result = https.begin(*client, dataline);

      if (!result)
      {
        Serial.println("Invalid request");
      }
      else
      {
        int httpCode = https.GET();
        if (httpCode > 0)
        {
          Serial.printf("HTTP Status: %d Message: ", httpCode);
          String payload = https.getString();
          Serial.println(payload);
        }
        else
        {
          Serial.printf("HTTP request failed. Error: %s\r\n", https.errorToString(httpCode).c_str());
        }
      }

      Serial.println(result);    //printing the respose from the submitted data
      https.end();  //Close connection  


    }
}

void wifi_connect(){
  WiFi.mode(WIFI_OFF); 
  delay(1000);
  WiFi.mode(WIFI_STA); //WiFi Station Mode default mode is both station and acccess point modes
 
  WiFi.begin(ssid, password); //connecting to router
  Serial.println("");
  Serial.print("Connecting");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  

}


//#### REQUIRED FUNCTIONS

void setup() {
  Serial.begin(115200);

  //connect to wireless
  wifi_connect();

  Serial.println(F("DHT11 test!"));  
  dht.begin();
}



void loop() {
  // Wait 5 mins between measurements.
  delay(30000);

  read_settings();

  
}
