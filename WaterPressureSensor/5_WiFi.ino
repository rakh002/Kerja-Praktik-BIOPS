String scanner;
String list;

unsigned long scanStartMillis = 0;
Periodic apChecker(60000);

// Android dns requests
// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Initialize WiFi
bool isWiFiConnected = false;
bool scanningWiFi = false;

bool initWiFi()
{
  if (ssid == "")
  {
    Serial.println("Undefined SSID");
    return false;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long wifiStartTime = millis();

  while (millis() - wifiStartTime < 5000)
  { // Menunggu 5 detik
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected:");
      Serial.print("Time taken to connect WiFi: ");
      unsigned long wifiConnectionTime = millis() - wifiStartTime;
      Serial.print(wifiConnectionTime);
      Serial.println(" ms");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    delay(10);
  }

  Serial.println("Connection failed!");
  WiFi.disconnect();
  return false; // Kembalikan false jika tidak terhubung
}

void setupAP()
{
  Serial.println("Setting AP (Access Point)");
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void APLoop()
{
  if (apChecker.isTime())
  {
    // Jika sudah waktunya, cek apakah AP mode aktif
    if (WiFi.getMode() != WIFI_AP && WiFi.getMode() != WIFI_AP_STA)
    {
      Serial.println("AP mode terhenti, mengaktifkan kembali...");
      setupAP();
    }
  }
}

void wifiScanNetwork(int n)
{
  StaticJsonDocument<1000> wifi_list;
  JsonArray ssid = wifi_list.createNestedArray("ssid");
  JsonArray rssi = wifi_list.createNestedArray("rssi");
  JsonArray encrypt = wifi_list.createNestedArray("encrypt");
  for (int i = 0; i < n; ++i)
  {
    ssid.add(WiFi.SSID(i));
    rssi.add(WiFi.RSSI(i));
    if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
    {
      encrypt.add("o");
    }
    else
    {
      encrypt.add("l");
    }
    delay(10);
  }
  scanner = ""; // Kosongkan scanner sebelum serialisasi
  serializeJson(wifi_list, scanner);
  list = scanner; // Simpan hasil dalam variabel list
  Serial.println(list);
  WiFi.scanDelete();
}
