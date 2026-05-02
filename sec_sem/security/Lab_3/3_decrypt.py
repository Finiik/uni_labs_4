from Crypto.PublicKey import RSA #[cite: 3]
from Crypto.Cipher import PKCS1_OAEP #[cite: 3]

def decrypt_file(input_filename, output_filename):
    try:
        # Імпорт закритого ключа з файлу
        privatekey = RSA.importKey(open('private.pem').read()) #[cite: 3]
        decryptor = PKCS1_OAEP.new(privatekey) #[cite: 3]
        
        # Зчитування зашифрованих даних
        with open(input_filename, 'rb') as f:
            encrypted = f.read()
            
        # Розшифрування повідомлення
        decrypted = decryptor.decrypt(encrypted) #[cite: 3]
        
        # Збереження розшифрованого тексту
        with open(output_filename, 'w', encoding='utf-8') as f:
            f.write(decrypted.decode('utf-8'))
            
        print(f"✅ Файл успішно розшифровано! Результат у '{output_filename}'.")
        
    except FileNotFoundError:
        print("❌ Помилка: Не знайдено зашифрований файл або файл 'private.pem'.")
    except ValueError:
        print("❌ Помилка: Не вдалося розшифрувати файл. Можливо, використано неправильний закритий ключ.")

if __name__ == "__main__":
    print("--- Розшифрування RSA ---")
    in_file = input("Введіть ім'я зашифрованого файлу (напр., encrypted.bin): ")
    out_file = input("Введіть ім'я для розшифрованого файлу (напр., decrypted.txt): ")
    decrypt_file(in_file, out_file)