#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>
#include "Digital_Light_TSL2561.h"

using namespace std;

int main ()
{
    uint16_t a,b,c;
    a = 0;
    b = 0;
    c = 1;
    
    uint16_t address = 0x29; /* i2c device address */
    Digital_Light_TSL2561 light = Digital_Light_TSL2561(address);

    cout << "Seeed Studio Digital Light Sensor" << endl;
    while(1) {
        light.getLux();
        cout << "The light value is: " << light.calculateLux(a,b,c) << endl; 
    }

    return 0;
}
