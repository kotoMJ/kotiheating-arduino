
ESP8266WebServer server(80);

#define SERVER_AUTH_REQUIRED if (!serverAuthenticateUser()) return;

void serverSetup() {
  server.on("/", HTTP_GET, serverStatus);
  server.on("/timetable", HTTP_GET, serverTimetable);
  server.on("/timetable", HTTP_POST, serverSetTimetable);

  server.begin();
}

bool serverAuthenticateUser() {
  if (server.authenticate("admin", DEVICE_PASSWORD)) {
    return true;
  } else {
    //server.sendHeader("WWW-Authenticate", "Basic realm=\"Login Required\"");
    server.send(401, "text/json", "{\"error\":\"Login required\"}\n");
    return false;
  }
}

void serverStatus() {
  String response = "{\"uptime\":";
  response += millis() / 1000L;
  response += ",\"mac\":\"";
  response += WiFi.macAddress();
  response += "\",\"ip\":\"";
  response += WiFi.localIP().toString();
  response += "\",\"wallTime\":";
  response += ntpWallTime();
  response += ",\"mode\":";
  response += logicMode;
  response += ",\"manualTemp\":";
  response += logicManualTemp;
  response += ",\"heating\":";
  response += logicHeatingOn ? "true" : "false";
  response += ",\"targetTemp\":";
  response += logicTargetTemp;
  response += "}\n";

  server.send(200, "text/json", response);
}

void serverTimetable() {
  String response = "{\"timetable\":[";
  for (uint8_t day = 0; day < 7; day++) {
    response += '[';
    for (uint8_t hour = 0; hour < 24; hour++) {
      response += logicAutoTemp[day][hour];
      if (hour != 23) {
        response += ',';
      }
    }
    response += ']';
    if (day != 6) {
      response += ',';
    }
  }
  response += "]}\n";

  server.send(200, "text/json", response);
}

void serverSetTimetable() {
  uint8_t day = server.arg("day").toInt();
  uint8_t hour = server.arg("hour").toInt();
  int16_t temp = server.arg("temp").toInt();

  if (day >= 7 || hour >= 24 || temp == 0) {
    server.send(400, "text/json", "{\"error\":\"Invalid input\"}\n");
    return;
  }

  logicAutoTemp[day][hour] = temp;
  
  String response = "{\"hour\":";
  response += hour;
  response += ",\"day\":";
  response += day;
  response += ",\"temp\":";
  response += temp;
  response += "}\n";
  server.send(200, "text/json", response);
}


