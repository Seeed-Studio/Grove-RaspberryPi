#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>

#include "Barometer.h"

int main(){
    float temperature;
    float pressure;
    float atm;
    float altitude;

    Barometer mBarometer = Barometer();

    int i = 0;

    for(i; i< 10 ;i++) {

        temperature = mBarometer.bmp085GetTemperature(mBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
        pressure = mBarometer.bmp085GetPressure(mBarometer.bmp085ReadUP());//Get the temperature
        altitude = mBarometer.calcAltitude(pressure); //Uncompensated caculation - in Meters 
        atm = pressure / 101325;

        std::cout << "Temperature: " << temperature << " deg c" << std::endl;
        std::cout << "Pressure: " << pressure << " Pa" << std::endl;
        std::cout << "Ralated Atmosphere: " << atm << std::endl;
        std::cout << "Altitude: " << altitude << " m" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
