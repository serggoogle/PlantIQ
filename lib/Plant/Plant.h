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
    Plant(const char* name, const char* type, const char* species);
    const char* getName();
    const char* getType();
    const char* getSpecies();
private:
    std::string _name;
    std::string _species;
    std::string _type = "plant";

};

#endif
