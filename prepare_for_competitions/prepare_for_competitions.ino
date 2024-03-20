#include <Servo.h>                                          //Подключение библиотеки для работы с сервоприводами
String mes = "";                                            //Пременная для приёма значений с Serial порта
Servo s[5];
int Spins[5] = {3, 5, 6, 9, 10};                            //Пины сервоприводов
int Ppins[5] = {A0, A1, A2, A3, A4};                        //Пины потенциометров
float L[5] = {180, 3, 0, 172, 168};                         //Ладонь
float K[5] = {0, 180, 180, 80, 0};                          //Кулак
float OK [5] = {180, 3, 120, 109, 86};                      //Окей
float V [5] = {0, 3, 2, 63, 72};                            //Виктори
float F [5] = {0, 180, 0, 130, 150};                        //Первый
float Speed [5] = {0.9, 0.9, 0.9, 0.02, 0.4};               //Скорость
float Now[5] = {180, 3, 0, 172, 168};                       //Текущий угол сервопривода
float Need[5];                                              //Необходимый угол сервопривода
int Vector[5];                                              //Вектор движения сервы
bool but [2] = {0, 0};                                      //Массив хранящий текущее и прошлое значение кнопки


void corner() {                                             //Функция изменения углов от текущих до необходимых с использованием скоростей и векторов
  for (int i = 0; i < 5; i ++) {                            //Запоминание текущего угла сервопривода
    Now[i] = s[i].read();
  }
  for (int i = 0; i < 5; i ++) {                            //Изменение углов от текущих до необходимых с использованием скоростей векторов
    Vector[i] = abs(Need[i] - Now[i]) / (Need[i] - Now[i]); //Определение вектора движения
    while (abs(Need[i] - Now[i]) > Speed[i]) {              //Цикл изменяющий угол сервопривода до тех пор, пока разница не будет меньше скорости
      Now[i] += Speed[i] * Vector[i];                       //Изменение текущего угло на величину скорости в соответсвующем направлении
      s[i].write(Now[i]);                                   //Обновление угла сервопривода после вычислений
    }
  }
}


void Ser() {
  if (mes.lastIndexOf("P", 0) != -1) {                                         //Включение ручного режима управления
    s[0].write(map(analogRead(A0), 0, 1023, 0, 180));
    s[1].write(map(analogRead(A1), 0, 1023, 3, 180));
    s[2].write(map(analogRead(A2), 0, 1023, 0, 180));
    s[3].write(map(analogRead(A3), 0, 1023, 80, 172));
    s[4].write(map(analogRead(A4), 0, 1023, 0, 168));
    Serial.print(map(analogRead(A0), 0, 1023, 0, 180));
    Serial.print(", ");
    Serial.print(map(analogRead(A1), 0, 1023, 0, 180));
    Serial.print(", ");
    Serial.print(map(analogRead(A2), 0, 1023, 0, 180));
    Serial.print(", ");
    Serial.print(map(analogRead(A3), 0, 1023, 0, 180));
    Serial.print(", ");
    Serial.println(map(analogRead(A4), 0, 1023, 0, 180));
  }
  else if (mes.lastIndexOf("OK", 0) != -1) {                                  //Выполнение жеста окей
    for (int i = 0; i < 5; i ++) {
      Need[i] = OK[i];
    }
    Need[5] = OK[5];
    corner();
  }
  else if (mes.lastIndexOf("F", 0) != -1) {                                   //Выполнение жеста превый
    for (int i = 0; i < 5; i ++) {
      Need[i] = F[i];
    }
    corner();
  }
  else if (mes.lastIndexOf("K", 0) != -1) {                                  //Выполнение жеста кулак
    for (int i = 0; i < 5; i ++) {
      Need[i] = K[i];
    }
    corner();
  }
  else if (mes.lastIndexOf("V", 0) != -1) {                                   //Выполнение жеста виктори
    for (int i = 0; i < 5; i ++) {
      Need[i] = V[i];
    }
    corner();
  }
  else if (mes.lastIndexOf("L", 0) != -1) {                                   //Выполнение жеста ладонь
    for (int i = 0; i < 5; i ++) {
      Need[i] = L[i];
    }
    corner();
  }
}


void But() {
  but[1] = but[0];
  but[0] = not(digitalRead(11));
  if (but[0] != but[1]) {
    Serial.println('B');
    mes = ' '; 
    for (int i = 0; i < 5; i ++) {
      Need[i] = L[i];
    }
    corner();
  }
}


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i ++) {
    s[i].attach(Spins[i]);                                  //Назначение сервоприводам соответсвующих пинов
    pinMode(Ppins[i], INPUT);                               //Работа потенциометров в режиме приёма
  }
  for (int i = 0; i < 5; i ++) {
    s[i].write(L[i]);
  }
  pinMode(11, INPUT_PULLUP);
}


void loop() {
  if (Serial.available()) {                                 //Проверка сообщений в Serial порте
    mes = Serial.readString();                              //Считывание сообщения при условие, что оно пришло
  }
  But();
  Ser();
}
