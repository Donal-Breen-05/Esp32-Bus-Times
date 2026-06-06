#pragma once

#include <WebServer.h>

// adding inline means that the config variables are only made once 

//config
inline const char* wifi_SSID = "WIFINAME";
inline const char* wifi_password = "password";

//api 
inline const char* api_host = "api-lts.transportforireland.ie";
inline const char* api_key = "630688984d38409689932a37a8641bb9";
inline const uint16_t api_port = 443;
inline const char* api_path = "/lts/lts/v1/public/departures";

// bus stop 
//inline const char* stop_num = "2324"; 
//inline String stop_id = "8230DB00" + String(stop_num); //number of bus stop to track times for
//inline const char* stop_name = "Perrystown, Perrystown";// not needed 

inline String stop_num = "1112";

//dynamically generate the id for the stop 
inline String getStopId() {

    return "8230DB00" + stop_num;

}//end getStopId 

//webserver to change the stop number 
inline WebServer server(80); 