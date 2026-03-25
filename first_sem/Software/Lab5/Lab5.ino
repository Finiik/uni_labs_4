
#define BUTTON_PIN 4
#define LED_PIN 5

String inputString = "";
bool inputComplete = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("ESP32 UART Lab");
  Serial.println("Enter number of LED blinks and press Enter:");
}

void loop() {
  // Read serial input
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputComplete = true;
      break;
    } else if (isDigit(c)) {
      inputString += c;
    }
  }

  // When user presses Enter
  if (inputComplete) {
    int blinkCount = inputString.toInt();

    if (blinkCount > 0) {
      Serial.print("Blinking LED ");
      Serial.print(blinkCount);
      Serial.println(" times...");

      for (int i = 0; i < blinkCount; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
      }

      Serial.println("Done!");
    } else {
      Serial.println("Invalid number! Try again.");
    }

    // Reset for next input
    inputString = "";
    inputComplete = false;
    Serial.println("\nEnter another number:");
  }
}
