#include "Arduino.h"
#include "Plant.h"

Plant::Plant(const char* name)
{
    _name = name;
}

Plant::Plant(const char* name, const char* species)
{
    _name = name;
    _species = species;
    _label = "{job=\"" + _name + "\",species=\"" + _species + "\"}";
}

const char* Plant::getName()
{
    return _name.c_str();
}

const char* Plant::getSpecies() {
    return _species.c_str();
}

const char* Plant::getLabel(){
    return _label.c_str();
}