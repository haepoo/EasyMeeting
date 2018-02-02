#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "@WIFI.ID";     // your network SSID (name)
char password[] = "semogabermanfaat"; // your network key

// Initialize Telegram BOT
#define BOTtoken "543588614:AAEkpXTfP5IPczdCYhxm4zDTrmAiyxG1sI0"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int ledPin = 2;
int ledStatus = 0;
int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

//    if (text == "/daftar") { 
//      //masukin ke database, masi blm tau caranya 
//    } 

    if (text == "/nyalakan") {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Lampu dinyalakan", "");
    }

    if (text == "/matikan") {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Lampu dimatikan", "");
    }

    if (text == "/status") {
      if(ledStatus){
        bot.sendMessage(chat_id, "Lampu ruangan menyala", "");
      } else {
        bot.sendMessage(chat_id, "Lampu ruangan mati", "");
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

void setup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
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
  
  delay(10);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while(numNewMessages) {
      Serial.println(digitalRead(ledPin));
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    
    Bot_lasttime = millis();
  }
}
