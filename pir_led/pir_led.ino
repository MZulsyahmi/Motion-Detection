*---------------------KRENOVATOR---------------------
  PIR Sensor + LED

  Demonstrates on how the PIR sensor works with the LED
  as a triggered component. The sensor is known also as
  motion sensor which will detect any movement within
  the area. Thus, the LED will lights up when the sensor
  detected a movement.

  Get the code at github

  by M.Zulsyahmi @krenovator
  October 2018
 */
 
//fixed the time delay for 10 secs
#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int led = 12;               //D6
const int motionSensor = 13;      //D7

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void detectsMovement() {
  Serial.println("MOTION DETECTED!");
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped");
    digitalWrite(led, LOW);
    startTimer = false;
  }
}
