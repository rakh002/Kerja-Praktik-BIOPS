WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setTimeZone(int timeZoneOffset)
{
  timeClient.setTimeOffset(timeZoneOffset * 3600); // dikalikan 3600 untuk mengkonversi ke detik
}

void syncWithNTP()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    timeClient.update();
    time_t rawTime = timeClient.getEpochTime();

    // Menetapkan waktu untuk library Time
    setTime(rawTime);

    // Mendapatkan tanggal dan waktu yang diformat
    Serial.print(String(year()));
    Serial.print(String(month()));
    Serial.println(String(day()));

    String formattedDate = String(day()) + "-" + String(month()) + "-" + String(year());
    String formattedTime = String(hour()) + ":" + String(minute()) + ":" + String(second());

    Serial.println("Waktu dari NTP: " + formattedDate + " " + formattedTime);

    // Menetapkan waktu untuk RTC
    RtcDateTime ntpTime = RtcDateTime(year(), month(), day(), hour(), minute(), second());
    Rtc.SetDateTime(ntpTime);
    Serial.println("RTC berhasil disinkronkan dengan NTP");
  }
}
