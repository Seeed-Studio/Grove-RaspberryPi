/*
 * Barometer.cpp
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


#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>

#include "Barometer.h"

Barometer::Barometer(){
    init();
}

Barometer::~Barometer(){
}

void Barometer::init(void)
{
    ac1 = bmp085ReadInt(0xAA);
    ac2 = bmp085ReadInt(0xAC);
    ac3 = bmp085ReadInt(0xAE);
    ac4 = bmp085ReadInt(0xB0);
    ac5 = bmp085ReadInt(0xB2);
    ac6 = bmp085ReadInt(0xB4);
    b1 = bmp085ReadInt(0xB6);
    b2 = bmp085ReadInt(0xB8);
    mb = bmp085ReadInt(0xBA);
    mc = bmp085ReadInt(0xBC);
    md = bmp085ReadInt(0xBE);
}
// Read 1 unsigned charfrom the BMP085 at 'address'
// Return: the read byte;
char Barometer::bmp085Read(unsigned char address)
{
    int file;
    unsigned char buf[1] = {address};
    char reBuf[1] = {0};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,BMP085_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    write(file,buf,1);
    while(0 == reBuf[0]) {
        read(file,reBuf,1);
    }
    close(file);

    return reBuf[0];
}
// Read 2 bytes from the BMP085
// First unsigned charwill be from 'address'
// Second unsigned charwill be from 'address'+1
int16_t Barometer::bmp085ReadInt(unsigned char address)
{
    int file;
    unsigned char msb, lsb;
    unsigned char buf[1] = {address};
    unsigned char  reBuf[2] = {0,0};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,BMP085_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }

    write(file,buf,1);
    read(file,reBuf, 2);
    msb = reBuf[0];
    lsb = reBuf[1];
    close(file);

    return (int16_t) msb<<8 | lsb;
}
// Read the uncompensated temperature value
uint16_t Barometer::bmp085ReadUT()
{
    uint16_t ut;
    int file;
    unsigned char buf[2] = {0xF4,0x2E};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,BMP085_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }

    write(file,buf,2);
    close(file);
    sleep(1);

    ut = bmp085ReadInt(0xF6);
    return ut;
}
// Read the uncompensated pressure value
unsigned long Barometer::bmp085ReadUP()
{
    unsigned char msb, lsb, xlsb;
    unsigned long up = 0;
    int file;
    unsigned char a = 0x34 + (OSS<<6);
    unsigned char buf[2] = {0xF4,a};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,BMP085_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    write(file,buf,2);
    close(file);
    sleep(2 + (3<<OSS));

    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    msb = bmp085Read(0xF6);
    lsb = bmp085Read(0xF7);
    xlsb = bmp085Read(0xF8);
    up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
    return up;
}
void Barometer::writeRegister(int16_t deviceAddress, unsigned char address, unsigned char val)
{
    int file;
    unsigned char buf[2] = {address, val};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,deviceAddress) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    write(file,buf,2);
    close(file);
}
int16_t Barometer::readRegister(int16_t deviceAddress, unsigned char address)
{
    int file;
    unsigned char buf[1] = {address};
    int16_t reBuf[1] = {0};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,deviceAddress) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    write(file,buf,1);
    while(0 == reBuf[0]) {
        read(file,reBuf,1);
    }
    close(file);

    return reBuf[0];
}
float Barometer::calcAltitude(float pressure)
{
    float A = pressure/101325;
    float B = 1/5.25588;
    float C = pow(A,B);
    //printf("c = pow(a,b) = %d \n", &C);
    C = 1 - C;
    C = C /0.0000225577;
    return C;
}
float Barometer::bmp085GetTemperature(uint16_t ut)
{
    long x1, x2;

    x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
    x2 = ((long)mc << 11)/(x1 + md);
    PressureCompensate = x1 + x2;

    float temp = ((PressureCompensate + 8)>>4);
    temp = temp /10;

    return temp;
}
long Barometer::bmp085GetPressure(unsigned long up)
{
    long x1, x2, x3, b3, b6, p;
    unsigned long b4, b7;
    b6 = PressureCompensate - 4000;
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)(up - b3) * (50000>>OSS));
    if (b7 < 0x80000000)
        p = (b7<<1)/b4;
    else
        p = (b7/b4)<<1;

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;

    long temp = p;
    return temp;
}
