#include <fft.h>
#include <Servo.h>
#define num 256
#define aThreashold 2.6


int8_t im[num], data[num];
int i = 0, val;
bool fail = false;
bool work = true;
Servo s[5];
int p[5] = {3, 5, 6, 9, 10};
int emg[2][16];
int amp[2];
int Max[2];
int Min[2];
long int Time[2] = {0, 0};
int th[2];
int Signs[4][5] = {{180, 3, 0, 172, 168},                                //Ладонь
                   {0, 180, 180, 80, 0},                                 //Кулак
                   {180, 3, 120, 109, 86},                               //Ок
                   {0, 3, 0, 172, 168}};                                 //Тыдыщ


void act() {
  calc();
  th[0] = analogRead(A2);
  th[1] = analogRead(A3);
  if (amp[0] < th[0] and amp[1] < th[1]) {
    for (int i = 0; i < 5; i++) {
      s[i].write(Signs[0][i]);
    }
  }
  if (amp[0] > th[0] and amp[1] > th[1]) {
    for (int i = 0; i < 5; i++) {
      s[i].write(Signs[1][i]);
    }
  }
  if (amp[0] > th[0] and amp[1] < th[1]) {
    for (int i = 0; i < 5; i++) {
      s[i].write(Signs[2][i]);
    }
  }
  if (amp[0] < th[0] and amp[1] > th[1]) {
    for (int i = 0; i < 5; i++) {
      s[i].write(Signs[3][i]);
    }
  }
  digitalWrite(7, 0);
  Serial.print(amp[0]);
  Serial.print(",");
  Serial.print(amp[1]);
  Serial.print(",");
  Serial.print(th[0]);
  Serial.print(",");
  Serial.println(th[1]);
}


void calc() {
  for (int i = 0; i < 2; i++) {
    Max[i] = 0;
    Min[i] = 1023;
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 15; j++) {
      emg[i][j] = emg[i][j + 1];
    }
    emg[i][15] = analogRead(i);
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 16; j++) {
      if (emg[i][j] > Max[i]) {
        Max[i] = emg[i][j];
      }
      if (emg[i][j] < Min[i]) {
        Min[i] = emg[i][j];
      }
    }
    amp[i] = 0.7 * amp[i] + 0.3 * (Max[i] - Min[i]);
  }
}


void eeg() {
  int8_t sum = 0;
  for (i = 0; i < num; i++) {
    data[i] = analogRead(A4) / 8;
    if (data[i] < 2 || data[i] > 120) {
      fail = true;
    }
    //delay(2);
    im[i] = 0;
    sum = sum + data[i];
  }
  if (fail) {
    work = false;
    Time[1] = Time[0] + 6000;
    digitalWrite(4, 1);
    digitalWrite(2, 0);
  }
  else {
    digitalWrite(4, 0);
    for (i = 0; i < num; i++) {
      data[i] = data[i] - sum / num;
    }
    fix_fft(data, im, 8, 0);
    bool flag = false;
    for (i = 6; i < 15; i++) {
      if (sqrt(data[i]*data[i] + im[i]*im[i]) > aThreashold) {
        flag = true;
      }
    }
    if (flag) {
      work = false;
      Time[1] = Time[0] + 500;
      digitalWrite(2, 1);
      flag = false;
    }
    else {
      digitalWrite(2, 0);
    }
  }
  fail = false;
}


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {s[i].attach(p[i]);}
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT);
  for (int i = 0; i < 2; i ++) {for (int j = 0; j < 16; j++) {emg[i][j] = analogRead(i);}}
}


void loop() {
  Time[0] = millis();
  eeg();
  if (work) {act();}
  else {
    digitalWrite(7, 1);
    for (int i = 0; i < 5; i++) {s[i].write(Signs[0][i]);}
    if (Time[0] > Time[1]) {work = 1;}
  }
}
