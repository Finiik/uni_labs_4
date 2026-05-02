def process_text(action, input_file, output_file, key):
    try:
        # Відкриваємо вхідний файл для читання та вихідний для запису з кодуванням utf-8
        with open(input_file, 'r', encoding='utf-8') as f_in, \
             open(output_file, 'w', encoding='utf-8') as f_out:
            
            # Програма зчитує початковий файл з текстом рядок за рядком
            for line in f_in:
                processed_line = ""
                
                # Розбиває кожний прочитаний рядок на символи
                for char in line:
                    # Перетворює символ в ASCII-код (в Python це Unicode-код точки)
                    ascii_code = ord(char)
                    
                    if action == '1': # Шифрування
                        # Додає до ASCII кодів довжину ключа
                        new_ascii = ascii_code + key
                    elif action == '2': # Розшифрування
                        # Віднімає ключ від ASCII-коду
                        new_ascii = ascii_code - key
                    
                    # Щоб уникнути помилок виходу за межі допустимих символів Python (0x110000)
                    new_ascii = new_ascii % 1114112 
                    
                    # Перетворює утворені ASCII-коди назад у символи
                    processed_line += chr(new_ascii)
                
                # Записує їх у файл
                f_out.write(processed_line)
                
        print(f"\n✅ Операцію успішно завершено! Результат записано у файл: {output_file}")
        
    except FileNotFoundError:
        print(f"\n❌ Помилка: Вхідний файл '{input_file}' не знайдено. Перевірте правильність шляху.")
    except Exception as e:
        print(f"\n❌ Виникла непередбачена помилка: {e}")

def main():
    print("--- Лабораторна робота №1: Шифр зсуву ---")
    
    while True:
        # 1. Користувач отримує запит про дію
        print("\nОберіть дію:")
        print("1 - Шифрувати інформацію")
        print("2 - Розшифрувати інформацію")
        print("0 - Вийти з програми")
        
        action = input("Ваш вибір: ")
        
        if action == '0':
            print("Роботу програми завершено.")
            break
        elif action not in ['1', '2']:
            print("Некоректне введення. Будь ласка, введіть 1, 2 або 0.")
            continue
            
        # 2. Відбувається прив'язка до двох файлів
        input_file = input("Введіть адресу (ім'я) вхідного файлу (наприклад, input.txt): ")
        output_file = input("Введіть адресу (ім'я) вихідного файлу (наприклад, output.txt): ")
        
        # 3. Користувач отримує запит про довжину ключа
        try:
            key = int(input("Введіть ключ (ціле число): "))
        except ValueError:
            print("\n❌ Помилка: Ключ має бути цілим числом. Спробуйте ще раз.")
            continue
            
        # Запуск обробки
        process_text(action, input_file, output_file, key)

if __name__ == "__main__":
    main()