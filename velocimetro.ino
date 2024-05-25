#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int leitura;         
unsigned long tempo=0;
int count=0;
unsigned long rpm=0;
int lastSensorState = 0;
int sensorState = 0;
int sensorPin = 2;
unsigned long timenow=0;
unsigned long timeold=0;
int start=0;
const double circ = 3.33;

double speed = 0;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicializa a tela OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  
  pinMode(sensorPin, INPUT);
  Serial.begin(9600); //Enviar e receber dados em 9600 baud
  Serial.println("Gerando valores sequenciais");
}

void loop() {
  sensorState = digitalRead(sensorPin);

  if (sensorState != lastSensorState) {
    if (sensorState == LOW) {
      count++;
      if (count == 1 && start < 1) {
        timeold = millis();
      }
      if (count == 2) {
        timenow = millis();
        tempo = timenow - timeold;
        rpm = getRpm(tempo);
        speed = getSPeed(rpm, circ);
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
  rpm = 60000 / tempo;

  if (count == 1 && rpm < 20) {
    delay(100);
    Serial.print("RPM: ");
    Serial.println(0);
        display.clearDisplay();  
        display.setCursor(0, 0);
        display.print("RPM: ");
        display.println(0); 
        display.setCursor(0, 25);
        display.print("KM/H: ");
        display.println(0); 
        display.display();
  } 
  lastSensorState = sensorState;
}


unsigned long getRpm(unsigned long tempo){
  return 60000/tempo;
}

double getSPeed(unsigned long rpm, double circ){
  return ((rpm * circ)/60) * 3.6;
}

void printRpm(){
  display.setCursor(0, 0);
  display.print("RPM: ");
  display.println(rpm); 
}

void printKM(){
  display.setCursor(0, 25);
  display.print("KM/H: ");
  display.println(speed); 
}