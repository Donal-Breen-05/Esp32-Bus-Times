//headers
#include "config.h"
#include "wifi_manager.h"
#include "tfi_api.h"
#include "set_time.h"
#include "display.h"

//setup
void setup() {

  Serial.begin(115200); //set baud rate
  delay(1000);

  display_init();

  display_fill(COLOR_BLACK);

  display_set_text_size(3);
  display_set_text_color(COLOR_ORANGE);
  
  //title
  display_print(10, 20, "ESP-32 Bus Times");
  display_clear();

  //wifi & time 
  display_print(20, 20, "connecting......");
  connect_to_wifi();
  display_clear();
  
  //setup time 
  display_print(20, 20, "Setting time......");
  setup_time(); 
  display_clear();
  

}// end setup


//main loop
void loop() {

  display_clear();


  Bus busArr[3];
  int count = get_bus_times(busArr);

  //height for each entry 
  int blockHeight = 75; 

  for (int i = 0; i < count; i++) {

    int blockY = 10 + (blockHeight * i); // top of this bus block

    // line 1 route + mins "F2   5 mins"
    display_print(10, blockY,  busArr[i].route.c_str());
    display_print(180, blockY, busArr[i].numMins.c_str());
    
    // line 2 destination "Charlestown"
    display_print(10, blockY + 25, busArr[i].destination.c_str());

    // divider line below this block    
    display_line(10, blockY + 50, 630, blockY + 50, COLOR_ORANGE);

  }// end for 

  // time in bottom right
  String now = get_time_hhmm();
  display_print(230, 220, now.c_str());

  //wait 30 seconds 
  delay(30000);

}// end loop 