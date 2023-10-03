#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //add I2C library 

const int sensorPin = 3;  // Conecte o pino D0 do sensor Hall ao pino digital 3
Servo meuServo;  // Crie um objeto Servo
LiquidCrystal_I2C lcd(0x27,16,2); 

double velocMaxima = 50;
double velocidade = 0;
unsigned long startTime = 0;  // Variável para armazenar o tempo de início
unsigned long elapsedTime = 1;

double transformaVelocidade(int velocidade){
  return abs((velocidade - 50)*3.6);
}

void setup() {
  meuServo.attach(9);  // Conecte o servo ao pino 9
  lcd.init(); //Initialize the LCD 
  lcd.backlight(); //Set LCD background bright 
  Serial.begin(9600);  // Inicia a comunicação serial
  pinMode(sensorPin, INPUT);  // Configura o pino do sensor como entrada
}

void loop() {
  bool sensorValue = !(digitalRead(sensorPin));  // Lê o valor do sensor Hall
  if(sensorValue){
    delay(50);
    sensorValue = !(digitalRead(sensorPin));
    if(!sensorValue){
      Serial.println("CUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCUCU");
      elapsedTime = millis();
      velocidade = (1.63 / ((elapsedTime - startTime)/1000.00)*3.6);

      startTime = millis(); 

    }

  }
  lcd.setCursor(0,0);
  lcd.print((int)(velocidade));
  lcd.print("Km/h");
  Serial.println(velocidade * 3.6);
  Serial.println("Km/h");
  meuServo.write(transformaVelocidade(velocidade));
}
