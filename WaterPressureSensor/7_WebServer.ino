
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void prepareClock(RtcDateTime time)
{
  char buff_tanggal[30];
  sprintf(buff_tanggal, "%02u/%02u/%04u", time.Day(), time.Month(), time.Year());
  char buff_jam[30];
  sprintf(buff_jam, "%02u:%02u:%02u", time.Hour(), time.Minute(), time.Second());
  hari = NamaHari(time.DayOfWeek());
  tanggal = String(buff_tanggal);
  jam = String(buff_jam);
}

// Replaces placeholder with Sensor values
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "CURRENT")
  {
    return String(currentValue);
  }
  else if (var == "PREVIOUS")
  {
    return String(previousValue);
  }
  else if (var == "MAXIMAL")
  {
    return String(maxValue);
  }
  else if (var == "MINIMAL")
  {
    return String(minValue);
  }
  else if (var == "WIFISCANNER")
  {
    wifiScan = true;
    return list;
  }
  else if (var == "HARI")
  {
    return hari;
  }
  else if (var == "TANGGAL")
  {
    return tanggal;
  }
  else if (var == "JAM")
  {
    return jam;
  }
  return String();
}

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    // request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/wifiManagerB.html", String(), false, processor);
  }
};

void webIndexPage()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false, processor); });

  // Route to load css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to load js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });
  server.on("/ajax.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/ajax.js", "text/javascript"); });
}

void webWiFiManagerPage()
{
  server.on("/wifiManager.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/wifiManager.html", String(), false, processor); });

  server.on("/wifi_style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/wifi_style.css", "text/css"); });

  server.on("/wifi_js.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/wifi_js.js", "text/javascript"); });

  server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.print("/scan...");
    wifiScan = true;
    Serial.print("wifiScan = ");
    Serial.println(wifiScan);
    request->send(200, "application/json", processor("WIFISCANNER")); });
  server.serveStatic("/", SPIFFS, "/");

  server.on("/setupWiFi", HTTP_POST, [](AsyncWebServerRequest *request)
            {
  int params = request->params();
  for (int i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    if (p->isPost()) {
      // HTTP POST ssid value
      if (p->name() == PARAM_INPUT_1) {
        ssid = p->value().c_str();
        Serial.print("SSID set to: ");
        Serial.println(ssid);
        // Write file to save value
        writeFile(SPIFFS, ssidPath, ssid.c_str()); //input SSID ke file pass.txt
      }
      // HTTP POST pass value
      if (p->name() == PARAM_INPUT_2) {
        pass = p->value().c_str();
        Serial.print("Password set to: ");
        Serial.println(pass);
        // Write file to save value
        writeFile(SPIFFS, passPath, pass.c_str()); //input password ke file pass.txt
      }
    }
  }
  String responseHTML = "<script>alert('ESP32 akan restart. Koneksikan device ke wifi dan buka /esp32.local pada browser'); window.location.href='/';</script>";
  request->send(200, "text/html", responseHTML);
  delay(3000);
  ESP.restart(); });
}

void webGetValue()
{
  server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("CURRENT")); });
  server.on("/prev", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("PREVIOUS")); });
  server.on("/max", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("MAXIMAL")); });
  server.on("/min", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("MINIMAL")); });
}

void webClock()
{
  server.on("/hari", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("HARI")); });
  server.on("/tanggal", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("TANGGAL")); });
  server.on("/jam", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", processor("JAM")); });
}

void webTimeSyncPage()
{
  server.on("/timeSync.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/timeSync.html", String(), false, processor); });

  server.on("/time_style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/time_style.css", "text/css"); });

  server.on("/setNTPTime", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam("timezone")) { // Terakhir 'true' berarti parameter dalam body
        String timezone = request->getParam("timezone")->value();
        int timeZoneOffset = timezone.toInt(); // Ubah ke integer (misal UTC+7 menjadi 7)

        // Set zona waktu
        setTimeZone(timeZoneOffset);

        // Sinkronisasi dengan NTP
        syncWithNTP();

        message = "RTC berhasil disinkronkan dengan NTP. Zona waktu telah diperbarui menjadi UTC" + timezone;
    } else {
        message = "Tidak ada zona waktu yang diterima";
    }

    String htmlResponse = "<script>window.location.href='/timeSync.html'; alert('" + message + "');</script>";
    request->send(200, "text/html", htmlResponse); });

  server.on("/setManualTime", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam("date") && request->hasParam("time")){
      String date = request->getParam("date")->value(); // format "yyyy-mm-dd"
      String time = request->getParam("time")->value(); // format "hh:mm"

      RtcDateTime manualTime = parseDateTime(date, time);
      Rtc.SetDateTime(manualTime);

      message = "Waktu berhasil diatur secara manual ke " + date + "||" + time;
    } else{
      message = "Waktu gagal diatur secara manual";
    }

    String htmlResponse = "<script>window.location.href='/timeSync.html'; alert('" + message + "');</script>";
    request->send(200, "text/html", htmlResponse); });
}

void handleServer()
{
  webIndexPage();

  // Get values of sensor
  webGetValue();
  // Get value of time and date
  webClock();

  webTimeSyncPage();
  webWiFiManagerPage();
}

void mdnsSetup()
{
  if (!MDNS.begin("esp32"))
  {
    Serial.println("Error setting up MDNS responder!");
  }
  else
  {
    Serial.println("mDNS responder started");
  }
}

void setupCaptivePortal()
{
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
}

void commonSetup()
{
  handleServer();

  Serial.println("Starting DNS Server");
  setupCaptivePortal();
  server.serveStatic("/", SPIFFS, "/");

  // Start OTA
  AsyncElegantOTA.begin(&server);

  // Start Server
  server.begin();
  delay(100);
}
