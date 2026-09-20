#define TRIG_PIN 14
#define ECHO_PIN 12
#define LED_PIN 27

float threshold = 100.0;
float distance;
float percentage;

int currentLevel = 0;
int previousLevel = -1;

unsigned long lastMeasurement = 0;
const unsigned long measurementInterval = 10000;

unsigned long previousMillis = 0;

int blinkCount = 0;
bool ledState = false;

bool patternActive = false;
bool continuousBlink = false;

int blinkLimit = 0;

float getDistance();
void readThreshold();
void startPattern(int level);
void updateLED();

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.println("SMART PEOPLE DETECTION SYSTEM");
  Serial.println("Enter threshold distance in cm:");
  Serial.println("Example: 100");
  Serial.println("Measurement interval: 10 seconds");
}

void loop() {
  readThreshold();

  if (millis() - lastMeasurement >= measurementInterval) {
    lastMeasurement = millis();

    distance = getDistance();

    if (distance <= 0) {
      Serial.println("No object detected");
      currentLevel = 0;
    } else {
      percentage = ((threshold - distance) / threshold) * 100;
      percentage = constrain(percentage, 0, 100);

      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" cm | Percentage: ");
      Serial.print(percentage);
      Serial.println("%");

      if (percentage >= 95) {
        currentLevel = 3;
      } else if (percentage >= 60) {
        currentLevel = 2;
      } else if (percentage >= 30) {
        currentLevel = 1;
      } else {
        currentLevel = 0;
      }
    }

    if (currentLevel != previousLevel) {
      startPattern(currentLevel);
      previousLevel = currentLevel;
    }
  }

  updateLED();

  delay(10);
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2;
}

void readThreshold() {
  if (Serial.available()) {
    float newThreshold = Serial.parseFloat();

    if (newThreshold > 0) {
      threshold = newThreshold;

      Serial.print("New Threshold: ");
      Serial.print(threshold);
      Serial.println(" cm");

      while (Serial.available()) {
        Serial.read();
      }

      previousLevel = -1;
    }
  }
}

void startPattern(int level) {
  blinkCount = 0;
  ledState = false;

  digitalWrite(LED_PIN, LOW);

  patternActive = true;
  continuousBlink = false;

  if (level == 0) {
    patternActive = false;
    Serial.println("LED OFF");
  } else if (level == 1) {
    blinkLimit = 1;
    Serial.println("30% LEVEL - BLINK ONCE");
  } else if (level == 2) {
    blinkLimit = 2;
    Serial.println("60% LEVEL - BLINK TWICE");
  } else if (level == 3) {
    continuousBlink = true;
    Serial.println("95% LEVEL - CONTINUOUS BLINKING");
  }

  previousMillis = millis();
}

void updateLED() {
  if (!patternActive) {
    digitalWrite(LED_PIN, LOW);
    return;
  }

  unsigned long currentMillis = millis();

  if (continuousBlink) {
    if (currentMillis - previousMillis >= 300) {
      previousMillis = currentMillis;

      ledState = !ledState;

      digitalWrite(LED_PIN, ledState);
    }

    return;
  }

  if (currentMillis - previousMillis >= 200) {
    previousMillis = currentMillis;

    ledState = !ledState;

    digitalWrite(LED_PIN, ledState);

    if (ledState == false) {
      blinkCount++;

      if (blinkCount >= blinkLimit) {
        patternActive = false;
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}
