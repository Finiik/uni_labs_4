import json
import os
from base64 import b64encode, b64decode
from Crypto.Cipher import DES, AES
from Crypto.Util.Padding import pad, unpad
from Crypto.Random import get_random_bytes

def encrypt_file(algo_name, input_file, output_file, key_file):
    try:
        # Зчитуємо дані з текстового файлу у вигляді байтів
        with open(input_file, 'rb') as f:
            data = f.read()

        # Налаштування алгоритму та генерація ключа
        if algo_name == 'DES':
            key = get_random_bytes(8) # 8 байтів для DES
            cipher = DES.new(key, DES.MODE_CBC)
            block_size = DES.block_size
        else: # AES
            key = get_random_bytes(16) # 16 байтів для AES
            cipher = AES.new(key, AES.MODE_CBC)
            block_size = AES.block_size

        # Зберігаємо згенерований ключ у файл для подальшого розшифрування
        with open(key_file, 'wb') as f:
            f.write(key)

        # Шифруємо дані, попередньо вирівнявши їх блоками (pad)
        ct_bytes = cipher.encrypt(pad(data, block_size))
        
        # Кодуємо вектор ініціалізації (IV) та шифротекст у Base64
        iv = b64encode(cipher.iv).decode('utf-8')
        ct = b64encode(ct_bytes).decode('utf-8')
        
        # Пакуємо у JSON
        json_data = json.dumps({'iv': iv, 'ciphertext': ct})

        # Записуємо JSON у вихідний файл
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(json_data)

        print(f"\n✅ Файл успішно зашифровано алгоритмом {algo_name}.")
        print(f"📁 Зашифрований файл: {output_file}")
        print(f"🔑 Ключ збережено у: {key_file} (Збережіть його для розшифрування!)")

    except FileNotFoundError:
        print(f"\n❌ Помилка: Вхідний файл '{input_file}' не знайдено.")
    except Exception as e:
        print(f"\n❌ Помилка шифрування: {e}")

def decrypt_file(algo_name, input_file, output_file, key_file):
    try:
        # Читаємо ключ із файлу
        with open(key_file, 'rb') as f:
            key = f.read()

        # Читаємо JSON із зашифрованими даними
        with open(input_file, 'r', encoding='utf-8') as f:
            json_input = f.read()

        # Розпаковуємо JSON та декодуємо Base64
        b64 = json.loads(json_input)
        iv = b64decode(b64['iv'])
        ct = b64decode(b64['ciphertext'])

        # Ініціалізуємо шифратор для розшифрування
        if algo_name == 'DES':
            cipher = DES.new(key, DES.MODE_CBC, iv)
            block_size = DES.block_size
        else: # AES
            cipher = AES.new(key, AES.MODE_CBC, iv)
            block_size = AES.block_size

        # Розшифровуємо та знімаємо вирівнювання (unpad)
        pt = unpad(cipher.decrypt(ct), block_size)

        # Записуємо розшифровані байти у вихідний файл
        with open(output_file, 'wb') as f:
            f.write(pt)

        print(f"\n✅ Файл успішно розшифровано алгоритмом {algo_name}.")
        print(f"📁 Розшифрований текст збережено у: {output_file}")

    except FileNotFoundError:
        print(f"\n❌ Помилка: Файл або ключ не знайдено. Перевірте назви.")
    except Exception as e:
        print(f"\n❌ Помилка розшифрування (можливо, невірний ключ або алгоритм): {e}")

def main():
    print("--- Лабораторна робота №2: Алгоритми DES та AES ---")
    
    while True:
        print("\nОберіть дію:")
        print("1 - Зашифрувати файл")
        print("2 - Розшифрувати файл")
        print("0 - Вийти")
        
        action = input("Ваш вибір: ")
        
        if action == '0':
            break
        elif action not in ['1', '2']:
            print("Некоректне введення.")
            continue
            
        print("\nОберіть алгоритм:")
        print("1 - DES")
        print("2 - AES")
        algo_choice = input("Ваш вибір: ")
        
        algo_name = 'DES' if algo_choice == '1' else 'AES'
        
        in_file = input("Введіть ім'я вхідного файлу: ")
        out_file = input("Введіть ім'я вихідного файлу: ")
        key_file = input("Введіть ім'я файлу ключа (наприклад, key.bin): ")
        
        if action == '1':
            encrypt_file(algo_name, in_file, out_file, key_file)
        elif action == '2':
            decrypt_file(algo_name, in_file, out_file, key_file)

if __name__ == "__main__":
    main()