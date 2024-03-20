int emg0 [64];
int emg1 [64];
int amp[2] = {0, 0};
int Max[2] = {0, 0};
int Min[2] = {1000, 1000};
int th[2] = {0, 0};
int L[2] = {0, 0};
byte B [11] = {0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b11000000, 0b11100000, 0b11110000, 0b11111000, 0b11111100, 0b11111110, 0b11111111};
long int Time[2] = {0, 0};

void amplitude() {
  for (int i = 0; i < 63; i++) {
    emg0[i] = emg0[i + 1];
    emg1[i] = emg1[i + 1];
  }
  emg0[63] = analogRead(A0);
  emg1[63] = analogRead(A1);
  Min[0] = Min[1] = 1024;
  Max[0] = Max[1] = 0;
  for (int i = 0; i < 64; i++) {
    if (emg0[i] < Min[0]) {
      Min[0] = emg0[i];
    }
    if (emg0[0] > Max[0]) {
      Max[0] = emg0[i];
    }
    if (emg1[i] < Min[1]) {
      Min[1] = emg1[i];
    }
    if (emg1[0] > Max[1]) {
      Max[1] = emg1[i];
    }
  }
  amp[0] = 0.7 * amp[0] + 0.3 * (Max[0] - Min[0]);
  amp[1] = 0.7 * amp[1] + 0.3 * (Max[1] - Min[1]);
  th[0] = analogRead(A2);
  th[1] = analogRead(A3);
  if (amp[0] > th[0] and Time[0] > Time[1]) {
    L[1] = L[0];
    L[0] = min(10, L[0] + 1);
    Time[1] = Time[0] + 2000;
  }
  if (amp[1] > th[1] and Time[0] > Time[1]) {
    L[1] = L[0];
    L[0] = max(0, L[0] - 1);
    Time[1] = Time[0] + 2000;
  }
}


void led () {
  digitalWrite(10, 0);
  shiftOut(12, 11, LSBFIRST, B[L[0]]);
  digitalWrite(10, 1);
  digitalWrite(9, L[0] >= 1);
  digitalWrite(8, L[0] >= 2);
}


void output(){
  Serial.print(amp[0]);
  Serial.print(',');
  Serial.print(amp[1]);
  Serial.print(',');
  Serial.print(th[0]);
  Serial.print(',');
  Serial.print(th[1]);
  Serial.print(',');
  Serial.println(L[0]*100);
}

void setup() {
  Serial.begin(9600);
  for (int i = 8; i < 13; i++) {
    pinMode(i, OUTPUT);
  }
  for (int i = 0; i < 64; i++) {
    emg0[i] = analogRead(A0);
    emg1[i] = analogRead(A1);
  }
}

void loop() {
  Time[0] = millis();
  amplitude();
  if(L[0]!=L[1]){
    led();
  }
  output();
}
