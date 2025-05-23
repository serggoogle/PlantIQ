/*
    Plant.h -
    Created by Sergio Olvera, May 23, 2025
*/
#ifndef Plant_h
#define Plant_h

// #include <Arduino.h>
// Class definition
class Plant {
public:
    explicit Plant(const String &name);
    Plant(const String &name, const String &species);
    String getName();
    String getSpecies();

private:
    String _name;
    String _species;
};

#endif
