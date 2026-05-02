import requests #
from bs4 import BeautifulSoup as bs #
from urllib.parse import urljoin #
from pprint import pprint #
import sys #

# Ініціалізуємо сеанс запитів і встановлюємо User-Agent #[cite: 6]
s = requests.Session() #[cite: 6]
s.headers["User-Agent"] = "Chrome/83.0.4103.106" #[cite: 6]

def get_all_forms(url): #[cite: 6]
    """Отримує всі теги form з HTML-сторінки.""" #[cite: 6]
    soup = bs(s.get(url).content, "html.parser") #[cite: 6]
    return soup.find_all("form") #[cite: 6]

def get_form_details(form): #[cite: 6]
    """Аналізує корисну інформацію про форму (action, method, inputs).""" #[cite: 6]
    details = {} #[cite: 6]
    
    # Отримуємо цільову URL-адресу форми (action) #[cite: 6]
    try:
        action = form.attrs.get("action").lower() #[cite: 6]
    except:
        action = None #[cite: 6]
        
    # Отримуємо метод форми (POST, GET тощо) #[cite: 6]
    method = form.attrs.get("method", "get").lower() #[cite: 6]
    
    # Отримуємо всі атрибути поля введення (type, name, value) #[cite: 6]
    inputs = [] #[cite: 6]
    for input_tag in form.find_all("input"): #[cite: 6]
        input_type = input_tag.attrs.get("type", "text") #[cite: 6]
        input_name = input_tag.attrs.get("name") #[cite: 6]
        input_value = input_tag.attrs.get("value", "") #[cite: 6]
        inputs.append({"type": input_type, "name": input_name, "value": input_value}) #[cite: 6]
        
    # Додаємо все у словник #[cite: 6]
    details["action"] = action #[cite: 6]
    details["method"] = method #[cite: 6]
    details["inputs"] = inputs #[cite: 6]
    
    return details #[cite: 6]

def is_vulnerable(response): #[cite: 6]
    """Перевіряє, чи є на веб-сторінці помилки SQL у відповіді.""" #[cite: 6]
    errors = {
        # MySQL #[cite: 6]
        "you have an error in your sql syntax;", #[cite: 6]
        "warning: mysql", #[cite: 6]
        # SQL Server #[cite: 6]
        "unclosed quotation mark after the character string", #[cite: 6]
        # Oracle #[cite: 6]
        "quoted string not properly terminated", #[cite: 6]
    }
    
    for error in errors: #[cite: 6]
        # Якщо знайдено хоча б одну з помилок, повертаємо True #[cite: 6]
        if error in response.content.decode().lower(): #[cite: 6]
            return True #[cite: 6]
            
    # Помилок не виявлено #[cite: 6]
    return False #[cite: 6]

def scan_sql_injection(url): #[cite: 6]
    """Шукає форми та намагається розмістити лапки у полях введення і URL.""" #[cite: 6]
    # Перевірка самої URL-адреси #[cite: 6]
    for c in "\"'": #[cite: 6]
        # Додаємо символ лапки до URL-адреси #[cite: 6]
        new_url = f"{url}{c}" #[cite: 6]
        print("[!] Trying", new_url) #[cite: 6]
        
        # Робимо HTTP запит #[cite: 6]
        res = s.get(new_url) #[cite: 6]
        
        if is_vulnerable(res): #[cite: 6]
            print("[+] SQL Injection vulnerability detected, link:", new_url) #[cite: 6]
            return #[cite: 6]
            
    # Перевірка HTML-форм #[cite: 6]
    forms = get_all_forms(url) #[cite: 6]
    print(f"[+] Detected {len(forms)} forms on {url}.") #[cite: 6]
    
    for form in forms: #[cite: 6]
        form_details = get_form_details(form) #[cite: 6]
        
        for c in "\"'": #[cite: 6]
            data = {} #[cite: 6]
            
            for input_tag in form_details["inputs"]: #[cite: 6]
                if input_tag["value"] or input_tag["type"] == "hidden": #[cite: 6]
                    try:
                        data[input_tag["name"]] = input_tag["value"] + c #[cite: 6]
                    except:
                        pass #[cite: 6]
                elif input_tag["type"] != "submit": #[cite: 6]
                    data[input_tag["name"]] = f"test{c}" #[cite: 6]
                    
            # Об'єднуємо URL-адресу з action (URL-адресою запиту форми) #[cite: 6]
            form_url = urljoin(url, form_details["action"]) #[cite: 6]
            
            if form_details["method"] == "post": #[cite: 6]
                res = s.post(form_url, data=data) #[cite: 6]
            elif form_details["method"] == "get": #[cite: 6]
                res = s.get(form_url, params=data) #[cite: 6]
                
            # Перевіряємо, чи вразлива результуюча сторінка #[cite: 6]
            if is_vulnerable(res): #[cite: 6]
                print("[+] SQL Injection vulnerability detected, link:", form_url) #[cite: 6]
                print("[+] Form:") #[cite: 6]
                pprint(form_details) #[cite: 6]
                break #[cite: 6]

if __name__ == "__main__": #[cite: 6]
    if len(sys.argv) > 1:
        url = sys.argv[1] #[cite: 6]
        scan_sql_injection(url) #[cite: 6]
    else:
        print("❌ Помилка: Вкажіть URL для перевірки.")
        print("Використання: python3 sql_injection_detector.py <URL>")