Task taskNtpSync(5000L, TASK_FOREVER, &ntpSyncStartCb, &scheduler);
Task taskNtpTick(1000L, TASK_FOREVER, &ntpTickCb, &scheduler, true);

WiFiUDP ntpUdp;
IPAddress ntpServerIP;

#define TIME_UNKNOWN 0xffffffffUL
uint32_t currentTime;
uint32_t lastTimeSync;
int32_t tzOffset;

#define NTP_PACKET_SIZE 48
byte ntpBuffer[NTP_PACKET_SIZE];


void ntpSetup() {
  currentTime = TIME_UNKNOWN;
  lastTimeSync = 0;
  tzOffset = 3600*2;
  ntpUdp.begin(2390);
}

uint32_t ntpWallTime() {
  if (currentTime == TIME_UNKNOWN) return TIME_UNKNOWN;
  
  return currentTime + tzOffset;
}

void ntpTickCb() {
  if (currentTime == TIME_UNKNOWN) return;

  currentTime++;
}

void ntpSyncStartCb() {
  if (!WiFi.hostByName("tak.cesnet.cz", ntpServerIP)) {
    return;
  }

  // set all bytes in the buffer to 0
  memset(ntpBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  ntpBuffer[0] = 0b11100011;   // LI, Version, Mode
  ntpBuffer[1] = 0;     // Stratum, or type of clock
  ntpBuffer[2] = 6;     // Polling Interval
  ntpBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntpBuffer[12]  = 49;
  ntpBuffer[13]  = 0x4E;
  ntpBuffer[14]  = 49;
  ntpBuffer[15]  = 52;

  ntpUdp.beginPacket(ntpServerIP, 123);
  ntpUdp.write(ntpBuffer, NTP_PACKET_SIZE);
  ntpUdp.endPacket();

  taskNtpSync.setCallback(&ntpSyncFinishCb);
  taskNtpSync.setInterval(1000L);
  taskNtpSync.enableDelayed(0); // clears run counter
}

void ntpSyncFinishCb() {
  int success = ntpUdp.parsePacket() >= NTP_PACKET_SIZE;
  if (success) {
    ntpUdp.read(ntpBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(ntpBuffer[40], ntpBuffer[41]);
    unsigned long lowWord = word(ntpBuffer[42], ntpBuffer[43]);
    // Unix time starts on Jan 1 1970. NTP starts in 1900, difference in seconds is 2208988800:
    currentTime = ((highWord << 16) | lowWord) - 2208988800UL;
    lastTimeSync = millis();
  }

  // wait for ntp response five seconds and if this fails, wait one minute and try sync again
  if (success || taskNtpSync.getRunCounter() >= 5) {
    taskNtpSync.setCallback(&ntpSyncStartCb);
    taskNtpSync.setInterval(600000L);
    taskNtpSync.enableDelayed(0); // clears run counter
  }
}


