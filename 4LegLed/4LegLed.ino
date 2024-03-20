volatile int Flag = 0;
int pt = 0;
int pin[3] = {3, 5, 6};
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, but, FALLING);
}


void but(){
  Flag = (Flag+1)%3;
}
void loop() {
 pt = map(analogRead(A0), 0, 1023, 0, 255);
 analogWrite(3, 0);
 analogWrite(5, 0);
 analogWrite(6, 0);
 analogWrite(pin[Flag], pt);
 if (Flag == 0){
  Serial.print("R ");
 }
 if (Flag == 1){
  Serial.print("G ");
 }
 if (Flag == 2){
  Serial.print("B ");
 }
 Serial.println(pt);
}
