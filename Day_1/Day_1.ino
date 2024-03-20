#include <Servo.h>
String mes = "";
Servo s[5];
int Spins[5] = {3, 5, 6, 9, 10};
int Ppins[5] = {A0, A1, A2, A3, A4};
int L[5] = {180, 3, 0, 172, 168};
int K[5] = {0, 180, 180, 80, 0};
int OK [5] = {180, 3, 120, 109, 86};
int V [5] = {0, 3, 2, 63, 72};
int F [5] = {0, 180, 0, 130, 150};
float Speed [5] = {0.9, 0.9, 0.9, 0.04, 0.4};



void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i ++) {
    s[i].attach(Spins[i]);
    pinMode(Ppins[i], INPUT);
  }

}
void Ser(){
  if (mes == "P") {
    for (int i = 0; i < 5; i ++) {
      s[i].write(map(analogRead(Ppins[i]), 0, 1023, 0, 180));
      Serial.print(map(analogRead(Ppins[i]), 0, 1023, 0, 180));
      Serial.print(' ');
    }
    Serial.println(' ');
  }
  else if (mes == "OK"){
    for (int i = 0; i < 5; i ++) {
      s[i].write(L[i]);
    }
    for (int i = 0; i < 5; i ++) {
      for (int i = 0; i < 5; i ++) {
      s[i].write(L[i]);
    }
      s[i].write(OK[i]);
    }
  }
  else if (mes == "F"){
    for (int i = 0; i < 5; i ++) {
      s[i].write(F[i]);
    }
  }
  else if (mes == "K"){
    for (int i = 0; i < 5; i ++) {
      s[i].write(K[i]);
    }
  }
  else if (mes == "V"){
    for (int i = 0; i < 5; i ++) {
      s[i].write(V[i]);
    }
  }
  else {
    for (int i = 0; i < 5; i ++) {
      s[i].write(L[i]);
    }
  }
}

void loop() {
  if (Serial.available()) {
    mes = Serial.readString();
  }
  Ser();
  

}
