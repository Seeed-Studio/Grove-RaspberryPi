#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <inttypes.h>

#include "ADC121.h"

int main () {

    ADC121 mADC121 = ADC121(0x55);


    // mADC121.initADC();


    int i = 0;
    for(i; i < 20; i++){

        std::cout << "ADC Volue: " << mADC121.readADC() << std::endl;
    }

    //while(1) {
        //std::cout << "ADC Volue: " << mADC121.readADC() << std::endl;
    //}


    return 0;
}
