import hashlib #

def hash_text_demo():
    print("--- 1. Хешування текстового рядка ---")
    # Перетворюємо рядок у байти за допомогою кодування UTF-8 #
    message = "Some text to hash".encode() #
    
    # MD5 (Застарілий, не рекомендується використовувати) #
    print(f"MD5:        {hashlib.md5(message).hexdigest()}") #
    
    # SHA-2 (SHA-256 та SHA-512 - надійні, але повільніші) #
    print(f"SHA-256:    {hashlib.sha256(message).hexdigest()}") #
    print(f"SHA-512:    {hashlib.sha512(message).hexdigest()}") #
    
    # SHA-3 (Більш безпечний, повністю відмінний від SHA-2) #[cite: 8]
    print(f"SHA-3-256:  {hashlib.sha3_256(message).hexdigest()}") #[cite: 8]
    print(f"SHA-3-512:  {hashlib.sha3_512(message).hexdigest()}") #[cite: 8]
    
    # BLAKE2 (Швидший за MD5/SHA і безпечніший за SHA-2) #[cite: 8]
    print(f"BLAKE2s:    {hashlib.blake2s(message).hexdigest()}") #[cite: 8]
    print(f"BLAKE2b:    {hashlib.blake2b(message).hexdigest()}") #[cite: 8]


def hash_file_demo(filename):
    print(f"\n--- 2. Хешування вмісту файлу '{filename}' ---")
    try:
        # Щоб захешувати файл, достатньо прочитати його в бінарному режимі ('rb') #[cite: 8]
        with open(filename, 'rb') as f:
            file_bytes = f.read() #[cite: 8]
            
        print(f"MD5:        {hashlib.md5(file_bytes).hexdigest()}")
        print(f"SHA-256:    {hashlib.sha256(file_bytes).hexdigest()}")
        print(f"BLAKE2b:    {hashlib.blake2b(file_bytes).hexdigest()}")
        
    except FileNotFoundError:
        print(f"❌ Помилка: Файл '{filename}' не знайдено.")


if __name__ == "__main__":
    # Демонстрація хешування звичайного тексту
    hash_text_demo()
    
    # Виконання пункту 6: створюємо тестовий файл
    test_file = "my_secret_data.txt"
    with open(test_file, "w", encoding="utf-8") as file:
        file.write("Це дуже важливий текст для перевірки цілісності даних.")
        
    # Хешуємо щойно створений файл
    hash_file_demo(test_file)