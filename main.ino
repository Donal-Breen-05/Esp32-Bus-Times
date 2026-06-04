//headers
#include "config.h"
#include "wifi_manager.h"
#include "tfi_api.h"
#include "set_time.h"

//setup
void setup() {

  Serial.begin(115200); //set baud rate
  delay(1000);

  //title
  Serial.println("\n==============================");
  Serial.println("   ESP-32 Bus Times");
  Serial.println("==============================\n");

  //wifi & time 
  connect_to_wifi();
  setup_time(); 

}// end setup


//main loop
void loop() {

  get_bus_times();

  delay(30000);

}//end loop//end loop 
