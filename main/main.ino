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
  delay(1000)
  display_clear();

  //wifi & time 
  display_print(20, 20, "connecting......");
  delay(1000)
  connect_to_wifi();
  display_clear();
  
  display_print(20, 20, "Setting time......");
  delay(1000)
  setup_time(); 
  display_clear();
  

}// end setup


//main loop
void loop() {

  //clear 
  display_clear(); 

  // struct array 
  Bus busArr[4];
  int count = get_bus_times(busArr);

  for (int i = 0; i < count; i++) {

    // print to screen the bus route , time , destination 
    String line = busArr[i].route + " -> " + busArr[i].numMins + " (" + busArr[i].destination + ")";

    display_print(10 , 20 * i , line.c_str()); 

  }// end for 

  //wait 30 seconds
  delay(30000);

}//end loop
