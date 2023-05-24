// ===== Read Potensio, normalisasi, LoRa, Buzzer =====

// For Arduino Uno
// Diameter Potensio = 0.5 cm
// Keliling Potensio = phi*0.5 = 1.5708 cm -> 1 Putaran
// Range data Potensio = 0 - 1023 -> 10 Putaran -> range 1 putaran = 0 - 102.3
// Konstanta Normalisasi x = 1.5708 / 102.3 = 0.01535

float k_norm = 0.01535;
float jarak_perpindahan = 0.0;
int nilai_sensor = 0;
String condition = "Normal";

// ===== LoRa =====

#include <SPI.h>
#include <LoRa.h>
int counter = 0;

// ===== Buzzer =====
const int buzzerPin = 3;
const int volumeLevel1 = 50;
const int volumeLevel2 = 150;

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT);
    while (!Serial);
    Serial.println("LoRa Sender");
    
    if (!LoRa.begin(433E6)) {
      Serial.println("Starting LoRa failed!");
      while (1);
    }

     // ===== Buzzer =====
     pinMode(buzzerPin, OUTPUT);
}
 
void loop() {
    nilai_sensor = analogRead(A0); 
    jarak_perpindahan = nilai_sensor * k_norm;
    Serial.print(jarak_perpindahan); 
    Serial.println(" cm");
    Serial.print("ADC  : ");
    Serial.println(nilai_sensor);
    

    if (jarak_perpindahan <= 5){
        condition = "Normal";
        noTone(buzzerPin);
      } else if (jarak_perpindahan >= 5 && jarak_perpindahan <= 10){
        condition = "Awas";
        playBuzzer(volumeLevel1);
      } else{
        condition = "Bahaya";
        playBuzzer(volumeLevel2);
      }

    // send packet
    Serial.print("Send : ");
    LoRa.beginPacket();
    LoRa.print(jarak_perpindahan);
    LoRa.endPacket();
    
    delay(500);
}

void playBuzzer(int volume) {
  analogWrite(buzzerPin, volume);
  delay(750); // Mainkan bunyi selama 500ms
//  noTone(buzzerPin); // Matikan bunyi
  analogWrite(buzzerPin, volume*0.5);
  delay(250); // Mainkan bunyi selama 500ms
//  noTone(buzzerPin); // Matikan bunyi
}
