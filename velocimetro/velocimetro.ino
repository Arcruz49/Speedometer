#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define o número de colunas e linhas do LCD
int lcdColumns = 16;
int lcdRows = 2;

// Define o endereço do display LCD, número de colunas e linhas
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

unsigned long tempo = 0;
int count = 0;
unsigned long rpm = 0;
int lastSensorState = 0;
int sensorState = 0;
int sensorPin = 4; 
int analogPin = 34; 
unsigned long timenow = 0;
unsigned long timeold = 0;
int start = 0;
const double circ = 166.5; 
double speed = 0;

void setup() {
  // Inicializa a comunicação I2C nos pinos GPIO 18 (SDA) e 19 (SCL)
  Wire.begin(18, 19);
  
  // Inicializa o display LCD
  lcd.init();
  // Liga a luz de fundo do LCD
  lcd.backlight();

  starting();
  defaultPrint();
  
  pinMode(sensorPin, INPUT);
  pinMode(analogPin, INPUT); 
  Serial.begin(9600); 
  Serial.println("Gerando valores sequenciais");
}

void loop() {
  sensorState = digitalRead(sensorPin);
  int analogValue = analogRead(analogPin); 

  if (sensorState != lastSensorState) {
    if (sensorState == LOW) {
      count++;
      if (count == 1 && start < 1) {
        timeold = millis();
      }
      if (count == 2) {
        timenow = millis();
        tempo = timenow - timeold;

        if (tempo > 0) {
          rpm = getRpm(tempo);
          speed = getSpeed(rpm, circ);
        } else {
          rpm = 0;
          speed = 0;
        }
        
        drawSpeedometer();
        drawRPM();

        timeold = timenow;
        count = 1;
        start = 1;
      }
    }
  }

  timenow = millis();
  tempo = timenow - timeold;
  if (tempo > 0) {
    rpm = getRpm(tempo);
  } else {
    rpm = 0;
  }

  if (count == 1 && rpm < 20) {
    delay(100);
    defaultPrint();
  } 

  lastSensorState = sensorState;
}

unsigned long getRpm(unsigned long tempo) {
  double periodMinutes = tempo / 60000.0;
  
  unsigned long rpm = 1 / periodMinutes;
  
  return rpm;
}

double getSpeed(unsigned long rpm, double circ) {
  double speedKmPerHour = (rpm * circ * 60) / 100000.0;
  return speedKmPerHour;
}

void drawRPM() {
  lcd.setCursor(0, 0);
  lcd.print("RPM: ");
  lcd.print(rpm);
}

void drawSpeedometer() {
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(speed, 1);
  lcd.print(" km/h");
}

void defaultPrint() {
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("RPM: 0");
  
  lcd.setCursor(0, 1);
  lcd.print("Speed: 0.0 km/h");
}

void starting() {
  lcd.setCursor(0, 0);
  lcd.print("starting");
  delay(3200); // Delay para simular o tempo do processo de inicialização
}
