#include <Servo.h>
int pins[5] = {3, 5, 6 , 9, 10};
int led[2] = {2, 4};
bool data0[2] = {0, 0};
bool data1[2] = {0, 0};
int emg0[64];
int emg1[64];
int amp0 = 0;
int amp1 = 0;
int min0 = 1023;
int min1 = 1023;
int max0 = 0;
int max1 = 0;
int Switch = 0;
bool Choose = 0;
int* G[6][5] = {{0, 180, 175, 93, 37}, {180, 0, 110, 17, 140}, {0, 180, 0, 70, 110}, {180, 180, 0, 110, 180}, {0, 180, 175, 110, 180}, {180, 180, 175, 110, 180}};
int L[5] = {180, 0, 0, 110, 180};
Servo s[5];


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    s[i].attach(pins[i]);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(led[i], OUTPUT);
  }
  for (int i = 0; i < 64; i++) {
    emg0[i] = analogRead(A0);
    emg1[i] = analogRead(A1);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}
void loop() {
  for (int i = 0; i < 63; i++) {
    emg0[i] = emg0[i + 1];
    emg1[i] = emg1[i + 1];
  }
  emg0[63] = analogRead(A0);
  emg1[63] = analogRead(A1);
  min0 = 1023;
  min1 = 1023;
  max0 = 0;
  max1 = 0;
  for (int i = 0; i < 64; i++) { //   поиск минимума и максимума
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
  }
  amp0 = 0.3 * (max0 - min0) + 0.7 * amp0;
  amp1 = 0.3 * (max1 - min1) + 0.7 * amp1;
  data0[1] = data0[0];
  data1[1] = data1[0];
  data0[0] = amp0 > 300;
  data1[0] = amp1 > 470;
  if (data0[0] and data0[0] != data0[1]) {
    Switch = (Switch + 1) % 6;
    digitalWrite(led[0], Switch%2);
    digitalWrite(led[1], (Switch+1)%2);
  }
  if (data1[0] and data1[0] != data1[1]) {
    Choose = 1;
  }
  else if (data1[0] == 0 and data1[0] != data1[1]) {
    Choose = 0;
  }
  if (Choose) {
    for (int i = 0; i < 5; i++) {
      s[i].write(G[Switch][i]);
    }
  }
  else {
    for (int i = 0; i < 5; i++) {
      s[i].write(L[i]);
    }
  }
  Serial.print(amp0);
  Serial.print(',');
  Serial.println(amp1);
}
