void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  String text = String((char *) &payload[0]);
  char * textC = (char *) &payload[0];
  String rssi;
  String temp;
  int nr;
  int on;
  uint32_t rmask;
  int i;
  char b[10];   //declaring character array
  String str;  //declaring string

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        delay(5);
        webSocket.sendTXT(num, "C");
      }
      break;
    case WStype_TEXT:

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");

      switch(payload[0]) {
        case 'w': case 'W':  // Request RSSI wx
          rssi = String(WiFi.RSSI());
          // Serial.printf("[%u] Got message: %s\n", num, payload);
          delay(5);
          webSocket.sendTXT(0, rssi);
          break;

        case 'p': // ping, will reply pong
          Serial.printf("[%u] Got message: %s\n", num, payload);
          delay(5);
          webSocket.sendTXT(0, "pong");
          break;

        case 'e': case 'E':   //Echo
          delay(5);
          webSocket.sendTXT(0, text);
          break;

        default:
          delay(5);
          webSocket.sendTXT(0, "**** UNDEFINED ****");
          Serial.printf("[%u] Got UNDEFINED message: %s\n", num, payload);
          break;
      }
      break;

    case WStype_BIN:
      Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
      hexdump(payload, lenght);

      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;
  }
}
