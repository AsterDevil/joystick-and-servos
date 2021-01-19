#include <Servo.h>
#include <EEPROM.h>

#define Button A2
#define Mode 1
// Использовать только при первом запуске
#define FIRSTRUN

Servo Servo_X;
Servo Servo_Y;

int const potPin1 = A0; // Аналоговый вход для потенциометра координаты X
int const potPin2 = A1; // Аналоговый вход для потенциометра координаты Y
int potVal_X;           // Переменная для хранения значения аналогового входа
int potVal_Y;           // Переменная для хранения значения аналогового входа
int angle_X = 90;       // Переменная для хранения угла поворота
int angle_Y = 90;       // Переменная для хранения угла поворота
int Center_X; // Для хранения значения нейтрального положения джойстика по оси X 
int Center_Y; // Для хранения значения нейтрального положения джойстика по оси Y
int Bttn = 1; // Кнопка джойстика

void setup() {
  Servo_X.attach(9);  // Цепляем датчик сервы к пину 9
  Servo_Y.attach(10); // Цепляем датчик сервы к пину 10
  // Из-за разброса параметров джойстика для начала
  // считываем значения потенциометров в нейтральной позиции
  Center_X = analogRead(potPin1);
  Center_Y = analogRead(potPin2);
  pinMode(Button, INPUT_PULLUP);
  pinMode(LED_BUILTIN,  OUTPUT);
  #ifdef FIRSTRUN
    EEPROM.update(Mode,255); // При первом запуске в ячейку выбора режима записываем все единицы
  #endif
}

void loop() {
  potVal_X = analogRead(potPin1); // Читаем показания потенциометра X
  potVal_Y = analogRead(potPin2); // Читаем показания потенциометра Y

Bttn = digitalRead(Button);
// Устраняем дребезг контактов
if(!digitalRead(Button)){
  do{
    delay(20);
    }while(!digitalRead(Button));
  }
// Если кнопка была нажата/отпущена - меняем режим работы
if(EEPROM.read(Mode)&!Bttn){
  EEPROM.update(Mode,!EEPROM.read(Mode)); // Инвертируем байт режима в ЕЕПРОМ
  Bttn=1;
  // Индицируем режим работы встроенным светодиодом
  // Если светится - линейный режим работы, нет - пропорциональный
  if(EEPROM.read(Mode)){
    digitalWrite(LED_BUILTIN, HIGH);
    // Линейный режим работы
    // Если джойстик сместить на некоторый угол - меняем угол сервы на +-1 градус
    if(analogRead(potPin1)>=Center_X+150) angle_X++; 
    if(analogRead(potPin2)>=Center_Y+150) angle_Y++; 
    if(analogRead(potPin1)<=Center_X-150) angle_X--; 
    if(analogRead(potPin2)<=Center_Y-150) angle_Y--; 
    // При увеличении угла наклона джойстика вдвое - добавляем еще +-1 градус
    if(analogRead(potPin1)>=Center_X+300) angle_X++; 
    if(analogRead(potPin2)>=Center_Y+300) angle_Y++; 
    if(analogRead(potPin1)<=Center_X-300) angle_X--; 
    if(analogRead(potPin2)<=Center_Y-300) angle_Y--; 
    }
  else digitalWrite(LED_BUILTIN, LOW);
  // Пропорциональный режим работы
  // Переводим показаня потенцоиметров в угол поворота
  angle_X = map(potVal_X, 0, 1023, 0, 180);
  angle_Y = map(potVal_Y, 0, 1023, 0, 180);
  }
  // Задаем положение сервы
  Servo_X.write(angle_X);
  Servo_Y.write(angle_Y);
  // Ждем реакции сервы. Механика, ёпта!
  delay(15);
} 
