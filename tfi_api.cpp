//headers 
#include <WiFi.h>// core esp 
#include <WiFiClientSecure.h>//core esp 
#include <ArduinoJson.h>// library written by Benoit Blanchon 

//headers 
#include "config.h"
#include "set_time.h"
#include "tfi_api.h"

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

  if (!client.connect(api_host , api_port)){

    Serial.println("ERROR - connection failed");
    return;

  }//end if
   
  //Get Current time 
  String now = get_time(); 
  
  //request body 
  String body =
        "{"
        "\"clientTimeZoneOffsetInMS\":0,"
        "\"departureDate\":\"" + now + "\","
        "\"departureTime\":\"" + now + "\","
        "\"stopIds\":[\"" + String(stop_id) + "\"],"
        "\"stopType\":\"BUS_STOP\","
        "\"stopName\":\"" + String(stop_name) + "\","
        "\"requestTime\":\"" + now + "\","
        "\"departureOrArrival\":\"DEPARTURE\","
        "\"refresh\":false"
        "}";

  //send request
  client.println("POST " + String(api_path) + " HTTP/1.1");

  // headers
  client.println("Host: " + String(api_host));

  //api key 
  client.println("Ocp-Apim-Subscription-Key: " + String(api_key));

  client.println("Content-Type: application/json");

  //close connection 
  client.println("Connection: close");

  client.print("Content-Length: ");

  client.println(body.length());

  client.println();

  // body
  client.print(body);

  client.flush();

  //waits for availability 
  while(!client.available()) {

    delay(10);

    if(!client.connected()) {

        Serial.println(
            "ERROR - server disconnected"
        );

        return;

    }// end if 

  }//end while 

  
  //check http status 
  String status = client.readStringUntil('\n');

  status.trim();

  Serial.println(status);

  // if bad http response
  if(!status.startsWith("HTTP/1.1 200")){

      Serial.println("ERROR - bad response");

      client.stop();
      return;

  }// end if 
  
  //skip remaining headers 
  while(client.connected()) {

    String line = client.readStringUntil('\n');

    line.trim();

    if(line.length() == 0) {

        break;
    }// end if 

  }// while 
  
  delay(100);
 
  StaticJsonDocument<256> filter;

  filter["stopDepartures"][0]["serviceNumber"] = true; // 15A , F2 , 150 etc 
  filter["stopDepartures"][0]["destination"] = true; // dame st , charlestown ........
  filter["stopDepartures"][0]["realTimeDeparture"] = true; 
  filter["stopDepartures"][0]["scheduledDeparture"] = true; 

  //json document
  StaticJsonDocument<4096> doc;


   //parse json
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
  
  
  //print departures
  JsonArray departures =
    doc["stopDepartures"];

  //if departures is empty 
  if (departures.isNull()) { 

    Serial.println("Error - No departures"); 

  }// end if 

  for(JsonObject dep : departures) {

    String route = dep["serviceNumber"] | "";
    String destination = dep["destination"] | "";
    String departure = dep["realTimeDeparture"] | "";

    if(departure == "") {

      departure = dep["scheduledDeparture"] | "";

    } // end if 

    Serial.println(route + " -> " + departure + " (" + destination + ")" );

  }// end for 

}//end bus times




