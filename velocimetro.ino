#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); 
  }
  
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  starting();
  defaultPrint();
  
  pinMode(sensorPin, INPUT);
  pinMode(analogPin, INPUT); 
  Serial.begin(9600); 
  Serial.println("Gerando valores sequenciais");
}

void loop() {
  display.clearDisplay(); 
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
        
        display.clearDisplay();  
        
        printRpm();
        printKM();

        display.display(); 

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
  //Velocidade do veículo (km/h) = (RPM das rodas × diâmetro do pneu (cm) × π × 60) / 10^5
  //double speedKmPerHour = (rpm * circ * 60) / 100000;
  double speedKmPerHour = rpm / 10.0;

  return speedKmPerHour;
}

void printRpm() {
  display.setCursor(0, 0);
  display.print("RPM: ");
  display.println(rpm); 
}

void printKM() {
  display.setCursor(0, 25);
  display.print("KM/H: ");
  char speedStr[6]; 
  dtostrf(speed, 4, 1, speedStr); 
  display.print(speedStr); 
  display.print(" "); 
}

void defaultPrint() {
  display.clearDisplay();  
  display.setCursor(0, 0);
  display.print("RPM: ");
  display.println(0); 
  display.setCursor(0, 25);
  display.print("KM/H: ");
  display.println(0); 
  display.display();
}

void starting() {
  String txt = "starting";
  display.print(txt);
  for (int i = 0; i <= 3; i++) {
    display.setCursor(0, 0);
    display.print(txt);      
    display.display();       
    delay(800);              
    txt = txt + ".";         
    display.clearDisplay();
    display.display();
  }
}
