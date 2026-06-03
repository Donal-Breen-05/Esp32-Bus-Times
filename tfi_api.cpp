//headers 
#include <WiFi.h>// core esp 
#include <WiFiClientSecure.h>//core esp 
#include <ArduinoJson.h>// library written by Benoit Blanchon 

//headers 
#include "config.h"
#include "transport_api.h"

//fetch data from the api
void get_bus_times(){

  //if wifi not connected
  if(WiFi.status() != WL_CONNECTED) {

    Serial.println("ERROR - not connected");
    return;

  }// end if

  Serial.println("HEAP - " + String(ESP.getFreeHeap()));

  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect(api , port)){

    Serial.println("ERROR - connection failed");
    return;

  }//end if

  //send request
  String path = "/gtfsr/v2/gtfsr?format=json";

  client.println("GET " + path + " HTTP/1.0");
  client.println("Host: " + String(api));
  client.println("x-api-key: " + String(key));
  client.println("Accept: application/json");
  client.println("Accept-Encoding: identity");
  client.println("Connection: close");
  client.println();

  //skip headers
  unsigned long timeout = millis() + 1000;

  while (client.connected() && millis() < timeout) {

    String line = client.readStringUntil('\n');
    line.trim();

    if (line.length() == 0) break;

  }// end while

  delay(100);

  //filtering the json
  StaticJsonDocument<512> filter;

  filter["entity"][0]["trip_update"]["trip"]["route_id"] = true;
  filter["entity"][0]["trip_update"]["stop_time_update"][0]["stop_id"] = true;
  filter["entity"][0]["trip_update"]["stop_time_update"][0]["arrival"]["time"] = true;

  StaticJsonDocument<4096> doc;

  DeserializationError error =

    deserializeJson(

      doc,
      client,
      DeserializationOption::Filter(filter)

    );

  client.stop();

  //if error
  if(error) {

    Serial.println("ERROR - parse failed ");
    Serial.println("HEAP" + String(ESP.getFreeHeap()));
    return;

  }//end if

}//end bus times
