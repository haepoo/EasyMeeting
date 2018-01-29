#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>
#define LED 1 //led pin number

// Initialize Wifi connection to the router
const char* ssid     = "xxxx";
const char* password = "yyyy";

// Initialize Telegram BOT
const char BotToken[] = "543588614:AAEkpXTfP5IPczdCYhxm4zDTrmAiyxG1sI0";

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  bot.begin();
  pinMode(LED, OUTPUT);
}

void loop() {
    message m = bot.getUpdates(); // Read new messages
    if (m.text.equals("on")) {
      digitalWrite(LED,1);
      bot.sendMessage(m.chat_id, "Led is ON");
    }
    else if (m.text.equals("off")) {
      digitalWrite(LED, 0);
      bot.sendMessage(m.chat_id, "Led is OFF");
    }
//    if ( m.chat_id != 0 ){ // Checks if there are some updates
//      Serial.println(m.text);
//      bot.sendMessage(m.chat_id, m.text);  // Reply to the same chat with the same text
//    } else {
//      Serial.println("no new message");
//    }
}
