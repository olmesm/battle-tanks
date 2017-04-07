/*
 * ############################
 * ####### BATTLE TANKS #######
 * ############################
 *
 * Control your hovering tank via Wifi.
 * Destroy each other with Lasers.
 *
 * Leggo!
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFiMulti.h>
#include <FS.h>

ESP8266WiFiMulti WiFiMulti;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

ESP8266WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char DomainName[] = "battle";
char* tankApName = "tank-x";

void setup() {
  Serial.begin(115200);
  Serial.print("\n");
  
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(tankApName);

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

// start DNS server for a specific domain name
  dnsServer.start(DNS_PORT, DomainName, apIP);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  webServer.onNotFound([](){
    if(!handleFileRead(webServer.uri()))
      webServer.send(404, "text/plain", "FileNotFound");
  });
  
  webServer.begin();

  // Needed this to stabilize Websocket connection
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
}

void loop() {
  webSocket.loop();
  dnsServer.processNextRequest();
  webServer.handleClient();
}
