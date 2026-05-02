import os
import json
import base64
import sqlite3
import win32crypt
from Crypto.Cipher import AES
import shutil
from datetime import timezone, datetime, timedelta

def get_browser_datetime(chromedate):
    """Перетворює формат дати Chromium у зручний формат datetime."""
    return datetime(1601, 1, 1) + timedelta(microseconds=chromedate)

def get_encryption_key():
    """Витягує та декодує AES ключ для Opera GX."""
    # Змінено шлях на директорію Opera GX
    local_state_path = os.path.join(os.environ["USERPROFILE"], 
                                    "AppData", "Roaming", "Opera Software", "Opera GX Stable", 
                                    "Local State")
    
    with open(local_state_path, "r", encoding="utf-8") as f:
        local_state = f.read()
        
    local_state = json.loads(local_state)
    key = base64.b64decode(local_state["os_crypt"]["encrypted_key"])
    key = key[5:]
    return win32crypt.CryptUnprotectData(key, None, None, None, 0)[1]

def decrypt_password(password, key):
    """Розшифровує пароль за допомогою AES ключа."""
    try:
        iv = password[3:15]
        password = password[15:]
        cipher = AES.new(key, AES.MODE_GCM, iv)
        return cipher.decrypt(password)[:-16].decode()
    except:
        try:
            return str(win32crypt.CryptUnprotectData(password, None, None, None, 0)[1])
        except:
            return ""

def main():
    try:
        key = get_encryption_key()
    except FileNotFoundError:
        print("❌ Помилка: Не вдалося знайти файл Local State. Можливо, Opera GX не встановлена або шлях відрізняється.")
        return

    # Змінено шлях до бази даних паролів Opera GX
    db_path = os.path.join(os.environ["USERPROFILE"], "AppData", "Roaming",
                           "Opera Software", "Opera GX Stable", "Login Data")
                           
    filename = "OperaData.db"
    
    try:
        shutil.copyfile(db_path, filename)
    except FileNotFoundError:
        print("❌ Помилка: Не вдалося знайти файл Login Data. У цьому браузері ще немає збережених паролів.")
        return
    
    db = sqlite3.connect(filename)
    cursor = db.cursor()
    cursor.execute("select origin_url, action_url, username_value, password_value, date_created, date_last_used from logins order by date_created")
    
    output_data = ""
    passwords_count = 0
    
    for row in cursor.fetchall():
        origin_url = row[0]
        action_url = row[1]
        username = row[2]
        password = decrypt_password(row[3], key)
        date_created = row[4]
        date_last_used = row[5]
        
        if username or password:
            output_data += f"Origin URL: {origin_url}\n"
            output_data += f"Action URL: {action_url}\n"
            output_data += f"Username: {username}\n"
            output_data += f"Password: {password}\n"
            passwords_count += 1
        else:
            continue
            
        if date_created != 86400000000 and date_created:
            output_data += f"Creation date: {str(get_browser_datetime(date_created))}\n"
            
        if date_last_used != 86400000000 and date_last_used:
            output_data += f"Last Used: {str(get_browser_datetime(date_last_used))}\n"
            
        output_data += "="*50 + "\n"
        
    cursor.close()
    db.close()
    
    try:
        os.remove(filename)
    except:
        pass

    output_filename = "operagx_passwords.txt"
    with open(output_filename, "w", encoding="utf-8") as file:
        file.write(output_data)

    if passwords_count > 0:
        print(f"\n✅ Успішно знайдено {passwords_count} паролів з Opera GX.")
        print(f"📁 Всі дані збережено у файл: {output_filename}")
    else:
        print("\n⚠️ Увага: Базу даних знайдено, але в ній немає збережених паролів.")

if __name__ == "__main__":
    main()