import sqlite3

class DBHelper:
	def __init__(self, dbname="easymeeting.db"):
		self.dbname = dbname
		self.conn = sqlite3.connect(dbname)

	def setup(self):
		print("Database dibuat...")
		tblprtstmt = "CREATE TABLE IF NOT EXISTS kontrol (id INTEGER NOT NULL PRIMARY KEY, perintah TEXT)"
		stmt = "INSERT OR REPLACE INTO kontrol VALUES (1, 'Menyalakan Lampu, AC, dan LCD'), (2, 'Mematikan Lampu, AC, dan LCD')"
		tblpgnstmt = "CREATE TABLE IF NOT EXISTS pengguna (nama_pengguna text, kata_sandi text)"
		self.conn.execute(tblprtstmt)
		self.conn.execute(stmt)
		self.conn.execute(tblpgnstmt)
		self.conn.commit()

	def add_item(self, item_text, owner):
		# stmt = "INSERT INTO items (description, owner) VALUES (?, ?)"
		# args = (item_text, owner)
		# self.conn.execute(stmt, args)
		self.conn.commit()

	def delete_item(self, item_text, owner):
		# stmt = "DELETE FROM items WHERE description = (?) AND owner = (?)"
		# args = (item_text, owner)
		# self.conn.execute(stmt, args)
		self.conn.commit()

	def get_items(self, owner):
		# stmt = "SELECT description FROM items WHERE owner = (?)"
		# args = (owner, )
		return 0

	def baca_kontrol(self):
		stmt = "SELECT perintah FROM kontrol"
		args = ()
		return [x[0] for x in self.conn.execute(stmt)]

	def get_items(self, owner):
		stmt = "SELECT description FROM items WHERE owner = (?)"
		args = (owner, )
		return 0