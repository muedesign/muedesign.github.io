# muedesign.github.io
Documentation for "mudesign" Mini Embedded Development Boards. 


__µEC3 - ESP32-C3 Mini Development Board:__

<p align="justify">
µEC3 is a high-performance, tiny microcontroller development board based on ESP32-C3 WROOM 02 module. The board runs on 32-bit RISC V single core processor with a maximum frequency of 160 MHz. The module has 4 MB flash memory and the board is specifically targeted for low power IOT sensing applications. The board size is compact (2.7 x 2.4 cm) making it suitable for space constrained IOT applications and wearables.
The module is equipped with PCB antenna with support for 2.4 GHz WiFi, Bluetooth LE communication. Board programming is done via on-chip/native USB without using an external USB-UART bridge chip. 
</p>

<img width="975" height="360" alt="image" src="https://github.com/user-attachments/assets/7aceb1cd-45ef-4d02-b896-e142655c717a" />


**µES3 - ESP32-S3 Mini Development Board:**

<p align="justify">
µES3 is a high-performance, tiny microcontroller development board based on ESP32-S3 WROOM 1 N16R8 module. The board runs on LX7 Xtensa dual core 32-bit microprocessor with a maximum frequency of 240 MHz. The module has 16 MB flash memory and 8 MB PSRAM which is specifically targeted for memory intensive applications like AI, speech/audio and image processing. 
The module is equipped with a PCB antenna trace which facilitates 2.4 GHz WiFi and Bluetooth (BLE) communication. Board programming is done via on-chip/native USB without using an external USB-UART bridge chip. The board size is compact (2.7 x 2.4 cm) making it suitable for space constrained IOT, camera vision and wearable applications.
</p>

<img width="1189" height="318" alt="image" src="https://github.com/user-attachments/assets/351e7158-735d-437d-8560-5d246d0ad6c9" />

**General Instructions for working with Arduino IDE**


Arduino IDE programming
1. Download the latest version of Arduino IDE.
2. Include the ESP board package link as given below in the “Additional board manager URL under File-Preferences”:
“https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json”
2. As next step, install the esp32 library from the “Tools-Board-Board Manager” by typing esp32 in the search box and choosing the latest library version.
3. Once ESP32 library is installed, connect the development board and choose the “ESP32 C3 Dev Module or ESP32 S3 Dev Module” under “Tools – Board – esp32”. 
4. Upload the code and start working with the board.


**Note: For serial communication, under "Tools-USB CDC On Boot:", keep it "Enabled" **


