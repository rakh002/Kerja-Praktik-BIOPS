// Watchdog Timer
Periodic wt_mqtt(15000);
Periodic wt_clock(1000);

unsigned long scanStartTime = 0;
unsigned long samplingStartTime = 0;
unsigned long interval = 1000;
