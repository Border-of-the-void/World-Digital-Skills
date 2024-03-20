#define button 4
int led [3] = {0, 1, 2};              // Пины светодиодов: красный, жёлтый, зелёный
long int Time [2] = {0, 0};           // Время: входа в цикл, нажатия кнопки
bool but [2] = {0, 0};                // Состояние кнопки: ново, старое
bool work = true;                     // Состояние аварийного режима
bool ryg [3] = {0, 0, 0};             // Сигнал светодиодов: красный, жёлтый, зелёный

void checkButon() {
  but[1] = but[0];
  but[0] = !digitalRead(button);
  if (but[0] and but[0] != but[1]) {
    Time[1] = millis() + 3000;
    ryg[2] = 1;
  }
  if (not(but[0]) and but[0] != but[1]) {
    ryg[2] = 0;
  }
}

void crash() {
  if (but[0] and Time[0] > Time[1] and work) {   // Включение Аварийного режима
    work = not(work);
    ryg[1] = not(ryg[1]);
    ryg[2] = 0;
    
  }
  if (digitalRead(button)) {
    work = true;
  }
  if (ryg[1] == 0) {
    digitalWrite(led[2], ryg[2]);
  }
  digitalWrite(led[1], ryg[1]);
}

void act() {
  digitalWrite(led[2], 1);
}

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(led[0], OUTPUT);
  pinMode(led[1], OUTPUT);
  pinMode(led[2], OUTPUT);
}

void loop() {
  Time[0] = millis();                            // Время входа в цикл
  checkButon();                                  // Проверка состояния кнопки
  crash();                                       // Включение аварийного режима
  if (work){
    act ();                                      // Работа в штатном режиме
  }
}
