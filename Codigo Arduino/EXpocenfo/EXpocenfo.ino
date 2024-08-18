#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PulseSensorPlayground.h>
#include <FastLED.h>
#include <Wire.h>
#include <LCD-I2C.h>
#include <stdlib.h>
#include "EasyMFRC522.h"

#define NUM_LEDS 1
#define DATA_PIN 2
#define LED_TYPE WS2812B
#define MAX_STRING_SIZE 100
#define BLOCK 1
CRGB leds[NUM_LEDS];

const char* ssid = "PlayStation";
const char* password = "55225517";
const char* serverUrl = "https://cardiology-assistant-v2.vercel.app/api/assistant-pulse-rate";
const int PulseWire = 33;
int Threshold = 550;
LCD_I2C lcd(0x27, 16, 2);
PulseSensorPlayground pulseSensor;
EasyMFRC522 rfidReader(4, 5);

void connectToWiFi() {
  lcd.clear();
  lcd.print("Conectando");
  lcd.setCursor(0, 1);
  lcd.print("a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2500);
    lcd.clear();
    lcd.print("Conectando");
    lcd.setCursor(0, 1);
    lcd.print("a WiFi...");
    delay(5000);
    lcd.clear();
  }
  lcd.clear();
  lcd.print("Conectado!!");
  delay(5000);
  lcd.clear();
}

void waitForRFID() {
  bool success = false;
  lcd.print("Acerca");
  lcd.setCursor(0, 1);
  lcd.print("la tarjeta");
  Serial.println("Esperando tarjeta...");
  while (!success) {
    success = rfidReader.detectTag();
    if (success) {
      Serial.println("Tarjeta detectada!");
    } else {
      Serial.println("No se detecta tarjeta.");
    }
    delay(500);
  }
  lcd.clear();
  lcd.print("Tarjeta");
  lcd.setCursor(0, 1);
  lcd.print("detectada");
  delay(2000);
  lcd.clear();
}

void setup() {
  lcd.begin(); // Inicializa la comunicación con la pantalla LCD
  lcd.display(); // Enciende la pantalla LCD
  lcd.backlight();
  Serial.begin(115200);

  // Initialize RFID reader
  rfidReader.init();

  // Initialize LEDs
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Black; // Ensure the LED is off initially
  FastLED.show();

  connectToWiFi();

  // Configura el objeto PulseSensor, asignando nuestras variables a él
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    lcd.print("Bienvenido");
    delay(5000);
    lcd.clear();
  }
}


void loop() {
  waitForRFID();
  
  lcd.print("Pon el dedo");
  lcd.setCursor(0, 1);
  lcd.print("en el sensor");
  delay(5000);

  // Código principal a ejecutar después de la configuración del sensor y conexión a WiFi
  int myBPM = pulseSensor.getBeatsPerMinute();
  while (myBPM <= 0) {
    delay(2500);
    myBPM = pulseSensor.getBeatsPerMinute();
  }
  lcd.clear();
  lcd.print("Pulso cardiaco");
  lcd.setCursor(0, 1);
  lcd.print(myBPM);
  delay(5000);
  lcd.clear();

  // Crear el objeto JSON
  StaticJsonDocument<200> jsonDocument;
  jsonDocument["value"] = myBPM;

  // Convertir el JSON a String
  String jsonString;
  serializeJson(jsonDocument, jsonString);

  // Realizar la solicitud HTTP POST
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonString);

  lcd.print("Espere");
  lcd.setCursor(0, 1);
  lcd.print("Por favor");
  delay(10000);
  lcd.clear();

  if (httpResponseCode > 0) {
    if (httpResponseCode == 200) {
      lcd.print("Ya tienes");
      lcd.setCursor(0, 1);
      lcd.print("Planes");
      leds[0] = CRGB(0, 0, 255);  // Azul
      FastLED.show();
      delay(3000);  // Espera 1 segundo
       lcd.clear();
      leds[0] = CRGB(0, 0, 0);  // Apaga el LED
      FastLED.show();
    } else if (httpResponseCode == 202) {
      lcd.print("Todo en orden");
      delay(5000);
      lcd.clear();
      leds[0] = CRGB(255, 0, 0);  // Verde
      FastLED.show();
      leds[0] = CRGB(0, 0, 0);  // Apaga el LED
      FastLED.show();
    } else if (httpResponseCode == 500) {
      lcd.print("Error de server");
      leds[0] = CRGB(0, 255, 0);  // Rojo
      FastLED.show();
      delay(3000);
      lcd.clear();
      leds[0] = CRGB(0, 0, 0);  // Apaga el LED
      FastLED.show();
    }
  } else {
    if (httpResponseCode == -11) {
      lcd.print("Se han creado");
      lcd.setCursor(0, 1);
      lcd.print("Planes");
      delay(5000);
      lcd.clear();
      leds[0] = CRGB(128, 255, 0);  // Naranja
      FastLED.show();
      delay(3000);
      leds[0] = CRGB(0, 0, 0);  // Apaga el LED
      FastLED.show();
    } else {
      lcd.print("Error!!");
      leds[0] = CRGB(0, 255, 0);  // Rojo
      FastLED.show();
      delay(3000);
      leds[0] = CRGB(0, 0, 0);  // Apaga el LED
      FastLED.show();
      lcd.clear();
    }
  }
  http.end();
  lcd.print("Espere");
  lcd.setCursor(0, 1);
  lcd.print("Por favor");

  delay(30000); // Esperar un tiempo antes de repetir el proceso
  lcd.clear();
}
