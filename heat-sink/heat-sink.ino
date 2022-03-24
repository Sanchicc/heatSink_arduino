#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DFRobot_DHT11.h>
#include <Math.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN D5
#define ONE_WIRE_BUS D4
#define FAN D1
WiFiClient client;
ESP8266WebServer server(80);
Ticker ticker;
OneWire onewire(ONE_WIRE_BUS);
DallasTemperature sensors(&onewire);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);

// 封装各种参数
struct
{
  float max_t = 40.0f;
  int status;
  String temperature;
  String model;
} msg;

void setup()
{
  Serial.begin(115200);
  wifi();

  pinMode(FAN, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(FAN, 1);

  // 默认自动模式
  ticker.attach(1, control_temperature, false);
  msg.model = "自动";

  sensors.begin();
  timeClient.begin();
  DHT.read(DHT11_PIN);
  sensors.requestTemperatures();

  if (SPIFFS.begin())
  { // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  }
  else
  {
    Serial.println("SPIFFS Failed to Start.");
  }

  server.begin();
  server.on("/submit", Receive);
  server.on("/json", sendJsonData);
  server.onNotFound(handleTheClient);
}

void loop()
{
  server.handleClient();
    
  msg.temperature = String(max(sensors.getTempCByIndex(0), (float)DHT.temperature));
  
  // 轮流更新数据
  static long i = 0;

  if (i == 1)
  {
    DHT.read(DHT11_PIN);
  }
  else if (i == 1000)
  {
    sensors.requestTemperatures();
  }
  else if (i == 2000)
  {
    timeClient.update();
  }

  if (i == 10 || i == 1000)
  {
    Serial.print("时间：" + String(timeClient.getFormattedTime()) + "温度更新:" + String(msg.temperature));
    Serial.println("湿度更新:" + String(DHT.humidity));
  }

  i += 1;
  if (i > 100000)
    i = 0;
}

// 回应ajax
void sendJsonData()
{
  server.send(200, "text/plain", String(getJsonData()));
}

//自动模式
void control_temperature(bool flag)
{
  static int time_flag = 0;
  time_flag++;

  if (msg.temperature.toFloat() > msg.max_t)
  {
    digitalWrite(FAN, 0);
    time_flag = 0;
  }
  else
  {
    if (digitalRead(FAN))
      // 控制台的调用那么无视三十秒
      if (time_flag >= 15 || flag == true)
      { // 保持30秒再关
        time_flag = 0;
        digitalWrite(FAN, 1);
      }
  }
}

void Receive()
{
  String status = server.arg("status");
  double tmp = server.arg("max_t").toFloat();

  if (tmp != 0.00)
  {
    msg.max_t = tmp;
  }

  Serial.println("最大温度：" + String(msg.max_t));

  if (status == "open")
  {
    ticker.detach();
    digitalWrite(FAN, 0);
    msg.model = "常开";
  }
  else if (status == "close")
  {
    ticker.detach();
    digitalWrite(FAN, 1);
    msg.model = "关闭";
  }
  else if (status == "auto")
  {
    // 如果刚才是开着的先关掉
    if (msg.model == "常开")
      digitalWrite(FAN, 1);
    ticker.detach();
    ticker.attach(1, control_temperature, false);
    msg.model = "自动";
  }
  Serial.println("模式: " + String(status));
  msg.status = digitalRead(FAN);

  control_temperature(true);
  
  server.send(200);
}

void handleTheClient()
{
  // 获取用户请求网址信息
  String webAddress = server.uri();

  // 通过handleFileRead函数处处理用户访问
  bool fileReadOK = handleFileRead(webAddress);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK)
  {
    server.send(404, "text/plain", "404 Not Found");
  }
}

void wifi()
{
  WiFi.begin("Linksys", "succerseng");
  Serial.print("\nConnecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print('.');
  }

  Serial.print("Success\nIP:");
  Serial.print(WiFi.localIP());
  Serial.print("\nSSID:");
  Serial.println(WiFi.SSID());
}

bool handleFileRead(String path)
{ //处理浏览器HTTP访问
  if (path.endsWith("/"))
  {                       // 如果访问地址以"/"为结尾
    path = "/index.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
  }

  String contentType = getContentType(path); // 获取文件类型

  if (SPIFFS.exists(path))
  {                                       // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(path, "r");   // 则尝试打开该文件
    server.streamFile(file, contentType); // 并且将该文件返回给浏览器
    file.close();                         // 并且关闭文件
    return true;                          // 返回true
  }
  return false; // 如果文件未找到，则返回false
}

/*
var data = {
    temperature: '0',
    model: '0',
    max_t: '0',
    status: '0',
    Humidity: '0'
};
*/
String getJsonData()
{
  String json = "{";
  json += "\"temperature\":\"" + String(msg.temperature) + "\",";
  json += "\"model\":\"" + String(msg.model) + "\",";
  json += "\"max_t\":\"" + String(msg.max_t) + "\",";
  json += "\"status\":\"" + String(digitalRead(FAN) ? "关闭" : "开启") + "\",";
  json += "\"Humidity\":\"" + String(DHT.humidity) + "\"";
  json += "}";
  return json;
}

// 获取文件类型
String getContentType(String filename)
{
  if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}
