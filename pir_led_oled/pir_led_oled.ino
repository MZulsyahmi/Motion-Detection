/*---------------------KRENOVATOR---------------------
  PIR Sensor + LED + OLED Display

  Demonstrates on how the PIR sensor works with the LED
  as a triggered component. The sensor is known also as
  motion sensor which will detect any movement within
  the area. Thus, the LED will lights up when the sensor
  detected a movement. Then it will display in OLED LCD
  whether there is a movement or not.

  Get the code at github
  http://bit.ly/PIR_MotionDetection

  by M.Zulsyahmi @krenovator
  October 2018
 */
 
//fixed the time delay for 10 secs
#define timeSeconds 10

/*the library for OLED display*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Set GPIOs for LED and PIR Motion Sensor
const int led = 12;               //D6
const int motionSensor = 13;      //D7

#define OLED_RESET LED_BUILTIN        //Enable the OLED
Adafruit_SSD1306 display(OLED_RESET); //Use the library

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void detectsMovement() {
  display.setTextColor(WHITE);            //set the color of the text
  display.setCursor(0,0);                 //place the text (column,row)
  display.setTextSize(2);                 //text size
  display.print("MOTION \n DETECTED!");
  digitalWrite(led, HIGH);                //LED HIGH(ON)
  startTimer = true;
  lastTrigger = millis();
  display.display();                      //push to display
}
void setup() {
  Serial.print("Device Started");

  //clear the display
  display.clearDisplay();

  // Serial port for debugging purposes
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  //clear buffer
  display.clearDisplay();
}

void loop() {
  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("Motion \n stopped");
    digitalWrite(led, LOW);
    startTimer = false;
    display.display();
  }
  display.clearDisplay();
}
