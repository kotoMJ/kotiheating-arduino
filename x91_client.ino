
ESP8266WiFiMulti WiFiMulti;

Task clientTask(10000, TASK_FOREVER, &connectClient, &scheduler, false);


void connectClient(){

	if (WiFi.status() == WL_CONNECTED) {

	}
}
