#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DFRobot_DHT11.h>
#include <Math.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN D5

#define ONE_WIRE_BUS D4
#define FAN D1
WiFiClient client;
ESP8266WebServer server(80);
Ticker ticker;
Ticker ticker2;
int fanStatus;
OneWire onewire(ONE_WIRE_BUS);
DallasTemperature sensors(&onewire);
String temperature;
String Model = "自动";
double max_t = 40.0;

void setup()
{
  Serial.begin(115200);
  wifi();

  pinMode(FAN, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(FAN, 1);

  ticker.attach_ms(500, detect);

  // 默认自动模式
  ticker2.attach(1, control_temperature);

  sensors.begin();

  if (SPIFFS.begin())
  { // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  }
  else
  {
    Serial.println("SPIFFS Failed to Start.");
  }

  server.begin();
  server.on("/fan", fanControl);
  server.on("/fanStatus", sendfanStatus);
  server.on("/temperature", sendTemperature);
  server.on("/Humidity", sendHumidity);
  server.on("/Model", sendModel);
  server.on("/Temperature_MAX", sendTemperature_MAX);
  server.onNotFound(handleTheClient);
}

void loop()
{
  server.handleClient();

  static int i = 0;
  i++;
  if (i == 100)
  {
    DHT.read(DHT11_PIN);
    sensors.requestTemperatures();

    temperature = String(max(sensors.getTempCByIndex(0), (float)DHT.temperature));

    Serial.print("temp:");
    Serial.println(temperature);
    Serial.print("humi:");
    Serial.println(DHT.humidity);
    i = 0;
  }
}

/* 
    回应ajax
 */

// 发送温度
void sendTemperature()
{
  server.send(200, "text/plain", String(temperature));
}

// 发送模式
void sendModel()
{
  server.send(200, "text/plain", Model);
}

// 发送湿度
void sendHumidity()
{
  server.send(200, "text/plain", String(DHT.humidity));
}

void sendTemperature_MAX()
{
  server.send(200, "text/plain", String(max_t));
}

void sendfanStatus()
{
  server.send(200, "text/plain", digitalRead(FAN) ? "关闭" : "开启");
}

void detect()
{
  fanStatus = digitalRead(FAN);
}

void fanControl()
{
  String status = server.arg("status");
  double tmp = server.arg("max_t").toDouble();

  if (tmp != 0.00)
  {
    max_t = tmp;
  }

  Serial.println("最大温度：" + String(max_t));

  if (status == "open")
  {
    ticker2.detach();
    digitalWrite(FAN, 0);
    Model = "常开";
  }
  else if (status == "close")
  {
    ticker2.detach();
    digitalWrite(FAN, 1);
    Model = "关闭";
  }
  else if (status == "auto")
  {
    ticker2.detach();
    ticker2.attach(1, control_temperature);
    Model = "自动";
  }
  Serial.println("FAN: " + String(status));

  server.sendHeader("Location", "/");
  server.send(303);
}

void control_temperature()
{
  static int time_flag = 0;
  time_flag++;

  if (temperature.toDouble() > max_t)
  {
    digitalWrite(FAN, 0);
    time_flag = 0;
  }
  else
  {
    if (digitalRead(FAN))
      if (time_flag >= 30)
      { // 保持30秒再关
        time_flag = 0;
        digitalWrite(FAN, 1);
      }
  }
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
  {                     // 如果访问地址以"/"为结尾
    path = "/fan.html"; // 则将访问地址修改为/index.html便于SPIFFS访问
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
