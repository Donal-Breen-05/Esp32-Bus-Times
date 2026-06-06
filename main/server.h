#pragma once 
#include "config.h" 

inline void start_server(){ 
  
  server.on("/", [](){
      
      // website 
      String html = 
        "<html><body style='font-family:sans-serif;max-width:400px;margin:40px auto'>"
      "<h2>ESP32 Bus Tracker</h2>"
      "<p>Current stop: <b>" + stop_num + "</b></p>"
      "<form method='POST' action='/set'>"
      "<label>Stop Number:</label><br>"
      "<input type='text' name='stop' value='" + stop_num + "' style='padding:8px;font-size:16px'><br><br>"
      "<input type='submit' value='Update' style='padding:8px 16px;font-size:16px'>"
      "</form>"
      "</body></html>";
      }); 

}// end start_server
