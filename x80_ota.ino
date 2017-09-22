
void otaUpdatePassword() {
#if OTA_REQUIRES_PASSWORD
#endif
}

void otaSetup() {
  MDNS.setInstanceName("SmartHeating");
  ArduinoOTA.setHostname("SmartHeating");
  ArduinoOTA.setPassword(DEVICE_PASSWORD);

  ArduinoOTA.onStart([]() {
    relaySet(false);
  });
  ArduinoOTA.begin();
}

