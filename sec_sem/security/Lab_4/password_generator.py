from argparse import ArgumentParser #
import secrets #
import random #
import string #

# Налаштування Argument Parser
parser = ArgumentParser(
    prog='Password Generator', #[cite: 4]
    description='Generate any number of passwords with this tool.' #[cite: 4]
)

# Додавання аргументів до парсера
parser.add_argument("-n", "--numbers", default=0, help="Number of digits in the PW", type=int) #[cite: 4]
parser.add_argument("-l", "--lowercase", default=0, help="Number of lowercase chars in the PW", type=int) #[cite: 4]
parser.add_argument("-u", "--uppercase", default=0, help="Number of uppercase chars in the PW", type=int) #[cite: 4]
parser.add_argument("-s", "--special-chars", default=0, help="Number of special chars in the PW", type=int) #[cite: 4]

# Додавання аргументу загальної довжини
parser.add_argument("-t", "--total-length", type=int,
                    help="The total password length. If passed, it will ignore -n, -l, -u and -s, "
                         "and generate completely random passwords with the specified length") #[cite: 4]

# Кількість паролів (перевіряємо, щоб тип був int)
parser.add_argument("-a", "--amount", default=1, type=int) #[cite: 4]
parser.add_argument("-o", "--output-file") #[cite: 4]

# Парсинг аргументів командного рядка
args = parser.parse_args() #[cite: 4]

# Список паролів
passwords = [] #[cite: 4]

# Цикл для заданої кількості паролів
for _ in range(args.amount): #[cite: 4]
    if args.total_length: #[cite: 4]
        # Генерація випадкового пароля на основі загальної довжини з усіх доступних символів
        passwords.append("".join(
            [secrets.choice(string.digits + string.ascii_letters + string.punctuation) \
             for _ in range(args.total_length)])) #[cite: 4]
    else:
        password = [] #[cite: 4]
        
        # Скільки цифр має містити пароль
        for _ in range(args.numbers): #[cite: 4]
            password.append(secrets.choice(string.digits)) #[cite: 4]
            
        # Скільки великих літер має містити пароль
        for _ in range(args.uppercase): #[cite: 4]
            password.append(secrets.choice(string.ascii_uppercase)) #[cite: 4]
            
        # Скільки маленьких літер має містити пароль
        for _ in range(args.lowercase): #[cite: 4]
            password.append(secrets.choice(string.ascii_lowercase)) #[cite: 4]
            
        # Скільки спеціальних символів має містити пароль
        for _ in range(args.special_chars): #[cite: 4]
            password.append(secrets.choice(string.punctuation)) #[cite: 4]
            
        # Перемішування списку з усіма можливими літерами, цифрами та символами
        random.shuffle(password) #[cite: 4]
        
        # Об'єднання символів у рядок
        password = "".join(password) #[cite: 4]
        
        # Додавання цього пароля до загального списку
        passwords.append(password) #[cite: 4]

# Збереження пароля у .txt файл
if args.output_file: #[cite: 4]
    # Використовуємо 'a' (append) замість 'w' для збереження всіх результатів у один файл
    with open(args.output_file, 'a') as f: #[cite: 4]
        f.write('\n'.join(passwords) + '\n') #[cite: 4]

print('\n'.join(passwords)) #[cite: 4]