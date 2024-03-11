#define BLYNK_TEMPLATE_ID   "user13"
#define BLYNK_TEMPLATE_NAME "user13@server.wyns.it"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHT_PIN 5 
#define LED_PIN 19
#define POTENTIOMETER_PIN 34
#define BUTTON_PIN 35 

char auth[] = "4xC5Q4NGsTf9zoVqNmk8nzS7KLM1P9bp";
char ssid[] = "embed";
char pass[] = "weareincontrol";

DHT dht(DHT_PIN, DHT11);

float gewensteTemperatuur = 0; 
float gemetenTemperatuur = 0;
bool verwarmingsketelStatus = false;
bool gebruikPotentiometer = true; 

BLYNK_WRITE(V3) {
  if (!gebruikPotentiometer) {
    gewensteTemperatuur = param.asFloat();
  }
}

BLYNK_WRITE(V6) { 
  gebruikPotentiometer = !gebruikPotentiometer; 
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.begin();
}

void loop() {
  Blynk.run();

  gemetenTemperatuur = dht.readTemperature();

  
  if (gebruikPotentiometer) {
    int potValue = analogRead(POTENTIOMETER_PIN);
    gewensteTemperatuur = map(potValue, 0, 4096, 0, 30);
  }

  
  if (gemetenTemperatuur < gewensteTemperatuur) {
    digitalWrite(LED_PIN, HIGH);
    verwarmingsketelStatus = true;
  } else {
    digitalWrite(LED_PIN, LOW);
    verwarmingsketelStatus = false;
  }

  
  Serial.print("Gemeten temperatuur: ");
  Serial.print(gemetenTemperatuur);
  Serial.print(" °C, Gewenste temperatuur: ");
  Serial.print(gewensteTemperatuur);
  Serial.print(" °C, Verwarmingsketel: ");
  Serial.println(verwarmingsketelStatus ? "Aan" : "Uit");

  
  Blynk.virtualWrite(V2, gemetenTemperatuur);
  Blynk.virtualWrite(V3, gewensteTemperatuur);
  Blynk.virtualWrite(V4, verwarmingsketelStatus ? "Aan" : "Uit");
}
