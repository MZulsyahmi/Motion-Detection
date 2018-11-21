/*---------------------KRENOVATOR---------------------
  PIR Sensor + LED + OLED Display + MQTT

  Demonstrates on how the PIR sensor works with the LED
  as a triggered component. The sensor is known also as
  motion sensor which will detect any movement within
  the area. Thus, the LED will lights up when the sensor
  detected a movement. Then it will display in OLED LCD
  whether there is a movement or not. The movement can
  be monitored via internet through Adafruit MQTT.

  Get the code at github

  by M.Zulsyahmi @krenovator
  October 2018
 */
 
//fix the time delay for 10 secs
#define timeSeconds 10

/*the library for OLED display*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Set GPIOs for LED and PIR Motion Sensor
const int led = 12;           //D6
const int motionSensor = 13;  //D7
int i;                        //the initialize for the function to be read in Adafruit

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
  i  = 1;                                 //call 1 if there is movement
  digitalWrite(led, HIGH);                //LED HIGH(ON)
  startTimer = true;
  lastTrigger = millis();
  display.display();                      //push to display
}

//initialize for the Adafruit codes
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#if (SSD1306_LCDHEIGHT != 64)   //to declare the pixel of OLED
#error("Height incorrect, please fix Adafruit_SSD1306.h!");   //if got an error, the codes need to be changed
#endif

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Your SSID"  //Remote Year / MyEdTech
#define WLAN_PASS       "SSID Password" //RemoteYearKL / myedtechpnt

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Your Username"
#define AIO_KEY         "AIO Key"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Motion = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/your feeds name");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.print("Device Started");

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
  
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&onoffbutton);
}
uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
 
/************************ The controlled coding ********************************/

  // Current time
  now = millis();
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print("Motion stopped");
    i = 0;                                 //call 0 if there is no movement
    digitalWrite(led, LOW);                //LED LOW(OFF)
    startTimer = false;
    display.display();
  }
  display.clearDisplay();

/************************ Condition to publish ********************************/
  
  if (!Motion.publish(i)) {
    Serial.println(F("Failed"));
  }
  else {
    Serial.println(F("OK!"));
  }

  delay(2000);
  display.clearDisplay();

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
