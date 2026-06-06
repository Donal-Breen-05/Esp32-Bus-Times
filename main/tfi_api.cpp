#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "config.h"
#include "set_time.h"
#include "tfi_api.h"

// Reads a chunked HTTP body and returns the decoded payload
String read_chunked_body(WiFiClientSecure &client) {
  String result = "";
  while (client.connected() || client.available()) {
    // Read chunk size line (hex digits followed by \r\n)
    String sizeLine = client.readStringUntil('\n');
    sizeLine.trim();

    // Strip chunk extensions if present (e.g. "1a3;ext=val")
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
      }
    }

    result += chunk;

    // Consume trailing \r\n after chunk data
    client.readStringUntil('\n');
  }
  return result;
}

// Helper: parse "HH:MM" or ISO "YYYY-MM-DDTHH:MM:SS..." into minutes since midnight
int to_minutes(const String &timeStr) {
  if (timeStr.length() < 5) return -1;
  
  // ISO format: "2026-06-05T19:59:45.000+00:00" — extract the T## part
  int tPos = timeStr.indexOf('T');
  if (tPos != -1) {
    int hours = timeStr.substring(tPos + 1, tPos + 3).toInt();
    int mins  = timeStr.substring(tPos + 4, tPos + 6).toInt();
    return hours * 60 + mins;
  }

  // Short format: "HH:MM" or "HH:MM:SS"
  int hours = timeStr.substring(0, 2).toInt();
  int mins  = timeStr.substring(3, 5).toInt();
  return hours * 60 + mins;
}

int get_bus_times(Bus busArr[3]) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR - not connected");
    return 0;
  }


  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect(api_host, api_port)) {
    Serial.println("ERROR - connection failed");
    return 0;
  }

  String now = get_time();
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

  client.println("POST " + String(api_path) + " HTTP/1.1");
  client.println("Host: " + String(api_host));
  client.println("Ocp-Apim-Subscription-Key: " + String(api_key));
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(body.length());
  client.println();
  client.print(body);

  // Wait for response
  unsigned long timeout = millis();
  while (!client.available()) {
    delay(10);
    if (!client.connected()) {
      Serial.println("ERROR - server disconnected");
      return 0; 
    }
    if (millis() - timeout > 10000) {
      Serial.println("ERROR - response timeout");
      return 0;
    }
  }

  // Read HTTP status line
  String status = client.readStringUntil('\n');
  status.trim();
  Serial.println(status);
  if (!status.startsWith("HTTP/1.1 200")) {
    Serial.println("ERROR - bad HTTP response");
    return 0;
  }

  // Read headers, detect chunked transfer encoding
  bool isChunked = false;
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    line.trim();
    if (line.equalsIgnoreCase("transfer-encoding: chunked")) {
      isChunked = true;
    }
    if (line.length() == 0) break; // blank line = end of headers
  }

  // Read body
  String payload = isChunked ? read_chunked_body(client) : client.readString();
  client.stop();

  Serial.println("RAW JSON:");
  Serial.println(payload);

  if (payload.length() < 10) {
    Serial.println("ERROR - empty payload");
    return 0;
  }

  DynamicJsonDocument doc(32768);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("JSON ERROR: ");
    Serial.println(error.c_str());
    return 0;
  }

  JsonArray departures = doc["stopDepartures"];
  if (departures.isNull() || departures.size() == 0) {
    Serial.println("No departures found");
    return 0;
  }

  int now_mins = to_minutes(now);
  int count = 0 ; 
  for (JsonObject dep : departures) {

    //only show first 3 
    if (count >= 3) break; 

    String route = dep["serviceNumber"] | "";
    String dest  = dep["destination"]    | "";
    String time  = dep["realTimeDeparture"] | "";
    if (time == "") time = dep["scheduledDeparture"] | "";

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
      }
    }

    //update struct obj 
    busArr[count].route = route; 
    busArr[count].numMins = label; 
    busArr[count].destination = dest; 

    //debug print 
    Serial.println(route + " -> " + label + " (" + dest + ")");
    count++; 
  }

  return count; 
}