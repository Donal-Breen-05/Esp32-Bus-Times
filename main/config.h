#pragma once

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
inline const char* stop_id = "8230DB001112"; //number of bus stop to track times for
inline const char* stop_name = "Perrystown, Perrystown";
