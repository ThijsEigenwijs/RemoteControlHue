#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "credentials.h"

const char* host = "192.168.0.136";

#ifndef credentials
const char *ssid = "vanikke";
const char *pass = "thijstan2310";
#endif

const int amountButtons = 4;
const int buttons[] = {D2, D3, D4, D5};
const char* action[] = {"allOn", "allOff", "brightness 255 \nwhite 600 9 \nwhite 600 1 ", "brightness 255 \nwhite 100 9 \nwhite 100 1 "};

bool buttonState[amountButtons];

WiFiClient CH;
const int sshPort = 22;

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)


void setup() {

  Serial.begin(115200);
  Serial.println("Hello!");
  for (int i = 0; i < amountButtons; i++) {
    pinMode(buttons[i], INPUT);
  }
  Serial.println("Cheching buttons!");
  checkButton();
  Serial.println("Something with Wifi!");
  wifiConnect();
  Serial.println("Sending Everything!");
  sendCommand();

}

void loop() {
  delay(100);
  checkButton();
  sendCommand();
}

bool checkButton() {
  bool state = true;
  for (int i = 0; i < amountButtons; i++) {
    buttonState[i] = digitalRead(buttons[i]);
    if (buttonState[i] == false)
      state = false;
  }
  return state;
}

void wifiConnect() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    //This is indeed somekind of bootloop, to prevent it from rebooting we delay it
    delay(150);
    Serial.print(".");
  }

  Serial.print("\nWifi - Status: ");
  // Indicate if there is a connection
  Serial.println("Connected");

  if (!CH.connect(host, sshPort)) {
    Serial.println("Connection Failed...");
  }
  CH.println("name Remote1 ");

}

void sendCommand() {


  unsigned long currentMillis = millis();



  for (int i = 0; i < amountButtons; i++) {
    if (buttonState[i] == false) {
      if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        Serial.println(action[i]);
        CH.print(action[i]);
        CH.print(" \n");
      }
    }
  }
  //CH.stop();
}

