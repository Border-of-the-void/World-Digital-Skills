#include <Servo.h>
#define button 11
int led [3] = {4, 7, 8};              // Пины светодиодов: красный, жёлтый, зелёный
long int Time [2] = {0, 0};           // Время: входа в цикл, нажатия кнопки
bool but [2] = {0, 0};                // Состояние кнопки: новое, старое
bool work = true;                     // Состояние аварийного режима
int pins[5] = {3, 5, 6 , 9, 10};
Servo s[5];
int L[5] = {180, 0, 0, 180, 112};
int K[5] = {0, 180, 175, 93, 37};
int OK[5] = {180, 0, 123, 138, 29};
int F[5] = {0, 180, 0, 175, 52};
String com = "";
int emg0[64];
int emg1[64];
int amp0 = 0;
int amp1 = 0;
int min0 = 1023;
int min1 = 1023;
int max0 = 0;
int max1 = 0;

void amplitude() {
  min0 = 1023;
  min1 = 1023;
  max0 = 0;
  max1 = 0;
  for (int i; i < 63; i++) {
    emg0[i] = emg0[i + 1];
    emg1[i] = emg1[i + 1];
  }
  emg0[63] = analogRead(A0);
  emg1[63] = analogRead(A1);
  for (int i; i < 64; i++) {
    if (emg0[i] < min0) {
      min0 = emg0[i];
    }
    else if (emg0[i] > max0) {
      max0 = emg0[i];
    }
    if (emg1[i] < min1) {
      min1 = emg1[i];
    }
    else if (emg1[i] > max1) {
      max1 = emg1[i];
    }
    Serial.print(amp0);
    Serial.print(",");
    Serial.println(amp1);
  }
  amp0 = 0.3 * (max0 - min0) + 0.7 * amp0;
  amp1 = 0.3 * (max1 - min1) + 0.7 * amp1;
}

void checkButon() {                   // Проверка состояния кнопки
  but[1] = but[0];
  but[0] = !digitalRead(button);
  if (but[0] and but[0] != but[1]) {
    Time[1] = millis() + 3000;
    work = false;
  }
  if (not(but[0]) and but[0] != but[1]) {
  }
}

void crash() {                         // Включение аварийного режима
  if (Time[0] % 250 > 125) {
    digitalWrite(led[0], 0);
    digitalWrite(led[1], 1);
  }
  else {
    digitalWrite(led[0], 1);
    digitalWrite(led[1], 0);
  }
  digitalWrite(led[2], 0);
  if (but[0] and Time[0] > Time[1]) {
    work = true;
  }
  for (int i = 0; i < 5; i++) {
    s[i].write(L[i]);
  }
}

void act() {                                   // Работа в штатном режиме
  digitalWrite(led[0], 0);
  digitalWrite(led[1], 0);
  digitalWrite(led[2], 1);
  serial_check();
  amplitude();
}

void serial_check() {
  if (Serial.available()) {
    com = Serial.readString();
  }
  if (com == "L") {
    for (int i = 0; i < 5; i++) {
      s[i].write(L[i]);
    }
  }
  if (com == "K") {
    for (int i = 0; i < 5; i++) {
      s[i].write(K[i]);
    }
  }
  if (com == "OK") {
    for (int i = 0; i < 5; i++) {
      s[i].write(OK[i]);
    }
  }
  if (com == "1") {
    for (int i = 0; i < 5; i++) {
      s[i].write(F[i]);
    }
  }
}




void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    s[i].attach(pins[i]);
  }
  for (int i; i < 64; i++) {
    emg0[i] = analogRead(A0);
    emg1[i] = analogRead(A1);
  }
  pinMode(button, INPUT_PULLUP);
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  pinMode(led[2], OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
}

void loop() {
  Time[0] = millis();                            // Время входа в цикл
  checkButon();                                  // Проверка состояния кнопки
  if (work) {
    act();                                       // Включение аварийного режима
  }
  else {
    crash();                                     // Работа в штатном режиме
  }
}
