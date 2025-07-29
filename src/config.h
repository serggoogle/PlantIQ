#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

constexpr int8_t SHT31_ADDR = 0x44; // Set to 0x45 for alternate i2c addr
constexpr size_t BATCH_SIZE = 5;
constexpr size_t NUM_SERIES = 4;
constexpr size_t BUFFER_SIZE = 1024;
constexpr uint16_t MAX_MOISTURE_LEVEL = 3800;
constexpr uint16_t MIN_MOISTURE_LEVEL = 0; // Minimum moisture level for the sensor
constexpr const char* PLANT_NAME = "test-plant-1";
constexpr const char* PLANT_SPECIES = "TestSpecies";
constexpr float DEFAULT_SENSOR_VALUE = 0.0f;
constexpr bool DEBUG = false;

#endif // CONFIG_H