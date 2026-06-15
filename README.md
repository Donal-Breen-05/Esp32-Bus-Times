# Esp32-Bus-Times
An overengineered solution to bus times written in C++ for the ESP32 using a "CYD" - Cheap Yellow Display
- Uses the TFI Live API to get times for buses for a specified stop
- Hosts a web server to allow the user to change what bus stop they want to track
- Displays the current time in 24-hour format

---
# Images
## Device


<table>
  <tr>
    <td align="center" style="border: 2px solid #ccc; padding: 10px;">
      <img src="ReadMeImages/stop1112.jpg" width="300" style="transform: rotate(+90deg);" /><br/>
      <em>Stop 1112</em>
    </td>
    <td align="center" style="border: 2px solid #ccc; padding: 10px;">
      <img src="ReadMeImages/stop2324.jpg" width="300" style="transform: rotate(-180deg);" /><br/>
      <em>Stop 2324</em>
    </td>
  </tr>
</table>

## Web Server

<table>
  <tr>
    <td align="center" style="border: 2px solid #ccc; padding: 10px;">
      <img src="ReadMeImages/server1.jpg" width="300" /><br/>
      <em>Change Stop</em>
    </td>
    <td align="center" style="border: 2px solid #ccc; padding: 10px;">
      <img src="ReadMeImages/server2.jpg" width="300" /><br/>
      <em>Success</em>
    </td>
  </tr>
</table>

---
# Usage / Installation
- Clone the repository
- Install the required libraries: `ArduinoJSON.h` and `TFT_eSPI.h`
- Flash the files using **Arduino IDE**
- Use the web server to set the stop number to your bus stop

*Note: As I used an ESP32 CYD, my code may not work on all other ESP32 CYD modules as they have different pin layouts and drivers, so there may be issues with the display code for some users. You may need to edit your `User_setup.h` file to allow your display to work with the code.*
