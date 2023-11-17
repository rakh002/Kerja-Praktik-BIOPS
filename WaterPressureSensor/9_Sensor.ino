int getValue()
{
  const int pressurePin = 34; // Ganti dengan pin analog yang sesuai
  const float voltageMin = 0.5; // Tegangan minimum (dalam volt)
  const float voltageMax = 4.5; // Tegangan maksimum (dalam volt)
  const float pressureMin = 0.0*10; // Tekanan minimum (dalam bar)
  const float pressureMax = 1.6*10; // Tekanan maksimum (dalam bar)

  int sensorValue = analogRead(pressurePin);
  
  // Mengkonversi nilai tegangan menjadi nilai tekanan
  float voltage = map(sensorValue, 0, 1023, voltageMin * 1000, voltageMax * 1000) / 1000.0;
  float pressure = map(voltage, voltageMin, voltageMax, pressureMin, pressureMax);
 
  return pressure;
}

void bacaData()
{
  previousValue = currentValue;
  currentValue = getValue();
}

void updateData()
{
  // Memperbarui data jarak maksimal dan minimal
  if (maxValue == 0)
  { // Data awal
    maxValue = currentValue;
  }
  else if (currentValue > maxValue)
  { // Data baru
    maxValue = currentValue;
  }
  if (minValue == 0)
  { // Data awal
    minValue = currentValue;
  }
  else if (currentValue < minValue)
  { // Data baru
    minValue = currentValue;
  }
}

void printData()
{
  char buff[80];
  sprintf(buff, "Current: %d  | prev: %d  | max: %d  | min: %d", currentValue, previousValue, maxValue, minValue);
  Serial.println(buff);
}
