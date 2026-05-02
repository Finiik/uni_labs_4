from Crypto.PublicKey import RSA #[cite: 3]
from Crypto.Cipher import PKCS1_OAEP #[cite: 3]

def encrypt_file(input_filename, output_filename):
    try:
        # Імпорт відкритого ключа з файлу
        publickey = RSA.importKey(open('public.pem').read()) #[cite: 3]
        encryptor = PKCS1_OAEP.new(publickey) #[cite: 3]
        
        # Зчитування тексту з файлу
        with open(input_filename, 'r', encoding='utf-8') as f:
            msg = f.read()
            
        # Шифрування повідомлення
        encrypted = encryptor.encrypt(msg.encode('utf-8')) #[cite: 3]
        
        # Збереження зашифрованих байтів у файл
        with open(output_filename, 'wb') as f:
            f.write(encrypted)
            
        print(f"✅ Файл успішно зашифровано! Результат у '{output_filename}'.")
        
    except FileNotFoundError:
        print("❌ Помилка: Не знайдено файл з текстом або файл 'public.pem'.")

if __name__ == "__main__":
    print("--- Шифрування RSA ---")
    in_file = input("Введіть ім'я файлу для шифрування (напр., message.txt): ")
    out_file = input("Введіть ім'я для зашифрованого файлу (напр., encrypted.bin): ")
    encrypt_file(in_file, out_file)