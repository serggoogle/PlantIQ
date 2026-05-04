#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

constexpr int8_t SHT31_ADDR = 0x44; // Set to 0x45 for alternate i2c addr
constexpr const char* NTP_SERVER = "pool.ntp.org";
const char* MOISTURE_SENSOR_NAME = "moisture";
const char* TEMPERATURE_F_SENSOR_NAME = "temperature_f";
const char* TEMPERATURE_C_SENSOR_NAME = "temperature_c";
const char* HUMIDITY_SENSOR_NAME = "humidity";
const char* DEVICE_ID = "plant_iq_device_001";
constexpr const char* PLANT_NAME = "testPlant";
constexpr const char* PLANT_TYPE = "testType";
constexpr const char* PLANT_SPECIES = "testSpecies";
constexpr float DEFAULT_SENSOR_VALUE = 0.0f;
constexpr bool DEBUG = false;
constexpr size_t PUBLISH_INTERVAL_MS = 15000;
#endif // CONFIG_H