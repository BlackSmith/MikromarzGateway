#include <Arduino.h>
#include <HardwareSerial.h>
#include <MikromarzMeter.h>

#ifdef ESP8266
  #define PRINTER Serial1
#elif ESP32
  #define PRINTER Serial
#endif

MikromarzMeter mm = MikromarzMeter();

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
    for (byte i=1; i<=NUMBER_PHASES; i++) {
      PRINTER.printf("Power %d: %ld W\n", i, (long)mm.getPower(i));
    }
    PRINTER.println();
    for (byte i=1; i<=NUMBER_PHASES; i++) {
      PRINTER.printf("Energy %d (high tarif): %ld W/h\n", i,
                     (long)mm.getEnergy(i, TARIF_HIGHT));
      #ifdef START_ENERGY_LOW_TARIF
        PRINTER.printf("Energy %d (low tarif): %ld W/h\n", i,
                      (long)mm.getEnergy(i, TARIF_LOW));
      #endif
      PRINTER.println();
    }
    #ifdef START_ENERGY_LOW_TARIF
      PRINTER.printf("Tarif: %s",
        (mm.getTarif() == TARIF_HIGHT ? "high" : "low"));
      PRINTER.println();
    #endif
  }
  PRINTER.println("------------------------");
  delay(500);
}
