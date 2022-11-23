/* 
โค้ดต้นฉบับจาก Example > ESP8266WebServer > AdvancedWebServer
อ้างอิงโค้ด Web Server https://iotkiddie.com/blog/esp8266-webserver/
อ้างอิงโค้ด Serail STM->ESP https://iotkiddie.com/blog/serial-uart-esp-stm/
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

// ตั้งค่า softwareSerial ที่พิน RX,TX = D4,D3
SoftwareSerial stmSerial(D4, D3); // RX, TX

// ตั้งค่า wifi ที่ต้องการเชื่อมต่อ
#ifndef STASSID
#define STASSID "ชื่อไวไฟ"
#define STAPSK  "รหัสไวไฟ"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;


// ตัวแปรที่ต้องรับค่าจาก STM
int distance,light,led1,led2;

// สร้างเว็บเซอเวอร์ที่พอร์ต 80 พอร์ตมาตรฐานของหน้าเว็บ
ESP8266WebServer server(80);

// ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser
void handleRoot() {
  html();
}

// ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser กรณีไม่มี url ที่เรียกหา
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup(void) {

  Serial.begin(115200);

  // ตั้งค่า softwareSerial ที่ความเร็ว 9600
  stmSerial.begin(9600);

// เชื่อมต่อไวไฟ
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // แสดงค่า ip address

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

// ส่งค่า HTML หน้าเว็บกลับไปยัง Web Browser กรณีต่างๆ
  server.on("/", handleRoot);   // กรณีเรียกหน้าแรก
  server.onNotFound(handleNotFound);// กรณีเรียกหน้าอื่นๆ
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  // ถ้าได้รับค่ามาจาก STM ผ่าน serial uart
  while (stmSerial.available() > 0) {

    // รับค่าต่างๆที่ได้รับจากบอร์ด STM เก็บไว้ในตัวแปร
    distance = stmSerial.parseInt();
    light = stmSerial.parseInt();
    led1 = stmSerial.parseInt();
    led2 = stmSerial.parseInt();

// ถ้าจบบรรทัดแล้ว(ตัว \n คือจบบรรทัด) ให้แสดงค่าที่ได้รับผ่าน Serial monitor
    if (stmSerial.read() == '\n') {
      Serial.print("distance : ");
      Serial.print(distance);
      Serial.print(" light : ");
      Serial.print(light);
      Serial.print(" led1 : ");
      Serial.print(led1);
      Serial.print(" led2 : ");
      Serial.println(led2);
    }
  }
}

void html() {
  String payload;   // สร้างตัวแปรสตริงเก็บโค้ด html

// กำหนดค่า html และรวมกับค่าจากเซนเซอร์ โดยให้รีเฟรชอัตโนมัติทุก 10 วินาที  กำหนดเวลาที meta http-equiv='refresh' content='10'
  payload = "<html>\
  <head>\
    <meta http-equiv='refresh' content='10'/>\
    <meta charset='UTF-8'>\
    <title>ESP8266 Intruder alert control</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; text-align: center; }\
      .button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;}\
    </style>\
    <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.0/css/all.min.css'/>\
    <script src='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.0/js/all.min.js'></script>\
  </head>\
  <body>\
    <h1>Intruder alert</h1>\
    <p>distance : " + String(distance) + " cm</p>\
    <p>light : " + String(light) + "</p>";

    if(led1) {
      payload+="<p>led1 : ON</p>";
    }else{
      payload+="<p>led1 : OFF</p>";
    }

    if(led2) {
      payload+="<p>led2 : ON</p>";
    }else{
      payload+="<p>led2 : OFF</p>";
    }
    
  payload+="</body>\
</html>";

  server.send(200, "text/html", payload);
}
