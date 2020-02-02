#include <Arduino.h>
#include <MikromarzMeter.h>

MikromarzMeter mm = MikromarzMeter(SE1_PM2);

void setup() {  
  Serial.begin(9600);
  mm.setup();
}

void loop() {  
  if (mm.readData()) {
    for (byte i=1; i<4; i++) {
      Serial.printf("Power %d: ", i);
      Serial.print((long)mm.getPower(i));
      Serial.println(" kW");
    }
    Serial.println();
    for (byte i=1; i<4; i++) {
      Serial.printf("Energy %d (high tarif): ", i);
      Serial.print((long)mm.getEnergy(i, TARIF_HIGHT));
      Serial.println(" kWh");
      Serial.printf("Energy %d (low tarif): ", i);
      Serial.print((long)mm.getEnergy(i, TARIF_LOW));
      Serial.println(" kWh");
      Serial.println();
    }
  }
  
  Serial.printf("Tarif: %s\n", (mm.getTarif() == TARIF_HIGHT ? "high" : "low"));
  Serial.println("------------------------");
  delay(1000);
}
