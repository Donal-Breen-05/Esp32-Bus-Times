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

  //title
  Serial.println("\n==============================");
  Serial.println("   ESP-32 Bus Times");
  Serial.println("==============================\n");

  //display test 
  display_init();
  Serial.println("displayinit"); 

  display_fill(COLOR_RED);
  delay(1000);

  display_fill(COLOR_GREEN);
  delay(1000);

  display_fill(COLOR_BLUE);
  delay(1000);

  display_fill(COLOR_BLACK);

  display_set_text_size(3);
  display_set_text_color(COLOR_WHITE);

  display_print(20, 20, "HELLO");

  
  //wifi & time 
  connect_to_wifi();
  setup_time(); 

}// end setup


//main loop
void loop() {

  get_bus_times();

  delay(30000);

}//end loop//end loop 
