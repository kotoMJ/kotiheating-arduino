Task displayTask(480, TASK_FOREVER, &displayCb, &scheduler, false);

void displayTemp(int16_t temp) {
  if (temp == TEMP_INVALID) {
    lcd.print("--.-\xdf ");
    return;
  }

  if (temp < 100) lcd.write(' ');
  lcd.print(temp / 10);
  lcd.write('.');
  lcd.print(temp % 10);
  lcd.write(0xdf);
  lcd.write(' ');
}

void displayCb() {
  lcd.setCursor(0, 0);
  displayTemp(tempValue);
  lcd.write(0x7e);
  lcd.write(' ');
  displayTemp(logicTargetTemp);
  lcd.write(logicHeatingOn ? '+' : '-');
  
  lcd.setCursor(0, 1);
  if (logicMode == LOGIC_MODE_OFF) {
    lcd.print("OFF    ");
  } else if (logicMode == LOGIC_MODE_MANUAL) {
    lcd.print("MANUAL ");
  } else if (logicMode == LOGIC_MODE_AUTO) {
    lcd.print("AUTO   ");
  } else {
    lcd.print("ERR! ");
  }

  uint32_t wallTime = ntpWallTime();
  if (wallTime == TIME_UNKNOWN) {
    lcd.print("--:-- --");
  } else {
    uint8_t dayOfWeek = ((wallTime / 86400) + 4) % 7;
    uint8_t hour = (wallTime % 86400) / (60 * 60);
    uint8_t minute = (wallTime % 3600) / 60;

    if (hour < 10) lcd.write('0');
    lcd.print(hour);
    lcd.write(':');
    if (minute < 10) lcd.write('0');
    lcd.print(minute);
    lcd.write(' ');

    switch (dayOfWeek) {
      case 0: lcd.print("SU"); break;
      case 1: lcd.print("MO"); break;
      case 2: lcd.print("TU"); break;
      case 3: lcd.print("WE"); break;
      case 4: lcd.print("TH"); break;
      case 5: lcd.print("FR"); break;
      case 6: lcd.print("SA"); break;
    }
  }
  lcd.write(WiFi.status() == WL_CONNECTED ? ' ' : 'X');
}

