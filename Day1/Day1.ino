unsigned long time;
#include <Servo.h>
#define DATA 13
#define LATCH 12
#define CLOCK 11
String com = "";
Servo s0;
Servo s1;
Servo s2;
Servo s3;
Servo s4;
int a0;
int a1;
int a2;
int a3;
int a4;

void setup() {
  Serial.begin(9600);
  s0.attach(3);
  s1.attach(5);
  s2.attach(6);
  s3.attach(9);
  s4.attach(10);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
}

void loop() {
  if (Serial.available()) {
    com = Serial.readString(); //Serial.readStringUntil('\n');1
  }
  //Serial.println(com);
  if (com == "L") {
    s0.write(180);
    s1.write(0);
    s2.write(0);
    s3.write(180);
    s4.write(112);
    digitalWrite(LATCH, 0);
    shiftOut(DATA, CLOCK, LSBFIRST, 0b00011001);
    digitalWrite(LATCH, 1);
  }
  if (com == "K") {
    s0.write(0);
    s1.write(180);
    s2.write(175);
    s3.write(93);
    s4.write(37);
    time = millis() / 500 % 2;
    if (time) {
      digitalWrite(LATCH, 0);
      shiftOut(DATA, CLOCK, LSBFIRST, 0b00101111);
      digitalWrite(LATCH, 1);
    }
    else {
      digitalWrite(LATCH, 0);
      shiftOut(DATA, CLOCK, LSBFIRST, 0b00001011);
      digitalWrite(LATCH, 1);
    }
  }
  if (com == "OK") {
    s0.write(180);
    s1.write(0);
    s2.write(123);
    s3.write(138);
    s4.write(29);
    time = millis() / 500 % 2;
    if (time) {
      digitalWrite(LATCH, 0);
      shiftOut(DATA, CLOCK, LSBFIRST, 0b01111101);
      digitalWrite(LATCH, 1);
    }
    else {
      digitalWrite(LATCH, 0);
      shiftOut(DATA, CLOCK, LSBFIRST, 0b00101111);
      digitalWrite(LATCH, 1);
    }
  }
  if (com == "1") {
    s0.write(0);
    s1.write(180);
    s2.write(0);
    s3.write(175);
    s4.write(52);
    digitalWrite(LATCH, 0);
    shiftOut(DATA, CLOCK, LSBFIRST, 0b00100100);
    digitalWrite(LATCH, 1);
  }
  if (com == "P") {
    a0 = map(analogRead(A0), 0, 1023, 0, 180);
    a1 = map(analogRead(A1), 0, 1023, 0, 180);
    a2 = map(analogRead(A2), 0, 1023, 0, 180);
    a3 = map(analogRead(A3), 0, 1023, 0, 180);
    a4 = map(analogRead(A4), 0, 1023, 0, 180);
    s0.write(a0);
    s1.write(a1);
    s2.write(a2);
    s3.write(a3);
    s4.write(a4);
    digitalWrite(LATCH, 0);
    shiftOut(DATA, CLOCK, LSBFIRST, 0b01101011);
    digitalWrite(LATCH, 1);
    Serial.print(a0);
    Serial.print(' ');
    Serial.print(a1);
    Serial.print(' ');
    Serial.print(a2);
    Serial.print(' ');
    Serial.print(a3);
    Serial.print(' ');
    Serial.println(a4);
  }
}
