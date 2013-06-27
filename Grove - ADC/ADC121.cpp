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
    uint16_t data[1] = {REG_ADDR_RESULT};
    uint16_t address[1] = {ADDR_ADC121};
    uint16_t buf[2] = {0,0};

    /* open the i2c dev */
    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,ADDR_ADC121) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }

    write(file,data,1);
    // write(file, address, 1);

    sleep(1);
    // read from ADDR_ADC121
    read(file, buf, 2);
    close(file);

    // get data
    int x0 = buf[0] & 0x0f;
    int x1 = buf[1];

    x0 = x0 << 8;

    int x2 = x0+x1;

    float value = x2/4096.0*3.0*2.0;
    return value;

}

