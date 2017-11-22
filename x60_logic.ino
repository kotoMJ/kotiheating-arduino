Task logicTask(1000, TASK_FOREVER, &logicCb, &scheduler, false);

#define LOGIC_MODE_OFF 0
#define LOGIC_MODE_MANUAL 1
#define LOGIC_MODE_AUTO 2
#define LOGIC_MODE_MAX 3

#define LOGIC_FREEZE_TEMP 50

#define LOGIC_MANUAL_MIN 180
#define LOGIC_MANUAL_MAX 280


uint8_t logicMode = LOGIC_MODE_AUTO;
int16_t logicManualTemp = 220;
int16_t logicHysteresis = 3;
bool logicHeatingOn = false;
int16_t logicTargetTemp = TEMP_INVALID;

int16_t logicAutoTemp[7][24] = {
  //0    1     2    3   4   5    6     7    8   9     10  11   12   13   14    15   16   17   18  19   20   21   22   23
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}, // NE
  {150, 150, 150, 150, 150, 150, 150, 150, 200, 200, 200, 200, 200, 220, 220, 220, 220, 220, 220, 220, 150, 150, 150, 150}, // PO
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}, // UT
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 150, 150}, // ST
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}, // CT
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}, // PA
  {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}, // SO
};

void logicModeSwitch() {
  logicMode++;
  if (logicMode >= LOGIC_MODE_MAX) {
    logicMode = 0;
  }
  logicCb();
}

void logicChangeManualTemp() {
  logicManualTemp += 10;
  if (logicManualTemp > LOGIC_MANUAL_MAX) logicManualTemp = LOGIC_MANUAL_MIN;
  logicCb();
}

void logicSetHeating(bool on) {
  logicHeatingOn = on;
  relaySet(on);
}

void logicCb() {
  if (logicMode == LOGIC_MODE_OFF) {
    logicEvaluateTemp(LOGIC_FREEZE_TEMP);
  } else if (logicMode == LOGIC_MODE_MANUAL) {
    logicEvaluateTemp(logicManualTemp);
  } else if (logicMode == LOGIC_MODE_AUTO) {
    uint32_t wallTime = ntpWallTime();
    if (wallTime == TIME_UNKNOWN) {
      logicEvaluateTemp(logicManualTemp);
    } else {
      logicEvaluateTemp(logicAutoTempForTime(wallTime));
    }
  }
}

int16_t logicAutoTempForTime(uint32_t wallTime) {
  uint8_t dayOfWeek = ((wallTime / 86400) + 4) % 7;
  uint8_t hour = (wallTime % 86400) / (60 * 60);
  return logicAutoTemp[dayOfWeek][hour];
}

void logicEvaluateTemp(int16_t targetTemp) {
  if (tempValue == TEMP_INVALID) {
    logicSetHeating(false);
    return;
  }
  logicTargetTemp = targetTemp;

  if (tempValue < targetTemp - logicHysteresis && !logicHeatingOn) {
    logicSetHeating(true);
  }
  if (tempValue > targetTemp + logicHysteresis && logicHeatingOn) {
    logicSetHeating(false);
  }
}

