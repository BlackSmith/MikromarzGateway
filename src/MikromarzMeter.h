#ifndef MikromarzMeter_h
#define MikromarzMeter_h
#include <Arduino.h>
#include <HardwareSerial.h>

#define MM_INVERTOR "YES"

// Define where debug output will be printed.
#ifndef DEBUG_PRINTER
    #define DEBUG_PRINTER Serial
#endif

#ifndef MM_INPUT_SERIAL
    #ifdef ESP8266
        #define MM_INPUT_SERIAL Serial
    #elif ESP32
        #define MM_INPUT_SERIAL Serial1
    #endif
#endif
#ifndef OUTPUT_SERIAL
    #ifdef ESP8266
        #define MM_OUTPUT_SERIAL Serial
    #elif ESP32
        #ifdef MM_INVERTOR
            #define MM_OUTPUT_SERIAL Serial1
        #else
            #define MM_OUTPUT_SERIAL Serial2
        #endif
    #endif
#endif

#ifdef ESP32
    #define SerialConfig uint32_t
#endif
#ifndef MM_SERIAL_BAUD
    #define MM_SERIAL_BAUD 127659
#endif
#ifndef MM_SERIAL_CONFIG
    #define MM_SERIAL_CONFIG SERIAL_8E1
#endif

#ifndef MM_SERIAL_RX_PIN        //Read channel - USB green wire
    #ifdef ESP8266
        #define MM_SERIAL_RX_PIN 3
    #elif ESP32
        #define MM_SERIAL_RX_PIN 16
    #endif
#endif

#ifndef MM_SERIAL_TX_PIN        //Transmit channel - USB white wire
    #ifdef ESP8266
        #define MM_SERIAL_TX_PIN 1
    #elif ESP32
        #define MM_SERIAL_TX_PIN 17
    #endif
#endif

enum meterType {
    SE1_PM2  // Eletricity - https://www.mikromarz.com/www-mikromarz-cz/eshop/51-1-Elektromery/179-2-3-fazove-elektromery/5/690-3-fazovy-2-tarifni-elektromer-SE1-PM2
};

enum tarif {
    TARIF_LOW,
    TARIF_HIGHT
};

class MikromarzMeter
{
public:
    MikromarzMeter(meterType type);
    void setup(uint8_t rxPin=MM_SERIAL_RX_PIN, uint8_t txPin=MM_SERIAL_TX_PIN,
               SerialConfig config=MM_SERIAL_CONFIG, unsigned long bound=MM_SERIAL_BAUD);
    bool readData();
    uint64_t getPower(byte phase);
    uint64_t getEnergy(byte phase, tarif t=TARIF_HIGHT);
    tarif getTarif();
private:
    void sendRequest();
    meterType type;
    uint64_t calculate_number(byte start, byte size);
    bool checkPhase(byte phase);
};
#endif