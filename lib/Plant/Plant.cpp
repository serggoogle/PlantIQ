#include "Arduino.h"
#include "Plant.h"

Plant::Plant(const char* name)
{
    _name = name;
}

Plant::Plant(const char* name, const char* type, const char* species)
{
    _name = name;
    _type = type;
    _species = species;
}

const char* Plant::getName()
{
    return _name.c_str();
}

const char* Plant::getSpecies() {
    return _species.c_str();
}

const char* Plant::getType() {
    return _type.c_str();
}