from Crypto.PublicKey import RSA #

def generate_keys():
    print("Генерація пари RSA ключів (це може зайняти кілька секунд)...")
    # Генерація ключів розміром 3072 біти
    keyPair = RSA.generate(3072) #
    
    # Створення та збереження відкритого ключа у файл
    pubKey = keyPair.publickey() #
    pubKeyPEM = pubKey.exportKey() #
    with open("public.pem", "wb") as file_out: #
        file_out.write(pubKeyPEM) #
    print("✅ Відкритий ключ збережено у файл 'public.pem'")
    
    # Створення та збереження закритого ключа у файл
    privKeyPEM = keyPair.exportKey() #
    with open("private.pem", "wb") as file_out: #[cite: 3]
        file_out.write(privKeyPEM) #[cite: 3]
    print("✅ Закритий ключ збережено у файл 'private.pem'")

if __name__ == "__main__":
    generate_keys()