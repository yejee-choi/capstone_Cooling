#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial bluetooth(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int VOLT_PIN = A0;
float R1 = 30000.0;
float R2 = 7500.0;

const int FAN_1 = 9;
const int FAN_2 = 10;
const int MOS = 12;

bool autoMode = false;

const int NUM_SAMPLES = 5;
int filteredTem = 0;
float alpha = 0.2;

void setup() {
    Serial.begin(9600);
    bluetooth.begin(9600);
    sensors.begin();

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("System Init...");

    pinMode(MOS, OUTPUT);
    pinMode(FAN_1, OUTPUT);
    pinMode(FAN_2, OUTPUT);
    pinMode(VOLT_PIN, INPUT);
    delay(1000);
    lcd.clear();
}

void loop() {
    int sensorValue = analogRead(VOLT_PIN);
    float vout = (sensorValue * 5.0) / 1024.0; 
    float vin = vout / (R2 / (R1 + R2));

    int batteryPercent = (int)((vin / 10.0) * 100);
    if (batteryPercent > 100) batteryPercent = 100;
    if (batteryPercent < 0) batteryPercent = 0;


    sensors.requestTemperatures();
    int tem = sensors.getTempCByIndex(0);

    if (bluetooth.available()) {
        char val = bluetooth.read();

    if (val == '1') {
        autoMode = false;
        digitalWrite(FAN_1, HIGH);
        digitalWrite(MOS, HIGH);
        }
    else if (val == '0') {
        autoMode = false;
        digitalWrite(FAN_1, LOW);
        digitalWrite(MOS, LOW);
    }
    else if (val == 'A') {
        autoMode = true;
    }
  }

    if (autoMode) {
        if (tem >= 28) {
            analogWrite(FAN_1, 255);
            
            digitalWrite(MOS, HIGH);
        }
        else if(tem >= 26){
            analogWrite(FAN_1, 100);
            
            digitalWrite(MOS, HIGH);
        }
    else {
        digitalWrite(FAN_1, LOW);

        digitalWrite(MOS, LOW);
    }
  }

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tem);
    lcd.print((char)223); // 도씨 기호
    lcd.print("C     ");

    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(vin, 1); // 소수점 1자리
    lcd.print("v  ");
    lcd.print(batteryPercent);
    lcd.print("%   ");

    Serial.print("Temp: ");
    Serial.print(tem);
    Serial.print(" C | Mode: ");
    Serial.println(autoMode ? "AUTO" : "MANUAL");

    bluetooth.print(tem);
    bluetooth.print(",");
    bluetooth.println(batteryPercent);
    delay(500);
}