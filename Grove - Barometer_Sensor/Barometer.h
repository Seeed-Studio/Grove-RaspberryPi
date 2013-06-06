/*
   Barometer library V1.0
   2010 Copyright (c) Seeed Technology Inc.  All right reserved.

   Original Author: LG

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
   */
#ifndef __BAROMETER_H__
#define __BAROMETER_H__

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>

const unsigned char OSS = 0;

#define BMP085_ADDRESS 0x77
class Barometer
{
    public: 
        Barometer();
        virtual ~Barometer();
        void init(void);
        long PressureCompensate;
        float bmp085GetTemperature(uint16_t ut);
        long bmp085GetPressure(unsigned long up);
        float calcAltitude(float pressure);
        uint16_t bmp085ReadUT(void);
        unsigned long bmp085ReadUP(void);

    private:
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
        char bmp085Read(unsigned char address);
        int16_t bmp085ReadInt(unsigned char address);
        void writeRegister(int16_t deviceAddress, unsigned char address, unsigned char val);
        int16_t readRegister(int16_t deviceAddress, unsigned char address);
};

#endif
