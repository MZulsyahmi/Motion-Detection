/*---------------------KRENOVATOR---------------------
  PIR Sensor

  Demonstrates on how the PIR sensor works. It is known
  as motion sensor which will detect any movement within
  the area.

  Get the code at github

  by M.Zulsyahmi @krenovator
  October 2018
 */

//fixed the time delay for 10 secs
#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int pirSensor = 13;      //D7

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, starts a timer
void detectsMovement() {
  Serial.println("MOTION DETECTED!");
  startTimer = true;
  lastTrigger = millis();
}
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(pirSensor, INPUT_PULLUP);
  // Set pirSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(pirSensor), detectsMovement, RISING);
}

void loop() {
  // Current time
  now = millis();
  // After the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped");
    startTimer = false;
  }
}
