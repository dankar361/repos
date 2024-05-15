#include <LiquidCrystalRus.h>
LiquidCrystalRus lcd(3, 4, 8, 9, 10, 11);

//--------- Датчик температуры Dallas 1820-------------------//
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_TEMP 22
OneWire oneWire(ONE_WIRE_TEMP);
DallasTemperature sensorTemp(&oneWire);
int LED_R = 23;
int LED_Y = 24;
int LED_G = 25;//
//--------------------------------------------------//

//--------- Серводвигатель-------------------//
#include <Servo.h>
Servo motor;
//--------------------------------------------------//

//#define Laser A0
//unsigned int imTime=0; 
//unsigned int dist=0;

//--------- Ультрозвуковой датчик-------------------//
#define Trig 13
#define Echo 12
unsigned int imTime=0; 
unsigned int dist=0;
//--------------------------------------------------//

//--------- Датчик влажности-------------------//
#include <Wire.h>
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 27
/*byte degree[8] = // кодируем символ градуса
{
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
};*/
//--------------------------------------------------//

int led = 7;
int regim, noch, flag1, den, foto, temperature, val, rast, rast1, rast2, rast3, distMax, i_ugol;
//режим работы,режим сна(ожидание),флаг(переход в режим ожидания?),считывание яркости окружения?
температурная заглушка?,рандом от 0 до 6(прикол),rast=все отвечают за дистанцию вероятнее всего с разных потоков (общий раст среднее арифметическое)
float time, time1;
bool flag2;

int sig = 0;        // определение нажатий кнопки
int Counter = 0;    // определение нажатий кнопки
int lastSignal = 0; // определение нажатий кнопки


#define button 26
#define fotoresistor A1
#define termo A2
#define LCD 6
#define buzer 5

float Ultr_dist()
{
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  imTime = pulseIn(Echo, HIGH);
  dist = imTime / 58;
  return dist;
}

/*float ILS_dist()
{
  float volt = analogRead(Laser) * 5.0 / 1024.0;
  float dist = 65 * pow(volt, -1.01);
  return dist;
}*/

void setup()
{
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(LCD, OUTPUT);
  //lcd.setCursor(0, 0);  
  //lcd.print("Расстояние:"); 
  //lcd.setCursor(4, 1); 
  //lcd.print("см");

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  
  //pinMode(Laser, INPUT);
  
  Serial.begin(9600);
  randomSeed (analogRead(5));

  sensorTemp.begin();
  // устанавливаем разрешение датчика от 9 до 12 бит
  sensorTemp.setResolution(12);

  motor.attach(2);
  motor.write(0);

  //lcd.createChar(1, degree);
}

void loop()
{

  if (regim == 0) 
  {
    regsleep();
  }

  if (regim == 1) 
  {
    reghello();//просто приветствие
  }

  if (regim == 2) 
  {
    regwait();
  }
  if (regim == 3) 
  {
    regtemp();
    digitalWrite(LCD, LOW);
    lcd.clear();
    lcd.print("Режим ожидания");
  }

  if (regim == 4) 
  {
    regekran();
    digitalWrite(LCD, LOW);
    lcd.clear();
    lcd.print("Предсказание");
  }

  if (regim == 5) 
  {
    regpoisk();
    digitalWrite(LCD, LOW);
    lcd.clear();
    lcd.print("Поиск предмета");
  }
  
  if (regim == 6) 
  {
    digitalWrite(LCD, LOW);
    lcd.clear();
    lcd.print("Игра скорость");
    lcd.setCursor(0, 1);   
    lcd.print("Нажми кнопку");
    regspeed();
  }

  foto = analogRead(fotoresistor);

  if (foto < 350)
  {
    noch = 1;
  }
  delay(200);
  //regsleep();
  /*if (foto > 750)
  {
    analogWrite(led, 255);
  }
  delay(5000);
  Serial.println(foto);
  
  /*dist = Ultr_dist();
  lcd.setCursor(0, 1);   
  lcd.print(dist);
  Serial.println(dist);
  
  
  if (dist > 30)
  {
    digitalWrite(led, HIGH);
  } 
  else
  {
    digitalWrite(led, LOW);    
  }*/
  Serial.print(foto);
  Serial.print("_");
  Serial.print(noch);
  Serial.print("_");
  Serial.println(regim);
  delay(200);
}

void regsleep()//приусловии что сейчас темно(ночь) переходит в режим сна  при выходе из режима сна идет проверка флага,переход в режим ожидания или режим приветствия?
{
  if (noch == 1)
  {
    analogWrite(led, 255);
    digitalWrite(LCD, LOW);
    lcd.clear();
    lcd.print("Режим сна");
  }
  /*else
  {
    analogWrite(led, 40);
  }*/
  
  if (noch == 0)
  {
    digitalWrite(LCD, LOW);
    int chk = DHT.read(DHT11_PIN);
    lcd.clear();
    lcd.print("Режим ожидания");
    lcd.setCursor(0, 1);
    lcd.print("Влаж:  %");
    lcd.setCursor(5, 1);
    lcd.print(DHT.humidity);
    lcd.setCursor(9, 1);
    lcd.print("Т:   C");
    lcd.setCursor(11, 1);
    lcd.print(DHT.temperature);
    //delay(1);
      
  }
  flag1 = digitalRead(button);
  delay(100);
  if (flag1 == HIGH)
  {
    regim = 2;
  }
  if ((foto > 400) && (noch == 1))
  {
    regim = 1;
    noch = 0;
    analogWrite(led, 40);
  }
}

void reghello()//модуль старта включается при запуске?
{
  song();//проигрывает "мелодию" указанную ниже
  analogWrite(led, 40);
  digitalWrite(LCD, LOW);
  lcd.clear();//чистка монитора
  lcd.print("Привет!");
  lcd.setCursor(0, 1);
  lcd.print("Пробуждайся!");
  delay(3000);
  regim = 2;
}

void song()
{
  tone (buzer, 1000, 1000);
  analogWrite(buzer, LOW);
}

void regwait()//режим ожидания команды включающийся после 30 секунд простоя в режиме reghello(прерывается нажатием кнопки?)
{
  //analogWrite(led, 255);
  //digitalWrite(LCD, HIGH);
  lcd.clear();
  lcd.print("Нажми кнопку для");
  lcd.setCursor(0, 1);
  lcd.print("выбора режима");
  delay(800);
  time = millis();
  while((regim == 2) && (millis() - time <= 5000))
  {
  sig = digitalRead(button);
    if (sig != lastSignal)
    {
      if (sig == HIGH)
      {
        Counter ++;
        delay(1);  
      }
      lastSignal = sig;
    }
    if (Counter > 5)
    {
      Counter = 1;
    }
  //digitalWrite(LCD, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);  
    lcd.print("Номер режима:"); 
    lcd.setCursor(14, 0); 
    lcd.print(Counter);
    delay(200);
    Serial.print("Counter ");
    Serial.println(Counter);
  } 
    if (Counter == 1)
    {
      regim = 3;
    }
    if (Counter == 2)
    {
      regim = 4;
    }
    if (Counter == 3)
    {
      regim = 5;
    }
    if (Counter == 4)
    {
      regim = 6;
    }
    /*if (millis() - time > 15000 || Counter == 5)
    {
      regim = 0;
    }*/
  
  /*Serial.print("-------");
  Serial.println(flag2);*/
}
void regtemp ()
{
  
  float temperatureDallas; // переменная для хранения температуры  
  sensorTemp.requestTemperatures();  // отправляем запрос на измерение температуры    
  temperatureDallas = sensorTemp.getTempCByIndex(0); // считываем данные из регистра датчика
  
  //Serial.print("Temp C: "); // выводим температуру в Serial-порт
  //Serial.println(temperatureDallas);
  delay(100);
  digitalWrite(LCD, HIGH);
  lcd.clear();
  lcd.print("Температура");
  lcd.setCursor(12, 0); 
  lcd.print(temperatureDallas);
  delay(500);
  if(temperatureDallas < 20)
  {
    lcd.setCursor(0, 1);
    lcd.print("Холодно");
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_G, HIGH);
  }
  if(temperatureDallas > 20 && temperatureDallas < 27)
  {
    lcd.setCursor(0, 1);
    lcd.print("Нормально");
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_G, HIGH);
  }
  if(temperatureDallas > 27)
  {
    lcd.setCursor(0, 1);
    lcd.print("Тепло");
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_G, HIGH);
  }

  
  delay(3000);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_G, LOW);      
  Counter = 0;
  regim = 0;
}
void regekran ()
{
  lcd.setCursor(0, 0);  //установка курсора на первый символ в первой строке
  lcd.print("Оценка на зачете"); // вывод на экран надписи: Экран судьбы
  lcd.setCursor(4, 1); // установка курсора на первый символ во второй строке
  lcd.print("Поехали!"); // вывод на экран надписи: готов к работе
  val = random(0, 6);  // присваивается случайное число переменной val
  lcd.clear(); // очистка экрана
  if (val == 0) {lcd.print("Оценка - 0");lcd.setCursor(0, 1); lcd.print("Армия близко");}
  if (val == 1) {lcd.print("Оценка - 1");lcd.setCursor(0, 1); lcd.print("Учи мат. часть");}
  if (val == 2) {lcd.print("Оценка - 2");lcd.setCursor(0, 1); lcd.print("Жду через год");}
  if (val == 3) {lcd.print("Оценка - 3");lcd.setCursor(0, 1); lcd.print("Проскочил");}
  if (val == 4) {lcd.print("Оценка - 4");lcd.setCursor(0, 1); lcd.print("Что-то знаешь");}
  if (val == 5) {lcd.print("Оценка - 5");lcd.setCursor(0, 1); lcd.print("Молодец");}
  if (val == 6) {lcd.print("Оценка - 5");lcd.setCursor(0, 1); lcd.print("Молодец");}
  delay (2000);
  Counter = 0;
  regim = 0;
}

void regpoisk()//поиск расстояния до предмета перед роботом?
{
  analogWrite(led, 0);
  digitalWrite(LCD, HIGH);
  lcd.clear();
  lcd.print("Поиск");
  distMax = 5000;
  //lcd.setCursor(12, 0); 
  //lcd.print(temperatureDallas);
  //rast = Ultr_dist();
  // Serial.println(rast);
  
  for (int i = 5; i <= 180 ; i += 5 )
  {
    motor.write(i);
    delay(150);
    rast1 = Ultr_dist();
    delay(1);
    rast2 = Ultr_dist();
    delay(1);
    rast3 = Ultr_dist();
    delay(1);
    rast = ( rast1 + rast2 + rast3 ) / 3;
    Serial.println(rast);
    if ( rast < distMax)
    {
      distMax = rast;
      i_ugol = i;
    }
    lcd.setCursor(0, 1); 
    lcd.print(i * 0.55);
    lcd.setCursor(4, 1); 
    lcd.print("%");
  }
  motor.write(i_ugol);
  lcd.clear(); 
  lcd.print("Предмет найден");
  lcd.setCursor(0, 1); 
  lcd.print("Угол");
  lcd.setCursor(6, 1); 
  lcd.print(i_ugol);
   lcd.setCursor(10, 1); 
  lcd.print("Рас");
  lcd.setCursor(14, 1); 
  lcd.print(distMax);
  delay(3000);
  
  for (int i = i_ugol; i >= 0 ; i -= 5 )
  {
    motor.write(i);
    delay(150);
    rast1 = Ultr_dist();
    delay(1);
    rast2 = Ultr_dist();
    delay(1);
    rast3 = Ultr_dist();
    delay(1);
    rast = ( rast1 + rast2 + rast3 ) / 3;
    Serial.println(rast);
  }
  
  digitalWrite(2, LOW);
  Counter = 0;
  regim = 0;
}

void regspeed()//игра на провод обьектом перед датчиком определения
{
  analogWrite(led, 0);
  flag2 = LOW;
  while (flag2 == LOW)
  {
    flag2 = digitalRead(button);
  }
  for (int i = 5; i > 0; i--)
  {
    lcd.clear(); 
    lcd.print("Приготовтесь");
    lcd.setCursor(0, 1); 
    lcd.print(i);
    delay(1000);
  }
  tone (buzer, 1000, 1000);

  int j = 0;
  time1 = millis();
  lcd.clear(); 
  lcd.print("Количество очков");
  while( j < 5 )
  {
    rast1 = Ultr_dist();
    delay(1);
    rast2 = Ultr_dist();
    delay(1);
    rast3 = Ultr_dist();
    delay(1);
    rast = ( rast1 + rast2 + rast3 ) / 3;
    delay(1);
    if ( rast > 50 )
    {
      flag2 = 1;
    }
    if ( rast < 50 && flag2 == 1 )
    {
      flag2 = 0;
      j += 1;
    }
    Serial.println(j);
    lcd.setCursor(0, 1); 
    lcd.print(j);    
  }
  delay(500);
  tone (buzer, 2000, 2000);

  lcd.clear(); 
  lcd.print("Время игры");
  lcd.setCursor(0, 1); 
  int k = (millis()-time1)/1000;
  lcd.print(k);
  lcd.setCursor(5, 1); 
  lcd.print("секунд");
  delay(3000);
  
  Counter = 0;
  regim = 0;
}
