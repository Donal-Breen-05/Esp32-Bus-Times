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

        //send to server 
        server.send(200, "text/html", html);
      }); 

  server.on("/set", HTTP_POST, []() { 
      
      if (server.hasArg("stop")) { 
          
        stop_num = server.arg("stop"); 
        Serial.println("stop number updated to " + stop_num);


        //after change 
        String html = 
          "<html><body style='font-family:sans-serif;max-width:400px;margin:40px auto'>"
          "<h2>Updated!</h2>"
          "<p>Stop set to: <b>" + stop_num + "</b> </p>"
          "<a href='/'>Back</a>"
          "</body></html>";

          // send to server 
          server.send(200, "text/html", html);
      } else { 
        server.send(400 , "text/plain" , "missing stop paramater"); 
      }//end else 
      });
  
  //start server 
  server.begin();
  Serial.println("Config server started");
  Serial.print("Visit http://");
  Serial.println(WiFi.localIP());

}// end start_server
