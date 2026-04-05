// dependencies 
#include <WiFi.h> //core esp 
#include <WiFiClientSecure.h> //core esp 
#include <ArduinoJson.h> //Library written by Benoit Blanchon

//config 
const char* wifi_SSID = "WIFINAME"; 
const char* wifi_password = "password"; 
const char* api = "https://api.nationaltransport.ie/gtfsr/v2/gtfsr"; 
const char* key = "YOUR_API_KEY"; //replace with your own api key 
const int* port = 443;
const int* stop_num = 1234; //number of bus stop to track times for 



void connect_wifi(){ 
  
  int attempts = 0 ;

  //disconnect then reconnect 
  WiFi.disconnect(true); 
  delay(1000); 
  WiFi.mode(WIFI_STA); 
  delay(500); 
  WiFi.begin(wifi_SSID, wifi_password);//connect 
  
  Serial.print("\nConnecting to the Wifi " + wifi_SSID); 
  
  while(WiFi.status() != WL_CONNECTED) { 

    delay(500) 
    Serial.print("\nConnecting........");
    
    //timeout 
    if (++attempts > 40){ 
      Serial.println("ERROR - could not connect"); 
      return; 
    }//end if     

  }//end while
  
  //if connected
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());


}//end connect_wifi 

//fetch data from the api 
void get_bus_times(const char* api ,const char* key , const int* port){ 

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
  }//end if 
  
  //send request 
  String path = api + stop_num; 
  client.println("GET " + path + " HTTP/1.0");
  client.println("Host: " + String(api));
  client.println("Accept: application/json");
  client.println("Accept-Encoding: identity");
  client.println("Connection: close");
  client.println();

  client.stop(); 

  if(error) { 
    Serial.println("ERROR - parse failed "); 
    Serial.println("HEAP" + String(ESP.getFreeHeap()));
  }//end if 
  

}//end bus times 

//setup 
void setup() { 
  Serial.begin(115200); //set baud rate 
  delay(1000); 

  //title 
  Serial.println("\n==============================");
  Serial.println("   ESP-32 Bus Times");
  Serial.println("==============================\n");

  //wifi 
  connect_wifi(); 

}// end serial 



//main loop 
void loop() { 
  //loop 
}//end loop 
