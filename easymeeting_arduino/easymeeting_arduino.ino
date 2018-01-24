#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

//inisialisasi koneksi wifi arduino ke router
char ssid     = "xxxx";
char password = "yyyy";

//inisialisasi token bot telegram
#define BOTtoken "543588614:AAEkpXTfP5IPczdCYhxm4zDTrmAiyxG1sI0"

WiFiClientSecure client;
UniversalTelegramBot bot (BOTtoken, client);

int Bot_mtbs = 1000;
long Bot_lasttime;

const int pinLampu = 13;
int statusLampu = 0;

DynamicJsonBuffer jsonBuffer;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessage");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++){
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    if (from_name == "") from_name = "Guest";

    if (text == "/daftar") {
      //masukin ke database, masi blm tau caranya
    }

    if (text == "/nyalakan") {
      statusLampu = 1; 
      digitalWrite(pinLampu, HIGH);
      bot.sendMessage(chat_id, "Lampu dinyalakan", "");
    }
    
    if (text == "/matikan") {
      statusLampu = 0;
      digitalWrite(pinLampu, LOW); 
      bot.sendMessage(chat_id, "Lampu dimatikan", "");
    }

    if (text == "/status") {
      if (statusLampu) {
        bot.sendMessage(chat_id, "Lampu menyala", "");
      } else {
        bot.sendMessage(chat_id, "Lampu mati, "");
      }
    }

    if (text == "/kontrol") {
      String keyboardJson = "[[\"/nyalakan\", \"/matikan\"],[\"/status\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Pilih menu kontrol dibawah ini", "", keyboardJson, true);
    }
    
    if (text == "/start") {
      String welcome = "Selamat datang di EasyMeeting Bot, " + from_name + "!\n\n";
      welcome += "Ketik /daftar untuk mendaftar akun, dengan format /daftar <ID Karyawan>\n";
      welcome += "e.g. /daftar 12345\n\n";
      welcome += "Ketik /pilihruang untuk memilih ruang rapat\n";
      welcome += "Ketik /kontrol untuk membuka menu kontrol ruang rapat";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup(){
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // attempt to connect to Wifi network:  
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  bot.begin();
  pinMode(ledPin, OUTPUT);
  delay(10);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}



