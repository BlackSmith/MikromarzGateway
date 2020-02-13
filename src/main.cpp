#include <Arduino.h>
#include <HardwareSerial.h>
#include <MikromarzMeter.h>

#ifdef ESP8266
  #define PRINTER Serial1
#elif ESP32
  #define PRINTER Serial
#endif

MikromarzMeter mm = MikromarzMeter(SE1_PM2);

void setup() {
  mm.setup();
  #ifdef ESP8266
    PRINTER.begin(9600, SERIAL_8N1, SERIAL_FULL, 1);
  #elif ESP32
    PRINTER.begin(9600);
  #endif
  //PRINTER.setDebugOutput(true);
  PRINTER.printf("Configured.");
}

void loop() {
  if (mm.readData()) {
    for (byte i=1; i<4; i++) {
      PRINTER.printf("Power %d: %ld W\n", i, (long)mm.getPower(i));
    }
    PRINTER.println();
    for (byte i=1; i<4; i++) {
      PRINTER.printf("Energy %d (high tarif): %ld kW/h\n", i,
                     (long)mm.getEnergy(i, TARIF_HIGHT));
      PRINTER.printf("Energy %d (low tarif): %ld kW/h\n", i,
                     (long)mm.getEnergy(i, TARIF_LOW));
      PRINTER.println();
    }
    PRINTER.printf("Tarif: %s",
      (mm.getTarif() == TARIF_HIGHT ? "high" : "low"));
    PRINTER.println();
  }
  PRINTER.println("------------------------");
  delay(500);
}
