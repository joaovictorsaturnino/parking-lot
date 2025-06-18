#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoHA.h>

// ==== Definições de Hardware ====
#define TRIG_PIN1 12
#define ECHO_PIN1 13
#define TRIG_PIN2 14
#define ECHO_PIN2 27
#define TRIG_PIN3 26
#define ECHO_PIN3 25

#define VAGAS_TOTAIS 3

// ==== Configurações da rede Wi-Fi e do MQTT ====
#define WIFI_SSID        "Wokwi-GUEST"
#define WIFI_PASSWORD    ""
#define BROKER_ADDR      IPAddress(10,0,0,102)  // IP da sua VM com Home Assistant
#define BROKER_USERNAME  "usermqtt"
#define BROKER_PASSWORD  "passmqtt"

// ==== Objetos Globais ====
LiquidCrystal_I2C LCD(0x27, 16, 2);

WiFiClient client;
HADevice device("estacionamento-esp32");  // ID único
HAMqtt mqtt(client, device);
HASensor vagasLivresSensor("vagas_livres");
HASensor vagasOcupadasSensor("vagas_ocupadas");

unsigned long lastUpdate = 0;
const unsigned long publishInterval = 5000;

// ==== Setup Inicial ====
void setup() {
  Serial.begin(115200);

  // Inicialização dos pinos
  pinMode(TRIG_PIN1, OUTPUT); pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT); pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT); pinMode(ECHO_PIN3, INPUT);

  // Inicializa LCD
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Vagas livres:");

  // Conexão WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  // Configuração do dispositivo MQTT
  device.setName("Estacionamento ESP32");
  device.setManufacturer("DIY");
  device.setModel("UltrasonicParking");
  device.setSoftwareVersion("1.0.0");
  device.enableSharedAvailability();

  // Define os sensores MQTT
  vagasLivresSensor.setName("Vagas Livres");
  vagasLivresSensor.setDeviceClass("none");
  vagasLivresSensor.setIcon("mdi:car");

  vagasOcupadasSensor.setName("Vagas Ocupadas");
  vagasOcupadasSensor.setDeviceClass("none");
  vagasOcupadasSensor.setIcon("mdi:car-brake-hold");

  // Inicia conexão MQTT
  mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
}

// ==== Função para medir distância ====
long medirDistanciaCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracao = pulseIn(echoPin, HIGH);
  return duracao * 0.034 / 2;
}

// ==== Loop principal ====
void loop() {
  mqtt.loop(); // mantém a conexão MQTT

  // Lê distâncias
  int ocupadas = 0;
  if (medirDistanciaCM(TRIG_PIN1, ECHO_PIN1) < 20) ocupadas++;
  if (medirDistanciaCM(TRIG_PIN2, ECHO_PIN2) < 20) ocupadas++;
  if (medirDistanciaCM(TRIG_PIN3, ECHO_PIN3) < 20) ocupadas++;
  int livres = VAGAS_TOTAIS - ocupadas;

  // Atualiza LCD
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Vagas livres:");
  LCD.setCursor(0, 1);
  LCD.print(livres);
  delay(500);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Vagas ocupadas:");
  LCD.setCursor(0, 1);
  LCD.print(ocupadas);

  // Publica no MQTT periodicamente
  if (millis() - lastUpdate >= publishInterval) {
    lastUpdate = millis();

    vagasLivresSensor.setValue(String(livres).c_str());
    vagasOcupadasSensor.setValue(String(ocupadas).c_str());

    Serial.println("MQTT atualizado:");
    Serial.println("Vagas livres: " + String(livres));
    Serial.println("Vagas ocupadas: " + String(ocupadas));
  }
}
