#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

SoftwareSerial stmSerial(D4, D3); // RX, TX

#ifndef STASSID
#define STASSID "G6PD_2.4G"
#define STAPSK  "570610193"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

int distance,light,led1,led2;

ESP8266WebServer server(80);

void handleRoot() {
  html();
}

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
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);

  Serial.begin(115200);
  stmSerial.begin(9600);

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
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  // if there's any serial available, read it:
  while (stmSerial.available() > 0) {

    distance = stmSerial.parseInt();
    light = stmSerial.parseInt();
    led1 = stmSerial.parseInt();
    led2 = stmSerial.parseInt();

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
  String payload;

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
