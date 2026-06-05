//headers 
#include <WiFi.h>// core esp 
#include <WiFiClientSecure.h>//core esp 
#include <ArduinoJson.h>// library written by Benoit Blanchon 

//headers 
#include "config.h"
#include "set_time.h"
#include "tfi_api.h"

String read_chunked_body(WfifClientSecure &client) { 

  String result = ""; 

  while (client.connected() || client.available()) {
    
    // Read chunk size line (hex digits followed by \r\n)
    String sizeLine = client.readStringUntil('\n');
    sizeLine.trim();

    // Strip chunk extensions 
    int semiPos = sizeLine.indexOf(';');
    if (semiPos != -1) sizeLine = sizeLine.substring(0, semiPos);

    if (sizeLine.length() == 0) continue;

    long chunkSize = strtol(sizeLine.c_str(), nullptr, 16);
    if (chunkSize == 0) break; // last chunk

    // Read exactly chunkSize bytes
    String chunk = "";
    long remaining = chunkSize;
    
    while (remaining > 0 && (client.connected() || client.available())) {
      
      if (client.available()) {
        
        chunk += (char)client.read();
        remaining--;
        
      } else {
        
        delay(5);
        
      }//end else 
      
    }// end while
    
    result += chunk; 

    // consume trailing \r\n 
    client.readStringUntil('n');
    
  }// end while 
  return result; 
}// read_chunked_body


//YYYY-MM-DDTHH:MM:SS into minutes since midnight
int to_minutes(const String &timeStr) {
  if (timeStr.length() < 5) return -1;
  
  int tPos = timeStr.indexOf('T');
  if (tPos != -1) {
    
    int hours = timeStr.substring(tPos + 1, tPos + 3).toInt();
    int mins  = timeStr.substring(tPos + 4, tPos + 6).toInt();
    return hours * 60 + mins;
    
  }//end if 


  int hours = timeStr.substring(0, 2).toInt();
  int mins  = timeStr.substring(3, 5).toInt();
  return hours * 60 + mins;
  
}//to minuites 

//fetch data from the api
int get_bus_times(Bus busArr[4){

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

  //wait for response 
  unsigned long timeout = millis(); 
  while(!client.available()) {
    delay(10); 
    if (!client.connected()) {
      Serial.println("ERROR - server disconnected");
      return;
    }// end if 
    if (millis() - timeout > 10000) {
      Serial.println("ERROR - response timeout");
      return;
    }// end if 

  }// while 

  
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
  
  //detect chunked transfer encoding
  bool isChunked = false;

  while (client.connected()) {
    
    String line = client.readStringUntil('\n');
    line.trim();
    
    if (line.equalsIgnoreCase("transfer-encoding: chunked")) {
      
      isChunked = true;
      
    }// end if 
    
    if (line.length() == 0) break; //end of header 
    
  }// end while 
  
  // Read body
  String payload = isChunked ? read_chunked_body(client) : client.readString();
  client.stop();

  Serial.println("RAW JSON:");
  Serial.println(payload);

  //empty resonse 
  if (payload.length() < 10) {
    
    Serial.println("ERROR - empty payload");
    return;
    
  }//end if 

  //32kb 
  DynamicJsonDocument doc(32768);
  DeserializationError error = deserializeJson(doc, payload);

  //if error
  if(error) {

    Serial.println("ERROR - parse failed ");
    Serial.println("HEAP" + String(ESP.getFreeHeap()));
    return;

  }//end if
  
  // check departures exist 
  JsonArray departures = doc["stopDepartures"];
  if (departures.isNull() || departures.size() == 0) {
    
    Serial.println("No departures found");
    return;
    
  }// end if 

  int now_mins = to_minutes(now);
  int count = 0 ; 

  for(JsonObject dep : departures) {

    //only show first 4 
    if (count >= 4) break; 

    String route = dep["serviceNumber"] | "";
    String destination = dep["destination"] | "";
    String departure = dep["realTimeDeparture"] | "";

    if( (time == "") time = dep["scheduledDeparture"] | "") {

      // show the mins or due 
      String label = "";
      int dep_mins = to_minutes(time);
      if (now_mins >= 0 && dep_mins >= 0) {
        
        int diff = dep_mins - now_mins;
        if (diff < 0) {
          
          label = "departed";
          
        } else if (diff < 1) {
          
          label = "Due";
        
        } else {
          
          label = String(diff) + " min" + (diff == 1 ? "" : "s");
        
        }// end else 
        
      }//end if
      
    } // end if 

    //update struct obj 
    busArr[count].route = route; 
    busArr[count].numMins = label; 
    busArr[count].destination = dest; 
    
    //debug print 
    Serial.println(route + " -> " + label + " (" + dest + ")");
    count++; 
    
  }// end for 

}//end bus times




