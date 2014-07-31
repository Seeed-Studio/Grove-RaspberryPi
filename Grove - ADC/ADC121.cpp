/*
 * ADC121.cpp
 * Demo code for RP's ADC
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : seeed fellow
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


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <inttypes.h>

#include "ADC121.h"


ADC121::ADC121(uint16_t address){

}

ADC121::~ADC121(){

}

void ADC121::initADC(){

    int file;
    uint16_t data[1] = {0x20};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,ADDR_ADC121) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }

    write(file,data,1);
    close(file);
}

float ADC121::readADC(){

    int file;
    uint16_t initData[2] = {REG_ADDR_CONFIG, 0x20};
    //uint16_t iData[1] = {0x20};

    uint16_t data[1] = {REG_ADDR_RESULT};
    uint16_t buf[2] = {0,0};
    //int b[1] = {0};
    //printf("int b[0] = %d\n", &b[0]);
    //printf("init buf[0] = %d, buf[1] = %d\n", buf[0], buf[1]);

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    /* control the i2c device, and get the file handle */
    if(ioctl(file,I2C_SLAVE,ADDR_ADC121) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }

    write(file, initData, 2); /* init ADC */
    //write(file, iData, 1); [> init ADC <]
    write(file, data, 1); /* writing REG_ADDR_RESULT to get data */
    sleep(0.1); /* sleep unit is s */



    // read from ADDR_ADC121
    if(read(file, buf, 2)) {

        //printf("b[0] = %d\n", &b[0]);
        //printf("buf[0] = %d, buf[1] = %d\n", buf[0], buf[1]);

        // get data
        int x0 = buf[0] & 0x0f;
        int x1 = buf[1];

        x0 = x0 << 8;

        int x2 = x0+x1;
        //printf("x2: %d\n", x2);

        // calculating the data
        float value = x2/4096.0*3.0*2.0;
        close(file);
        return value;

    } else {
        printf("can't read anything");
        close(file);

        return -1;
    }


}

