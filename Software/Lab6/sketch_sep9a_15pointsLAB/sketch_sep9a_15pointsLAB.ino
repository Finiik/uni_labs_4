#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Lab-324";      // <-- Введи свою WiFi мережу
const char* password = "lab324!!";  // <-- Введи пароль

WebServer server(80);

int temperature = 0;

void handleRoot() {
  String html = "<html><head>"
                "<meta http-equiv='refresh' content='2' />"
                "<style>body{font-family:Arial;text-align:center;padding-top:40px;font-size:28px;}</style>"
                "</head><body>"
                "<h2>Температура ESP32</h2>"
                "<p>" + String(temperature) + " °C</p>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleJSON() {
  String json = "{\"temperature\": " + String(temperature) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);

  // WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Web server endpoints
  server.on("/", handleRoot);
  server.on("/data", handleJSON);
  server.begin();

  // Random seed
  randomSeed(analogRead(0));
}

void loop() {
  server.handleClient();

  // Generate fake temperature 0..30°C
  temperature = random(0, 31);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  delay(2000);
}
