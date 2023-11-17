#define NODE "Sensor" // untuk ID client

const char *mqttServer = "iot.tf.itb.ac.id"; // Ganti dengan alamat IP atau nama domain broker MQTT
const int mqttPort = 1883;                   // Port broker MQTT

const char *clientID = NODE;

// Define Topic
#define TOPIC_BASE "Kerja-Praktik"
#define TOPIC_NODE TOPIC_BASE "/" NODE

const char Topic[32] = TOPIC_NODE;

// Variabel untuk publish
#define MQTT_STR_MAX 256

char Payload[MQTT_STR_MAX];

bool isMQTTConnected = false;

WiFiClient net;
MQTTClient client;

void mqttSetup()
{
  client.begin(mqttServer, net);
  mqttReconnect();
}

void mqttReconnect()
{
  unsigned long mqttStartTime = millis();
  client.setWill("Kerja-Praktik/Status", "offline", true, 0);
  if (!client.connect(clientID))
  {
    isMQTTConnected = false;
    Serial.println("mqtt connection failed !!!");
  }
  else
  {
    isMQTTConnected = true;
    Serial.println("\nconnected to broker!");
  }
  unsigned long mqttConnectionTime = millis() - mqttStartTime;
  Serial.print("Time taken to connect MQTT: ");
  Serial.print(mqttConnectionTime);
  Serial.println(" ms");
}

void mqttLoop()
{
  if (!client.connected())
  {
    digitalWrite(LED2, HIGH);
    Serial.println("\nConnecting...");
    if (wt_mqtt.isTime())
    {
      return mqttReconnect();
    }
  }
  client.loop();
}

void mqttPublish(String Payload, uint8_t qos)
{
  client.publish(Topic, Payload, false, qos);
}
