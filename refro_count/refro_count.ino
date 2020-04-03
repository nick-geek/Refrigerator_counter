//Добавить - сигнализацию о открытой двери более 1 минуты

#include <avr/eeprom.h>

int data_pin  = 9;   //Пин подключен к DS входу 74HC595
int latch_pin = 10;  //Пин подключен к ST_CP входу 74HC595
int clock_pin = 11;  //Пин подключен к SH_CP входу 74HC595

//Пины разрядов цифр
int pins_digits[4] = {2, 3, 4, 5};
//Массив для хранения выводимых чисел
int digits[4];
//Возможные цифры
byte numbers_array[10]= {
  0b11111101,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110
  };
//Читаем последнее число включений из EEPROM
int number_openings = eeprom_read_byte(0);

void setup() {
  //Настройка выходов на транзисторы
  for(int i; i<4; i++){
    pinMode(pins_digits[i], OUTPUT);
  }
  
  //Настройка 74HC595
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  //Сборс счётчика
  pinMode(13, INPUT);
  if (digitalRead(13)){number_openings = 0;}

  ++number_openings;
  eeprom_write_byte (0, number_openings);

}
 
void loop() {
    show(number_openings);
}

void showNumber(int digit_number, int sign){
  //Зажигаем нужные сегменты 
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, numbers_array[sign]); 
  digitalWrite(latch_pin, HIGH);
  
  //Включаем нужный разряд(одну из четырех цифр)
  int num_digit = pins_digits[digit_number-1];
  
  for(int i; i<4; i++){
    //Выключаем все
    digitalWrite(pins_digits[i], LOW);
  }
  
  //Включаем нужный разряд
  digitalWrite(num_digit, HIGH);
     
  delay(5);
}

void show(int sign)
{ 
  //Преобразуем число в массив для вывода
  digits[0] = sign/1000;    //Тысячи
  digits[1] = sign/100%10;  //Сотки
  digits[2] = sign/10%10;   //Десятки
  digits[3] = sign%10;      //Единицы

  if (sign >= 1000){showNumber(4, digits[0]);}
  if (sign >= 100) {showNumber(3, digits[1]);}
  if (sign >= 10)  {showNumber(2, digits[2]);}
  showNumber(1, digits[3]);
  
}
