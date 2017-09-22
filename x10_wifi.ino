
void wifiSetup() {
  lcd.setCursor(0,0);
  lcd.print("Connecting");

  if (WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect()) WiFi.setAutoConnect(false);
  WiFi.persistent(false); // avoid rewriting ssid/pass in flash
  WiFi.begin(CFG_SSID, CFG_PASS);

  int counter = 0;
  lcd.setCursor(0,1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(700);
    lcd.write('.');
    counter++;
    if (counter > 15) {
      lcd.clear();
      lcd.print("WiFi failed!");
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    lcd.clear();
    lcd.print("IP: ");
    lcd.setCursor(0,1);
    lcd.print(WiFi.localIP());
  }
  delay(2000);
  lcd.clear();
}


