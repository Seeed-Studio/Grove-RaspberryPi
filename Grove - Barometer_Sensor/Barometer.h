/*
 * Barometer.h
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
