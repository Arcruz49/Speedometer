#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //add I2C library 


Servo meuServo;  // Crie um objeto Servo
LiquidCrystal_I2C lcd(0x27,16,2); 

int posInicial = 0;    // Posição inicial do servo
int posFinal = 180;    // Posição final do servo
int passo = 1;         // O quanto o servo se move a cada passo
int delayEntrePassos = 10;  // Tempo de espera entre cada passo (em milissegundos)
double velocMaxima = 50;
double velocidade = 10;

double transformaVelocidade(int velocidade){
  return abs((velocidade - 50)*3.6);
}

void setup() {
  meuServo.attach(9);  // Conecte o servo ao pino 9
  lcd.init(); //Initialize the LCD 
  lcd.backlight(); //Set LCD background bright 
}

void loop() {
  meuServo.write(transformaVelocidade(velocidade));
  lcd.setCursor(0,0);
  lcd.print((int)velocidade);
  lcd.print("Km/h");

}