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
    explicit Plant(const char* name);
    Plant(const char* name, const char* species);
    const char* getName();
    const char* getSpecies();
    const char* getLabel();

private:
    std::string _name;
    std::string _species;
    std::string _label;
};

#endif
