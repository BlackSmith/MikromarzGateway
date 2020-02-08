#include <Arduino.h>
#include <HardwareSerial.h>
#include <MikromarzMeter.h>

MikromarzMeter mm = MikromarzMeter(SE1_PM2);

void setup() {  
  mm.setup();
  Serial1.begin(9600, SERIAL_8N1, SERIAL_FULL, 1);
  Serial1.setDebugOutput(true);
  Serial1.printf("Configured.");
}

void loop() {
  if (mm.readData()) {
    for (byte i=1; i<4; i++) {
      Serial1.printf("Power %d: ", i);
      Serial1.print((long)mm.getPower(i));
      Serial1.println(" W");
    }
    Serial1.println();
    for (byte i=1; i<4; i++) {
      Serial1.printf("Energy %d (high tarif): ", i);
      Serial1.print((long)mm.getEnergy(i, TARIF_HIGHT));
      Serial1.println(" Wh");
      Serial1.printf("Energy %d (low tarif): ", i);
      Serial1.print((long)mm.getEnergy(i, TARIF_LOW));
      Serial1.println(" Wh");
      Serial1.println();
    }
  }
  
  Serial1.printf("Tarif: %s", (mm.getTarif() == TARIF_HIGHT ? "high" : "low"));
  Serial1.println();
  Serial1.println("------------------------");
  delay(5300);
}
