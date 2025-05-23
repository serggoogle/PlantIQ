#include "Arduino.h"
#include "Plant.h"

Plant::Plant(const String &name)
{
    _name = name;
}

Plant::Plant(const String &name, const String &species)
{
    _name = name;
    _species = species;
}

String Plant::getName()
{
    return _name;
}

String Plant::getSpecies() {
    return _species;
}