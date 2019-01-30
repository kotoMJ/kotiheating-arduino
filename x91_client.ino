
//#include <WiFiClientSecure.h>

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial


//10 000ms = 10s
//10 000ms * 60 = 600 000 = 10min
//10 000ms * 60 * 5 = 300 000 = 5min
Task clientTask(60000, TASK_FOREVER, &connectClient, &scheduler, false);

void connectClient(){
	if (WiFi.status() == WL_CONNECTED) {
    String sha1 = getSHA1();
		updateTimetableFromServer(sha1);
		updateModeFromServer(sha1)
    delay(3000);
		sendStatusToServer(sha1);
	}
}

String getSHA1(){
  String sha1;
  HTTPClient http;
  http.begin("http://peaceful-shelf-58932.herokuapp.com/fingerprint/sha1");
  int httpCode = http.GET();
  //String ret = "68 47 F6 2E 5D 89 DC E6 5E 0B 81 EB DE 85 A0 82 CB 05 11 7A";//default
  if(httpCode > 0) {
    sha1 =  http.getString();
    USE_SERIAL.println("[HTTP] GET SHA1 from peaceful-shelf-58932.herokuapp.com success!... sha1:" + sha1); 
  }else {
    USE_SERIAL.printf("[HTTP] GET SHA1 from peaceful-shelf-58932.herokuapp.com FAILURE!... code: %d\n", httpCode); 
  }
  http.end();
  return sha1;
}

void updateTimetableFromServer(String sha1Fingerprint) {
		HTTPClient http;
    http.begin("https://kotopeky.cz/api/kotinode/heating/schedule/raw/"+ String(DEVICE_ID),sha1Fingerprint);
    http.addHeader("key", DEVICE_PASSWORD); 
    USE_SERIAL.print("[HTTP] GET https://kotopeky.cz/api/kotinode/heating/schedule/raw/"+String(DEVICE_ID)+"\n");
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
              USE_SERIAL.print("set day:"); 
              USE_SERIAL.print(incomming_day);
              USE_SERIAL.print("set hour:");
              USE_SERIAL.print(incomming_hour);
              USE_SERIAL.print("set value:");
              USE_SERIAL.println(token);
              logicAutoTemp[incomming_day][incomming_hour] = (int16_t)atoi(token); //update schedule
              token = strtok(NULL, separator);
              incomming_hour++;
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

void updateModeFromServer(String sha1Fingerprint) {
    String forceModeString;
    HTTPClient http;
    http.begin("https://kotopeky.cz/api/kotinode/heating/mode/raw/"+String(DEVICE_ID),sha1Fingerprint);
    http.addHeader("key", DEVICE_PASSWORD); 
    USE_SERIAL.print("[HTTP] GET https://kotopeky.cz/api/kotinode/heating/mode/raw/"+String(DEVICE_ID)+"\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode == HTTP_CODE_OK) {
            forceModeString =  http.getString();
            USE_SERIAL.println("[HTTP] GET forceMode success!... forceMode:" + forceModeString); 
           logicModeForce(forceModeString);
        }
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

void sendStatusToServer(String sha1Fingerprint) {
      HTTPClient http;
      http.begin("https://kotopeky.cz/api/kotinode/heating/status/"+String(DEVICE_ID),sha1Fingerprint);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("key", DEVICE_PASSWORD); 
        String payload = "{";
        payload +=  "\"heatingId\": \"" + String(DEVICE_ID) + "\",";
        payload +=  "\"heatingName\": \"KotoOffice\",";
        payload += "\"temperature\": \"" + String(tempValue) + "\",";
        payload +=  "\"heatingDateTime\": \"" + deviceDateTime() + "\",";
        payload +=  "\"heatingMode\": \"" + String(logicMode) + "\",";
        payload += "\"timetableDevice\":[";
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
		  case 6: ret+="[day:SA]"; break;
		}


	  }
	  return ret;
}


