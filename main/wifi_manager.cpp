
//dependancies 
#include <WiFi.h>//core esp 

//headers 
#include "config.h"
#include "wifi_manager.h"

//connect wifi
void connect_to_wifi(){  // had to rename(similar name to inbuilt function)

  int attempts = 0 ;

  //disconnect then reconnect
  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_STA);
  delay(500);

  WiFi.begin(wifi_SSID, wifi_password);//connect

  Serial.print("\nConnecting to the Wifi ");
  Serial.print(wifi_SSID);

  while(WiFi.status() != WL_CONNECTED) {

    delay(500);
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
