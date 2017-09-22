Task btnTask(20, TASK_FOREVER, &btnCb, &scheduler, false);

uint8_t btnCounter1 = 0;
uint8_t btnCounter2 = 0;

void btnEvent1();
void btnEvent2();

void btnSetup() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_BUTTON2, INPUT_PULLUP);
}

bool btnRead1() {
  return digitalRead(PIN_BUTTON1) == LOW;
}

bool btnRead2() {
  return digitalRead(PIN_BUTTON2) == LOW;
}

void btnCb() {
  if (btnRead1()) {
    if (btnCounter1 >= 3) {
      if (btnCounter1 != 255) {
        btnEvent1();
        btnCounter1 = 255;
      }
    } else {
      btnCounter1++;
    }
  } else {
    btnCounter1 = 0;
  }

  if (btnRead2()) {
    if (btnCounter2 >= 3) {
      if (btnCounter2 != 255) {
        btnEvent2();
        btnCounter2 = 255;
      }
    } else {
      btnCounter2++;
    }
  } else {
    btnCounter2 = 0;
  }
}

