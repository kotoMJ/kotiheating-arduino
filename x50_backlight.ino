Task backlightTask(30000L, 1, &backlightCb, &scheduler, false);

bool backlightOn = true;

void backlightCb() {
  backlightOn = false;
  lcd.noBacklight();
}

bool backlightRefresh() {
  bool result = backlightOn == false;
  backlightOn = true;
  
  backlightTask.restartDelayed(0);
  lcd.backlight();
  return result;
}

