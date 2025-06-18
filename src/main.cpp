#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     0
#define UTC_OFFSET_DST 0

const int trigPin1 = 12;
const int echoPin1 = 13;
const int trigPin2 = 14;
const int echoPin2 = 27;
const int trigPin3 = 26;
const int echoPin3 = 25;

const int vagasLivres = 3;
int vagasOcupadas = 0;

// Função atualizada: recebe TRIG e ECHO
long distanciaSensoresCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duracao = pulseIn(echoPin, HIGH);
  return duracao * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  // Configura pinos dos sensores
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  // Inicializa LCD
  LCD.init();
  LCD.backlight();
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Iniciando...");
  delay(2000);
}

void loop() {
  vagasOcupadas = 0;

  long dist1 = distanciaSensoresCm(trigPin1, echoPin1);
  long dist2 = distanciaSensoresCm(trigPin2, echoPin2);
  long dist3 = distanciaSensoresCm(trigPin3, echoPin3);

  if (dist1 < 20) vagasOcupadas++;
  if (dist2 < 20) vagasOcupadas++;
  if (dist3 < 20) vagasOcupadas++;

  int vagas = vagasLivres - vagasOcupadas;

  // Exibe no LCD
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Vagas livres:");
  LCD.setCursor(0, 1);
  LCD.print(vagas);
  delay(3000);

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Vagas ocupadas:");
  LCD.setCursor(0, 1);
  LCD.print(vagasOcupadas);
  delay(3000);
}
