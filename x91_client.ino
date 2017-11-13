
//#include <WiFiClientSecure.h>

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial


//10 000ms = 10s
//10 000ms * 60 = 600 000 = 10min
Task clientTask(10000, TASK_FOREVER, &connectClient, &scheduler, false);

//const char* host = "kotopeky.cz";
//const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "08 2F 51 75 3D 8C 50 A7 CA D1 6D 0E E9 9F DB 9A AD CA E3 DD";

void connectClient(){
	if (WiFi.status() == WL_CONNECTED) {
		updateTimetableFromServer();
    //delay(1000);
		//sendStatusToServer();
	}
}

void updateTimetableFromServer() {
		HTTPClient http;
    //http.begin("http://192.168.1.56:8080/api/kotinode/heating/schedule/raw");
    http.begin("https://kotopeky.cz/api/kotinode/heating/schedule/raw","08 2F 51 75 3D 8C 50 A7 CA D1 6D 0E E9 9F DB 9A AD CA E3 DD");
    http.addHeader("key", DEVICE_PASSWORD); 
    USE_SERIAL.print("[HTTP] GET https://kotopeky.cz/api/kotinode/heating/schedule/raw \n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            char paychars[672];//672
            payload.toCharArray(paychars, payload.length());
            char separator[] = " ";
            char *token;

            USE_SERIAL.println(payload.length());
            USE_SERIAL.println(payload);

            uint8_t incomming_day = 0;
            uint8_t incomming_hour = 0;

            token = strtok(paychars, separator);
            USE_SERIAL.print("firsttoken:"); USE_SERIAL.println(token); 
            while (token != NULL){ // delimiter is the space
              USE_SERIAL.print("day:"); 
              USE_SERIAL.print(incomming_day);
              USE_SERIAL.print("hour:");
              USE_SERIAL.print(incomming_hour);
              USE_SERIAL.print("string:");
              USE_SERIAL.println(token);
              logicAutoTemp[incomming_day][incomming_hour++] = (int16_t)atoi(token); //update schedule
              token = strtok(NULL, separator);
              //incomming_hour++;
              if (incomming_hour>23) {
                incomming_day++;
                incomming_hour = 0;
              }
            }
        }
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

void sendStatusToServer() {
      HTTPClient http;
      //http.begin("http://192.168.1.56:8080/api/kotinode/heating/status");
      http.begin("https://kotopeky.cz/api/kotinode/heating/status","08 2F 51 75 3D 8C 50 A7 CA D1 6D 0E E9 9F DB 9A AD CA E3 DD");
      http.addHeader("Content-Type", "application/json");
      http.addHeader("key", DEVICE_PASSWORD); 
        String payload = "{";
        payload += "\"temperature\":12,";
        payload +=  "\"deviceDateTime\": \"" + deviceDateTime() + "\",";
        payload += "\"timetable\":[";
        for (uint8_t day = 0; day < 7; day++) {
          payload += '[';
          for (uint8_t hour = 0; hour < 24; hour++) {
            payload += logicAutoTemp[day][hour];
            if (hour != 23) {
              payload += ',';
            }
          }
          payload += ']';
          if (day != 6) {
            payload += ',';
          }
        }
        payload += "]}\n";

      USE_SERIAL.println(payload);
      //int httpCode = http.POST("{\"temperature\":\"11\"}");
      int httpCode = http.POST(payload);
      if(httpCode > 0) {
      USE_SERIAL.printf("sendStatusToServer [HTTP] POST... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("sendStatusToServer [HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
            String payload = http.getString();
            USE_SERIAL.println(payload);
        }
      http.end();
}

String deviceDateTime(){
uint32_t wallTime = ntpWallTime();

  String ret = "";
  if (wallTime == TIME_UNKNOWN) {
    ret+="UNKNOWN";
  } else {
    uint8_t dayOfWeek = ((wallTime / 86400) + 4) % 7;
    uint8_t hour = (wallTime % 86400) / (60 * 60);
    char buff[20];
    String hourStr = itoa(hour,buff,10);
    uint8_t minute = (wallTime % 3600) / 60;
    String minuteStr = itoa(minute,buff,10);

    ret+="[hour:"+hourStr+"]";
    ret+="[minute:"+minuteStr+"]";

    switch (dayOfWeek) {
      case 0: ret+="[day:SU]"; break;
      case 1: ret+="[day:MO]"; break;
      case 2: ret+="[day:TU]"; break;
      case 3: ret+="[day:WE]"; break;
      case 4: ret+="[day:TH]"; break;
      case 5: ret+="[day:FR]"; break;
      case 6: ret+="[day:SU]"; break;
    }
    
    
  }
  return ret;
}


