// COMP-10184 – Mohawk College
// 
// @author     Luca Quacquarelli
// @id         000838997
//
// I, Luca Quacquarelli, 000838997 certify that this material is my original work.  
// No other person's work has been used without due acknowledgement.
//
#include <ESP8266WiFi.h>
#include <Arduino.h> 
#include "secrets.h"
#include "ThingSpeak.h"

char ssid[] = SSID;   // your network SSID (name) 
char pass[] = PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int number = 0;
WiFiClient  client;

// controls time in milliseconds 
unsigned long iControlTimer = 0;
// store movement detections 
int iMyMovement = 0;


unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
// pin assignments for external LEDs 
#define PIN_LED_GREEN   D1 
#define PIN_LED_YELLOW  D2 
#define PIN_LED_RED     D3 
 
// pin assignment for PIR input 
#define PIN_PIR         D5  
 
 #define DELAY   2
 // ***********************************
void IRAM_ATTR isr() {
  digitalWrite(PIN_LED_GREEN, LOW); 
  digitalWrite(PIN_LED_YELLOW, HIGH); 
}


void setup() { 

  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak



  //attachInterrupt(digitalPinToInterrupt(PIN_LED_GREEN), , CHANGE);
  // setup LED outputs 
  pinMode(PIN_LED_GREEN, OUTPUT); 
  pinMode(PIN_LED_YELLOW, OUTPUT); 
  pinMode(PIN_LED_RED, OUTPUT); 
 
  // setup PIR input 
  pinMode(PIN_PIR, INPUT); 
 
  // built-in LED 
  pinMode(LED_BUILTIN, OUTPUT); 

  // startup messages 
  Serial.println();
  Serial.println("COMP-10184 - Motion Events");
  Serial.println("Name: Luca Quacquarelli");
  Serial.println("Student ID: 000838997");
  Serial.println();
  // YELLOW ON 
  
  digitalWrite(PIN_LED_GREEN, LOW); 
  digitalWrite(PIN_LED_YELLOW, HIGH); 
  digitalWrite(PIN_LED_RED, LOW); 
  
} 
 
void loop() { 

  digitalWrite(LED_BUILTIN, HIGH); 
   // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    //Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    digitalWrite(PIN_LED_GREEN, HIGH); 
    digitalWrite(PIN_LED_YELLOW, LOW);
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); 
    Serial.println("\nConnected.");
    delay(5000); 
  }



  iMyMovement = digitalRead(PIN_PIR);
  
  if(iMyMovement > 0) { 
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 1, number, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    // change the value
    number++;
    if(number > 99){
      number = 0;
    }  
    Serial.println("Motion activated!");
    iControlTimer = iControlTimer + DELAY;
    //while (iControlTimer != 0) {
        digitalWrite(PIN_LED_GREEN, LOW); 
        digitalWrite(PIN_LED_RED, HIGH);  
        delay(2000);
        digitalWrite(PIN_LED_GREEN, HIGH); 
        digitalWrite(PIN_LED_RED, LOW);  
        //iControlTimer--;

    //break;
        
    //}
  }
  else {
    digitalWrite(PIN_LED_RED, LOW);  
  }


  delay(20000); // Wait 20 seconds to update the channel again
}

