// Connect the analog signal on the GPIO pin. 
// Read the analog input on the GPIO pin and print as 12-bit value. 

const int analogPin = 0; // ADC-capable GPIO pin on the board. Connect the analog signal to GPIO 0. Edit this pin if needed by looking at the pinout details given for the board in the GITHUB page. 
int sensorValue = 0;

void setup() {
  Serial.begin(9600); // Start serial communication
  delay(1000);
}

void loop() {
  sensorValue = analogRead(analogPin); // Read the analog input (0 - 4095) and present it as as 12 bit value
  Serial.println(sensorValue);         // Print value to Serial Monitor
  delay(500);                        // Delay between readings
}