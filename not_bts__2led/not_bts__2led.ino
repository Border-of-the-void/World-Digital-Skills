int min0, min1;
int max0, max1;
int amp0 = 0;
int amp1 = 0;
int mas0[64];
int mas1[64];
int Time;
#define ts 80

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  for (int i = 0; i < 64; i++) {
    mas0[i] = analogRead(A0);
    mas1[i] = analogRead(A1);
  }
}

void loop() {
  for (int i = 0; i < 63; i++) {
    mas0[i] = mas0[i + 1] ;
    mas1[i] = mas1[i + 1];
  }
  mas0[63] = analogRead(A0);
  mas1[63] = analogRead(A1);
  min0 = min1 = 1024;
  max0 = max1 = 0;
  for (int i = 0; i < 64; i++) {
    if (mas0[i]  < min0) {
      min0 = mas0[i];
    }
    if (mas0[i] > max0) {
      max0 = mas0[i];
    }
    if (mas1[i]  < min1) {
      min1 = mas1[i];
    }
    if (mas1[i] > max1) {
      max1 = mas1[i];
    }
  }
  amp0 = 0.3 * (max0 - min0) + 0.7 * amp0;
  amp1 = 0.3 * (max1 - min1) + 0.7 * amp1;
  
  //LED
  if (amp0 < ts) {
    digitalWrite(3, 0);
  }
  if (amp1 < ts) {
    digitalWrite(5, 0);
  }
  //FIRST
  if (amp0 > ts and amp1 < ts) {
    Time = millis() % 1000;
    if (Time < 200) {
      digitalWrite(3, 1);
    }
    else {
      digitalWrite(3, 0);
    }
  }
  //SECOND
  if (amp0 < ts and amp1 > ts) {
    Time = millis() % 500;
    if (Time < 250) {
      digitalWrite(5, 1);
    }
    else {
      digitalWrite(5, 0);
    }
  }
  //TOGETHER
  if (amp0 > ts and amp1 > ts) {
    Time = millis() % 1000;
    if (Time < 500) {
      digitalWrite(3, 1);
      digitalWrite(5, 0);
    }
    else {
      digitalWrite(3, 0);
      digitalWrite(5, 1);
    }
  
  }
  Serial.print(amp0);
  Serial.print(',');
  Serial.println(amp1);
}
