/*
 * main.cpp
 * A library for barometer at RP
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : LG
 * Create Time:
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
