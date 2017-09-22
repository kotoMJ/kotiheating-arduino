Task tempTask(800, TASK_FOREVER, &tempCb, &scheduler, false);

int16_t tempValue = TEMP_INVALID;

void tempPrepare() {
  ds.reset();
  ds.skip();
  ds.write(0x44);
}

int16_t tempRead() {
  ds.reset();
  ds.skip();
  ds.write(0xBE);

  uint8_t data[9];
  for (uint8_t i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  if (OneWire::crc8(data, 8) != data[8]) {
    return TEMP_INVALID;
  }

  int16_t raw = (data[1] << 8) | data[0];
  return (int)((float)raw / 1.60);
}

void tempCb() {
  tempValue = tempRead();
  tempPrepare();
}

