void btnEvent1() {
  if (backlightRefresh()) return;

  logicModeSwitch();
  displayCb();
}

void btnEvent2() {
  if (backlightRefresh()) return;
  if (logicMode == LOGIC_MODE_MANUAL) {
    logicChangeManualTemp();
    displayCb();
  }
}

void setup() {
  Serial.begin(115200);

  btnSetup();
  relaySetup();
  tempPrepare();
  logicSetHeating(false);

  lcd.begin();
  lcd.backlight();

  wifiSetup();

  otaSetup();
  ntpSetup();
  serverSetup();
  taskNtpSync.enable();
  backlightTask.enableDelayed(0);
  tempTask.enableDelayed(0);
  btnTask.enable();
  logicTask.enable();
  displayTask.enable();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  scheduler.execute();
}



