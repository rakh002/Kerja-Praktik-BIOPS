void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Wire.begin();
  scanI2CDevices();
  setupLED();
  setupTime();
  initSDCard();
  initSPIFFS();

  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);

  Serial.println(ssid);
  Serial.println(pass);

  if (initWiFi())
  {
    isWiFiConnected = true;
    setupAP();
    mdnsSetup();
    commonSetup();
  }
  else
  {
    isWiFiConnected = false;
    setupAP();
    commonSetup();
  }

  wifiScan = true;
  mqttSetup();
}

void loop()
{
  RtcDateTime now = Rtc.GetDateTime();

  int wifiStatus = WiFi.status();

  if (wifiStatus == WL_CONNECTED)
  {
    isWiFiConnected = true;
  }
  else
  {
    isWiFiConnected = false;
    dnsServer.processNextRequest();
  }

  if (wifiScan && !scanningWiFi)
  {
    Serial.println("Scan start");
    WiFi.scanNetworks(true, true); // Mulai scan asinkron
    delay(100);
    scanningWiFi = true;
    wifiScan = false;
    scanStartTime = millis();
  }

  APLoop();

  if (wt_clock.isTime())
  {
    prepareClock(now);
  }

  if (millis() - samplingStartTime >= interval)
  {
    Serial.print("Waktu sampling = ");
    unsigned long samplingTime = millis() - samplingStartTime;
    Serial.print(samplingTime);
    Serial.println(" ms");
    samplingStartTime = millis();
    bacaData();
    checkData = true;
  }

  if (checkData)
  {
    updateData();
    printData();
    checkData = false;

    prepareFileName(now);

    prepareTimestamp(now);

    File file = SD.open(fileName);
    File temp = SD.open(tempFileName);

    if (isWiFiConnected)
    {
      Serial.println("wifi connected");
      Serial.println(WiFi.localIP());
      digitalWrite(LED1, LOW);
      mqttLoop();
      if (isMQTTConnected)
      {
        digitalWrite(LED2, LOW);
        mqttPublish(dataMessage, 2);
        storeDataConnected(temp, file);
      }
      else
      {
        digitalWrite(LED2, HIGH);
        storeDataDisconnected(temp, file);
      }
    }
    else
    {
      Serial.println("wifi disconnect");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      storeDataDisconnected(temp, file);
    }
  }

  
  if (scanningWiFi && WiFi.scanComplete() >= 0)
  {
    int n = WiFi.scanComplete(); // Mendapatkan jumlah network

    unsigned long scanTime = millis() - scanStartTime;
    Serial.print("Time to scan network: ");
    Serial.print(scanTime);
    Serial.println(" ms");

    Serial.print("nilai n: ");
    Serial.println(n);

    Serial.println("Scan done");

    wifiScanNetwork(n);   // Memanggil fungsi untuk proses hasil scan
    scanningWiFi = false; // Reset flag scanning
  }
}
