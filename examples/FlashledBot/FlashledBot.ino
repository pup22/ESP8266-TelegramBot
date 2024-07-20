/*******************************************************************
 *  this is a basic example how to program a Telegram Bot          *
 *  using TelegramBOT library on ESP8266                           *
 *                                                                 *
 *  Open a conversation with the bot, you can command via Telegram *
 *  a led from ESP8266 GPIO                                        *
 *  https://web.telegram.org/#/im?p=@FlashledBot_bot               *
 *                                                                 *
 *  written by Giancarlo Bacchio                                   *
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ESP8266TelegramBOT.h"
#include "../secret.h"

#define LED 2

// Initialize Wifi connection to the router
char ssid[] = SSID;              // your network SSID (name)
char pass[] = PASS;                              // your network key



// Initialize Telegram BOT (in secret.h file)
// #define BOTtoken "134745667:AAETzUWRQdb9xbMX_s-q_50U6ffgXcW3ldg"  //token of FlashledBOT
// #define BOTname "FlashledBot"
// #define BOTusername "FlashledBot_bot"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 2500; //mean time between scan messages
unsigned long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;



/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/
void Bot_ExecMessages() {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    if (bot.message[i][5] == "/ledon") {
      digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
      bot.sendMessage(bot.message[i][4], "Led is ON", "");
    }
    if (bot.message[i][5] == "/ledoff") {
      digitalWrite(LED, HIGH);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(bot.message[i][4], "Led is OFF", "");
    }
    if (bot.message[i][5] == "/start") {
      String wellcome = "Wellcome from FlashLedBot, your personal Bot on ESP8266 board";
      String wellcome1 = "/ledon : to switch the Led ON";
      String wellcome2 = "/ledoff : to switch the Led OFF";
      bot.sendMessage(bot.message[i][4], wellcome, "");
      bot.sendMessage(bot.message[i][4], wellcome1, "");
      bot.sendMessage(bot.message[i][4], wellcome2, "");
      Start = true;
    }
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT); // initialize digital pin as an output.

  // attempt to connect to Wifi network:
  {
    Serial.print("\nConnecting Wifi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.hostname("ESP_Test"); // имя для ESP
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(LED, !digitalRead(LED));
      Serial.print(".");
      delay(100);
    }
    digitalWrite(LED, LOW);

    Serial.println("\nWiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
  }
  bot.begin();      // launch Bot functionalities
}



void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    Bot_ExecMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  }
}



