String dirLevel0;
String dirLevel1;
String tempFileName;
String fileName;
String dataMessage;
String header = "Timestamps,Current,Prev,Max,Min\r\n";

bool availDir = false;
// Initialize SD card
void initSDCard()
{
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void createDir(fs::FS &fs, const char *path)
{
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

void removeDir(fs::FS &fs, const char *path)
{
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path))
  {
    Serial.println("Dir removed");
  }
  else
  {
    Serial.println("rmdir failed");
  }
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}

void prepareFileName(RtcDateTime time)
{
  dirLevel0 = "/" + String(time.Year());
  dirLevel1 = dirLevel0 + "/" + getMonths(time.Month());
  fileName = dirLevel1 + "/" + String(time.Day()) + ".csv";
  tempFileName = "/temp.csv";
}

void prepareTimestamp(RtcDateTime time)
{
  char buff[30];
  sprintf(buff, "%02u/%02u/%04u||%02u:%02u:%02u", time.Day(), time.Month(), time.Year(), time.Hour(), time.Minute(), time.Second());
  timeStamp = String(buff);
  dataMessage = timeStamp + "," + String(currentValue) + "," + String(previousValue) + "," + String(maxValue) + "," + String(minValue) + "\r\n";
}

void writeToSDCard()
{
  createDir(SD, dirLevel0.c_str());
  createDir(SD, dirLevel1.c_str());
  writeFile(SD, fileName.c_str(), header.c_str());
  appendFile(SD, fileName.c_str(), dataMessage.c_str());
}

void writeToTemp()
{
  writeFile(SD, tempFileName.c_str(), dataMessage.c_str());
  appendFile(SD, tempFileName.c_str(), dataMessage.c_str());
}

void publishFromTemp(String path)
{
  unsigned long publishtempStartTime = millis();
  File temp = SD.open(path);
  Serial.println("open path: " + path);
  while (temp.available())
  {
    // Baca satu baris data dari file
    String dataLine = temp.readStringUntil('\n');
    // Serial.println(dataLine);
    // Publish data tiap baris ke MQTT topic tertentu
    mqttPublish(dataLine, 2);
  }
  Serial.println(path + " published");
  unsigned long publishtempTime = millis() - publishtempStartTime;
  Serial.print("Time to publish from /temp: ");
  Serial.print(publishtempTime);
  Serial.println(" ms");
  temp.close();
  deleteFile(SD, path.c_str());
}

void storeDataConnected(bool temp, bool file)
{
  if (!temp && !file)
  {
    Serial.println("temp 0 | file 0");
    writeToSDCard();
  }
  else if (temp && !file)
  {
    Serial.println("temp 1 | file 0");
    publishFromTemp(tempFileName);
    writeToSDCard();
  }
  else if (temp && file)
  {
    Serial.println("temp 1 | file 1");
    publishFromTemp(tempFileName);
    appendFile(SD, fileName.c_str(), dataMessage.c_str());
  }
  else if (!temp && file)
  {
    Serial.println("temp 0 | file 1");
    appendFile(SD, fileName.c_str(), dataMessage.c_str());
  }
}

void storeDataDisconnected(bool temp, bool file)
{
  if (!temp && !file)
  {
    writeToTemp();
    writeToSDCard();
  }
  else if (!temp && file)
  {
    writeToTemp();
    appendFile(SD, fileName.c_str(), dataMessage.c_str());
  }
  else if (temp && !file)
  {
    appendFile(SD, tempFileName.c_str(), dataMessage.c_str());
    writeToSDCard();
  }
  else if (temp && file)
  {
    appendFile(SD, tempFileName.c_str(), dataMessage.c_str());
    appendFile(SD, fileName.c_str(), dataMessage.c_str());
  }
}

// Initialize SPIFFS
void initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}
