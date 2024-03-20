#include <Servo.h>
float Speed [5] = {0.9, 0.9, 0.9, 0.03, 0.2};
int L[5] = {180, 0, 0, 110, 180};
int K[5] = {0, 180, 175, 30, 110};
float N[5] = {180, 0, 0, 110, 180};
bool but[2] = {0, 0};
int p[5] = {3, 5, 6, 9, 10};
Servo s[5];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    s[i].attach(p[i]);
  }
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  but[1] = but[0];
  but[0] = !digitalRead(11);
  if (but[0]) {
    for (int i = 0; i < 5; i++) {
      N[i] = s[i].read();
      while (abs(N[i]  - L[i]) > Speed[i]) {
        N[i] += Speed[i] * abs(L[i] - N[i]) / (L[i] - N[i]);
        s[i].write(N[i]);

      }
    }
  }
  if (not(but[0])) {
    for (int i = 0; i < 5; i++) {
      N[i] = s[i].read();
      while (abs(N[i]  - K[i]) > Speed[i]) {
        N[i] += Speed[i] * abs(K[i] - N[i]) / (K[i] - N[i]);
        s[i].write(N[i]);
      }
    }
  }
}
