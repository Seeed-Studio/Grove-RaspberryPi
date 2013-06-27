
#include <inttypes.h>

#ifndef __ADC121_H__
#define __ADC121_H__

#define ADDR_ADC121             0x55
#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

class ADC121 {

    public:
        ADC121(uint16_t address);
        virtual ~ADC121();

        void initADC();
        float readADC();
};

#endif //__ADC121_H__
