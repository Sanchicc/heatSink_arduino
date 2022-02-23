#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
#include <Ticker.h>

Ticker ticker;
Ticker ticker2;
ESP8266WiFiMulti wifiMulti;
WiFiClient tcpClient;
PubSubClient MQTTClient(tcpClient);
bool button_state;
const char* mqtt_host = "test.ranye-iot.net";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // wifi
  wifiMulti.addAP("4B602", "h13822064292");
  Serial.print("WiFi Connecting...");
  while(wifiMulti.run() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("Success. \nSSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // mqtt
  MQTTClient.setServer(mqtt_host, 1883);
  MQTTClient.setCallback(callBackHandle);

  connectMQTT();

  // button
  pinMode(D3, INPUT_PULLUP);

  // ticker
  ticker.attach_ms(100, detect);
  ticker2.attach(5, MQTTpublish);
}

void connectMQTT()
{
  String client_id = "ESP8266-" + WiFi.macAddress();
  if(MQTTClient.connect(client_id.c_str())){
    Serial.println("Connect to MQTT server success.");
    String mqtt_topic = "ESP8266-Sub-" + WiFi.macAddress();
    MQTTClient.subscribe(mqtt_topic.c_str());
    Serial.println("Subscribe " + mqtt_topic + " success.");
  }
  else{
    Serial.println("Failed to connect MQTT server.");
    delay(5000);
    Serial.println("Try again.");
    connectMQTT();
  }
}

// handle MQTT massage
void callBackHandle(char* topic, byte* payload, long int length){
  Serial.print("Recived a massage.\nTopic: [" + String(topic) + "]: {");
  for(int i = 0; i < length; i++)
    Serial.print(char(payload[i]));
  Serial.println("}Total length: " + String(length));
}

// update flash button state
void detect(){
  button_state = digitalRead(D3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!MQTTClient.connected()){
    connectMQTT();
  }
  MQTTClient.loop();
}

void MQTTpublish(){
  String mqtt_topic = "ESP8266-Pub-" + WiFi.macAddress();

  String state;
  if(button_state)
    state = "OFF";
  else
    state = "ON";
    
  MQTTClient.publish(mqtt_topic.c_str(), state.c_str());
  Serial.println("Publish " + mqtt_topic + ":" + state);
}
