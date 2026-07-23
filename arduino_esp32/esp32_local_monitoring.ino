#include <ETH.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN          2
#define PIRPIN          15
#define MQ2PIN          14

#define DHTTYPE         DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* mqtt_server   = "server_IP"; 
const int   mqtt_port     = 1883;
const char* mqtt_topic    = "infrastructure/telemetry/node_01";

const char* location_tag  = "rack_01";
const char* device_tag    = "wt32_eth01";

static bool eth_connected = false;

unsigned long lastMsgTime = 0;
const long interval       = 2000;

WiFiClient ethClient;
PubSubClient client(ethClient);

// Ethernet cable's connection
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");

      ETH.setHostname("wt32-env-node01");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Physical Cable Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH Got Valid IP Address: ");
      Serial.println(ETH.localIP());
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Physical Cable Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Process Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

// MQTT Broker reconnection Loop
void reconnect() {

  while (!client.connected()) {
    if (!eth_connected) {
      Serial.println("Cannot attempt MQTT reconnect: Ethernet link is down.");
      delay(2000);
      return;
    }

    Serial.print("Attempting connection to Mosquitto Broker...");
    
    String clientId = "WT32Node-" + String(random(0, 0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("SUCCESSFULLY CONNECTED");
    } else {
      Serial.print("FAILED, rc=");
      Serial.print(client.state());
      Serial.println(" -> Retrying connection profile in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);

  ETH.begin();

  dht.begin();
  pinMode(PIRPIN, INPUT);
  pinMode(MQ2PIN, INPUT);

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsgTime > interval) {
    lastMsgTime = now;

    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();
    int smoke_level   = analogRead(MQ2PIN);
    int motion_state  = digitalRead(PIRPIN);

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Critical Error: Failed to parse reading streams from DHT11 module.");
      return;
    }

    // for debuging
    Serial.print("[TEMP]: "); Serial.print(temperature); Serial.print("C | ");
    Serial.print("[HUM]: ");  Serial.print(humidity);    Serial.print("% | ");
    Serial.print("[GAS]: ");  Serial.print(smoke_level);  Serial.print(" | ");
    Serial.print("[PIR]: ");  Serial.println(motion_state);

    // for InfluxDB
    String payload = "environment,location=" + String(location_tag) + 
                     ",device=" + String(device_tag) + 
                     " temperature=" + String(temperature, 1) + 
                     ",humidity=" + String(humidity, 1) + 
                     ",smoke_level=" + String(smoke_level) + 
                     ",motion=" + String(motion_state);

    // publish
    if (eth_connected && client.connected()) {
      if (client.publish(mqtt_topic, payload.c_str())) {
        Serial.println("--> Frame successfully transmitted to MQTT Broker!");
      } else {
        Serial.println("--> Transmission Error: Failed to push message payload out.");
      }
    }
  }
}
