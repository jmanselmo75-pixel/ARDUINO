#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo barrier;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigPin 9
#define echoPin 10
#define waterSensor A0
#define buzzer 7

#define redPin 3
#define greenPin 4
#define bluePin 5

#define servoPin 6

long duration;
int distance;
int waterValue;

int warnPosition = 45;   // WARNING position
int floodPosition = 130;  // FLOOD position

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  barrier.attach(servoPin);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  delay(500);
}

void loop() {

  // Ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Water sensor
  waterValue = analogRead(waterSensor);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.print(distance);
  lcd.print(" cm");

  // ================= FLOOD =================
  if (distance <= 2 || waterValue > 500) {
    lcd.setCursor(0,1);
    lcd.print("Status: FLOOD");

    barrier.write(floodPosition);

    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);

    // Alarm
    for(int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
    }
  }

  // ================= WARNING =================
  else if (distance > 2 && distance < 5) {
    lcd.setCursor(0,1);
    lcd.print("Status: WARN");

    barrier.write(warnPosition);

    // YELLOW
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);

    digitalWrite(buzzer, LOW);
  }

  // ================= NORMAL =================
  else {
    lcd.setCursor(0,1);
    lcd.print("Status: NORMAL");

    //  WARNING POSITION
    barrier.write(warnPosition);

    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);

    digitalWrite(buzzer, LOW);
  }

  delay(500);
}