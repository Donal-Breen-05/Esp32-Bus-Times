// dependencies 
#include <WiFi.h> //core esp 


//config 
const char* wifi_SSID = "WIFINAME"; 
const char* wifi_password = "password"; 




void connect_wifi(){ 
  
  int attempts = 0 ;

  //disconnect then reconnect 
  WiFi.disconnect(true); 
  delay(1000); 
  WiFi.mode(WIFI_STA); 
  delay(500); 
  WiFi.begin(wifi_SSID, wifi_password);//connect 
  
  Serial.print("\nConnecting to the Wifi \"%s\"" , wifi_SSID); 
  
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
