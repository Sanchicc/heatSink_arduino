#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
#define FAN D1
WiFiClient client;
ESP8266WebServer server(80);
Ticker ticker;
Ticker T;
int fanStatus;
OneWire onewire(ONE_WIRE_BUS);
DallasTemperature sensors(&onewire);
String temperature;
String Condition = "自动";
double max_t = 30.0;

void setup() {
  Serial.begin(115200);
  wifi();
  ticker.attach(500, detect);

  pinMode(FAN, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(FAN, LOW);

  sensors.begin();

  if (SPIFFS.begin()) {                     // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
  }

  server.begin();
  server.on("/fan", fanControl);
  server.on("/temperature", sendTemperature);
  server.on("/Condition", sendCondition);
  server.on("/Temperature_MAX", sendTemperature_MAX);
  server.onNotFound(handleTheClient);

  // 默认自动模式
  T.attach(1000, control_temperature);
}

void loop() {
  server.handleClient();
  sensors.requestTemperatures();
  static int i = 0;
  if (i++ == 10) {
    temperature = String(sensors.getTempCByIndex(0));
    Serial.println(temperature);
    i = 0;
  }
}

// 回应ajax
void sendTemperature() {
  server.send(200, "text/plain", temperature);
}

void sendCondition() {
  server.send(200, "text/plain", Condition);
}

void sendTemperature_MAX(){
  server.send(200, "text/plain", String(max_t));
}

void detect() {
  fanStatus = digitalRead(FAN);
}

void fanControl() {
  String status = server.arg("status");
  double tmp = server.arg("max_t").toDouble();
  
  if(tmp != 0.00){
    max_t = tmp;
  }
  
  Serial.println("最大温度：" + String(max_t));
  
  if (status == "open") {
    T.detach();
    digitalWrite(FAN, 0);
    Condition = "常开";
  }
  else if (status == "close") {
    T.detach();
    digitalWrite(FAN, 1);
    Condition = "关闭";
  } else if (status == "auto") {
    T.attach(1000, control_temperature);
    Condition = "自动";
  }
  Serial.println("FAN: " + String(status));

  server.sendHeader("Location", "/");
  server.send(303);
}

void control_temperature() {
  if (temperature.toDouble() > max_t) {
    digitalWrite(FAN, 0);
    Serial.println("open");
  } else {
    digitalWrite(FAN, 1);
    Serial.println("close");
  }
}

void handleTheClient() {
  // 获取用户请求网址信息
  String webAddress = server.uri();

  // 通过handleFileRead函数处处理用户访问
  bool fileReadOK = handleFileRead(webAddress);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK) {
    server.send(404, "text/plain", "404 Not Found");
  }
}

void wifi() {
  WiFi.begin("Linksys", "succerseng");
  Serial.print("\nConnecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }

  Serial.print("Success\nIP:");
  Serial.print(WiFi.localIP());
  Serial.print("\nSSID:");
  Serial.println(WiFi.SSID());
}

bool handleFileRead(String path) {            //处理浏览器HTTP访问
  if (path.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    path = "/fan.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  }

  String contentType = getContentType(path);  // 获取文件类型

  if (SPIFFS.exists(path)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(path, "r");          // 则尝试打开该文件
    server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

// 获取文件类型
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
