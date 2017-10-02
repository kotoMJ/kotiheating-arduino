
void relaySetup() {
  pinMode(PIN_RELAY, INPUT);
  digitalWrite(PIN_RELAY, LOW);
}

void relaySet(bool on) {
  if (on) {
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, LOW);
  } else {
    pinMode(PIN_RELAY, INPUT);
  }
}

