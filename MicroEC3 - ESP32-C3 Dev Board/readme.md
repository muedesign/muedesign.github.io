__µEC3 - ESP32-C3 Mini Development Board:__

**Note: For serial communication in Arduino IDE, under "Tools-USB CDC On Boot:" keep it "Enabled"**

<p align="justify">
µEC3 is a high-performance, tiny microcontroller development board based on ESP32-C3 WROOM 02 module. The board runs on 32-bit RISC V single core processor with a maximum frequency of 160 MHz. The module has 4 MB flash memory and the board is specifically targeted for low power IOT sensing applications. The board size is compact (2.7 x 2.4 cm) making it suitable for space constrained IOT applications and wearables.
The module is equipped with PCB antenna with support for 2.4 GHz WiFi, Bluetooth LE communication. Board programming is done via on-chip/native USB without using an external USB-UART bridge chip. 
</p>

<img width="975" height="360" alt="image" src="https://github.com/user-attachments/assets/7aceb1cd-45ef-4d02-b896-e142655c717a" />


**General Instructions for working with Arduino IDE**


Arduino IDE programming:
1. Download the latest version of Arduino IDE.
2. Include the ESP board package link as given below in the “Additional board manager URL under File-Preferences”:
“https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json”
2. As next step, install the esp32 library from the “Tools-Board-Board Manager” by typing esp32 in the search box and choosing the latest library version.
3. Once ESP32 library is installed, connect the development board and choose the “ESP32 C3 Dev Module or ESP32 S3 Dev Module” under “Tools – Board – esp32”. 
4. Upload the code and start working with the board.





