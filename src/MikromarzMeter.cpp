#include "MikromarzMeter.h"
#include <Arduino.h>
#include <HardwareSerial.h>

byte record[50];  //38 228 0   0 0 0 0 0 0 0 2 0 0 0 2 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0   156 4   # SE1-PM2

const byte sizeOfHeader = 2;

const int matrix[4] = {1, 256, 65536, 16777216}; // 256^index
const byte request[] = {0x0A, 0xE4, 0x00, 0xFF, 0xCE, 0x9F, 0x11, 0x4E, 0x40, 0xAE};

MikromarzMeter::MikromarzMeter(meterType type) {
    this->type = type;
}

void MikromarzMeter::setup(uint8_t rxPin, uint8_t txPin, SerialConfig config, unsigned long bound) {
    MM_INPUT_SERIAL.end();
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    #ifdef ESP8266
        MM_INPUT_SERIAL.begin(bound, config);
        MM_INPUT_SERIAL.pins(rxPin, txPin);
    #elif ESP32
        #ifndef MM_INVERTER
            MM_INPUT_SERIAL.begin(bound, config, rxPin, -1, true);
            MM_OUTPUT_SERIAL.end();
            MM_OUTPUT_SERIAL.begin(bound, config, -1, txPin);
        #else
            MM_INPUT_SERIAL.begin(bound, config, rxPin, txPin);
        #endif
    #endif
}

bool MikromarzMeter::readData() {
    sendRequest();
    if (MM_INPUT_SERIAL.available() > 0) {
        byte num = MM_INPUT_SERIAL.readBytes(record, sizeof(record));
        if (num == record[0]) {
            return true;
        }
    }
    return false;
}


uint64_t MikromarzMeter::getPower(byte phase) {
    if (checkPhase(phase)) {
        return calculate_number(sizeOfHeader + 2*(phase - 1) + 1, 2);    // 3,5,7
    }
    return 0;
}

uint64_t MikromarzMeter::getEnergy(byte phase, tarif t) {
    if (checkPhase(phase)) {
        byte offset = sizeOfHeader + (t == TARIF_HIGHT ? 8 : 20);
        return calculate_number(offset + 4*(phase - 1), 4);
    }
    return 0;
}

tarif MikromarzMeter::getTarif() {
    return record[34] == 0 ? TARIF_HIGHT : TARIF_LOW;
}

void MikromarzMeter::sendRequest() {
    MM_OUTPUT_SERIAL.write(request, sizeof(request));
    delay(50);
}

bool  MikromarzMeter::checkPhase(byte phase) {
    if (phase < 1 || phase > 3) {
        DEBUG_PRINTER.println(F("Phase is out of range (1-3)."));
        return false;
    }
    return true;
}

uint64_t MikromarzMeter::calculate_number(byte start, byte size) {
  uint64_t res = 0;
  for (int iLoop=size-1; iLoop>=0; iLoop--) {
    res = res + record[start+iLoop] * matrix[iLoop];
  }
  return res;
}