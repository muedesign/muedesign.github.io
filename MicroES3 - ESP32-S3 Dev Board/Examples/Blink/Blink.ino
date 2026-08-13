// Repeatedly blink the onboard SMD LED which is connected to GPIO pin 5

#define LED_PIN 5 // GPIO pin 5 is connected to onboard SMD LED

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Turn LED ON
  delay(1000);                 // Wait 1 second
  digitalWrite(LED_PIN, LOW);  // Turn LED OFF
  delay(1000);                 // Wait 1 second
}