
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT11
#define MQ2PIN 14  
#define PIRPIN 15         

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(MQ2PIN, INPUT);
  pinMode(PIRPIN, INPUT);
}

void loop() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  int mq2Value = analogRead(MQ2PIN);
  bool gasDetected = mq2Value > 512; 

  bool motionDetected = digitalRead(PIRPIN);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Gas Detected: ");
  Serial.print(gasDetected ? "Yes" : "No");
  Serial.print(", Motion Detected: ");
  Serial.println(motionDetected ? "Yes" : "No");

  delay(2000);
}
