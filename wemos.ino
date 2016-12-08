#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();
void outletOneOn();
void outletOneOff();
void outletTwoOn();
void outletTwoOff();

// Change this before you flash
const char* ssid = "Skynet";
const char* password = "8039795700";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *outletOne = NULL;
Switch *outletTwo = NULL;

// Set Relay Pins
int relayOne = 12;
int relayTwo = 13;
int relayThree = 14;
int relayFour = 16;

void setup()
{
  Serial.begin(115200);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    outletOne = new Switch("Outlet One", 82, outletOneOn, outletOneOff);
    outletTwo = new Switch("Outlet Two", 83, outletTwoOn, outletTwoOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*outletOne);
    upnpBroadcastResponder.addDevice(*outletTwo);

    //Set relay pins to outputs
       pinMode(12,OUTPUT); 
       pinMode(13,OUTPUT);
       pinMode(14,OUTPUT);
       pinMode(16,OUTPUT);
       
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      lightOne->serverLoop();
      lightTwo->serverLoop();
      outletOne->serverLoop();
      outletTwo->serverLoop();
	 }
}

void lightOneOn() {
    Serial.print("Switch 1 turn on ...");
    digitalWrite(relayOne, HIGH);   // sets relayOne on
}

void lightOneOff() {
    Serial.print("Switch 1 turn off ...");
    digitalWrite(relayOne, LOW);   // sets relayOne off
}

void lightTwoOn() {
    Serial.print("Switch 2 turn on ...");
    digitalWrite(relayThree, HIGH);   // sets relayOne on
}

void lightTwoOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayThree, LOW);   // sets relayOne on
}

//sockets

void outletOneOn() {
    Serial.print("Socket 1 turn on ...");
    digitalWrite(relayFour, HIGH);   // sets relayOne on
}

void outletOneOff() {
    Serial.print("Socket 1 turn off ...");
    digitalWrite(relayFour, LOW);   // sets relayOne off
}

void outletTwoOn() {
    Serial.print("Socket 2 turn on ...");
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
}

void outletTwoOff() {
  Serial.print("Socket 2 turn off ...");
  digitalWrite(relayTwo, LOW);   // sets relayOne on
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
