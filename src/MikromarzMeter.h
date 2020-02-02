#ifndef MikromarzMeter_h
#define MikromarzMeter_h
#include <Arduino.h>
#include <HardwareSerial.h>

#define WSDR_DEBUG
// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef WSDR_DEBUG
#define DEBUG_PRINT(...)                  \
    {                                     \
        DEBUG_PRINTER.print(__VA_ARGS__); \
    }
#define DEBUG_PRINTLN(...)                  \
    {                                       \
        DEBUG_PRINTER.println(__VA_ARGS__); \
    }
#define DEBUG_PRINTF(...)                  \
    {                                      \
        DEBUG_PRINTER.printf(__VA_ARGS__); \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#define DEBUG_PRINTF(...) \
    {                     \
    }
#endif

enum meterType {
    SE1_PM2  // Eletricity - https://www.mikromarz.com/www-mikromarz-cz/eshop/51-1-Elektromery/179-2-3-fazove-elektromery/5/690-3-fazovy-2-tarifni-elektromer-SE1-PM2
};

enum tarif {
    TARIF_LOW,
    TARIF_HIGHT
};

const unsigned long MM_SERIAL_BAUD = 127659;
const uint32_t MM_SERIAL_CONFIG = SERIAL_8E1;
const uint8_t MM_SERIAL_RX_PIN = 16;     //Read channel - USB green wire
const uint8_t MM_SERIAL_TX_PIN = 17;     //Transmit channel - USB white wire



class MikromarzMeter
{
public:
    MikromarzMeter(meterType type);
    void setup(uint8_t rxPin=MM_SERIAL_RX_PIN, uint8_t txPin=MM_SERIAL_TX_PIN, 
               uint32_t config=MM_SERIAL_CONFIG, unsigned long bound=MM_SERIAL_BAUD);
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