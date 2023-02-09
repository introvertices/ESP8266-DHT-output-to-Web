# ESP8266-DHT-output-to-Web
An Arduino ESP8266 script to read sensor data and send it via PHP to your website over WiFi.


Made because I found it was an absolute pain to find useful tutorials and materials to get this working, particularly for the 8266 rather than the 32.

Sketch uploaded to a Keyestudio NODEMCU board, but should work with any ESP8266.


## Instructions:

1. Change SSID and PW in sketch
2. Upload subtemp.php to your host (make sure you have PHP access!)
3. Modify line in index.html to point the embed to the text file that is created from the readings, and upload to your webspace. The text file will be made in the same folder that subtemp.php lives in.
4. Change Line 86 (String dataline) to point to the above file
5. Upload sketch
6. Use the serial monitor to check everything goes. You might also want to reduce the duration of readings on line 151 (written in milliseconds).


Feel free to fork and modify! I'm still a bit of a C++ newb so I did my best <3


## Notes on sensors:

- Always check whether your sensor has a resistor in it - Keyestudio's DHT does, so I didn't need to include one. Depending on your board you may need to hook up the DHT to the VIN to give it 5v as 3.3v may not be enough power and give wildly inaccurate readings.

- DHT11 sensors do have a pretty wide margin of error across brands, so keep that in mind if you are looking for very accurate readings.
