#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient wificlient; // TCP
PubSubClient client(wificlient); // MQTT client

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // connect to wifi and print IP
  WiFi.mode(WIFI_STA);
  Serial.print("WIFI Connecting...");
  WiFi.begin("4B602","h13822064292");
  while(WiFi.status() != 3)
  {
    Serial.print('.');
    delay(500);//一定要延时不然会炸
  }
  Serial.print("Connected success. \nIP: ");
  Serial.println(WiFi.localIP());

  const char* host = "test.ranye-iot.net";
  client.setServer(host, 1883);
  client.setCallback(callback);

  pinMode(BUILTIN_LED, OUTPUT);
}

void callback(char* topic, byte* payload, unsigned int length)
{
  if((char)payload[0] == '1')
    digitalWrite(BUILTIN_LED, HIGH);
  if((char)payload[0] == '0')
    digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  while(!client.connected())
  {
    const char *client_id = "ESP8266-succerseng";
    while(!client.connect(client_id))
      delay(5000);
    const char *topic = "home/succerseng/led";
    client.subscribe(topic);
  }
  client.loop();

  // send every 2 seconds
  static long lastMsg = 0;
  if(millis() - lastMsg > 2000){
    lastMsg = millis();
    if(digitalRead(BUILTIN_LED))
      client.publish("home/succerseng/led", "state: ON");
    else
      client.publish("home/succerseng/led", "state: OFF");
  }
}
