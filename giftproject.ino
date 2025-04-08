
#include <Servo.h>

Servo pokrywkaServo;
Servo konfettiServo;

int buzzer = 8;
int przycisk = 2;
int ledy[] = {3, 4, 5, 6};

int czasMinuty = 0;
unsigned long ostatniKlik = 0;
bool odliczanie = false;

void setup() {
  pokrywkaServo.attach(9);
  konfettiServo.attach(10);

  pinMode(buzzer, OUTPUT);
  pinMode(przycisk, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(ledy[i], OUTPUT);
    digitalWrite(ledy[i], HIGH); //wszyskie ledy swieca
  }
}

void loop() {
  if (!odliczanie) {
    if (digitalRead(przycisk) == LOW) {
      delay(200); // likwidacja szumow
      czasMinuty++;
      if (czasMinuty > 4) czasMinuty = 1;
      pokazLedy(czasMinuty);
      ostatniKlik = millis();
    }

    if (czasMinuty > 0 && millis() - ostatniKlik >10000) { //uruchomienie trybu odliczania po 10 sekundach
      odliczanie = true;
      odliczanieCzasu(czasMinuty*60*1000);
      resetuj();
    }
  }
}

void pokazLedy(int ilosc) {
  for (int i = 0; i < 4; i++) {
    if (i < ilosc) digitalWrite(ledy[i],HIGH);
    else digitalWrite(ledy[i], LOW);
  }
}

void odliczanieCzasu(unsigned long czas) {
  unsigned long start = millis();
  unsigned long teraz = start;

  while (teraz - start < czas) {
    unsigned long zostalo = czas-(teraz - start);

    int beepCzas = 5000;
    if (zostalo <= 30000) beepCzas = 3000;
    if (zostalo <= 10000) beepCzas = 1000;
    if (zostalo <= 5000)  beepCzas = 500;
    if (zostalo <= 1000)  beepCzas = 100;

    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);

    migaj(zostalo);

    delay(beepCzas - 50);

    if (zostalo <= 1000) {
      pokrywkaServo.write(180);
      delay(2000);
      pokrywkaServo.write(0);
    }
    if (zostalo <= 3000) {
      konfettiServo.write(180);
      delay(200);
      konfettiServo.write(0);
    }

    teraz = millis();
  }
}

void migaj(unsigned long zostalo) {
  int czasMigania = 500;
  if (zostalo <= 30000) czasMigania = 300;
  if (zostalo <= 10000) czasMigania = 100;
  if (zostalo <= 5000) czasMigania = 50;

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledy[i], LOW);
  }
  delay(czasMigania / 2);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledy[i], HIGH);
  }
  delay(czasMigania / 2);
}

void resetuj() {
  odliczanie = false;
  czasMinuty = 0;
  pokrywkaServo.write(0);
  konfettiServo.write(0);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledy[i], HIGH); //powrót do początku
  }
}
