import json 
import requests
import time
import urllib
from dbhelper import DBHelper

db = DBHelper()

# define the token of bot and url
TOKEN = "543588614:AAEkpXTfP5IPczdCYhxm4zDTrmAiyxG1sI0"
URL = "https://api.telegram.org/bot{}/".format(TOKEN)

# ambil konten url dan mengembalikan string
def get_url(url):
	response = requests.get(url)
	content = response.content.decode("utf8")
	return content

# ngambil json dari urlnya
def get_json_from_url(url):
	content = get_url(url)
	js = json.loads(content)
	return js

# terus mengupdate telegram setiap ada yang mengirim pesan
def get_updates(offset=None):
	url = URL + "getUpdates?timeout=100"
	if offset:
		url += "&offset={}".format(offset)
	js = get_json_from_url(url)
	return js

# mengambil chat id dan teks terakhir yang dikirim ke bot
def get_last_chat_id_and_text(updates):
	num_updates = len(updates["result"])
	last_update = num_updates - 1
	text = updates["result"][last_update]["message"]["text"]
	chat_id = updates["result"][last_update]["message"]["chat"]["id"]
	return (text, chat_id)

# membalas pesan
def send_message(text, chat_id, reply_markup=None):
	text = urllib.parse.quote_plus(text)
	url = URL + "sendMessage?text={}&chat_id={}&parse_mode=Markdown".format(text, chat_id)
	if reply_markup:
		url+="&reply_markup={}".format(reply_markup)
	get_url(url)

# mencari id update tertinggi
def get_last_update_id(updates):
	update_ids = []
	for update in updates["result"]:
		update_ids.append(int(update["update_id"]))
	return max(update_ids)

def build_keyboard(items):
	keyboard = [[item] for item in items]
	reply_markup = {"keyboard":keyboard, "one_time_keyboard": True}
	return json.dumps(reply_markup)
	
# ngehandel perintah	
def handle_updates(updates):
	for update in updates["result"]:
		text = update["message"]["text"]
		chat = update["message"]["chat"]["id"]
		items = db.get_items(chat)
		kontrol = db.baca_kontrol()
		# ruang = db.pilih_ruang(chat)
		if text == "/kontrol":
			keyboard = build_keyboard(kontrol)
			send_message("Ingin mengontrol apa?", chat, keyboard)
		# elif text == "Menyalakan Lampu, AC, dan LCD":
		# 	#ngecek id ada di database apagak
		# elif text == "/matikan":
		# 	#ngecek id ada di database apagak
		# # elif text == "/pilihruang":

		elif text == "/start":
			send_message("Selamat datang di EasyMeeting Bot!\n\nKetik /masuk untuk autorisasi akun\nKetik /daftar untuk mendaftar akun\nKetik /pilihruang untuk memilih ruang rapat\nKetik /kontrol untuk menyalakan dan mematikan ruang rapat", chat)
		elif text.startswith("/"):
			continue
		# elif text in kontrol:
		# 	db.delete_item(text, chat)
		# 	items = db.get_items(chat)
		# 	message = "\n".join(items)
		# 	send_message(message, chat)
		# else:
		# 	db.add_item(text, chat)
		# 	items = db.get_items(chat)
		# 	message = "\n".join(items)
		# 	send_message(message, chat)



def main():
	print ("server easymeeting bot dijalankan...")
	db.setup()
	last_update_id = None
	# ngecek apakah pesan yang terbaru adalah last_textchat
	while True:
		print("getting updates")
		updates = get_updates(last_update_id)
		if len(updates["result"]) > 0:
			last_update_id = get_last_update_id(updates) + 1
			handle_updates(updates)

# agar codingan terus berjalan, tanpa perlu dirun ulang
if __name__ == '__main__': 
	main()
	