#ifndef TFI_API_H
#define TFI_API_H


// bus times struct 
struct Bus { 

  String route; //15A etc 
  String numMins; // "5 mins" 
  String destination; // "(Charlestown)"

};//end Bus 

int get_bus_times(Bus busArr[4]);


#endif
